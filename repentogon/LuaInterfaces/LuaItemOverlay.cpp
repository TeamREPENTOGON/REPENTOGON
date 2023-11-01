#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetItemOverlay)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ItemOverlay** ud = (ItemOverlay**)lua_newuserdata(L, sizeof(ItemOverlay*));
	*ud = game->GetItemOverlay();
	luaL_setmetatable(L, lua::metatables::ItemOverlayMT);
	return 1;
}

LUA_FUNCTION(Lua_ItemOverlayShow)
{
	ItemOverlay* itemOverlay = *lua::GetUserdata<ItemOverlay**>(L, 1, lua::metatables::ItemOverlayMT);
	int overlayID = (int)luaL_checkinteger(L, 2);
	int delay = (int)luaL_optinteger(L, 3, 0);
	Entity_Player* player = NULL;
	if (lua_gettop(L) == 4 && !lua_isnil(L, 4)) {
		player = lua::GetUserdata<Entity_Player*>(L, 4, lua::Metatables::ENTITY, "Entity_Player");
	}
	itemOverlay->Show(overlayID, delay, player);
	return 0;
}

LUA_FUNCTION(Lua_ItemOverlayGetOverlayID)
{
	ItemOverlay* itemOverlay = *lua::GetUserdata<ItemOverlay**>(L, 1, lua::metatables::ItemOverlayMT);
	lua_pushinteger(L, itemOverlay->_overlayID);

	return 1;
}

LUA_FUNCTION(Lua_ItemOverlayGetSprite)
{
	ItemOverlay* itemOverlay = *lua::GetUserdata<ItemOverlay**>(L, 1, lua::metatables::ItemOverlayMT);
	ANM2* anm2 = itemOverlay->GetSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_ItemOverlayGetDelay)
{
	ItemOverlay* itemOverlay = *lua::GetUserdata<ItemOverlay**>(L, 1, lua::metatables::ItemOverlayMT);
	lua_pushinteger(L, itemOverlay->_delay);

	return 1;
}

LUA_FUNCTION(Lua_ItemOverlayGetPlayer) {
	ItemOverlay* itemOverlay = *lua::GetUserdata<ItemOverlay**>(L, 1, lua::metatables::ItemOverlayMT);
	Entity_Player* player = itemOverlay->_player;
	if (!player) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	return 1;
}

LUA_FUNCTION(Lua_ItemOverlayGetMegaMushPlayerSprite)
{
	ItemOverlay* itemOverlay = *lua::GetUserdata<ItemOverlay**>(L, 1, lua::metatables::ItemOverlayMT);
	ANM2* anm2 = &itemOverlay->_megaMushPlayerSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static void RegisterItemOverlay(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::GAME, "GetItemOverlay", Lua_GetItemOverlay);

	luaL_Reg functions[] = {
		{ "GetOverlayID", Lua_ItemOverlayGetOverlayID },
		{ "Show", Lua_ItemOverlayShow },
		{ "GetSprite", Lua_ItemOverlayGetSprite },
		//{ "GetDelay", Lua_ItemOverlayGetDelay },
		//{ "GetPlayer", Lua_ItemOverlayGetPlayer },
		{ "GetMegaMushPlayerSprite", Lua_ItemOverlayGetMegaMushPlayerSprite },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::ItemOverlayMT, lua::metatables::ItemOverlayMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterItemOverlay(_state);
}