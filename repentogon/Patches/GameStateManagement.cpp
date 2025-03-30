#include "GameStateManagerment.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "Log.h"

#include <filesystem>
#include "ItemPoolManager.h"

#pragma region Helpers

static GameStateSlot get_game_state_slot(GameState* state) noexcept
{
	if (g_Manager == nullptr || g_Game == nullptr)
	{
		return GameStateSlot::NULL_SLOT;
	}

	static const std::map<uintptr_t, GameStateSlot> slotMap = {
		{ (uintptr_t)&g_Manager->_gamestate, GameStateSlot::SAVE_FILE },
		{ (uintptr_t)&g_Game->_glowingHourglassStates[0]._gameState, GameStateSlot::GLOWING_HOURGLASS_1 },
		{ (uintptr_t)&g_Game->_glowingHourglassStates[1]._gameState, GameStateSlot::GLOWING_HOURGLASS_2 }
	};

	uintptr_t slotPtr = reinterpret_cast<uintptr_t>(state);
	auto it = slotMap.find(slotPtr);

	if (it == slotMap.end())
	{
		return GameStateSlot::NULL_SLOT;
	}

	return it->second;
}

static inline bool is_remote_save(GameState* gameState) noexcept
{
	auto gameStateIOPtr = reinterpret_cast<void**>(gameState->_saveFile);
	if (!gameStateIOPtr)
	{
		return !(gameState->_cloudGameStatePath.empty() || gameState->_cloudRerunStatePath.empty());
	}

	return *gameStateIOPtr == &SteamCloudFile_vftable;
}

static inline std::string get_state_file_name(GameState* gameState, uint32_t saveSlot, bool isRerun) noexcept
{
	return (is_remote_save(gameState) ? "remote" : "local") + std::string("_") + (isRerun ? "rerunstate" : "gamestate") + std::to_string(saveSlot);
}

static inline uint32_t get_state_slot(GameState* gameState, GameStateIO* io, bool isRerun) noexcept
{
	const char* filePath = nullptr;
	if (io)
	{
		filePath = io->GetFilePath();
	}

	if (!filePath)
	{
		if (isRerun)
		{
			filePath = !gameState->_cloudRerunStatePath.empty() ? gameState->_cloudRerunStatePath.c_str() : gameState->_localRerunStatePath.c_str();
		}
		else
		{
			filePath = !gameState->_cloudGameStatePath.empty() ? gameState->_cloudGameStatePath.c_str() : gameState->_localGameStatePath.c_str();
		}
	}

	if (!filePath)
	{
		return 0;
	}

	std::string fileName = std::filesystem::path(filePath).stem().string();
	if (fileName.empty())
	{
		return 0;
	}

	return std::atoi(&fileName.back());
}

static inline std::string get_state_file_name(GameState* gameState, GameStateIO* io, bool isRerun) noexcept
{
	uint32_t save_slot = get_state_slot(gameState, io, isRerun);
	if (!(1 <= save_slot && save_slot <= 3))
	{
		return "";
	}

	return get_state_file_name(gameState, save_slot, isRerun);
}

#pragma endregion

static inline void save_game_state(uint32_t slot) noexcept
{
	ItemPoolManager::__SaveState(slot);
}

static inline void restore_game_state(uint32_t slot, bool startGame) noexcept
{
	ItemPoolManager::__RestoreState(slot);
}

static inline void clear_game_state(uint32_t slot) noexcept
{
	ItemPoolManager::__ClearSaveState(slot);
}

static inline bool write_save(const std::string& fileName, bool isRerun) noexcept
{
	ItemPoolManager::__SaveToDisk(fileName, isRerun);
	return true;
}

// Return false to invalidate the game state
static inline bool read_save(const std::string& fileName, bool isRerun) noexcept
{
	ItemPoolManager::__LoadFromDisk(fileName, isRerun);
	return true;
}

