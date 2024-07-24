#pragma once

#include "IsaacRepentance.h"

struct Lua_EntitySaveState {
    std::vector<EntitySaveState>* vec;
    int index;
};

struct Lua_EntitiesSaveStateVector {
    std::vector<EntitySaveState>* data;
};

struct Lua_GridEntitiesSaveStateVector {
    std::vector<GridEntityDesc>* data;
};