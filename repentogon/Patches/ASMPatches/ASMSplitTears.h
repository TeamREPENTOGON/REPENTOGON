#pragma once

#include <variant>
#include <string>

#include "IsaacRepentance.h"
#include "LuaCore.h"

namespace SplitTears {

enum SplitTearType {
	SPLIT_GENERIC,
	SPLIT_QUAD,  // Cricket's Body
	SPLIT_PARASITE,
	SPLIT_BONE,
	SPLIT_ABSORB,  // Lachryphagy
	SPLIT_SPORE,  // Mucormycosis
	SPLIT_STICKY,  // Explosivo/Booger/Spore for knives/lasers/ludo
	SPLIT_BURST,  // Haemolacria
	SPLIT_POP,  // Pop! + Lasers
	SPLIT_MULTIDIMENSIONAL,
	SPLIT_ANGELIC_PRISM,
};

typedef std::variant<SplitTearType, std::string> CustomSplitTearType;

// Shared lua function for Entity_Tear, Entity_Laser, and Entity_Knife
LUALIB_API int Lua_FireSplitTear(lua_State* L);

void ASMPatchesForSplitTearCallback();

}  // namespace SplitTears
