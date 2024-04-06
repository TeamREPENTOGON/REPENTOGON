
#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <memory>

#include <cstring>

#include "XMLData.h"
#include "StagesStuff.h"

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
//Stages XML Hijack

int bckmstage = -1;
int bckmmusic = -1;

int bckbstage = -1;
int bckbbackdrop = -1;

int bcknstage = -1;
string bcknname;
HOOK_METHOD(Level, Init, ()-> void) {
	if (bckmmusic > -1) {
		g_Game->_roomConfig._stages[bckmstage]._musicId = bckmmusic;
		bckmstage = -1;
		bckmmusic = -1;
	}
	if (bckbbackdrop > -1) {
		g_Game->_roomConfig._stages[bckbstage]._backdrop = bckbbackdrop;
		bckbstage = -1;
		bckbbackdrop = -1;
	}
	if (bcknstage > -1) {
		g_Game->_roomConfig._stages[bcknstage]._displayName = bcknname;
		bcknstage = -1;
	}
	super();
}

void SetCurrentFloorMusic(int etype) {
	int currstageid = RoomConfig::GetStageID(g_Game->_stage, g_Game->_stageType, -1);
	if (bckmmusic == -1) {
		bckmstage = currstageid;
		bckmmusic = g_Game->_roomConfig._stages[currstageid]._musicId;
	}
	if (g_Manager->_musicmanager._currentId == g_Game->_roomConfig._stages[currstageid]._musicId) {
		g_Manager->_musicmanager.Play(etype, 0.1f);
		g_Manager->_musicmanager.UpdateVolume();
	}
	g_Game->_roomConfig._stages[currstageid]._musicId = etype;
}

void SetCurrentFloorBackdrop(int etype) {
	int currstageid = RoomConfig::GetStageID(g_Game->_stage, g_Game->_stageType, -1);
	if (bckbbackdrop == -1) {
		bckbstage = currstageid;
		bckbbackdrop = g_Game->_roomConfig._stages[currstageid]._backdrop;
	}
	Backdrop* bck = g_Game->_room->GetBackdrop();
	if (bck->backdropId == g_Game->_roomConfig._stages[currstageid]._backdrop) {
		bck->Init(etype, true);
	}
	g_Game->_roomConfig._stages[currstageid]._backdrop = etype;
}

void SetCurrentFloorName(string etype) {
	int currstageid = RoomConfig::GetStageID(g_Game->_stage, g_Game->_stageType, -1);
	if (bcknstage == -1) {
		bcknstage = currstageid;
		bcknname = g_Game->_roomConfig._stages[currstageid]._displayName;
	}
	g_Game->_roomConfig._stages[currstageid]._displayName = etype;
}


bool IsOnSecondFloor() {
	int stageid = g_Game->_stage;
	return (stageid == 2) || (stageid == 4) || (stageid == 6) || (stageid == 8) || (g_Game->_curses & (1 << 1)); //has curse XL
}

extern int toint(const string& str);

tuple<int, int> GetSetStage(int stageid, bool secondfloor) {
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
int lastparentstage = 0;
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


tuple<int, int> ConsoleStageIdToTuple(const string& input) {
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

extern std::vector<std::string> ParseCommand2(const std::string& command, int size);


HOOK_METHOD(Console, RunCommand, (std_string& in, std_string* out, Entity_Player* player)-> void) {
	if (in.rfind("stage ", 0) == 0) {
		std::vector<std::string> cmdlets = ParseCommand2(in, 2);
		if (cmdlets.size() > 1) {
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
	}
	super(in, out, player);
}

LUA_FUNCTION(Lua_SetCurrentFloorMusic)
{
	if (!lua_isnumber(L, 1)) { return luaL_error(L, "Expected MusicId as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	int etype = (int)luaL_checknumber(L, 1);
	SetCurrentFloorMusic(etype);
	return 0;

}

LUA_FUNCTION(Lua_SetCurrentFloorBackdrop)
{
	if (!lua_isnumber(L, 1)) { return luaL_error(L, "Expected BackdropId as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	int etype = (int)luaL_checknumber(L, 1);
	SetCurrentFloorBackdrop(etype);
	return 0;

}

LUA_FUNCTION(Lua_SetCurrentFloorName)
{
	if (!lua_isstring(L, 1)) { return luaL_error(L, "Expected floorname as parameter #1, got %s", lua_typename(L, lua_type(L, 1))); }
	string etype = luaL_checkstring(L, 1);
	SetCurrentFloorName(etype);
	return 0;

}

LUA_FUNCTION(Lua_GetCurrentStageConfigId)
{
	lua_pushinteger(L, RoomConfig::GetStageID(g_Game->_stage, g_Game->_stageType, -1));
	return 1;

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "SetCurrentFloorMusic", Lua_SetCurrentFloorMusic);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "SetCurrentFloorBackdrop", Lua_SetCurrentFloorBackdrop);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "SetCurrentFloorName", Lua_SetCurrentFloorName);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetCurrentStageConfigId", Lua_GetCurrentStageConfigId);
}
