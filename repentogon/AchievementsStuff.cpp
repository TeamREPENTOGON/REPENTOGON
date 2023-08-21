#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <memory>
#include <array>

#include <cstring>

#include "XMLData.h"
#include "AchievementsStuff.h"

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "mologie_detours.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

#include "ImGuiFeatures/LogViewer.h"
#include <lua.hpp>
#include "LuaCore.h"
#include <filesystem>
#include <regex>

using namespace rapidxml;
using namespace std;


extern unordered_map<string, bool > Achievements;
string achivjsonpath;


void SaveAchieveemntsToJson() {
	std::string directory = achivjsonpath.substr(0, achivjsonpath.find_last_of("\\/"));
	if (!CreateDirectory(directory.c_str(), NULL)) {
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			logViewer.AddLog("[REPENTOGON]", "Error creating Repentogon Save directory\n");
			return;
		}
	}

	rapidjson::Document doc;
	doc.SetObject();

	for (auto& kv : Achievements) {
		doc.AddMember(rapidjson::Value(kv.first.c_str(), kv.first.size(), doc.GetAllocator()),
			rapidjson::Value(kv.second), doc.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	ofstream ofs(achivjsonpath);
	ofs << buffer.GetString() << std::endl;
	logViewer.AddLog("[REPENTOGON]", "Achievements saved to : % s \n", achivjsonpath.c_str());
}

void LoadAchievementsFromJson() {
	Achievements.clear();
	ifstream ifs(achivjsonpath);
	if (!ifs.good()) {
		logViewer.AddLog("[REPENTOGON]", "No achievements for saveslot in: %s \n", achivjsonpath.c_str());
		return;
	}

	string content((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));

	rapidjson::Document doc;
	doc.Parse(content.c_str());
	if (!doc.IsObject()) {
		return;
	}
	for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it) {
		string key = it->name.GetString();
		auto& value = it->value;
		if (Achievements.find(key) == Achievements.end()) {
			Achievements[key] = value.GetBool();
		}
	}
	logViewer.AddLog("[REPENTOGON]", "Achievements loaded from: %s \n", achivjsonpath.c_str());
}


HOOK_METHOD(Manager, LoadGameState, (int saveslot) -> void) {
	achivjsonpath = string((char*)&g_SaveDataPath) + "Repentogon/moddedachievements" + to_string(saveslot) + ".json";

	LoadAchievementsFromJson();
	super(saveslot);
}


#include <queue>
int dummyachiev = -1;
bool achievdone = false;
bool blocksteam = true;
int lastdummyachievframe = 10;
queue<int> pendingachievs;

HOOK_METHOD(PersistentGameData, TryUnlock, (int achieveemntid) -> bool) {
	if (achieveemntid <= 638) {
		return super(achieveemntid);
	}
	ANM2* AchievPop = g_Manager->GetAchievementOverlay()->GetANM2();
	XMLAttributes modachiev = XMLStuff.AchievementData->nodes[achieveemntid];
	string achievid = modachiev["name"] + modachiev["sourceid"];
	if (!Achievements[achievid]) {
		Achievements[achievid] = true;
		SaveAchieveemntsToJson();
		pendingachievs.push(achieveemntid);


		if (dummyachiev < 0) {
			dummyachiev = 2;
			for (int x = 2; x < 638; x++) {
				if (this->achievements[x]) {
					dummyachiev = x;
					break;
				}
			}
		}
		printf("[Achiev] Using %d for fake achievement %d \n", dummyachiev, achieveemntid);
		int had = this->achievements[dummyachiev];
		int dum = dummyachiev;
		this->achievements[dummyachiev] = 0;
		blocksteam = true;
		super(dummyachiev);
		blocksteam = false;
		this->achievements[dum] = had;
		return true;
	}
	return false;
}

HOOK_METHOD(PersistentGameData, UnlockSteamAchievement, (int achieveemntid) -> void) {
	if (!blocksteam) {
		return super(achieveemntid);
	}
	printf("[Achiev] Steam Achievement blocked for %d \n", achieveemntid);
}

HOOK_METHOD(AchievementOverlay, Update, () -> void) {
	super();
	if ((this->status == 1) && (!achievdone) && (this->displayedachievement == dummyachiev)) {
		achievdone = true;
		lastdummyachievframe = this->frame;
		int achieveemntid = pendingachievs.front();
		pendingachievs.pop();
		ANM2* AchievPop = g_Manager->GetAchievementOverlay()->GetANM2();
		XMLAttributes modachiev = XMLStuff.AchievementData->nodes[achieveemntid];
		AchievPop->ReplaceSpritesheet(3, modachiev["gfxroot"] + modachiev["gfx"]);
		AchievPop->LoadGraphics(true);
		AchievPop->Update();
		if (pendingachievs.size() == 0) {
			dummyachiev = -1;
		}
	}
	else if (this->status != 1) { achievdone = false; }

}

HOOK_METHOD(PersistentGameData, Unlocked, (int achieveemntid) -> bool) {
	if (achieveemntid <= 638) {
		return super(achieveemntid);
	}
	XMLAttributes modachiev = XMLStuff.AchievementData->nodes[achieveemntid];
	string achievid = modachiev["name"] + modachiev["sourceid"];
	return Achievements[achievid];
}

static int Lua_GetAchievementByName(lua_State* L) {
	string text = string(luaL_checkstring(L, 1));
	if (XMLStuff.AchievementData->byname.count(text) > 0)
	{
		XMLAttributes ent = XMLStuff.AchievementData->GetNodeByName(text);
		if ((ent.count("id") > 0) && (ent["id"].length() > 0)) {
			lua_pushnumber(L, stoi(ent["id"]));
			return 1;
		}
	};
	lua_pushnumber(L, 0);
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);

	lua_getglobal(state, "Isaac");
	lua_pushstring(state, "GetAchievementIdByName");
	lua_pushcfunction(state, Lua_GetAchievementByName);
	lua_rawset(state, -3);
	lua_pop(state, 1);

}