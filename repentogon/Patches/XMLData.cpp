#if !defined(XMLDATALoaded)
#define XMLDATALoaded true

#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <memory>

#include <cstring>

#include "XMLData.h"

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "mologie_detours.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "../ImGuiFeatures/LogViewer.h"
#include <lua.hpp>
#include "LuaCore.h"
#include <filesystem>
#include <regex>

using namespace rapidxml;
using namespace std;

char* bosspoolsxml; //caching this ffs
bool itempoolerror = false;
bool xmlsloaded = false;

char* lastmodid = "BaseGame";
bool iscontent = false;
bool isitemmetadata = false;
bool no = false;
vector<string> xmlerrors;
string currpath;

unordered_map<string, int> xmlnodeenum;
unordered_map<string, int> xmlmaxnode;
unordered_map<string, int> xmlfullmerge;
XMLData XMLStuff;


void ClearXMLData() {
	XMLStuff.PlayerData->Clear();
	XMLStuff.ItemData->Clear();
	XMLStuff.NullItemData->Clear();
	XMLStuff.PoolData->Clear();
	XMLStuff.TrinketData->Clear();
	XMLStuff.MusicData->Clear();
	XMLStuff.SoundData->Clear();
	XMLStuff.PillData->Clear();
	XMLStuff.CardData->Clear();
	XMLStuff.ChallengeData->Clear();
	XMLStuff.NightmareData->Clear();
	XMLStuff.CostumeData->Clear();
	XMLStuff.NullCostumeData->Clear();
	XMLStuff.CurseData->Clear();
	XMLStuff.WispData->Clear();
	XMLStuff.WispColorData->Clear();
	XMLStuff.ModData->Clear();
}

//lazy mem clears to prevent leaks
void mclear(char* x) {
	if (x != nullptr) {
		delete[] x;
	}
}
void mclear(XMLAttributes* x) {
	if (x != nullptr) {
		x->clear();
	}
}
void mclear(xml_document<char>* x) {
	if (x != nullptr) {
		x->clear();
	}
}
//lazy mem clears to prevent leaks

void CharToChar(char** dest, char* source) {
	size_t sourceSize = strlen(source) + 1;
	char* destination = new char[sourceSize];
	strcpy(destination, source);
	mclear(*dest);
	*dest = destination;
}

int toint(const string &str) {
	if (str.length() > 0) {
		char* endPtr;
		int returnval = strtol(str.c_str(), &endPtr, 0);
		if (endPtr != "\0") {
			return returnval;
		}
	}
		return 0;
}

XMLAttributes BuildGenericEntry(xml_node<char>* node) {
	XMLAttributes mod;
		for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
		{
			mod[attr->name()] = attr->value();
		}
	return mod;
}


void ProcessModEntry(char* xmlpath,ModEntry* mod) {
	if (mod != NULL) { //it is null when its loading vanilla stuff
		lastmodid = mod->GetId();
	}
	else {
		lastmodid = "BaseGame";
	}
	if ((stringlower(xmlpath).find("/content/") != string::npos) || (stringlower(xmlpath).find("/content-dlc3/") != string::npos)) {
		iscontent = true;
	}
	else {
		iscontent = false;
	}
	if ((string(lastmodid).length() == 0) || ((string(lastmodid)== "BaseGame") && iscontent)) {
		string path = string(xmlpath);
		int first = path.find("/mods/") + 6;
		int last = path.find("/content");
		if (!iscontent) {
			last = path.find("/resources");
		}
		else if (last <= 0) {
			last = path.find("/content-dlc3");
		}
		path = path.substr(first, last - first); //when the id is null(which it can fucking be) just use the folder name as ID...
		lastmodid = new char[path.length() + 1]; //this is the sort of stuff I dont like about C++
		strcpy(lastmodid, path.c_str());
	}
	//printf("path: %s (mod:%s iscontent:%d) \n", xmlpath,lastmodid,iscontent);
	logViewer.AddLog("[REPENTOGON]", "Mod ID: %s \n", lastmodid);
	
}

ModEntry* GetModEntryById(const string &Id) {
	for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if ((mod->GetId() != NULL) && (strcmp(Id.c_str(), mod->GetId()) == 0)) {
			return mod;
		}
	}
	return NULL;
}

ModEntry* GetModEntryByName(const string &Name) {
	for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if (strcmp(Name.c_str(), mod->GetName().c_str())==0) {
			return mod;
		}
	}
	return NULL;
}

ModEntry* GetModEntryByDir(const string &Dir) {
	for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if (strcmp(Dir.c_str(), mod->GetDir().c_str())==0) {
			return mod;
		}
	}
	return NULL;
}

ModEntry* GetModEntryByContentPath(const string &path) {
	if ((path.find("/content/") != string::npos) && (path.find("/mods/") != string::npos)) {
		std::regex regex("/mods/(.*?)/content/");
		std::smatch match;
		if (std::regex_search(path, match, regex)) {
			if (XMLStuff.ModData->byfolder.count(match.str(1)) > 0) {
				return XMLStuff.ModData->modentries[XMLStuff.ModData->byfolder[match.str(1)]];
			}
		}
	}
	else if ((path.find("/content-dlc3/") != string::npos) && (path.find("/mods/") != string::npos)) {
		std::regex regex("/mods/(.*?)/content-dlc3/");
		std::smatch match;
		if (std::regex_search(path, match, regex)) {
			if (XMLStuff.ModData->byfolder.count(match.str(1)) > 0) {
				return XMLStuff.ModData->modentries[XMLStuff.ModData->byfolder[match.str(1)]];
			}
		}
	}
	return NULL;
}

void UpdateXMLModEntryData() {
	for (ModEntry* entry : g_Manager->GetModManager()->_mods) {
		int idx = 0;
		XMLAttributes mod;
		if ((entry->GetId() != NULL) &&(string(entry->GetId()).length() > 0)) { idx = XMLStuff.ModData->byid[string(entry->GetId())]; }
		else { idx = XMLStuff.ModData->byname[entry->GetName()];}
		mod = XMLStuff.ModData->nodes[idx];
		mod["realdirectory"] = entry->GetDir();
		mod["fulldirectory"] = std::filesystem::current_path().string() + "/mods/" + entry->GetDir();
		
		if (entry->IsEnabled()) { mod["enabled"] = "true";}
		else { mod["enabled"] = "false"; }
		XMLStuff.ModData->nodes[idx] = mod;
		XMLStuff.ModData->modentries[idx] = entry;
		XMLStuff.ModData->byfolder[mod["realdirectory"]] = idx;
		XMLStuff.ModData->byfullpath[mod["fulldirectory"]] = idx;
		
	}
}

//Shameless chatgpt copypaste function
string getFileName(const string& filePath) {
	// Find the position of the last directory separator
	size_t lastSeparator = filePath.find_last_of("/\\");

	// Extract the substring after the last separator
	std::string fileName = filePath.substr(lastSeparator + 1);

	// Remove the file extension
	size_t extensionPos = fileName.find_last_of(".");
	if (extensionPos != std::string::npos) {
		fileName = fileName.substr(0, extensionPos);
	}

	return fileName;
}
//end of Shameless chatgpt copypaste function


//Custom BigBook Anims
HOOK_METHOD(ItemOverlay, Show, (int eOverlayID, int delay, Entity_Player* player)-> void) {
	if ((eOverlayID > 46) && (XMLStuff.GiantBookData->nodes.find(eOverlayID) != XMLStuff.GiantBookData->nodes.end())){
		super(1, delay, player);
		this->_overlayID = eOverlayID;
		ANM2* sprite = this->GetSprite();
		XMLAttributes att = XMLStuff.GiantBookData->nodes[eOverlayID];
		if (att.find("anm2") != att.end()) {
			sprite->Load(att["anm2root"] + att["anm2"],true);
			sprite->LoadGraphics(true);
			sprite->Update();
		}
		if (att.find("gfx") != att.end()) {
			sprite->ReplaceSpritesheet(0, att["anm2root"] + att["gfx"]);
			sprite->LoadGraphics(true);
			sprite->Update();
		}
		if (XMLStuff.GiantBookData->childs.find(eOverlayID) != XMLStuff.GiantBookData->childs.end()) {
			if (XMLStuff.GiantBookData->childs[eOverlayID].find("layer") != XMLStuff.GiantBookData->childs[eOverlayID].end()) {
				XMLChilds childs = XMLStuff.GiantBookData->childs[eOverlayID];
				for each (XMLAttributes  att2 in childs["layer"])
				{
					if (att2.find("name") != att.end()) {
						for (int i = sprite->GetLayerCount() - 1; i >= 0; i--) {
							LayerState* ls = sprite->GetLayer(i);
							if (ls->GetName().compare(att2["name"]) == 0) {
								ls->_color._tint[0] = (float)toint(att2["r"]) / 255;
								ls->_color._tint[1] = (float)toint(att2["g"]) / 255;
								ls->_color._tint[2] = (float)toint(att2["b"]) / 255;
								ls->_color._tint[3] = (float)toint(att2["a"]) / 255;
							}
						}
					}
				}
				sprite->LoadGraphics(true);
				sprite->Update();
			}
		}
		if (att.find("anim") != att.end()) {
			sprite->Play(att["anim"].c_str(), false);
		}
		else {
			sprite->Play("Appear", true);
		}
		return;
	}
	super(eOverlayID, delay, player);
}

//Custom BigBook Anims

//Cutscene XML hijack
string ogcutscenespath;
int queuedhackyxmlvalue = 0;
int queuedhackyxmltarget = 0;
int queuedhackyxmlmaxval = 0;

bool HasOneModOn() {
	for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if (mod->IsEnabled()) {
			return true;
		}
	}
	return false;
}

HOOK_METHOD(Cutscene, Init, (char* xmlfilepath)-> void) {
	if ((!g_Manager->GetOptions()->ModsEnabled()) || !HasOneModOn()) { return super(xmlfilepath); }
	if (ogcutscenespath.length() == 0) {
		ogcutscenespath = string(xmlfilepath);
	}
	super(xmlfilepath);
}


