#pragma once

#include "IsaacRepentance.h"
#include "XMLData.h"

class ModEntryEx {
public:
	ANM2 _customMapItemIcons;
	ANM2 _customMinimapIcons;
	ANM2 _customStageIcons;
};

struct CurseSpriteData {
	ModEntryEx* modEx;
	ANM2* customANM2;
	uint32_t frameNum;
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

	ModEntryEx* GetEx(const std::string& xmlmodid) {
		if (ModEntry* mod = GetModByXMLDataID(xmlmodid)) {
			return GetEx(mod);
		}
		return nullptr;
	}

    void AddEx(ModEntry* mod) {
        modExtensions[mod];
    }

	static const std::map<uint32_t, CurseSpriteData>& GetCurseSpriteMap();

private:
    std::unordered_map<ModEntry*, ModEntryEx> modExtensions;

    
    CustomModManager() = default;

    //CustomModManager(const CustomModManager&) = delete;
    CustomModManager& operator=(const CustomModManager&) = delete;
};

void ASMPatchesForCustomModManager();