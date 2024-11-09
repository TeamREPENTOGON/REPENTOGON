#include <algorithm>
#include <iostream>
#include <sstream>

#include "ParserDefinitions.h"

std::string PointerKindToString(PointerKind kind) {
    switch (kind) {
    case LREF:
        return "&";

    case RREF:
        return "&&";

    case POINTER:
        return "*";
    }

    return "";
}

std::string PointerDecl::ToString() const {
    std::ostringstream str;
    if (_const) {
        str << "const";
    }
    str << PointerKindToString(_kind);

    return str.str();
}

std::string PointerDecl::GetPrefix() const {
    std::ostringstream str;
    //str << "!";
    switch (_kind) {
    case LREF:
        if (_const) {
            str << "L";
        }
        else {
            str << "l";
        }
        break;

    case RREF:
        if (_const) {
            str << "R";
        }
        else {
            str << "r";
        }
        break;

    case POINTER:
        if (_const) {
            str << "P";
        }
        else {
            str << "p";
        }
        break;
    }

    //str << "!";
    return str.str();
}

std::string BasicTypeToString(BasicTypes type) {
    switch (type) {
    case CHAR:
        return "char";

    case INT:
        return "int";

    case BOOL:
        return "bool";

    case VOID:
        return "void";

    case FLOAT:
        return "float";

    case DOUBLE:
        return "double";

    default:
        throw std::runtime_error("Invalid basic type");
    }
}

std::string BasicType::ToString() const {
    std::ostringstream res;
    if (_sign) {
        switch (*_sign) {
        case SIGNED:
            res << "signed ";
            break;

        case UNSIGNED:
            res << "unsigned ";
            break;
        }
    }
    else {
        if (_type == BasicTypes::INT || _type == BasicTypes::CHAR) {
            res << "signed ";
        }
    }

    if (_length) {
        switch (*_length) {
        case LONG:
            res << "long ";
            break;

        case LONGLONG:
            res << "long long ";
            break;

        case SHORT:
            res << "short ";
        }
    }

    res << BasicTypeToString(_type);

    return res.str();
}

std::string BasicType::GetAbsoluteName() const {
    std::ostringstream str;

    if (_sign) {
        switch (*_sign) {
        case SIGNED:
            str << "s";
            break;

        case UNSIGNED:
            str << "u";
        }
    }
    else {
        if (_type == INT || _type == CHAR) {
            str << "s";
        }
    }

    switch (_type) {
    case FLOAT:
        str << "f";
        break;

    case DOUBLE:
        str << "d";
        break;

    case INT:
        str << "i";
        break;

    case CHAR:
        str << "c";
        break;

    case BOOL:
        str << "b";
        break;

    case VOID:
        str << "v";
    }

    if (_length) {
        switch (*_length) {
        case LONG:
            if (_type == INT) {
                str << "32";
            }
            else {
                str << "80";
            }
            break;

        case SHORT:
            str << "16";
            break;

        case LONGLONG:
            str << "64";
            break;
        }
    }

    return str.str();
}

size_t BasicType::size() const {
    switch (_type) {
    case CHAR:
        return 1;

    case INT:
        if (_length) {
            switch (*_length) {
            case SHORT:
                return 2;

            case LONG:
                return 4;

            case LONGLONG:
                return 8;
            }
        }
        else {
            return 4;
        }

    case FLOAT:
        return 4;

    case DOUBLE:
        if (_length) {
            switch (*_length) {
            case LONG:
                return 12;

            default:
                return 8;
            }
        }
        else {
            return 8;
        }

    case BOOL:
        return 1;

    case VOID:
        return 0;

    default:
        throw std::runtime_error("Non existant basic type");
    }
}

Type::~Type() {
    if (std::holds_alternative<FunctionPtr*>(_value)) {
        // delete std::get<FunctionPtr*>(_name);
        _value = (FunctionPtr*)0;
    }
}

bool Type::IsPointer() const {
    return !_pointerDecl.empty() || IsFunctionPointer();
}

