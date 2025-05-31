#include "CodeEmitter.h"
#include "ParserCore.h"

#include <fstream>
#include <iostream>

static Logger parserLogger(LOG_INFO, "ZHL Generator", {
    new StdoutLogOutput(),
    new StderrLogOutput(),
    new FileLogOutput("libzhlgen.log", "w")
});

Logger* ParserLogger() {
    return &parserLogger;
}

CodeEmitter::TypeEmitter::TypeEmitter(CodeEmitter* emitter) : _emitter(emitter) {

}

template<typename T>
void CodeEmitter::TypeEmitter::operator()(T const& t) {
    _emitter->EmitType(t);
}

CodeEmitter::TypeGenerator::TypeGenerator(CodeEmitter* emitter) : _emitter(emitter) {

}

template<typename T>
std::string CodeEmitter::TypeGenerator::operator()(T const& t) {
    return _emitter->GenerateType(t);
}

CodeEmitter::CodeEmitter(TypeMap* types, AsmDefMap* asmDefs, bool test) :
    _types(types), _asmDefs(asmDefs) {
    if (test) {
        _decls.open("IsaacRepentance.h");
        _impl.open("IsaacRepentance.cpp");
        _json.open("hooks.json");
    }
    else {
        _decls.open("../include/IsaacRepentance.h");
        _impl.open("../libzhl/IsaacRepentance.cpp");
        _json.open("../include/hooks.json");
    }

}

bool CodeEmitter::ProcessZHLFiles(fs::path const& base) {
    bool ok = true;

	for (fs::directory_entry const& entry : fs::directory_iterator(base)) {
		//std::cout << "File " << entry.path() << std::endl;
		if (!entry.is_regular_file())
			continue;

		//std::cout << "Ext " << entry.path().extension() << std::endl;
		if (entry.path().extension() != std::string(".zhl"))
			continue;

        if (!ProcessFile(entry.path())) {
            ok = false;
        }
	}

    return ok;
}

bool CodeEmitter::ProcessFile(fs::path const& file) {
    std::ifstream stream(file);

    if (!stream.is_open()) {
        return false;
    }

    // std::cout << "Visiting " << path << std::endl;
    antlr4::ANTLRInputStream input_stream(stream);
    ZHLLexer lexer(&input_stream);
    antlr4::CommonTokenStream tokens(&lexer);
    ZHLParser parser(&tokens);

    antlr4::tree::ParseTree* tree = parser.zhl();

    if (lexer.getNumberOfSyntaxErrors() != 0 || parser.getNumberOfSyntaxErrors() != 0) {
        if constexpr (std::is_same_v<char, fs::path::value_type>) {
            ParserLogger()->Fatal("Errors encountered while parsing %s\n", file.c_str());
        } else if constexpr (std::is_same_v<wchar_t, fs::path::value_type>) {
            ParserLogger()->Fatal("Errors encountered while parsing %ls\n", file.c_str());
        } else {
            ParserLogger()->Fatal("Errors encountered while parsing input\n");
        }

        return false;
    }

    Parser p2(&_global, _types, _asmDefs, file.string());
    bool result = std::any_cast<bool>(p2.visit(tree));

    if (!result) {
        if constexpr (std::is_same_v<char, fs::path::value_type>) {
            ParserLogger()->Fatal("Errors encountered while visting %s\n", file.c_str());
        } else if constexpr (std::is_same_v<wchar_t, fs::path::value_type>) {
            ParserLogger()->Fatal("Errors encountered while visting %ls\n", file.c_str());
        } else {
            ParserLogger()->Fatal("Errors encountered while visiting input\n");
        }
    }

    return result;
}

void CodeEmitter::Emit() {
    EmitGlobalPrologue();
    EmitImplPrologue();
    EmitJsonPrologue();

    EmitForwardDecl();

    EmitDecl();

    for (GenericCode const& code : _global._generic) {
        Emit(code);
        EmitNL();
    }

    for (VariableSignature const& sig : _global._references) {
        Emit(sig);
    }

    for (auto const& [_, type] : *_types) {
        if (type->IsStruct() && !type->_pending) {
            Emit(type->GetStruct());
        }
    }

    for (Signature const& sig : _global._signatures) {
        Emit(sig, false);
    }

    BuildExternalNamespaces();
    for (auto const& [name, _] : _externals) {
        EmitNamespace(name);
    }

    EmitAsmDefinitions();

    EmitJsonEpilogue();
}

