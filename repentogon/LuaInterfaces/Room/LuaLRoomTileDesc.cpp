#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_RoomGetLRoomTileDesc) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	LRoomTileDesc desc;
	desc = *room->GetLRoomTileDesc(&desc);

	LRoomTileDesc* ud = (LRoomTileDesc*)lua_newuserdata(L, sizeof(LRoomTileDesc));
	luaL_setmetatable(L, lua::metatables::LRoomTileDescMT);
	memcpy(ud, &desc, sizeof(LRoomTileDesc));

	return 1;
}

// high
LUA_FUNCTION(Lua_LRoomTileDesc_GetHighTopLeftX) {
	LRoomTileDesc* desc = lua::GetUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	lua_pushinteger(L, desc->_high[0].x);

	return 1;
}

// LRoomTileDesc is clearly superior to LRoomAreaDesc because it has twice the functions
LUA_FUNCTION(Lua_LRoomTileDesc_GetHighTopLeftY) {
	LRoomTileDesc* desc = lua::GetUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	lua_pushinteger(L, desc->_high[0].y);

	return 1;
}

LUA_FUNCTION(Lua_LRoomTileDesc_GetHighBottomRightX) {
	LRoomTileDesc* desc = lua::GetUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	lua_pushinteger(L, desc->_high[1].x);

	return 1;
}

LUA_FUNCTION(Lua_LRoomTileDesc_GetHighBottomRightY) {
	LRoomTileDesc* desc = lua::GetUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	lua_pushinteger(L, desc->_high[1].y);

	return 1;
}

// low
LUA_FUNCTION(Lua_LRoomTileDesc_GetLowTopLeftX) {
	LRoomTileDesc* desc = lua::GetUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	lua_pushinteger(L, desc->_low[0].x);

	return 1;
}

LUA_FUNCTION(Lua_LRoomTileDesc_GetLowTopLeftY) {
	LRoomTileDesc* desc = lua::GetUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	lua_pushinteger(L, desc->_low[0].y);

	return 1;
}

LUA_FUNCTION(Lua_LRoomTileDesc_GetLowBottomRightX) {
	LRoomTileDesc* desc = lua::GetUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	lua_pushinteger(L, desc->_low[1].x);

	return 1;
}

LUA_FUNCTION(Lua_LRoomTileDesc_GetLowBottomRightY) {
	LRoomTileDesc* desc = lua::GetUserdata<LRoomTileDesc*>(L, 1, lua::metatables::LRoomTileDescMT);
	lua_pushinteger(L, desc->_low[1].y);

	return 1;
}

static void RegisterLRoomTileDesc(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ROOM, "GetLRoomTileDesc", Lua_RoomGetLRoomTileDesc);

	luaL_Reg functions[] = {
		{ "GetHighTopLeftX", Lua_LRoomTileDesc_GetHighTopLeftX },
		{ "GetHighTopLeftY", Lua_LRoomTileDesc_GetHighTopLeftY },
		{ "GetHighBottomRightX", Lua_LRoomTileDesc_GetHighBottomRightX },
		{ "GetHighBottomRighty", Lua_LRoomTileDesc_GetHighBottomRightY },
		{ "GetLowTopLeftX", Lua_LRoomTileDesc_GetLowTopLeftX },
		{ "GetLowTopLeftY", Lua_LRoomTileDesc_GetLowTopLeftY },
		{ "GetLowBottomRightX", Lua_LRoomTileDesc_GetLowBottomRightX },
		{ "GetLowBottomRighty", Lua_LRoomTileDesc_GetLowBottomRightY },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::LRoomTileDescMT, lua::metatables::LRoomTileDescMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterLRoomTileDesc(_state);
}