static std::vector<std::string> ParseCommand2(const std::string &command, int size = 0) {
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
int hijackedcutscene = 0;
HOOK_METHOD(Console, RunCommand, (std_string& in, std_string* out, Entity_Player* player)-> void) {
	if (in.rfind("cutscene", 0) == 0) {
		std::vector<std::string> cmdlets = ParseCommand2(in, 2);
		int id = stoi(cmdlets[1]);
		if (id > 26) {
			hijackedcutscene = id;
			super(std::string("cutscene 1"), out, player);
			return;
		}
	}
	super(in, out, player);
}

HOOK_METHOD(Cutscene, Show, (int cutsceneid)-> void) {
	if ((!g_Manager->GetOptions()->ModsEnabled()) || !HasOneModOn()) { return super(cutsceneid); }
	queuedhackyxmlvalue = cutsceneid;
	queuedhackyxmltarget = 1;
	queuedhackyxmlmaxval = 26;
	if (hijackedcutscene > 0) { queuedhackyxmlvalue = hijackedcutscene; hijackedcutscene = 0; }
	char* xml = new char[ogcutscenespath.length() + 1];
	strcpy(xml, ogcutscenespath.c_str()); //Will change this so it only makes this swap thing when the cutscene is not present on the already loaded xml later, but I'll leave it like this now for testing the method without having people needing to manually play custom cutscenes 
	Init(xml);
	super(1);
	mclear(xml);
	queuedhackyxmltarget = 0;
	queuedhackyxmlvalue = 0;
}

//Cutscene XML Hijack

//Stages XML Hijack
unordered_map<int, tuple<int, int> > stageidtotuple;
void initstagetotuple() {
	//Vanilla
	stageidtotuple[1] = { 1,0 };
	stageidtotuple[2] = { 1,1 };
	stageidtotuple[3] = { 1,2 };
	stageidtotuple[4] = { 3,0 };
	stageidtotuple[5] = { 3,1 };
	stageidtotuple[6] = { 1,2 };
	stageidtotuple[7] = { 5,0 };
	stageidtotuple[8] = { 5,1 };
	stageidtotuple[9] = { 5,2 };
	stageidtotuple[10] = { 7,0 };
	stageidtotuple[11] = { 7,1 };
	stageidtotuple[12] = { 7,2 };
	stageidtotuple[13] = { 9,0 };
	stageidtotuple[14] = { 10,0 };
	stageidtotuple[15] = { 10,1 };
	stageidtotuple[16] = { 11,0 };
	stageidtotuple[17] = { 11,1 };
	stageidtotuple[26] = { 12,0 };
	//Vanilla
	//Greed
	stageidtotuple[24] = { 6,0 };
	stageidtotuple[25] = { 7,0 };
	//Greed
	//Repentance
	stageidtotuple[27] = { 1,4 };
	stageidtotuple[28] = { 1,5 };
	stageidtotuple[29] = { 3,4 };
	stageidtotuple[30] = { 3,5 };
	stageidtotuple[31] = { 5,4 };
	stageidtotuple[32] = { 5,5 };
	stageidtotuple[33] = { 7,4 };
	stageidtotuple[35] = { 13,0 };
	//Repentance
}

bool IsOnSecondFloor() {
	int stageid = g_Game->_stage;
	return (stageid == 2) || (stageid == 4) || (stageid == 6) || (stageid == 8) || (g_Game->_curses & (1 << 1)); //has curse XL
}

tuple<int, int> GetSetStage(int stageid,bool secondfloor) {
	if (stageidtotuple.empty()) {
		initstagetotuple();
	}
	if (stageidtotuple.count(stageid) > 0) {
		tuple<int, int> ret = stageidtotuple[stageid];
		if (secondfloor && (get<0>(ret) < 9)) {
			ret = { get<0>(ret) + 1,get<1>(ret) };
		}
		return ret;
	}
	else {
		return { stageid,0 };
	}
}

string ogstagespath;
int queuedstage = 0;
int queuedalt = 0;
int lastrequest = 0;
/*
HOOK_STATIC(RoomConfig, GetStageID, (unsigned int LevelStage, unsigned int StageType, unsigned int Mode)-> unsigned int, __cdecl) {
	unsigned int stageid = super(LevelStage,StageType, Mode);
	//printf("getstage: %d \n", stageid);
	return stageid;
}
*/
int lastparentstage=0;
int setstageoverloadid = 0;
int setstageoverloadalt = 0;
/*
HOOK_METHOD(Level, SetStage, (int a, int b)-> void) {
	int stageid = a;
	int alt = b;
	if (setstageoverloadid > 0) { stageid = setstageoverloadid; setstageoverloadid = 0; }
	if (setstageoverloadalt > 0) { alt = setstageoverloadalt; setstageoverloadalt = 0; }
	char* xml = new char[ogstagespath.length() + 1];
	strcpy(xml, ogstagespath.c_str());
	tuple<int,int> idx = { stageid,alt };
	if (XMLStuff.StageData->bystagealt.count(idx) > 0) {
		XMLAttributes* targetstage = new XMLAttributes(XMLStuff.StageData->nodes[XMLStuff.StageData->bystagealt[idx]]);
		int parentstage = toint((*targetstage)["basestage"]);
		if (parentstage == 0) { parentstage = 1; }
		queuedhackyxmlvalue = stageid;
		queuedhackyxmltarget = parentstage;
		queuedhackyxmlmaxval = 36;
		//if (lastparentstage != stageid) {
			for (int i = 0; i <= 36; i++) {
				g_Game->GetRoomConfig()->UnloadStage(i);
			}
			g_Game->GetRoomConfig()->LoadStages(xml);
		//}
		printf("setstageX: %d %d  \n", stageid, alt);
		tuple<int, int> setstg = GetSetStage(parentstage, IsOnSecondFloor());
		super(get<0>(setstg), get<1>(setstg));
		printf("done");
		lastparentstage = get<0>(setstg);
		setstg;
	}
	else if (lastparentstage == get<0>(GetSetStage(a, IsOnSecondFloor()))){
		//no = true;
		//for (int i = 0; i <= 36; i++) {
			g_Game->GetRoomConfig()->UnloadStage(stageid);
		//}
		g_Game->GetRoomConfig()->LoadStages(xml);
		super(stageid, alt);
		lastparentstage = 0;
	}
	else {
		super(stageid, alt);
	}
	queuedhackyxmlvalue = 0;
	queuedhackyxmltarget = 0;
	mclear(xml);
}
*/
/*
HOOK_METHOD(RoomConfig, LoadStages, (char* xmlpath)-> void) {
	if (ogstagespath.length() == 0) {
		ogstagespath = xmlpath;
	}
	printf("stagexml: %s \n", xmlpath);
	super(xmlpath);
}
*/
tuple<int, int> ConsoleStageIdToTuple(const string &input) {
	string* numberPart = new string("");
	int letterValue = 0;
	for (char c : input) {
		if (isdigit(c)) {
			*numberPart += c;
		}
		else {
			if (isalpha(c)) {
				int x = toint(*numberPart);
				delete numberPart;
				return { x ,c - 'a' + 1 };
			}
		}
	}
	int y = toint(*numberPart);
	delete numberPart;
	return { y ,0 };
}

HOOK_METHOD(Console, RunCommand, (std_string& in, std_string* out, Entity_Player* player)-> void) {
	if (in.rfind("stage", 0) == 0) {
		std::vector<std::string> cmdlets = ParseCommand2(in, 2);
		tuple<int, int> id = ConsoleStageIdToTuple(cmdlets[1]);
		if (XMLStuff.StageData->bystagealt.count(id) == 0) { //stage 14 works without intervention, lol
			super(in, out, player);
			return;
		}
		else {
			//super(in, out, player); //we still run it just in case I dunno, does nothing anyway
			setstageoverloadid = get<0>(id);
			setstageoverloadalt = get<1>(id);
			g_Game->GetConsole()->RunCommand(string("stage 1"), out, player);
			return;
		}
	}
	super(in, out, player);
}
//Stages XML Hijack


//backdrop hijack (commented until we figure out backdropconfig stucture)
/*
int lasthackybackid = -1;
HOOK_METHOD(Backdrop, Init, (uint32_t bcktype, bool loadgraphics)-> void) {
	if ((XMLStuff.BackdropData->nodes.count(bcktype) > 0) && ((bcktype == 1) || (bcktype > 60))) {
		if (lasthackybackid != bcktype) {
			XMLAttributes node = XMLStuff.BackdropData->nodes[bcktype];
			super(1, true);
			//for (int i = 1; i <= 60; i++) {
				//printf("dasfdsaf %s \n", this->configurations[i].gfx.c_str());
			//}
			
			//for (int i = 0; i <= 60; i++) {
			/*
			this->configurations[1].gfx = node["gfx"];
			
			this->configurations[1].walls = toint(node["walls"]);
			this->configurations[1].wallVariants = toint(node["wallvariants"]);
			this->configurations[1].floors = toint(node["floors"]);
			this->configurations[1].floorVariants = toint(node["floorvariants"]);
			this->configurations[1].lFloorGfx = node["lfloorgfx"];
			this->configurations[1].nFloorGfx = node["nfloorgfx"];
			this->configurations[1].waterGfx = node["watergfx"];
			//this->configurations[1].reversewatergfx = node["reversewatergfx"]; //missing
			this->configurations[1].props = node["props"];
			this->configurations[1].rocks = node["rocks"];
			this->configurations[1].pit = node["pit"];
			this->configurations[1].waterPit = node["waterpit"];
			this->configurations[1].bridge = node["bridge"];
			this->configurations[1].door = node["door"];
			this->configurations[1].holeInWall = node["holeinwall"];
			this->configurations[1].waterPitsMode = toint(node["waterpitsmode"]);
			*/
/*
			super(1, true);
			//}
			lasthackybackid = bcktype;
		}
		else {
			super(1, loadgraphics);
		}
	}
	else {
	super(bcktype, loadgraphics);
	}
}
void SwapBackdrop(int source, int target) {
	
}
*/
//
//#include <time.h>
bool initedxmlenums = false;
void ProcessXmlNode(xml_node<char>* node) {
	if (!node || no || xmlsloaded) { return; }
	if (queuedhackyxmlvalue > 0) { return; }
	//clock_t tStart = clock();
	//if (currpath.length() > 0) { printf("Loading: %s \n", currpath.c_str()); }
	Manager* manager = g_Manager;
	StringTable* stringTable = manager->GetStringTable();
	uint32_t unk;
	string middleman = stringlower(node->name());
	const char* nodename = middleman.c_str(); 
	if (!initedxmlenums) {
		initedxmlenums = true; initxmlnodeenum(); initxmlmaxnodeenum(); initxmlfullmergelist();
	}
	if (xmlnodeenum.find(middleman) == xmlnodeenum.end()) { return; }
	int nodetypeid = xmlnodeenum[middleman];
	xml_node<char>* daddy;
	xml_node<char>* babee;
	int id = 0;
	int idnull = 1;
	switch(nodetypeid){
	case 1: //entity
		for (xml_node<char>* auxnode = node; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes entity;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				entity[stringlower(attr->name())] = attr->value();
			}
			daddy = node->parent();
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				entity[stringlower(attr->name())] = string(attr->value());
			}
			int type = toint(entity["id"]);
			int var = 0;
			int sub = 0;


			if (entity["subtype"].length() > 0) { sub = toint(entity["subtype"]); }
			tuple idx = { type, var, sub };
			if (entity["variant"].length() > 0) { var = toint(entity["variant"]); idx = { type, var, sub }; }
			else if ((type < 10) || (type >= 1000)) {
				while (XMLStuff.EntityData->nodes.find(idx) != XMLStuff.EntityData->nodes.end()) {
					var += 1;
					idx = { type, var, sub };
				}
			}


			if (iscontent && (XMLStuff.EntityData->nodes.find(idx) != XMLStuff.EntityData->nodes.end())) {
				XMLAttributes collider = XMLStuff.EntityData->nodes[idx];
				XMLAttributes collidermod = XMLStuff.ModData->nodes[XMLStuff.ModData->byid[collider["sourceid"]]];
				XMLAttributes  lastmod = XMLStuff.ModData->nodes[XMLStuff.ModData->byid[lastmodid]];
				//g_Game->GetConsole()->PrintError(toIsaacString("[XML] The entity:" + entity["name"] + "(From: " + lastmodid + ") collides with " + collider["name"] + "from (" + collidermod["name"] + ")"));
				if (false) {
					printf("[XML] The entity: %s(From: %s) collides with %s (from %s) \n", entity["name"].c_str(), lastmod["name"].c_str(), collider["name"].c_str(), collidermod["name"].c_str());
				}
				//Conflict resolve emulation begin
				if ((type < 10) || (type >= 1000)) {
					do {
						var += 1;
						idx = { type, var, sub };
					} while (XMLStuff.EntityData->nodes.find(idx) != XMLStuff.EntityData->nodes.end());
				}
				else {
					do {
						type += 1;
						idx = { type, var, sub };
					} while (XMLStuff.EntityData->nodes.find(idx) != XMLStuff.EntityData->nodes.end());
				}
				//Conflict resolve emulation end
			}
			entity["id"] = to_string(type);
			entity["type"] = to_string(type);
			entity["variant"] = to_string(var);
			entity["sourceid"] = lastmodid;
			if (entity["name"].find("#") != string::npos) {
				entity["untranslatedname"] = entity["name"];
				entity["name"] = string(stringTable->GetString("Entities", 0, entity["name"].substr(1, entity["name"].length()).c_str(), &unk));
				if (entity["name"].compare("StringTable::InvalidKey") == 0) { entity["name"] = entity["untranslatedname"]; }
			}
			if (entity.count("customtags") > 0) {
				// Convert the customtags attribute to lowercase and parse each individual tag.
				const string customtagsstr = stringlower(entity["customtags"].c_str());
				if (!customtagsstr.empty()) {
					stringstream tagstream(customtagsstr);
					string tag;
					while (getline(tagstream, tag, ' ')) {
						if (!tag.empty()) {
							XMLStuff.EntityData->customtags[idx].insert(tag);
						}
					}
				}
			}
			XMLStuff.EntityData->ProcessChilds(auxnode, idx);
			XMLStuff.EntityData->nodes[idx] = entity;
			XMLStuff.EntityData->byorder[XMLStuff.EntityData->nodes.size()] = idx;
			XMLStuff.EntityData->byname[entity["name"]] = idx;
			XMLStuff.EntityData->bytype[entity["id"]] = idx;
			XMLStuff.EntityData->bybossid[entity["bossid"]] = idx;
		}
	break;
	case 2: //player 
		id = 0;
		daddy = node->parent();
		for (xml_node<char>* auxnode = node; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes player;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				player[stringlower(attr->name())] = string(attr->value());
			}
			daddy = node->parent();
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				player[stringlower(attr->name())] = string(attr->value());
			}
			string oldid = player["id"];
			if ((player.find("id") != player.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(player["id"]);
			}
			else {
				if (player.find("id") != player.end()) { player["relativeid"] = player["id"]; }
				XMLStuff.PlayerData->maxid += 1;
				player["id"] = to_string(XMLStuff.PlayerData->maxid);
				id = XMLStuff.PlayerData->maxid;
			}
			if (id > XMLStuff.PlayerData->maxid) {
				XMLStuff.PlayerData->maxid = id;
			}
			if (oldid.length() > 0) { player["id"] = oldid; }

			if (player["name"].find("#") != string::npos) {
				player["untranslatedname"] = player["name"];
				player["name"] = string(stringTable->GetString("Players", 0, player["name"].substr(1, player["name"].length()).c_str(), &unk));
				if (player["name"].compare("StringTable::InvalidKey") == 0) { player["name"] = player["untranslatedname"]; }
			}

			if (player["birthright"].find("#") != string::npos) {
				player["untranslatedbirthright"] = player["birthright"];
				player["birthright"] = string(stringTable->GetString("Players", 0, player["birthright"].substr(1, player["birthright"].length()).c_str(), &unk));
				if (player["birthright"].compare("StringTable::InvalidKey") == 0) { player["birthright"] = player["untranslatedbirthright"]; }
			}

			if ((player.find("bskinparent") != player.end()) || ((id > 20) && (id <= 40))){
				XMLStuff.PlayerData->byname[player["name"] + "-Tainted-"] = id; }
			else{ 
				XMLStuff.PlayerData->byname[player["name"]] = id; }

			if (player.find("bskinparent") != player.end()) {
				XMLStuff.PlayerData->bynamemod[player["name"] + lastmodid + "-Tainted-"] = id;
			}
			else {
				XMLStuff.PlayerData->bynamemod[player["name"] + lastmodid] = id;
			}

			player["sourceid"] = lastmodid;
			if (player.find("relativeid") != player.end()) { XMLStuff.PlayerData->byrelativeid[lastmodid + player["relativeid"]] = id;}
			//printf("playa: %d (%s) \n", id, player["name"].c_str());
			XMLStuff.PlayerData->ProcessChilds(auxnode, id);
			XMLStuff.PlayerData->nodes[id] = player;
			XMLStuff.PlayerData->bymod[lastmodid].push_back(id);
			XMLStuff.ModData->players[lastmodid] += 1;
			}
	break;
	case 3: //pocketitems
		id = 1;
		for (xml_node<char>* auxnode = node->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			if ((strcmp(auxnode->name(), "card") == 0) || (strcmp(auxnode->name(), "rune") == 0)) {
				XMLAttributes card;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					card[stringlower(attr->name())] = string(attr->value());
				}
				if (card.count("id") && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) { 
					id = toint(card["id"]);
				}
				else {
					if (card.count("id") > 0) { card["relativeid"] = card["id"]; }
					XMLStuff.CardData->maxid +=  1;
					card["id"] = to_string(XMLStuff.CardData->maxid);
					id = XMLStuff.CardData->maxid;
				}
				if (id > XMLStuff.CardData->maxid) {
					XMLStuff.CardData->maxid = id;
				}
				card["sourceid"] = lastmodid;

				if (card["name"].find("#") != string::npos) {
					card["untranslatedname"] = card["name"];
					card["name"] = string(stringTable->GetString("PocketItems", 0, card["name"].substr(1, card["name"].length()).c_str(), &unk));
					if (card["name"].compare("StringTable::InvalidKey") == 0) { card["name"] = card["untranslatedname"]; }
				}

				if (card["description"].find("#") != string::npos) {
					card["untranslateddescription"] = card["name"];
					card["description"] = string(stringTable->GetString("PocketItems", 0, card["description"].substr(1, card["description"].length()).c_str(), &unk));
					if (card["description"].compare("StringTable::InvalidKey") == 0) { card["description"] = card["untranslateddescription"]; }
				}
				if (card.count("relativeid") > 0) { XMLStuff.CardData->byrelativeid[lastmodid + card["relativeid"]] = id; }
				XMLStuff.CardData->ProcessChilds(auxnode, id);
				XMLStuff.CardData->nodes[id] = card;
				XMLStuff.CardData->bynamemod[card["name"] + lastmodid] = id;
				XMLStuff.CardData->bymod[lastmodid].push_back(id);
				XMLStuff.CardData->byfilepathmulti.tab[currpath].push_back(id);
				XMLStuff.CardData->byname[card["name"]] = id;
				XMLStuff.ModData->cards[lastmodid] += 1;
			}
			else if ((strcmp(auxnode->name(), "pilleffect") == 0)) {

				XMLAttributes pill;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					pill[stringlower(attr->name())] = string(attr->value());
				}
				if ((pill.find("id") != pill.end()) && ((strcmp(lastmodid,"BaseGame") == 0) || !iscontent)) { 
					id = toint(pill["id"]);
				}
				else {
					if (pill.find("id") != pill.end()) { pill["relativeid"] = pill["id"]; }
					XMLStuff.PillData->maxid += 1;
					pill["id"] = to_string(XMLStuff.PillData->maxid);
					id = XMLStuff.PillData->maxid;
				}
				if (id > XMLStuff.PillData->maxid) {
					XMLStuff.PillData->maxid = id;
				}
				pill["sourceid"] = lastmodid;

				if (pill["name"].find("#") != string::npos) {
					pill["untranslatedname"] = pill["name"];
					pill["name"] = string(stringTable->GetString("PocketItems", 0, pill["name"].substr(1, pill["name"].length()).c_str(), &unk));
					if (pill["name"].compare("StringTable::InvalidKey") == 0) { pill["name"] = pill["untranslatedname"]; }
				}

				if (pill["description"].find("#") != string::npos) {
					pill["untranslateddescription"] = pill["name"];
					pill["description"] = string(stringTable->GetString("PocketItems", 0, pill["description"].substr(1, pill["description"].length()).c_str(), &unk));
					if (pill["description"].compare("StringTable::InvalidKey") == 0) { pill["description"] = pill["untranslateddescription"]; }
				}
				if (pill.find("relativeid") != pill.end()) { XMLStuff.PillData->byrelativeid[lastmodid + pill["relativeid"]] = id; }
				XMLStuff.PillData->ProcessChilds(auxnode, id);
				XMLStuff.PillData->byname[pill["name"]] = id;
				XMLStuff.PillData->bynamemod[pill["name"] + lastmodid] = id;
				XMLStuff.PillData->bymod[lastmodid].push_back(id);
				XMLStuff.PillData->byfilepathmulti.tab[currpath].push_back(id);
				XMLStuff.PillData->nodes[id] = pill;
				XMLStuff.ModData->pills[lastmodid] += 1;
			}
		}
	break;
	case 4: //items
		daddy = node;
		id = 1;
		for (xml_node<char>* auxnode = node->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			string meh = stringlower(auxnode->name());
			const char* auxnodename = meh.c_str();
			if ((strcmp(auxnodename, "active") == 0) || (strcmp(auxnodename, "passive") == 0) || (strcmp(auxnodename, "familiar") == 0) || (strcmp(auxnodename, "item") == 0)) {
				XMLAttributes item;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					item[stringlower(attr->name())] = string(attr->value());
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					item[stringlower(attr->name())] = string(attr->value());
				}
				if (isitemmetadata) { //items_metadata lazy bypass
					if (item.count("id") > 0) {
						if (strcmp(lastmodid, "BaseGame") != 0) {
							item["id"] = to_string(XMLStuff.ItemData->byrelativeid[lastmodid + item["id"]]);
						}
						id = toint(item["id"]);
						XMLAttributes s = XMLStuff.ItemData->nodes[id];
						s["tags"] = item["tags"];
						s["quality"] = item["quality"];
						XMLStuff.ItemData->nodes[id] = s;
					}
				}
				else { 
					string oldid =  item["id"];
					if ((item.find("id") != item.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
						id = toint(item["id"]);
					}
					else {
						if (item.find("id") != item.end()) { item["relativeid"] = item["id"]; }
						XMLStuff.ItemData->maxid += 1;
						item["id"] = to_string(XMLStuff.ItemData->maxid);
						id = XMLStuff.ItemData->maxid;
					}
					if (id > XMLStuff.ItemData->maxid) {
						XMLStuff.ItemData->maxid = id;
					}
					if (oldid.length() > 0) {
						item["id"] = oldid;
					}

					item["sourceid"] = lastmodid;
					item["type"] = auxnodename;
					//printf("iname: %s // %s (%s) \n", item["name"].c_str(), item["description"].c_str(), item["id"].c_str());
					if (item["name"].find("#") != string::npos) {
						item["untranslatedname"] = item["name"];
						item["name"] = string(stringTable->GetString("Items", 0, item["name"].substr(1, item["name"].length()).c_str(), &unk));
						if (item["name"].compare("StringTable::InvalidKey") == 0) { item["name"] = item["untranslatedname"]; }
					}

					if (item["description"].find("#") != string::npos) {
						item["untranslateddescription"] = item["description"];
						item["description"] = string(stringTable->GetString("Items", 0, item["description"].substr(1, item["description"].length()).c_str(), &unk));
						if (item["description"].compare("StringTable::InvalidKey") == 0) { item["description"] = item["untranslateddescription"]; }
					}
					if (item.find("relativeid") != item.end()) { XMLStuff.ItemData->byrelativeid[lastmodid + item["relativeid"]] = id; }
					XMLStuff.ItemData->ProcessChilds(auxnode, id);
					XMLStuff.ItemData->bynamemod[item["name"] + lastmodid] = id;
					XMLStuff.ItemData->bymod[lastmodid].push_back(id);
					XMLStuff.ItemData->byfilepathmulti.tab[currpath].push_back(id);
					XMLStuff.ItemData->byname[item["name"]] = id;
					XMLStuff.ItemData->nodes[id] = item;
					XMLStuff.ModData->items[lastmodid] += 1;
				}
			}
			else if ((strcmp(auxnodename, "trinket") == 0)) {
				XMLAttributes trinket;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					trinket[stringlower(attr->name())] = string(attr->value());
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					trinket[stringlower(attr->name())] = string(attr->value());
				}
				string oldid = trinket["id"];
				if (isitemmetadata) { //items_metadata lazy bypass
					if (trinket.find("id") != trinket.end()) {
						if (strcmp(lastmodid, "BaseGame") != 0) {
							trinket["id"] = to_string(XMLStuff.TrinketData->byrelativeid[lastmodid + trinket["id"]]);
						}
						id = toint(trinket["id"]);
						XMLAttributes s = XMLStuff.TrinketData->nodes[id];
						s["tags"] = trinket["tags"];
						s["quality"] = trinket["quality"];
						XMLStuff.TrinketData->nodes[id] = s;
					}
				}
				else {
					if ((trinket.find("id") != trinket.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
						id = toint(trinket["id"]);
					}
					else {
						if (trinket.find("id") != trinket.end()) { trinket["relativeid"] = trinket["id"]; }
						XMLStuff.TrinketData->maxid += 1;
						trinket["id"] = to_string(XMLStuff.TrinketData->maxid);
						id = XMLStuff.TrinketData->maxid;
					}
					if (id > XMLStuff.TrinketData->maxid) {
						XMLStuff.TrinketData->maxid = id;
					}
					if (oldid.length() > 0) { trinket["id"] = oldid; }

					trinket["sourceid"] = lastmodid;
					trinket["type"] = auxnode->name();
					//printf("tname: %s // %s (%s) \n", trinket["name"].c_str(), trinket["description"].c_str(), trinket["id"].c_str());
					if (trinket["name"].find("#") != string::npos) {
						trinket["untranslatedname"] = trinket["name"];
						trinket["name"] = string(stringTable->GetString("Items", 0, trinket["name"].substr(1, trinket["name"].length()).c_str(), &unk));
						if (trinket["name"].compare("StringTable::InvalidKey") == 0) { trinket["name"] = trinket["untranslatedname"]; }
					}

					if (trinket["description"].find("#") != string::npos) {
						trinket["untranslateddescription"] = trinket["description"];
						trinket["description"] = string(stringTable->GetString("Items", 0, trinket["description"].substr(1, trinket["description"].length()).c_str(), &unk));
						if (trinket["description"].compare("StringTable::InvalidKey") == 0) { trinket["description"] = trinket["untranslateddescription"]; }
					}
					if (trinket.find("relativeid") != trinket.end()) { XMLStuff.TrinketData->byrelativeid[lastmodid + trinket["relativeid"]] = id; }
					XMLStuff.TrinketData->ProcessChilds(auxnode, id);
					XMLStuff.TrinketData->bynamemod[trinket["name"] + lastmodid] = id;
					XMLStuff.TrinketData->bymod[lastmodid].push_back(id);
					XMLStuff.TrinketData->byfilepathmulti.tab[currpath].push_back(id);
					XMLStuff.TrinketData->byname[trinket["name"]] = id;
					XMLStuff.TrinketData->nodes[id] = trinket;
					XMLStuff.ModData->trinkets[lastmodid] += 1;
				}
			} else if (!isitemmetadata && (strcmp(auxnodename, "null") == 0)) {
				XMLAttributes item;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					item[stringlower(attr->name())] = string(attr->value());
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					item[stringlower(attr->name())] = string(attr->value());
				}
				item["sourceid"] = lastmodid;
				item["type"] = auxnodename;

				if ((item.find("id") != item.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					id = toint(item["id"]);
				} else {
					if (item.find("id") != item.end()) {
						item["relativeid"] = item["id"];
					}
					XMLStuff.NullItemData->maxid += 1;
					item["id"] = to_string(XMLStuff.NullItemData->maxid);
					id = XMLStuff.NullItemData->maxid;
				}

				if (id > XMLStuff.NullItemData->maxid) {
					XMLStuff.NullItemData->maxid = id;
				}

				if (item.find("relativeid") != item.end()) { XMLStuff.NullItemData->byrelativeid[lastmodid + item["relativeid"]] = id; }

				XMLStuff.NullItemData->ProcessChilds(auxnode, id);
				XMLStuff.NullItemData->bynamemod[item["name"] + lastmodid] = id;
				XMLStuff.NullItemData->bymod[lastmodid].push_back(id);
				XMLStuff.NullItemData->byfilepathmulti.tab[currpath].push_back(id);
				XMLStuff.NullItemData->byname[item["name"]] = id;
				XMLStuff.NullItemData->nodes[id] = item;
				XMLStuff.ModData->nullitems[lastmodid] += 1;
			}
		}
		break;
	case 5: //bombcostumes
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes bombcostume;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				bombcostume[stringlower(attr->name())] = string(attr->value());
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				bombcostume[stringlower(attr->name())] = string(attr->value());
			}
			string oldid = bombcostume["variant"];
			if ((bombcostume.find("variant") != bombcostume.end())){ // && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(bombcostume["variant"]);
			}
			else {
				if (bombcostume.find("variant") != bombcostume.end()) { bombcostume["relativeid"] = bombcostume["variant"]; }
				XMLStuff.BombCostumeData->maxid = XMLStuff.BombCostumeData->maxid + 1;
				bombcostume["id"] = to_string(XMLStuff.BombCostumeData->maxid);
				id = XMLStuff.BombCostumeData->maxid;
			}
			if (id > XMLStuff.BombCostumeData->maxid) {
				XMLStuff.BombCostumeData->maxid = id;
			}
			if (oldid.length() > 0) { bombcostume["variant"] = oldid; }
			if (bombcostume.find("name") == bombcostume.end()) { bombcostume["name"] = bombcostume["variant"]; }
			bombcostume["sourceid"] = lastmodid;
			XMLStuff.BombCostumeData->ProcessChilds(auxnode, id);
			if (bombcostume.count("relativeid") > 0) { XMLStuff.BombCostumeData->byrelativeid[lastmodid + bombcostume["relativeid"]] = id; }
			XMLStuff.BombCostumeData->bynamemod[bombcostume["name"] + lastmodid] = id;
			XMLStuff.BombCostumeData->bymod[lastmodid].push_back(id);
			XMLStuff.BombCostumeData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.BombCostumeData->byname[bombcostume["name"]] = id;
			XMLStuff.BombCostumeData->nodes[id] = bombcostume;
			XMLStuff.ModData->bombcostumes[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
	break;
	case 6: //music
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes music;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				music[stringlower(attr->name())] = string(attr->value());
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				music[stringlower(attr->name())] = string(attr->value());
			}
			string oldid =music["id"];
			if ((music.find("id") != music.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(music["id"]);
			}
			else {
				if (music.find("id") != music.end()) { music["relativeid"] = music["id"]; }
				XMLStuff.MusicData->maxid = XMLStuff.MusicData->maxid + 1;
				music["id"] = to_string(XMLStuff.MusicData->maxid);
				id = XMLStuff.MusicData->maxid;
			}
			if (id > XMLStuff.MusicData->maxid) {
				XMLStuff.MusicData->maxid = id;
			}
			if (oldid.length() > 0) { music["id"] = oldid; }
			music["sourceid"] = lastmodid;
			XMLStuff.MusicData->ProcessChilds(auxnode, id);
			if (music.find("relativeid") != music.end()) { XMLStuff.MusicData->byrelativeid[lastmodid + music["relativeid"]] = id; }
			XMLStuff.MusicData->bynamemod[music["name"] + lastmodid] = id;
			XMLStuff.MusicData->bymod[lastmodid].push_back(id);
			XMLStuff.MusicData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.MusicData->byname[music["name"]] = id;
			XMLStuff.MusicData->nodes[id] = music;
			XMLStuff.ModData->musictracks[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
	break;
	case 7: //sounds
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes sound;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				sound[stringlower(attr->name())] = string(attr->value());
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				sound[stringlower(attr->name())] = string(attr->value());
			}
			string oldid = sound["id"];
			if ((sound.find("id") != sound.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(sound["id"]);
			}
			else {
				if (sound.find("id") != sound.end()) { sound["relativeid"] = sound["id"]; }
				XMLStuff.SoundData->maxid = XMLStuff.SoundData->maxid + 1;
				sound["id"] = to_string(XMLStuff.SoundData->maxid);
				id = XMLStuff.SoundData->maxid;
			}
			if (id > XMLStuff.SoundData->maxid) {
				XMLStuff.SoundData->maxid = id;
			}
			if (oldid.length() > 0) { sound["id"] = oldid; }
			sound["sourceid"] = lastmodid;
			XMLStuff.SoundData->ProcessChilds(auxnode, id);
			if ((sound.find("name") == sound.end()) && (XMLStuff.SoundData->childs[id]["sample"].size() > 0) && (XMLStuff.SoundData->childs[id]["sample"][0].count("path") != 0)) {
				sound["name"] = getFileName(XMLStuff.SoundData->childs[id]["sample"][0]["path"]);
			}
			//printf("sound: %s (%d) \n",sound["name"].c_str(),id);
			if (sound.find("relativeid") != sound.end()) { XMLStuff.SoundData->byrelativeid[lastmodid + sound["relativeid"]] = id; }
			XMLStuff.SoundData->bynamemod[sound["name"] + lastmodid] = id;
			XMLStuff.SoundData->bymod[lastmodid].push_back(id);
			XMLStuff.SoundData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.SoundData->byname[sound["name"]] = id;
			XMLStuff.SoundData->nodes[id] = sound;
			XMLStuff.ModData->sounds[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
		break;
	case 8: //achievements
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes achievement;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				achievement[stringlower(attr->name())] = string(attr->value());
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				achievement[stringlower(attr->name())] = string(attr->value());
			}
			string oldid = achievement["id"];
			if ((achievement.find("id") != achievement.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(achievement["id"]);
			}
			else {
				if (achievement.find("id") != achievement.end()) { achievement["relativeid"] = achievement["id"]; }
				XMLStuff.AchievementData->maxid = XMLStuff.AchievementData->maxid + 1;
				achievement["id"] = to_string(XMLStuff.AchievementData->maxid);
				id = XMLStuff.AchievementData->maxid;
			}
			if (id > XMLStuff.AchievementData->maxid) {
				XMLStuff.AchievementData->maxid = id;
			}
			if (oldid.length() > 0) { achievement["id"] = oldid; }
			if (achievement.find("sourceid") == achievement.end()) {
				achievement["sourceid"] = lastmodid;
			}
			XMLStuff.AchievementData->ProcessChilds(auxnode, id);
			if (achievement.count("name") == 0){
				achievement["name"] = achievement["text"];
			}
			if (achievement["name"].length() == 0) {
				achievement["name"] = achievement["steam_name"];
			}
			//printf("achievement: %s (%d) \n", achievement["name"].c_str(),id);
			if (achievement.count("relativeid") > 0) { XMLStuff.AchievementData->byrelativeid[lastmodid + achievement["relativeid"]] = id; }
			XMLStuff.AchievementData->bynamemod[achievement["name"] + lastmodid] = id;
			XMLStuff.AchievementData->bymod[lastmodid].push_back(id);
			XMLStuff.AchievementData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.AchievementData->byname[achievement["name"]] = id;
			XMLStuff.AchievementData->nodes[id] = achievement;
			XMLStuff.ModData->sounds[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
	break;
	case 9: //challenges
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes challenge;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				challenge[stringlower(attr->name())] = string(attr->value());
			}
			string oldid = challenge["id"];
			if ((challenge.find("id") != challenge.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(challenge["id"]);
			}
			else {
				if (challenge.find("id") != challenge.end()) { challenge["relativeid"] = challenge["id"]; }
				XMLStuff.ChallengeData->maxid = XMLStuff.ChallengeData->maxid + 1;
				challenge["id"] = to_string(XMLStuff.ChallengeData->maxid);
				id = XMLStuff.ChallengeData->maxid;
			}
			if (id > XMLStuff.ChallengeData->maxid) {
				XMLStuff.ChallengeData->maxid = id;
			}
			if (oldid.length() > 0) { challenge["id"] = oldid; }
			challenge["sourceid"] = lastmodid;
			if (challenge.find("relativeid") != challenge.end()) { XMLStuff.ChallengeData->byrelativeid[lastmodid + challenge["relativeid"]] = id; }
			XMLStuff.ChallengeData->ProcessChilds(auxnode, id);
			XMLStuff.ChallengeData->bynamemod[challenge["name"] + lastmodid] = id;
			XMLStuff.ChallengeData->bymod[lastmodid].push_back(id);
			XMLStuff.ChallengeData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.ChallengeData->byname[challenge["name"]] = id;
			XMLStuff.ChallengeData->nodes[id] = challenge;
			XMLStuff.ModData->challenges[lastmodid] += 1;
		}
	break;
	case 10: //backdrops
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes backdrop;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				backdrop[stringlower(attr->name())] = string(attr->value());
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				backdrop[stringlower(attr->name())] = string(attr->value());
			}
			string oldid = backdrop["id"];
			if ((backdrop.count("id") > 0)) {
				id = toint(backdrop["id"]);
			}
			else {
				if (backdrop.find("id") != backdrop.end()) { backdrop["relativeid"] = backdrop["id"]; }
				XMLStuff.BackdropData->maxid = XMLStuff.BackdropData->maxid + 1;
				backdrop["id"] = to_string(XMLStuff.BackdropData->maxid);
				id = XMLStuff.BackdropData->maxid;
			}
			if (id > XMLStuff.BackdropData->maxid) {
				XMLStuff.BackdropData->maxid = id;
			}
			if (oldid.length() > 0) { backdrop["id"] = oldid; }
			if (backdrop.find("name") != backdrop.end()) {
				backdrop["name"] = backdrop["gfx"].substr(0, backdrop["gfx"].length() - 4);
				int pos = backdrop["name"].find("_");
				if ((pos < 4) && (pos > 0)) {
					backdrop["name"] = backdrop["name"].substr(pos+1, backdrop["name"].length());
				}
				//printf("%s \n", backdrop["name"].c_str());
			}

			backdrop["sourceid"] = lastmodid;
			if (backdrop.find("relativeid") != backdrop.end()) { XMLStuff.BackdropData->byrelativeid[lastmodid + backdrop["relativeid"]] = id; }
			XMLStuff.BackdropData->ProcessChilds(auxnode, id);
			XMLStuff.BackdropData->bynamemod[backdrop["name"] + lastmodid] = id;
			XMLStuff.BackdropData->bymod[lastmodid].push_back(id);
			XMLStuff.BackdropData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.BackdropData->byname[backdrop["name"]] = id;
			XMLStuff.BackdropData->nodes[id] = backdrop;
			XMLStuff.ModData->backdrops[lastmodid] += 1;
		}
	break;
	case 11://cutscenes
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes cutscene;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				cutscene[stringlower(attr->name())] = string(attr->value());
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				cutscene[stringlower(attr->name())] = string(attr->value());
			}
			//This one is prechecked before even loading so id stuff is already resolved
			//if ((cutscene.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(cutscene["id"]);
				int oldid = id;
				if (cutscene.count("realid") > 0) {
					id = toint(cutscene["realid"]);
					cutscene["id"] = cutscene["realid"];
				}
			//}
			/*
			else {
				if (cutscene.count("id") > 0) { cutscene["relativeid"] = cutscene["id"]; }
				XMLStuff.ChallengeData->maxid = XMLStuff.ChallengeData->maxid + 1;
				cutscene["id"] = to_string(XMLStuff.ChallengeData->maxid);
				id = XMLStuff.ChallengeData->maxid;
			}
			if (id > XMLStuff.ChallengeData->maxid) {
				XMLStuff.ChallengeData->maxid = id;
			}
			*/
				if (oldid > 0) { cutscene["id"] = oldid; }

			if (cutscene.find("id") == cutscene.end()) { cutscene["sourceid"] = "BaseGame"; };
			if (cutscene.find("relativeid") != cutscene.end()) { XMLStuff.CutsceneData->byrelativeid[cutscene["sourceid"] + cutscene["relativeid"]] = id; }
			XMLStuff.CutsceneData->ProcessChilds(auxnode, id);
			XMLStuff.CutsceneData->bynamemod[cutscene["name"] + cutscene["sourceid"]] = id;
			XMLStuff.CutsceneData->bymod[cutscene["sourceid"]].push_back(id);
			XMLStuff.CutsceneData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.CutsceneData->byname[cutscene["name"]] = id;
			XMLStuff.CutsceneData->nodes[id] = cutscene;
			XMLStuff.ModData->cutscenes[cutscene["sourceid"]] += 1;
		}
	break;
	case 12: //stages
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes stage;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				stage[stringlower(attr->name())] = string(attr->value());
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				stage[stringlower(attr->name())] = string(attr->value());
			}
			string oldid = stage["id"];
			id = toint(stage["id"]);
			
			if (stage.find("realid") != stage.end()) {
				id = toint(stage["realid"]);
				stage["id"] = stage["realid"];
			}
			if (oldid.length() > 0) { stage["id"] = oldid; }

			if (stage.find("sourceid") == stage.end()) { stage["sourceid"] = "BaseGame"; };
			if (stage.find("relativeid") != stage.end()) { XMLStuff.StageData->byrelativeid[stage["sourceid"] + stage["relativeid"]] = id; }

			if (stage["name"].find("#") != string::npos) {
				stage["untranslatedname"] = stage["name"];
				stage["name"] = string(stringTable->GetString("Stages", 0, stage["name"].substr(1, stage["name"].length()).c_str(), &unk));
				if (stage["name"].compare("StringTable::InvalidKey") == 0) { stage["name"] = stage["untranslatedname"]; }
			}
			//printf("stage: %s (%d)", stage["name"].c_str(), id);
			XMLStuff.StageData->ProcessChilds(auxnode, id);
			XMLStuff.StageData->bynamemod[stage["name"] + stage["sourceid"]] = id;
			XMLStuff.StageData->bymod[stage["sourceid"]].push_back(id);
			//XMLStuff.StageData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.StageData->byname[stage["name"]] = id;
			if (stage.find("consoleid") != stage.end()) {
				tuple<int, int> idx = { toint(stage["consoleid"]), toint(stage["stagealt"]) };
				XMLStuff.StageData->bystagealt[idx] = id;
			}
			//XMLStuff.StageData->bybasestage[toint(stage["basestage"])] = id;
			XMLStuff.StageData->nodes[id] = stage;
			XMLStuff.ModData->stages[stage["sourceid"]] += 1;
			
		}
	break;
	case 13://recipes
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes recipe;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				recipe[stringlower(attr->name())] = string(attr->value());
			}
			//if ((challenge.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				//id = toint(challenge["id"]);
			//}
			//else {
			int oldid = toint(recipe["id"]);
				//if (challenge.count("id") > 0) { challenge["relativeid"] = challenge["id"]; }
				XMLStuff.RecipeData->maxid = XMLStuff.RecipeData->maxid + 1;
				recipe["id"] = to_string(XMLStuff.RecipeData->maxid);
				id = XMLStuff.RecipeData->maxid;
			//}
			if (id > XMLStuff.RecipeData->maxid) {
				XMLStuff.RecipeData->maxid = id;
			}
			if (oldid > 0) {
				recipe["id"] = oldid;
			}
			if ((recipe.find("input") != recipe.end()) && (recipe.find("name") == recipe.end())) { recipe["name"] = recipe["input"]; };
			recipe["sourceid"] = lastmodid;
			if (recipe.find("relativeid") != recipe.end()) { XMLStuff.RecipeData->byrelativeid[lastmodid + recipe["relativeid"]] = id; }
			XMLStuff.RecipeData->ProcessChilds(auxnode, id);
			XMLStuff.RecipeData->bynamemod[recipe["name"] + lastmodid] = id;
			XMLStuff.RecipeData->bymod[lastmodid].push_back(id);
			XMLStuff.RecipeData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.RecipeData->byname[recipe["name"]] = id;
			XMLStuff.RecipeData->nodes[id] = recipe;
			XMLStuff.ModData->recipes[lastmodid] += 1;
		}
	break;
	case 14: //wisps
		daddy = node;
		id = 1;
		for (xml_node<char>* auxnode = node->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			string middleman = stringlower(auxnode->name());
			const char* nodename = middleman.c_str();
			if ((strcmp(nodename, "color") == 0)) {
				XMLAttributes color;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					color[stringlower(attr->name())] = string(attr->value());
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					color[stringlower(attr->name())] = string(attr->value());
				}
				int oldid = toint(color["id"]);
				if ((color.find("id") != color.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					id = toint(color["id"]);
				}
				else {
					if (color.find("id") != color.end()) { color["relativeid"] = color["id"]; }
					XMLStuff.WispColorData->maxid = XMLStuff.WispColorData->maxid + 1;
					color["id"] = to_string(XMLStuff.WispColorData->maxid);
					id = XMLStuff.WispColorData->maxid;
				}
				if (id > XMLStuff.WispColorData->maxid) {
					XMLStuff.WispColorData->maxid = id;
				}
				if (oldid > 0) {
					color["id"] = oldid;
				}
				color["sourceid"] = lastmodid;
				if (color.find("relativeid") != color.end()) { XMLStuff.WispColorData->byrelativeid[lastmodid + color["relativeid"]] = id; }
				XMLStuff.WispColorData->ProcessChilds(auxnode, id);
				XMLStuff.WispColorData->bynamemod[color["name"] + lastmodid] = id;
				XMLStuff.WispColorData->bymod[lastmodid].push_back(id);
				XMLStuff.WispColorData->byfilepathmulti.tab[currpath].push_back(id);
				XMLStuff.WispColorData->byname[color["name"]] = id;
				XMLStuff.WispColorData->nodes[id] = color;
				XMLStuff.ModData->wispcolors[lastmodid] += 1;
			}
			else if ((strcmp(nodename, "wisp") == 0)) {
					XMLAttributes wisp;
					for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
					{
						wisp[stringlower(attr->name())] = string(attr->value());
					}
					for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
					{
						wisp[stringlower(attr->name())] = string(attr->value());
					}
					int oldid = toint(wisp["id"]);
					if ((wisp.find("id") != wisp.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
						id = toint(wisp["id"]);
					}
					else {
						if (wisp.find("id") != wisp.end()) { wisp["relativeid"] = wisp["id"]; }
						XMLStuff.WispData->maxid = XMLStuff.WispData->maxid + 1;
						wisp["id"] = to_string(XMLStuff.WispData->maxid);
						id = XMLStuff.WispData->maxid;
					}
					if (id > XMLStuff.WispData->maxid) {
						XMLStuff.WispData->maxid = id;
					}
					if (oldid > 0) {
						wisp["id"] = oldid;
					}
					wisp["sourceid"] = lastmodid;
					if (wisp.find("relativeid") != wisp.end()) {
						id = toint(XMLStuff.ItemData->nodes[XMLStuff.ItemData->byrelativeid[lastmodid + wisp["relativeid"]]]["id"]);
						wisp["id"] = to_string(id);
						XMLStuff.WispData->byrelativeid[lastmodid + wisp["relativeid"]] = id;
						wisp["name"] = XMLStuff.ItemData->nodes[XMLStuff.ItemData->byrelativeid[lastmodid +  wisp["relativeid"]]]["name"];
					}
					else {
						wisp["name"] = XMLStuff.ItemData->nodes[toint(wisp["id"])]["name"];
					}
					if (id == 0) {
						id = XMLStuff.WispData->maxid + 2000;
						wisp["id"] = to_string(id);
					}
					//printf("Wisp #%d(%s) : %s \n", toint(wisp["id"]), wisp["id"].c_str(), wisp["name"].c_str());
					XMLStuff.WispData->ProcessChilds(auxnode, id);
					XMLStuff.WispData->bynamemod[wisp["name"] + lastmodid] = id;
					XMLStuff.WispData->bymod[lastmodid].push_back(id);
					XMLStuff.WispData->byfilepathmulti.tab[currpath].push_back(id);
					XMLStuff.WispData->byname[wisp["name"]] = id;
					XMLStuff.WispData->nodes[id] = wisp;
					XMLStuff.ModData->wisps[lastmodid] += 1;

				}

			}
		break;
	case 15://locusts
		daddy = node;
		id = 1;
		for (xml_node<char>* auxnode = node->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			string middleman = stringlower(auxnode->name());
			const char* nodename = middleman.c_str();
			if ((strcmp(nodename, "color") == 0)) {
				XMLAttributes color;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					color[stringlower(attr->name())] = string(attr->value());
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					color[stringlower(attr->name())] = string(attr->value());
				}
				int oldid = toint(color["id"]);
				if ((color.find("id") != color.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					id = toint(color["id"]);
				}
				else {
					if (color.find("id") != color.end()) { color["relativeid"] = color["id"]; }
					XMLStuff.LocustColorData->maxid = XMLStuff.LocustColorData->maxid + 1;
					color["id"] = to_string(XMLStuff.LocustColorData->maxid);
					id = XMLStuff.LocustColorData->maxid;
				}
				if (id > XMLStuff.LocustColorData->maxid) {
					XMLStuff.LocustColorData->maxid = id;
				}
				if (oldid > 0) {
					color["id"] = oldid;
				}
				color["sourceid"] = lastmodid;
				if (color.find("relativeid") != color.end()) { XMLStuff.LocustColorData->byrelativeid[lastmodid + color["relativeid"]] = id; }
				XMLStuff.LocustColorData->ProcessChilds(auxnode, id);
				XMLStuff.LocustColorData->bynamemod[color["name"] + lastmodid] = id;
				XMLStuff.LocustColorData->bymod[lastmodid].push_back(id);
				XMLStuff.LocustColorData->byfilepathmulti.tab[currpath].push_back(id);
				XMLStuff.LocustColorData->byname[color["name"]] = id;
				XMLStuff.LocustColorData->nodes[id] = color;
				XMLStuff.ModData->locustcolors[lastmodid] += 1;
			}
			else if ((strcmp(nodename, "locust") == 0)) {
				XMLAttributes locust;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					locust[stringlower(attr->name())] = string(attr->value());
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					locust[stringlower(attr->name())] = string(attr->value());
				}
				int oldid = toint(locust["id"]);
				if ((locust.find("id") != locust.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					id = toint(locust["id"]);
				}
				else {
					if (locust.find("id") != locust.end()) { locust["relativeid"] = locust["id"]; }
					XMLStuff.LocustData->maxid = XMLStuff.LocustData->maxid + 1;
					locust["id"] = to_string(XMLStuff.LocustData->maxid);
					id = XMLStuff.LocustData->maxid;
				}
				if (id > XMLStuff.LocustData->maxid) {
					XMLStuff.LocustData->maxid = id;
				}
				if (oldid > 0) {
					locust["id"] = oldid;
				}
				locust["sourceid"] = lastmodid;
				if (locust.find("relativeid") != locust.end()) {
					id = toint(XMLStuff.ItemData->nodes[XMLStuff.ItemData->byrelativeid[lastmodid + locust["relativeid"]]]["id"]);
					locust["id"] = to_string(id);
					XMLStuff.WispData->byrelativeid[lastmodid + locust["relativeid"]] = id;
					locust["name"] = XMLStuff.ItemData->nodes[XMLStuff.ItemData->byrelativeid[lastmodid + locust["relativeid"]]]["name"];
				}
				else {
					locust["name"] = XMLStuff.ItemData->nodes[toint(locust["id"])]["name"];
				}
				if (id == 0) {
					id = XMLStuff.LocustData->maxid + 2000;
					locust["id"] = to_string(id);
				}
				//printf("Wisp #%d(%s) : %s \n", toint(wisp["id"]), wisp["id"].c_str(), wisp["name"].c_str());
				XMLStuff.LocustData->ProcessChilds(auxnode, id);
				XMLStuff.LocustData->bynamemod[locust["name"] + lastmodid] = id;
				XMLStuff.LocustData->bymod[lastmodid].push_back(id);
				XMLStuff.LocustData->byfilepathmulti.tab[currpath].push_back(id);
				XMLStuff.LocustData->byname[locust["name"]] = id;
				XMLStuff.LocustData->nodes[id] = locust;
				XMLStuff.ModData->locusts[lastmodid] += 1;

			}

		}
	break;
	case 16: //nightmares
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes nightmare;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				nightmare[stringlower(attr->name())] = string(attr->value());
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				nightmare[stringlower(attr->name())] = string(attr->value());
			}
			string oldid= nightmare["id"];
			if ((nightmare.find("id") != nightmare.end()) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(nightmare["id"]);
			}
			else {
				if (nightmare.find("id") != nightmare.end()) { nightmare["relativeid"] = nightmare["id"]; }
				XMLStuff.NightmareData->maxid = XMLStuff.NightmareData->maxid + 1;
				nightmare["id"] = to_string(XMLStuff.NightmareData->maxid);
				id = XMLStuff.NightmareData->maxid;
			}
			if (id > XMLStuff.NightmareData->maxid) {
				XMLStuff.NightmareData->maxid = id;
			}
			if (oldid.length() > 0) {
				nightmare["id"] = oldid;
			}
			if ((nightmare.find("name") == nightmare.end()) && (nightmare.count("anm2") != 0)) {
				nightmare["name"] = getFileName(nightmare["anm2"]);
			}

			nightmare["sourceid"] = lastmodid;
			if (nightmare.find("relativeid") != nightmare.end()) { XMLStuff.NightmareData->byrelativeid[lastmodid + nightmare["relativeid"]] = id; }
			XMLStuff.NightmareData->ProcessChilds(auxnode, id);
			XMLStuff.NightmareData->bynamemod[nightmare["name"] + lastmodid] = id;
			XMLStuff.NightmareData->bymod[lastmodid].push_back(id);
			XMLStuff.NightmareData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.NightmareData->byname[nightmare["name"]] = id;
			XMLStuff.NightmareData->nodes[id] = nightmare;
			XMLStuff.ModData->nightmares[lastmodid] += 1;
		}
		break;
	case 17://curses 
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes curse;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				curse[stringlower(attr->name())] = string(attr->value());
			}
			//if ((curse.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				//id = toint(curse["id"]);
			//}
			//else {
				if (curse.count("id") > 0) { curse["relativeid"] = curse["id"]; }
				curse["id"] = to_string(XMLStuff.CurseData->maxid);
				id = XMLStuff.CurseData->maxid;
				XMLStuff.CurseData->maxid = XMLStuff.CurseData->maxid * 2; //I still cant believe this
			//}
			if (id > XMLStuff.CurseData->maxid) {
				XMLStuff.CurseData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				curse[stringlower(attr->name())] = attr->value();
			}
			if (curse["name"].find("#") != string::npos) {
				curse["untranslatedname"] = curse["name"];
				curse["name"] = string(stringTable->GetString("Curses", 0, curse["name"].substr(1, curse["name"].length()).c_str(), &unk));
				if (curse["name"].compare("StringTable::InvalidKey") == 0) { curse["name"] = curse["untranslatedname"]; }
			}

			curse["sourcepath"] = currpath;
			curse["sourceid"] = lastmodid;
			if (curse.count("relativeid") > 0) { XMLStuff.CurseData->byrelativeid[lastmodid + curse["relativeid"]] = id; }
			//printf("curse: %d - %s \n", id, curse["name"].c_str());
			XMLStuff.CurseData->ProcessChilds(auxnode, id);
			XMLStuff.CurseData->bynamemod[curse["name"] + lastmodid] = id;
			XMLStuff.CurseData->bymod[lastmodid].push_back(id);
			XMLStuff.CurseData->byfilepathmulti.tab[currpath].push_back(id);
			if (XMLStuff.CurseData->byname.count(curse["name"]) == 0) { //to prioritize vanilla in case of Curse of the Giant hacky
				XMLStuff.CurseData->byname[curse["name"]] = id;
			}
			XMLStuff.CurseData->nodes[id] = curse;
			XMLStuff.ModData->curses[lastmodid] += 1;
		}
		break;
	case 18://bosses
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes boss;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				boss[stringlower(attr->name())] = string(attr->value());
			}
			if (boss.count("id") > 0) { id = stoi(boss["id"]); }else {
				boss["id"] = to_string(XMLStuff.BossPortraitData->maxid);
				id = XMLStuff.BossPortraitData->maxid;
				XMLStuff.BossPortraitData->maxid = XMLStuff.BossPortraitData->maxid +1;
			}
			if (id > XMLStuff.BossPortraitData->maxid) {
				XMLStuff.BossPortraitData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				boss[stringlower(attr->name())] = attr->value();
			}

			boss["sourcepath"] = currpath;
			if (boss.count("sourceid") <= 0) { boss["sourceid"] = "BaseGame"; }
			//printf("curse: %d - %s \n", id, curse["name"].c_str());
			XMLStuff.BossPortraitData->ProcessChilds(auxnode, id);
			XMLStuff.BossPortraitData->bynamemod[boss["name"] + lastmodid] = id;
			XMLStuff.BossPortraitData->bymod[lastmodid].push_back(id);
			XMLStuff.BossPortraitData->byfilepathmulti.tab[currpath].push_back(id);
			if (XMLStuff.BossPortraitData->byname.count(boss["name"]) == 0) { //to prioritize vanilla in case of Curse of the Giant hacky
				XMLStuff.BossPortraitData->byname[boss["name"]] = id;
			}
			XMLStuff.BossPortraitData->nodes[id] = boss;
			XMLStuff.ModData->bossportraits[lastmodid] += 1;
		}
	break;
	case 19: //costumes
		id = 1;
		idnull = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			if (string(auxnode->name()) != "costume") continue;
			XMLAttributes costume;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				costume[stringlower(attr->name())] = stringlower(attr->value());
			}
			if ((strcmp(costume["type"].c_str(), "passive") == 0) || (strcmp(costume["type"].c_str(), "active") == 0) || (strcmp(costume["type"].c_str(), "trinket") == 0) || (strcmp(costume["type"].c_str(), "familiar") == 0)){
				if ((costume.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					id = toint(costume["id"]);
				}
				else {
					XMLStuff.CostumeData->maxid = XMLStuff.CostumeData->maxid + 1;
					if (costume.count("id") > 0) {
						costume["relativeid"] = costume["id"];
						if ((costume.count("type") > 0)) {
							if ((strcmp(costume["type"].c_str(), "trinket") == 0)) {
								costume["id"] = to_string(XMLStuff.TrinketData->byrelativeid[lastmodid + costume["relativeid"]]);
							}
							else {
								costume["id"] = to_string(XMLStuff.ItemData->byrelativeid[lastmodid + costume["relativeid"]]);
							}
						}
					}
					else {
						costume["id"] = to_string(XMLStuff.NullCostumeData->maxid + 5000); //lol just to be safe its unordered, it fiiiine (this should only happen if modders fuck up too, it should always have an id)
					}					
					id = toint(costume["id"]);
				}
				if (id > XMLStuff.CostumeData->maxid) {
					XMLStuff.CostumeData->maxid = id;
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					costume[stringlower(attr->name())] = attr->value();
				}
				if ((costume.count("name") == 0) && (costume.count("anm2path") != 0)) {
					costume["name"] = getFileName(costume["anm2path"]);
				}
				costume["sourceid"] = lastmodid;
				//printf("costume: %s %s (%s) \n", costume["name"].c_str(), costume["id"].c_str(), costume["type"].c_str());
				if (costume.count("relativeid") > 0) { XMLStuff.CostumeData->byrelativeid[lastmodid + costume["relativeid"]] = id; }
				XMLStuff.CostumeData->ProcessChilds(auxnode, id);
				XMLStuff.CostumeData->bynamemod[costume["name"] + lastmodid] = id;
				XMLStuff.CostumeData->bymod[lastmodid].push_back(id);
				XMLStuff.CostumeData->byfilepathmulti.tab[currpath].push_back(id);
				XMLStuff.CostumeData->byname[costume["name"]] = id;
				XMLStuff.CostumeData->nodes[id] = costume;
				XMLStuff.ModData->costumes[lastmodid] += 1;
			}
			else {
				if ((costume.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					idnull = toint(costume["id"]);
				}
				else {
					if (costume.count("id") > 0) {costume["relativeid"] = costume["id"];}
					if (costume.count("relativeid") > 0 && XMLStuff.NullItemData->byrelativeid.count(lastmodid + costume["relativeid"]) > 0) {
						// If the costume shares a relative ID with a NullItem from the same mod, use the ID of that NullItem.
						// This aligns with Repentogon's support for null items/costumes (NullItemsAndCostumes.cpp).
						idnull = XMLStuff.NullItemData->byrelativeid[lastmodid + costume["relativeid"]];
					}
					else {
						// The game will auto-generate a new NullItem for this costume and assign this costume to that ID. Account for that.
						// Just to be clear, yes, incrementing and using the maxid from NullItemData and not NullCostumeData is intentional here.
						XMLStuff.NullItemData->maxid = XMLStuff.NullItemData->maxid + 1;
						idnull = XMLStuff.NullItemData->maxid;
					}
				}
				if (idnull > XMLStuff.NullCostumeData->maxid) {
					XMLStuff.NullCostumeData->maxid = idnull;
				}
				costume["id"] = to_string(idnull);
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					costume[stringlower(attr->name())] = attr->value();
				}
				if ((costume.count("name") == 0) && (costume.count("anm2path") != 0)) {
					costume["name"] = getFileName(costume["anm2path"]);
				}
				costume["sourceid"] = lastmodid;
				if (costume.count("relativeid") > 0) { XMLStuff.NullCostumeData->byrelativeid[lastmodid + costume["relativeid"]] = idnull; }
				//printf("nullcostume: %s %s (%s) \n", costume["name"].c_str(), costume["id"].c_str(), costume["type"].c_str());
				XMLStuff.NullCostumeData->ProcessChilds(auxnode, idnull);
				XMLStuff.NullCostumeData->bynamemod[costume["name"] + lastmodid] = idnull;
				XMLStuff.NullCostumeData->bymod[lastmodid].push_back(idnull);
				XMLStuff.NullCostumeData->byfilepathmulti.tab[currpath].push_back(idnull);
				XMLStuff.NullCostumeData->byname[costume["name"]] = idnull;
				XMLStuff.NullCostumeData->nodes[idnull] = costume;
				XMLStuff.ModData->nullcostumes[lastmodid] += 1;
			
			}
		}
	break;
	case 20: //itempools
		id = -1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes itempool;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				itempool[stringlower(attr->name())] = string(attr->value());
			}
			if ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent){
				XMLStuff.PoolData->maxid = XMLStuff.PoolData->maxid + 1;
				id = XMLStuff.PoolData->maxid;
			}
			else {
				id = XMLStuff.PoolData->byname[itempool["name"]];
			}

			itempool["sourceid"] = lastmodid;
			XMLStuff.PoolData->ProcessChilds(auxnode, id);
			
			if ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent) {
				XMLStuff.PoolData->bynamemod[itempool["name"] + lastmodid] = id;
				XMLStuff.PoolData->bymod[lastmodid].push_back(id);
				XMLStuff.PoolData->byfilepathmulti.tab[currpath].push_back(id);
				XMLStuff.PoolData->byname[itempool["name"]] = id;
				XMLStuff.PoolData->nodes[id] = itempool;
			}
			XMLStuff.ModData->itempools[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
		break;
	case 21://bosspools
		lastmodid = "BaseGame"; //no mods supported yet
		id = -1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes bosspool;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				bosspool[stringlower(attr->name())] = string(attr->value());
			}
			if ((XMLStuff.BossPoolData->byname[bosspool["name"]] != NULL) && (lastmodid == "BaseGame")) { //its loading both the rep and AB+ files, kill me!
				XMLStuff.BossPoolData->Clear();
			}
			if (XMLStuff.BossPoolData->byname[bosspool["name"]] == NULL) {
				XMLStuff.BossPoolData->maxid = XMLStuff.BossPoolData->maxid + 1;
				id = XMLStuff.BossPoolData->maxid;
			}
			else {
				id = XMLStuff.BossPoolData->byname[bosspool["name"]];
			}

			if (bosspool.count("sourceid") <= 0) { bosspool["sourceid"] = "BaseGame"; }
			XMLStuff.BossPoolData->ProcessChilds(auxnode, id);

			if ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent) {
				XMLStuff.BossPoolData->bynamemod[bosspool["name"] + lastmodid] = id;
				XMLStuff.BossPoolData->bymod[lastmodid].push_back(id);
				XMLStuff.BossPoolData->byfilepathmulti.tab[currpath].push_back(id);
				XMLStuff.BossPoolData->byname[bosspool["name"]] = id;
				XMLStuff.BossPoolData->nodes[id] = bosspool;
			}
			//XMLStuff.ModData->bosspools[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
		xmlsloaded = true; //this is the last xml to load after the game fucking started! (on game::start)
	break;
	case 22: //giantbook
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes attributes;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				attributes[stringlower(attr->name())] = string(attr->value());
			}		
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				attributes[stringlower(attr->name())] = string(attr->value());
			}
			string oldid = attributes["id"];
			if ((attributes.find("id") != attributes.end()) && ((attributes.find("sourceid") == attributes.end()) || !iscontent)) {
				id = toint(attributes["id"]);
			}

			else {
				if (attributes.find("id") != attributes.end()) { attributes["relativeid"] = attributes["id"]; }
				XMLStuff.GiantBookData->maxid = XMLStuff.GiantBookData->maxid + 1;
				attributes["id"] = to_string(XMLStuff.GiantBookData->maxid);
				id = XMLStuff.GiantBookData->maxid;
			}
			if (id > XMLStuff.GiantBookData->maxid) {
				XMLStuff.GiantBookData->maxid = id;
			}

			if (attributes.find("sourceid") == attributes.end()) {
				attributes["sourceid"] = lastmodid;
			}
			XMLStuff.GiantBookData->ProcessChilds(auxnode, id);
			if ((attributes.find("name") == attributes.end()) && (attributes.find("gfx") != attributes.end())) {
				attributes["name"] = getFileName(attributes["gfx"]);
			}
			//printf("giantbook: %s (%d) \n", attributes["name"].c_str(),id);
			if (attributes.find("relativeid") != attributes.end()) { XMLStuff.GiantBookData->byrelativeid[attributes["sourceid"] + attributes["relativeid"]] = id; }
			XMLStuff.GiantBookData->bynamemod[attributes["name"] + attributes["sourceid"]] = id;
			XMLStuff.GiantBookData->bymod[attributes["sourceid"]].push_back(id);
			XMLStuff.GiantBookData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.GiantBookData->byname[attributes["name"]] = id;
			XMLStuff.GiantBookData->nodes[id] = attributes;
			//XMLStuff.ModData->sounds[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
		break;
	case 23: //bossrush
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes attributes;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				attributes[stringlower(attr->name())] = string(attr->value());
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				attributes[stringlower(attr->name())] = string(attr->value());
			}
			attributes["id"] = to_string(id);
			if (id > XMLStuff.BossRushData->maxid) {
				XMLStuff.BossRushData->maxid = id;
			}
			if (attributes.find("sourceid") == attributes.end()) {
				lastmodid = "BaseGame";
				attributes["sourceid"] = lastmodid;
			}
			XMLStuff.BossRushData->ProcessChilds(auxnode, id);
			//printf("giantbook: %s (%d) \n", attributes["name"].c_str(),id);
			if (attributes.find("relativeid") != attributes.end()) { XMLStuff.BossRushData->byrelativeid[attributes["sourceid"] + attributes["relativeid"]] = id; }
			XMLStuff.BossRushData->bynamemod[attributes["name"] + attributes["sourceid"]] = id;
			XMLStuff.BossRushData->bymod[attributes["sourceid"]].push_back(id);
			XMLStuff.BossRushData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.BossRushData->byname[attributes["name"]] = id;
			XMLStuff.BossRushData->nodes[id] = attributes;
			id++;
			//XMLStuff.ModData->sounds[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
		break;
	case 24: //playerforms
		id = 1;
		daddy = node;
		babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes attributes;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				attributes[stringlower(attr->name())] = string(attr->value());
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				attributes[stringlower(attr->name())] = string(attr->value());
			}
			string oldid = attributes["id"];
			if ((attributes.find("id") != attributes.end()) && ((attributes.find("sourceid") == attributes.end()) || !iscontent)) {
				id = toint(attributes["id"]);
			}

			else {
				if (attributes.find("id") != attributes.end()) { attributes["relativeid"] = attributes["id"]; }
				XMLStuff.PlayerFormData->maxid = XMLStuff.PlayerFormData->maxid + 1;
				attributes["id"] = to_string(XMLStuff.PlayerFormData->maxid);
				id = XMLStuff.PlayerFormData->maxid;
			}
			if (id > XMLStuff.PlayerFormData->maxid) {
				XMLStuff.PlayerFormData->maxid = id;
			}

			if (attributes.find("sourceid") == attributes.end()) {
				attributes["sourceid"] = lastmodid;
			}
			XMLStuff.PlayerFormData->ProcessChilds(auxnode, id);
			if ((attributes.find("name") == attributes.end()) && (attributes.find("gfx") != attributes.end())) {
				attributes["name"] = getFileName(attributes["gfx"]);
			}

			if (attributes["name"].find("#") != string::npos) {
				attributes["untranslatedname"] = attributes["name"];
				attributes["name"] = string(stringTable->GetString("Default", 0, attributes["name"].substr(1, attributes["name"].length()).c_str(), &unk));
				if (attributes["name"].compare("StringTable::InvalidKey") == 0) { attributes["name"] = attributes["untranslatedname"]; }
			}
			
			//printf("giantbook: %s (%d) \n", attributes["name"].c_str(),id);
			if (attributes.find("relativeid") != attributes.end()) { XMLStuff.PlayerFormData->byrelativeid[attributes["sourceid"] + attributes["relativeid"]] = id; }
			XMLStuff.PlayerFormData->bynamemod[attributes["name"] + attributes["sourceid"]] = id;
			XMLStuff.PlayerFormData->bymod[attributes["sourceid"]].push_back(id);
			XMLStuff.PlayerFormData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.PlayerFormData->byname[attributes["name"]] = id;
			XMLStuff.PlayerFormData->nodes[id] = attributes;
			//XMLStuff.ModData->sounds[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
		break;
	case 99: //name for mod metadata
	if (node->parent() && (strcmp(stringlower(node->parent()->name()).c_str(), "metadata") == 0)) {
		daddy = node->parent();
		XMLAttributes mod;
		string tags = "";
		for (xml_node<char>* auxnode = daddy->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			{
				string meh = stringlower(auxnode->name());
				const char* auxnodename = meh.c_str();
				if (auxnode->value() && (strcmp(auxnodename, "tag") != 0)){
					mod[auxnodename] = auxnode->value();
				}
				else if ((strcmp(auxnodename, "tag") == 0) && auxnode->first_attribute("id")) {
					tags += string(auxnode->first_attribute("id")->value()) +", ";
				}
			}
		}
		int idx;
		if (mod.count("id") <= 0) { mod["id"] = mod["name"];  }

		if (XMLStuff.ModData->byid.find(mod["id"]) != XMLStuff.ModData->byid.end()) {
			idx = XMLStuff.ModData->byid[mod["id"]];
		}
		else {
			XMLStuff.ModData->maxid += 1;
			idx = XMLStuff.ModData->maxid;
		}
			
		mod["tags"] = tags;
		XMLStuff.ModData->nodes[idx] = mod;
		XMLStuff.ModData->byid[mod["id"]] = idx;
		XMLStuff.ModData->bydirectory[mod["directory"]] = idx;
		XMLStuff.ModData->byname[mod["name"]] = idx;
	}
	break;
	}
	//printf("Time taken: %.20fs in %s\n", (double)(clock() - tStart) / CLOCKS_PER_SEC, nodename);
}

