#include "CodeEmitter.h"

CodeEmitter::TypeEmitter::TypeEmitter(CodeEmitter* emitter) : _emitter(emitter) {

}

template<typename T>
void CodeEmitter::TypeEmitter::operator()(T const& t) {
    _emitter->EmitType(t);
}

CodeEmitter::CodeEmitter(bool test) {
    if (test) {
        _decls.open("IsaacRepentance.h");
        _impl.open("IsaacRepentance.cpp");
    }
    else {
        _decls.open("../include/IsaacRepentance.h");
        _impl.open("../libzhl/IsaacRepentance.cpp");
    }

}

void CodeEmitter::ProcessZHLFiles(fs::path const& base) {
	for (fs::directory_entry const& entry : fs::directory_iterator(base)) {
		//std::cout << "File " << entry.path() << std::endl;
		if (!entry.is_regular_file())
			continue;

		//std::cout << "Ext " << entry.path().extension() << std::endl;
		if (entry.path().extension() != std::string(".zhl"))
			continue;

		ProcessFile(entry.path());
	}
}

void CodeEmitter::ProcessFile(fs::path const& file) {
    std::ifstream stream(file);

    if (!stream.is_open()) {
        return;
    }

    // std::cout << "Visiting " << path << std::endl;
    antlr4::ANTLRInputStream input_stream(stream);
    ZHLLexer lexer(&input_stream);
    antlr4::CommonTokenStream tokens(&lexer);
    ZHLParser parser(&tokens);

    antlr4::tree::ParseTree* tree = parser.zhl();

    if (lexer.getNumberOfSyntaxErrors() != 0 || parser.getNumberOfSyntaxErrors() != 0) {
        std::cerr << "Error while parsing " << file << std::endl;
        throw std::runtime_error("Parse error");
    }
    Parser p2(&_global, &_types, file.string());
    p2.visit(tree);
}

void CodeEmitter::Emit() {
    CheckDependencies();
    CheckVTables();

    EmitDecl();
    EmitGlobalPrologue();
    EmitImplPrologue();

    EmitForwardDecl();

    for (GenericCode const& code : _global._generic) {
        Emit(code);
        EmitNL();
    }

    for (VariableSignature const& sig : _global._references) {
        Emit(sig);
    }

    for (auto const& [_, type] : _types) {
        if (type.IsStruct() && !type._pending) {
            Emit(type.GetStruct());
        }
    }

    for (Signature const& sig : _global._signatures) {
        Emit(sig, false);
    }

    BuildExternalNamespaces();
    for (auto const [name, _] : _externals) {
        EmitNamespace(name);
    }
}

void CodeEmitter::EmitForwardDecl() {
    for (auto const& [_, type] : _types) {
        if (type.IsStruct() && !type._pending) {
            Emit("struct ");
            Emit(type.GetStruct()._name);
            Emit(";");
            EmitNL();
        }
    }
}

void CodeEmitter::Emit(Type const& type) {
    if (type._synonym) {
        Emit(*type._synonym);
        return;
    }

    if (type._base) {
        Emit(*type._base);
        if (type._const) {
            Emit(" const");
        }
        else if (type.IsPointer()) {
            Emit(type._pointerDecl.front());
        }
    }
    else {
        std::visit(TypeEmitter(this), type._value);
    }
}

void CodeEmitter::Emit(Struct const& s) {
    if (InProcessing(s) || Emitted(s)) {
        return;
    }

    _processingStructures.insert(s._name);

    EmitDependencies(s);

    _currentStructure = &s;

    EmitDecl();
    EmitTab();
    Emit("struct ");
    Emit(s._name);
    if (!s._parents.empty()) {
        Emit(" : ");
        for (size_t i = 0; i < s._parents.size(); ++i) {
            Emit(VisibilityToString(std::get<Visibility>(s._parents[i])));
            Emit(" ");
            Emit(std::get<Type*>(s._parents[i])->GetStruct()._name);
            if (i != s._parents.size() - 1) {
                Emit(", ");
            }
        }
    }
    Emit(" {");
    EmitNL();
    IncrDepth();

    for (Variable const& var : s._namespace._fields) {
        Emit(var);
    }

    for (Signature const& sig : s._namespace._signatures) {
        Emit(sig, false);
    }

    for (Signature const& sig : s._overridenVirtualFunctions) {
        Emit(sig, true);
    }

    for (std::variant<Signature, Skip, Function> const& sig : s._virtualFunctions) {
        Emit(sig);
    }

    for (GenericCode const& code : s._namespace._generic) {
        Emit(code);
        EmitNL();
    }

    DecrDepth();
    Emit("};");
    EmitNL();
    EmitNL();

    _processingStructures.erase(s._name);
    _emittedStructures.insert(s._name);

    _currentStructure = nullptr;
}

