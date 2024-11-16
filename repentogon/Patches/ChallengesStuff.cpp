#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <memory>
#include <array>
#include <cmath>

#include <cstring>

#include "XMLData.h"
#include "ChallengesStuff.h"
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

using namespace rapidxml;
using namespace rapidjson;
using namespace std;

unordered_map<string, int > Challenges;
string chaljsonpath;

void SaveChallengesToJson() {
	rapidjson::Document doc;
	doc.SetObject();

	//SavingAchievements
	ArrayToJson(&doc, "Challenges", Challenges);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	ofstream ofs(chaljsonpath);
	ofs << buffer.GetString() << std::endl;
	logViewer.AddLog("[REPENTOGON]", "Challenges saved to : % s \n", chaljsonpath.c_str());
}

void LoadChallengesFromJson() {
	InitChals();
	rapidjson::Document doc = GetJsonDoc(&chaljsonpath);
	if (!doc.IsObject()) {
		logViewer.AddLog("[REPENTOGON]", "No challenges for saveslot in: %s \n", chaljsonpath.c_str());
		return;
	}
	for (auto itr = doc["Challenges"].MemberBegin(); itr != doc["Challenges"].MemberEnd(); ++itr) {
		if (Challenges[itr->name.GetString()] < itr->value.GetInt()) {
			Challenges[itr->name.GetString()] = itr->value.GetInt(); //only load if the stored value is higher
		}
	}

	logViewer.AddLog("[REPENTOGON]", "Challenges loaded from: %s \n", chaljsonpath.c_str());
}


void UndoChallenge(int challengeid) {
	if (challengeid < 0) { return; }
	if (challengeid <= 45) {
		g_Manager->GetPersistentGameData()->challenges[challengeid] = false;
		g_Manager->GetPersistentGameData()->Save();
	}
	else {
		XMLAttributes node = XMLStuff.ChallengeData->GetNodeById(challengeid);
		Challenges[node["name"] + node["sourceid"]] = 0;
		SaveChallengesToJson();
	}
}

HOOK_METHOD(Manager, SetSaveSlot, (unsigned int slot) -> void) {
	super(slot);
	int saveslot = 1;
	if (slot > 0) { saveslot = slot; }
	chaljsonpath = std::string(REPENTOGON::GetRepentogonDataPath());
	chaljsonpath.append("challenges").append(to_string(saveslot)).append(".json");

	LoadChallengesFromJson();
}

float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

vector<int> visiblechallenges;

HOOK_METHOD(Manager, StartNewGame, (int playerType, int challenge, Seeds unk, int difficulty) -> void) {
	if (g_MenuManager != NULL) {
		if ((g_MenuManager->_selectedMenuID == 15)) {
			int sel = g_MenuManager->GetMenuCustomChallenge()->SelectedElement;
			if ((sel == 1) || ((int)(visiblechallenges.size() - 1) < (sel - 46))) {
				return super(0, 1, unk, difficulty);
			}
			return super(playerType, visiblechallenges[sel - 46], unk, difficulty);
		}
	}
	return super(playerType, challenge, unk, difficulty);

}

HOOK_METHOD(Menu_Challenge, Render, () -> void) {
	LayerState* frame = this->ChallengeMenuSprite.GetLayer(26);
	LayerState* frame2 = this->ChallengeMenuSprite.GetLayer(27);
	LayerState* frame3 = this->ChallengeMenuSprite.GetLayer(28);
	int sel = this->SelectedChallengeID;
	float targetyoffset = 0;
		if (sel > 4) { targetyoffset = (float)((sel - 4) * 22); }

		frame->_pos.y = lerp(frame->_pos.y, targetyoffset,0.05f);
		frame2->_pos.y = frame->_pos.y;
		frame3->_pos.y = frame->_pos.y;
	super();
}
ANM2 Streak = ANM2();
int prevchalselect = -1;
unordered_map<int, bool> lockedchallenges;
string lockedchalstr = string("LOCKED :(");


int firstunlocked = 1;
int lastunlocked = 1;

void boundselchal(Menu_CustomChallenge* s) {
	if (firstunlocked == lastunlocked) { s->SelectedElement = firstunlocked; return; }
	if ((s->SelectedElement) > (lastunlocked)) {
		if (prevchalselect == firstunlocked) {
			s->SelectedElement = lastunlocked;
		}
		else {
			s->SelectedElement = firstunlocked;
		}
	}
	else if ((s->SelectedElement) < firstunlocked){
			s->SelectedElement = lastunlocked;
		
	}
}
bool visibleinit = false;
HOOK_METHOD(PersistentGameData, TryUnlock, (int achievementID) -> bool) {
	bool ret = super(achievementID);
	visibleinit = false;
	return ret;
}
HOOK_METHOD(Console, RunCommand, (std_string& in, std_string* out, Entity_Player* player) -> void) {
	super(in,out, player);
	if (in.find("lockachievement ",15)) {
		visibleinit = false;	
	}
}

void skiplocked(Menu_CustomChallenge* m) {
	if (visibleinit && (visiblechallenges.size() > 0)) {
		int p = m->SelectedElement;
		do {
			p = m->SelectedElement;
			int sel = visiblechallenges[m->SelectedElement - 46];
			if (lockedchallenges[sel]) {
				if (prevchalselect < m->SelectedElement) {
					m->SelectedElement = m->SelectedElement + 1;
				}
				else {
					m->SelectedElement = m->SelectedElement - 1;
				}
			}
			boundselchal(m);
			//ZHL::Log("%d %d\n", this->SelectedElement,p);
		} while ((p != m->SelectedElement));
	}
}