bool Type::IsEmpty() const {
    return std::holds_alternative<EmptyType>(_value);
}

bool Type::IsArray() const {
    return _array;
}

bool Type::IsStruct() const {
    return std::holds_alternative<Struct>(_value);
}

bool Type::IsBasic() const {
    return std::holds_alternative<BasicType>(_value);
}

bool Type::IsResolved() const {
    if (std::holds_alternative<std::string>(_value)) {
        return _synonym != nullptr || _size;
    }
    else {
        return !IsEmpty();
    }
    // return !std::holds_alternative<std::string>(_value) && !IsEmpty();
}

bool Type::IsFunctionPointer() const {
    return std::holds_alternative<FunctionPtr*>(_value);
}

Struct const& Type::GetStruct() const {
    return std::get<Struct>(_value);
}

Type* Type::GetTrueType() {
    if (!_synonym) {
        return this;
    }
    else {
        return _synonym->GetTrueType();
    }
}

std::string Type::GetPrefix() const {
    std::ostringstream str;
    str << "!!";

    if (_pointerDecl.size() != 0) {
        // std::ostringstream str;
        for (PointerDecl const& decl : _pointerDecl) {
            str << decl.GetPrefix();
        }
    }

    // str << "!!";
    if (_const) {
        str << "C";
    }

    if (_array) {
        str << "a" << _arraySize;
    }

    str << "!!";
    return str.str();
}

std::string Type::GetFullName() const {
    if (_synonym) {
        return _synonym->GetFullName();
    }

    std::ostringstream str;
    // str << "Prefix: " << GetPrefix() << "; name: " << _name;
    str << GetPrefix() << _name;
    if (_name == "") {
        throw std::runtime_error("Empty name");
    }
    return str.str();
}

std::string Type::ToString(bool full) const {
    std::ostringstream res;
    res << "[" << GetFullName() << "] ";
    if (_const) {
        res << "const ";
    }

    if (!_base) {
        if (std::holds_alternative<BasicType>(_value)) {
            res << std::get<BasicType>(_value).ToString();
        }
        else if (std::holds_alternative<Struct>(_value)) {
            res << std::get<Struct>(_value).ToString(full);
        }
        else if (std::holds_alternative<FunctionPtr*>(_value)) {
            res << std::get<FunctionPtr*>(_value)->ToString();
        }
        else {
            if (_synonym) {
                if (!std::holds_alternative<std::string>(_value)) {
                    std::cerr << "Type " << GetFullName() << " is a synonym that doesn't hold a name, somehow" << std::endl;
                }
                res << std::get<std::string>(_value) << " => " << _synonym->ToString(false);
            }
            else {
                if (_size) {
                    res << std::get<std::string>(_value) << " (size " << *_size << ")";
                }
                else {
                    res << "(unresolved) " << std::get<std::string>(_value) << "(" << this << ")";
                }
            }
            // res << "(unresolved) " << std::get<std::string>(_value) << "(" << this << ")";
        }

        if (_array) {
            res << "[" << _arraySize << "] ";
        }
    }
    else {
        if (_pointerDecl.size() > 0) {
            res << _pointerDecl.front().ToString() << " ";
        }

        res << _base->ToString(false);
    }

    // for (PointerDecl const& decl: _pointerDecl) {
    //    res << decl.ToString() << " ";
    // }

    return res.str();
}

class InvalidTypeSizeException : public std::exception {
public:
    virtual const char* what() const override {
        return _str.str().c_str();
    }

    std::ostringstream& get() {
        return _str;
    }

private:
    std::ostringstream _str;
};

