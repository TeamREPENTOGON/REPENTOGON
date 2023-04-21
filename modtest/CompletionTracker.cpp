
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include <bitset>

#include <Windows.h>
#include <fstream>
#include <array>

#include <lua.hpp>
#include "LuaCore.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "XMLData.h"
using namespace std;

extern XMLData XMLStuff;

typedef enum CompletionType {
	MOMS_HEART = 0,
	ISAAC = 1,
	SATAN = 2,
	BOSS_RUSH = 3,
	BLUE_BABY = 4,
	LAMB = 5,
	MEGA_SATAN = 6,
	ULTRA_GREED = 7,
	//scrappedmark ?
	HUSH = 9,
	//scrappedmark ?
	ULTRA_GREEDIER = 11,
	DELIRIUM = 12,
	MOTHER = 13,
	BEAST = 14,
};
unordered_map <int, int> CompletionTypeRender;
bool initializedrendercmpl = false;
void InitMarkRenderTypes() {
	initializedrendercmpl = true;
	CompletionTypeRender[1] = MOMS_HEART;
	CompletionTypeRender[2] = ISAAC;
	CompletionTypeRender[3] = SATAN;
	CompletionTypeRender[4] = BOSS_RUSH;
	CompletionTypeRender[5] = BLUE_BABY;
	CompletionTypeRender[6] = LAMB;
	CompletionTypeRender[7] = MEGA_SATAN;
	CompletionTypeRender[8] = ULTRA_GREED; //this one is also greedier tho
	CompletionTypeRender[9] = HUSH;
	CompletionTypeRender[0] = DELIRIUM;
	CompletionTypeRender[10] = MOTHER;
	CompletionTypeRender[11] = BEAST;
}


unordered_map<string, std::array<int, 15> > CompletionMarks;
string jsonpath;

