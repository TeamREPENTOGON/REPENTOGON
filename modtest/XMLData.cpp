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

using namespace rapidxml;
using namespace std;

char* lastmodid = "";
bool iscontent = false;

XMLData XMLStuff;

extern LogViewer logViewer;

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

string stringlower(char* str)
{
	string s = string(str);
	for (auto& c : s) {
		c = tolower(c);
	}
	return s;
}

unordered_map<string, string> BuildGenericEntry(xml_node<char>* node) {
	unordered_map<string, string> mod;
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


void ProcessXmlNode(xml_node<char>* node) {
	if (!node) { return; }
	if ((strcmp(node->name(), "entity") == 0))
	{
		for (xml_node<char>* auxnode = node; auxnode; auxnode = auxnode->next_sibling()) {
			unordered_map<string, string> entity;
			xml_attribute<>* subattr;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				entity[stringlower(attr->name())] = attr->value();
				if (strcmp(stringlower(attr->name()).c_str(), "subtype") == 0) {
					subattr = attr;
				}
			}
			int var = -1;
			int sub = -1;
			if (entity["variant"].length() > 0) { var = stoi(entity["variant"]); }
			if (entity["subtype"].length() > 0) { sub = stoi(entity["subtype"]); }
			tuple idx = { stoi(entity["id"]), var, sub };
			if (iscontent && (XMLStuff.EntityData.entities.count(idx) > 0)) {
				unordered_map<string, string> collider = XMLStuff.EntityData.entities[idx];
				unordered_map<string, string> collidermod = XMLStuff.ModData.mods[XMLStuff.ModData.modbyid[collider["sourceid"]]];
				unordered_map<string, string>  lastmod = XMLStuff.ModData.mods[XMLStuff.ModData.modbyid[lastmodid]];
				//g_Game->GetConsole()->PrintError(toIsaacString("[XML] The entity:" + entity["name"] + "(From: " + lastmodid + ") collides with " + collider["name"] + "from (" + collidermod["name"] + ")"));
				logViewer.AddLog("[REPENTOGON]", "[XML] The entity: %s(From: %s) collides with %s (from %s) \n", entity["name"].c_str(), lastmod["name"].c_str(), collider["name"].c_str(), collidermod["name"].c_str());
			}	
			entity["sourceid"] = lastmodid;
			XMLStuff.EntityData.entities[idx] = entity;
			XMLStuff.EntityData.entitybyname[entity["name"]] = idx;
			XMLStuff.EntityData.entitybytype[entity["id"]] = idx;
		}
	}
	else if ((strcmp(node->name(),"player") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node->parent();
		for (xml_node<char>* auxnode = node; auxnode; auxnode = auxnode->next_sibling()) {
			unordered_map<string, string> player;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				player[stringlower(attr->name())] = string(attr->value());
			}
			if (player.count("id") > 0) {
				id = stoi(player["id"]);
			}
			else {
				XMLStuff.PlayerData.maxid = XMLStuff.PlayerData.maxid + 1;
				player["id"] = to_string(XMLStuff.PlayerData.maxid);
				id = XMLStuff.PlayerData.maxid;
			}
			if (id > XMLStuff.PlayerData.maxid) {
				XMLStuff.PlayerData.maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				player[attr->name()] = attr->value();
			}

			if (player.count("bskinparent") > 0){
				XMLStuff.PlayerData.playerbyname[player["name"] + "-Tainted-"] = id; }
			else{ 
				XMLStuff.PlayerData.playerbyname[player["name"]] = id; }

			if (player.count("bskinparent") > 0) {
				XMLStuff.PlayerData.playerbynamemod[player["name"] + lastmodid + "-Tainted-"] = id;
			}
			else {
				XMLStuff.PlayerData.playerbynamemod[player["name"] + lastmodid] = id;
			}

			player["sourceid"] = lastmodid;

			XMLStuff.PlayerData.players[id] = player;
			XMLStuff.ModData.modplayers[lastmodid] += 1;
			}
	}
	else if ((strcmp(node->name(), "name") == 0) && node->parent() && (strcmp(node->parent()->name(), "metadata") == 0)) {
		xml_node<char>* daddy = node->parent();
		unordered_map<string, string> mod;
		string tags = "";

		for (xml_node<char>* auxnode = daddy->first_node(); auxnode; auxnode = auxnode->next_sibling()) {
			{
				if (auxnode->value() && (strcmp(node->name(), "tag") != 0)){
					mod[auxnode->name()] = auxnode->value();
				}
				else if ((strcmp(auxnode->name(), "tag") == 0) && auxnode->first_attribute("id") && auxnode->first_attribute("id")->value()) {
					tags += string(auxnode->first_attribute("id")->value()) +", ";
				}
			}
		}
		int idx;
		if (XMLStuff.ModData.modbyid.find(mod["id"]) != XMLStuff.ModData.modbyid.end()) {
			idx = XMLStuff.ModData.modbyid[mod["id"]];
		}
		else {
			XMLStuff.ModData.nomods = XMLStuff.ModData.nomods + 1;
			idx = XMLStuff.ModData.nomods;
		}
		XMLStuff.ModData.mods[idx] = mod;
		XMLStuff.ModData.modbyid[mod["id"]] = idx;
		XMLStuff.ModData.modbydirectory[mod["directory"]] = idx;
		XMLStuff.ModData.modbyname[mod["name"]] = idx;
		mod["tags"] = tags;
	}
}

HOOK_METHOD(xmlnode_rep, first_node, (char* name, int size, bool casesensitive)->xml_node<char>*) {
	xml_node<char>* node = super(name, size, casesensitive);
	ProcessXmlNode(node);
	return super(name, size, casesensitive);
}


HOOK_METHOD(Music, LoadConfig, (char* xmlpath, bool unk)->void) {
	//printf("Music: %s", xmlpath);
	super(xmlpath,  unk);
}


//d:\steam\steamapps\common\the binding of isaac rebirth/mods/musicar/content/entities2.xml
HOOK_METHOD(EntityConfig, Load, (char* xmlpath, ModEntry* mod)->void) {
	ProcessModEntry(xmlpath, mod);
	super(xmlpath, mod);
}


/*
bool no = false;
HOOK_STATIC(Manager, Update, () -> void) {
	if (!no) {
		no = true;

		printf("playerdata \n");
		for (const auto& player : XMLStuff.PlayerData.players) {
			for each (const auto& att in player.second)
			{
				printf("%s ='%s'", att.first.c_str(), att.second.c_str());
			}
			printf("\n");
		}
		printf("moddata----------------------------- \n");

		for (const auto& mod : XMLStuff.ModData.mods) {
			for each (const auto& att in mod.second)
			{
				printf("%s ='%s'", att.first.c_str(), att.second.c_str());
			}
			printf("\n");
		}

	}
	super();
}
*/
#endif