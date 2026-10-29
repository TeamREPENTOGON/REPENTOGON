#pragma once

#include <string>
#include "LuaCore.h"
#include "IsaacRepentance.h"

enum GameStateSlot
{
	NULL_SLOT = -1,
	SAVE_FILE = 0,
	GLOWING_HOURGLASS_1 = 1,
	GLOWING_HOURGLASS_2 = 2,
};

struct GameStateSaveInfo
{
	/// @brief saveSlot is a 1-based index.
	/// also encodes isUnknown
	int saveSlot = 0;
	std::string fileName;
	bool isRemote = false;

	bool IsUnknown() const;
};

inline bool GameStateSaveInfo::IsUnknown() const
{
	return this->saveSlot < 1;
}

inline bool IsGlowingHourglassSlot(GameStateSlot slot)
{
	return slot == GameStateSlot::GLOWING_HOURGLASS_1 || slot == GameStateSlot::GLOWING_HOURGLASS_2;
};

GameStateSlot GetGameStateSlot(GameState* state);

namespace SaveStateManager
{
	namespace detail
	{
		void BindLuaCallbacks(lua_State* L, int tbl);
	}
};