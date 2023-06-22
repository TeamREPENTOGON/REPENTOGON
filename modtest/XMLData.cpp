#if !defined(XMLDATALoaded)
#define XMLDATALoaded true

#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <cctype>

#include "XMLData.h"

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "mologie_detours.h"
#include "rapidxml.hpp"

#include "LogViewer.h"
#include <lua.hpp>
#include "LuaCore.h"
#include <filesystem>
#include <regex>

using namespace rapidxml;
using namespace std;

char* lastmodid = "BaseGame";
bool iscontent = false;
bool isitemmetadata = false;

vector<string> xmlerrors;
string currpath;

XMLData XMLStuff;


void ClearXMLData() {
	XMLStuff.PlayerData->Clear();
	XMLStuff.ItemData->Clear();
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

IsaacString toIsaacString(string s) {
	IsaacString str;
	const char* err = s.c_str();
	if (strlen(err) < 16) {
		strcpy(str.text, err);
	}
	else {
		*(char**)str.text = (char*)err;
	}
	return str;
}

int toint(string str) {
	try {
		return stoi(str);
	}
	catch (exception ex) {
		return 0;
	}
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

	if (stringlower(xmlpath).find("/content/") > 0) {
		iscontent = true;
	}
	else {
		iscontent = false;
	}
	if (string(lastmodid).length() == 0) {
		string path = string(xmlpath);
		int first = path.find("/mods/") + 6;
		int last = path.find("/content");
		if (!iscontent) {
			last = path.find("/resources");
		}
		path = path.substr(first, last - first); //when the id is null(which it can fucking be) just use the folder name as ID...
		lastmodid = new char[path.length() + 1]; //this is the sort of stuff I dont like about C++
		strcpy(lastmodid, path.c_str());
	}
	logViewer.AddLog("[REPENTOGON]", "Mod ID: %s \n", lastmodid);
	
}

ModEntry* GetModEntryById(string Id) {
	for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if ((mod->GetId() != NULL) && (strcmp(Id.c_str(), mod->GetId()) == 0)) {
			return mod;
		}
	}
}

ModEntry* GetModEntryByName(string Name) {
	for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if (strcmp(Name.c_str(), mod->GetName().c_str())==0) {
			return mod;
		}
	}
}

ModEntry* GetModEntryByDir(string Dir) {
	for (ModEntry* mod : g_Manager->GetModManager()->_mods) {
		if (strcmp(Dir.c_str(), mod->GetDir().c_str())==0) {
			return mod;
		}
	}
}