HOOK_METHOD(Menu_CustomChallenge, Update, () -> void) {
	if (!visibleinit) {
		visibleinit = true;
		firstunlocked = 1;
		visiblechallenges.clear();
		for (int i = 46; i <= XMLStuff.ChallengeData->maxid; i++) {
			XMLAttributes node = XMLStuff.ChallengeData->GetNodeById(i);
			if ((node.find("hidden") == node.end()) || (node["hidden"] == "false")) {
				visiblechallenges.push_back(i);
				lockedchallenges[i] = false;
				if ((node.begin() != node.end()) && (node.find("achievements") != node.end()) && !(MeetsComaSeparatedAchievs(node["achievements"]))) {
					lockedchallenges[i] = true;
				}
				else {
					if (firstunlocked == 1) { firstunlocked = 46 + (visiblechallenges.size() - 1); }
					lastunlocked = 46 + (visiblechallenges.size() - 1);
				}
			}
		}
		 this->SelectedElement=firstunlocked;
		 if (firstunlocked == 1) {
			 this->SelectedElement = 1;
		 }
	}
	if ((visiblechallenges.size() > 0)) {
		skiplocked(this);
	}
	else
	{
		this->SelectedElement = 1;
	}
	prevchalselect = this->SelectedElement;
	super();
}

HOOK_METHOD(Menu_CustomChallenge, Render, () -> void) {
	boundselchal(this);
	skiplocked(this);
	int sel = this->SelectedElement;
	float targetyoffset = 0;
	ANM2* s = &g_MenuManager->GetMenuChallenge()->ChallengeMenuSprite;
	FontSettings settings;
	settings._align = 0;  // DrawStringAlignment.TOP_LEFT
	KColor color(0.14f, 0.15f, 0.21f, 1.f);
	Vector scale(0.8f, 1.f);

	super();
	if (!Streak._loaded) {
		Streak.construct_from_copy(s);
		Streak.Load(REPENTOGON::GetRGONGfxAbsolutePath("gfx/ui/challengesmenustuff.anm2"), true);
		Streak.LoadGraphics(true);
		Streak.Play("Idle", true);
		Streak.Update();
	}
	Vector ref = g_MenuManager->_ViewPosition; 
	ref = Vector(ref.x + 39, ref.y + 15);
	Vector offset = Vector(ref.x - 950, ref.y + 216);
	Vector pos = Vector(70 + offset.x, 53 + offset.y);
	Vector z = Vector(0, 0);
	int count = 0;
	for each(int i in visiblechallenges) {
		count++;
		XMLAttributes node = XMLStuff.ChallengeData->GetNodeById(i);
		string order = to_string(count) + ".";
		string challengesrt = (order + node["name"]);
		pos.y += 25;
		const char* text = challengesrt.c_str();
		Vector renderPos(pos.x, pos.y - 20);
		color._alpha = 1;
		if (lockedchallenges[i]) {
			string locked = order + lockedchalstr;
			if ((node.find("lockeddesc") != node.end())) {
				locked = order + node["lockeddesc"];
			}
			color._alpha = 0.5;
			g_Manager->_font2_TeamMeatEx12.DrawString(locked.c_str(), renderPos, scale, &color, &settings);
		}
		else {
			g_Manager->_font2_TeamMeatEx12.DrawString(challengesrt.c_str(), renderPos, scale, &color, &settings);
			if (Challenges[node["name"] + node["sourceid"]] > 0) {
				srand(i-40);
				Streak.SetFrame(&string("Idle"), (float)(rand() % 6));
				Streak.Update();
				Streak._scale.x =  (float)(g_Manager->_font2_TeamMeatEx12.GetStringWidth(node["name"].c_str()) / 294.0);
				Streak._offset.x = (float)(g_Manager->_font2_TeamMeatEx12.GetStringWidth(order.c_str()) - 10);
				Streak.Render(&pos, &z, &z);
			}
		}
	}
}

int tointc(const string& str) {
	if (str.length() > 0) {
		char* endPtr;
		int returnval = strtol(str.c_str(), &endPtr, 0);
		if (endPtr != "\0") {
			return returnval;
		}
	}
	return 0;
}

HOOK_METHOD(PersistentGameData, AddChallenge, (int challengeid) -> void) {
	//if (achieveemntid < 0) {
		//AccomplishCondition(achieveemntid);
		//return false;
	//}
	if ((challengeid >= 0) && (challengeid <= 45)) {
		return super(challengeid);
	}
	XMLAttributes modachiev = XMLStuff.ChallengeData->GetNodeById(challengeid);
	string chalid = modachiev["name"] + modachiev["sourceid"];
	Challenges[chalid] += 1;
	if (modachiev.find("unlockachievement") != modachiev.end()) {
		g_Manager->GetPersistentGameData()->TryUnlock(tointc(modachiev["unlockachievement"]));
	}
	SaveChallengesToJson();
	ZHL::Log("[Callenge] %s completed (total: %d)", modachiev["name"].c_str(), Challenges[chalid]);
}
