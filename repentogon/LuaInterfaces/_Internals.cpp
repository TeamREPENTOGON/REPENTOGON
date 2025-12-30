#include "_Internals.h"

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../Patches/ExtraRenderSteps.h"
#include "../SaveStateManagement/EntitySaveStateManagement.h"

namespace ErrorDisplay = ExtraRenderSteps::ErrorDisplay;

LUA_FUNCTION(Lua_RaiseModError)
{
    const char* modName = luaL_checkstring(L, 1);
    std::string message = std::string(modName) + " is causing errors!";

    ErrorDisplay::RaiseError(message, ErrorDisplay::LUA_ERROR_PRIORITY);

    return 0;
}

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
    lua::TableAssoc(L, "RaiseModError", Lua_RaiseModError);
	lua::TableAssoc(L, "GetModId", Lua_GetModId);

	lua_newtable(L);
	{
		lua::LuaStackProtector protector(L);
		EntitySaveStateManagement::detail::Init::RegisterLuaInternals(L, -1);
	}
	lua_setfield(L, -2, "ESSM");
}