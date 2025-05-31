#include "ASMDefinition.h"
#include "SigScan.h"
#include "logger/logger.h"

ASMDefinition::ASMDefinition(const char* ptr, const char* name, const char* signature) :
    _ptr(ptr), _name(name), _signature(signature) {
    sASMDefinitionHolder->RegisterDefinition(this);
}

bool ASMDefinitionHolder::Init() {
    bool ok = true;

    for (ASMDefinition* definition : _definitions) {
        if (!AddDefinition(definition)) {
            ok = false;
        }
    }

    return ok;
}

bool ASMDefinitionHolder::AddDefinition(ASMDefinition* definition) {
    SigScan scan(definition->_signature);
    if (!scan.Scan()) {
        _misses.push_back(definition->_name);
        return false;
    } else {
        _idToAddr[definition->_ptr] = scan.GetAddress();
        return true;
    }
}

LIBZHL_API ASMDefinitionHolder _instance;

ASMDefinitionHolder* ASMDefinitionHolder::instance() {
    return &_instance;
}