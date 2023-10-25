#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GameGetMinimap)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Minimap** toLua = (Minimap**)lua_newuserdata(L, sizeof(Minimap*));
	*toLua = game->GetMinimap();
	luaL_setmetatable(L, lua::metatables::MinimapMT);
	return 1;
}

LUA_FUNCTION(Lua_MinimapGetDisplayedSize)
{
	Minimap* minimap = *lua::GetUserdata<Minimap**>(L, 1, lua::metatables::MinimapMT);
	Vector buffer;
	buffer = *minimap->GetDisplayedSize(buffer);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), buffer);

	return 1;
}

static void RegisterMinimap(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::GAME, "GetMinimap", Lua_GameGetMinimap);

	luaL_Reg functions[] = {
		{ "GetDisplayedSize", Lua_MinimapGetDisplayedSize },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::MinimapMT, lua::metatables::MinimapMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterMinimap(_state);
}