void CodeEmitter::Dump() {
    std::set<std::string> pending;
    for (auto const& [name, type] : _types) {
        std::cout << name << " => " << type.ToString(true) << std::endl;

        if (!type.IsResolved()) {
            if (type.IsEmpty()) {
                std::cout << "EMPTY TYPE" << std::endl;
            }
            else {
                pending.insert(std::get<std::string>(type._value));
            }
        }
    }

    std::cout << "UNRESOLVED TYPES:" << std::endl;
    for (std::string const& s : pending) {
        std::cout << s << std::endl;
    }
}

void CodeEmitter::EmitType(BasicType const& t) {
    if (t._sign) {
        Emit(SignednessToString(*t._sign));
        Emit(" ");
    }

    if (t._length) {
        Emit(LengthToString(*t._length));
        Emit(" ");
    }

    Emit(BasicTypeToString(t._type));
}

void CodeEmitter::EmitType(Struct const& s) {
    Emit(s._name);

    if (s._template) {
        Emit("<");
        for (size_t i = 0; i < s._templateParams.size(); ++i) {
            Emit(*s._templateParams[i]);
            if (i != s._templateParams.size() - 1) {
                Emit(", ");
            }
        }
        Emit(">");
    }
}

void CodeEmitter::EmitType(FunctionPtr* ptr) {
    if (_variableContext) {
        Emit(*ptr->_ret);
        Emit(" (");

        if (ptr->_convention) {
            Emit("");
        }

        if (ptr->_scope) {
            Emit(*ptr->_scope);
            Emit("::");
        }

        Emit("*");
        Emit(_variableContext->_name);
        Emit(")(");

        for (size_t i = 0; i < ptr->_parameters.size(); ++i) {
            Emit(*(ptr->_parameters[i]));
            if (i != ptr->_parameters.size()) {
                Emit(", ");
            }
        }

        Emit(")");
    }
}

void CodeEmitter::EmitType(std::string const& t) {
    Emit(t);
}

void CodeEmitter::EmitType(EmptyType const&) {
    throw std::runtime_error("[FATAL] Attempting to emit empty type");
}

void CodeEmitter::EmitTab() {
    for (size_t i = 0; i < _emitDepth; ++i) {
        *_emitContext << "\t";
    }
}

void CodeEmitter::Emit(std::string const& s) {
    *_emitContext << s;
}

void CodeEmitter::EmitNL() {
    *_emitContext << std::endl;
}

void CodeEmitter::EmitDecl() {
    _emitContext = &_decls;
}

void CodeEmitter::EmitImpl() {
    _emitContext = &_impl;
}

void CodeEmitter::IncrDepth() {
    ++_emitDepth;
}

void CodeEmitter::DecrDepth() {
    --_emitDepth;
}

void CodeEmitter::EmitDependencies(Struct const& s) {
    for (Type* dep : s._deps) {
        if (dep->IsStruct()) {
            Emit(std::get<Struct>(dep->_value));
        }
        else {
            std::ostringstream str;
            str << "[ERROR] Structure " << s._name << " specifies unresolved dependency " << dep->_name << std::endl;
            throw std::runtime_error(str.str());
        }
    }

    for (auto const& [dep, vis] : s._parents) {
        if (dep->IsStruct()) {
            Emit(dep->GetStruct());
        }
        else {
            std::ostringstream str;
            str << "[ERROR] Structure " << s._name << " specified unresolved parent " << dep->_name << std::endl;
            throw std::runtime_error(str.str());
        }
    }
}

bool CodeEmitter::Emitted(Struct const& s) const {
    return _emittedStructures.find(s._name) != _emittedStructures.end();
}

