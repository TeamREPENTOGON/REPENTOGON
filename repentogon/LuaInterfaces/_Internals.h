#pragma once

#include "LuaCore.h"

namespace LuaInternals
{
    void RegisterInternals(lua_State* L);
    void RaiseInitError();
}

namespace LuaInternals::detail
{
    void RaiseInitError();
}