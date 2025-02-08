#include "ParserCore.h"

Parser::Parser(Namespace* global, TypeMap* types, std::string const& filename) : _global(global), _types(types), _currentFile(filename) {

}

std::any Parser::visitZhl(ZHLParser::ZhlContext* ctx) {
    visitChildren(ctx);
    return !_hasFatalError;
}

std::any Parser::visitFunction(ZHLParser::FunctionContext* ctx) {
    Function fn;
    _currentFunction = &fn;

    std::vector<std::string> parts = std::any_cast<std::vector<std::string>>(visit(ctx->nestedName()));
    fn._name = parts.back(); // MergeNameParts(parts);

    std::optional<std::string> convention;
    if (auto node = ctx->CallingConvention()) {
        convention = node->getText();
        if (*convention != "") {
            fn._convention = StringToConvention(*convention);
        }
        
        if (convention == "__thiscall") {
            if (!_currentStruct) {
                if (ctx->nestedName()->functionName().size() == 0) {
                    // _errors << ErrorLogger::error << "__thiscall convention applied to non member function " << fn._name << ErrorLogger::_end;
                    _hasFatalError = true;
                    ParserLogger()->Error("%s: __thiscall convention applied to non member function %s\n", _currentFile.c_str(), fn._name.c_str());

                }
                fn._convention = CallingConventions::THISCALL;
            }
            else {
                fn._convention = CallingConventions::THISCALL;
            }

            fn._kind = METHOD;
        }
    }

    ReadQualifiers(ctx->Qualifier(), fn);

    if (parts.size() > 1 && !fn.IsStatic()) {
        fn._kind = METHOD;
    }
    else {
        fn._kind = FUNCTION;
    }

    fn._ret = std::any_cast<Type*>(visit(ctx->type()));

    std::string fullNameStr = GetCurrentFunctionQualifiedName(fn._name);
    const char* fullName = fullNameStr.c_str();

    if (fn.IsVirtual() && fn.IsStatic()) {
        ParserLogger()->Error("%s: static and virtual qualifiers applied together to function %s\n",
            _currentFile.c_str(), fullName);
        _hasFatalError = true;
    }

    if (fn.IsStatic() && fn._convention && fn._convention == CallingConventions::THISCALL) {
        ParserLogger()->Error("%s: __thiscall convention applied to static function %s\n",
            _currentFile.c_str(), fullName);
        _hasFatalError = true;
    }

    if (auto args = ctx->funArgs()) {
        fn._params = std::any_cast<std::vector<FunctionParam>>(visit(ctx->funArgs()));

        if (fn._convention) {
            for (FunctionParam const& param : fn._params) {
                if (param._reg) {
                    ParserLogger()->Warn("%s: calling convention %s specified on function %s, you cannot specify "
                        "a register on a parameter (specified register %s on parameter %s)\n",
                        _currentFile.c_str(), CallingConventionToString(*fn._convention).c_str(),
                        fullName, RegisterToString(*param._reg).c_str(),
                        param._name.c_str());
                }
            }
        }
    }

    _currentFunction = nullptr;
    return std::make_tuple(parts.size() > 1, parts.front(), fn);
}

std::any Parser::visitReference(ZHLParser::ReferenceContext* ctx) {
    Variable var;
    _currentVariable = &var;

    var._name = ctx->Name()->getText();
    var._type = std::any_cast<Type*>(visit(ctx->type()));

    _currentVariable = nullptr;
    return var;
}

/// _currentFunction assumed not null
std::any Parser::visitFunArgs(ZHLParser::FunArgsContext* ctx) {
    std::vector<FunctionParam> params;
    typedef std::pair<bool, std::vector<unsigned int>> RegUse;
    std::array<RegUse, Registers::MAX> state = { { { false,  {} } } };

    int i = 0;
    for (ZHLParser::FunArgContext* arg : ctx->funArg()) {
        FunctionParam param = std::any_cast<FunctionParam>(visit(arg));
        if (param._reg) {
            RegUse& use = state[*param._reg];
            use.first = true;
            use.second.push_back(i);
        }
        params.push_back(param);
        ++i;
    }

    i = 0;
    for (RegUse const& use : state) {
        if (use.first) {
            if (use.second.size() > 1) {
                _hasFatalError = true;

                std::ostringstream stream;

                stream << _currentFile << ": register " << RegisterToString((Registers)i) << 
                    " used on multiple parameters in function " << 
                    GetCurrentFunctionQualifiedName(_currentFunction->_name) << std::endl;
                stream << "\tInfringing parameters are ";
                for (int paramPos : use.second) {
                    FunctionParam const& param = params[paramPos];
                    stream << param._name << " (param " << i << ")";
                    if (paramPos != use.second.back()) {
                        stream << ", ";
                    }
                }
                stream << std::endl;

                ParserLogger()->Error("%s", stream.str().c_str());
            }
        }

        ++i;
    }

    return params;
}

