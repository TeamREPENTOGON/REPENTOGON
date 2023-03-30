#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int Lua_PickupSetAlternatePedestal(lua_State* L) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	int pedestalType = luaL_checkinteger(L, 2);

	pickup->SetAlternatePedestal(pedestalType);

	return 0;
}

int Lua_PickupTryRemoveCollectible(lua_State* L) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushboolean(L, pickup->TryRemoveCollectible());

	return 1;
}

int Lua_PickupSetForceBlind(lua_State* L) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	bool setBlind = lua_toboolean(L, 2);

	pickup->SetForceBlind(setBlind);

	return 0;
}

int Lua_PickupIsBlind(lua_State* L) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	bool isBlind = pickup->IsBlind();

	lua_pushboolean(L, isBlind);
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ENTITY_PICKUP;
	lua::RegisterFunction(state, mt, "IsBlind", Lua_PickupIsBlind);
	lua::RegisterFunction(state, mt, "SetAlternatePedestal", Lua_PickupSetAlternatePedestal);
	lua::RegisterFunction(state, mt, "TryRemoveCollectible", Lua_PickupTryRemoveCollectible);
	lua::RegisterFunction(state, mt, "SetForceBlind", Lua_PickupSetForceBlind);
}