#pragma once
#include "IsaacRepentance.h"
#include <map>

class ModEntryEx {
public:
	ANM2 _customMinimapANM2;
};

class CustomModManager
{
public:
    static CustomModManager& GetInstance() {
        static CustomModManager instance;
        return instance;
    }

    ModEntryEx* GetEx(ModEntry* mod) {
        auto it = modExtensions.find(mod);
        return (it != modExtensions.end()) ? &it->second : nullptr;
    }

    void AddEx(ModEntry* mod) {
        modExtensions[mod];
    }

private:
    std::map<ModEntry*, ModEntryEx> modExtensions;

    
    CustomModManager() = default;

    //CustomModManager(const CustomModManager&) = delete;
    CustomModManager& operator=(const CustomModManager&) = delete;
};

void ASMPatchesForCustomModManager();