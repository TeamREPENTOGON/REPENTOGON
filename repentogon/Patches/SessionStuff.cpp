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
#include "../MiscFunctions.h"
#include <lua.hpp>
#include "LuaCore.h"
#include "JsonSavedata.h"
#include <filesystem>
#include <regex>
#include <queue>
#include "SessionStuff.h"

using namespace rapidxml;
using namespace rapidjson;
using namespace std;

unordered_map < string, vector<int>> PlayerFormTracker; //player<formcounts>
unordered_map <string, vector<int> > TagPlayerForms; //tag<formids>
unordered_map <int, vector<string> > TagsForItem; //itemid<tags>
bool formsbuilt = false;

queue<int> pendingforms;
string sessionjsonpath;


void SaveSessionToJson() {
	rapidjson::Document doc;
	doc.SetObject();

	//SavingAchievements
	ArrayOfArrayToJson(&doc, "PlayerFormTracker", PlayerFormTracker);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	ofstream ofs(sessionjsonpath);
	ofs << buffer.GetString() << std::endl;
	logViewer.AddLog("[REPENTOGON]", "Session saved to : % s \n", sessionjsonpath.c_str());
}

void LoadSessionFromJson() {
	InitAchievs();
	rapidjson::Document doc = GetJsonDoc(&sessionjsonpath);
	if (!doc.IsObject()) {
		logViewer.AddLog("[REPENTOGON]", "No Session for saveslot in: %s \n", sessionjsonpath.c_str());
		return;
	}
	JsonToArrayOfArray(doc["PlayerFormTracker"], PlayerFormTracker);

	logViewer.AddLog("[REPENTOGON]", "Session loaded from: %s \n", achivjsonpath.c_str());
}


HOOK_METHOD(Manager, LoadGameState, (int saveslot) -> void) {
	sessionjsonpath = std::string(REPENTOGON::GetRepentogonDataPath());
	sessionjsonpath.append("session").append(to_string(saveslot)).append(".json");

	LoadSessionFromJson();
	super(saveslot);
}

HOOK_METHOD(Game, Start, (int playertype, int challenge, Seeds seeds, unsigned int difficulty) -> void) {
	if (!formsbuilt) { formsbuilt = true; BuildFormArrays(); }
	printf("New Game Started \n");
	PlayerFormTracker.clear();
	super(playertype, challenge, seeds, difficulty);
}

HOOK_STATIC(LuaEngine, PostGameStart, (unsigned int state) -> void, __stdcall) {
	if (state == 1) {
		if (!formsbuilt) { formsbuilt = true; BuildFormArrays(); }
		printf("Game Continued \n");
	}
	super(state);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	BuildFormArrays();
	super();
}

HOOK_METHOD_PRIORITY(Entity_Player, RemoveCollectible, 9999, (unsigned int CollectibleType, bool IgnoreModifiers, unsigned int ActiveSlot, bool RemoveFromPlayerForm)-> void) {
	if (RemoveFromPlayerForm) {
		IncreasePlayerFormCountersForItem(this, CollectibleType, -1);
	}
	super(CollectibleType, IgnoreModifiers, ActiveSlot, RemoveFromPlayerForm);
}

HOOK_METHOD_PRIORITY(Entity_Player, AddCollectible, 9999, (int type, int charge, bool firsttime, int slot, int vardata)-> void) {
	IncreasePlayerFormCountersForItem(this, type, 1);
	super(type, charge, firsttime, slot, vardata);
}


HOOK_METHOD_PRIORITY(Entity_Player, IncrementPlayerFormCounter, 9999, (int ePlayerForm, int num)-> void) {
	if (ePlayerForm > 14) {

	}
	else {
		super(ePlayerForm, num);
	}
}

