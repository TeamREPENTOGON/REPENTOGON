#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*LUA_FUNCTION(Lua_GetDebugRenderer)
{
	Game* game = lua::GetRawUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	DebugRenderer** ud = (DebugRenderer**)lua_newuserdata(L, sizeof(DebugRenderer*));
	*ud = game->GetDebugRenderer();
	luaL_setmetatable(L, lua::metatables::DebugRendererMT);
	return 1;
}
*/

LUA_FUNCTION(Lua_DebugRendererGet) {
	DebugRenderer* debugRenderer = g_Game->GetDebugRenderer();
	int index = (int)luaL_optinteger(L, 1, -1);
	bool unk = lua::luaL_optboolean(L, 2, false);
	Shape** ud = (Shape**)lua_newuserdata(L, sizeof(Shape*));
	*ud = debugRenderer->Get(index, unk);
	luaL_setmetatable(L, lua::metatables::ShapeMT);

	return 1;
}

static void RegisterDebugRenderer(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetDebugRenderer", Lua_GetDebugRenderer);
	lua_newtable(L);
	//luaL_Reg functions[] = {
	lua::TableAssoc(L, "Get", Lua_DebugRendererGet);
//		{ NULL, NULL }
	//};
	//lua::RegisterNewClass(L, lua::metatables::DebugRendererMT, lua::metatables::DebugRendererMT, functions);
	lua_setglobal(L, "DebugRenderer");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(_state);
	RegisterDebugRenderer(_state);
}

