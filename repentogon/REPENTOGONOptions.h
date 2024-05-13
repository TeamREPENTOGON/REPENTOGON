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
			
			ini["VanillaTweaks"]["BetterVoidGeneration"] = "0";
			ini["VanillaTweaks"]["HushPanicStateFix"] = "1";
			ini["VanillaTweaks"]["KeyMasterDealChance"] = "0";
			ini["VanillaTweaks"]["QuickRoomClear"] = "0";
			ini["VanillaTweaks"]["StatHUDPlanetarium"] = "1";
			ini["VanillaTweaks"]["SkipIntro"] = "0";
			ini["VanillaTweaks"]["PreventModUpdates"] = "0";
			ini["VanillaTweaks"]["FastLasers"] = "0";
			ini["VanillaTweaks"]["InterpolV2"] = "0";
			ini["internal"]["DidModReset"] = "0";
			ini["internal"]["EnableUnifont"] = "1";
			ini["internal"]["UnifontRenderMode"] = "0";
			ini["internal"]["LastSaveFile"] = "0";
			ini["internal"]["FileMap"] = "1";
			iniFile.generate(ini, true);
		}

		iniFile.read(ini);

		betterVoidGeneration = defstoi(ini["VanillaTweaks"]["BetterVoidGeneration"],0);
		hushPanicStateFix = defstoi(ini["VanillaTweaks"]["HushPanicStateFix"],1);
		keyMasterDealChance = defstoi(ini["VanillaTweaks"]["KeyMasterDealChance"],0);
		quickRoomClear = defstoi(ini["VanillaTweaks"]["QuickRoomClear"], 0);
		statHUDPlanetarium = defstoi(ini["VanillaTweaks"]["StatHUDPlanetarium"], 1);
		skipIntro = defstoi(ini["VanillaTweaks"]["SkipIntro"], 0);
		preventModUpdates = defstoi(ini["VanillaTweaks"]["PreventModUpdates"], 0);
		fastLasers = defstoi(ini["VanillaTweaks"]["FastLasers"], 0);
		interpolV2 = defstoi(ini["VanillaTweaks"]["InterpolV2"], 0);
		enableUnifont = defstoi(ini["internal"]["EnableUnifont"], 1);
		unifontRenderMode = defstoi(ini["internal"]["UnifontRenderMode"], 0);
		lastSaveFile = defstoi(ini["internal"]["LastSaveFile"], 0);
		fileMap = defstoi(ini["internal"]["FileMap"], 1);
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

	bool Write(char* category, char* field, int value) {
		mINI::INIFile iniFile(optionsPath);
		char buff[128];
		sprintf_s(buff, "%d", value);
		ini[category][field] = buff;
		return iniFile.write(ini);
	}

	void Save() {
		Write("VanillaTweaks", "BetterVoidGeneration", betterVoidGeneration);
		Write("VanillaTweaks", "HushPanicStateFix",    hushPanicStateFix);
		Write("VanillaTweaks", "KeyMasterDealChance",  keyMasterDealChance);
		Write("VanillaTweaks", "QuickRoomClear",	   quickRoomClear);
		Write("VanillaTweaks", "PreventModUpdates",	   preventModUpdates);
		Write("VanillaTweaks", "StatHUDPlanetarium",   statHUDPlanetarium);
		Write("VanillaTweaks", "SkipIntro", skipIntro);
		Write("VanillaTweaks", "FastLasers", fastLasers);
		Write("VanillaTweaks", "InterpolV2", interpolV2);
		Write("internal",	   "EnableUnifont",		   enableUnifont);
		Write("internal",	   "UnifontRenderMode",	   unifontRenderMode);
		Write("internal", "LastSaveFile", lastSaveFile);
		Write("internal", "FileMap", fileMap);
	}

	mINI::INIStructure ini;
	bool betterVoidGeneration;
	bool hushPanicStateFix;
	bool keyMasterDealChance;
	bool quickRoomClear;
	bool preventModUpdates;
	bool enableUnifont;
	bool statHUDPlanetarium;
	int unifontRenderMode;
	bool fastLasers;
	int lastSaveFile;
	bool skipIntro;
	bool interpolV2;
	bool fileMap;
	std::string optionsPath;
};

extern REPENTOGONOptions repentogonOptions;