void CodeEmitter::AssertEmitted(Struct const& s) {
    if (!Emitted(s)) {
        std::ostringstream str;
        str << "[FATAL] Structure " << _currentStructure->_name << " has not specified direct dependency on structure " << s._name << std::endl;
        throw std::runtime_error(str.str());
    }
}

bool CodeEmitter::InProcessing(Struct const& s) const {
    return _processingStructures.find(s._name) != _processingStructures.end();
}

void CodeEmitter::AssertReady(Type const* t) {
    if (t->_pending && t->_name != "std") {
        std::ostringstream str;
        str << "[FATAL] Structure " << std::get<Struct>(t->_value)._name << " has not been properly defined" << std::endl;
        throw std::runtime_error(str.str());
    }
}

void CodeEmitter::CheckVTables() {
    for (auto const& [_, type] : _types) {
        if (!type.IsStruct()) {
            continue;
        }

        Struct const& s = type.GetStruct();
        CheckVTableInternalConsistency(s);
        
        std::set<std::string> seen;
        std::vector<Struct const*> parents;
        std::stack<Struct const*> parentsBuffer;

        for (auto const& [parent, _] : s._parents) {
            Struct const& parentStruct = parent->GetStruct();
            if (seen.find(parentStruct._name) != seen.end()) {
                continue;
            }

            parentsBuffer.push(&parentStruct);
            seen.insert(parentStruct._name);
            // CheckVTableHierarchyConsistency(s, parent->GetStruct());
        }

        while (!parentsBuffer.empty()) {
            Struct const* parent = parentsBuffer.top();
            parentsBuffer.pop();

            for (auto const& [parentParent, _] : parent->_parents) {
                Struct const& parentStruct = parentParent->GetStruct();
                if (seen.find(parentStruct._name) != seen.end()) {
                    continue;
                }

                parentsBuffer.push(&parentStruct);
            }

            parents.push_back(parent);
        }

        CheckVTableHierarchyConsistency(s, parents);
    }
}

void CodeEmitter::CheckVTableInternalConsistency(Struct const& s) {
    // Check that each function declared as override does not conflict with 
    // a function not declared as override.

    for (Signature const& sig : s._overridenVirtualFunctions) {
        for (std::variant<Signature, Skip, Function> const& fn : s._virtualFunctions) {
            Function const* function = GetFunction(fn);

            if (function) {
                if (function->_name == sig._function._name) {
                    if (*function == sig._function) {
                        std::ostringstream str;
                        str << "[FATAL] In structure " << s._name << ", virtual function " << sig._function._name << " overrides itself" << std::endl;
                        throw std::runtime_error(str.str());
                    }
                }
            }
        }
    }
}

void CodeEmitter::CheckVTableHierarchyConsistency(Struct const& s, std::vector<Struct const*> const& parents) {
    // Check that each function declared as override has an actual non overidden
    // version somewhere.

    for (Signature const& sig : s._overridenVirtualFunctions) {
        bool found = false;
        for (Struct const* parent : parents) {
            for (std::variant<Signature, Skip, Function> const& fn : parent->_virtualFunctions) {
                Function const* function = GetFunction(fn);
                if (function) {
                    if (function->_name == sig._function._name) {
                        if (*function == sig._function) {
                            found = true;
                            break;
                        }
                    }
                }
            }

            if (found) {
                break;
            }
        }

        if (!found) {
            std::ostringstream str;
            str << "Structure " << s._name << " specified function " << sig._function._name << " as an override, but it doesn't override anything" << std::endl;
            throw std::runtime_error(str.str());
        }
    }

    // Check that each function declared as not override is not actually an override.
    for (std::variant<Signature, Skip, Function> const& fun : s._virtualFunctions) {
        if (std::holds_alternative<Skip>(fun)) {
            continue;
        }

        Function const* source = GetFunction(fun);
        std::optional<std::string> badOverride;

        for (Struct const* parent : parents) {
            for (std::variant<Signature, Skip, Function> const& other : parent->_virtualFunctions) {
                Function const* function = GetFunction(other);
                if (function) {
                    if (*source == *function) {
                        badOverride = parent->_name;
                        break;
                    }
                }
            }

            if (badOverride) {
                std::ostringstream str;
                str << "Structure " << s._name << " specified function " << source->_name <<
                    " as a non override, but it overrides its parent in class " << *badOverride << std::endl;
                throw std::runtime_error(str.str());
            }
        }
    }
}