HOOK_METHOD(xmlnode_rep, first_node, (char* name, int size, bool casesensitive)->xml_node<char>*) {
	if (xmlsloaded) { return  super(name, size, casesensitive); }
	xml_node<char>* node = super(name, size, casesensitive);
	if ((node != nullptr)) {
		xml_attribute<char>* err =  node->first_attribute("xmlerror");
		if ((currpath.length() > 0) && (err != nullptr)){
			string error = "[XMLError] " + xmlerrors[toint(err->value())] + " in " + currpath;
			g_Game->GetConsole()->PrintError(error);
			KAGE::LogMessage(3,(error + "\n").c_str());
			//printf("%s \n", error.c_str());
		}
		ProcessXmlNode(node);
	}
	return node;
}

char * rootnodename(char* a) {
	string input = (string)a;
	size_t start = input.find('<'); 
	if (start == string::npos) {
		return ""; 
	}
	start++;  
	size_t end = input.find('>', start); 
	if (end == string::npos) {
		return "";  
	}
	size_t length = end - start;
	string str = input.substr(start, length);
	//printf(" %d %d %d %s", end, length, start, str.c_str());
	char* mutableCharPtr = new char[str.length() + 1];
	strcpy(mutableCharPtr, str.c_str());
	return mutableCharPtr;
}

