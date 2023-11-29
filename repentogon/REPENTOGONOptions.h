#pragma once
#include "IsaacRepentance.h"
#include "ImGuiFeatures/LogViewer.h"
#include "MiscFunctions.h"

#define MINI_CASE_SENSITIVE
#include "ini.h"

#include <filesystem>

inline int defstoi(const std::string& str, int default) {
	try {
		return std::stoi(str);
	}
	catch (...) {
	}
	return default;
}

struct REPENTOGONOptions {
	void Init() {
		optionsPath = std::string(REPENTOGON::GetRepentogonDataPath());
		optionsPath.append("options.ini");

		mINI::INIFile iniFile(optionsPath);

		if (!std::filesystem::exists(optionsPath)) {
			printf("INI doesn't exist, creating\n");
			
			ini["VanillaTweaks"]["BetterVoidGeneration"] = "1";
			ini["VanillaTweaks"]["HushPanicStateFix"] = "1";
			ini["VanillaTweaks"]["KeyMasterDealChance"] = "1";
			ini["VanillaTweaks"]["QuickRoomClear"] = "0";
			ini["VanillaTweaks"]["PreventModUpdates"] = "0";
			ini["internal"]["DidModReset"] = "0";
			ini["internal"]["EnableUnifont"] = "1";
			iniFile.generate(ini, true);
		}

		iniFile.read(ini);

		betterVoidGeneration = defstoi(ini["VanillaTweaks"]["BetterVoidGeneration"],1);
		hushPanicStateFix = defstoi(ini["VanillaTweaks"]["HushPanicStateFix"],1);
		keyMasterDealChance = defstoi(ini["VanillaTweaks"]["KeyMasterDealChance"],1);
		quickRoomClear = defstoi(ini["VanillaTweaks"]["QuickRoomClear"], 0);
		preventModUpdates = defstoi(ini["VanillaTweaks"]["PreventModUpdates"], 0);
		enableUnifont = defstoi(ini["internal"]["EnableUnifont"], 1);

		printf("Loaded REPENTOGON INI\n");
	}

	bool GetBool(char* category, char* field) {
		mINI::INIFile iniFile(optionsPath);
		return ini[category][field] == "1" ? true : false;
	}

	bool Write(char* category, char* field, char* value) {
		mINI::INIFile iniFile(optionsPath);
		ini[category][field] = value;
		return iniFile.write(ini);
	}

	bool Write(char* category, char* field, bool value) {
		mINI::INIFile iniFile(optionsPath);
		ini[category][field] = value? "1" : "0";
		return iniFile.write(ini);
	}

	void Save() {
		Write("VanillaTweaks", "BetterVoidGeneration", betterVoidGeneration);
		Write("VanillaTweaks", "HushPanicStateFix",    hushPanicStateFix);
		Write("VanillaTweaks", "KeyMasterDealChance",  keyMasterDealChance);
		Write("VanillaTweaks", "QuickRoomClear",	   quickRoomClear);
		Write("VanillaTweaks", "PreventModUpdates",	   preventModUpdates);
		Write("internal",	   "EnableUnifont",		   enableUnifont);
	}

	mINI::INIStructure ini;
	bool betterVoidGeneration;
	bool hushPanicStateFix;
	bool keyMasterDealChance;
	bool quickRoomClear;
	bool preventModUpdates;
	bool enableUnifont;
	std::string optionsPath;
};

extern REPENTOGONOptions repentogonOptions;