void CodeEmitter::CheckDependencies() {
    for (auto const& [_, type] : _types) {
        if (!type.IsStruct()) {
            continue;
        }

        CheckDependencies(type);
    }
}

void CodeEmitter::CheckDependencies(Type const& t) {
    Struct const& s = t.GetStruct();

    // Assert dependencies have a definition
    for (Type* t: s._deps) {
        AssertReady(t);
    }

    // Assert parents have a definition
    for (auto const& [type, _] : s._parents) {
        AssertReady(type);
    }

    // Check fields
    for (Variable const& v : s._namespace._fields) {
        if (v._type->IsStruct() && !v._type->IsPointer()) {
            AssertReady(v._type);

            Struct const& var = v._type->GetStruct();

            bool found = false;
            for (Type* t : s._deps) {
                Struct const& dep = t->GetStruct();
                if (dep._name == var._name) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                std::stack<Type*> parents;
                for (auto const& [type, _] : s._parents) {
                    parents.push(type);
                }

                while (!parents.empty()) {
                    Type* t = parents.top();
                    parents.pop();

                    if (t->GetStruct()._name == var._name) {
                        found = true;
                        break;
                    }

                    for (auto const& [type, _] : t->GetStruct()._parents) {
                        parents.push(type);
                    }
                }
            }

            if (!found) {
                _logger << ErrorLogger::warn << " Structure " << s._name << "'s field " << v._name << " has non pointer structure type " << v._type->GetStruct()._name << 
                    " which is not specified in the dependency list, this may lead to compile errors" << ErrorLogger::endl << ErrorLogger::_end;
            }
        }
    }
}

void CodeEmitter::Emit(Variable const& var) {
    _variableContext = &var;

    EmitTab();
    Emit(*var._type);
    Emit(" ");

    if (!std::holds_alternative<FunctionPtr*>(var._type->_value)) {
        Emit(var._name);
    }

    if (var._type->IsArray()) {
        Emit("[");
        Emit(std::to_string(var._type->_arraySize));
        Emit("]");
    }

    Emit(";");
    EmitNL();

    _variableContext = nullptr;
}

void CodeEmitter::Emit(Signature const& var, bool isVirtual) {
    EmitTab();
    Function const& fun = var._function;
    Emit("LIBZHL_API ");
    /* if (isVirtual) {
        Emit("virtual ");
    } */
    EmitFunction(fun);
    EmitNL();

    EmitAssembly(var, isVirtual, false);

    // Emit function twice, one with the vtable, one without.
    // This allows parent calls, otherwise only virtual calls would be possible 
    // as the parent call would enter a virtual call.
    if (isVirtual) {
        Signature copy(var);
        std::ostringstream name;
        name << "Original_" << var._function._name;
        copy._function._name = name.str();
        Emit(copy, false);
    }
}

void CodeEmitter::EmitFunction(Function const& fun) {
    uint32_t qualifiers = fun._qualifiers;
    if (qualifiers & STATIC) {
        Emit("static ");
    }

    Emit(*fun._ret);
    Emit(" ");

    if (fun._convention) {
        if (*fun._convention == THISCALL && _currentStructure) {
            // Nop
        }
        else {
            Emit(CallingConventionToString(*fun._convention));
            Emit(" ");
        }
    }
    else {
        if (fun._kind == FUNCTION && !fun.IsCleanup()) {
            Emit("__stdcall ");
        }
    }

    Emit(fun._name);
    Emit("(");
    for (size_t i = 0; i < fun._params.size(); ++i) {
        Emit(*(fun._params[i]._type));
        Emit(" ");
        Emit(fun._params[i]._name);
        if (i != fun._params.size() - 1) {
            Emit(", ");
        }
    }
    Emit(");");
}

void CodeEmitter::Emit(std::variant<Signature, Skip, Function> const& sig) {
    if (std::holds_alternative<Skip>(sig)) {
        Emit("virtual void unk");
        Emit(std::to_string(_virtualFnUnk));
        Emit("() { }");
        ++_virtualFnUnk;
    }
    else if (std::holds_alternative<Signature>(sig)) {
        Emit(std::get<Signature>(sig), true);
    }
    else {
        Function const& fn = std::get<Function>(sig);
        EmitFunction(fn);
        EmitNL();
        EmitAssembly(fn, true, false);

        Function copy = fn;
        copy._name.append("_Original");
        EmitFunction(copy);
        EmitAssembly(copy, true, true);
    }
}

