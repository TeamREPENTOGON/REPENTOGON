#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"


LUA_FUNCTION(Lua_HUDFlashRedHearts) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 2, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");

	hud->FlashRedHearts(player);
	return 0;
}

LUA_FUNCTION(Lua_HUDGetChargeBarSprite)
{
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = hud->GetChargeBarSprite();
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetPickupsHUDSprite) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = hud->GetPickupsHUDSprite();
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "FlashRedHearts", Lua_HUDFlashRedHearts },
		{ "GetChargeBarSprite", Lua_HUDGetChargeBarSprite },
		{ "GetPickupsHUDSprite", Lua_HUDGetPickupsHUDSprite },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::HUD, functions);
}