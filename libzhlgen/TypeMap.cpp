#include "TypeMap.h"

#include <stdexcept>

TypeMap::~TypeMap() {
    for (auto const& [_, type] : _types) {
        delete type;
    }
}

Type* TypeMap::Get(std::string const& name, bool create) {
    auto iter = _types.find(name);
    if (iter != _types.end()) {
        return iter->second;
    } else {
        if (!create) {
            return nullptr;
        } else {
            return New(name);
        }
    }
}

Type* TypeMap::New(std::string const& name) {
    Type* result = new Type();
    auto iter = _types.insert(std::make_pair(name, result));
    if (!iter.second) {
        delete result;
        return nullptr;
    }

    result->_name = name;
    return result;
}

Type* TypeMap::Get(BasicType const& type, bool create) {
    return Get(type.GetAbsoluteName(), create);
}

Type* TypeMap::MakePointer(Type const* base, PointerDecl const& decl) {
    Type tentative = *base;
    tentative._pointerDecl.push_back(decl);

    std::string name = tentative.GetFullName();
    Type* type = Get(name, false);
    if (!type) {
        type = New(name);
        type->_name = base->_name;
        type->_base = (Type*)base;
        type->_pointerDecl.push_back(decl);
    }

    if (!type->_base) {
        throw std::runtime_error("Pointer to type has no base type to point to");
    }

    return type;
}

Type* TypeMap::MakeConst(Type const* base) {
    if (base->_const) {
        return (Type*)base;
    }

    Type attempt = *base;
    attempt._const = true;
    std::string name = attempt.GetFullName();

    Type* type = Get(name, false);
    if (!type) {
        type = New(name);
        type->_name = base->_name;
        type->_base = (Type*)base;
        type->_const = true;
    }

    if (!type->_base) {
        throw std::runtime_error("Const type has no base type to make const");
    }

    return type;
}

Type* TypeMap::MakeArray(Type const* base, Array const& array) {
    if (base->_array) {
        throw std::runtime_error("Multi dimensional arrays are not supported");
    }

    Type attempt = *base;
    attempt._array = true;
    attempt._arraySize = array._size;

    std::string name = attempt.GetFullName();
    Type* type = Get(name, false);
    if (!type) {
        type = New(name);
        type->_array = true;
        type->_arraySize = array._size;
        type->_name = base->_name;
        type->_base = (Type*)base;
    }

    if (!type->_base) {
        throw std::runtime_error("Array of type has no base type");
    }

    return type;
}