void UpdateOddXMLSourceData()
{
	for each (auto & att in XMLStuff.CurseData->nodes)
	{
		XMLAttributes node = att.second;
		ModEntry* mod = GetModEntryByContentPath(node["sourcepath"]);
		if (mod != NULL) {
			node["sourceid"] = string(mod->GetId());
			XMLStuff.ModData->curses["BaseGame"] -= 1;
			XMLStuff.ModData->curses[node["sourceid"]] += 1;
			XMLStuff.CurseData->nodes[att.first] = node;
		}
	}
}

HOOK_METHOD(Manager, LoadConfigs,()->void) {
	super();
	UpdateXMLModEntryData(); //resources are already loaded by this point only mod content remains
	UpdateOddXMLSourceData(); //these get loaded in limbo so I need to do this to get the proper mod source
}

HOOK_METHOD(ItemConfig, LoadPocketItems, (char* xmlpath, ModEntry* modentry)->void) {
	if (xmlsloaded){ return super(xmlpath, modentry); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, modentry);
	currpath = "";
}

HOOK_METHOD(ItemConfig, Load, (char* xmlpath, ModEntry* modentry)->void) {
	//if (xmlsloaded) { return super(xmlpath, modentry); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, modentry);
	currpath = "";
}

HOOK_METHOD(RoomConfig, LoadCurses, (char* xmlpath, bool ismod)->void) {
	if (xmlsloaded) { return super(xmlpath, ismod); }
	//printf("curse you %s \n", xmlpath);
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadWispConfig, (char* xmlpath, ModEntry* modentry)->void) {
	if (xmlsloaded) { return super(xmlpath, modentry); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, modentry);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadLocustConfig, (char* xmlpath, ModEntry* modentry)->void) {
	if (xmlsloaded) { return super(xmlpath, modentry); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, modentry);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadBombConfigRules, (char* xmlpath, bool ismod)->void) {
	if (xmlsloaded) { return super(xmlpath, ismod); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadCraftingRecipes, (char* xmlpath, ModEntry* modentry)->void) {
	if (xmlsloaded) { return super(xmlpath, modentry); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, modentry);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadMetadata, (char* xmlpath, ModEntry* modentry)->void) {
	if (xmlsloaded) { return super(xmlpath, modentry); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	isitemmetadata = true;
	super(xmlpath, modentry);
	isitemmetadata = false;
	currpath = "";
}

HOOK_METHOD(Music, LoadConfig, (char* xmlpath, bool ismod)->void) {
	if (xmlsloaded) { return super(xmlpath, ismod); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(SFXManager, LoadConfig, (char* xmlpath, bool ismod)->void) {
	if (xmlsloaded) { return super(xmlpath, ismod); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(Manager, LoadChallenges, (char* xmlpath, bool ismod)->void) {
	if (xmlsloaded) { return super(xmlpath, ismod); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(NightmareScene, LoadConfig, (char* xmlpath)->void) {
	if (xmlsloaded) { return super(xmlpath); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadCostumes, (char* xmlpath, ModEntry* modentry)->void) {
	if (xmlsloaded) { return super(xmlpath, modentry); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, modentry);
	currpath = "";
}

HOOK_METHOD(EntityConfig, Load, (char* xmlpath, ModEntry* mod)->void) {
	if (xmlsloaded) { return super(xmlpath, mod); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, mod);
	super(xmlpath, mod);
	currpath = "";
}

HOOK_METHOD(ItemPool, load_pools, (char* xmlpath, bool ismod)->void) {
	if (xmlsloaded) { return super(xmlpath, ismod); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(EntityConfig, LoadPlayers, (char* xmlpath, ModEntry* modentry)->void) {
	if (xmlsloaded) { return super(xmlpath, modentry); }
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, modentry);
	currpath = "";
}


bool Lua_PushXMLSubNodes(lua_State* L, vector<XMLAttributes> node)
{
	lua_newtable(L);
	int count = 1;
	for each (const XMLAttributes & elem in node)
	{
		lua_newtable(L);
		for each (const auto & att in elem)
		{
			printf("attr: %s / %s \n", att.first.c_str(), att.second.c_str());
			lua_pushstring(L, att.first.c_str());
			lua_pushstring(L, att.second.c_str());
			lua_settable(L, -3);
		}
		lua_rawseti(L, -2, count++);
	}
	return true;
}

bool Lua_PushXMLNode(lua_State* L, XMLAttributes node, unordered_map<string, vector <XMLAttributes>> childs)
{
	if (node.end() == node.begin()) { return false; }
	lua_newtable(L);
	for each (const auto & att in node)
	{
		printf("attr: %s / %s \n", att.first.c_str(), att.second.c_str());
		lua_pushstring(L, att.first.c_str());
		lua_pushstring(L, att.second.c_str());
		lua_settable(L, -3);
	}
	for each (const auto & att in childs)
	{
		//printf("---childattr: %s / %s \n", att.first.c_str(), att.first.c_str());
		lua_pushstring(L, att.first.c_str());
		Lua_PushXMLSubNodes(L, childs[att.first]);
		lua_settable(L, -3);
	}

	return true;
}

LUA_FUNCTION(Lua_FromTypeVarSub)
{
	if (!lua_isnumber(L, 1)) { return luaL_error(L, "Expected EntityType as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	int etype = (int)luaL_checknumber(L, 1);
	int evar = (int)luaL_optnumber(L ,2 ,0);
	int esub = (int)luaL_optnumber(L, 3, 0);
	bool strict = lua::luaL_optboolean(L,4, false);
	XMLAttributes Node = XMLStuff.EntityData->GetNodesByTypeVarSub(etype, evar, esub, strict);
	tuple idx = { toint(Node["type"]), toint(Node["variant"]), toint(Node["subtype"]) };
	Lua_PushXMLNode(L, Node, XMLStuff.EntityData->childs[idx]);
	return 1;
}

LUA_FUNCTION(Lua_GetFromEntity)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	if (entity == NULL) { return luaL_error(L, "Expected entity as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	int etype = *entity->GetType();
	int evar = *entity->GetVariant();
	int esub = *entity->GetSubType();
	if (evar == NULL) { evar = 0; }
	if (esub == NULL) { esub = 0; }

	bool automatic = lua::luaL_optboolean(L, 2, true);
	bool strict = lua::luaL_optboolean(L, 3, false);
	XMLAttributes Node;
	XMLChilds Childs;
	if (automatic){
	switch (etype) {
		case 1:
			Node = XMLStuff.PlayerData->nodes[esub];
			Childs = XMLStuff.PlayerData->childs[esub];
			break;
		case 5:
			if ((evar == 100) && (esub > 0)) {
				Node = XMLStuff.ItemData->nodes[esub]; 
				Childs = XMLStuff.ItemData->childs[esub];
			}
			else if ((evar == 300) && (esub > 0)) {
				Node = XMLStuff.CardData->nodes[esub]; 
				Childs = XMLStuff.CardData->childs[esub];
			}
			else if ((evar == 350) && (esub > 0)) { 
				Node = XMLStuff.TrinketData->nodes[esub]; 
				Childs = XMLStuff.TrinketData->childs[esub];
			}
			else{ 
				Node = XMLStuff.EntityData->GetNodesByTypeVarSub(etype, evar, esub, strict); 
				Childs = XMLStuff.EntityData->childs[{ toint(Node["type"]), toint(Node["variant"]), toint(Node["subtype"]) }];
			}
			break;
		default:
			Node = XMLStuff.EntityData->GetNodesByTypeVarSub(etype, evar, esub, strict); 
			Childs = XMLStuff.EntityData->childs[{ toint(Node["type"]), toint(Node["variant"]), toint(Node["subtype"]) }];
			break;
		}
	}
	else {
		Node = XMLStuff.EntityData->GetNodesByTypeVarSub(etype, evar, esub, strict);
		Childs = XMLStuff.EntityData->childs[{ toint(Node["type"]), toint(Node["variant"]), toint(Node["subtype"]) }];
	}
	Lua_PushXMLNode(L, Node,Childs);
	return 1;
}


LUA_FUNCTION(Lua_GetEntryByIdXML)
{
	if (!lua_isnumber(L, 1)) { return luaL_error(L, "Expected XMLNode as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	if (!lua_isnumber(L, 2)) { return luaL_error(L, "Expected number as parameter #2, got %s", lua_typename(L, lua_type(L, 2))); }
	int nodetype = (int)luaL_checknumber(L, 1);
	int id = (int)luaL_checknumber(L, 2);
	XMLAttributes Node;
	XMLChilds Childs;
	switch (nodetype) {
	case 0:
		Node = XMLStuff.ModData->nodes[id];
		Childs = XMLStuff.ModData->childs[id];
		break;
	case 1:
		Node = XMLStuff.EntityData->nodes[XMLStuff.EntityData->byorder[id]];
		Childs = XMLStuff.EntityData->childs[XMLStuff.EntityData->byorder[id]];
		break;
	case 2:
		Node = XMLStuff.PlayerData->nodes[id];
		Childs = XMLStuff.PlayerData->childs[id];
		break;
	case 3:
		Node = XMLStuff.ItemData->nodes[id];
		Childs = XMLStuff.ItemData->childs[id];
		break;
	case 4:
		Node = XMLStuff.TrinketData->nodes[id];
		Childs = XMLStuff.TrinketData->childs[id];
		break;
	case 5:
		Node = XMLStuff.PillData->nodes[id];
		Childs = XMLStuff.PillData->childs[id];
		break;
	case 6:
		Node = XMLStuff.CardData->nodes[id];
		Childs = XMLStuff.CardData->childs[id];
		break;
	case 7:
		Node = XMLStuff.MusicData->nodes[id];
		Childs = XMLStuff.MusicData->childs[id];
		break;
	case 8:
		Node = XMLStuff.SoundData->nodes[id];
		Childs = XMLStuff.SoundData->childs[id];
		break;
	case 9:
		Node = XMLStuff.ChallengeData->nodes[id];
		Childs = XMLStuff.ChallengeData->childs[id];
		break;
	case 10:
		Node = XMLStuff.PoolData->nodes[id];
		Childs = XMLStuff.PoolData->childs[id];
		break;
	case 11:
		Node = XMLStuff.NightmareData->nodes[id];
		Childs = XMLStuff.NightmareData->childs[id];
		break;
	case 12:
		Node = XMLStuff.CostumeData->nodes[id];
		Childs = XMLStuff.CostumeData->childs[id];
		break;
	case 13:
		Node = XMLStuff.NullCostumeData->nodes[id];
		Childs = XMLStuff.CostumeData->childs[id];
		break;
	case 14:
		Node = XMLStuff.WispData->nodes[id];
		Childs = XMLStuff.WispData->childs[id];
		break;
	case 15:
		Node = XMLStuff.WispColorData->nodes[id];
		Childs = XMLStuff.WispColorData->childs[id];
		break;
	case 16:
		Node = XMLStuff.CurseData->nodes[id];
		Childs = XMLStuff.CurseData->childs[id];
		break;
	case 17:
		Node = XMLStuff.LocustData->nodes[id];
		Childs = XMLStuff.LocustData->childs[id];
		break;
	case 18:
		Node = XMLStuff.LocustColorData->nodes[id];
		Childs = XMLStuff.LocustColorData->childs[id];
		break;
	case 19:
		Node = XMLStuff.BombCostumeData->nodes[id];
		Childs = XMLStuff.BombCostumeData->childs[id];
		break;
	case 20:
		Node = XMLStuff.RecipeData->nodes[id];
		Childs = XMLStuff.RecipeData->childs[id];
		break;
	case 21:
		Node = XMLStuff.BossPoolData->nodes[id];
		Childs = XMLStuff.BossPoolData->childs[id];
		break;
	case 22:
		Node = XMLStuff.BossPortraitData->nodes[id];
		Childs = XMLStuff.BossPortraitData->childs[id];
		break;
	case 23:
		Node = XMLStuff.CutsceneData->nodes[id];
		Childs = XMLStuff.CutsceneData->childs[id];
		break;
	case 24:
		Node = XMLStuff.StageData->nodes[id];
		Childs = XMLStuff.StageData->childs[id];
		break;
	case 25:
		Node = XMLStuff.BackdropData->nodes[id];
		Childs = XMLStuff.BackdropData->childs[id];
		break;
	case 26:
		Node = XMLStuff.AchievementData->nodes[id];
		Childs = XMLStuff.AchievementData->childs[id];
		break;
	case 27:
		Node = XMLStuff.GiantBookData->nodes[id];
		Childs = XMLStuff.GiantBookData->childs[id];
		break;
	case 28:
		Node = XMLStuff.BossRushData->nodes[id];
		Childs = XMLStuff.BossRushData->childs[id];
		break;
	case 29:
		Node = XMLStuff.PlayerFormData->nodes[id];
		Childs = XMLStuff.PlayerFormData->childs[id];
		break;
	case 30:
		Node = XMLStuff.NullItemData->nodes[id];
		Childs = XMLStuff.NullItemData->childs[id];
		break;
	}
	Lua_PushXMLNode(L, Node, Childs);
	return 1;
}

LUA_FUNCTION(Lua_GetEntryByNameXML)
{
	if (!lua_isnumber(L, 1)) { return luaL_error(L, "Expected XMLNode as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	if (!lua_isstring(L, 2)) { return luaL_error(L, "Expected string as parameter #2, got %s", lua_typename(L, lua_type(L, 2))); }
	int nodetype = (int)luaL_checknumber(L, 1);
	string entityname = string(luaL_checkstring(L, 2));
	XMLAttributes Node;
	XMLChilds Childs;
	switch (nodetype) {
	case 0:
		Node = XMLStuff.ModData->GetNodeByName(entityname);
		Childs = XMLStuff.ModData->childs[XMLStuff.ModData->byname[entityname]];
		break;
	case 1:
		Node = XMLStuff.EntityData->GetNodeByName(entityname);
		Childs = XMLStuff.EntityData->childs[XMLStuff.EntityData->byname[entityname]];
		break;
	case 2:
		Node = XMLStuff.PlayerData->GetNodeByName(entityname);
		Childs = XMLStuff.PlayerData->childs[XMLStuff.PlayerData->byname[entityname]];
		break;
	case 3:
		Node = XMLStuff.ItemData->GetNodeByName(entityname);
		Childs = XMLStuff.ItemData->childs[XMLStuff.ItemData->byname[entityname]];
		break;
	case 4:
		Node = XMLStuff.TrinketData->GetNodeByName(entityname);
		Childs = XMLStuff.TrinketData->childs[XMLStuff.TrinketData->byname[entityname]];
		break;
	case 5:
		Node = XMLStuff.PillData->GetNodeByName(entityname);
		Childs = XMLStuff.PillData->childs[XMLStuff.PillData->byname[entityname]];
		break;
	case 6:
		Node = XMLStuff.CardData->GetNodeByName(entityname);
		Childs = XMLStuff.CardData->childs[XMLStuff.CardData->byname[entityname]];
		break;
	case 7:
		Node = XMLStuff.MusicData->GetNodeByName(entityname);
		Childs = XMLStuff.MusicData->childs[XMLStuff.MusicData->byname[entityname]];
		break;
	case 8:
		Node = XMLStuff.SoundData->GetNodeByName(entityname);
		Childs = XMLStuff.SoundData->childs[XMLStuff.SoundData->byname[entityname]];
		break;
	case 9:
		Node = XMLStuff.ChallengeData->GetNodeByName(entityname);
		Childs = XMLStuff.ChallengeData->childs[XMLStuff.ChallengeData->byname[entityname]];
		break;
	case 10:
		Node = XMLStuff.PoolData->GetNodeByName(entityname);
		Childs = XMLStuff.PoolData->childs[XMLStuff.PoolData->byname[entityname]];
		break;
	case 11:
		Node = XMLStuff.NightmareData->GetNodeByName(entityname);
		Childs = XMLStuff.NightmareData->childs[XMLStuff.NightmareData->byname[entityname]];
		break;
	case 12:
		Node = XMLStuff.CostumeData->GetNodeByName(entityname);
		Childs = XMLStuff.CostumeData->childs[XMLStuff.CostumeData->byname[entityname]];
		break;
	case 13:
		Node = XMLStuff.NullCostumeData->GetNodeByName(entityname);
		Childs = XMLStuff.CostumeData->childs[XMLStuff.NullCostumeData->byname[entityname]];
		break;
	case 14:
		Node = XMLStuff.WispData->GetNodeByName(entityname);
		Childs = XMLStuff.WispData->childs[XMLStuff.WispData->byname[entityname]];
		break;
	case 15:
		Node = XMLStuff.WispColorData->GetNodeByName(entityname);
		Childs = XMLStuff.WispColorData->childs[XMLStuff.WispColorData->byname[entityname]];
		break;
	case 16:
		Node = XMLStuff.CurseData->GetNodeByName(entityname);
		Childs = XMLStuff.CurseData->childs[XMLStuff.CurseData->byname[entityname]];
		break;
	case 17:
		Node = XMLStuff.LocustData->GetNodeByName(entityname);
		Childs = XMLStuff.LocustData->childs[XMLStuff.LocustData->byname[entityname]];
		break;
	case 18:
		Node = XMLStuff.LocustColorData->GetNodeByName(entityname);
		Childs = XMLStuff.LocustColorData->childs[XMLStuff.LocustColorData->byname[entityname]];
		break;
	case 19:
		Node = XMLStuff.BombCostumeData->GetNodeByName(entityname);
		Childs = XMLStuff.BombCostumeData->childs[XMLStuff.BombCostumeData->byname[entityname]];
		break;
	case 20:
		Node = XMLStuff.RecipeData->GetNodeByName(entityname);
		Childs = XMLStuff.RecipeData->childs[XMLStuff.RecipeData->byname[entityname]];
		break;
	case 21:
		Node = XMLStuff.BossPoolData->GetNodeByName(entityname);
		Childs = XMLStuff.BossPoolData->childs[XMLStuff.BossPoolData->byname[entityname]];
		break;
	case 22:
		Node = XMLStuff.BossPortraitData->GetNodeByName(entityname);
		Childs = XMLStuff.BossPortraitData->childs[XMLStuff.BossPortraitData->byname[entityname]];
		break;
	case 23:
		Node = XMLStuff.CutsceneData->GetNodeByName(entityname);
		Childs = XMLStuff.CutsceneData->childs[XMLStuff.CutsceneData->byname[entityname]];
		break;
	case 24:
		Node = XMLStuff.StageData->GetNodeByName(entityname);
		Childs = XMLStuff.StageData->childs[XMLStuff.StageData->byname[entityname]];
		break;
	case 25:
		Node = XMLStuff.BackdropData->GetNodeByName(entityname);
		Childs = XMLStuff.BackdropData->childs[XMLStuff.BackdropData->byname[entityname]];
		break;
	case 26:
		Node = XMLStuff.AchievementData->GetNodeByName(entityname);
		Childs = XMLStuff.AchievementData->childs[XMLStuff.AchievementData->byname[entityname]];
		break;
	case 27:
		Node = XMLStuff.GiantBookData->GetNodeByName(entityname);
		Childs = XMLStuff.GiantBookData->childs[XMLStuff.GiantBookData->byname[entityname]];
		break;
	case 28:
		Node = XMLStuff.BossRushData->GetNodeByName(entityname);
		Childs = XMLStuff.BossRushData->childs[XMLStuff.BossRushData->byname[entityname]];
		break;
	case 29:
		Node = XMLStuff.PlayerFormData->GetNodeByName(entityname);
		Childs = XMLStuff.PlayerFormData->childs[XMLStuff.PlayerFormData->byname[entityname]];
		break;
	case 30:
		Node = XMLStuff.NullItemData->GetNodeByName(entityname);
		Childs = XMLStuff.NullItemData->childs[XMLStuff.NullItemData->byname[entityname]];
		break;
	}	
	Lua_PushXMLNode(L, Node,Childs);
	return 1;
}
LUA_FUNCTION(Lua_GetNumEntries)
{
	if (!lua_isnumber(L, 1)) { return luaL_error(L, "Expected XMLNode as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	int nodetype = (int)luaL_checknumber(L, 1);
	XMLAttributes Node;
	XMLChilds Childs;
	switch (nodetype) {
	case 0:
		lua_pushinteger(L, XMLStuff.ModData->nodes.size());
		break;
	case 1:
		lua_pushinteger(L, XMLStuff.EntityData->nodes.size());
		break;
	case 2:
		lua_pushinteger(L, XMLStuff.PlayerData->nodes.size());
		break;
	case 3:
		lua_pushinteger(L, XMLStuff.ItemData->nodes.size());
		break;
	case 4:
		lua_pushinteger(L, XMLStuff.TrinketData->nodes.size());
		break;
	case 5:
		lua_pushinteger(L, XMLStuff.PillData->nodes.size());
		break;
	case 6:
		lua_pushinteger(L, XMLStuff.CardData->nodes.size());
		break;
	case 7:
		lua_pushinteger(L, XMLStuff.MusicData->nodes.size());
		break;
	case 8:
		lua_pushinteger(L, XMLStuff.SoundData->nodes.size());
		break;
	case 9:
		lua_pushinteger(L, XMLStuff.ChallengeData->nodes.size());
		break;
	case 10:
		lua_pushinteger(L, XMLStuff.PoolData->nodes.size());
		break;
	case 11:
		lua_pushinteger(L, XMLStuff.NightmareData->nodes.size());
		break;
	case 12:
		lua_pushinteger(L, XMLStuff.CostumeData->nodes.size());
		break;
	case 13:
		lua_pushinteger(L, XMLStuff.NullCostumeData->nodes.size());
		break;
	case 14:
		lua_pushinteger(L, XMLStuff.WispData->nodes.size());
		break;
	case 15:
		lua_pushinteger(L, XMLStuff.WispColorData->nodes.size());
		break;
	case 16:
		lua_pushinteger(L, XMLStuff.CurseData->nodes.size());
		break;
	case 17:
		lua_pushinteger(L, XMLStuff.LocustData->nodes.size());
		break;
	case 18:
		lua_pushinteger(L, XMLStuff.LocustColorData->nodes.size());
		break;
	case 19:
		lua_pushinteger(L, XMLStuff.BombCostumeData->nodes.size());
		break;
	case 20:
		lua_pushinteger(L, XMLStuff.RecipeData->nodes.size());
		break;
	case 21:
		lua_pushinteger(L, XMLStuff.BossPoolData->nodes.size());
		break;
	case 22:
		lua_pushinteger(L, XMLStuff.BossPortraitData->nodes.size());
		break;
	case 23:
		lua_pushinteger(L, XMLStuff.CutsceneData->nodes.size());
		break;
	case 24:
		lua_pushinteger(L, XMLStuff.StageData->nodes.size());
		break;
	case 25:
		lua_pushinteger(L, XMLStuff.BackdropData->nodes.size());
		break;
	case 26:
		lua_pushinteger(L, XMLStuff.AchievementData->nodes.size());
		break;
	case 27:
		lua_pushinteger(L, XMLStuff.GiantBookData->nodes.size());
		break;
	case 28:
		lua_pushinteger(L, XMLStuff.BossRushData->nodes.size());
		break;
	case 29:
		lua_pushinteger(L, XMLStuff.PlayerFormData->nodes.size());;
		break;
	case 30:
		lua_pushinteger(L, XMLStuff.NullItemData->nodes.size());;
		break;
	}	
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	ClearXMLData();
	super();
	lua::LuaStackProtector protector(_state);

	lua_newtable(_state);
	lua::TableAssoc(_state, "GetEntryByName", Lua_GetEntryByNameXML);
	lua::TableAssoc(_state, "GetEntryById", Lua_GetEntryByIdXML);
	lua::TableAssoc(_state, "GetNumEntries", Lua_GetNumEntries);
	lua::TableAssoc(_state, "GetEntityByTypeVarSub", Lua_FromTypeVarSub);
	lua::TableAssoc(_state, "GetEntryFromEntity", Lua_GetFromEntity);
	lua_setglobal(_state, "XMLData");
}


//Crash Prevention//
int getLineNumber(const char* data, const char* errorOffset) {
	if (strlen(errorOffset) <= 0) { return 0; }
	int lineNumber = 1;
	const char* current = data;
	while (current < errorOffset) {
		if (*current == '\n') {
			lineNumber++;
		}
		current++;
	}
	return lineNumber;
}

bool XMLParse(xml_document<char>* xmldoc, char* xml,const string &dir) {
	try {
		if (strlen(xml) == strlen(xml + 1)) {
			xmldoc->parse<0>(xml);
		}else{
			char* zeroTerminatedStr  = new char[strlen(xml) + 1];
			strcpy(zeroTerminatedStr, xml);
			xmldoc->parse<0>(zeroTerminatedStr);
		}
		return true;
	}
	catch (rapidxml::parse_error err) {
		int lineNumber = getLineNumber(xml, err.where<char>());
		string a = stringlower((char*)string(xml).substr(0, 60).c_str());
		string reason = err.what() + string(" at line ") + to_string(lineNumber);
		string error = "[XMLError] " + reason + " in " + dir;
		g_Game->GetConsole()->PrintError(error);
		KAGE::LogMessage(3, (error + "\n").c_str());
		//printf("%s \n", error.c_str());
		//mclear(xmldoc);
	}
	return false;
}

char* GetResources(char* xml,const string &dir,const string &filename) {
	vector<string> paths = { dir + "\\resources\\" + filename, dir + "\\resources-dlc3\\" + filename };
	for (const string & path : paths) {
		ifstream file(path.c_str());
		if (file.is_open()) {
			std::stringstream sbuffer;
			sbuffer << file.rdbuf();
			string filedata = sbuffer.str();
			char* buffer = new char[filedata.length()];
			strcpy(buffer, filedata.c_str());
			return buffer;
		}
	}
	return "";
}

bool GetContent(const string &dir, xml_document<char>* xmldoc) {
	ifstream file(dir.c_str());
	if (file.is_open()) {
//		printf("path: %s \n", dir.c_str());
		std::stringstream sbuffer;
		sbuffer << file.rdbuf();
		string filedata = sbuffer.str();
		char* buffer = new char[filedata.length() + 1];
		strcpy(buffer, filedata.c_str());
		if (XMLParse(xmldoc, buffer, dir)) {
		delete[] buffer;
		return true;
		}
	}
	return false;
}

xml_node<char>* find_child(
	xml_node<char>* parent,
	const string& type,
	const string& attribute,
	const string& value)
{
	xml_node<char>* node = parent->first_node(type.c_str());
	//printf("node1");
	try {
	while (node)
	{
		xml_attribute<char>* attr = node->first_attribute(attribute.c_str());
		if (attr && value == attr->value()) return node;
		node = node->next_sibling(type.c_str());
	}
	//printf("node2");
	return node;
	}
	catch(exception ex){
		return NULL;
	}
}

char* IntToChar(int number) {
	std::string numberString = std::to_string(number);
	char* charPointer = new char[numberString.length() + 1];
	std::strcpy(charPointer, numberString.c_str());
	return charPointer;
}

void CustomXMLCrashPrevention(xml_document<char>* xmldoc, const char* filename) {
	if (strcmp(filename, "cutscenes.xml") == 0) {
		int id = 26;
		xml_node<char>* root = xmldoc->first_node();
		for (xml_node<char>* auxnode = root->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes node;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				node[stringlower(attr->name())] = string(attr->value());
			}
			if (node.count("realid") == 0) {				
				if (node.count("id") > 0) {
					int nodeid = stoi(node["id"]);
					if (nodeid > 26) {
						if (node.count("relativeid") == 0) {
							xml_attribute<char>* relativeid = new xml_attribute<char>(); relativeid->name("relativeid"); relativeid->value(IntToChar(stoi(node["id"]))); auxnode->append_attribute(relativeid);
						}
						auxnode->remove_attribute(auxnode->first_attribute("id"));
						id += 1;
						xml_attribute<char>* realid = new xml_attribute<char>(); realid->name("realid"); realid->value(IntToChar(id)); auxnode->append_attribute(realid);						
						node["realid"] = IntToChar(id);//node["id"]; //I dont think ids should be respected here
					}
					else {
						xml_attribute<char>* realid = new xml_attribute<char>(); realid->name("realid"); realid->value(IntToChar(stoi(node["id"]))); auxnode->append_attribute(realid);
						node["realid"] = node["id"];
					}
				}
				else {
					id += 1;
					xml_attribute<char>* realid = new xml_attribute<char>(); realid->name("realid"); realid->value(IntToChar(id)); auxnode->append_attribute(realid);
					node["realid"] = to_string(id);
				}
				/*
				if (queuedcutscene > 0) {
					if ((strcmp(node["realid"].c_str(), IntToChar(queuedcutscene)) == 0)) {
						if (auxnode->first_attribute("id") != NULL){
							auxnode->remove_attribute(auxnode->first_attribute("id"));
						}
						xml_attribute<char>* xxx = new xml_attribute<char>(); xxx->name("id"); xxx->value("1"); auxnode->append_attribute(xxx); //porn
						
					}
					else if (auxnode->first_attribute("id") != NULL) {
						xml_attribute<char>* realid = new xml_attribute<char>(); realid->name("realid"); realid->value(auxnode->first_attribute("id")->value()); auxnode->append_attribute(realid);
						auxnode->remove_attribute(auxnode->first_attribute("id"));
					}
				}*/
			}
		}
	}else if (strcmp(filename, "stages.xml") == 0) {
		int id = 36;
		xml_node<char>* root = xmldoc->first_node();
		for (xml_node<char>* auxnode = root->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes node;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				node[stringlower(attr->name())] = string(attr->value());
			}
			if (node.count("realid") == 0) {
				if (node.count("id") > 0) {
					int nodeid = toint(node["id"]);
					if (nodeid > 36) {
						xml_attribute<char>* relativeid = new xml_attribute<char>(); relativeid->name("relativeid"); relativeid->value(IntToChar(stoi(node["id"]))); auxnode->append_attribute(relativeid);
						auxnode->remove_attribute(auxnode->first_attribute("id"));
						id += 1;
						xml_attribute<char>* realid = new xml_attribute<char>(); realid->name("realid"); realid->value(IntToChar(id)); auxnode->append_attribute(realid);
						node["realid"] = IntToChar(id);//node["id"]; //I dont think ids should be respected here
					}
					else {
						xml_attribute<char>* realid = new xml_attribute<char>(); realid->name("realid"); realid->value(IntToChar(stoi(node["id"]))); auxnode->append_attribute(realid);
						node["realid"] = node["id"];
					}
				}
				else {
					id += 1;
					xml_attribute<char>* realid = new xml_attribute<char>(); realid->name("realid"); realid->value(IntToChar(id)); auxnode->append_attribute(realid);
					node["realid"] = to_string(id);
				}
				int music = toint(node["music"]);
				if ((music == 0) && (XMLStuff.MusicData->byname.count(node["music"]) > 0)) {
					auxnode->first_attribute("music")->value(IntToChar(XMLStuff.MusicData->byname[node["music"]]));
				}
			}
		}
	}
}


bool endsWithPools(const char* str) {
	int lastIndex = strlen(str) - 1;

	// Find the index of the last non-space character
	while (lastIndex >= 0 && std::isspace(str[lastIndex])) {
		lastIndex--;
	}

	// Check if the substring from the last non-space character to the end is "</pool>"
	const char* target = "</pool>";
	int targetLength = strlen(target);
	int startIndex = lastIndex - targetLength + 1;

	if (startIndex >= 0 && strncmp(&str[startIndex], target, targetLength) == 0) {
		return true;
	}

	return false;
}

char* replaceChar(const char* str, const char* search, const char* replace) {
	int searchLen = strlen(search);
	int replaceLen = strlen(replace);
	int strLen = strlen(str);
	int count = 0;

	const char* pos = str;
	while ((pos = strstr(pos, search)) != nullptr) {
		count++;
		pos += searchLen;
	}

	int newLen = strLen + count * (replaceLen - searchLen);

	char* result = new char[newLen + 1]; // +1 for the null terminator
	char* resultPos = result;

	pos = str;
	while ((pos = strstr(pos, search)) != nullptr) {
		int prefixLen = pos - str;
		strncpy(resultPos, str, prefixLen);
		resultPos += prefixLen;

		strcpy(resultPos, replace);
		resultPos += replaceLen;

		pos += searchLen;
		str = pos;
	}

	strcpy(resultPos, str);

	return result;
}


char* replaceLastChar(const char* str, const char* search, const char* replace) {
	int searchLen = strlen(search);
	int replaceLen = strlen(replace);
	int strLen = strlen(str);
	int count = 0;

	const char* pos = str;
	while ((pos = strstr(pos, search)) != nullptr) {
		count++;
		pos += searchLen;
	}

	if (count == 0) {
		char* result = new char[strLen + 1];
		strcpy(result, str);
		return result;
	}

	int newLen = strLen + (count - 1) * (replaceLen - searchLen);

	char* result = new char[newLen + 1]; 
	char* resultPos = result;

	pos = str;
	const char* lastPos = nullptr;
	while ((pos = strstr(pos, search)) != nullptr) {
		lastPos = pos;
		pos += searchLen;
	}

	int prefixLen = lastPos - str;
	strncpy(resultPos, str, prefixLen);
	resultPos += prefixLen;

	strcpy(resultPos, replace);
	resultPos += replaceLen;

	strcpy(resultPos, lastPos + searchLen);

	return result;
}


bool NodeHasSameAttrs(xml_node<>* node1, xml_node<>* node2) {
	for (xml_attribute<>* attr1 = node1->first_attribute(); attr1; attr1 = attr1->next_attribute()) {
		xml_attribute<>* attr2 = node2->first_attribute(attr1->name());
		if (!attr2 || strcmp(attr1->value(), attr2->value()) != 0) {
			return false;
		}
	}

	for (xml_attribute<>* attr2 = node2->first_attribute(); attr2; attr2 = attr2->next_attribute()) {
		xml_attribute<>* attr1 = node1->first_attribute(attr2->name());
		if (!attr1) {
			return false;
		}
	}

	return true;
}

int maxnodebackdrop = 60;

char * BuildModdedXML(char * xml,const string &filename,bool needsresourcepatch) {
	if (no) {return xml;}
	//resources
	if (needsresourcepatch) {
		for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
			if (mod->IsEnabled()) {
				string dir = std::filesystem::current_path().string() + "\\mods\\" + mod->GetDir();
				string resourcesdir = dir + "\\resources\\" + filename;
				char* xmlaux = GetResources(xml, dir, filename);
				if (strlen(xmlaux) > 1) {
					xml_document<char>* xmldoc = new xml_document<char>();
					if ((strcmp(filename.c_str(), "bosspools.xml")==0) && endsWithPools(xml)) {
						CharToChar(&xml, replaceLastChar(xml, "</pool>", "</bosspools>"));
					}
					if (XMLParse(xmldoc, xmlaux, resourcesdir)) {
						//mclear(xml);
						CharToChar(&xml, xmlaux);
					}
					mclear(xmldoc);
				}
			}
		}
	}
	//resources
	//content
    for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if (mod->IsEnabled()) {
			string lastmodid = string(mod->GetId());
			if (string(lastmodid).length() == 0) {
				lastmodid = string(mod->GetDir());
			}

			string dir = std::filesystem::current_path().string() + "\\mods\\" + mod->GetDir();
			string contentsdir = dir + "\\content\\" + filename;

			xml_document<char>* xmldoc = new xml_document<char>();
			if (XMLParse(xmldoc, xml, filename)) {
				xml_node<char>* root = xmldoc->first_node();
				xml_document<char>* resourcesdoc = new xml_document<char>();
				if (GetContent(contentsdir, resourcesdoc)) {
					xml_node<char>* resourcescroot = resourcesdoc->first_node();
					if (strcmp(filename.c_str(), "bosspools.xml") == 0) {
						for (xml_node<char>* auxnode = resourcescroot->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
							XMLAttributes node;
							for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
							{
								node[stringlower(attr->name())] = string(attr->value());
							}
							xml_node<char>* tocopy = find_child(root, auxnode->name(), "name", node["name"]);
							if ((tocopy == NULL) || (tocopy->first_attribute("name")->value() != node["name"])) {
								xml_node<char>* clonedNode = xmldoc->clone_node(auxnode);
								xml_attribute<char>* sourceid = new xml_attribute<char>();
								sourceid->name("sourceid");
								sourceid->value(lastmodid.c_str());
								clonedNode->append_attribute(sourceid);
								root->append_node(clonedNode);
							}
							else {
								for (xml_node<char>* auxchild = auxnode->first_node(); auxchild; auxchild = auxchild->next_sibling()) {
									xml_node<char>* clonedNode = xmldoc->clone_node(auxchild);
									xml_attribute<char>* sourceid = new xml_attribute<char>(); sourceid->name("sourceid"); sourceid->value(lastmodid.c_str()); clonedNode->append_attribute(sourceid);
									tocopy->append_node(clonedNode);
								}
							}
						}
					}else if (strcmp(filename.c_str(), "bossportraits.xml") == 0) {
						for (xml_node<char>* auxnode = resourcescroot->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
							xml_node<char>* clonedNode = xmldoc->clone_node(auxnode);
							xml_attribute<char>* sourceid = new xml_attribute<char>();sourceid->name("sourceid");sourceid->value(lastmodid.c_str());clonedNode->append_attribute(sourceid);
							root->append_node(clonedNode);
						}
					}
					else if (strcmp(filename.c_str(), "achievements.xml") == 0) {
						for (xml_node<char>* auxnode = resourcescroot->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
							xml_node<char>* clonedNode = xmldoc->clone_node(auxnode);
							xml_attribute<char>* sourceid = new xml_attribute<char>(); sourceid->name("sourceid"); sourceid->value(lastmodid.c_str()); clonedNode->append_attribute(sourceid);
							root->append_node(clonedNode);
						}
					}
					else if (strcmp(filename.c_str(), "backdrops.xml") == 0) {
						for (xml_node<char>* auxnode = resourcescroot->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
							xml_node<char>* clonedNode = xmldoc->clone_node(auxnode);
							xml_attribute<char>* sourceid = new xml_attribute<char>(); sourceid->name("sourceid"); sourceid->value(lastmodid.c_str()); clonedNode->append_attribute(sourceid);

							XMLAttributes node;
							for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
							{
								node[stringlower(attr->name())] = string(attr->value());
							}
							if (node.count("id") == 0) {
								maxnodebackdrop += 1;
								xml_attribute<char>* newid = new xml_attribute<char>(); newid->name("id"); newid->value(IntToChar(maxnodebackdrop)); clonedNode->append_attribute(newid);
							}

							root->append_node(clonedNode);
						}
					}
					else if (strcmp(filename.c_str(), "stages.xml") == 0) {
						//printf("1");
						for (xml_node<char>* auxnode = resourcescroot->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
							XMLAttributes node;
							for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
							{
								node[stringlower(attr->name())] = string(attr->value());
							}
							int id = toint(node["id"]);
							int music = toint(node["music"]);
							int backdrop = toint(node["backdrop"]);
							if ((music == 0) && (node["music"].length() > 0)) {
								char* track = IntToChar(XMLStuff.MusicData->byname[node["music"]]);
								auxnode->first_attribute("music")->value(track);
							}
							xml_node<char>* clonedNode = xmldoc->clone_node(auxnode);
							xml_attribute<char>* sourceid = new xml_attribute<char>(); sourceid->name("sourceid"); sourceid->value(lastmodid.c_str()); clonedNode->append_attribute(sourceid);
							root->append_node(clonedNode);
							//printf("2");
						}
					}
					else if (strcmp(filename.c_str(), "ambush.xml") == 0) {
						resourcescroot = resourcesdoc->first_node("bossrush"); //only bossrush works anyway, the rest was deprecated in rpeentance, so why bother?
						root = xmldoc->first_node("bossrush");
						for (xml_node<char>* auxnode = resourcescroot->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
							xml_node<char>* clonedNode = xmldoc->clone_node(auxnode);
							xml_attribute<char>* sourceid = new xml_attribute<char>(); sourceid->name("sourceid"); sourceid->value(lastmodid.c_str()); clonedNode->append_attribute(sourceid);
							root->append_node(clonedNode);
						}
					}
					else if (xmlfullmerge.find(filename) != xmlfullmerge.end()) { //generic fullmerge (a.k.a too complex to be minimalistic about it)
					}
					else if(xmlmaxnode.find(filename) != xmlmaxnode.end()){ //generic
						for (xml_node<char>* auxnode = resourcescroot->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
							xml_node<char>* clonedNode = xmldoc->clone_node(auxnode);
							xml_attribute<char>* sourceid = new xml_attribute<char>(); sourceid->name("sourceid"); sourceid->value(lastmodid.c_str()); clonedNode->append_attribute(sourceid);

							XMLAttributes node;
							for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
							{
								node[stringlower(attr->name())] = string(attr->value());
							}
							xmlmaxnode[filename]++;
							if (node.count("id") == 0) {
								xml_attribute<char>* newid = new xml_attribute<char>(); newid->name("id"); newid->value(IntToChar(xmlmaxnode[filename])); clonedNode->append_attribute(newid);
							}
							else {
								if (node.count("relativeid") == 0) {
									xml_attribute<char>* newid = new xml_attribute<char>(); newid->name("relativeid"); newid->value(clonedNode->first_attribute("id")->value()); clonedNode->append_attribute(newid);
								}
								clonedNode->first_attribute("id")->value(IntToChar(xmlmaxnode[filename]));
							}

							root->append_node(clonedNode);
						}
					}
					//actual shit

				}
				CustomXMLCrashPrevention(xmldoc, filename.c_str());
				resourcesdoc->clear();
				ostringstream modifiedXmlStream;
				modifiedXmlStream << *xmldoc;
				mclear(xmldoc);
				string modifiedXml = modifiedXmlStream.str();
				//printf("\nasdsasad: (%d)\n", strlen(xml));
				//mclear(xml);
				 xml =new char[modifiedXml.length() + 1];
				std::strcpy(xml, modifiedXml.c_str());
				modifiedXmlStream.clear();
			}
			else{
				mclear(xmldoc);
			}
		}
	}	

	xml_document<char>* xmldoc = new xml_document<char>();
	if (XMLParse(xmldoc, xml, filename)) {
		xml_node<char>* root = xmldoc->first_node();
		if (queuedhackyxmlvalue > 0) {
			if (strcmp(filename.c_str(), "cutscenes.xml") == 0) {
				xml_node<char>* tocopy = find_child(root, "cutscene", "realid", IntToChar(queuedhackyxmlvalue));
				if (tocopy != NULL) {
					if (tocopy->first_attribute("id")) { tocopy->remove_attribute(tocopy->first_attribute("id")); }
					xml_attribute<char>* attid = new xml_attribute<char>(); attid->name("id"); attid->value(IntToChar(queuedhackyxmltarget)); tocopy->append_attribute(attid);
				}
			}
			else if (strcmp(filename.c_str(), "stages.xml") == 0) {
				//printf("1");
				xml_node<char>* tocopy = find_child(root, "stage", "consoleid", IntToChar(queuedhackyxmlvalue));
				if (tocopy != NULL) {
					if (tocopy->first_attribute("id")) { tocopy->remove_attribute(tocopy->first_attribute("id")); }
					xml_attribute<char>* attid = new xml_attribute<char>(); attid->name("id"); attid->value(IntToChar(queuedhackyxmltarget)); tocopy->append_attribute(attid);
				}
				//printf("2");
				xml_node<char>* todel = find_child(root, "stage", "id", IntToChar(queuedhackyxmltarget));
				if (todel != NULL) {
					if (todel->first_attribute("id")) { todel->remove_node(todel); }
				}
				//printf("3");
			}
		}
		ostringstream modifiedXmlStream;
		modifiedXmlStream << *xmldoc;
		delete xmldoc;
		string modifiedXml = modifiedXmlStream.str();
		xml = new char[modifiedXml.length() + 1];
		std::strcpy(xml, modifiedXml.c_str());
	}
	if (queuedhackyxmlvalue > 0) {
		//printf("hackies done");
		//printf("s: %s",xml); 
	}
	if (xmlfullmerge.find(filename) != xmlfullmerge.end()) { printf("s: %s", xml); }
	//if (strcmp(filename.c_str(), "ambush.xml") == 0) { printf("s: %s", xml); }
	//content
	return xml;
}



bool charfind(const char* target, const char* lookup, size_t maxOffset) {
	size_t haystackLen = strlen(target);
	size_t needleLen = strlen(lookup);
	if (maxOffset >= haystackLen) {
		return nullptr; // Offset exceeds the haystack length, no match possible.
	}
	if (maxOffset + needleLen > haystackLen) {
		maxOffset = haystackLen - needleLen; // Adjust the offset to avoid overflows.
	}


	for (size_t i = 0; i <= maxOffset; i++) {
		// Compare characters one by one in lowercase
		bool match = true;
		for (size_t j = 0; j < needleLen; j++) {
			if (std::tolower(target[i + j]) != std::tolower(lookup[j])) {
				match = false;
				return match;
			}
		}

		if (match) {
			return match;
		}
	}

	return false;
}


HOOK_METHOD(xmldocument_rep, parse, (char* xmldata)-> void) {
	if (xmlsloaded) {
		//printf("XML: %s", xmldata);
		if ((bosspoolsxml != NULL) && (charfind(xmldata, "<bosspool", 50))) {
			char* x = new char[strlen(bosspoolsxml)];
			strcpy(x, bosspoolsxml);
			super(x);
			mclear(x);
			return;
		}
		else if (charfind(xmldata, "<cuts", 50)) {
			super(BuildModdedXML(xmldata, "cutscenes.xml", false));
		}
		else if (charfind(xmldata, "<itempoo", 50)) {
			//XMLStuff.PoolData->Clear();
			return super(xmldata);
		}
		else{
			return super(xmldata); 
		}
	}
	if (g_Manager->GetOptions()->ModsEnabled()) {
	try {
		//string xml = string(xmldata);
		//string xmlsub = xml.substr(0, 60);
		//string a = stringlower(xmlsub.c_str());
		if (charfind(xmldata,"<itempoo", 50)) {
			//XMLStuff.PoolData->Clear();
		}

		if (charfind(xmldata, "<bosspool", 50)) {
			if (bosspoolsxml != NULL) {
				super(bosspoolsxml);
			}
			else {
				char* x = BuildModdedXML(xmldata, "bosspools.xml", true);
				bosspoolsxml = new char[strlen(x)];
				strcpy(bosspoolsxml, x);
				super(x);
			}
		}else if (charfind(xmldata, "<backd", 50)) {
				super(BuildModdedXML(xmldata, "backdrops.xml", false));
		}else if (charfind(xmldata, "<bosse", 50)) {
			super(BuildModdedXML(xmldata, "bossportraits.xml", false));
		}
		else if (charfind(xmldata, "<ach", 50)) {
			XMLStuff.AchievementData->Clear();
			super(BuildModdedXML(xmldata, "achievements.xml", false));
		}else if (charfind(xmldata, "<cuts", 50)) {
			super(BuildModdedXML(xmldata, "cutscenes.xml", false));
		}
		else if (charfind(xmldata, "<giantb", 50)) {
			super(BuildModdedXML(xmldata, "giantbook.xml", false));
		}
		else if (charfind(xmldata, "<playerfo", 50)) {
			//printf("yoyoyo %s", BuildModdedXML(xmldata, "playerforms.xml", false));
			super(BuildModdedXML(xmldata, "playerforms.xml", false));
		}
		else if ((charfind(xmldata, "<ambush", 50)) || (charfind(xmldata, "<bossru", 50)) || (charfind(xmldata, "<bossamb", 50))) {
			super(BuildModdedXML(xmldata, "ambush.xml", false));
		}
		else if (charfind(xmldata, "<stages", 50)) {
			super(BuildModdedXML(xmldata, "stages.xml", false));
		}else if (charfind(xmldata, "<reci",  50)) {
			string xml = string(xmldata);
			regex regexPattern(R"(\boutput\s*=\s*["']([^"']+)["'])");
			smatch match;
			auto words_begin = std::sregex_iterator(xml.begin(), xml.end(), regexPattern);
			auto words_end = std::sregex_iterator();

			for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
				string itemname = (*it)[1].str();
				//printf("itemname: %s \n", itemname.c_str());
				size_t pos = 0;
				int itemid = toint(itemname);
				if (itemid == 0) {
					string itemid = to_string(XMLStuff.ItemData->byname[itemname]);
					while ((pos = xml.find(itemname, pos)) != std::string::npos) {
						xml.replace(pos, itemname.length(), itemid);
						pos += itemname.length();
					}
				}

			}
			char* s = new char[xml.length() + 1];
			std::strcpy(s, xml.c_str());
			super(s);
		}else{
			super(xmldata);
		}
	}
	catch (rapidxml::parse_error err) {
		int lineNumber = getLineNumber(xmldata, err.where<char>());
		string xml = string(xmldata);
		string xmlsub = xml.substr(0, 60);
		string a = stringlower(xmlsub.c_str());
		xmlerrors.push_back(err.what() + string(" at line ") + to_string(lineNumber));
		if (a.find("<ent") < 50) {
			a = "<entities anm2root=\"gfx/\" version=\"5\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </entities>";
		}
		else if (a.find("<mus") < 50) {
			a = "<music root=\"music/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </music>";
		}
		else if (a.find("<cuts") < 50) {
			a = "<cutscenes root=\"gfx/cutscenes/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </cutscenes>";
		}
		else if (a.find("<pock") < 50) {
			a = "<pocketitems xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </pocketitems>";
		}
		else if (a.find("<itempo") < 50) {
			itempoolerror = true;
			a = "<ItemPools xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </ItemPools>";
		}
		else if (a.find("<item") < 50) {
			a = "<items gfxroot=\"gfx/items/\" version=\"1\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </items>";
		}
		else if (a.find("<so") < 50) {
			a = "<sounds root=\"sfx/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </sounds>";
		}
		else if (a.find("<play") < 50) {
			a = "<players nameimageroot=\"gfx/ui/boss/\" portraitroot=\"gfx/ui/stage/\" root=\"gfx/characters/costumes/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </players>";
		}
		else if (a.find("<chal") < 50) {
			a = "<challenges version=\"1\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </challenges>";
		}	
		else if (a.find("<ni") < 50) { //I made this match 2 letters, because adding the 3rd one made me uncomfortable
			a = "<nightmares root=\"gfx/ui/stage/\" progressAnm2=\"progress.anm2\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </nightmares>";
		}
		else if (a.find("<cos") < 50) { 
			a = "<costumes anm2root=\"gfx/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </costumes>";
		}
		else if (a.find("<bomb") < 50) {
			a = "<bombcostumes anm2root=\"gfx/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </bombcostumes>";
		}
		else if (a.find("<cus") < 50) { //typo
			a = "<costumes anm2root=\"gfx/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </costumes>";
		}
		else if (a.find("<curs") < 50) { 
			a = "<curses xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </curses>";
		}
		else if (a.find("<loc") < 50) {
			a = "<locusts gfxroot=\"gfx/familiar/wisps/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </locusts>";
		}
		else if (a.find("<wis") < 50) { 
			a = "<wisps gfxroot=\"gfx/familiar/wisps/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </wisps>";
		}
		else if (a.find("<reci") < 50) {
			a = "<recipes xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </recipes>";
		}
		else if (a.find("<whis") < 50) { //typo
			a = "<wisps gfxroot=\"gfx/familiar/wisps/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </wisps>";
		}
		else if (a.find("<achi") < 50) { 
			a = "<achievements gfxroot=\"gfx/ui/achievement/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </achievements>";
		}
		printf("ERROR: %s", a.c_str());
		
		xmldata = new char[a.length() + 1];
		strcpy(xmldata, a.c_str());
		super(xmldata);
	}
	}
	else {
		super(xmldata);
	}
}
//Crash Prevention//

#endif