std::any Parser::visitFunArg(ZHLParser::FunArgContext* ctx) {
    FunctionParam param;
    _currentParam = &param;

    FullNameV name = std::any_cast<FullNameV>(visit(ctx->fullName()));
    Type* type = std::any_cast<Type*>(visit(ctx->type()));

    if (std::holds_alternative<Array>(name)) {
        Array& arr = std::get<Array>(name);
        type = _types->MakeArray(type, arr);
        param._name = arr._name;
    }
    else {
        param._name = std::get<std::string>(name);
    }

    param._type = type;

    if (auto argParam = ctx->argParam()) {
        param._reg = std::any_cast<Registers>(visit(argParam));
    }

    _currentParam = nullptr;
    return param;
}

std::any Parser::visitArgParam(ZHLParser::ArgParamContext* ctx) {
    std::string name = ctx->Register()->toString();

    std::map<std::string, Registers> assoc;
    assoc["eax"] = Registers::EAX;
    assoc["ebx"] = Registers::EBX;
    assoc["ecx"] = Registers::ECX;
    assoc["edx"] = Registers::EDX;
    assoc["esi"] = Registers::ESI;
    assoc["edi"] = Registers::EDI;
    assoc["esp"] = Registers::ESP;
    assoc["ebp"] = Registers::EBP;
    assoc["xmm0"] = Registers::XMM0;
    assoc["xmm1"] = Registers::XMM1;
    assoc["xmm2"] = Registers::XMM2;
    assoc["xmm3"] = Registers::XMM3;
    assoc["xmm4"] = Registers::XMM4;
    assoc["xmm5"] = Registers::XMM5;
    assoc["xmm6"] = Registers::XMM6;
    assoc["xmm7"] = Registers::XMM7;

    return assoc[name];
}

std::any Parser::visitOptNamedFunArgs(ZHLParser::OptNamedFunArgsContext* ctx) {
    std::vector<Type*> params;
    for (auto argCtx : ctx->optNamedFunArg()) {
        params.push_back(std::any_cast<Type*>(visit(argCtx)));
    }

    return params;
}

std::any Parser::visitOptNamedFunArg(ZHLParser::OptNamedFunArgContext* ctx) {
    Type* param = std::any_cast<Type*>(visit(ctx->type()));
    return param;
}

std::any Parser::visitTemplateSpec(ZHLParser::TemplateSpecContext* ctx) {
    std::vector<Type*> types;
    for (auto typeCtx : ctx->type()) {
        types.push_back(std::any_cast<Type*>(visit(typeCtx)));
    }
    return types;
}

std::any Parser::visitType(ZHLParser::TypeContext* ctx) {
    // The type if never qualified at first
    std::pair<Type*, bool> base = std::any_cast<std::pair<Type*, bool>>(visit(ctx->typeSpecifier()));
    Type* type = base.first;

    if (auto templateSpecCtx = ctx->templateSpec()) {
        if (!std::holds_alternative<std::string>(type->_value)) {
            ParserLogger()->Error("%s: type %s used as base template, but already resolved\n",
                _currentFile.c_str(), type->GetFullName().c_str());
            return type;
        }
        type->_templateBase = true;

        Struct s;
        s._name = std::get<std::string>(type->_value);
        s._template = true;
        s._templateParams = std::any_cast<std::vector<Type*>>(visit(templateSpecCtx));
        
        type = _types->Get(s.GetTemplateName());
        if (type->IsEmpty()) {
            type->_value = s;
        }
    }

    if (base.second) {
        type = _types->MakeConst(type);
    }

    // std::vector<PointerDecl> pointers;
    for (auto ptrCtx : ctx->pointerAttribute()) {
        PointerDecl decl = std::any_cast<PointerDecl>(visit(ptrCtx));
        type = _types->MakePointer(type, decl);
    }

    return type;
}