void CodeEmitter::Emit(PointerDecl const& ptr) {
    switch (ptr._kind) {
    case LREF:
        Emit(" &");
        break;

    case RREF:
        Emit(" &&");
        break;

    case POINTER:
        Emit(" *");
        break;
    }

    if (ptr._const) {
        Emit(" const");
    }
}

enum FunctionDefFlags {
    DEF_THISCALL = 1 << 0,
    DEF_CLEANUP = 1 << 1,
    DEF_VOID = 1 << 2,
    DEF_LONGLONG = 1 << 3
};

void CodeEmitter::EmitAssembly(std::variant<Signature, Function> const& sig, bool isVirtual, bool isPure) {
    uint32_t depth = _emitDepth;
    _emitDepth = 0;
    EmitImpl();

    Function const* fnPtr;
    if (std::holds_alternative<Signature>(sig)) {
        fnPtr = &std::get<Signature>(sig)._function;
    }
    else {
        fnPtr = &std::get<Function>(sig);
    }

    Function const& fn = *fnPtr;
    bool emittingPureVirtual = std::holds_alternative<Function>(sig);

    if (!isPure) {
        // Start namespace
        Emit("namespace _fun");
        Emit(std::to_string(_nEmittedFuns));
        Emit(" {");
        EmitNL();
        IncrDepth();

        // Function pointer
        EmitTab();
        Emit("static void* func = 0;");
        EmitNL();
        EmitTab();
    }

    // Parameters
    uint32_t flags = GetFlags(fn);
    bool hasImplicitOutput;
    uint32_t stackSize, fnStackSize;
    if (!isPure) {
        std::tie(hasImplicitOutput, stackSize, fnStackSize) = EmitArgData(fn);
    }

    // Function definition object
    if (!isPure) {
        if (!emittingPureVirtual) {
            EmitTab();
            Emit("static FunctionDefinition fn(\"");
            if (_currentStructure) {
                Emit(_currentStructure->_name);
                Emit("::");
            }
            Emit(fn._name);
            Emit("\", ");
            EmitTypeID(fn);
            Emit(", \"");
            Emit(std::get<Signature>(sig)._sig);
            Emit("\", args, ");
            Emit(std::to_string(fn._params.size() + (hasImplicitOutput ? 1 : 0)));

            Emit(", ");
            Emit(std::to_string(flags));
            Emit(", &func);");
            EmitNL();

        }

        // End namespace
        DecrDepth();
        Emit("}");
        EmitNL();
        EmitNL();
    }

    // Function implementation
    Emit("__declspec(naked) ");
    Emit(*fn._ret);
    Emit(" ");
    if (fn._convention) {
        if (*fn._convention != THISCALL) {
            Emit(CallingConventionToString(*fn._convention));
            Emit(" ");
        }
    }
    else {
        if (fn._kind == FUNCTION && !fn.IsCleanup()) {
            Emit("__stdcall ");
        }
    }

    if (_currentStructure) {
        Emit(_currentStructure->_name);
        Emit("::");
    }

    Emit(fn._name);
    Emit("(");
    for (size_t i = 0; i < fn._params.size(); ++i) {
        FunctionParam const& param = fn._params[i];
        Emit(*param._type);
        Emit(" ");
        Emit(param._name);

        if (i != fn._params.size() - 1) {
            Emit(", ");
        }
    }
    Emit(") {");
    EmitNL();

    IncrDepth();
    EmitInstruction("__asm {");
    IncrDepth();

    if (!isPure) {
        // Prologue
        EmitInstruction("push ebp");
        EmitInstruction("mov ebp, esp");
        // Save parameters
        if (flags & FunctionDefFlags::DEF_VOID) {
            EmitInstruction("push eax");
        }

        if (flags & FunctionDefFlags::DEF_VOID || !(flags & FunctionDefFlags::DEF_LONGLONG)) {
            EmitInstruction("push edx");
        }

        EmitInstruction("push ebx");
        EmitInstruction("push ecx");
        EmitInstruction("push esi");
        EmitInstruction("push edi");

        // Call function
        // Iterate through arguments in reverse order
        size_t k = stackSize + 8;
        for (int i = fn._params.size() - 1; i >= 0; --i) {
            FunctionParam const& param = fn._params[i];
            if (param._reg) {
                Registers reg = *param._reg;
                std::string regStr = RegisterToString(reg);
                std::ostringstream ins;
                if (reg < XMM0) {
                    ins << "mov ";
                }
                else {
                    ins << "movd ";
                }

                k -= 4;
                ins << regStr << ", [ebp + " << k << "] // " << param._name;
                EmitInstruction(ins.str());

            }
            else {
                int count = (int)std::ceil(param._type->size() / 4.f);
                for (int j = 0; j < count; ++j) {
                    k -= 4;
                    std::ostringstream ins;
                    ins << "push [ebp + " << k << "] // " << param._name;
                    EmitInstruction(ins.str());
                }
            }
        }

        if (hasImplicitOutput) {
            k -= 4;
            std::ostringstream ins;
            ins << "push [ebp + " << k << "] // implicit_output";
            EmitInstruction(ins.str());
        }

        if (k != 8) {
            std::cerr << "[WARN] Expected to have 8 bytes remaining on stack, but " << k << " bytes remain" << std::endl;
        }

        if (isVirtual) {
            std::ostringstream mov, call;
            mov << "mov eax, [ecx]";
            EmitInstruction(mov.str());

            uint32_t position;
            if (std::holds_alternative<Signature>(sig)) {
                position = _currentStructure->GetVirtualFunctionSlot(std::get<Signature>(sig), true);
            }
            else {
                position = _currentStructure->GetVirtualFunctionSlot(std::get<Function>(sig));
            }
            call << "call [eax + " << position * 4 << "]";
            EmitInstruction(call.str());
        }
        else {
            std::ostringstream callIns;
            callIns << "call _fun" << _nEmittedFuns << "::func";
            EmitInstruction(callIns.str());
        }

        if (flags & FunctionDefFlags::DEF_CLEANUP && fnStackSize > 0) {
            std::ostringstream retIns;
            retIns << "add esp, " << fnStackSize;
            EmitInstruction(retIns.str());
        }

        // Epilogue
        EmitInstruction("pop edi");
        EmitInstruction("pop esi");
        EmitInstruction("pop ecx");
        EmitInstruction("pop ebx");

        if (flags & FunctionDefFlags::DEF_VOID || !(flags & FunctionDefFlags::DEF_LONGLONG)) {
            EmitInstruction("pop edx");
        }

        if (flags & FunctionDefFlags::DEF_VOID) {
            EmitInstruction("pop eax");
        }

        EmitInstruction("mov esp, ebp");
        EmitInstruction("pop ebp");

        if (!fn.IsCleanup()) {
            EmitInstruction("ret " + std::to_string(stackSize));
        }
        else {
            EmitInstruction("ret");
        }
    }
    else {
        // Call the function regardless, so the pure call handler can react
        std::ostringstream mov, call;
        mov << "mov eax, 0";
        EmitInstruction(mov.str());

        // uint32_t position = _currentStructure->GetVirtualFunctionSlot(std::get<Function>(sig));
        // call << "call [eax + " << position * 4 << "]";
        call << "call eax";
        EmitInstruction(call.str());
    }

    DecrDepth();
    EmitTab();
    Emit("}");
    EmitNL();
    DecrDepth();
    Emit("}");
    EmitNL();
    EmitNL();

    EmitDecl();
    _emitDepth = depth;
    ++_nEmittedFuns;
}

