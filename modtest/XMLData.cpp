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

using namespace rapidxml;
using namespace std;

char* lastmodid = "BaseGame";
bool iscontent = false;

XMLData XMLStuff;


void ClearXMLData() {
	XMLStuff.PlayerData.players.clear();
	XMLStuff.PlayerData.playerbymod.clear();
	XMLStuff.PlayerData.playerbyname.clear();
	XMLStuff.PlayerData.playerbynamemod.clear();

	XMLStuff.EntityData.entities.clear();
	XMLStuff.EntityData.entitybyname.clear();
	XMLStuff.EntityData.entitybynamemod.clear();
	XMLStuff.EntityData.entitybytype.clear();
	XMLStuff.EntityData.entitybytypevar.clear();
	XMLStuff.EntityData.maxid = 0;

	XMLStuff.MusicData.musicbymod.clear();
	XMLStuff.MusicData.musicbyname.clear();
	XMLStuff.MusicData.musicbynamemod.clear();
	XMLStuff.MusicData.tracks.clear();
	XMLStuff.MusicData.maxid = 118; //last music track, will probably make it automatic later, like the other stuff

	XMLStuff.ModData.mods.clear();
	XMLStuff.ModData.modbyid.clear();
	XMLStuff.ModData.modbyname.clear();
	XMLStuff.ModData.modentities.clear();
	XMLStuff.ModData.modmusictracks.clear();
	XMLStuff.ModData.modplayers.clear();
	XMLStuff.ModData.mods.clear();
	XMLStuff.ModData.nomods = 0;
}