void SaveCompletionMarksToJson() {
	std::string directory = jsonpath.substr(0, jsonpath.find_last_of("\\/"));
	if (!CreateDirectory(directory.c_str(), NULL)) {
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			printf("[REPENTOGON] Error creating Repentogon Save directory \n");
			return ;
		}
	}

	rapidjson::Document doc;
	doc.SetObject();

	for (auto& kv : CompletionMarks) {
		rapidjson::Value array(rapidjson::kArrayType);
		for (int i = 0; i < 15; ++i) {
			array.PushBack(kv.second[i], doc.GetAllocator());
		}
		doc.AddMember(rapidjson::Value(kv.first.c_str(), kv.first.size(), doc.GetAllocator()),
			array, doc.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	ofstream ofs(jsonpath);
	ofs << buffer.GetString() << std::endl;
	printf("[REPENTOGON] Completion Marks saved to: %s \n", jsonpath.c_str());
}

void LoadCompletionMarksFromJson() {
	CompletionMarks.clear();
	ifstream ifs(jsonpath);
	if (!ifs.good()) {
		printf("[REPENTOGON] No marks for saveslot in: %s \n", jsonpath.c_str());
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
		if (CompletionMarks.find(key) == CompletionMarks.end()) {
			CompletionMarks[key] = array<int, 15>{};
		}

		auto& value = it->value;
		if (!value.IsArray() || value.Size() != 15) {
			continue;
		}

		for (int i = 0; i < 15; ++i) {
			if (!value[i].IsInt()) {
				continue;
			}
			CompletionMarks[key][i] = value[i].GetInt();
		}
	}
	printf("[REPENTOGON] Completion Marks loaded from: %s \n", jsonpath.c_str());
}

int selectedchar = 0;
int ischartainted = false;
int hidemarks = false;

unordered_map<string, string> GetPlayerDataForMarks(int playerid) {
	unordered_map<string, string> playerdata = XMLStuff.PlayerData.players[playerid];
	if (playerdata.count("completionparent") > 0) {
		string aidx = playerdata["sourceid"] + "-" + playerdata["completionparent"];
		if (XMLStuff.PlayerData.playerbynamemod.count(aidx) == 0) {
			if (XMLStuff.PlayerData.playerbyname.count(playerdata["completionparent"]) > 0) {
				return XMLStuff.PlayerData.players[XMLStuff.PlayerData.playerbyname[playerdata["completionparent"]]];
			}
		}
		else {
			return XMLStuff.PlayerData.players[XMLStuff.PlayerData.playerbynamemod[aidx]];
		}
	}
	return playerdata;
}

string stolower(char* str)
{
	string s = string(str);
	for (auto& c : s) {
		c = tolower(c);
	}
	return s;
}

array<int, 15> GetMarksForPlayer(int playerid,ANM2* anm) {
	array<int, 15> marks;
	if (XMLStuff.PlayerData.players.count(playerid) > 0) {
		unordered_map<string, string> playerdata = GetPlayerDataForMarks(playerid);
		string idx = playerdata["sourceid"] + "-" + playerdata["name"];
		ischartainted = false;
		if (playerdata.count("bskinparent") > 0) {
			idx = idx + "-Tainted-";
			ischartainted = true;
		}
		hidemarks = false;
		if (playerdata.count("nomarks") > 0) {
			if (strcmp(stolower((char *)playerdata["sourceid"].c_str()).c_str(), "false")) {
				hidemarks = true;
			}
		}
		marks = CompletionMarks[idx];
		if (ischartainted) {
			anm->SetLayerFrame(0, marks[CompletionTypeRender[0]] + 3);
		}
		else {
			anm->SetLayerFrame(0, marks[CompletionTypeRender[0]]);
		}
		for (int i = 1; i <= 11; i++) {
			if ((i == 8) && marks[ULTRA_GREEDIER]) {
				anm->SetLayerFrame(i, marks[CompletionTypeRender[i]]);
			}
			else {
				anm->SetLayerFrame(i, marks[CompletionTypeRender[i]]);
			}
		}

		anm->Update();

		return marks;
	}
	return { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
}


HOOK_METHOD(Manager, LoadGameState, (int saveslot) -> void) {
	jsonpath = string((char*)&g_SaveDataPath) + "Repentogon/moddedcompletionmarks" + to_string(saveslot) +".json";

	LoadCompletionMarksFromJson();
	if (!initializedrendercmpl){
		InitMarkRenderTypes();
	}
	super(saveslot);
}
/*
HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	LoadCompletionMarksFromJson();
	InitMarkRenderTypes();
	super();
}
*/

HOOK_METHOD_PRIORITY(Manager, RecordPlayerCompletion,100, (int eEvent) -> void) {
	int numplayers = g_Game->GetNumPlayers();
	for(int i = 0;i<numplayers;i++){
		int playertype = g_Game->GetPlayer(i)->GetPlayerType();
	
		unordered_map<string, string> playerdata = GetPlayerDataForMarks(playertype);
		string idx = playerdata["sourceid"] + "-" + playerdata["name"];
		if (playerdata.count("bskinparent") > 0) {
			idx = idx + "-Tainted-";
		}
		if (CompletionMarks.count(idx) == 0) {
			CompletionMarks[idx] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		}
		int difficulty = g_Game->GetDifficulty();
		int marktype = 1;
		if ((difficulty == 1) || (difficulty == 3)) { //1:hard 3:greedier
			marktype = 2;
		}
		CompletionMarks[idx][eEvent] = marktype;
	}
	SaveCompletionMarksToJson();
	super(eEvent);
}

HOOK_METHOD(PauseScreen, Render, () -> void) {
	super();
	int playertype = g_Game->GetPlayer(0)->GetPlayerType();
	if ((playertype > 40) && (this->status !=2)) {
		NullFrame* nul = this->GetANM2()->GetNullFrame("CompletionWidget"); 
		Vector* widgtpos = nul->GetPos();
		Vector* widgtscale = nul->GetScale();
		CompletionWidget* cmpl = this->GetCompletionWidget();

		ANM2* anm = cmpl->GetANM2();

		array marks = GetMarksForPlayer(playertype,anm);
		if (!hidemarks){
			cmpl->CharacterId = playertype;
			cmpl->Render(new Vector((g_WIDTH * 0.6) + widgtpos->x, (g_HEIGHT * 0.5) +widgtpos->y), widgtscale);
		}
	}
}


HOOK_STATIC(ModManager, RenderCustomCharacterMenu, (int CharacterId, Vector* RenderPos, ANM2* DefaultSprite) -> void, __stdcall) {
	selectedchar = CharacterId;
	super(CharacterId, RenderPos, DefaultSprite);
}
HOOK_METHOD(Menu_Character, Render, () -> void) {
	super();
	CompletionWidget* cmpl = this->GetCompletionWidget();
	if(this->charaslot > 17){
		
		Vector* ref = (Vector *)(g_MenuManager + 60);
		Vector* cpos = new Vector(ref->x - 80, ref->y + 894);
		ANM2* anm = cmpl->GetANM2();
		
		array marks = GetMarksForPlayer(selectedchar,anm);
		if (!hidemarks) {
			cmpl->CharacterId = selectedchar;
			cmpl->Render(new Vector(ref->x + 80, ref->y + 860), new Vector(1, 1));
		}

	}
}