void CodeEmitter::EmitInstruction(std::string const& ins) {
    EmitTab();
    Emit(ins);
    EmitNL();
}

void CodeEmitter::Emit(VariableSignature const& sig) {
    std::string refName("__ptr_" + sig._variable._name);

    Emit("extern LIBZHL_API ");
    Emit(*sig._variable._type);
    Emit("* ");
    Emit(refName);;
    Emit(";");
    EmitNL();
    Emit("#define ");
    Emit(sig._variable._name);
    Emit(" (*");
    Emit(refName);
    Emit(")");
    EmitNL();

    EmitAssembly(sig);
}

void CodeEmitter::EmitAssembly(VariableSignature const& sig) {
    std::string refName("__ptr_" + sig._variable._name);

    EmitImpl();

    Emit("namespace _var");
    Emit(std::to_string(_nEmittedVars));
    Emit(" {");
    EmitNL();
    IncrDepth();
    EmitTab();
    Emit("static VariableDefinition varObj(\"");
    Emit(sig._variable._name);
    Emit("\", \"");
    Emit(sig._sig);
    Emit("\", &");
    Emit(refName);
    Emit(");");
    EmitNL();
    DecrDepth();
    Emit("}");
    EmitNL();
    Emit(*sig._variable._type);
    Emit(" ");
    Emit(sig._variable._name);
    Emit(";");
    EmitNL();
    EmitNL();

    ++_nEmittedVars;

    EmitDecl();
}

