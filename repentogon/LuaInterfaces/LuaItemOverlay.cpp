#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*LUA_FUNCTION(Lua_GetItemOverlay)
{
	Game* game = lua::GetRawUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ItemOverlay** ud = (ItemOverlay**)lua_newuserdata(L, sizeof(ItemOverlay*));
	*ud = game->GetItemOverlay();
	luaL_setmetatable(L, lua::metatables::ItemOverlayMT);
	return 1;
}
*/

LUA_FUNCTION(Lua_ItemOverlayShow)
{
	ItemOverlay* itemOverlay = g_Game->GetItemOverlay();
	int overlayID = (int)luaL_checkinteger(L, 1);
	int delay = (int)luaL_optinteger(L, 2, 0);
	Entity_Player* player = NULL;
	if (lua_gettop(L) == 4 && !lua_isnil(L, 3)) {
		player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 3, lua::Metatables::ENTITY, "Entity_Player");
	}
	itemOverlay->Show(overlayID, delay, player);
	return 0;
}

LUA_FUNCTION(Lua_ItemOverlayGetOverlayID)
{
	ItemOverlay* itemOverlay = g_Game->GetItemOverlay();
	lua_pushinteger(L, itemOverlay->_overlayID);

	return 1;
}

LUA_FUNCTION(Lua_ItemOverlayGetSprite)
{
	ItemOverlay* itemOverlay = g_Game->GetItemOverlay();
	ANM2* anm2 = itemOverlay->GetSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_ItemOverlayGetDelay)
{
	ItemOverlay* itemOverlay = g_Game->GetItemOverlay();
	lua_pushinteger(L, itemOverlay->_delay);

	return 1;
}

LUA_FUNCTION(Lua_ItemOverlayGetPlayer) {
	ItemOverlay* itemOverlay = g_Game->GetItemOverlay();
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
	ItemOverlay* itemOverlay = g_Game->GetItemOverlay();
	ANM2* anm2 = &itemOverlay->_megaMushPlayerSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static void RegisterItemOverlay(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetItemOverlay", Lua_GetItemOverlay);
	lua_newtable(L);

	
	lua::TableAssoc(L, "GetOverlayID", Lua_ItemOverlayGetOverlayID );
		lua::TableAssoc(L, "Show", Lua_ItemOverlayShow );
		lua::TableAssoc(L, "GetSprite", Lua_ItemOverlayGetSprite );
		lua::TableAssoc(L, "GetDelay", Lua_ItemOverlayGetDelay);
		lua::TableAssoc(L, "GetPlayer", Lua_ItemOverlayGetPlayer);
			lua::TableAssoc(L, "GetMegaMushPlayerSprite", Lua_ItemOverlayGetMegaMushPlayerSprite );

			lua_setglobal(L, "ItemOverlay");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterItemOverlay(_state);
}