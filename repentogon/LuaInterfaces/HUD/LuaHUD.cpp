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
	ANM2* sprite = &hud->_chargeBarSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetPickupsHUDSprite) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_pickupHUDSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetHeartsSprite) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_heartsSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetCardsPillsSprite) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_cardsPillsSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetStreakSprite) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_streakSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetFortuneSprite) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_fortuneSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetCoopMenuSprite) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_coopMenuSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetInventorySprite) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_inventorySprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetCraftingSprite) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_craftingTableSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetPoopSpellSprite) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_poopSpellsSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetIsVisible) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	lua_pushboolean(L, hud->_isVisible);

	return 1;
}

LUA_FUNCTION(Lua_HUDSetIsVisible) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	hud->_isVisible = lua::luaL_checkboolean(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_HUDGetBossHPBarFill) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	lua_pushnumber(L, hud->_bossHPBarFill);

	return 1;
}
LUA_FUNCTION(Lua_HUDSetBossHPBarFill) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	hud->_bossHPBarFill = (float)luaL_checknumber(L, 2);

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "FlashRedHearts", Lua_HUDFlashRedHearts },
		{ "GetChargeBarSprite", Lua_HUDGetChargeBarSprite },
		{ "GetHeartsSprite", Lua_HUDGetHeartsSprite },
		{ "GetPickupsHUDSprite", Lua_HUDGetPickupsHUDSprite },
		{ "GetCardsPillsSprite", Lua_HUDGetCardsPillsSprite },
		{ "GetStreakSprite", Lua_HUDGetStreakSprite },
		{ "GetFortuneSprite", Lua_HUDGetFortuneSprite },
		{ "GetCoopMenuSprite", Lua_HUDGetCoopMenuSprite },
		{ "GetInventorySprite", Lua_HUDGetInventorySprite },
		{ "GetCraftingSprite", Lua_HUDGetCraftingSprite },
		{ "GetPoopSpellSprite", Lua_HUDGetPoopSpellSprite },
		{ "IsVisible", Lua_HUDGetIsVisible },
		{ "SetIsVisible", Lua_HUDSetIsVisible },
		{ "GetBossHPBarFill", Lua_HUDGetBossHPBarFill },
		{ "SetBossHPBarFill", Lua_HUDSetBossHPBarFill },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::HUD, functions);
}