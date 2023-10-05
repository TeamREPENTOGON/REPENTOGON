#pragma once

#include <lua.hpp>
#include "IsaacRepentance.h"

struct LuaLevelGeneratorRoom {
	LevelGenerator* context = nullptr;
	LevelGenerator_Room* room = nullptr;
	bool cleanup = false;
};
