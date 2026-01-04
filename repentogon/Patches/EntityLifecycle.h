#pragma once

#include "LuaCore.h"

namespace EntityLifecycle::detail::Init
{
    void BindLuaCallbacks(lua_State* L, int tblIdx);
}

namespace EntityLifecycle::detail::Patches
{
    void ApplyPatches();
}