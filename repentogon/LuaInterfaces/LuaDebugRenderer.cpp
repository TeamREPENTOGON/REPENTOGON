#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetDebugRenderer)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	DebugRenderer** ud = (DebugRenderer**)lua_newuserdata(L, sizeof(DebugRenderer*));
	*ud = game->GetDebugRenderer();
	luaL_setmetatable(L, lua::metatables::DebugRendererMT);
	return 1;
}

LUA_FUNCTION(Lua_DebugRendererGet) {
	DebugRenderer* debugRenderer = *lua::GetUserdata<DebugRenderer**>(L, 1, lua::metatables::DebugRendererMT);
	int index = (int)luaL_checkinteger(L, 2);
	bool unk = lua_toboolean(L, 3);
	Shape** ud = (Shape**)lua_newuserdata(L, sizeof(Shape*));
	*ud = debugRenderer->Get(index, unk);
	luaL_setmetatable(L, lua::metatables::ShapeMT);

	return 1;
}

static void RegisterDebugRenderer(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::GAME, "GetDebugRenderer", Lua_GetDebugRenderer);

	luaL_Reg functions[] = {
		{ "Get", Lua_DebugRendererGet },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::DebugRendererMT, lua::metatables::DebugRendererMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(_state);
	RegisterDebugRenderer(_state);
}