std::any Parser::visitTypeSpecifier(ZHLParser::TypeSpecifierContext* ctx) {
    Type* result = nullptr;

    auto cstQual = ctx->Const();
    bool isConst = !cstQual.empty();

    if (cstQual.size() == 2) {
        ParserLogger()->Warn("%s: const specified multiple times in %s\n",
            _currentFile.c_str(), GetContext().c_str());
    }

    if (auto simple = ctx->simpleType()) {
        // result._value = std::any_cast<BasicType>(visit(simple));
        BasicType basic = std::any_cast<BasicType>(visit(simple));
        // std::cout << "Found basic type " << basic.GetAbsoluteName() << std::endl;
        result = _types->Get(basic);
        result->_value = basic;
    }
    else {
        // std::string name = MergeNameParts(std::any_cast<std::vector<std::string>>(visit(ctx->nestedName())));
        // Struct* target = GetStructByName(name);
        // result._value = name;
        // result._name = target;

        std::vector<std::string> parts = std::any_cast<std::vector<std::string>>(visit(ctx->nestedName()));
        result = _types->Get(parts.front());

        if (result->IsEmpty()) {
            result->_value = parts.front();
        }
    }

    return std::make_pair(result, isConst);
}

std::any Parser::visitSimpleType(ZHLParser::SimpleTypeContext* ctx) {
    BasicType basic;

    if (ctx->Bool()) {
        basic._type = BasicTypes::BOOL;
    }
    else if (ctx->Void()) {
        basic._type = BasicTypes::VOID;
    }
    else if (ctx->Char()) {
        basic._type = BasicTypes::CHAR;
    }
    else if (ctx->Double()) {
        basic._type = BasicTypes::DOUBLE;
    }
    else if (ctx->Float()) {
        basic._type = BasicTypes::FLOAT;
    }
    else {
        basic._type = BasicTypes::INT;
    }

    if (auto sign = ctx->simpleTypeSignedness()) {
        if (basic._type != BasicTypes::CHAR && basic._type != BasicTypes::INT) {
            ParserLogger()->Warn("%s: cannot specify sign modified (%s) on type %s in %s\n",
                _currentFile.c_str(), sign->getText().c_str(), BasicTypeToString(basic._type).c_str(),
                GetContext().c_str());
        }
        else {
            basic._sign = std::any_cast<Signedness>(visit(sign));
        }
    }

    auto length = ctx->simpleTypeLength();
    if (length.size()) {
        if (basic._type != BasicTypes::DOUBLE && basic._type != BasicTypes::INT) {
            ParserLogger()->Warn("%s: cannot specify length modifier on type %s in %s\n",
                _currentFile.c_str(), BasicTypeToString(basic._type).c_str(), GetContext().c_str());
        }
        else {
            if (length.size() == 1) {
                basic._length = std::any_cast<Length>(visit(length.front()));
            }
            else {
                Length first = std::any_cast<Length>(visit(length[0]));
                Length second = std::any_cast<Length>(visit(length[1]));

                if (first != second) {
                    _hasFatalError = true;
                    ParserLogger()->Error("%s: cannot specify short and long simultaneously on type %s in %s\n",
                        _currentFile.c_str(), BasicTypeToString(basic._type).c_str(),
                        GetContext().c_str());
                }
                else if (first == Length::SHORT) {
                    ParserLogger()->Warn("%s: short specified multiple times on type %s in %s\n",
                        _currentFile.c_str(), BasicTypeToString(basic._type).c_str(),
                        GetContext().c_str());
                    basic._length = first;
                }
                else {
                    basic._length = Length::LONGLONG;
                }
            }
        }
    }

    return basic;
}

std::any Parser::visitSimpleTypeSignedness(ZHLParser::SimpleTypeSignednessContext* ctx) {
    if (ctx->Signed()) {
        return Signedness::SIGNED;
    }
    else {
        return Signedness::UNSIGNED;
    }
}

std::any Parser::visitSimpleTypeLength(ZHLParser::SimpleTypeLengthContext* ctx) {
    if (ctx->Long()) {
        return Length::LONG;
    }
    else {
        return Length::SHORT;
    }
}

std::any Parser::visitNestedName(ZHLParser::NestedNameContext* ctx) {
    std::vector<std::string> partsStr;
    partsStr.push_back(ctx->Name()->getText());
    if (auto functionName = ctx->functionName(); !functionName.empty())
        partsStr.push_back(std::any_cast<std::string>(visit(ctx->functionName().front())));
    std::string typen = partsStr[0];

    if (partsStr.size() > 1) {
        if (!_types->Get(typen, false)) {
            Type* type = _types->New(typen);
            Struct s;
            s._name = typen;
            type->_value = s;
        }
    }

    return partsStr;
}

