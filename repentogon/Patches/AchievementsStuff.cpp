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
#include <math.h>  

using namespace rapidxml;
using namespace rapidjson;
using namespace std;


unordered_map<int, int > Conditionals; //condid - achievid
unordered_map<string, int > Achievements;
unordered_map<int, unordered_map<int, vector<int>> > CompletionMarkListeners;
unordered_map<int, unordered_map<int, vector<int>> > EventCounterListeners;
unordered_map<int, vector<int>> AchievementListeners; //achievementid - achievements to complete
unordered_map<tuple<int, int, int>, unordered_map<int, vector<int>>> BossDeathListeners;
unordered_map<string, int> simplifiedeventsenum;
string achivjsonpath;

bool sourceswithachievset = false;

int dummyachiev = -1;
bool achievdone = false;
bool blocksteam = false;
int lastdummyachievframe = 10;
queue<int> pendingachievs;


bool achdebugmode = false;

void SaveAchieveemntsToJson() {
	if (!achdebugmode && !sourceswithachievset) { achdebugmode = true; }
	if (achdebugmode) { return; }
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
	//JsonToArray(doc["Achievements"],Achievements);
	for (auto itr = doc["Achievements"].MemberBegin(); itr != doc["Achievements"].MemberEnd(); ++itr) {
		if (Achievements[itr->name.GetString()] < itr->value.GetInt()) {
			Achievements[itr->name.GetString()] = itr->value.GetInt(); //only load if the stored value is higher, otherwise the conditions of the achievement may have changed
		}
	}

	logViewer.AddLog("[REPENTOGON]", "Achievements loaded from: %s \n", achivjsonpath.c_str());
}

