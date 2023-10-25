#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PickupSetAlternatePedestal) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	int pedestalType = (int)luaL_checkinteger(L, 2);

	pickup->SetAlternatePedestal(pedestalType);

	return 0;
}

LUA_FUNCTION(Lua_PickupTryRemoveCollectible) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushboolean(L, pickup->TryRemoveCollectible());

	return 1;
}

LUA_FUNCTION(Lua_PickupSetForceBlind) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	bool setBlind = lua_toboolean(L, 2);

	pickup->SetForceBlind(setBlind);

	return 0;
}

LUA_FUNCTION(Lua_PickupIsBlind) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	bool isBlind = pickup->IsBlind();

	lua_pushboolean(L, isBlind);
	return 1;
}

LUA_FUNCTION(Lua_PickupGetVarData) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushinteger(L, *pickup->GetVarData());

	return 1;
}

LUA_FUNCTION(Lua_PickupSetVarData) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	*pickup->GetVarData() = (int)lua_tointeger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PickupSetNewOptionsPickupIndex) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushinteger(L, pickup->SetNewOptionsPickupIndex());

	return 1;
}

LUA_FUNCTION(Lua_PickupTryInitOptionCycle) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	int numCycle = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, pickup->TryInitOptionCycle(numCycle));

	return 1;
}

LUA_FUNCTION(Lua_PickupGetDropDelay)
{
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushinteger(L, *pickup->GetDropDelay());

	return 1;
}

LUA_FUNCTION(Lua_PickupSetDropDelay)
{
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	*pickup->GetDropDelay() = (int)luaL_checkinteger(L, 2);

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "IsBlind", Lua_PickupIsBlind },
		{ "SetAlternatePedestal", Lua_PickupSetAlternatePedestal },
		{ "TryRemoveCollectible", Lua_PickupTryRemoveCollectible },
		{ "SetForceBlind", Lua_PickupSetForceBlind },
		{ "GetVarData", Lua_PickupGetVarData },
		{ "SetVarData", Lua_PickupSetVarData },
		{ "SetNewOptionsPickupIndex", Lua_PickupSetNewOptionsPickupIndex },
		{ "TryInitOptionCycle", Lua_PickupTryInitOptionCycle },
		{ "GetDropDelay", Lua_PickupGetDropDelay },
		{ "SetDropDelay", Lua_PickupSetDropDelay },
		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_PICKUP, functions);
}