std::any Parser::visitPointerAttribute(ZHLParser::PointerAttributeContext* ctx) {
    PointerDecl decl;

    auto refs = ctx->CppRef();
    switch (refs.size()) {
    case 0:
        decl._kind = PointerKind::POINTER;
        break;

    case 1:
        decl._kind = PointerKind::LREF;
        break;

    case 2:
        decl._kind = PointerKind::RREF;
        break;

    default:
        throw std::runtime_error("Invalid pointer qualifier");
    }

    decl._const = ctx->Const() != nullptr;

    return decl;
}

std::any Parser::visitGenericCode(ZHLParser::GenericCodeContext* ctx) {
    std::string code(ctx->GenericCode()->getText());
    std::string stripped = code.substr(3, code.length() - 6);
    if (code.length() < 6) {
        stripped = "";
    }

    // std::cerr << "GENERIC CODE: " << code << " (" << code.size() << ", " << code.length() << ")" << std::endl;

    if (_currentStruct) {
        _currentStruct->_namespace._generic.push_back(stripped);
    }
    else {
        _global->_generic.push_back(stripped);
    }

    return 0;
}

std::any Parser::visitClass(ZHLParser::ClassContext* ctx) {
    std::string name(ctx->Name()->getText());
    Type* type = _types->Get(name);
    type->_pending = false;

    Struct* st;

    // std::cout << "Visiting type " << name << std::endl;
    if (!type->IsStruct()) {
        Struct s;
        s._name = name;
        type->_value = s;

        // std::cout << "Type " << name << " was not a structure, now it is (" << type << ")" << std::endl;
    }

    st = &(std::get<Struct>(type->_value));
    st->_name = name;
    _currentStruct = st;

    if (auto deps = ctx->depends()) {
        st->_deps = std::any_cast<std::set<Type*>>(visit(ctx->depends()));
    }

    if (auto inheritance = ctx->inheritance()) {
        st->_parents = std::any_cast<std::vector<std::tuple<Type*, Visibility>>>(visit(ctx->inheritance()));
    }

    visit(ctx->classBody());

    if (auto number = ctx->Number()) {
        size_t size;
        std::string sizeStr(ctx->Number()->getText());
        size = std::strtoull(sizeStr.c_str(), NULL, 0);
        st->_size = size;
    }

    return 0;
}

std::any Parser::visitDepends(ZHLParser::DependsContext* ctx) {
    std::set<Type*> deps;
    auto names = ctx->Name();

    for (size_t i = 0; i < names.size(); ++i) {
        std::string name(names[i]->getText());
        // Struct* st = GetStructByName(name);
        Type* t = _types->Get(name);
        if (!t->IsStruct()) {
            Struct s;
            s._name = name;
            t->_value = s;
        }

        if (deps.find(t) != deps.end()) {
            ParserLogger()->Warn("%s: specified %s as dependency for %s multiple times\n",
                _currentFile.c_str(), name.c_str(), _currentStruct->_name.c_str());
        }
        else if (name == _currentStruct->_name) {
            ParserLogger()->Warn("%s: structure %s depends on itself\n",
                _currentFile.c_str(), name.c_str());
        }
        else {
            deps.insert(t);
        }
    }

    return deps;
}

std::any Parser::visitClassBody(ZHLParser::ClassBodyContext* ctx) {
    for (auto classSigCtx : ctx->classSignature()) {
        Signature signature = std::any_cast<Signature>(visit(classSigCtx));
        _currentStruct->_namespace._signatures.push_back(signature);
    }

    for (auto fieldCtx : ctx->classField()) {
        visit(fieldCtx);
    }

    for (auto genericCodeCtx : ctx->genericCode()) {
        visit(genericCodeCtx);
    }

    for (auto functionPtrCtx : ctx->functionPtr()) {
        visit(functionPtrCtx);
    }

    auto vtablesCtx = ctx->vtable();
    if (vtablesCtx.size() > 1) {
        _hasFatalError = true;
        ParserLogger()->Error("%s: multiple vtable fields in structure %s\n",
            _currentFile.c_str(), _currentStruct->_name.c_str());
    }
    else if (vtablesCtx.size() == 1) {
        visit(vtablesCtx.front());
    }

    return 0;
}

std::any Parser::visitVtable(ZHLParser::VtableContext* ctx) {
    for (auto entryCtx : ctx->vtableEntry()) {
        visit(entryCtx);
    }

    return 0;
}

