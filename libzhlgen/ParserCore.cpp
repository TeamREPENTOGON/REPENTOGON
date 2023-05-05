#include "ParserCore.h"

ErrorLogger::~ErrorLogger() {
    if (_filename) {
        std::cerr << "In file " << *_filename << std::endl;
    }
    for (std::string const& err : _errors) {
        std::cerr << "\t" << err << std::endl;
    }
}

void ErrorLogger::SetFile(std::string const& filename) {
    _filename = filename;
}

ErrorLogger& ErrorLogger::endl(ErrorLogger& logger) {
    logger.newline();
    return logger;
}

ErrorLogger& ErrorLogger::error(ErrorLogger& logger) {
    logger.log_error();
    return logger;
}

ErrorLogger& ErrorLogger::warn(ErrorLogger& logger) {
    logger.log_warn();
    return logger;
}

void ErrorLogger::log_error() {
    _buf << "[ERROR] ";
    _errored = true;
}

void ErrorLogger::log_warn() {
    _buf << "[WARN] ";
}

ErrorLogger& ErrorLogger::operator<<(ErrorLogger& (*v)(ErrorLogger&)) {
    return v(*this);
}

ErrorLogger& ErrorLogger::operator<<(EndToken const& token) {
    _errors.push_back(_buf.str());
    _buf.str("");
    _buf.clear();
    return *this;
}

void ErrorLogger::newline() {
    _buf << std::endl;
}

bool ErrorLogger::IsErrored() {
    return _errored;
}

ErrorLogger::EndToken ErrorLogger::_end;

Parser::Parser(Namespace* global, std::map<std::string, Type>* types) : _global(global), _types(types) {

}

std::any Parser::visitZhl(ZHLParser::ZhlContext* ctx) {
    return visitChildren(ctx);
}

