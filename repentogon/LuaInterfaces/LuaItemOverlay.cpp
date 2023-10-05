#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_GetItemOverlay(lua_State* L) 
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ItemOverlay** ud = (ItemOverlay**)lua_newuserdata(L, sizeof(ItemOverlay*));
	*ud = game->GetItemOverlay();
	luaL_setmetatable(L, lua::metatables::ItemOverlayMT);
	return 1;
}

int Lua_ItemOverlayShow(lua_State* L)
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

int Lua_ItemOverlayGetOverlayID(lua_State* L)
{
	ItemOverlay* itemOverlay = *lua::GetUserdata<ItemOverlay**>(L, 1, lua::metatables::ItemOverlayMT);
	lua_pushinteger(L, itemOverlay->GetOverlayID());

	return 1;
}

int Lua_ItemOverlayGetSprite(lua_State* L)
{
	ItemOverlay* itemOverlay = *lua::GetUserdata<ItemOverlay**>(L, 1, lua::metatables::ItemOverlayMT);
	ANM2* anm2 = itemOverlay->GetSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

int Lua_ItemOverlayGetDelay(lua_State* L)
{
	ItemOverlay* itemOverlay = *lua::GetUserdata<ItemOverlay**>(L, 1, lua::metatables::ItemOverlayMT);
	lua_pushinteger(L, *itemOverlay->GetDelay());

	return 1;
}

static int Lua_ItemOverlayGetPlayer(lua_State* L) {
	ItemOverlay* itemOverlay = *lua::GetUserdata<ItemOverlay**>(L, 1, lua::metatables::ItemOverlayMT);
	Entity_Player* player = itemOverlay->GetPlayer();
	if (!player) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	return 1;
}

static void RegisterItemOverlay(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetItemOverlay");
	lua_pushcfunction(L, Lua_GetItemOverlay);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, lua::metatables::ItemOverlayMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetOverlayID", Lua_ItemOverlayGetOverlayID },
		{ "Show", Lua_ItemOverlayShow },
		{ "GetSprite", Lua_ItemOverlayGetSprite },
		//{ "GetDelay", Lua_ItemOverlayGetDelay },
		//{ "GetPlayer", Lua_ItemOverlayGetPlayer },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterItemOverlay(state);
}