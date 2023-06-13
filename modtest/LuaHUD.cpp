#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"


static int Lua_HUDFlashRedHearts(lua_State* L) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 2, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");

	hud->FlashRedHearts(player);
	return 0;
}

static int Lua_HUDGetPickupsHUDSprite(lua_State* L) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = hud->GetPickupsHUDSprite();
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::HUD;
	lua::RegisterFunction(state, mt, "FlashRedHearts", Lua_HUDFlashRedHearts);
	lua::RegisterFunction(state, mt, "GetPickupsHUDSprite", Lua_HUDGetPickupsHUDSprite);
}