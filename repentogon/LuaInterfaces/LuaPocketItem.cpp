#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PlayerGetPocketItem) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slotID = (int)luaL_checkinteger(L, 2);
	PocketItem** ud = (PocketItem**)lua_newuserdata(L, sizeof(PocketItem*));
	*ud = player->GetPocketItem(slotID);
	luaL_setmetatable(L, lua::metatables::PocketItemMT);
	return 1;
}

LUA_FUNCTION(Lua_PocketItemGetSlot) {
	PocketItem* pocketItem = *lua::GetUserdata<PocketItem**>(L, 1, lua::metatables::PocketItemMT);
	lua_pushinteger(L, pocketItem->_slot);
	return 1;
}

LUA_FUNCTION(Lua_PocketItemGetType) {
	PocketItem* pocketItem = *lua::GetUserdata<PocketItem**>(L, 1, lua::metatables::PocketItemMT);
	lua_pushinteger(L, pocketItem->_type);
	return 1;
}

static void RegisterPocketItem(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY_PLAYER, "GetPocketItem", Lua_PlayerGetPocketItem);

	luaL_Reg functions[] = {
		{ "GetSlot", Lua_PocketItemGetSlot},
		{ "GetType", Lua_PocketItemGetType},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::PocketItemMT, lua::metatables::PocketItemMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	RegisterPocketItem(_state);
}

