#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int Lua_PickupSetAlternatePedestal(lua_State* L) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	int pedestalType = luaL_checkinteger(L, 2);

	pickup->SetAlternatePedestal(pedestalType);

	return 1;
}

static void RegisterPickupSetAlternatePedestal(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY_PICKUP);
	lua_pushstring(L, "SetAlternatePedestal");
	lua_pushcfunction(L, Lua_PickupSetAlternatePedestal);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_PickupTryRemoveCollectible(lua_State* L) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	lua_pushboolean(L, pickup->TryRemoveCollectible());

	return 1;
}

static void RegisterPickupTryRemoveCollectible(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY_PICKUP);
	lua_pushstring(L, "TryRemoveCollectible");
	lua_pushcfunction(L, Lua_PickupTryRemoveCollectible);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_PickupSetForceBlind(lua_State* L) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	bool setBlind = lua_toboolean(L, 2);

	pickup->SetForceBlind(setBlind);

	return 1;
}

static void RegisterPickupSetForceBlind(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY_PICKUP);
	lua_pushstring(L, "SetForceBlind");
	lua_pushcfunction(L, Lua_PickupSetForceBlind);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_PickupIsBlind(lua_State* L) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	bool isBlind = pickup->IsBlind();

	lua_pushboolean(L, isBlind);
	return 1;
}

static void RegisterPickupIsBlind(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY_PICKUP);
	lua_pushstring(L, "IsBlind");
	lua_pushcfunction(L, Lua_PickupIsBlind);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	//lua::LuaStackProtector protector(state);
	RegisterPickupIsBlind(state);
	RegisterPickupSetAlternatePedestal(state);
	RegisterPickupTryRemoveCollectible(state);
	RegisterPickupSetForceBlind(state);
}