void CodeEmitter::EmitGlobalPrologue() {
    Emit("#pragma once");
    EmitNL();
    EmitNL();
    Emit("#include \"libzhl.h\"");
    EmitNL();
    EmitNL();
}

void CodeEmitter::EmitImplPrologue() {
    EmitImpl();
    Emit("#include \"HookSystem_private.h\"");
    EmitNL();
    Emit("#include \"IsaacRepentance.h\"");
    EmitNL();
    EmitNL();

    EmitDecl();
}

void CodeEmitter::BuildExternalNamespaces() {
    for (ExternalFunction const& fn : _global._externs) {
        _externals[fn._namespace].push_back(&fn);
    }
}

void CodeEmitter::EmitNamespace(std::string const& name) {
    EmitNL();
    std::vector<const ExternalFunction*> const& funcs = _externals[name];
    EmitDecl();
    Emit("namespace ");
    Emit(name);
    Emit(" {");
    EmitNL();
    IncrDepth();
    for (const ExternalFunction* fn : funcs) {
        EmitTab();
        EmitFunction(fn->_fn);
        Emit(*fn);
        EmitNL();
    }
    DecrDepth();
    Emit("}");
}

void CodeEmitter::Emit(const ExternalFunction& fn) {
    uint32_t depth = _emitDepth;
    _emitDepth = 0;
    EmitImpl();
    Emit("namespace _extFun");
    Emit(std::to_string(_nEmittedExternal));
    Emit(" {");
    EmitNL();
    IncrDepth();
    EmitTab();
    Emit("static void* ptr = GetProcAddress(GetModuleHandle(\"");
    Emit(fn._dll);
    Emit("\"), \"");
    Emit(fn._name);
    Emit("\");");
    EmitNL();
    EmitTab();
    Emit("static void* func = 0;");
    EmitNL();
    EmitTab();
    auto [hasImplicitOutput, _, __] = EmitArgData(fn._fn);
    EmitTab();
    Emit("static FunctionDefinition fn(\"");
    Emit(fn._namespace);
    Emit("::");
    Emit(fn._fn._name);
    Emit("\", ");
    EmitTypeID(fn._fn);
    Emit(", ptr, args, ");
    Emit(std::to_string(fn._fn._params.size() + (hasImplicitOutput ? 1 : 0)));
    Emit(", ");
    Emit(std::to_string(GetFlags(fn._fn)));
    Emit(", &func);");
    EmitNL();
    DecrDepth();
    Emit("}");
    EmitNL();
    EmitNL();
    ++_nEmittedExternal;
    EmitDecl();

    _emitDepth = depth;
}