void CodeEmitter::EmitAsmDefinitions() {
    EmitAsmDefinitions_Decl();
    EmitAsmDefinitions_Impl();
}

void CodeEmitter::EmitAsmDefinitions_Decl() {
    EmitDecl();
    uint32_t depth = ResetDepth();
    Emit("namespace AsmDefinitions {");
    EmitNL();
    IncrDepth();

    for (auto const& [name, _] : *_asmDefs) {
        EmitTab();
        std::ostringstream stream;
        stream << "extern LIBZHL_API char " << name << ";";
        Emit(stream.str());
        EmitNL();
    }

    DecrDepth();
    Emit("}");
    EmitNL();

    RestoreDepth(depth);
}

void CodeEmitter::EmitAsmDefinitions_Impl() {
    EmitImpl();
    uint32_t depth = ResetDepth();
    Emit("namespace AsmDefinitions {");
    EmitNL();
    IncrDepth();

    for (auto const& [name, signature] : *_asmDefs) {
        EmitTab();
        std::ostringstream stream;
        stream << "char " << name << ";";
        Emit(stream.str());
        EmitNL();
    }

    EmitNL();

    for (auto const& [name, signature] : *_asmDefs) {
        EmitTab();
        std::ostringstream stream;
        stream << "ASMDefinition asm_" << name << "(&" << name << ", \"" << name <<
            "\", " << signature << ");";
        Emit(stream.str());
        EmitNL();
    }

    DecrDepth();
    Emit("}");
    RestoreDepth(depth);
}

void CodeEmitter::EmitForwardDecl() {
    EmitDecl();
    for (auto const& [_, type] : *_types) {
        if (type->IsStruct() && !type->_pending) {
            Emit("struct ");
            Emit(type->GetStruct()._name);
            Emit(";");
            EmitNL();
        }
    }
}

void CodeEmitter::EmitJsonPrologue() {
    EmitJson();
    Emit("[\n");
}

void CodeEmitter::EmitJsonEpilogue() {
    EmitJson();
    Emit("\t{ \"function\": \"\", \"hook\": \"no\" }\n");
    Emit("]");
}

void CodeEmitter::EmitJson(Function const& fn, std::string const& internalName,
    std::string const& fullName) {
    EmitJson();
    Emit("\t{ \"function\": \"");
    Emit(internalName);
    Emit("\", \"fullName\": \"");
    Emit(fullName);
    Emit("\", \"hook\": ");
    if (fn.CanHook()) {
        Emit("\"yes\"");
    } else {
        Emit("\"no\"");
    }
    Emit(" },\n");
}

std::string CodeEmitter::GenerateType(Type const& type) {
    std::string result = "";

    if (type._synonym) {
        return GenerateType(*type._synonym);
    }

    if (type._base) {
        result = GenerateType(*type._base);
        if (type._const) {
            result += " const";
        } else if (type.IsPointer()) {
            result += GenerateType(type._pointerDecl.front());
        }
    } else {
        result = std::visit(TypeGenerator(this), type._value);
    }

    return result;
}

void CodeEmitter::Emit(Type const& type) {
    Emit(GenerateType(type));
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
    std::ostringstream size;
    size << " // 0x" << std::hex << s.size();
    Emit(size.str());
    EmitNL();
    EmitNL();

    _processingStructures.erase(s._name);
    _emittedStructures.insert(s._name);

    _currentStructure = nullptr;
}

