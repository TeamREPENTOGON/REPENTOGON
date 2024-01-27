#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*LUA_FUNCTION(Lua_GetProceduralItemManager) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ProceduralItemManager** ud = (ProceduralItemManager**)lua_newuserdata(L, sizeof(void*));
	*ud = game->GetProceduralItemManager();
	luaL_setmetatable(L, lua::metatables::ProceduralItemManagerMT);
	return 1;
}
*/

LUA_FUNCTION(Lua_PIMCreateProceduralItem)
{
	ProceduralItemManager* pim = g_Game->GetProceduralItemManager();
	unsigned int seed = (unsigned int)luaL_checkinteger(L, 1);
	unsigned int unk = (unsigned int)luaL_checkinteger(L, 2);
	lua_pushinteger(L, pim->CreateProceduralItem(seed, unk));
	return 1;
}

LUA_FUNCTION(Lua_PIMGetProceduralItemCount)
{
	ProceduralItemManager* pim = g_Game->GetProceduralItemManager();
	lua_pushinteger(L, pim->GetProceduralItems()->size());
	return 1;
}

LUA_FUNCTION(Lua_PIMGetProceduralItem)
{
	ProceduralItemManager* pim = g_Game->GetProceduralItemManager();
	std::vector<ProceduralItem*>* items = pim->GetProceduralItems();
	int index = (int)luaL_checkinteger(L, 1);
	if (index >= 0 && (unsigned int)index < items->size()) [[likely]] {
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
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetProceduralItemManager", Lua_GetProceduralItemManager);
	lua_newtable(L);
	
	lua::TableAssoc(L, "CreateProceduralItem", Lua_PIMCreateProceduralItem);
	lua::TableAssoc(L, "GetProceduralItemCount", Lua_PIMGetProceduralItemCount);
	lua::TableAssoc(L, "GetProceduralItem", Lua_PIMGetProceduralItem);
	lua_setglobal(L, "ProceduralItemManager");
	//lua::RegisterNewClass(L, lua::metatables::ProceduralItemManagerMT, lua::metatables::ProceduralItemManagerMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterProceduralItemManager(_state);
}