#pragma once

#include "LuaCore.h"

namespace EntityLifecycle
{
    namespace detail
    {
        namespace Init
        {
            void BindLuaCallbacks(lua_State* L, int tblIdx);
        }
    
        namespace Patches
        {
            void ApplyPatches();
        }
    }
}