std::any Parser::visitVtableEntry(ZHLParser::VtableEntryContext* ctx) {
    if (ctx->Skip()) {
        _currentStruct->_virtualFunctions.push_back(Skip());
    }
    else if (ctx->Pure()) {
        Function func = std::any_cast<Function>(visit(ctx->classFunction()));
        func._qualifiers |= PURE;
        _currentStruct->_virtualFunctions.push_back(func);
    } 
    else {
        VirtualFunction function = std::any_cast<VirtualFunction>(visit(ctx->vtableSignature()));
        if (function._override) {
            _currentStruct->_overridenVirtualFunctions.push_back(function._signature);
        }
        else {
            _currentStruct->_virtualFunctions.push_back(function._signature);
        }
    }

    return 0;
}

std::any Parser::visitVtableSignature(ZHLParser::VtableSignatureContext* ctx) {
    VirtualFunction function;
    function._signature = std::any_cast<Signature>(visit(ctx->classSignature()));
    function._override = ctx->Override() != nullptr;
    return function;
}

std::any Parser::visitClassSignature(ZHLParser::ClassSignatureContext* ctx) {
    std::string sig(ReadSignature(ctx->Signature()->getText()));
    Function function = std::any_cast<Function>(visit(ctx->classFunction()));
    Signature signature;
    signature._sig = sig;
    signature._function = function;
    return signature;
}

std::any Parser::visitFunctionName(ZHLParser::FunctionNameContext* ctx) {
    if (ctx->Name()) {
        return ctx->Name()->getText();
    }
    else {
        std::ostringstream result;
        result << "operator" << ctx->OpSymbol()->getText();
        return result.str();
    }
}

std::any Parser::visitClassFunction(ZHLParser::ClassFunctionContext* ctx) {
    Function res;
    _currentFunction = &res;
    res._name = std::any_cast<std::string>(visit(ctx->functionName()));
    ReadQualifiers(ctx->Qualifier(), res);
    res._ret = std::any_cast<Type*>(visit(ctx->type()));

    if (auto argsCtx = ctx->funArgs()) {
        res._params = std::any_cast<std::vector<FunctionParam>>(visit(argsCtx));
    }

    if (!res.IsStatic()) {
        res._kind = METHOD;
    }
    else {
        res._kind = FUNCTION;
    }

    _currentFunction = nullptr;
    return res;
}

std::any Parser::visitClassField(ZHLParser::ClassFieldContext* ctx) {
    Type* base = std::any_cast<Type*>(visit(ctx->type()));
    for (auto fieldCtx : ctx->innerField()) {
        Type* copy = base;
        auto [decls, name, offset] =
            std::any_cast<std::tuple<std::vector<PointerDecl>, FullNameV, size_t>>(visit(fieldCtx));

        Variable variable;
        if (std::holds_alternative<Array>(name)) {
            Array const& arr = std::get<Array>(name);
            copy = _types->MakeArray(copy, arr);
            variable._name = arr._name;
        }
        else {
            variable._name = std::get<std::string>(name);
        }

        for (PointerDecl const& decl : decls) {
            copy = _types->MakePointer(copy, decl);
        }


        variable._type = copy;
        variable._offset = offset;
        _currentStruct->_namespace._fields.push_back(variable);
    }

    return 0;
}

std::any Parser::visitInnerField(ZHLParser::InnerFieldContext* ctx) {
    std::vector<PointerDecl> ptrs;
    for (auto decl : ctx->innerFieldPtr()) {
        ptrs.push_back(std::any_cast<PointerDecl>(visit(decl)));
    }

    FullNameV name = std::any_cast<FullNameV>(visit(ctx->fullName()));
    std::string offsetStr(ctx->Number()->getText());
    
    size_t offset = std::strtoull(offsetStr.c_str(), NULL, 0);

    return std::make_tuple(ptrs, name, offset);
}

std::any Parser::visitInnerFieldPtr(ZHLParser::InnerFieldPtrContext* context) {
    PointerDecl decl;
    decl._kind = POINTER;
    decl._const = (context->Const() != nullptr);
    return decl;
}

std::any Parser::visitFullName(ZHLParser::FullNameContext* ctx) {
    FullNameV result;
    std::string name(ctx->Name()->getText());
    if (ctx->RightRBracket()) {
        Array arr;
        arr._name = name;
        std::string number(ctx->Number()->getText());
        // std::cerr << "[INFO] Array length = " << number << std::endl;
        arr._size = std::stoull(number, nullptr, 0);
        result = arr;
    }
    else {
        result = name;
    }

    return result;
}

