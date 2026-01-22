#pragma once

#include "LuaCore.h"

namespace EntityManager
{
    void CommitRemovedEntities();
}

namespace EntityManager::detail::Init
{
    void BindLuaCallbacks(lua_State* L, int tblIdx);
}

namespace EntityManager::detail::Patches
{
    void ApplyPatches();
}