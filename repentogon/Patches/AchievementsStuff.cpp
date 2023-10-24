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

#include "../ImGuiFeatures/LogViewer.h"
#include <lua.hpp>
#include "LuaCore.h"
#include "JsonSavedata.h"
#include <filesystem>
#include <regex>

using namespace rapidxml;
using namespace rapidjson;
using namespace std;


unordered_map<string, int > Achievements;
unordered_map<int, unordered_map<int, vector<int>> > CompletionMarkListeners;
unordered_map<int, unordered_map<int, vector<int>> > EventCounterListeners;
unordered_map<tuple<int, int, int>, unordered_map<int, vector<int>>> BossDeathListeners;
unordered_map<string, int> simplifiedeventsenum;
string achivjsonpath;



#include <queue>
int dummyachiev = -1;
bool achievdone = false;
bool blocksteam = true;
int lastdummyachievframe = 10;
queue<int> pendingachievs;


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

	//SavingAchievements
	ArrayToJson(&doc, "Achievements", Achievements);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	ofstream ofs(achivjsonpath);
	ofs << buffer.GetString() << std::endl;
	logViewer.AddLog("[REPENTOGON]", "Achievements saved to : % s \n", achivjsonpath.c_str());
}

void LoadAchievementsFromJson() {
	InitAchievs();
	rapidjson::Document doc = GetJsonDoc(&achivjsonpath);
	if (!doc.IsObject()) {
		logViewer.AddLog("[REPENTOGON]", "No achievements for saveslot in: %s \n", achivjsonpath.c_str());
		return;
	}
	JsonToArray(doc["Achievements"],Achievements);

	logViewer.AddLog("[REPENTOGON]", "Achievements loaded from: %s \n", achivjsonpath.c_str());
}

HOOK_METHOD(PersistentGameData, TryUnlock, (int achieveemntid) -> bool) {
	if (achieveemntid <= 638) {
		return super(achieveemntid);
	}
	ANM2* AchievPop = g_Manager->GetAchievementOverlay()->GetANM2();
	XMLAttributes modachiev = XMLStuff.AchievementData->nodes[achieveemntid];
	string achievid = modachiev["name"] + modachiev["sourceid"];
	if (Achievements[achievid] <= 1) {
		Achievements[achievid] = 2; //2 is for notified, 1 is accomplished, <1 is in progress
		SaveAchieveemntsToJson();
		if (g_Manager->GetOptions()->PopUpsEnabled()) { //it is prevented even without this check, but theres no point in doing the hackies if thats the case.
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
		}
		else { printf("[Achiev] Modded popup prevented due to pops disabled \n"); }
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

HOOK_METHOD(PersistentGameData, IncreaseEventCounter, (int eEvent, int val) -> void) {
	int numplayers = g_Game->GetNumPlayers();
	for (int i = 0; i < numplayers; i++) {
		int playertype = g_Game->GetPlayer(i)->GetPlayerType();
		RunTrackersForEventCounter(eEvent, playertype);
	}
	return super(eEvent,val);
}


HOOK_METHOD(Manager, LoadGameState, (int saveslot) -> void) {
	achivjsonpath = string((char*)&g_SaveDataPath) + "Repentogon/achievements" + to_string(saveslot) + ".json";

	LoadAchievementsFromJson();
	super(saveslot);
}

bool LockAchievement(int achievementid) {
	PersistentGameData* ps = g_Manager->GetPersistentGameData();
	bool had = ps->achievements[achievementid];
	ps->achievements[achievementid] = 0;
	ps->TryUnlock(0);
	return had != ps->achievements[achievementid];
}

static std::vector<std::string> ParseCommandA(std::string command, int size = 0) {
	std::vector<std::string> cmdlets;

	std::stringstream sstream(command);
	std::string cmdlet;
	char space = ' ';
	while (std::getline(sstream, cmdlet, space)) {
		cmdlet.erase(std::remove_if(cmdlet.begin(), cmdlet.end(), ispunct), cmdlet.end());
		cmdlets.push_back(cmdlet);
		if (size > 0 && cmdlets.size() == size) {
			break;
		}
	}
	return cmdlets;
}


LUA_FUNCTION(Lua_GetAchievementByName) {
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

	lua::LuaStackProtector protector(_state);

	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetAchievementIdByName", Lua_GetAchievementByName);
}

HOOK_METHOD(Console, RunCommand, (std_string& in, std_string* out, Entity_Player* player)-> void) {
	if (in.rfind("lockachievement", 0) == 0) {
		std::vector<std::string> cmdlets = ParseCommandA(in, 2);
		int id = toint(cmdlets[1]);
		if (LockAchievement(id)) {
			g_Game->GetConsole()->PrintError("Achievement Locked!");
		}
		else {
			g_Game->GetConsole()->PrintError("No Achievement Locked (was already locked!)");
		}
	}
	if (in.rfind("achievement", 0) == 0) {
		std::vector<std::string> cmdlets = ParseCommandA(in, 2);
		int id = toint(cmdlets[1]);
		PersistentGameData* ps = g_Manager->GetPersistentGameData();
		if (ps->TryUnlock(id)) {
			g_Game->GetConsole()->PrintError("Achievement UnLocked!");
		}
		else {
			g_Game->GetConsole()->PrintError("No Achievement UnLocked (was already unlocked!)");
		}
	}
	super(in, out, player);
}