ModEntry* GetModEntryByContentPath(string path) {
	if ((path.find("/content/") != string::npos) && (path.find("/mods/") != string::npos)) {
		std::regex regex("/mods/(.*?)/content/");
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
		
		if (entry->IsEnabled()) { mod["enabled"] = "true"; }
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


void ProcessXmlNode(xml_node<char>* node) {
	if (!node) { return; }
	//if (currpath.length() > 0) { printf("Loading: %s \n", currpath.c_str()); }
	Manager* manager = g_Manager;
	StringTable* stringTable = manager->GetStringTable();
	uint32_t unk;
	string middleman = stringlower(node->name());
	const char* nodename = middleman.c_str();//fucking christ, dude, C++ has these things that drive you nuts sometimes...
	if ((strcmp(nodename, "entity") == 0))
	{
		for (xml_node<char>* auxnode = node; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes entity;
			xml_attribute<>* subattr;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				entity[stringlower(attr->name())] = attr->value();
			}
			int type = toint(entity["id"]);
			int var = 0;
			int sub = 0;
			
			
			if (entity["subtype"].length() > 0) { sub = toint(entity["subtype"]); }
			tuple idx = { type, var, sub };
			if (entity["variant"].length() > 0) { var = toint(entity["variant"]); idx = { type, var, sub };}
			else if((type < 10) || (type >= 1000)) {
				while (XMLStuff.EntityData->nodes.count(idx) > 0) {
					var += 1;
					idx = { type, var, sub };
				} 
			}

			
			if (iscontent && (XMLStuff.EntityData->nodes.count(idx) > 0)) {
				XMLAttributes collider = XMLStuff.EntityData->nodes[idx];
				XMLAttributes collidermod = XMLStuff.ModData->nodes[XMLStuff.ModData->byid[collider["sourceid"]]];
				XMLAttributes  lastmod = XMLStuff.ModData->nodes[XMLStuff.ModData->byid[lastmodid]];
				//g_Game->GetConsole()->PrintError(toIsaacString("[XML] The entity:" + entity["name"] + "(From: " + lastmodid + ") collides with " + collider["name"] + "from (" + collidermod["name"] + ")"));
				if (false){
					printf("[XML] The entity: %s(From: %s) collides with %s (from %s) \n", entity["name"].c_str(), lastmod["name"].c_str(), collider["name"].c_str(), collidermod["name"].c_str());
				}
				//Conflict resolve emulation begin
				if ((type < 10) || (type >= 1000)) {
					do {
						var += 1;
						idx = { type, var, sub };
					} while (XMLStuff.EntityData->nodes.count(idx) > 0);
				}
				else {
					do {
						type += 1;
						idx = { type, var, sub };
					} while (XMLStuff.EntityData->nodes.count(idx) > 0);
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
			}
			XMLStuff.EntityData->ProcessChilds(auxnode, idx);
			XMLStuff.EntityData->nodes[idx] = entity;
			XMLStuff.EntityData->byname[entity["name"]] = idx;
			XMLStuff.EntityData->bytype[entity["id"]] = idx;
		}
	}
	else if ((strcmp(nodename,"player") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node->parent();
		for (xml_node<char>* auxnode = node; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes player;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				player[stringlower(attr->name())] = string(attr->value());
			}
			if ((player.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(player["id"]);
			}
			else {
				if (player.count("id") > 0) { player["relativeid"] = player["id"]; }
				XMLStuff.PlayerData->maxid += 1;
				player["id"] = to_string(XMLStuff.PlayerData->maxid);
				id = XMLStuff.PlayerData->maxid;
			}
			if (id > XMLStuff.PlayerData->maxid) {
				XMLStuff.PlayerData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				player[stringlower(attr->name())] = attr->value();
			}

			if (player["name"].find("#") != string::npos) {
				player["untranslatedname"] = player["name"];
				player["name"] = string(stringTable->GetString("Players", 0, player["name"].substr(1, player["name"].length()).c_str(), &unk));
			}

			if (player["birthright"].find("#") != string::npos) {
				player["untranslatedbirthright"] = player["birthright"];
				player["birthright"] = string(stringTable->GetString("Players", 0, player["birthright"].substr(1, player["birthright"].length()).c_str(), &unk));
			}

			if (player.count("bskinparent") > 0){
				XMLStuff.PlayerData->byname[player["name"] + "-Tainted-"] = id; }
			else{ 
				XMLStuff.PlayerData->byname[player["name"]] = id; }

			if (player.count("bskinparent") > 0) {
				XMLStuff.PlayerData->bynamemod[player["name"] + lastmodid + "-Tainted-"] = id;
			}
			else {
				XMLStuff.PlayerData->bynamemod[player["name"] + lastmodid] = id;
			}

			player["sourceid"] = lastmodid;
			if (player.count("relativeid") > 0) { XMLStuff.PlayerData->byrelativeid[lastmodid + player["relativeid"]] = id;}

			XMLStuff.PlayerData->ProcessChilds(auxnode, id);
			XMLStuff.PlayerData->nodes[id] = player;
			XMLStuff.ModData->players[lastmodid] += 1;
			}
	}
	else if ((strcmp(nodename, "pocketitems") == 0)) {
		int id = 1;
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
				}

				if (card["description"].find("#") != string::npos) {
					card["untranslateddescription"] = card["name"];
					card["description"] = string(stringTable->GetString("PocketItems", 0, card["description"].substr(1, card["description"].length()).c_str(), &unk));
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
				if (pill.count("id") && ((strcmp(lastmodid,"BaseGame") == 0) || !iscontent)) { 
					id = toint(pill["id"]);
				}
				else {
					if (pill.count("id") > 0) { pill["relativeid"] = pill["id"]; }
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
				}

				if (pill["description"].find("#") != string::npos) {
					pill["untranslateddescription"] = pill["name"];
					pill["description"] = string(stringTable->GetString("PocketItems", 0, pill["description"].substr(1, pill["description"].length()).c_str(), &unk));
				}
				if (pill.count("relativeid") > 0) { XMLStuff.PillData->byrelativeid[lastmodid + pill["relativeid"]] = id; }
				XMLStuff.PillData->ProcessChilds(auxnode, id);
				XMLStuff.PillData->byname[pill["name"]] = id;
				XMLStuff.PillData->bynamemod[pill["name"] + lastmodid] = id;
				XMLStuff.PillData->bymod[lastmodid].push_back(id);
				XMLStuff.PillData->byfilepathmulti.tab[currpath].push_back(id);
				XMLStuff.PillData->nodes[id] = pill;
				XMLStuff.ModData->pills[lastmodid] += 1;
			}
		}
	}
	else if ((strcmp(nodename, "items") == 0)) {
		xml_node<char>* daddy = node;
		int id = 1;
		for (xml_node<char>* auxnode = node->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			string meh = stringlower(auxnode->name());
			const char* auxnodename = meh.c_str();
			if ((strcmp(auxnodename, "active") == 0) || (strcmp(auxnodename, "passive") == 0) || (strcmp(auxnodename, "familiar") == 0) || (strcmp(auxnodename, "item") == 0)) {
				XMLAttributes item;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
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
					if (item.count("id") && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
						id = toint(item["id"]);
					}
					else {
						if (item.count("id") > 0) { item["relativeid"] = item["id"]; }
						XMLStuff.ItemData->maxid += 1;
						item["id"] = to_string(XMLStuff.ItemData->maxid);
						id = XMLStuff.ItemData->maxid;
					}
					if (id > XMLStuff.ItemData->maxid) {
						XMLStuff.ItemData->maxid = id;
					}
					for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
					{
						item[stringlower(attr->name())] = attr->value();
					}

					item["sourceid"] = lastmodid;
					item["type"] = auxnodename;
					//printf("iname: %s // %s (%s) \n", item["name"].c_str(), item["description"].c_str(), item["id"].c_str());
					if (item["name"].find("#") != string::npos) {
						item["untranslatedname"] = item["name"];
						item["name"] = string(stringTable->GetString("Items", 0, item["name"].substr(1, item["name"].length()).c_str(), &unk));
					}

					if (item["description"].find("#") != string::npos) {
						item["untranslateddescription"] = item["description"];
						item["description"] = string(stringTable->GetString("Items", 0, item["description"].substr(1, item["description"].length()).c_str(), &unk));
					}
					if (item.count("relativeid") > 0) { XMLStuff.ItemData->byrelativeid[lastmodid + item["relativeid"]] = id; }
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
				if (isitemmetadata) { //items_metadata lazy bypass
					if (trinket.count("id") > 0) {
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
					if (trinket.count("id") && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
						id = toint(trinket["id"]);
					}
					else {
						if (trinket.count("id") > 0) { trinket["relativeid"] = trinket["id"]; }
						XMLStuff.TrinketData->maxid += 1;
						trinket["id"] = to_string(XMLStuff.TrinketData->maxid);
						id = XMLStuff.TrinketData->maxid;
					}
					if (id > XMLStuff.TrinketData->maxid) {
						XMLStuff.TrinketData->maxid = id;
					}
					for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
					{
						trinket[stringlower(attr->name())] = attr->value();
					}

					trinket["sourceid"] = lastmodid;
					trinket["type"] = auxnode->name();
					//printf("tname: %s // %s (%s) \n", trinket["name"].c_str(), trinket["description"].c_str(), trinket["id"].c_str());
					if (trinket["name"].find("#") != string::npos) {
						trinket["untranslatedname"] = trinket["name"];
						trinket["name"] = string(stringTable->GetString("Items", 0, trinket["name"].substr(1, trinket["name"].length()).c_str(), &unk));
					}

					if (trinket["description"].find("#") != string::npos) {
						trinket["untranslateddescription"] = trinket["description"];
						trinket["description"] = string(stringTable->GetString("Items", 0, trinket["description"].substr(1, trinket["description"].length()).c_str(), &unk));
					}
					if (trinket.count("relativeid") > 0) { XMLStuff.TrinketData->byrelativeid[lastmodid + trinket["relativeid"]] = id; }
					XMLStuff.TrinketData->ProcessChilds(auxnode, id);
					XMLStuff.TrinketData->bynamemod[trinket["name"] + lastmodid] = id;
					XMLStuff.TrinketData->bymod[lastmodid].push_back(id);
					XMLStuff.TrinketData->byfilepathmulti.tab[currpath].push_back(id);
					XMLStuff.TrinketData->byname[trinket["name"]] = id;
					XMLStuff.TrinketData->nodes[id] = trinket;
					XMLStuff.ModData->trinkets[lastmodid] += 1;
				}
			}
		}
	}
	else if ((strcmp(nodename, "bombcostumes") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes bombcostume;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				bombcostume[stringlower(attr->name())] = string(attr->value());
			}
			if ((bombcostume.count("variant") > 0)){ // && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(bombcostume["variant"]);
			}
			else {
				if (bombcostume.count("variant") > 0) { bombcostume["relativeid"] = bombcostume["variant"]; }
				XMLStuff.BombCostumeData->maxid = XMLStuff.BombCostumeData->maxid + 1;
				bombcostume["id"] = to_string(XMLStuff.BombCostumeData->maxid);
				id = XMLStuff.BombCostumeData->maxid;
			}
			if (id > XMLStuff.BombCostumeData->maxid) {
				XMLStuff.BombCostumeData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				bombcostume[attr->name()] = attr->value();
			}
			if (bombcostume.count("name") == 0) { bombcostume["name"] = bombcostume["variant"]; }
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
	}
	else if ((strcmp(nodename, "music") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes music;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				music[stringlower(attr->name())] = string(attr->value());
			}
			if ((music.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(music["id"]);
			}
			else {
				if (music.count("id") > 0) { music["relativeid"] = music["id"]; }
				XMLStuff.MusicData->maxid = XMLStuff.MusicData->maxid + 1;
				music["id"] = to_string(XMLStuff.MusicData->maxid);
				id = XMLStuff.MusicData->maxid;
			}
			if (id > XMLStuff.MusicData->maxid) {
				XMLStuff.MusicData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				music[attr->name()] = attr->value();
			}
			music["sourceid"] = lastmodid;
			XMLStuff.MusicData->ProcessChilds(auxnode, id);
			if (music.count("relativeid") > 0) { XMLStuff.MusicData->byrelativeid[lastmodid + music["relativeid"]] = id; }
			XMLStuff.MusicData->bynamemod[music["name"] + lastmodid] = id;
			XMLStuff.MusicData->bymod[lastmodid].push_back(id);
			XMLStuff.MusicData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.MusicData->byname[music["name"]] = id;
			XMLStuff.MusicData->nodes[id] = music;
			XMLStuff.ModData->musictracks[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
	}
	else if ((strcmp(nodename, "sounds") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes sound;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				sound[stringlower(attr->name())] = string(attr->value());
			}
			if ((sound.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(sound["id"]);
			}
			else {
				if (sound.count("id") > 0) { sound["relativeid"] = sound["id"]; }
				XMLStuff.SoundData->maxid = XMLStuff.SoundData->maxid + 1;
				sound["id"] = to_string(XMLStuff.SoundData->maxid);
				id = XMLStuff.SoundData->maxid;
			}
			if (id > XMLStuff.SoundData->maxid) {
				XMLStuff.SoundData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				sound[stringlower(attr->name())] = attr->value();
			}
			sound["sourceid"] = lastmodid;
			XMLStuff.SoundData->ProcessChilds(auxnode, id);
			if ((sound.count("name") == 0) && (XMLStuff.SoundData->childs[id]["sample"].size() > 0) && (XMLStuff.SoundData->childs[id]["sample"][0].count("path") != 0)) {
				sound["name"] = getFileName(XMLStuff.SoundData->childs[id]["sample"][0]["path"]);
			}
			//printf("sound: %s (%d) \n",sound["name"].c_str(),id);
			if (sound.count("relativeid") > 0) { XMLStuff.SoundData->byrelativeid[lastmodid + sound["relativeid"]] = id; }
			XMLStuff.SoundData->bynamemod[sound["name"] + lastmodid] = id;
			XMLStuff.SoundData->bymod[lastmodid].push_back(id);
			XMLStuff.SoundData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.SoundData->byname[sound["name"]] = id;
			XMLStuff.SoundData->nodes[id] = sound;
			XMLStuff.ModData->sounds[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
	}
	else if ((strcmp(nodename, "challenges") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes challenge;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				challenge[stringlower(attr->name())] = string(attr->value());
			}
			if ((challenge.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(challenge["id"]);
			}
			else {
				if (challenge.count("id") > 0) { challenge["relativeid"] = challenge["id"]; }
				XMLStuff.ChallengeData->maxid = XMLStuff.ChallengeData->maxid + 1;
				challenge["id"] = to_string(XMLStuff.ChallengeData->maxid);
				id = XMLStuff.ChallengeData->maxid;
			}
			if (id > XMLStuff.ChallengeData->maxid) {
				XMLStuff.ChallengeData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				challenge[stringlower(attr->name())] = attr->value();
			}
			challenge["sourceid"] = lastmodid;
			if (challenge.count("relativeid") > 0) { XMLStuff.ChallengeData->byrelativeid[lastmodid + challenge["relativeid"]] = id; }
			XMLStuff.ChallengeData->ProcessChilds(auxnode, id);
			XMLStuff.ChallengeData->bynamemod[challenge["name"] + lastmodid] = id;
			XMLStuff.ChallengeData->bymod[lastmodid].push_back(id);
			XMLStuff.ChallengeData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.ChallengeData->byname[challenge["name"]] = id;
			XMLStuff.ChallengeData->nodes[id] = challenge;
			XMLStuff.ModData->challenges[lastmodid] += 1;
		}
	}
	else if ((strcmp(nodename, "recipes") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
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
				//if (challenge.count("id") > 0) { challenge["relativeid"] = challenge["id"]; }
				XMLStuff.RecipeData->maxid = XMLStuff.RecipeData->maxid + 1;
				recipe["id"] = to_string(XMLStuff.RecipeData->maxid);
				id = XMLStuff.RecipeData->maxid;
			//}
			if (id > XMLStuff.RecipeData->maxid) {
				XMLStuff.RecipeData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				recipe[stringlower(attr->name())] = attr->value();
			}
			if (recipe.count("input") > 0) { recipe["name"] = recipe["input"]; };
			recipe["sourceid"] = lastmodid;
			if (recipe.count("relativeid") > 0) { XMLStuff.RecipeData->byrelativeid[lastmodid + recipe["relativeid"]] = id; }
			XMLStuff.RecipeData->ProcessChilds(auxnode, id);
			XMLStuff.RecipeData->bynamemod[recipe["name"] + lastmodid] = id;
			XMLStuff.RecipeData->bymod[lastmodid].push_back(id);
			XMLStuff.RecipeData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.RecipeData->byname[recipe["name"]] = id;
			XMLStuff.RecipeData->nodes[id] = recipe;
			XMLStuff.ModData->recipes[lastmodid] += 1;
		}
	}
	else if ((strcmp(nodename, "wisps") == 0)) {
		xml_node<char>* daddy = node;
		int id = 1;
		for (xml_node<char>* auxnode = node->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			string middleman = stringlower(auxnode->name());
			const char* nodename = middleman.c_str();
			if ((strcmp(nodename, "color") == 0)) {
				XMLAttributes color;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					color[stringlower(attr->name())] = string(attr->value());
				}
				if ((color.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					id = toint(color["id"]);
				}
				else {
					if (color.count("id") > 0) { color["relativeid"] = color["id"]; }
					XMLStuff.WispColorData->maxid = XMLStuff.WispColorData->maxid + 1;
					color["id"] = to_string(XMLStuff.WispColorData->maxid);
					id = XMLStuff.WispColorData->maxid;
				}
				if (id > XMLStuff.WispColorData->maxid) {
					XMLStuff.WispColorData->maxid = id;
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					color[stringlower(attr->name())] = attr->value();
				}
				color["sourceid"] = lastmodid;
				if (color.count("relativeid") > 0) { XMLStuff.WispColorData->byrelativeid[lastmodid + color["relativeid"]] = id; }
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
					if ((wisp.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
						id = toint(wisp["id"]);
					}
					else {
						if (wisp.count("id") > 0) { wisp["relativeid"] = wisp["id"]; }
						XMLStuff.WispData->maxid = XMLStuff.WispData->maxid + 1;
						wisp["id"] = to_string(XMLStuff.WispData->maxid);
						id = XMLStuff.WispData->maxid;
					}
					if (id > XMLStuff.WispData->maxid) {
						XMLStuff.WispData->maxid = id;
					}
					for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
					{
						wisp[stringlower(attr->name())] = attr->value();
					}
					wisp["sourceid"] = lastmodid;
					if (wisp.count("relativeid") > 0) {
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
		}
	else if ((strcmp(nodename, "locusts") == 0)) {
		xml_node<char>* daddy = node;
		int id = 1;
		for (xml_node<char>* auxnode = node->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			string middleman = stringlower(auxnode->name());
			const char* nodename = middleman.c_str();
			if ((strcmp(nodename, "color") == 0)) {
				XMLAttributes color;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					color[stringlower(attr->name())] = string(attr->value());
				}
				if ((color.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					id = toint(color["id"]);
				}
				else {
					if (color.count("id") > 0) { color["relativeid"] = color["id"]; }
					XMLStuff.LocustColorData->maxid = XMLStuff.LocustColorData->maxid + 1;
					color["id"] = to_string(XMLStuff.LocustColorData->maxid);
					id = XMLStuff.LocustColorData->maxid;
				}
				if (id > XMLStuff.LocustColorData->maxid) {
					XMLStuff.LocustColorData->maxid = id;
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					color[stringlower(attr->name())] = attr->value();
				}
				color["sourceid"] = lastmodid;
				if (color.count("relativeid") > 0) { XMLStuff.LocustColorData->byrelativeid[lastmodid + color["relativeid"]] = id; }
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
				if ((locust.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					id = toint(locust["id"]);
				}
				else {
					if (locust.count("id") > 0) { locust["relativeid"] = locust["id"]; }
					XMLStuff.LocustData->maxid = XMLStuff.LocustData->maxid + 1;
					locust["id"] = to_string(XMLStuff.LocustData->maxid);
					id = XMLStuff.LocustData->maxid;
				}
				if (id > XMLStuff.LocustData->maxid) {
					XMLStuff.LocustData->maxid = id;
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					locust[stringlower(attr->name())] = attr->value();
				}
				locust["sourceid"] = lastmodid;
				if (locust.count("relativeid") > 0) {
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
		}
	else if ((strcmp(nodename, "nightmares") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes nightmare;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				nightmare[stringlower(attr->name())] = string(attr->value());
			}
			if ((nightmare.count("id") > 0) && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
				id = toint(nightmare["id"]);
			}
			else {
				if (nightmare.count("id") > 0) { nightmare["relativeid"] = nightmare["id"]; }
				XMLStuff.NightmareData->maxid = XMLStuff.NightmareData->maxid + 1;
				nightmare["id"] = to_string(XMLStuff.NightmareData->maxid);
				id = XMLStuff.NightmareData->maxid;
			}
			if (id > XMLStuff.NightmareData->maxid) {
				XMLStuff.NightmareData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				nightmare[stringlower(attr->name())] = attr->value();
			}
			if ((nightmare.count("name") == 0) && (nightmare.count("anm2") != 0)) {
				nightmare["name"] = getFileName(nightmare["anm2"]);
			}

			nightmare["sourceid"] = lastmodid;
			if (nightmare.count("relativeid") > 0) { XMLStuff.NightmareData->byrelativeid[lastmodid + nightmare["relativeid"]] = id; }
			XMLStuff.NightmareData->ProcessChilds(auxnode, id);
			XMLStuff.NightmareData->bynamemod[nightmare["name"] + lastmodid] = id;
			XMLStuff.NightmareData->bymod[lastmodid].push_back(id);
			XMLStuff.NightmareData->byfilepathmulti.tab[currpath].push_back(id);
			XMLStuff.NightmareData->byname[nightmare["name"]] = id;
			XMLStuff.NightmareData->nodes[id] = nightmare;
			XMLStuff.ModData->nightmares[lastmodid] += 1;
		}
	}
	else if ((strcmp(nodename, "curses") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
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
	}
	else if ((strcmp(nodename, "costumes") == 0)) {
		int id = 1;
		int idnull = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
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
					XMLStuff.NullCostumeData->maxid = XMLStuff.NullCostumeData->maxid + 1;
					costume["id"] = to_string(XMLStuff.NullCostumeData->maxid);
					idnull = XMLStuff.NullCostumeData->maxid;
				}
				if (idnull > XMLStuff.NullCostumeData->maxid) {
					XMLStuff.NullCostumeData->maxid = idnull;
					costume["id"] = idnull;
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					costume[stringlower(attr->name())] = attr->value();
				}
				if ((costume.count("name") == 0) && (costume.count("anm2path") != 0)) {
					costume["name"] = getFileName(costume["anm2path"]);
				}
				costume["sourceid"] = lastmodid;
				if (costume.count("relativeid") > 0) { XMLStuff.NullCostumeData->byrelativeid[lastmodid + costume["relativeid"]] = id; }
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
	}
	else if ((strcmp(nodename, "itempools") == 0)) {
		int id = -1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
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

			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				itempool[stringlower(attr->name())] = attr->value();
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
	}
	else if ((strcmp(nodename, "name") == 0) && node->parent() && (strcmp(stringlower(node->parent()->name()).c_str(), "metadata") == 0)) {
		xml_node<char>* daddy = node->parent();
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
}

HOOK_METHOD(xmlnode_rep, first_node, (char* name, int size, bool casesensitive)->xml_node<char>*) {
	xml_node<char>* node = super(name, size, casesensitive);
	if (node != nullptr) {
		xml_attribute<char>* err =  node->first_attribute("xmlerror");
		if ((currpath.length() > 0) && (err != nullptr)){
			string error = "[XMLError] " + xmlerrors[toint(err->value())] + " in " + currpath;
			g_Game->GetConsole()->PrintError(error);
			KAGE::LogMessage(3,(error + "\n").c_str());
			printf("%s \n", error.c_str());
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

HOOK_METHOD(ItemConfig, LoadPocketItems, (char* xmlpath, int ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(ItemConfig, Load, (char* xmlpath, int ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(RoomConfig, LoadCurses, (char* xmlpath, bool ismod)->void) {
	//printf("curse you %s \n", xmlpath);
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadWispConfig, (char* xmlpath, int ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadLocustConfig, (char* xmlpath, int ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadBombConfigRules, (char* xmlpath, int ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadCraftingRecipes, (char* xmlpath, int ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadMetadata, (char* xmlpath, int ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	isitemmetadata = true;
	super(xmlpath, ismod);
	isitemmetadata = false;
	currpath = "";
}

HOOK_METHOD(Music, LoadConfig, (char* xmlpath, bool ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(SFXManager, LoadConfig, (char* xmlpath, bool ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(Manager, LoadChallenges, (char* xmlpath, char ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(NightmareScene, LoadConfig, (char* xmlpath)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath);
	currpath = "";
}

HOOK_METHOD(ItemConfig, LoadCostumes, (char* xmlpath, int ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(EntityConfig, Load, (char* xmlpath, ModEntry* mod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, mod);
	super(xmlpath, mod);
	currpath = "";
}

HOOK_METHOD(ItemPool, load_pools, (char* xmlpath, char ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
	currpath = "";
}

HOOK_METHOD(EntityConfig, LoadPlayers, (char* xmlpath, int ismod)->void) {
	currpath = string(xmlpath);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
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

int Lua_FromTypeVarSub(lua_State* L)
{
	if (!lua_isnumber(L, 1)) { return luaL_error(L, "Expected EntityType as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	int etype = (int)lua_tonumber(L, 1);
	int evar = 0;
	int esub = 0;
	bool strict = false;
	if (lua_isnumber(L, 2)) { evar = (int)lua_tonumber(L, 2); }
	if (lua_isnumber(L, 3)) { esub = (int)lua_tonumber(L, 3); }
	if (lua_isboolean(L, 4)) { strict = lua_toboolean(L, 4); }
	XMLAttributes Node = XMLStuff.EntityData->GetNodesByTypeVarSub(etype, evar, esub, strict);
	tuple idx = { toint(Node["type"]), toint(Node["variant"]), toint(Node["subtype"]) };
	Lua_PushXMLNode(L, Node, XMLStuff.EntityData->childs[idx]);
	return 1;
}

int Lua_GetFromEntity(lua_State* L)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	if (entity == NULL) { return luaL_error(L, "Expected entity as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	int etype = *entity->GetType();
	int evar = *entity->GetVariant();
	int esub = *entity->GetSubType();
	if (evar == NULL) { evar = 0; }
	if (esub == NULL) { esub = 0; }

	bool automatic = true;
	bool strict = false;
	if (lua_isboolean(L, 2)) { automatic = lua_toboolean(L, 2); }
	if (lua_isboolean(L, 3)) { strict = lua_toboolean(L, 3); }
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
	}
	Lua_PushXMLNode(L, Node,Childs);
	return 1;
}

int Lua_GetEntryByNameXML(lua_State* L)
{
	if (!lua_isnumber(L, 1)) { return luaL_error(L, "Expected XMLNode as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	if (!lua_isstring(L, 2)) { return luaL_error(L, "Expected string as parameter #2, got %s", lua_typename(L, lua_type(L, 1))); }
	int nodetype = (int)lua_tonumber(L, 1);
	string entityname = string(lua_tostring(L, 2));
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
		Childs = XMLStuff.BombCostumeData->childs[XMLStuff.RecipeData->byname[entityname]];
		break;
	}	
	Lua_PushXMLNode(L, Node,Childs);
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	ClearXMLData();
	super();
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_newtable(L); 
	lua_pushstring(L, "GetEntryByName"); 
	lua_pushcfunction(L, Lua_GetEntryByNameXML); 
	lua_settable(L, -3);
	lua_pushstring(L, "GetEntityByTypeVarSub");
	lua_pushcfunction(L, Lua_FromTypeVarSub);
	lua_settable(L, -3);
	lua_pushstring(L, "GetEntryFromEntity");
	lua_pushcfunction(L, Lua_GetFromEntity);
	lua_settable(L, -3);
	lua_setglobal(L, "XMLData"); 
	//lua_pop(L, 1);
	
}



//Crash Prevention//
int getLineNumber(const char* data, const char* errorOffset) {
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


HOOK_METHOD(xmldocument_rep, parse, (char* xmldata)-> void) {
	try {
		string a = stringlower((char*)string(xmldata).substr(0, 60).c_str());
		string xml = string(xmldata);
		if (a.find("<reci") < 50) {
			regex regexPattern(R"(\boutput\s*=\s*["']([^"']+)["'])");
			smatch match;
			auto words_begin = std::sregex_iterator(xml.begin(), xml.end(), regexPattern);
			auto words_end = std::sregex_iterator();

			for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
				string itemname = (*it)[1].str();
				//printf("itemname: %s \n", itemname.c_str());
				size_t pos = 0;
				try {
					int itemid = stoi(itemname);
				}
				catch (exception ex) {
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
		string a = stringlower((char *)string(xmldata).substr(0,60).c_str());
		xmlerrors.push_back(err.what() + string(" at line ") + to_string(lineNumber));
		if (a.find("<ent") < 50) {
			a = "<entities anm2root=\"gfx/\" version=\"5\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </entities>";
		}
		else if (a.find("<mus") < 50) {
			a = "<music root=\"music/\" xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </music>";
		}
		else if (a.find("<pock") < 50) {
			a = "<pocketitems xmlerror=\"" + to_string(xmlerrors.size() - 1) + "\"> </pocketitems>";
		}
		else if (a.find("<itempo") < 50) {
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
		//printf("asdad: %s", a.c_str());
		xmldata = new char[a.length() + 1];
		strcpy(xmldata, a.c_str());
		super(xmldata);
	}
}
//Crash Prevention//

//Sneaky modreloader code
#include <limits.h>
#include <iostream>

void GameRestart() {
	char path[1024];
	STARTUPINFO si = {};
	PROCESS_INFORMATION pi = {};
	LPSTR commandLine = GetCommandLine();
	if (CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		TerminateProcess(GetCurrentProcess(), 0);
	}
}
HOOK_METHOD(ModManager, Reset, () -> void) {
	//super();
	GameRestart();
}

//Sneaky modreloader code

#endif