bool no = false;
void PrintXMLStuff() {
	/*
	no = true;
	printf("music \n");
	for (const auto& player : XMLStuff.MusicData.tracks) {
		for each (const auto & att in player.second)
		{
			printf("%s ='%s'", att.first.c_str(), att.second.c_str());
		}
		printf("\n");
	}
	printf("music----------------------------- \n");
	
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
	*/
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
	Manager* manager = g_Manager;
	StringTable* stringTable = manager->GetStringTable();
	uint32_t unk;
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
			int type = stoi(entity["id"]);
			int var = 0;
			int sub = 0;
			
			
			if (entity["subtype"].length() > 0) { sub = stoi(entity["subtype"]); }
			tuple idx = { type, var, sub };
			if (entity["variant"].length() > 0) { var = stoi(entity["variant"]); idx = { type, var, sub };}
			else if((type < 10) || (type >= 1000)) {
				while (XMLStuff.EntityData.entities.count(idx) > 0) {
					var += 1;
					idx = { type, var, sub };
				} 
			}

			
			if (iscontent && (XMLStuff.EntityData.entities.count(idx) > 0)) {
				unordered_map<string, string> collider = XMLStuff.EntityData.entities[idx];
				unordered_map<string, string> collidermod = XMLStuff.ModData.mods[XMLStuff.ModData.modbyid[collider["sourceid"]]];
				unordered_map<string, string>  lastmod = XMLStuff.ModData.mods[XMLStuff.ModData.modbyid[lastmodid]];
				//g_Game->GetConsole()->PrintError(toIsaacString("[XML] The entity:" + entity["name"] + "(From: " + lastmodid + ") collides with " + collider["name"] + "from (" + collidermod["name"] + ")"));
				if (false){
					printf("[XML] The entity: %s(From: %s) collides with %s (from %s) \n", entity["name"].c_str(), lastmod["name"].c_str(), collider["name"].c_str(), collidermod["name"].c_str());
				}
				//Conflict resolve emulation begin
				if ((type < 10) || (type >= 1000)) {
					do {
						var += 1;
						idx = { type, var, sub };
					} while (XMLStuff.EntityData.entities.count(idx) > 0);
				}
				else {
					do {
						type += 1;
						idx = { type, var, sub };
					} while (XMLStuff.EntityData.entities.count(idx) > 0);
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
	else if ((strcmp(node->name(), "music") == 0)) {
		int id = 1;
		xml_node<char>* daddy = node;
		xml_node<char>* babee = node->first_node();
		for (xml_node<char>* auxnode = babee; auxnode; auxnode = auxnode->next_sibling()) {
			unordered_map<string, string> music;
			for (xml_attribute<>* attr = auxnode->first_attribute(); attr; attr = attr->next_attribute())
			{
				music[stringlower(attr->name())] = string(attr->value());
			}
			if (music.count("id") > 0) {
				id = stoi(music["id"]);
			}
			else {
				XMLStuff.MusicData.maxid = XMLStuff.MusicData.maxid + 1;
				music["id"] = to_string(XMLStuff.MusicData.maxid);
				id = XMLStuff.MusicData.maxid;
			}
			if (id > XMLStuff.MusicData.maxid) {
				XMLStuff.MusicData.maxid = id;
			}
			for (xml_attribute<>* attr = daddy->first_attribute(); attr; attr = attr->next_attribute())
			{
				music[attr->name()] = attr->value();
			}

			music["sourceid"] = lastmodid;


			XMLStuff.MusicData.musicbynamemod[music["name"] + lastmodid] = id;
			XMLStuff.MusicData.musicbymod[lastmodid] = id;
			XMLStuff.MusicData.musicbyname[music["name"]] = id;
			XMLStuff.MusicData.tracks[id] = music;
			XMLStuff.ModData.modmusictracks[lastmodid] += 1;
			//printf("music: %s id: %d // %d \n",music["name"].c_str(),id, XMLStuff.MusicData.maxid);
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
		if (mod.count("id") <= 0) { mod["id"] = mod["name"]; }
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

HOOK_METHOD(Manager, LoadConfigs,()->void) {
	//printf("yoyoyo ",this);
	super();
}

HOOK_METHOD(Music, LoadConfig, (char* xmlpath, bool ismod)->void) {
	//printf("music: %s \n", xmlpath);
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
		printf("ModID: %s", mod->GetId());
	}
	ProcessModEntry(xmlpath, mod);
	super(xmlpath, mod);
}



bool Lua_PushXMLNode(lua_State* L, XMLAttributes node)
{
	lua_newtable(L);
	for each (const auto & att in node)
	{
		printf("attr: %s / %s \n", att.first.c_str(), att.second.c_str());
		lua_pushstring(L, att.first.c_str());
		lua_pushstring(L, att.second.c_str());
		lua_settable(L, -3);
	}

	return true;
}


int Lua_GetEntityXML(lua_State* L)
{
	string entityname = string( lua_tostring(L, 1));
	XMLAttributes entity = XMLStuff.EntityData.entities[XMLStuff.EntityData.entitybyname[entityname]];
	printf("entityname %s %d", entityname.c_str(), entity.count("id"));
	Lua_PushXMLNode(L, entity);
	return 1;
}

int Lua_GetPlayerXML(lua_State* L)
{
	string entityname = string(lua_tostring(L, 1));
	XMLAttributes entity = XMLStuff.PlayerData.players[XMLStuff.PlayerData.playerbyname[entityname]];
	printf("playername %s %d", entityname.c_str(), entity.count("id"));
	Lua_PushXMLNode(L, entity);
	return 1;
}

int Lua_GetModXML(lua_State* L)
{
	string entityname = string(lua_tostring(L, 1));
	XMLAttributes entity = XMLStuff.ModData.mods[XMLStuff.ModData.modbyname[entityname]];
	printf("modname %s %d", entityname.c_str(), entity.count("id"));
	Lua_PushXMLNode(L, entity);
	return 1;
}

int Lua_GetMusicXML(lua_State* L)
{
	string entityname = string(lua_tostring(L, 1));
	XMLAttributes entity = XMLStuff.MusicData.tracks[XMLStuff.MusicData.musicbyname[entityname]];
	printf("musicname %s %d", entityname.c_str(), entity.count("id"));
	Lua_PushXMLNode(L, entity);
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	ClearXMLData();
	super();
	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_newtable(L); 
	lua_pushstring(L, "GetEntityByName"); 
	lua_pushcfunction(L, Lua_GetEntityXML); 
	lua_settable(L, -3);
	lua_pushstring(L, "GetPlayerByName");
	lua_pushcfunction(L, Lua_GetPlayerXML);
	lua_settable(L, -3);
	lua_pushstring(L, "GetMusicByName");
	lua_pushcfunction(L, Lua_GetMusicXML);
	lua_settable(L, -3);
	lua_pushstring(L, "GetModByName");
	lua_pushcfunction(L, Lua_GetModXML);
	lua_settable(L, -3);
	lua_setglobal(L, "XMLData"); 
	//lua_pop(L, 1);
	
}




#endif