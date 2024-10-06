#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_RoomGetLRoomAreaDesc) {
	Room * room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	LRoomAreaDesc desc;
	desc = *room->GetLRoomAreaDesc(&desc);

	LRoomAreaDesc* ud = (LRoomAreaDesc*)lua_newuserdata(L, sizeof(LRoomAreaDesc));
	luaL_setmetatable(L, lua::metatables::LRoomAreaDescMT);
	memcpy(ud, &desc, sizeof(LRoomAreaDesc));

	return 1;
}

// high
LUA_FUNCTION(Lua_LRoomAreaDesc_GetHighTopLeft) {
	LRoomAreaDesc* desc = lua::GetUserdata<LRoomAreaDesc*>(L, 1, lua::metatables::LRoomAreaDescMT);
	Vector* ud = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*ud = desc->_high[0];

	return 1;
}

LUA_FUNCTION(Lua_LRoomAreaDesc_GetHighBottomRight) {
	LRoomAreaDesc* desc = lua::GetUserdata<LRoomAreaDesc*>(L, 1, lua::metatables::LRoomAreaDescMT);
	Vector* ud = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*ud = desc->_high[1];

	return 1;
}

// low
LUA_FUNCTION(Lua_LRoomAreaDesc_GetLowTopLeft) {
	LRoomAreaDesc* desc = lua::GetUserdata<LRoomAreaDesc*>(L, 1, lua::metatables::LRoomAreaDescMT);
	Vector* ud = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*ud = desc->_low[0];

	return 1;
}

LUA_FUNCTION(Lua_LRoomAreaDesc_GetLowBottomRight) {
	LRoomAreaDesc* desc = lua::GetUserdata<LRoomAreaDesc*>(L, 1, lua::metatables::LRoomAreaDescMT);
	Vector* ud = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*ud = desc->_low[1];

	return 1;
}

static void RegisterLRoomAreaDesc(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ROOM, "GetLRoomAreaDesc", Lua_RoomGetLRoomAreaDesc);

	luaL_Reg functions[] = {
		{ "GetHighTopLeft", Lua_LRoomAreaDesc_GetHighTopLeft },
		{ "GetHighBottomRight", Lua_LRoomAreaDesc_GetHighBottomRight },
		{ "GetLowTopLeft", Lua_LRoomAreaDesc_GetLowTopLeft },
		{ "GetLowBottomRight", Lua_LRoomAreaDesc_GetLowBottomRight },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::LRoomAreaDescMT, lua::metatables::LRoomAreaDescMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterLRoomAreaDesc(_state);
}