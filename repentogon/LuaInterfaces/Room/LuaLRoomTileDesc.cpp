#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

void Lua_XYToInt2(lua_State* L, XY* xy) {
	lua_newtable(L);
	lua_pushinteger(L, 1);
	lua_pushinteger(L, xy->x);
	lua_rawset(L, -3);
	lua_pushinteger(L, 2);
	lua_pushinteger(L, xy->y);
	lua_rawset(L, -3);
}

LUA_FUNCTION(Lua_RoomGetLRoomTileDesc) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	LRoomTileDesc desc;
	desc = *room->GetLRoomTileDesc(&desc);

	LRoomTileDesc* ud = (LRoomTileDesc*)lua_newuserdata(L, sizeof(LRoomTileDesc));
	luaL_setmetatable(L, lua::metatables::LRoomTileDescMT);
	memcpy(ud, &desc, sizeof(LRoomTileDesc));

	return 1;
}

LUA_FUNCTION(Lua_LRoomTileDesc_GetRandomTile) {
	LRoomTileDesc* desc = lua::GetRawUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	unsigned int seed = (unsigned int)luaL_checkinteger(L, 2);
	if (seed == 0)
		return luaL_argerror(L, 2, "Seed must be non-zero!");

	XY ret;
	desc->GetRandomTile(&ret, seed);
	Lua_XYToInt2(L, &ret);

	return 1;
}

// high
LUA_FUNCTION(Lua_LRoomTileDesc_GetHighTopLeft) {
	LRoomTileDesc* desc = lua::GetRawUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	Lua_XYToInt2(L, &desc->_high[0]);

	return 1;
}

LUA_FUNCTION(Lua_LRoomTileDesc_GetHighBottomRight) {
	LRoomTileDesc* desc = lua::GetRawUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	Lua_XYToInt2(L, &desc->_high[1]);

	return 1;
}

// low
LUA_FUNCTION(Lua_LRoomTileDesc_GetLowTopLeft) {
	LRoomTileDesc* desc = lua::GetRawUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	Lua_XYToInt2(L, &desc->_low[0]);

	return 1;
}

LUA_FUNCTION(Lua_LRoomTileDesc_GetLowBottomRight) {
	LRoomTileDesc* desc = lua::GetRawUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	Lua_XYToInt2(L, &desc->_low[1]);

	return 1;
}

static void RegisterLRoomTileDesc(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ROOM, "GetLRoomTileDesc", Lua_RoomGetLRoomTileDesc);

	luaL_Reg functions[] = {
		{ "GetRandomTile", Lua_LRoomTileDesc_GetRandomTile },
		{ "GetHighTopLeft", Lua_LRoomTileDesc_GetHighTopLeft },
		{ "GetHighBottomRight", Lua_LRoomTileDesc_GetHighBottomRight },
		{ "GetLowTopLeft", Lua_LRoomTileDesc_GetLowTopLeft },
		{ "GetLowBottomRight", Lua_LRoomTileDesc_GetLowBottomRight },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::LRoomTileDescMT, lua::metatables::LRoomTileDescMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterLRoomTileDesc(_state);
}