size_t Type::size() const {
    // Array applies a multiplier before everything else
    if (_array) {
        return _arraySize * _base->size();
    }

    // Pointer overrides everything else
    if (_pointerDecl.size() > 0) {
        return 4;
    }

    if (_size) {
        return *_size;
    }

    if (_const) {
        return _base->size();
    }

    if (std::holds_alternative<EmptyType>(_value)) {
        InvalidTypeSizeException ex;
        ex.get() << "[FATAL] Type " << _name << " holds no type" << std::endl;
        throw ex;
    }
    else if (std::holds_alternative<std::string>(_value)) {
        if (_synonym) {
            return _synonym->size();
        }
        else {
            InvalidTypeSizeException ex;
            ex.get() << "[FATAL] Type " << _name << " (internally " << std::get<std::string>(_value) << ") holds no type" << std::endl;
            throw ex;
        }
    }
    else if (std::holds_alternative<BasicType>(_value)) {
        return std::get<BasicType>(_value).size();
    }
    else if (std::holds_alternative<FunctionPtr*>(_value)) {
        return 4;
    } 
    else {
        return std::get<Struct>(_value).size();
    }
}

size_t Type::alignment() const {
    if (_alignment)
        return *_alignment;

    Type* type = const_cast<Type*>(this)->GetTrueType();
    if (type->_alignment)
        return *type->_alignment;

    if (type->IsPointer()) {
        return 4;
    }
    else if (type->IsBasic()) {
        return type->size();
    }
    else if (type->IsStruct()) {
        // The alignment of a structure is recursively defined as the highest 
        // alignment constraint of its attributes. 
        //
        // Basic types (including pointers) have clearly defined alignment
        // constraints (see above).
        // When processing a structure, explode its internal structures as 
        // a collection of basic types.

        // Start by iterating over the fields of the struct itself
        size_t maxAlign = 0;
        for (Variable const& var : type->GetStruct()._namespace._fields) {
            // This cannot trigger an infinite recursion. If I start computing the 
            // alignment of the current type again, then it means there exists 
            // a dependency path in the code that leads back here, therefore 
            // the structure has an infinite size and the program is ill-formed.
            size_t align = var._type->alignment();
            if (align > maxAlign) {
                maxAlign = align;
            }

            // Under MSVC x86, the maximum alignment constraint is 8 (long long and 
            // long double)
            if (maxAlign == 8)
                return maxAlign;
        }

        for (std::tuple<Type*, Visibility> const& dep : type->GetStruct()._parents) {
            size_t align = std::get<Type*>(dep)->alignment();
            if (align > maxAlign) {
                maxAlign = align;
            }

            if (maxAlign == 8)
                return maxAlign;
        }

        return maxAlign;
    }
    else if (_array) {
        return _base->alignment();
    } else {
        throw std::runtime_error("Unable to compute alignment constraint");
        return 0;
    }
}

std::string Struct::ToString(bool full) const {
    std::ostringstream str;
    if (full) {
        str << "Structure " << _name << std::endl;
        str << "\tSignatures: " << std::endl;
        for (Signature const& sig : _namespace._signatures) {
            str << "sig = " << sig._sig << ", fn = " << sig._function.ToString() << std::endl;
        }
        str << "\tFields: " << std::endl;
        for (Variable const& variable : _namespace._fields) {
            str << variable._type->ToString(false) << " " << variable._name << " " << *variable._offset << std::endl;
        }
    }
    else {
        str << _name << " ";
    }
    return str.str();
}

size_t Struct::size() const {
    if (_size)
        return *_size;

    size_t s = 0;
    std::for_each(_namespace._fields.begin(), _namespace._fields.end(), [&s](Variable const& var) -> void { s += var._type->size();  });
    return s;
}

std::string Struct::GetTemplateName() const {
    if (!_template) {
        return _name;
    }

    std::ostringstream result;
    result << _name << "<";
    for (size_t i = 0; i < _templateParams.size(); ++i) {
        result << _templateParams[i]->_name;
        if (i != _templateParams.size() - 1) {
            result << ", ";
        }
    }
    result << ">";
    return result.str();
}

