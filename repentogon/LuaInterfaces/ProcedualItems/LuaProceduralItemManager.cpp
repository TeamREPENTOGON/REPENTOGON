#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetProceduralItemManager) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ProceduralItemManager** ud = (ProceduralItemManager**)lua_newuserdata(L, sizeof(void*));
	*ud = game->GetProceduralItemManager();
	luaL_setmetatable(L, lua::metatables::ProceduralItemManagerMT);
	return 1;
}

LUA_FUNCTION(Lua_PIMCreateProceduralItem)
{
	ProceduralItemManager* pim = *lua::GetUserdata<ProceduralItemManager**>(L, 1, lua::metatables::ProceduralItemManagerMT);
	unsigned int seed = (unsigned int)luaL_checkinteger(L, 2);
	unsigned int unk = (unsigned int)luaL_checkinteger(L, 3);
	lua_pushinteger(L, pim->CreateProceduralItem(seed, unk));
	return 1;
}

LUA_FUNCTION(Lua_PIMGetProceduralItemCount)
{
	ProceduralItemManager* pim = *lua::GetUserdata<ProceduralItemManager**>(L, 1, lua::metatables::ProceduralItemManagerMT);
	lua_pushinteger(L, pim->GetProceduralItems()->size());
	return 1;
}

LUA_FUNCTION(Lua_PIMGetProceduralItem)
{
	ProceduralItemManager* pim = *lua::GetUserdata<ProceduralItemManager**>(L, 1, lua::metatables::ProceduralItemManagerMT);
	std::vector<ProceduralItem*>* items = pim->GetProceduralItems();
	int index = (int)luaL_checkinteger(L, 2);
	if (l_likely(index >= 0 && (unsigned int)index < items->size())) {
		ProceduralItem** ud = (ProceduralItem**)lua_newuserdata(L, sizeof(void*));
		*ud = (*items)[index];
		luaL_setmetatable(L, lua::metatables::ProceduralItemMT);
	}
	else {
		lua_pushnil(L);
	}
	return 1;
}

static void RegisterProceduralItemManager(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::GAME, "GetProceduralItemManager", Lua_GetProceduralItemManager);

	luaL_Reg functions[] = {
		{ "CreateProceduralItem", Lua_PIMCreateProceduralItem },
		{ "GetProceduralItemCount", Lua_PIMGetProceduralItemCount},
		{ "GetProceduralItem", Lua_PIMGetProceduralItem},
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::ProceduralItemManagerMT, lua::metatables::ProceduralItemManagerMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterProceduralItemManager(_state);
}