void CodeEmitter::Dump() {
    std::set<std::string> pending;
    ParserLogger()->Debug("CodeEmitter: Dumping TypeMap\n");
    for (auto const& [name, type] : *_types) {
        std::ostringstream stream;
        stream << name << " => " << type->ToString(true) << " (size = ";
        try {
            stream << type->size();
        }
        catch (std::exception&) {
            stream << " ERROR";
        }

        stream << ")" << std::endl;

        if (!type->IsResolved()) {
            if (type->IsEmpty()) {
                stream << "EMPTY TYPE" << std::endl;
            }
            else {
                pending.insert(std::get<std::string>(type->_value));
            }
        }

        ParserLogger()->Debug("%s", stream.str().c_str());
    }

    std::ostringstream stream;
    stream << "UNRESOLVED TYPES: ";
    for (std::string const& s : pending) {
        stream << s << " ";
    }
    stream << std::endl;
    ParserLogger()->Debug("%s", stream.str().c_str());
}

std::string CodeEmitter::GenerateType(BasicType const& t) {
    std::string result;
    if (t._sign) {
        result += SignednessToString(*t._sign) + " ";
    }

    if (t._length) {
        result += LengthToString(*t._length) + " ";
    }

    result += BasicTypeToString(t._type);
    return result;
}

void CodeEmitter::EmitType(BasicType const& t) {
    Emit(GenerateType(t));
}

std::string CodeEmitter::GenerateType(Struct const& s) {
    std::string result = s._name;

    if (s._template) {
        result += "<";

        for (size_t i = 0; i < s._templateParams.size(); ++i) {
            result += GenerateType(*s._templateParams[i]);
            if (i != s._templateParams.size() - 1) {
                result += ", ";
            }
        }
        result += ">";
    }

    return result;
}

void CodeEmitter::EmitType(Struct const& s) {
    Emit(GenerateType(s));
}

std::string CodeEmitter::GenerateType(FunctionPtr* ptr) {
    std::string result = "";
    if (_variableContext) {
        result += GenerateType(*ptr->_ret) + " (";

        if (ptr->_convention) {
            result += "";   // TODO: why did I write that already ? Shouldn't I
                            // be emitting the calling convention here ?
        }

        if (ptr->_scope) {
            result += GenerateType(*ptr->_scope) + "::";
        }

        result += "*" + _variableContext->_name + ")(";

        for (size_t i = 0; i < ptr->_parameters.size(); ++i) {
            result += GenerateType(*(ptr->_parameters[i]));
            if (i != ptr->_parameters.size()) {
                result += ", ";
            }
        }

        result += ")";
    }

    return result;
}

void CodeEmitter::EmitType(FunctionPtr* ptr) {
    Emit(GenerateType(ptr));
}

std::string CodeEmitter::GenerateType(std::string const& t) {
    return t;
}

void CodeEmitter::EmitType(std::string const& t) {
    Emit(t);
}

std::string CodeEmitter::GenerateType(EmptyType const&) {
    throw std::runtime_error("[FATAL] Attempting to generate empty type");
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

void CodeEmitter::EmitJson() {
    _emitContext = &_json;
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
        if (var._name.substr(0, 3) != "pad") {
            Emit(std::to_string(var._type->_arraySize));
        } else {
            std::ostringstream stream;
            stream << "0x" << std::hex << var._type->_arraySize;
            Emit(stream.str());
        }
        Emit("]");
    }

    Emit(";");
    if (var._offset) {
        std::ostringstream offsetStream;
        offsetStream << " // 0x" << std::hex << *var._offset;
        Emit(offsetStream.str());
    }
    EmitNL();

    _variableContext = nullptr;
}

