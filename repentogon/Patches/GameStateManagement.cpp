#include "GameStateManagement.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "Log.h"

#include <filesystem>
#include "ItemPoolManager.h"
#include "VirtualRoomSets.h"

#include "../SaveStateManagement/EntitySaveStateManagement.h"

namespace ESSM = EntitySaveStateManagement;

#pragma region Helpers

static GameStateSlot get_game_state_slot(GameState* state)
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

static inline bool is_remote_save(GameState* gameState)
{
	auto gameStateIOPtr = reinterpret_cast<void**>(gameState->_saveFile);
	if (!gameStateIOPtr)
	{
		return !(gameState->_cloudGameStatePath.empty() || gameState->_cloudRerunStatePath.empty());
	}

	return *gameStateIOPtr == &SteamCloudFile_vftable;
}

static inline std::string get_state_file_name(GameState* gameState, uint32_t saveSlot, bool isRerun)
{
	return (is_remote_save(gameState) ? "remote" : "local") + std::string("_") + (isRerun ? "rerunstate" : "gamestate") + std::to_string(saveSlot);
}

static inline uint32_t get_state_slot(GameState* gameState, GameStateIO* io, bool isRerun)
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

	const char* slotString = &fileName.back();
	char* endPtr = nullptr;
	uint32_t slot = std::strtol(&fileName.back(), &endPtr, 10);

	if (endPtr != slotString + 1) // the slot should always be one character long, this also covers errno == ERANGE
	{
		return 0;
	}

	return slot;
}

static inline std::string get_state_file_name(GameState* gameState, GameStateIO* io, bool isRerun)
{
	uint32_t save_slot = get_state_slot(gameState, io, isRerun);
	if (!(1 <= save_slot && save_slot <= 3))
	{
		return "";
	}

	return get_state_file_name(gameState, save_slot, isRerun);
}

static bool is_file_name_valid(const std::string& fileName)
{
	if (fileName.empty())
	{
		return false;
	}

	const char* slotString = &fileName.back();
	char* endPtr = nullptr;
	uint32_t slot = std::strtol(&fileName.back(), &endPtr, 10);

	if (endPtr != slotString + 1) // the slot should always be one character long, this also covers errno == ERANGE
	{
		return false;
	}

	return true;
}

#pragma endregion

static inline void save_game_state(uint32_t slot)
{
	ItemPoolManager::__SaveState(slot);
	VirtualRoomSetManager::__SaveGameState(slot);
}

static inline void restore_game_state(uint32_t slot, bool startGame)
{
	ItemPoolManager::__RestoreState(slot);
}

static inline void clear_game_state(uint32_t slot)
{
	ItemPoolManager::__ClearSaveState(slot);
}

static inline bool write_save(const std::string& fileName, bool isRerun)
{
	ItemPoolManager::__SaveToDisk(fileName, isRerun);
	VirtualRoomSetManager::__WriteSave(fileName, isRerun);
	return true;
}

// Return false to invalidate the game state
static inline bool read_save(const std::string& fileName, bool isRerun)
{
	if (!VirtualRoomSetManager::__ReadSave(fileName, isRerun))
	{
		return false;
	}

	ItemPoolManager::__LoadFromDisk(fileName, isRerun);
	return true;
}

static inline void delete_save(const std::string& fileName, bool isRerun)
{
	ItemPoolManager::__DeleteGameState(fileName);
	VirtualRoomSetManager::__DeleteSave(fileName, isRerun);
	ESSM::detail::SaveData::DeleteGameState(fileName);
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
	ESSM::detail::SaveData::WriteState state = ESSM::detail::SaveData::WriteGameState();
	bool success = super(io);
	ESSM::detail::SaveData::RestoreWrittenStates(state);

	if (!success)
	{
		return false;
	}

	if (get_game_state_slot(this) != GameStateSlot::SAVE_FILE) // This can occur when (presumably) the save state is being sent to online players joining mid-run
	{
		ZHL::Log("[INFO] [GameStateManagement] writing non save file GameState, skipping write.\n");
		return true;
	}

	auto fileName = get_state_file_name(this, *io, false);
	if (!is_file_name_valid(fileName))
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping write.\n", fileName.c_str());
		return true;
	}

	ESSM::detail::SaveData::Serialize(fileName, state);
	return write_save(fileName, false);
}

HOOK_METHOD(GameState, write_rerun, (GameStateIO** io) -> bool)
{
	ESSM::detail::SaveData::WriteState state = ESSM::detail::SaveData::WriteGameState();
	bool success = super(io);
	ESSM::detail::SaveData::RestoreWrittenStates(state);

	if (!success)
	{
		return false;
	}

	auto fileName = get_state_file_name(this, *io, true);
	if (!is_file_name_valid(fileName))
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping write.\n", fileName.c_str());
		return true;
	}

	ESSM::detail::SaveData::Serialize(fileName, state);
	return write_save(fileName, true);
}

HOOK_METHOD(GameState, read, (GameStateIO** io, bool isLocalRun) -> bool)
{
	bool originalSuccess = super(io, isLocalRun);
	ESSM::detail::SaveData::ReadState essmReadState = ESSM::detail::SaveData::ReadGameState();
	bool success = originalSuccess && !ESSM::detail::SaveData::CheckErrors(essmReadState);

	if (!success)
	{
		if (originalSuccess)
		{
			ZHL::Log("[ERROR] [GameStateManagement] GameState failed internal validation on read.\n");
		}
		return false;
	}

	bool needsHandling = ESSM::detail::SaveData::NeedsHandling(essmReadState);

	if (!isLocalRun) // This occurs when loading a game state upon joining an already existing match.
	{
		ZHL::Log("[INFO] [GameStateManagement] reading non save file GameState, skipping read.\n");
		return !needsHandling;
	}

	auto fileName = get_state_file_name(this, *io, false);
	if (!is_file_name_valid(fileName))
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping read.\n", fileName.c_str());
		return !needsHandling;
	}

	if (!ESSM::detail::SaveData::Deserialize(fileName, essmReadState))
	{
		return false;
	}

	return read_save(fileName, false);
}

HOOK_METHOD(GameState, read_rerun, (GameStateIO** io) -> bool)
{
	bool originalSuccess = super(io);
	ESSM::detail::SaveData::ReadState essmReadState = ESSM::detail::SaveData::ReadGameState();
	bool success = originalSuccess && !ESSM::detail::SaveData::CheckErrors(essmReadState);

	if (!success)
	{
		if (originalSuccess)
		{
			ZHL::Log("[ERROR] [GameStateManagement] GameState failed internal validation on read.\n");
		}
		return false;
	}

	bool needsHandling = ESSM::detail::SaveData::NeedsHandling(essmReadState);

	auto fileName = get_state_file_name(this, *io, true);
	if (!is_file_name_valid(fileName))
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping read.\n", fileName.c_str());
		return !needsHandling;
	}

	if (!ESSM::detail::SaveData::Deserialize(fileName, essmReadState))
	{
		return false;
	}

	return read_save(fileName, true);
}

HOOK_METHOD(GameState, Delete, () -> void)
{
	auto fileName = get_state_file_name(this, this->_saveFile, false);
	super();
	if (!is_file_name_valid(fileName))
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
	if (!is_file_name_valid(fileName))
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown file name \"%s\", skipping delete.\n", fileName.c_str());
		return;
	}

	delete_save(fileName, true);
}
#pragma endregion