std::any Parser::visitSignature(ZHLParser::SignatureContext* ctx) {
    SignatureV sig;

    if (auto sigCtx = ctx->Signature()) {
        Signature s;
        s._sig = ReadSignature(sigCtx->getText());
        auto results = std::any_cast<std::tuple<bool, std::string, Function>>(visit(ctx->function()));
        s._function = std::get<Function>(results);
        sig = s;

        if (std::get<bool>(results)) {
            std::string typen(std::get<std::string>(results));
            Type* t = _types->Get(typen);
            if (!t->IsStruct()) {
                Struct s;
                s._name = typen;
                t->_value = s;
            }
            Namespace& nm = std::get<Struct>(t->_value)._namespace;
            nm._signatures.push_back(s);
        }
        else {
            _global->_signatures.push_back(s);
        }

        //std::cout << "Read function signature " << s._sig << " for " << s._function.ToString() << std::endl; // GetCurrentFunctionQualifiedName(s._function._name) << std::endl;
    }
    else {
        VariableSignature s;
        s._sig = ReadSignature(ctx->ReferenceSignature()->getText());
        s._variable = std::any_cast<Variable>(visit(ctx->reference()));
        sig = s;

        _global->_references.push_back(s);
        //std::cout << "Read reference signature " << s._sig << " for variable " << s._variable._name << std::endl;
    }

    return sig;
}

std::any Parser::visitForwardDecl(ZHLParser::ForwardDeclContext* ctx) {
    return 0;
}

std::any Parser::visitTypedef(ZHLParser::TypedefContext* ctx) {
    return 0;
}

std::any Parser::visitFunctionPtr(ZHLParser::FunctionPtrContext* ctx) {
    std::tuple<FunctionPtr*, std::string, size_t> ptr;
    if (ctx->fptr()) {
        ptr = std::any_cast<std::tuple<FunctionPtr*, std::string, size_t>>(visit(ctx->fptr()));
    }
    else {
        ptr = std::any_cast<std::tuple<FunctionPtr*, std::string, size_t>>(visit(ctx->memberPtr()));
    }

    FunctionPtr* fun = std::get<FunctionPtr*>(ptr);
    std::string name = std::get<std::string>(ptr);
    size_t offset = std::get<size_t>(ptr);

    Variable v;
    v._name = name;

    Type* t = _types->Get(fun->GetName());
    t->_value = fun;
    v._type = t;
    v._offset = offset;

    if (_currentStruct) {
        _currentStruct->_namespace._fields.push_back(v);
    }
    else {
        _global->_fields.push_back(v);
    }

    return 0;
}

std::any Parser::visitFptr(ZHLParser::FptrContext* ctx) {
    FunctionPtr* ptr = new FunctionPtr;
    ptr->_ret = std::any_cast<Type*>(visit(ctx->type()));

    if (auto convCtx = ctx->CallingConvention()) {
        std::string convention(ctx->CallingConvention()->getText());
        if (convention != "") {
            ptr->_convention = StringToConvention(convention);
        }
    }

    if (auto argsCtx = ctx->optNamedFunArgs()) {
        ptr->_parameters = std::any_cast<std::vector<Type*>>(visit(argsCtx));
    }

    size_t offset = std::strtoull(ctx->Number()->getText().c_str(), NULL, 0);

    return std::make_tuple(ptr, ctx->Name()->getText(), offset);
}

std::any Parser::visitMemberPtr(ZHLParser::MemberPtrContext* ctx) {
    FunctionPtr* ptr = new FunctionPtr;
    ptr->_ret = std::any_cast<Type*>(visit(ctx->type()));

    if (auto convCtx = ctx->CallingConvention()) {
        std::string convention(ctx->CallingConvention()->getText());
        if (convention != "") {
            ptr->_convention = StringToConvention(convention);
        }
    }

    std::string className(ctx->Name().front()->getText());
    Type* target = _types->Get(className);
    if (!target->IsStruct()) {
        Struct s;
        s._name = className;
        target->_value = s;
    }

    ptr->_scope = target;

    if (auto argsCtx = ctx->optNamedFunArgs()) {
        ptr->_parameters = std::any_cast<std::vector<Type*>>(visit(argsCtx));
    }

    size_t offset = std::strtoull(ctx->Number()->getText().c_str(), NULL, 0);

    return std::make_tuple(ptr, ctx->Name().back()->getText(), offset);
}

