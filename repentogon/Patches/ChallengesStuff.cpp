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
#include <LuaJIT/src/lua.hpp>
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

HOOK_METHOD(Menu_Challenge, Render, () -> void) {
	LayerState* frame = this->ChallengeMenuSprite.GetLayer(26);
	LayerState* frame2 = this->ChallengeMenuSprite.GetLayer(27);
	LayerState* frame3 = this->ChallengeMenuSprite.GetLayer(28);
	int sel = this->SelectedChallengeID;
	float targetyoffset = 0;
		if (sel > 4) { targetyoffset = ((sel - 4) * 22); }

		frame->_pos.y = lerp(frame->_pos.y, targetyoffset,0.05);
		frame2->_pos.y = frame->_pos.y;
		frame3->_pos.y = frame->_pos.y;
	super();
}
ANM2* Streak = new ANM2();

HOOK_METHOD(Menu_CustomChallenge, Render, () -> void) {
	int sel = this->SelectedElement;
	float targetyoffset = 0;
	ANM2* s = &g_MenuManager->GetMenuChallenge()->ChallengeMenuSprite;

	super();
	if (!Streak->_loaded) {
		Streak->construct_from_copy(s);
		string dir = string("../../../resources-repentogon/gfx/ui/challengesmenustuff.anm2");
		Streak->Load(dir, true);
		Streak->LoadGraphics(true);
		Streak->Play("Idle", true);
		Streak->Update();
	}
	Vector* ref = &g_MenuManager->_ViewPosition; 
	ref = new Vector(ref->x + 39, ref->y + 15);
	Vector* offset = new Vector(ref->x - 950, ref->y + 216);
	Vector pos = Vector(70 + offset->x, 53 + offset->y);
	Vector z = Vector(0, 0);
	for (int i = 46; i <= XMLStuff.ChallengeData->maxid;i++ ) {
		XMLAttributes node = XMLStuff.ChallengeData->GetNodeById(i);
		pos.y += 25;
		if (Challenges[node["name"] + node["sourceid"]] > 0) {
			string order = to_string(i - 45) + ".";
			srand(i-40);
			Streak->SetFrame(&string("Idle"), rand() % 6);
			Streak->Update();
			Streak->_scale.x =  (g_Manager->_font8_TeamMeat_12.GetStringWidthUTF8(node["name"].c_str()) / 294.0);
			Streak->_offset.x = g_Manager->_font8_TeamMeat_12.GetStringWidthUTF8(order.c_str()) - 10;
			Streak->Render(&pos, &z, &z);
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
	printf("[Callenge] %s completed (total: %d)", modachiev["name"].c_str(), Challenges[chalid]);
}
