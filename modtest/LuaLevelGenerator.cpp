#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

struct LuaLevelGeneratorRoom {
	LevelGenerator* context = nullptr;
	LevelGenerator_Room* room;
};

/* LUA_FUNCTION(lua_LGR_gc) {
	LuaLevelGeneratorRoom* room = lua::GetUserdata<LuaLevelGeneratorRoom*>(L, 1, lua::metatables::LevelGeneratorRoomMT);
	free(room);
} */

static void ExtractRooms(lua_State* L, bool deadEnds);
static void PushRoom(lua_State* L, int i, LevelGenerator* generator, LevelGenerator_Room& room);
static LevelGenerator* GetLevelGenerator(lua_State* L);
static LuaLevelGeneratorRoom* GetLGR(lua_State* L);

void ExtractRooms(lua_State* L, bool deadEnds) {
	LevelGenerator* generator = GetLevelGenerator(L); // lua::GetUserdata<LevelGenerator*>(L, 1, lua::metatables::LevelGeneratorMT);
	lua_newtable(L);
	int i = 1;
	std::vector<int>* target;
	if (deadEnds) {
		target = generator->GetDeadEnds();
	}
	else {
		target = generator->GetNonDeadEnds();
	}

	std::vector<LevelGenerator_Room>& rooms = *generator->GetAllRooms();
	for (int roomId : *target) {
		PushRoom(L, i, generator, rooms[roomId]);
		++i;
	}
}

void PushRoom(lua_State* L, int i, LevelGenerator* generator, LevelGenerator_Room& room) {
	lua_pushinteger(L, i);
	LuaLevelGeneratorRoom* data = (LuaLevelGeneratorRoom*)lua_newuserdatauv(L, sizeof(LuaLevelGeneratorRoom), 0);
	luaL_setmetatable(L, lua::metatables::LevelGeneratorRoomMT);
	data->context = generator;
	data->room = &room;
	lua_rawset(L, -3);
}

LevelGenerator* GetLevelGenerator(lua_State* L) {
	return lua::UserdataToData<LevelGenerator*>(luaL_checkudata(L, 1, lua::metatables::LevelGeneratorMT));
}

LuaLevelGeneratorRoom* GetLGR(lua_State* L) {
	return lua::GetUserdata<LuaLevelGeneratorRoom*>(L, 1, lua::metatables::LevelGeneratorRoomMT);
}

LUA_FUNCTION(lua_LG_GetAllRooms) {
	LevelGenerator* generator = GetLevelGenerator(L);
	lua_newtable(L);
	int i = 1;
	for (LevelGenerator_Room & room : *(generator->GetAllRooms())) {
		PushRoom(L, i, generator, room);
		++i;
	}
	return 1;
}

LUA_FUNCTION(lua_LG_GetNonDeadEnds) {
	ExtractRooms(L, false);
	return 1;
}

LUA_FUNCTION(lua_LG_GetDeadEnds) {
	ExtractRooms(L, true);
	return 1;
}

LUA_FUNCTION(lua_LGR_GenerationIndex) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushinteger(L, data->room->_generationIndex);
	return 1;
}

LUA_FUNCTION(lua_LGR_Column) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushinteger(L, data->room->_gridColIdx);
	return 1;
}

LUA_FUNCTION(lua_LGR_Line) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushinteger(L, data->room->_gridLineIdx);
	return 1;
}

LUA_FUNCTION(lua_LGR_Shape) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushinteger(L, data->room->_shape);
	return 1;
}

LUA_FUNCTION(lua_LGR_DoorMask) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_pushinteger(L, data->room->_doors);
	return 1;
}

LUA_FUNCTION(lua_LGR_Neighbors) {
	LuaLevelGeneratorRoom* data = GetLGR(L);
	lua_newtable(L);

	int i = 1;
	for (int idx : data->room->_neighbors) {
		lua_pushinteger(L, i);
		lua_pushinteger(L, idx);
		lua_rawset(L, -3);
		++i;
	}

	return 1;
}

static void RegisterLevelGenerator(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetAllRooms", lua_LG_GetAllRooms },
		{ "GetDeadEnds", lua_LG_GetDeadEnds },
		{ "GetNonDeadEnds", lua_LG_GetNonDeadEnds },
		{ NULL, NULL }
	};
	
	lua::RegisterNewClass(L, "LevelGenerator", lua::metatables::LevelGeneratorMT, functions);
}

static void RegisterLevelGeneratorRoom(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GenerationIndex", lua_LGR_GenerationIndex },
		{ "DoorMask", lua_LGR_DoorMask },
		{ "Column", lua_LGR_Column },
		{ "Line", lua_LGR_Line },
		{ "Shape", lua_LGR_Shape },
		{ "Neighbors", lua_LGR_Neighbors },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, "LevelGeneratorRoom", lua::metatables::LevelGeneratorRoomMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	RegisterLevelGenerator(this->_state);
	RegisterLevelGeneratorRoom(this->_state);
}