std::any Parser::visitFunction(ZHLParser::FunctionContext* ctx) {
    Function fn;
    _currentFunction = &fn;

    std::vector<std::string> parts = std::any_cast<std::vector<std::string>>(visit(ctx->nestedName()));
    fn._name = parts.back(); // MergeNameParts(parts);

    std::optional<std::string> convention;
    if (auto node = ctx->CallingConvention()) {
        convention = node->getText();
        if (convention == "__stdcall") {
            fn._convention = CallingConventions::STDCALL;
        }
        else if (convention == "__fastcall") {
            fn._convention = CallingConventions::FASTCALL;
        }
        else if (convention == "__cdecl") {
            fn._convention = CallingConventions::CDECL;
        }
        else if (convention == "__thiscall") {
            if (!_currentStruct) {
                if (ctx->nestedName()->Name().size() == 1) {
                    _errors << ErrorLogger::error << "__thiscall convention applied to non member function " << fn._name << ErrorLogger::_end;
                    // exit(-1);
                }
                fn._convention = CallingConventions::THISCALL;
            }
            else {
                fn._convention = CallingConventions::THISCALL;
            }
        }
    }

    ReadQualifiers(ctx->Qualifier(), fn);

    fn._ret = std::any_cast<Type*>(visit(ctx->type()));

    if (fn.IsVirtual() && fn.IsStatic()) {
        _errors << ErrorLogger::error << "static and qualifiers applied together to function " << GetCurrentFunctionQualifiedName(fn._name) << ErrorLogger::_end;
        // exit(-1);
    }

    if (fn.IsStatic() && fn._convention && fn._convention == CallingConventions::THISCALL) {
        _errors << ErrorLogger::error << "__thiscall convention applied to static function " << GetCurrentFunctionQualifiedName(fn._name) << ErrorLogger::_end;
        // exit(-1);
    }

    if (auto args = ctx->funArgs()) {
        fn._params = std::any_cast<std::vector<FunctionParam>>(visit(ctx->funArgs()));

        if (fn._convention) {
            for (FunctionParam const& param : fn._params) {
                if (param._reg) {
                    _errors << ErrorLogger::warn << "Calling convention " << CallingConventionToString(*fn._convention) << " specified on function " << GetCurrentFunctionQualifiedName(fn._name) <<
                        ", you cannot specify a register on a parameter (specified " << RegisterToString(*param._reg) << " on parameter " << param._name << ")" << ErrorLogger::_end;
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
                _errors << ErrorLogger::error << "Register " << RegisterToString((Registers)i) << " used on multiple parameters in function " << GetCurrentFunctionQualifiedName(_currentFunction->_name) << ErrorLogger::endl;
                _errors << "\tInfringing parameters are ";
                for (int paramPos : use.second) {
                    FunctionParam const& param = params[paramPos];
                    _errors << param._name << " (param " << i << ")";
                    if (paramPos != use.second.back()) {
                        _errors << ", ";
                    }
                }
                _errors << ErrorLogger::_end;
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
        type = ArrayQualify(type, arr);
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
    return 0;
}

std::any Parser::visitType(ZHLParser::TypeContext* ctx) {
    // The type if never qualified at first
    std::pair<Type*, bool> base = std::any_cast<std::pair<Type*, bool>>(visit(ctx->typeSpecifier()));
    Type* type = base.first;
    if (base.second) {
        type = ConstQualify(type);
    }

    // std::vector<PointerDecl> pointers;
    for (auto ptrCtx : ctx->pointerAttribute()) {
        PointerDecl decl = std::any_cast<PointerDecl>(visit(ptrCtx));
        type = PointerQualify(type, decl);
    }

    return type;
}

std::any Parser::visitTypeSpecifier(ZHLParser::TypeSpecifierContext* ctx) {
    Type* result = nullptr;

    auto cstQual = ctx->Const();
    bool isConst = !cstQual.empty();

    if (cstQual.size() == 2) {
        _errors << ErrorLogger::warn << "const used multiple times " << GetContext() << ErrorLogger::_end;
    }

    if (auto simple = ctx->simpleType()) {
        // result._value = std::any_cast<BasicType>(visit(simple));
        BasicType basic = std::any_cast<BasicType>(visit(simple));
        // std::cout << "Found basic type " << basic.GetAbsoluteName() << std::endl;
        result = GetOrCreateTypeByName(basic.GetAbsoluteName());
        result->_value = basic;
    }
    else {
        // std::string name = MergeNameParts(std::any_cast<std::vector<std::string>>(visit(ctx->nestedName())));
        // Struct* target = GetStructByName(name);
        // result._value = name;
        // result._name = target;

        std::vector<std::string> parts = std::any_cast<std::vector<std::string>>(visit(ctx->nestedName()));
        result = GetOrCreateTypeByName(parts.front());

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
            _errors << ErrorLogger::warn << "Cannot specify sign modifier (" << sign->getText() << ") on type " << BasicTypeToString(basic._type) << GetContext() << ErrorLogger::_end;
        }
        else {
            basic._sign = std::any_cast<Signedness>(visit(sign));
        }
    }
    else {
        if (basic._type == BasicTypes::CHAR || basic._type == BasicTypes::INT) {
            basic._sign = Signedness::SIGNED;
        }
    }

    auto length = ctx->simpleTypeLength();
    if (length.size()) {
        if (basic._type != BasicTypes::DOUBLE && basic._type != BasicTypes::INT) {
            _errors << ErrorLogger::warn << "Cannot specify length modifier on type " << BasicTypeToString(basic._type) << GetContext() << ErrorLogger::_end;
        }
        else {
            if (length.size() == 1) {
                basic._length = std::any_cast<Length>(visit(length.front()));
            }
            else {
                Length first = std::any_cast<Length>(visit(length[0]));
                Length second = std::any_cast<Length>(visit(length[1]));

                if (first != second) {
                    _errors << ErrorLogger::error << "Cannot specify short and long simultaneously on type " << BasicTypeToString(basic._type) << GetContext() << ErrorLogger::_end;
                }
                else if (first == Length::SHORT) {
                    _errors << ErrorLogger::warn << "short specified multiple times on type " << BasicTypeToString(basic._type) << GetContext() << ErrorLogger::_end;
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
    std::vector<antlr4::tree::TerminalNode*> parts = ctx->Name();
    std::vector<std::string> partsStr;
    for (auto x : parts)
        partsStr.push_back(x->toString());
    std::string typen = partsStr[0];
    // std::cout << "Nested name " << type << std::endl;

    if (partsStr.size() > 1) {
        if (auto it = _types->find(typen); it == _types->end()) {
            Type* type = GetOrCreateTypeByName(typen);
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
    Type* type = GetOrCreateTypeByName(name);
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

    return 0;
}

std::any Parser::visitDepends(ZHLParser::DependsContext* ctx) {
    std::set<Type*> deps;
    auto names = ctx->Name();

    for (int i = 0; i < names.size(); ++i) {
        std::string name(names[i]->getText());
        // Struct* st = GetStructByName(name);
        Type* t = GetOrCreateTypeByName(name);
        if (!t->IsStruct()) {
            Struct s;
            s._name = name;
            t->_value = s;
        }

        if (deps.find(t) != deps.end()) {
            _errors << ErrorLogger::warn << "Specified " << name << " as dependency for " << _currentStruct->_name << " multiple times " << ErrorLogger::_end;
        }
        else if (name == _currentStruct->_name) {
            _errors << ErrorLogger::warn << "Specified " << name << " as self dependency " << ErrorLogger::_end;
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
        _errors << ErrorLogger::warn << "Multiple vtable fields in structure " << _currentStruct->_name << ", keeping only the first" << ErrorLogger::endl << ErrorLogger::_end;
        visit(vtablesCtx.front());
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
    std::string sig(ctx->Signature()->getText());
    Function function = std::any_cast<Function>(visit(ctx->classFunction()));
    Signature signature;
    signature._sig = sig;
    signature._function = function;
    return signature;
}

std::any Parser::visitClassFunction(ZHLParser::ClassFunctionContext* ctx) {
    Function res;
    _currentFunction = &res;
    res._name = ctx->Name()->getText();
    ReadQualifiers(ctx->Qualifier(), res);
    res._ret = std::any_cast<Type*>(visit(ctx->type()));
    res._params = std::any_cast<std::vector<FunctionParam>>(visit(ctx->funArgs()));
    res._kind = FunctionKind::METHOD;

    _currentFunction = nullptr;
    return res;
}

std::any Parser::visitClassField(ZHLParser::ClassFieldContext* ctx) {
    Type* base = std::any_cast<Type*>(visit(ctx->type()));
    for (auto fieldCtx : ctx->innerField()) {
        Type* copy = base;
        auto [decls, name] =
            std::any_cast<std::tuple<std::vector<PointerDecl>, FullNameV>>(visit(fieldCtx));

        Variable variable;
        if (std::holds_alternative<Array>(name)) {
            Array const& arr = std::get<Array>(name);
            copy = ArrayQualify(copy, arr);
            variable._name = arr._name;
        }
        else {
            variable._name = std::get<std::string>(name);
        }

        for (PointerDecl const& decl : decls) {
            copy = PointerQualify(copy, decl);
        }


        variable._type = copy;
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

    return std::make_tuple(ptrs, name);
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
        std::stringstream s;
        s << ctx->Number()->getText();
        s >> arr._size;
        result = arr;
    }
    else {
        result = name;
    }

    return result;
}

std::any Parser::visitSignature(ZHLParser::SignatureContext* ctx) {
    std::string fullSignature;
    SignatureV sig;

    if (auto sigCtx = ctx->Signature()) {
        Signature s;
        fullSignature = sigCtx->getText();
        s._sig = fullSignature.substr(1, fullSignature.size() - 3);
        auto results = std::any_cast<std::tuple<bool, std::string, Function>>(visit(ctx->function()));
        s._function = std::get<Function>(results);
        sig = s;

        if (std::get<bool>(results)) {
            std::string typen(std::get<std::string>(results));
            Type* t = GetOrCreateTypeByName(typen);
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
        fullSignature = ctx->ReferenceSignature()->getText();
        s._sig = fullSignature.substr(1, fullSignature.size() - 3);
        s._variable = std::any_cast<Variable>(visit(ctx->reference()));

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
    std::tuple<FunctionPtr*, std::string> ptr;
    if (ctx->fptr()) {
        ptr = std::any_cast<std::tuple<FunctionPtr*, std::string>>(visit(ctx->fptr()));
    }
    else {
        ptr = std::any_cast<std::tuple<FunctionPtr*, std::string>>(visit(ctx->memberPtr()));
    }

    FunctionPtr* fun = std::get<FunctionPtr*>(ptr);
    std::string name = std::get<std::string>(ptr);

    Variable v;
    v._name = name;

    Type* t = GetOrCreateTypeByName(fun->GetName());
    t->_value = fun;
    v._type = t;

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

        if (convention == "__stdcall") {
            ptr->_convention = CallingConventions::STDCALL;
        }
        else if (convention == "__fastcall") {
            ptr->_convention = CallingConventions::FASTCALL;
        }
        else if (convention == "__cdecl") {
            ptr->_convention = CallingConventions::CDECL;
        }
        else if (convention == "__thiscall") {
            ptr->_convention = CallingConventions::THISCALL;
        }
    }


    if (auto argsCtx = ctx->optNamedFunArgs()) {
        ptr->_parameters = std::any_cast<std::vector<Type*>>(visit(argsCtx));
    }

    return std::make_tuple(ptr, ctx->Name()->getText());
}

std::any Parser::visitMemberPtr(ZHLParser::MemberPtrContext* ctx) {
    FunctionPtr* ptr = new FunctionPtr;
    ptr->_ret = std::any_cast<Type*>(visit(ctx->type()));

    if (auto convCtx = ctx->CallingConvention()) {
        std::string convention(ctx->CallingConvention()->getText());

        if (convention == "__stdcall") {
            ptr->_convention = CallingConventions::STDCALL;
        }
        else if (convention == "__fastcall") {
            ptr->_convention = CallingConventions::FASTCALL;
        }
        else if (convention == "__cdecl") {
            ptr->_convention = CallingConventions::CDECL;
        }
        else if (convention == "__thiscall") {
            ptr->_convention = CallingConventions::THISCALL;
        }
    }

    std::string className(ctx->Name().front()->getText());
    Type* target = GetOrCreateTypeByName(className);
    if (!target->IsStruct()) {
        Struct s;
        s._name = className;
        target->_value = s;
    }

    ptr->_scope = target;

    if (auto argsCtx = ctx->optNamedFunArgs()) {
        ptr->_parameters = std::any_cast<std::vector<Type*>>(visit(argsCtx));
    }

    return std::make_tuple(ptr, ctx->Name().back()->getText());
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
            _errors << ErrorLogger::warn << "Structure " << _currentStruct->_name << " specifies base class " << name << " multiple times " << ErrorLogger::endl << ErrorLogger::_end;
            continue;
        }

        seen.insert(name);

        Type* type = GetOrCreateTypeByName(name);
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
    } typeInfo;

    std::string name(ctx->Name()->getText());
    for (auto infoCtx : ctx->typeInfoDef()) {
        TypeInfoV info = std::any_cast<TypeInfoV>(visit(infoCtx));
        if (std::holds_alternative<Type*>(info)) {
            if (typeInfo._type) {
                _errors << ErrorLogger::error << "Multiple synonyms specified for type " << name << ErrorLogger::endl << ErrorLogger::_end;
            }
            else {
                typeInfo._type = std::get<Type*>(info);
            }
        }
        else {
            if (typeInfo._size != 0) {
                _errors << ErrorLogger::error << "Multiple sizes specified for type " << name << ErrorLogger::endl << ErrorLogger::_end;
            }
            else {
                typeInfo._size = std::get<size_t>(info);
            }
        }
    }

    Type* type = GetOrCreateTypeByName(name);
    if (typeInfo._type) {
        type->_synonym = *typeInfo._type;
    }

    if (typeInfo._size != 0) {
        type->_size = typeInfo._size;
    }

    return 0;
}

std::any Parser::visitTypeInfoDef(ZHLParser::TypeInfoDefContext* ctx) {
    TypeInfoV result;
    if (ctx->Size()) {
        std::stringstream s;
        s << ctx->Number()->getText();
        size_t size;
        s >> size;
        result = size;
    }
    else {
        result = std::any_cast<Type*>(visit(ctx->type()));
    }

    return result;
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
    _errors << ErrorLogger::warn << "Repeated '" << qualifier << "' qualifier on function " << name << ErrorLogger::endl << ErrorLogger::_end;
}

std::string Parser::MergeNameParts(std::vector<std::string> const& parts) {
    std::ostringstream s;
    for (int i = 0; i < parts.size(); ++i) {
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

Type* Parser::GetOrCreateTypeByName(std::string const& name) {
    if (auto it = _types->find(name); it != _types->end()) {
        return &(it->second);
    }
    else {
        Type t;
        t._name = name;
        auto iter = _types->insert(std::make_pair(name, t));
        return &(iter.first->second);
    }
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
            if (fn.IsCleanup() && !fn._convention) {
                WarnRepeatedFunctionQualifier(fn._name, text);
            }
            else if (fn._convention) {
                CallingConventions convention = *fn._convention;
                if (convention != CDECL) {
                    _errors << ErrorLogger::error << "cleanup specified on non cdecl (" << CallingConventionToString(convention) << ") function " << GetCurrentFunctionQualifiedName(fn._name) << ErrorLogger::_end;
                }
                else {
                    _errors << ErrorLogger::warn << "cleanup specified on cdecl function " << GetCurrentFunctionQualifiedName(fn._name) << ErrorLogger::_end;
                }
            }
            else {
                fn._qualifiers |= FunctionQualifiers::CLEANUP;
            }
        }
    }
}

Type* Parser::PointerQualify(Type const* source, PointerDecl const& decl) {
    Type tmp = *source;
    tmp._pointerDecl.push_back(decl);
    std::string name(tmp.GetFullName());
    // std::cout << "Adding pointer: " << source->GetFullName() << " => " << name << std::endl;
    Type* qualified = GetOrCreateTypeByName(name);
    if (!qualified->_base) {
        qualified->_base = (Type*)source;
        qualified->_pointerDecl.push_back(decl);
        qualified->_name = source->_name;
    }

    return qualified;
}

Type* Parser::ArrayQualify(Type const* source, Array const& array) {
    Type tmp = *source;
    tmp._array = true;
    tmp._arraySize = array._size;

    std::string name(tmp.GetFullName());
    Type* qualified = GetOrCreateTypeByName(name);
    if (!qualified->_base) {
        qualified->_base = (Type*)source;
        qualified->_array = true;
        qualified->_arraySize = array._size;
        qualified->_name = source->_name;
    }

    return qualified;
}

Type* Parser::ConstQualify(Type const* source) {
    Type tmp = *source;
    tmp._const = true;

    std::string name(tmp.GetFullName());
    Type* qualified = GetOrCreateTypeByName(name);
    if (!qualified->_base) {
        qualified->_base = (Type*)source;
        qualified->_name = source->_name;
        qualified->_const = true;
    }

    return qualified;
}