uint32_t Struct::GetVirtualFunctionSlotInternal(Signature const* signature) const {
    uint32_t result = 0;
    for (auto const& fn : _virtualFunctions) {
        if (std::holds_alternative<Signature>(fn)) {
            Signature const& sig = std::get<Signature>(fn);
            if (sig == *signature) {
                return result;
            }
        }
        else if (std::holds_alternative<Function>(fn)) {
            Function const& func = std::get<Function>(fn);

            if (func == signature->_function) {
                return result;
            }
        }

        result++;
    }

    throw std::runtime_error("Virtual function not registered");
    return 0;
}

uint32_t Struct::GetVirtualFunctionSlot(Signature const* signature, bool checkParent) const {
    if (_parents.size() > 1) {
        throw std::runtime_error("I don't want to implement multiple inheritance, are you sure there is no other way to design this ?");
    }

    if (_parents.size() == 0) {
        return GetVirtualFunctionSlotInternal(signature);
    }
    else {
        if (checkParent) {
            auto [parent, sig] = GetVirtualFunctionSource(signature->_function);
            if (!parent) {
                /* If parent and sig are both null, we are on a declaration of a pure function 
                 * that corresponds to nothing in the parents. Therefore we can simply get 
                 * its offset in the vtable of the current type.
                 */
                uint32_t result = std::get<Type*>(_parents.front())->GetStruct().GetNbVirtualFunctions();
                return result + GetVirtualFunctionSlotInternal(signature);
            }
            else {
                /* If sig is null, create a fake signature that contains the concrete version of 
                 * the function, so GetVirtualFunctionSlotInternal can later check the 
                 * functions in the vtable against something.
                 */
                if (!sig) {
                    Signature dummy;
                    dummy._sig = "";
                    dummy._function = signature->_function;
                    return parent->GetVirtualFunctionSlot(&dummy, false);
                }
                return parent->GetVirtualFunctionSlot(sig, false);
            }
        }
        else {
            uint32_t result = std::get<Type*>(_parents.front())->GetStruct().GetNbVirtualFunctions();
            return result + GetVirtualFunctionSlotInternal(signature);
        }
    }
}

uint32_t Struct::GetVirtualFunctionSlot(Function const* fn) const {
    if (_parents.size() > 1) {
        throw std::runtime_error("Multiple inheritance not supported");
    }

    uint32_t result = 0;
    if (_parents.size() == 1) {
        result = std::get<Type*>(_parents.front())->GetStruct().GetNbVirtualFunctions();
    }
    
    for (auto const& fun : _virtualFunctions) {
        if (!std::holds_alternative<Function>(fun)) {
            ++result;
            continue;
        }

        if (std::get<Function>(fun) == *fn) {
            return result;
        }
        else {
            ++result;
        }
    }

    throw std::runtime_error("Function not found");
    return 0;
}

uint32_t Struct::GetNbVirtualFunctions() const {
    if (_parents.size() > 1) {
        throw std::runtime_error("Multiple inheritance not supported");
    }

    uint32_t count = _virtualFunctions.size();
    if (_parents.size() == 1) {
        count += std::get<Type*>(_parents.front())->GetStruct().GetNbVirtualFunctions();
    }

    return count;
}

std::tuple<Struct*, Signature*> Struct::GetVirtualFunctionSource(Function const& fn) const {
    if (_parents.size() > 1) {
        throw std::runtime_error("Multiple inheritance not supported");
    }

    std::cout << "GetVirtualFunctionSource " << _name << "::" << fn._name << std::endl;
    Struct* resStruct = nullptr;
    Signature* resSignature = nullptr;
    if (_parents.size() == 1) {
        Struct const& parent = std::get<Type*>(_parents.front())->GetStruct();
        std::cout << "GetVirtualFunctionSource " << _name << "::" << fn._name << ", going for parent " << parent._name << std::endl;
        std::tie(resStruct, resSignature) = parent.GetVirtualFunctionSource(fn);
    }

    if (!resStruct) {
        for (auto const& function : _virtualFunctions) {
            if (std::holds_alternative<Skip>(function)) {
                continue;
            }

            if (std::holds_alternative<Function>(function)) {
                if (std::get<Function>(function) == fn) {
                    resStruct = const_cast<Struct*>(this);
                    resSignature = nullptr;
                    break;
                }
            }
            else {
                if (std::get<Signature>(function)._function == fn) {
                    resStruct = const_cast<Struct*>(this);
                    resSignature = const_cast<Signature*>(&(std::get<Signature>(function)));
                    break;
                }
            }
        }
    }

    return std::make_tuple(resStruct, resSignature);
}

