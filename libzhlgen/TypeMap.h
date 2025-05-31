#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "ParserDefinitions.h"

class TypeMap {
public:
    typedef std::map<std::string, Type*> Map;
    typedef Map::iterator iterator;
    typedef Map::const_iterator const_iterator;

    inline iterator begin() { return _types.begin(); }
    inline iterator end() { return _types.end(); }
    inline const_iterator begin() const { return _types.begin(); }
    inline const_iterator end() const { return _types.end(); }

    inline size_t size() const { return _types.size(); }

    ~TypeMap();
    Type* New(std::string const& name);
    Type* Get(std::string const& name, bool create = true);
    Type* Get(BasicType const& type, bool create = true);
    Type* MakePointer(Type const* base, PointerDecl const& decl);
    Type* MakeConst(Type const* base);
    Type* MakeArray(Type const* base, Array const& array);

private:
    std::map<std::string, Type*> _types;
};


typedef std::unordered_map<std::string, std::string> AsmDefMap;