void CodeEmitter::Emit(Signature const& var, bool isVirtual) {
    Function const& fun = var._function;

    if (fun.IsDebug()) {
        Emit("#ifdef DEBUG");
        EmitNL();
    }
    EmitTab();
    Emit("LIBZHL_API ");
    /* if (isVirtual) {
        Emit("virtual ");
    } */
    EmitFunction(fun);

    if (fun.IsDebug()) {
        Emit("#endif");
        EmitNL();
    }

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

void CodeEmitter::EmitFunction(Function const& fun, bool withPrefix) {
    if (withPrefix) {
        EmitTab();
        Emit("LIBZHL_API ");
    }

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
    EmitNL();
}

void CodeEmitter::Emit(std::variant<Signature, Skip, Function> const& sig) {
    if (std::holds_alternative<Skip>(sig)) {
        EmitTab();
        Emit("void virtual_unk");
        Emit(std::to_string(_virtualFnUnk));
        Emit("() { }");
        EmitNL();
        ++_virtualFnUnk;
    }
    else if (std::holds_alternative<Signature>(sig)) {
        Emit(std::get<Signature>(sig), true);
    }
    else {
        Function const& fn = std::get<Function>(sig);
        EmitFunction(fn, true);
        EmitAssembly(fn, true, false);

        Function copy = fn;
        copy._name.append("_Original");
        EmitFunction(copy, true);
        EmitAssembly(copy, true, true);
    }
}

std::string CodeEmitter::GenerateType(const PointerDecl& ptr) {
    std::string result = "";
    switch (ptr._kind) {
    case LREF:
        result = " &";
        break;

    case RREF:
        result = " &&";
        break;

    case POINTER:
        result = " *";
        break;
    }

    if (ptr._const) {
        result += " const";
    }

    return result;
}

void CodeEmitter::Emit(PointerDecl const& ptr) {
    Emit(GenerateType(ptr));
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
            std::string internalName = "_fun" + std::to_string(_nEmittedFuns);

            EmitTab();
            Emit("static FunctionDefinition fn(\"");
            if (_currentStructure) {
                Emit(_currentStructure->_name);
                Emit("::");
            }
            Emit(fn._name);
            Emit("\", \"");
            Emit(internalName);
            Emit("\", ");
            EmitTypeID(fn);
            Emit(", \"");
            Emit(std::get<Signature>(sig)._sig);
            Emit("\", args, ");
            Emit(std::to_string(fn._params.size() + (hasImplicitOutput ? 1 : 0)));

            Emit(", ");
            Emit(std::to_string(flags));
            Emit(", &func, ");
            if (fn.CanHook()) {
                Emit("true");
            } else {
                Emit("false");
            }
            Emit("); ");
            EmitNL();

            std::string fullName = GenerateFunctionPrototype(fn, true);
            _fullNameToInternalName[fullName] = internalName;
            EmitJson(fn, internalName, fullName);
            EmitImpl();
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
        if (*fn._convention != THISCALL && *fn._convention != X86_64 && *fn._convention != X86_64_OUTPUT) {
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

        // Analyze position of arguments
        std::vector<std::optional<Registers>> placement(fn._params.size());
        if (fn._convention && *fn._convention == CallingConventions::FASTCALL) {
            Registers next = Registers::ECX;
            int i = 0;
            for (FunctionParam const& param : fn._params) {
                Type* type = param._type;
                if (!param._reg && next != Registers::EAX && (type->IsPointer() || (type->IsBasic() && std::get<BasicType>(type->_value).size() <= 4))) {
                    placement[i] = next;

                    switch (next) {
                    case Registers::ECX:
                        next = Registers::EDX;
                        break;

                    case Registers::EDX:
                        next = Registers::EAX;
                        break;

                    default:
                        break;
                    }
                }
                ++i;
            }
        }

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
            else if (placement[i]) {
                // Do nothing, value already in register
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
            if (fn._convention && *fn._convention == CallingConventions::X86_64_OUTPUT) {
                ins << "mov ecx, [ebp + " << k << "] // implicit_output";
            }
            else {
                ins << "push [ebp + " << k << "] // implicit_output";
            }
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
                position = _currentStructure->GetVirtualFunctionSlot(&(std::get<Signature>(sig)), true);
            }
            else {
                position = _currentStructure->GetVirtualFunctionSlot(&(std::get<Function>(sig)));
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
    EmitDecl();
    Emit("#pragma once\n");
    EmitNL();
    Emit("#include \"libzhl.h\"\n"
        "#include <map>\n"
        "#include <vector>\n");
    EmitNL();
}

void CodeEmitter::EmitImplPrologue() {
    EmitImpl();
    Emit("#include \"ASMDefinition.h\"\n");
    Emit("#include \"HookSystem_private.h\"\n");
    Emit("#include \"IsaacRepentance.h\"\n");
    EmitNL();
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
        if (fn->_fn.IsDebug()) {
            Emit("#ifdef DEBUG");
            EmitNL();
        }
        EmitTab();
        EmitFunction(fn->_fn);
        Emit(*fn);
        EmitNL();
        if (fn->_fn.IsDebug()) {
            Emit("#endif");
            EmitNL();
        }
    }
    DecrDepth();
    Emit("}");
}

void CodeEmitter::Emit(const ExternalFunction& fn) {
    uint32_t depth = _emitDepth;
    _emitDepth = 0;
    EmitImpl();
    if (fn._fn.IsDebug()) {
        Emit("#ifdef DEBUG");
        EmitNL();
    }
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
    Emit("\", \"\", "); // Empty internal name
    EmitTypeID(fn._fn);
    Emit(", ptr, args, ");
    Emit(std::to_string(fn._fn._params.size() + (hasImplicitOutput ? 1 : 0)));
    Emit(", ");
    Emit(std::to_string(GetFlags(fn._fn)));
    Emit(", &func, ");
    if (fn._fn.CanHook()) {
        Emit("true");
    } else {
        Emit("false");
    }
    Emit("); ");
    EmitNL();
    DecrDepth();
    Emit("}");
    EmitNL();
    if (fn._fn.IsDebug()) {
        Emit("#endif");
        EmitNL();
    }
    EmitNL();
    ++_nEmittedExternal;
    EmitDecl();

    _emitDepth = depth;
}

std::tuple<bool, uint32_t, uint32_t> CodeEmitter::EmitArgData(Function const& fn) {
    bool hasImplicitOutput = fn._ret->size() > 8 || (fn._ret->IsStruct() && fn._kind == METHOD) || fn._convention == X86_64_OUTPUT;
    uint32_t stackSize = 0; // How much the trampoline takes on the stack
    uint32_t fnStackSize = 0; // How much the trampoline should pop after calling the function

    if (!hasImplicitOutput && fn._params.size() == 0) {
        Emit("static HookSystem::ArgData* args = nullptr;");
    }
    else {
        Emit("static HookSystem::ArgData args[] = {");
        EmitNL();
        IncrDepth();
        if (fn._convention && (*fn._convention == FASTCALL || *fn._convention == X86_64 || *fn._convention == X86_64_OUTPUT)) {
            if (*fn._convention == FASTCALL) {
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

                        Emit(", 0 }");

                        if (i != fn._params.size() - 1) {
                            Emit(",");
                        }

                        Emit("/* " + param._name + " */");
                        /* Do not actually increase the size of the stack here :
                         * because the trampoline is generated as a fastcall
                         * function, parameters are already placed in registers
                         * and not on the stack.
                         */

                         // stackSize += 4;
                    }
                    else {
                        EmitParamData(fn, param, &fnStackSize, &stackSize, fn._params.size() - 1);
                    }

                    /* if (hasImplicitOutput || i != fn._params.size() - 1) {
                        Emit(", ");
                    } */
                    EmitNL();
                }
            }
            else {
                size_t position = 0;
                if (hasImplicitOutput) {
                    EmitTab();
                    Emit("{ HookSystem::GPRegisters::ECX } ");
                    ++position;

                    if (fn._params.size() != 0) {
                        Emit(", ");
                    }

                    Emit(" // implicit_output");
                    EmitNL();

                    stackSize += 4;
                }

                for (size_t i = 0; i < fn._params.size(); ++i) {
                    FunctionParam const& param = fn._params[i];
                    EmitTab();

                    if (position < 2) {
                        if (param._type->size() > 4) {
                            std::ostringstream s;
                            s << "[FATAL] While emitting code for function " << fn.ToString() <<
                                " with x86-64 like calling convention, encountered parameter"
                                "that does not fit in register (parameter " << position + 1 << ")" << std::endl;
                            throw std::runtime_error(s.str());
                        }
                    }

                    if (param.IsX8664Valid(position)) {
                        Registers reg;
                        std::string category;
                        if (param._type->IsBasic()) {
                            BasicType basic = std::get<BasicType>(param._type->_value);

                            if (basic._type == BasicTypes::INT || basic._type == BasicTypes::BOOL || basic._type == BasicTypes::CHAR) {
                                reg = position == 0 ? Registers::ECX : Registers::EDX;
                                category = "GPRegisters";
                            }
                            else {
                                category = "XMMRegisters";
                                switch (position) {
                                case 0:
                                    reg = Registers::XMM0;
                                    break;

                                case 1:
                                    reg = Registers::XMM1;
                                    break;

                                case 2:
                                    reg = Registers::XMM2;
                                    break;

                                case 3:
                                    reg = Registers::XMM3;
                                    break;
                                }

                                Emit("{ HookSystem::XMMRegisters::");
                            }
                        }
                        else {
                            reg = position == 0 ? Registers::ECX : Registers::EDX;
                            category = "GPRegisters";
                        }

                        const_cast<FunctionParam&>(param)._reg = reg;

                        Emit("{ HookSystem::");
                        Emit(category);
                        Emit("::");
                        auto regStr = RegisterToString(reg);
                        std::transform(regStr.begin(), regStr.end(), regStr.begin(), ::toupper);
                        Emit(regStr);
                        Emit(" }");

                        stackSize += 4;
                        ++position;
                    }
                    else {
                        EmitParamData(fn, param, &fnStackSize, &stackSize, i != fn._params.size() - 1);
                    }

                    if (i != fn._params.size() - 1) {
                        Emit(", ");
                    }

                    Emit(" /* ");
                    Emit(param._name);
                    Emit(" */");
                    EmitNL();
                }
            }
        }
        else {
            if (hasImplicitOutput && (!fn._convention || fn._convention != X86_64_OUTPUT)) {
                EmitTab();
                Emit("{ HookSystem::NoRegister(), 1 }"); // 1: pointer
                if (fn._params.size() != 0) {
                    Emit(",");
                }
                Emit(" /* implicit_output */");
                EmitNL();

                stackSize += 4; // Pointer
                fnStackSize += 4;
            }

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
                    Emit(", 0 }");

                    if (i != fn._params.size() - 1) {
                        Emit(", ");
                    }

                    Emit("/* " + param._name + " */");

                    stackSize += 4;
                }
                else {
                    EmitParamData(fn, param, &fnStackSize, &stackSize, i != fn._params.size() - 1);
                }

                EmitNL();
            }
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
    EmitFunctionPrototype(fn, false);
    Emit(")");
}

