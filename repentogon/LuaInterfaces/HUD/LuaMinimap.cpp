#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*LUA_FUNCTION(Lua_GameGetMinimap)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Minimap** toLua = (Minimap**)lua_newuserdata(L, sizeof(Minimap*));
	*toLua = game->GetMinimap();
	luaL_setmetatable(L, lua::metatables::MinimapMT);
	return 1;
}
*/

LUA_FUNCTION(Lua_MinimapGetState)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua_pushinteger(L, minimap->_state);
	return 1;
}

LUA_FUNCTION(Lua_MinimapGetHoldTime)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua_pushinteger(L, minimap->_holdTime);
	return 1;
}

LUA_FUNCTION(Lua_MinimapGetDisplayedSize)
{
	Minimap* minimap = g_Game->GetMinimap();
	Vector buffer;
	buffer = *minimap->GetDisplayedSize(buffer);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), buffer);

	return 1;
}

LUA_FUNCTION(Lua_MinimapGetItemIconsSprite)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua::luabridge::UserdataPtr::push(L, &minimap->_itemIconsSprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_MinimapGetIconsSprite)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua::luabridge::UserdataPtr::push(L, &minimap->_iconsSprite, lua::GetMetatableKey(lua::Metatables::SPRITE));;
	return 1;
}

LUA_FUNCTION(Lua_MinimapGetShakeDuration)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua_pushinteger(L, minimap->_shakeDuration);
	return 1;
}

LUA_FUNCTION(Lua_MinimapSetShakeDuration)
{
	Minimap* minimap = g_Game->GetMinimap();
	minimap->_shakeDuration = (int)luaL_checkinteger(L, 1);
	return 0;
}

LUA_FUNCTION(Lua_MinimapGetShakeOffset)
{
	Minimap* minimap = g_Game->GetMinimap();
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), minimap->_shakeOffset);
	return 1;
}

LUA_FUNCTION(Lua_MinimapSetShakeOffset)
{
	Minimap* minimap = g_Game->GetMinimap();
	minimap->_shakeOffset = *lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	return 0;
}

static void RegisterMinimap(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetMinimap", Lua_GameGetMinimap);
	lua_newtable(L);
	//luaL_Reg functions[] = {
	lua::TableAssoc(L, "GetState", Lua_MinimapGetState);
	lua::TableAssoc(L, "GetHoldTime", Lua_MinimapGetHoldTime);
	lua::TableAssoc(L, "GetDisplayedSize", Lua_MinimapGetDisplayedSize);
	lua::TableAssoc(L, "GetItemIconsSprite", Lua_MinimapGetItemIconsSprite);
	lua::TableAssoc(L, "GetIconsSprite", Lua_MinimapGetIconsSprite);
	lua::TableAssoc(L, "GetShakeDuration", Lua_MinimapGetShakeDuration);
	lua::TableAssoc(L, "SetShakeDuration", Lua_MinimapSetShakeDuration);
	lua::TableAssoc(L, "GetShakeOffset", Lua_MinimapGetShakeOffset);
	lua::TableAssoc(L, "SetShakeOffset", Lua_MinimapSetShakeOffset);
	//};
	lua_setglobal(L, "Minimap");
	//lua::RegisterNewClass(L, lua::metatables::MinimapMT, lua::metatables::MinimapMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterMinimap(_state);
}