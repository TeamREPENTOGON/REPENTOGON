#pragma once

#include <string>

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
	uint32_t saveSlot = 0;
	std::string fileName;
	bool isRemote = false;

	bool IsUnknown() const;
};

inline bool GameStateSaveInfo::IsUnknown() const
{
	return this->saveSlot < 1;
}