HOOK_METHOD(PersistentGameData, TryUnlock, (int achieveemntid) -> bool) {
	//if (achieveemntid < 0) {
		//AccomplishCondition(achieveemntid);
		//return false;
	//}
	if ((achieveemntid >= 0) && (achieveemntid < 638)) {
		return super(achieveemntid);
	}
	ANM2* AchievPop = g_Manager->GetAchievementOverlay()->GetANM2();
	XMLAttributes modachiev = XMLStuff.AchievementData->GetNodeById(achieveemntid);
	string achievid = modachiev["name"] + modachiev["sourceid"];
	if (Achievements[achievid] <= 1) {
		Achievements[achievid] = 2; //2 is for notified, 1 is accomplished, <1 is in progress
		RunTrackersForAchievementCounter(achieveemntid);
		SaveAchieveemntsToJson();
		if (((modachiev.find("hidden") == modachiev.end()) || (modachiev["hidden"] == "false")) && g_Manager->GetOptions()->PopUpsEnabled()) { //it is prevented even without this check, but theres no point in doing the hackies if thats the case.
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
		else { printf("[Achiev] Modded popup prevented due to pops disabled or hidden achiev \n"); }
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

void ReplaceAchievementSprite(ANM2* AchievPop, int achieveemntid) {
	XMLAttributes modachiev = XMLStuff.AchievementData->GetNodeById(achieveemntid);
	//if (modachiev.find("anm2") != modachiev.end()) {
		//AchievPop->Load(modachiev["gfxroot"] + modachiev["anm2"], true);
		//AchievPop->LoadGraphics(true);
	//}
	//else {
		//string def = "gfx/ui/achievement/achievement.anm2";
		//AchievPop->Load(def, true);
		//AchievPop->LoadGraphics(true);
	//}
	if (modachiev.find("gfxback") != modachiev.end()) {
		AchievPop->ReplaceSpritesheet(2, modachiev["gfxroot"] + modachiev["gfxback"]);
		AchievPop->LoadGraphics(true);
	}
	AchievPop->ReplaceSpritesheet(3, modachiev["gfxroot"] + modachiev["gfx"]);
	AchievPop->LoadGraphics(true);
	AchievPop->Update();
	
}

HOOK_METHOD(AchievementOverlay, Update, () -> void) {
	super();
	if ((this->status == 1) && (!achievdone) && (this->displayedachievement == dummyachiev)) {
		achievdone = true;
		lastdummyachievframe = this->frame;
		int achieveemntid = pendingachievs.front();
		pendingachievs.pop();
		ANM2* AchievPop = g_Manager->GetAchievementOverlay()->GetANM2();
		ReplaceAchievementSprite(AchievPop, achieveemntid);

		if (pendingachievs.size() == 0) {
			dummyachiev = -1;
		}
	}
	else if (this->status != 1) { achievdone = false; }
	/*else {
		ANM2* AchievPop = g_Manager->GetAchievementOverlay()->GetANM2();
		if (AchievPop->_filename != "gfx/ui/achievement/achievement.anm2") {
			printf("filename: %s", AchievPop->_filename.c_str());
			string def = "gfx/ui/achievement/achievement.anm2";
			AchievPop->Load(def, true);
			AchievPop->LoadGraphics(true);
		}
	}*/

}

HOOK_METHOD(PersistentGameData, Unlocked, (int achieveemntid) -> bool) {
	if (achieveemntid < 638) {
		return super(achieveemntid);
	}
	XMLAttributes modachiev = XMLStuff.AchievementData->GetNodeById(achieveemntid);
	string achievid = modachiev["name"] + modachiev["sourceid"];
	return Achievements[achievid] > 0;
}

HOOK_METHOD(PersistentGameData, IncreaseEventCounter, (int eEvent, int val) -> void) {
	int numplayers = g_Game->GetNumPlayers();
	for (int i = 0; i < numplayers; i++) {
		int playertype = g_Game->GetPlayer(i)->GetPlayerType();
		RunTrackersForEventCounter(eEvent, playertype);
	}
	return super(eEvent,val);
}


HOOK_METHOD(Manager, SetSaveSlot, (unsigned int slot) -> void) {
	super(slot);
	int saveslot = 1;
	if (slot > 0) { saveslot = slot; }
	achivjsonpath = std::string(REPENTOGON::GetRepentogonDataPath());
	achivjsonpath.append("achievements").append(to_string(saveslot)).append(".json");

	LoadAchievementsFromJson();
}

bool LockAchievement(int achievementid) {
	if (!achdebugmode && !sourceswithachievset) { achdebugmode = true; }
	if (achdebugmode) { return false; }
	if (achievementid < 638) {
		PersistentGameData* ps = g_Manager->GetPersistentGameData();
		bool had = ps->achievements[achievementid];
		ps->achievements[achievementid] = 0;
		ps->TryUnlock(0);
		return had != ps->achievements[achievementid];
	}
	else {
		XMLAttributes node = XMLStuff.AchievementData->GetNodeById(achievementid);
		string idx = node["name"] + node["sourceid"];
		bool had = Achievements[idx] > 0;
		Achievements[idx] = -9999;
		SaveAchieveemntsToJson();
		LoadAchievementsFromJson();
		return had;
	}
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
	lua_pushinteger(L, GetAchievementIdByName(text));
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
		if (id == 0) {
			g_Game->GetConsole()->PrintError("No achievement Id Provided. \n");
		}
		else if (LockAchievement(id)) {
			g_Game->GetConsole()->Print("Locked achievement. \n", Console::Color::WHITE, 0x96U);
		}
		else {
			g_Game->GetConsole()->PrintError("Achievement already locked.");
		}
	}
	if (in.rfind("achievement", 0) == 0) {
		std::vector<std::string> cmdlets = ParseCommandA(in, 2);
		if (cmdlets.size() < 2) { g_Game->GetConsole()->PrintError("No achievement Id Provided. \n"); super(in, out, player); return; }
		int id = toint(cmdlets[1]);
		PersistentGameData* ps = g_Manager->GetPersistentGameData();
		if (id == 0) {
			g_Game->GetConsole()->PrintError("No achievement Id Provided. \n");
		}
		else if (ps->TryUnlock(id)) {
			g_Game->GetConsole()->Print("Unlocked achievement. \n", Console::Color::WHITE, 0x96U);
		}
		else {
			g_Game->GetConsole()->PrintError("Achievement already unlocked. \n");
		}
	}
	super(in, out, player);
}

//Unlocks
HOOK_METHOD(ItemPool, Init, (unsigned int seed, char* filename)-> void) {
	PersistentGameData* pgd = g_Manager->GetPersistentGameData();
	ItemConfig* itmcfg = g_Manager->GetItemConfig();
	for each (XMLAttributes xml in XMLStuff.ItemData->customachievitems) {
		int id = toint(xml["id"]);
		int achid = toint(xml["achievement"]);
		ItemConfig_Item* entry = itmcfg->GetCollectible(id);
		if (pgd->Unlocked(achid)) {
			entry->achievementId = -1;
		}
		else {
			entry->achievementId = achid;
		}
	}
	for each (XMLAttributes xml in XMLStuff.TrinketData->customachievitems) {
		int id = toint(xml["id"]);
		int achid = toint(xml["achievement"]);
		ItemConfig_Item* entry = itmcfg->GetTrinket(id);
		if (pgd->Unlocked(achid)) {
			entry->achievementId = -1;
		}
		else {
			entry->achievementId = achid;
		}
	}
	for each (XMLAttributes xml in XMLStuff.CardData->customachievitems) {
		int id = toint(xml["id"]);
		int achid = toint(xml["achievement"]);
		ItemConfig_Card* entry = itmcfg->GetCard(id);
		if (pgd->Unlocked(achid)) {
			entry->achievementId = -1;
		}
		else {
			entry->achievementId = achid;
		}
	}
	for each (XMLAttributes xml in XMLStuff.PillData->customachievitems) {
		int id = toint(xml["id"]);
		int achid = toint(xml["achievement"]);
		ItemConfig_Pill* entry = itmcfg->GetPillEffects()->at(id); //whatever, not getting GetPill's sig, its pointless
		if (pgd->Unlocked(achid)) {
			entry->achievementId = -1;
		}
		else {
			entry->achievementId = achid;
		}
	}
	super(seed, filename);
}

//Unlocks



bool modsecrets = false;
bool secretneedsupdate = false;
string secretssource = "BaseGame";

vector <string> SourcesWithAchiev;
int curridx = 0;
int currmax = 0;
int modmax = 0;

void PlayIfNotPlaying(ANM2* sprite,string* anim, bool force = false) {
	if (force || !sprite->IsPlaying(anim)) {
		sprite->Play(anim->c_str(), true);
		sprite->Update();
	}
}

string selectedanims[6] = { "Icon1UnlockedSelected" ,"Icon2UnlockedSelected","Icon3UnlockedSelected","Icon1LockedSelected","Icon2LockedSelected","Icon3LockedSelected" };
bool IsSpriteSelected(ANM2* sprite) {
	for (int i = 0; i <= 6; i++) {
		if (sprite->IsPlaying(&selectedanims[i])) {
			return true;
		}
	}
	return false;
}

int GetDir(double x) {
	if (x > 0.001) {
		return -1; //its in reverse
	}
	else if (x < -0.001) {
		return 1;
	}
	return 0;
}
int ConstrainId(int x) {
	int res = 0;
	if (x >= currmax) {
		res = ConstrainId(x- currmax);
	}
	else if (x < 0) {
		res = ConstrainId(currmax - x);
	}
	return res;
}

int achievbckup[638];
void BackupAchievsNOverride() {
	if (secretssource != "BaseGame") {
		PersistentGameData* pgd = g_Manager->GetPersistentGameData();
		for (int i = 1; i < currmax; i++) {
			XMLAttributes cur = XMLStuff.ModData->achievlistpermod[secretssource][i-1];
			achievbckup[i] = pgd->achievements[i];
			pgd->achievements[i] = pgd->Unlocked(toint(cur["id"]));
		}
	}
}

void RecoverAchievs() {
	if (secretssource != "BaseGame") {
		PersistentGameData* pgd = g_Manager->GetPersistentGameData();
		for (int i = 1; i < currmax; i++) {
			pgd->achievements[i] = achievbckup[i];
		}
	}
}


int achievidoffsets[9] = {-4,-3,-2,-1,0,1,2,3,4};
ANM2* copyselected;
void UpdateSecretsSprites(bool justpaper) {
	Menu_Stats* menstats = g_MenuManager->GetMenuStats();	
	PersistentGameData* pgd = g_Manager->GetPersistentGameData();
	XMLAttributes cur = XMLStuff.ModData->achievlistpermod[secretssource][menstats->_selectedAchievmentId];
	int id = toint(cur["id"]);
	if (pgd->Unlocked(id)) {
		ReplaceAchievementSprite(&menstats->_achievementsSprite, id);
	}
	else {
		menstats->_achievementsSprite.ReplaceSpritesheet(3, string("gfx/ui/achievement/achievement_locked.png"));
		menstats->_achievementsSprite.LoadGraphics(true);
		menstats->_achievementsSprite.Update();
	}
}

void ChangeCurrentSecrets(int dir) {
	int tgrtdir = curridx + dir;
	if (tgrtdir < 0) {
		tgrtdir = SourcesWithAchiev.size() - 1;
	}else if (tgrtdir >= SourcesWithAchiev.size()) {
		tgrtdir = 0;
	} 
	if (curridx != tgrtdir) {
		curridx = tgrtdir;
		secretssource = SourcesWithAchiev[curridx]; 
		currmax = XMLStuff.ModData->achievlistpermod[secretssource].size();
		Menu_Stats* menstats = g_MenuManager->GetMenuStats();
		menstats->_selectedAchievmentId = 0;
		menstats->_maxAchievementID = currmax;
		for (int i = 1; i <= currmax; i++) {
			AchievementOverlay_Entry overlay = menstats->_achievementOverlayEntries[i];
			XMLAttributes ach = XMLStuff.ModData->achievlistpermod[secretssource][i-1];
			overlay.id = toint(ach["id"]);
			overlay.gfx = ach["gfxroot"] + ach["gfx"];
			menstats->_achievementOverlayEntries[i] = overlay;
		}
		UpdateSecretsSprites(false);
	}
}

void SetUpReverseSourcesVec() {
		if (!sourceswithachievset) {
			Menu_Stats* menstats = g_MenuManager->GetMenuStats();
			sourceswithachievset = true;
			SourcesWithAchiev.clear();
			for each (const auto & entry in XMLStuff.ModData->achievlistpermod) {
				SourcesWithAchiev.push_back(entry.first);
				if (entry.first == secretssource) {
					curridx = SourcesWithAchiev.size() - 1;
				}
			}
			currmax = XMLStuff.ModData->achievlistpermod[secretssource].size();
		}
}



HOOK_METHOD(Menu_Stats, UpdateSecrets, () -> void) {
	BackupAchievsNOverride();
	super();
	RecoverAchievs();
	//UpdateSecretsSprites(false);
}

HOOK_METHOD(Menu_Stats, Render, () -> void) {
		super();
		if (this->_isAchievementScreenVisible && g_Manager->GetOptions()->ModsEnabled() && (SourcesWithAchiev.size() > 1)) {
			Vector* ref = &g_MenuManager->_ViewPosition;
			ref = new Vector(ref->x + 39, ref->y + 15);
			Vector* offset = new Vector(ref->x - 480, ref->y + 1350);
			Vector pos = Vector(-251 + offset->x, -3 + offset->y);
			Vector z = Vector(0, 0);
			//Vector* a = g_LuaEngine->GetMousePosition(&z, true);
			//printf("%f %f", a->x, a->y);
			DrawStringEntry* sourcename = new DrawStringEntry();
			if (secretssource == "BaseGame") {
				sourcename->_text = "Repentance";
			}
			else {
				sourcename->_text = XMLStuff.ModData->nodes[XMLStuff.ModData->byid[secretssource]]["name"].c_str();
			}
			sourcename->_color._blue = 1;
			sourcename->_color._green = 1;
			sourcename->_color._red = 1;
			sourcename->_color._alpha = 0.8;
			sourcename->_center = true;
			sourcename->_boxWidth = 900;
			sourcename->_x = pos.x;
			sourcename->_y = pos.y;
			g_Manager->_font7_TeamMeat_10.DrawString(*sourcename);
			float txtwidth = g_Manager->_font7_TeamMeat_10.GetStringWidth(sourcename->_text)/2;
			float x = 0;
			float y = 100;
			this->_cursorLeftSprite._scale = Vector(0.5, 0.5);
			this->_cursorLeftSprite._rotation = 90;
			this->_cursorLeftSprite.Render(&(*offset + Vector(192- txtwidth, 8)), &z, &z);
			this->_cursorLeftSprite._rotation = -90;
			this->_cursorLeftSprite.Render(&(*offset + Vector(205+ txtwidth, 4)), &z, &z);
			this->_cursorLeftSprite._rotation = 0;
			this->_cursorLeftSprite._scale = Vector(1, 1);
		}
}


HOOK_METHOD(Menu_Stats, Update, () -> void) {
	super();
	if (this->_isAchievementScreenVisible && g_Manager->GetOptions()->ModsEnabled()) {
		SetUpReverseSourcesVec();
		
		if (g_InputManagerBase.IsActionTriggered(22, -1, 0)) {
			ChangeCurrentSecrets(1);
		}else if (g_InputManagerBase.IsActionTriggered(23, -1, 0)) {
			ChangeCurrentSecrets(-1);
		}
		Menu_Stats* menstats = g_MenuManager->GetMenuStats();
		menstats->_maxAchievementID = currmax;
	}

}