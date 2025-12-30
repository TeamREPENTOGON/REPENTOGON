#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"

struct Lua_EntitySaveState {
    std::vector<EntitySaveState>* vec;
    int index;

    static inline EntitySaveState& GetEntitySaveState(lua_State* L, int idx)
    {
        Lua_EntitySaveState* ud = lua::GetRawUserdata<Lua_EntitySaveState*>(L, idx, lua::metatables::EntitySaveStateMT);
        EntitySaveState& st = (*ud->vec)[ud->index];
        return st;
    }
};

struct Lua_EntitiesSaveStateVector {
    std::vector<EntitySaveState>* data;
};

struct Lua_GridEntitiesSaveStateVector {
    std::vector<GridEntityDesc>* data;
};