std::string CodeEmitter::GenerateFunctionPrototype(Function const& fn, bool includeName) {
    std::string result = GenerateType(*fn._ret) + " (";
    if (_currentStructure && fn._kind == METHOD) {
        result += _currentStructure->_name + "::";
    }
    if (includeName) {
        result += fn._name;
    } else {
        result += "*";
    }

    result += ")(";
    for (size_t i = 0; i < fn._params.size(); ++i) {
        FunctionParam const& param = fn._params[i];
        result += GenerateType(*param._type);
        if (i != fn._params.size() - 1) {
            result += ", ";
        }
    }
    result += ")";
    return result;
}

void CodeEmitter::EmitFunctionPrototype(Function const& fn, bool includeName) {
    Emit(GenerateFunctionPrototype(fn, includeName));
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

void CodeEmitter::EmitParamData(Function const& fn, FunctionParam const& param, uint32_t* fnStackSize, uint32_t* stackSize, bool comma) {
    int paramSize = (int)std::ceil(param._type->size() / 4.f);
    if (paramSize == 0) {
        std::cerr << "Got a size of 0 for param " << param._name << " of type " << param._type->ToString(false) << " of function " << fn._name << ", true size is " << param._type->size() << std::endl;
    }
    Emit("{ HookSystem::NoRegister(), ");
    Emit(std::to_string(paramSize));
    Emit(" }");

    if (comma) {
        Emit(",");
    }
    Emit("/* " + param._name + " */");

    *stackSize += paramSize * 4;
    *fnStackSize += paramSize * 4;
}