std::string RegisterToString(Registers reg) {
    switch (reg) {
    case EAX:
        return "eax";

    case EBX:
        return "ebx";

    case ECX:
        return "ecx";

    case EDX:
        return "edx";

    case EDI:
        return "edi";

    case ESI:
        return "esi";

    case ESP:
        return "esp";

    case EBP:
        return "ebp";

    case XMM0:
    case XMM1:
    case XMM2:
    case XMM3:
    case XMM4:
    case XMM5:
    case XMM6:
    case XMM7:
        return std::string("xmm") + std::to_string(reg - XMM0);

    default: {
        std::ostringstream err;
        err << "Invalid register ID " << reg;
        throw std::runtime_error(err.str());
    }
    }
}

std::string CallingConventionToString(CallingConventions convention) {
    switch (convention) {
    case CDECL:
        return "__cdecl";

    case STDCALL:
        return "__stdcall";

    case FASTCALL:
        return "__fastcall";

    case THISCALL:
        return "__thiscall";

    case X86_64:
    case X86_64_OUTPUT:
        return "";

    default:
        throw std::runtime_error("Invalid calling convention");
    }
}

std::string FunctionParam::ToString() const {
    std::ostringstream s;
    s << _type->ToString(false) << " " << _name;
    if (_reg) {
        s << "<" << RegisterToString(*_reg) << ">";
    }

    return s.str();
}

bool Function::IsVirtual() const {
    return _qualifiers & VIRTUAL || _qualifiers & PURE;
}

bool Function::IsCleanup() const {
    return _qualifiers & CLEANUP || (_convention && (*_convention == CDECL || *_convention == X86_64 || *_convention == X86_64_OUTPUT));
}

bool Function::IsStatic() const {
    return _qualifiers & STATIC;
}

bool Function::IsPure() const {
    return _qualifiers & PURE;
}

bool Function::IsDebug() const {
    return _qualifiers & DEBUG;
}

std::string Function::ToString() const {
    std::ostringstream str;
    str << "Function " << _name << std::endl;
    str << "\tqualifiers = ";
    if (_qualifiers == 0) {
        str << "none";
    }
    else {
        if (IsDebug()) {
            str << "debug ";
        }

        if (IsVirtual()) {
            str << "virtual ";
        }
        else if (IsStatic()) {
            str << "static ";
        }

        if (IsCleanup()) {
            str << "cleanup ";
        }
    }
    str << std::endl;
    str << "\ttype = " << _ret->ToString(false) << std::endl;
    if (_convention) {
        str << "\tconvention = " << CallingConventionToString(*_convention) << std::endl;
    }
    str << "\tparams = " << std::endl;
    for (FunctionParam const& param : _params) {
        str << "\t\t" << param.ToString() << std::endl;
    }

    return str.str();
}

std::string FunctionPtr::GetName() const {
    std::ostringstream stream;
    stream << _ret->GetFullName() << "(";
    if (_scope) {
        stream << _scope->GetFullName() << "::";
    }
    stream << "*)(";

    for (size_t i = 0; i < _parameters.size(); ++i) {
        stream << _parameters[i]->GetFullName();
        if (i != _parameters.size() - 1) {
            stream << "|";
        }
    }

    stream << ")";

    return stream.str();
}

std::string FunctionPtr::ToString() const {
    std::ostringstream str;
    str << _ret->ToString(false) << "(";

    if (_convention) {
        str << *_convention << " ";
    }

    if (_scope) {
        str << _scope->ToString(false) << "::";
    }

    str << "*)(";

    for (size_t i = 0; i < _parameters.size(); ++i) {
        str << _parameters[i]->ToString(false);

        if (i != _parameters.size() - 1) {
            str << ", ";
        }
    }

    str << ")";

    return str.str();
}