static inline void delete_save(const std::string& fileName, bool isRerun) noexcept
{
	ItemPoolManager::__DeleteGameState(fileName);
}

#pragma region Hooks

HOOK_METHOD(Game, SaveState, (GameState* state) -> void)
{
	super(state);

	auto slot = get_game_state_slot(state);
	if (slot == GameStateSlot::NULL_SLOT)
	{
		ZHL::Log("[INFO] [GameStateManagement] could not determine Game State slot, skipping save.\n");
		return;
	}

	save_game_state(slot);
}

HOOK_METHOD(Game, RestoreState, (GameState* state, bool startGame) -> void)
{
	super(state, startGame);

	auto slot = get_game_state_slot(state);
	if (slot == GameStateSlot::NULL_SLOT)
	{
		ZHL::Log("[INFO] [GameStateManagement] could not determine Game State slot, skipping restore.\n");
		return;
	}

	restore_game_state(slot, startGame);
}

HOOK_METHOD(GameState, Clear, () -> void)
{
	super();

	auto slot = get_game_state_slot(this);
	if (slot == GameStateSlot::NULL_SLOT)
	{
		ZHL::Log("[INFO] [GameStateManagement] could not determine Game State slot, skipping clear.\n");
		return;
	}

	clear_game_state(slot);
}

HOOK_METHOD(GameState, write, (GameStateIO** io) -> bool)
{
	if (!super(io))
	{
		return false;
	}

	if (get_game_state_slot(this) != GameStateSlot::SAVE_FILE) // This can occur when (presumably) the save state is being sent to online players joining mid-run
	{
		ZHL::Log("[INFO] [GameStateManagement] writing non save file GameState, skipping write.\n");
		return true;
	}

	auto fileName = get_state_file_name(this, *io, false);
	if (fileName.empty() || std::atoi(&fileName.back()) == 0)
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping write.\n", fileName.c_str());
		return true;
	}

	return write_save(fileName, false);
}

HOOK_METHOD(GameState, write_rerun, (GameStateIO** io) -> bool)
{
	if (!super(io))
	{
		return false;
	}

	auto fileName = get_state_file_name(this, *io, true);
	if (fileName.empty() || std::atoi(&fileName.back()) == 0)
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping write.\n", fileName.c_str());
		return true;
	}

	return write_save(fileName, true);
}

HOOK_METHOD(GameState, read, (GameStateIO** io, bool isLocalRun) -> bool)
{
	if (!super(io, isLocalRun))
	{
		return false;
	}

	if (!isLocalRun) // This occurs when loading a game state upon joining an already existing match.
	{
		ZHL::Log("[INFO] [GameStateManagement] reading non save file GameState, skipping read.\n");
		return true;
	}

	auto fileName = get_state_file_name(this, *io, false);
	if (fileName.empty() || std::atoi(&fileName.back()) == 0)
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping read.\n", fileName.c_str());
		return true;
	}

	return read_save(fileName, false);
}

HOOK_METHOD(GameState, read_rerun, (GameStateIO** io) -> bool)
{
	if (!super(io))
	{
		return false;
	}

	auto fileName = get_state_file_name(this, *io, true);
	if (fileName.empty() || std::atoi(&fileName.back()) == 0)
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping read.\n", fileName.c_str());
		return true;
	}

	return read_save(fileName, true);
}

HOOK_METHOD(GameState, Delete, () -> void)
{
	auto fileName = get_state_file_name(this, this->_saveFile, false);
	super();
	if (fileName.empty() || std::atoi(&fileName.back()) == 0)
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping delete.\n", fileName.c_str());
		return;
	}

	delete_save(fileName, false);
}

HOOK_METHOD(GameState, DeleteRerun, () -> void)
{
	auto fileName = get_state_file_name(this, this->_saveFile, true);
	super();
	if (fileName.empty() || std::atoi(&fileName.back()) == 0)
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping delete.\n", fileName.c_str());
		return;
	}

	delete_save(fileName, true);
}
#pragma endregion