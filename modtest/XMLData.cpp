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

XMLData XMLStuff;


void ClearXMLData() {
	XMLStuff.PlayerData->nodes.clear();
	XMLStuff.PlayerData->bymod.clear();
	XMLStuff.PlayerData->byname.clear();
	XMLStuff.PlayerData->bynamemod.clear();

	XMLStuff.EntityData->nodes.clear();
	XMLStuff.EntityData->byname.clear();
	XMLStuff.EntityData->bynamemod.clear();
	XMLStuff.EntityData->bytype.clear();
	XMLStuff.EntityData->bytypevar.clear();
	XMLStuff.EntityData->maxid = 0;

	XMLStuff.CardData->nodes.clear();
	XMLStuff.CardData->byname.clear();
	XMLStuff.CardData->bynamemod.clear();
	XMLStuff.CardData->bypickup.clear();
	XMLStuff.CardData->maxid = 0;

	XMLStuff.MusicData->nodes.clear();
	XMLStuff.MusicData->byname.clear();
	XMLStuff.MusicData->bymod.clear();
	XMLStuff.MusicData->bynamemod.clear();
	XMLStuff.MusicData->maxid = 118; //last music track, will probably make it automatic later, like the other stuff

	XMLStuff.ModData->nodes.clear();
	XMLStuff.ModData->modentries.clear();
	XMLStuff.ModData->byid.clear();
	XMLStuff.ModData->byname.clear();
	XMLStuff.ModData->byfolder.clear();
	XMLStuff.ModData->bydirectory.clear();
	XMLStuff.ModData->byfullpath.clear();
	XMLStuff.ModData->entities.clear();
	XMLStuff.ModData->musictracks.clear();
	XMLStuff.ModData->cards.clear();
	XMLStuff.ModData->pills.clear();
	XMLStuff.ModData->players.clear();
	XMLStuff.ModData->items.clear();
	XMLStuff.ModData->maxid = 0;
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
	Manager* manager = g_Manager;
	StringTable* stringTable = manager->GetStringTable();
	uint32_t unk;
	if ((strcmp(node->name(), "entity") == 0))
	{
		for (xml_node<char>* auxnode = node; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes entity;
			xml_attribute<>* subattr;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				entity[stringlower(attr->name())] = attr->value();
				if (strcmp(stringlower(attr->name()).c_str(), "subtype") == 0) {
					subattr = attr;
				}
			}
			int type = stoi(entity["id"]);
			int var = 0;
			int sub = 0;
			
			
			if (entity["subtype"].length() > 0) { sub = stoi(entity["subtype"]); }
			tuple idx = { type, var, sub };
			if (entity["variant"].length() > 0) { var = stoi(entity["variant"]); idx = { type, var, sub };}
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
	else if ((strcmp(node->name(),"player") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node->parent();
		for (xml_node<char>* auxnode = node; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes player;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				player[stringlower(attr->name())] = string(attr->value());
			}
			if (player.count("id") > 0) {
				id = stoi(player["id"]);
			}
			else {
				XMLStuff.PlayerData->maxid += 1;
				player["id"] = to_string(XMLStuff.PlayerData->maxid);
				id = XMLStuff.PlayerData->maxid;
			}
			if (id > XMLStuff.PlayerData->maxid) {
				XMLStuff.PlayerData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				player[attr->name()] = attr->value();
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
			XMLStuff.PlayerData->ProcessChilds(auxnode, id);
			XMLStuff.PlayerData->nodes[id] = player;
			XMLStuff.ModData->players[lastmodid] += 1;
			}
	}
	else if ((strcmp(node->name(), "pocketitems") == 0)) {
		int id = 1;
		for (xml_node<char>* auxnode = node->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			if ((strcmp(auxnode->name(), "card") == 0) || (strcmp(auxnode->name(), "rune") == 0)) {
				XMLAttributes card;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					card[stringlower(attr->name())] = string(attr->value());
				}
				if (card.count("id") && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) { 
					id = stoi(card["id"]);
				}
				else {
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
				XMLStuff.CardData->ProcessChilds(auxnode, id);
				XMLStuff.CardData->nodes[id] = card;
				XMLStuff.CardData->bynamemod[card["name"] + lastmodid] = id;
				XMLStuff.CardData->bymod[lastmodid] = id;
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
					id = stoi(pill["id"]);
				}
				else {
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
				XMLStuff.PillData->ProcessChilds(auxnode, id);
				XMLStuff.PillData->byname[pill["name"]] = id;
				XMLStuff.PillData->bynamemod[pill["name"] + lastmodid] = id;
				XMLStuff.PillData->bymod[lastmodid] = id;
				XMLStuff.PillData->nodes[id] = pill;
				XMLStuff.ModData->pills[lastmodid] += 1;
			}
		}
	}
	else if ((strcmp(node->name(), "items") == 0)) {
		xml_node<char>* daddy = node;
		int id = 1;
		for (xml_node<char>* auxnode = node->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			if ((strcmp(auxnode->name(), "active") == 0) || (strcmp(auxnode->name(), "passive") == 0) || (strcmp(auxnode->name(), "familiar") == 0)) {
				XMLAttributes item;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					item[stringlower(attr->name())] = string(attr->value());
				}
				if ((item["name"].length() == 0) && (strcmp(lastmodid, "BaseGame") == 0)) { return; } //items_metadata lazy bypass
				if (item.count("id") && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					id = stoi(item["id"]);
				}
				else {
					XMLStuff.ItemData->maxid += 1;
					item["id"] = to_string(XMLStuff.ItemData->maxid);
					id = XMLStuff.ItemData->maxid;
				}
				if (id > XMLStuff.ItemData->maxid) {
					XMLStuff.ItemData->maxid = id;
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					item[attr->name()] = attr->value();
				}

				item["sourceid"] = lastmodid;
				item["type"] = auxnode->name();
				//printf("iname: %s // %s (%s) \n", item["name"].c_str(), item["description"].c_str(), item["id"].c_str());
				if (item["name"].find("#") != string::npos) {
					item["untranslatedname"] = item["name"];
					item["name"] = string(stringTable->GetString("Items", 0, item["name"].substr(1, item["name"].length()).c_str(), &unk));
				}

				if (item["description"].find("#") != string::npos) {
					item["untranslateddescription"] = item["description"];
					item["description"] = string(stringTable->GetString("Items", 0, item["description"].substr(1, item["description"].length()).c_str(), &unk));
				}
				XMLStuff.ItemData->ProcessChilds(auxnode, id);
				XMLStuff.ItemData->bynamemod[item["name"] + lastmodid] = id;
				XMLStuff.ItemData->bymod[lastmodid] = id;
				XMLStuff.ItemData->byname[item["name"]] = id;
				XMLStuff.ItemData->nodes[id] = item;
				XMLStuff.ModData->items[lastmodid] += 1;
			}
			else if ((strcmp(auxnode->name(), "trinket") == 0)) {
				XMLAttributes trinket;
				for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
				{
					trinket[stringlower(attr->name())] = string(attr->value());
				}
				if ((trinket["name"].length() == 0) && (strcmp(lastmodid, "BaseGame") == 0)) { return; } //items_metadata lazy bypass
				if (trinket.count("id") && ((strcmp(lastmodid, "BaseGame") == 0) || !iscontent)) {
					id = stoi(trinket["id"]);
				}
				else {
					XMLStuff.TrinketData->maxid += 1;
					trinket["id"] = to_string(XMLStuff.TrinketData->maxid);
					id = XMLStuff.TrinketData->maxid;
				}
				if (id > XMLStuff.TrinketData->maxid) {
					XMLStuff.TrinketData->maxid = id;
				}
				for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
				{
					trinket[attr->name()] = attr->value();
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
				XMLStuff.TrinketData->ProcessChilds(auxnode, id);
				XMLStuff.TrinketData->bynamemod[trinket["name"] + lastmodid] = id;
				XMLStuff.TrinketData->bymod[lastmodid] = id;
				XMLStuff.TrinketData->byname[trinket["name"]] = id;
				XMLStuff.TrinketData->nodes[id] = trinket;
				XMLStuff.ModData->trinkets[lastmodid] += 1;
			}
		}
	}
	else if ((strcmp(node->name(), "music") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes music;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				music[stringlower(attr->name())] = string(attr->value());
			}
			if (music.count("id") > 0) {
				id = stoi(music["id"]);
			}
			else {
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

			XMLStuff.MusicData->bynamemod[music["name"] + lastmodid] = id;
			XMLStuff.MusicData->bymod[lastmodid] = id;
			XMLStuff.MusicData->byname[music["name"]] = id;
			XMLStuff.MusicData->nodes[id] = music;
			XMLStuff.ModData->musictracks[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
	}
	else if ((strcmp(node->name(), "sounds") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			XMLAttributes sound;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				sound[stringlower(attr->name())] = string(attr->value());
			}
			if (sound.count("id") > 0) {
				id = stoi(sound["id"]);
			}
			else {
				XMLStuff.SoundData->maxid = XMLStuff.SoundData->maxid + 1;
				sound["id"] = to_string(XMLStuff.SoundData->maxid);
				id = XMLStuff.SoundData->maxid;
			}
			if (id > XMLStuff.SoundData->maxid) {
				XMLStuff.SoundData->maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				sound[attr->name()] = attr->value();
			}
			sound["sourceid"] = lastmodid;
			XMLStuff.SoundData->ProcessChilds(auxnode, id);
			if ((sound.count("name") == 0) && (XMLStuff.SoundData->childs[id]["sample"].size() > 0) && (XMLStuff.SoundData->childs[id]["sample"][0].count("path") != 0)) {
				sound["name"] = getFileName(XMLStuff.SoundData->childs[id]["sample"][0]["path"]);
			}
			//printf("sound: %s (%d) \n",sound["name"].c_str(),id);
			XMLStuff.SoundData->bynamemod[sound["name"] + lastmodid] = id;
			XMLStuff.SoundData->bymod[lastmodid] = id;
			XMLStuff.SoundData->byname[sound["name"]] = id;
			XMLStuff.SoundData->nodes[id] = sound;
			XMLStuff.ModData->sounds[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
		}
	}
	else if ((strcmp(node->name(), "name") == 0) && node->parent() && (strcmp(node->parent()->name(), "metadata") == 0)) {
		xml_node<char>* daddy = node->parent();
		XMLAttributes mod;
		string tags = "";

		for (xml_node<char>* auxnode = daddy->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			{
				if (auxnode->value() && (strcmp(auxnode->name(), "tag") != 0)){
					mod[auxnode->name()] = auxnode->value();
				}
				else if ((strcmp(auxnode->name(), "tag") == 0) && auxnode->first_attribute("id")) {
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
	ProcessXmlNode(node);
	return super(name, size, casesensitive);
}

HOOK_METHOD(Manager, LoadConfigs,()->void) {
	//printf("yoyoyo ",this);
	super();
	UpdateXMLModEntryData(); //resources are already loaded by this point only mod content remains
}

HOOK_METHOD(ItemConfig, LoadPocketItems, (char* xmlpath, int ismod)->void) {
	//printf("pocket: %s %d \n", xmlpath,ismod);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
}

HOOK_METHOD(ItemConfig, Load, (char* xmlpath, int ismod)->void) {
	//printf("item: %s %d \n", xmlpath,ismod);
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
}

HOOK_METHOD(Music, LoadConfig, (char* xmlpath, bool ismod)->void) {
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
}

HOOK_METHOD(SFXManager, LoadConfig, (char* xmlpath, bool ismod)->void) {
	ProcessModEntry(xmlpath, GetModEntryByContentPath(stringlower(xmlpath)));
	super(xmlpath, ismod);
}
/*
HOOK_METHOD(ModEntry, GetContentPath, (char** param_1, IsaacString* param_2)->void) {
	printf("music: %s // %s \n", &param_1, param_2->text);
	super(param_1, param_2);
	printf("music: %s // %s \n", &param_1, param_2->text);
}
*/

//d:\steam\steamapps\common\the binding of isaac rebirth/mods/musicar/content/entities2.xml
HOOK_METHOD(EntityConfig, Load, (char* xmlpath, ModEntry* mod)->void) {
	if(mod != NULL){
		//printf("ModID: %s", mod->GetId());
	}
	ProcessModEntry(xmlpath, mod);
	super(xmlpath, mod);
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
		printf("---childattr: %s / %s \n", att.first.c_str(), att.first.c_str());
		lua_pushstring(L, att.first.c_str());
		Lua_PushXMLSubNodes(L, childs[att.first]);
		lua_settable(L, -3);
	}

	return true;
}

int Lua_FromTypeVarSub(lua_State* L)
{
	if (!lua_isnumber(L, 1)) { return luaL_error(L, "Expected EntityType as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	int etype = lua_tonumber(L, 1);
	int evar = 0;
	int esub = 0;
	bool strict = false;
	if (lua_isnumber(L, 2)) { evar = lua_tonumber(L, 2); }
	if (lua_isnumber(L, 3)) { esub = lua_tonumber(L, 3); }
	if (lua_isboolean(L, 4)) { strict = lua_toboolean(L, 4); }
	XMLAttributes Node = XMLStuff.EntityData->GetNodesByTypeVarSub(etype, evar, esub, strict);
	tuple idx = { stoi(Node["type"]), stoi(Node["variant"]), stoi(Node["subtype"]) };
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
				Childs = XMLStuff.EntityData->childs[{ stoi(Node["type"]), stoi(Node["variant"]), stoi(Node["subtype"]) }];
			}
			break;
		default:
			Node = XMLStuff.EntityData->GetNodesByTypeVarSub(etype, evar, esub, strict); 
			Childs = XMLStuff.EntityData->childs[{ stoi(Node["type"]), stoi(Node["variant"]), stoi(Node["subtype"]) }];
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
	int nodetype = lua_tonumber(L, 1);
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




#endif