bool Function::operator==(const Function& other) const {
    if (&other == this) {
        return true;
    }

    if (_name != other._name) {
        return false;
    }

    if (_convention) {
        if (other._convention) {
            if (*_convention != *other._convention) {
                return false;
            }
        }
        else {
            return false;
        }
    }
    else {
        if (other._convention) {
            return false;
        }
    }

    if (*_ret != *other._ret) {
        return false;
    }

    if (_params.size() != other._params.size()) {
        return false;
    }

    for (size_t i = 0; i < _params.size(); ++i) {
        if (_params[i] != other._params[i]) {
            return false;
        }
    }

    return true;
}

bool Function::operator!=(const Function& other) const {
    return !(*this == other);
}

bool Type::operator==(const Type& other) const {
    if (this == &other) {
        return true;
    }
    
    if (GetFullName() == other.GetFullName()) {
        return true;
    }

    return false;
}

bool Type::operator!=(const Type& other) const {
    return !(*this == other);
}

bool FunctionParam::operator==(const FunctionParam& other) const {
    if (this == &other) {
        return true;
    }

    if (*_type != *other._type) {
        return false;
    }

    if (_reg) {
        if (!other._reg) {
            return false;
        }
        else {
            return *_reg == *other._reg;
        }
    }
    else {
        if (other._reg) {
            return false;
        }
    }

    return true;
}

bool FunctionParam::operator!=(const FunctionParam& other) const {
    return !(*this == other);
}

std::string VisibilityToString(Visibility v) {
    switch (v) {
    case PUBLIC:
        return "public";

    case PRIVATE:
        return "private";

    case PROTECTED:
        return "protected";
    }

    return "??visibility??";
}

std::string LengthToString(Length length) {
    switch (length) {
    case LONGLONG:
        return "long long";

    case LONG:
        return "long";

    case SHORT:
        return "short";
    }

    return "??length??";
}

std::string SignednessToString(Signedness sign) {
    switch (sign) {
    case SIGNED:
        return "signed";

    case UNSIGNED:
        return "unsigned";
    }

    return "??signdeness??";
}

bool Signature::operator==(Signature const& other) const {
    if (&other == this)
        return true;

    return _sig == other._sig;
}

bool Variable::operator<(Variable const& rhs) const {
    int64_t l = -1, r = -1;
    if (_offset)
        l = *_offset;

    if (rhs._offset)
        r = *rhs._offset;

    return l < r;
}

CallingConventions StringToConvention(std::string const& convention) {
    if (convention == "__stdcall") {
        return CallingConventions::STDCALL;
    }
    else if (convention == "__fastcall") {
        return CallingConventions::FASTCALL;
    }
    else if (convention == "__cdecl") {
        return CallingConventions::CDECL;
    }
    else if (convention == "__thiscall") {
        return CallingConventions::THISCALL;
    }
    else if (convention == "__x86_64") {
        return CallingConventions::X86_64;
    }
    else if (convention == "__x86_64_output") {
        return CallingConventions::X86_64_OUTPUT;
    }
    else {
        std::ostringstream err;
        err << "Invalid convention kill me: " << convention << std::endl;
        std::cerr << "SHUT UP: " << convention << std::endl;
        throw std::runtime_error(err.str());
    }
}

bool FunctionParam::IsX8664Valid(size_t position) const {
    if (position >= 4) {
        return false;
    }

    if (!_type->IsBasic() && !_type->IsPointer()) {
        return false;
    }

    if (_type->IsPointer()) {
        return position < 2;
    }

    BasicType basic = std::get<BasicType>(_type->_value);
    if (basic._type == BasicTypes::INT || basic._type == BasicTypes::BOOL || basic._type == BasicTypes::CHAR) {
        return position < 2;
    }
    else {
        return true; // If type is VOID the program is ill-formed so I don't care
    }
}