std::any Parser::visitInheritance(ZHLParser::InheritanceContext* ctx) {
    Visibility vis = Visibility::PUBLIC;
    std::vector<std::tuple<Type*, Visibility>> parents;
    std::set<std::string> seen;

    for (auto decl : ctx->inheritanceDecl()) {
        auto content = std::any_cast<std::pair<std::vector<std::string>, std::optional<Visibility>>>(visit(decl));
        std::vector<std::string>& first = content.first;
        std::optional<Visibility>& second = content.second;

        if (second) {
            vis = *second;
        }

        if (first.size() > 1) {
            throw std::runtime_error("Nested names not supported");
        }

        std::string name(first.front());
        if (seen.find(name) != seen.end()) {
            ParserLogger()->Warn("%s: structure %s inherits multiple times from structure %s\n",
                _currentFile.c_str(), _currentStruct->_name.c_str(), name.c_str());
            continue;
        }

        seen.insert(name);

        Type* type = _types->Get(name);
        if (!type->IsStruct()) {
            Struct s;
            s._name = name;
            type->_value = s;
        }

        parents.push_back(std::make_tuple(type, vis));
    }

    return parents;
}

std::any Parser::visitInheritanceDecl(ZHLParser::InheritanceDeclContext* ctx) {
    Visibility vis;
    if (auto visibility = ctx->Visibility()) {
        std::string visStr(visibility->getText());
        if (visStr == "public") {
            vis = Visibility::PUBLIC;
        }
        else if (visStr == "private") {
            vis = Visibility::PRIVATE;
        }
        else {
            vis = Visibility::PROTECTED;
        }
    }
    return std::pair<std::vector<std::string>, std::optional<Visibility>>(std::any_cast<std::vector<std::string>>(visit(ctx->nestedName())), ctx->Visibility() ? std::optional<Visibility>(vis) : std::nullopt);
}

std::any Parser::visitTypeInfo(ZHLParser::TypeInfoContext* ctx) {
    struct {
        std::optional<Type*> _type;
        size_t _size = 0;
        size_t _align = 0;
    } typeInfo;

    std::string name(ctx->Name()->getText());
    for (auto infoCtx : ctx->typeInfoDef()) {
        TypeInfoV info = std::any_cast<TypeInfoV>(visit(infoCtx));
        if (std::holds_alternative<Type*>(info)) {
            if (typeInfo._type) {
                _hasFatalError = true;
                ParserLogger()->Error("%s: multiple synonyms specified for type %s\n",
                    _currentFile.c_str(), name.c_str());
            }
            else {
                typeInfo._type = std::get<Type*>(info);
            }
        }
        else {
            TypeInfoData data = std::get<TypeInfoData>(info);
            if (data._tag == TypeInfoTag::SIZE) {
                if (typeInfo._size != 0) {
                    _hasFatalError = true;
                    ParserLogger()->Error("%s: multiple sizes specified for type %s\n",
                        _currentFile.c_str(), name.c_str());
                }
                else {
                    typeInfo._size = data._data;
                }
            }
            else {
                if (typeInfo._align != 0) {
                    _hasFatalError = true;
                    ParserLogger()->Error("%s: multiple alignments specified for type %s\n",
                        _currentFile.c_str(), name.c_str());
                }
                else {
                    typeInfo._align = data._data;
                }
            }
        }
    }

    Type* type = _types->Get(name);
    if (typeInfo._type) {
        type->_synonym = *typeInfo._type;
        type->_value = name;
    }

    if (typeInfo._size != 0) {
        type->_size = typeInfo._size;
    }

    if (typeInfo._align != 0) {
        type->_alignment = typeInfo._align;
    }

    return 0;
}

std::any Parser::visitTypeInfoDef(ZHLParser::TypeInfoDefContext* ctx) {
    TypeInfoV result;
    if (ctx->Size()) {
        std::string sizeStr(ctx->Number()->getText());
        TypeInfoData data;
        data._tag = TypeInfoTag::SIZE;
        data._data = std::strtoull(sizeStr.c_str(), NULL, 0);
        result = data;
    }
    else if (ctx->Synonym()) {
        result = std::any_cast<Type*>(visit(ctx->type()));
    }
    else {
        TypeInfoData data;
        data._tag = TypeInfoTag::ALIGN;
        std::string alignStr(ctx->Number()->getText());
        data._data = std::strtoull(alignStr.c_str(), NULL, 0);
        result = data;
    }

    return result;
}

