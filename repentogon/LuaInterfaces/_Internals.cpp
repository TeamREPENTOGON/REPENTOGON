#include "_Internals.h"

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../Patches/ExtraRenderSteps.h"

namespace ErrorDisplay = ExtraRenderSteps::ErrorDisplay;

LUA_FUNCTION(Lua_RaiseModError)
{
    const char* modName = luaL_checkstring(L, 1);
    std::string message = std::string(modName) + " is causing errors!";

    ErrorDisplay::RaiseError(message, ErrorDisplay::LUA_ERROR_PRIORITY);

    return 0;
}

void LuaInternals::RegisterInternals(lua_State *L)
{
    lua::TableAssoc(L, "RaiseModError", Lua_RaiseModError);
}