std::tuple<bool, uint32_t, uint32_t> CodeEmitter::EmitArgData(Function const& fn) {
    bool hasImplicitOutput = fn._ret->size() > 8 || (fn._ret->IsStruct() && fn._kind == METHOD);
    uint32_t stackSize = 0;
    uint32_t fnStackSize = 0;

    if (!hasImplicitOutput && fn._params.size() == 0) {
        Emit("static HookSystem::ArgData* args = nullptr;");
    }
    else {
        Emit("static HookSystem::ArgData args[] = {");
        EmitNL();
        IncrDepth();
        if (fn._convention && *fn._convention == FASTCALL) {
            Registers next = ECX;
            for (size_t i = 0; i < fn._params.size(); ++i) {
                FunctionParam const& param = fn._params[i];
                EmitTab();
                if (next != EAX && (param._type->IsPointer() || (param._type->IsBasic() && std::get<BasicType>(param._type->_value).size() <= 4))) {
                    Emit("{ HookSystem::GPRegisters::");
                    if (next == ECX) {
                        Emit("ECX");
                        next = EDX;
                    }
                    else {
                        Emit("EDX");
                        next = EAX;
                    }

                    Emit(", 0 } /* " + param._name + " */");
                    stackSize += 4;
                }
                else {
                    int paramSize = (int)std::ceil(param._type->size() / 4.f);
                    if (paramSize == 0) {
                        std::cerr << "Got a size of 0 for param " << param._name << " of type " << param._type->ToString(false) << " of function " << fn._name << ", true size is " << param._type->size() << std::endl;
                    }
                    Emit("{ HookSystem::NoRegister(), ");
                    Emit(std::to_string(paramSize));
                    Emit(" } /* " + param._name + " */");
                    stackSize += paramSize * 4;
                    fnStackSize += paramSize * 4;
                }

                if (hasImplicitOutput || i != fn._params.size() - 1) {
                    Emit(", ");
                }
                EmitNL();
            }
        }
        else {
            for (size_t i = 0; i < fn._params.size(); ++i) {
                EmitTab();
                FunctionParam const& param = fn._params[i];

                if (param._reg) {
                    Registers reg = *param._reg;
                    if (reg < XMM0) {
                        Emit("{ HookSystem::GPRegisters::");
                    }
                    else {
                        Emit("{ HookSystem::XMMRegisters::");
                    }

                    std::string regStr = RegisterToString(reg);
                    std::transform(regStr.begin(), regStr.end(), regStr.begin(), ::toupper);
                    Emit(regStr);
                    Emit(", 0 } /* " + param._name + " */");

                    stackSize += 4;
                }
                else {
                    int paramSize = (int)std::ceil(param._type->size() / 4.f);
                    if (paramSize == 0) {
                        std::cerr << "Got a size of 0 for param " << param._name << " of type " << param._type->ToString(false) << " of function " << fn._name << ", true size is " << param._type->size() << std::endl;
                    }
                    Emit("{ HookSystem::NoRegister(), ");
                    Emit(std::to_string(paramSize));
                    Emit(" } /* " + param._name + " */");

                    stackSize += paramSize * 4;
                    fnStackSize += paramSize * 4;
                }

                if (hasImplicitOutput || i != fn._params.size() - 1) {
                    Emit(", ");
                }
                EmitNL();
            }
        }

        if (hasImplicitOutput) {
            EmitTab();
            Emit("{ HookSystem::NoRegister(), 1 } // implicit_output"); // 1: pointer
            EmitNL();

            stackSize += 4; // Pointer
        }

        DecrDepth();
        EmitTab();
        Emit("};");
    }

    EmitNL();

    return std::make_tuple(hasImplicitOutput, stackSize, fnStackSize);
}

void CodeEmitter::EmitTypeID(Function const& fn) {
    Emit("typeid(");
    Emit(*fn._ret);
    Emit(" (");
    if (_currentStructure && fn._kind == METHOD) {
        Emit(_currentStructure->_name);
        Emit("::");
    }
    Emit("*)(");
    for (size_t i = 0; i < fn._params.size(); ++i) {
        FunctionParam const& param = fn._params[i];
        Emit(*param._type);
        if (i != fn._params.size() - 1) {
            Emit(", ");
        }
    }
    Emit("))");
}

uint32_t CodeEmitter::GetFlags(Function const& fn) const {
    uint32_t flags = 0;
    if (fn._kind == METHOD) {
        flags |= FunctionDefFlags::DEF_THISCALL;
    }

    if (fn.IsCleanup()) {
        flags |= FunctionDefFlags::DEF_CLEANUP;
    }

    if (fn._ret->IsBasic() && std::get<BasicType>(fn._ret->_value)._type == VOID) {
        flags |= FunctionDefFlags::DEF_VOID;
    }

    if (fn._ret->size() == 8) {
        flags |= FunctionDefFlags::DEF_LONGLONG;
    }

    return flags;
}

Function const* CodeEmitter::GetFunction(std::variant<Signature, Skip, Function> const& fn) {
    if (std::holds_alternative<Signature>(fn)) {
        return &(std::get<Signature>(fn)._function);
    }
    else if (std::holds_alternative<Function>(fn)) {
        return &std::get<Function>(fn);
    }
    else {
        return nullptr;
    }
}