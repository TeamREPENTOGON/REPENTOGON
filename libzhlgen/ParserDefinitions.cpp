#include <sstream>
#include <algorithm>

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
    return !_pointerDecl.empty();
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
            str << variable._type->ToString(false) << " " << variable._name << std::endl;
        }
    }
    else {
        str << _name << " ";
    }
    return str.str();
}

size_t Struct::size() const {
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
    return _qualifiers & VIRTUAL;
}

bool Function::IsCleanup() const {
    return _qualifiers & CLEANUP || (_convention && *_convention == CDECL);
}

bool Function::IsStatic() const {
    return _qualifiers & STATIC;
}

std::string Function::ToString() const {
    std::ostringstream str;
    str << "Function " << _name << std::endl;
    str << "\tqualifiers = ";
    if (_qualifiers == 0) {
        str << "none";
    }
    else {
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