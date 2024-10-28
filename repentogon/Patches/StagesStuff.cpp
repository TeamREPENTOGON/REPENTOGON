
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
