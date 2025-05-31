#pragma once

#include <unordered_map>
#include <vector>

#include "libzhl.h"

class ASMDefinition {
public:
    // All strings should be in .rdata, no copies are performed
    ASMDefinition(const char* ptr, const char* name, const char* signature);

    friend class ASMDefinitionHolder;
private:
    const char* _ptr;
    const char* _name;
    const char* _signature;
};

class ASMDefinitionHolder {
public:
    ASMDefinitionHolder() { }

    inline std::vector<const char*> const& GetMisses() const {
        return _misses;
    }

    inline void* GetDefinition(const char* id) {
        return _idToAddr[id];
    }

    bool Init();
    friend class ASMDefinition;

    static LIBZHL_API ASMDefinitionHolder* instance();

private:
    inline void RegisterDefinition(ASMDefinition* definition) {
        _definitions.push_back(definition);
    }

    bool AddDefinition(ASMDefinition* definition);

    std::vector<ASMDefinition*> _definitions;
    std::unordered_map<const char*, void*> _idToAddr;
    std::vector<const char*> _misses;
};

#define sASMDefinitionHolder ASMDefinitionHolder::instance()