std::any Parser::visitExternalFunc(ZHLParser::ExternalFuncContext* ctx) {
    ExternalFunction func;
    std::vector<antlr4::tree::TerminalNode*> names = ctx->Name();
    func._dll = names[0]->getText() + ".dll";
    func._name = names[1]->getText();
    auto [nested, first, fn] = std::any_cast<std::tuple<bool, std::string, Function>>(visit(ctx->function()));

    if (!nested) {
        _hasFatalError = true;
        ParserLogger()->Error("%s: external function %s from library %s lacks an englobing namespace\n",
            _currentFile.c_str(), fn._name.c_str(), func._dll.c_str());
        return 0;
    }

    func._namespace = first;
    func._fn = fn;

    _global->_externs.push_back(func);

    return 0;
}

std::string Parser::GetCurrentFunctionQualifiedName(std::string const& name) {
    if (!_currentStruct) {
        return name;
    }
    else {
        return _currentStruct->_name + "::" + name;
    }
}

void Parser::WarnRepeatedFunctionQualifier(std::string const& name, std::string const& qualifier) {
    ParserLogger()->Warn("%s: repeated '%s' qualifier on function %s\n",
        _currentFile.c_str(), qualifier.c_str(), name.c_str());
}

std::string Parser::MergeNameParts(std::vector<std::string> const& parts) {
    std::ostringstream s;
    for (size_t i = 0; i < parts.size(); ++i) {
        s << parts[i];
        if (i != parts.size() - 1) {
            s << "::";
        }
    }
    return s.str();
}

std::string Parser::GetContext() {
    std::ostringstream context;
    if (_currentParam) {
        context << " in parameter " << _currentParam->_name << " ";
        if (_currentFunction) {
            context << " in function " << _currentFunction->_name << " ";
            if (_currentStruct) {
                context << " in structure " << _currentStruct->_name << " ";
            }
        }
    }
    else if (_currentVariable) {
        context << " in variable " << _currentVariable->_name << " ";
        if (_currentStruct) {
            context << " in structure " << _currentStruct->_name << " ";
        }
    }
    else if (_currentFunction) {
        context << " in function " << _currentFunction->_name << " ";
        if (_currentStruct) {
            context << " in structure " << _currentStruct->_name << " ";
        }
    }
    else if (_currentStruct) {
        context << " in structure " << _currentStruct->_name << " ";
    }

    return context.str();
}

void Parser::ReadQualifiers(std::vector<antlr4::tree::TerminalNode*> const& qualifiers, Function& fn) {
    for (antlr4::tree::TerminalNode* x : qualifiers) {
        std::string text = x->getText();
        if (text == "virtual") {
            if (fn.IsVirtual()) {
                WarnRepeatedFunctionQualifier(fn._name, text);
            }
            else {
                fn._qualifiers |= FunctionQualifiers::VIRTUAL;
            }
        }
        else if (text == "static") {
            if (fn.IsStatic()) {
                WarnRepeatedFunctionQualifier(fn._name, text);
            }
            else {
                fn._qualifiers |= FunctionQualifiers::STATIC;
            }
        }
        else if (text == "cleanup") {
            if (fn.IsCleanup()) {
                WarnRepeatedFunctionQualifier(fn._name, text);
            }
            else if (fn._convention) {
                CallingConventions convention = *fn._convention;
                if (convention != CDECL) {
                    ParserLogger()->Error("%s: 'cleanup' specified on non cdecl (%s) function %s\n",
                        _currentFile.c_str(), CallingConventionToString(convention).c_str(),
                        GetCurrentFunctionQualifiedName(fn._name).c_str());
                }
                else {
                    ParserLogger()->Warn("%s: 'cleanup' specified on cdecl function\n",
                        _currentFile.c_str(),
                        GetCurrentFunctionQualifiedName(fn._name).c_str());
                }
            }
            else {
                fn._qualifiers |= FunctionQualifiers::CLEANUP;
            }
        }
        else if (text == "debug") {
            if (fn.IsDebug()) {
                WarnRepeatedFunctionQualifier(fn._name, text);
            }
            else {
                fn._qualifiers |= FunctionQualifiers::DEBUG;
            }
        }
        else if (text == "nohook") {
            if (!fn.CanHook()) {
                WarnRepeatedFunctionQualifier(fn._name, text);
            } else {
                fn._qualifiers |= FunctionQualifiers::NOHOOK;
            }
        }
    }
}

std::string Parser::ReadSignature(const std::string& signature) {
    return signature.substr(1, signature.length() - 3);
}