#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* ProceduralItemManagerMT = "ProceduralItemManager";

static int Lua_GetProceduralItemManager(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ProceduralItemManager** ud = (ProceduralItemManager**)lua_newuserdata(L, sizeof(void*));
	*ud = game->GetProceduralItemManager();
	luaL_setmetatable(L, ProceduralItemManagerMT);
	return 1;
}

int Lua_PIMCreateProceduralItem(lua_State* L)
{
	ProceduralItemManager* pim = *lua::GetUserdata<ProceduralItemManager**>(L, 1, ProceduralItemManagerMT);
	unsigned int seed = (unsigned int)luaL_checkinteger(L, 2);
	unsigned int unk = (unsigned int)luaL_checkinteger(L, 3);
	lua_pushinteger(L, pim->CreateProceduralItem(seed, unk));
	return 1;
}

int Lua_PIMGetProceduralItemCount(lua_State* L)
{
	ProceduralItemManager* pim = *lua::GetUserdata<ProceduralItemManager**>(L, 1, ProceduralItemManagerMT);
	lua_pushinteger(L, pim->GetProceduralItems()->size());
	return 1;
}

int Lua_PIMGetProceduralItem(lua_State* L)
{
	ProceduralItemManager* pim = *lua::GetUserdata<ProceduralItemManager**>(L, 1, ProceduralItemManagerMT);
	std::vector<ProceduralItem*> *items = pim->GetProceduralItems();
	int index = luaL_checkinteger(L, 2);
	if(l_likely(index >= 0 && index < items->size())){
		ProceduralItem** ud = (ProceduralItem **)lua_newuserdata(L, sizeof(void *));
		*ud = (*items)[index];
		luaL_setmetatable(L, "ProceduralItem");
	}else{
		lua_pushnil(L);
	}
	return 1;
}

static void RegisterProceduralItemManager(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetProceduralItemManager");
	lua_pushcfunction(L, Lua_GetProceduralItemManager);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, ProceduralItemManagerMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "CreateProceduralItem", Lua_PIMCreateProceduralItem },
		{ "GetProceduralItemCount", Lua_PIMGetProceduralItemCount},
		{ "GetProceduralItem", Lua_PIMGetProceduralItem},
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterProceduralItemManager(state);
}