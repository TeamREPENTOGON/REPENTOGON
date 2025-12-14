#include "_Internals.h"

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "../SaveStateManagement/EntitySaveStateManagement.h"

LUA_FUNCTION(Lua_GetModId)
{
	constexpr int tblIdx = 1;
	if (!lua_istable(L, tblIdx))
	{
		return luaL_argerror(L, tblIdx, "Expected a table.");
	}

	ModReference* modRef = g_LuaEngine->GetModRefByTable(tblIdx);
	if (!modRef)
	{
		lua_pushnil(L);
		return 1;
	}

	lua_pushstring(L, modRef->_mainLuaPath.c_str());
	return 1;
}

void LuaInternals::RegisterInternals(lua_State *L)
{
	lua::TableAssoc(L, "GetModId", Lua_GetModId);

	lua_newtable(L);
	EntitySaveStateManagement::Init::RegisterLuaInternals(L);
	lua_setfield(L, -2, "ESSM");
}