#include "GameStateManagement.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "Log.h"

#include <filesystem>
#include "../Patches/ItemPoolManager.h"
#include "../LuaInterfaces/_Internals.h"
#include "../LuaInterfaces/CustomCallbacks.h"
#include "../VirtualRoomConfig/VirtualRoomSetManager.h"
#include "../Patches/Stages/VirtualGridRooms.h"

#include "EntitySaveStateManagement.h"

namespace ESSM = EntitySaveStateManagement;

#define LOG_ERROR_HEADER "[ERROR] [SSM] - "

#pragma region Data

namespace
{
struct LuaCallbacks
{
	int storeGameState = LUA_NOREF;
    int restoreGameState = LUA_NOREF;
	int clearGameState = LUA_NOREF;
	int serializeGameState = LUA_NOREF;
	int deserializeGameState = LUA_NOREF;
	int deleteGameState = LUA_NOREF;
    int cleanupState = LUA_NOREF;
};
} // end namespace

static LuaCallbacks s_luaCallbacks;

#pragma endregion

#pragma region Helpers

static GameStateSaveInfo get_info(GameState* gameState, GameStateIO* io, bool isRerun);
static bool is_remote_save(GameState* gameState);
static uint32_t get_state_slot(GameState* gameState, GameStateIO* io, bool isRerun);
static std::string get_state_file_name(GameState* gameState, uint32_t saveSlot, bool isRerun);
static std::string get_state_file_name(GameState* gameState, GameStateIO* io, bool isRerun);
static bool is_file_name_valid(const std::string& fileName);

static GameStateSaveInfo get_info(GameState* gameState, GameStateIO* io, bool isRerun)
{
	GameStateSaveInfo info;
	if (!isRerun && GetGameStateSlot(gameState) != GameStateSlot::SAVE_FILE) // This can occur when (presumably) the save state is being sent to online players joining mid-run
	{
		return info;
	}

	info.saveSlot = get_state_slot(gameState, io, isRerun);
	if (!(1 <= info.saveSlot && info.saveSlot <= 3))
	{
		info.saveSlot = 0;
		return info;
	}

	info.isRemote = is_remote_save(gameState);
	info.fileName = get_state_file_name(gameState, info.saveSlot, isRerun);
	if (!is_file_name_valid(info.fileName))
	{
		info.saveSlot = 0;
		return info;
	}

	return info;
};

GameStateSlot GetGameStateSlot(GameState* state)
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

static bool is_remote_save(GameState* gameState)
{
	auto gameStateIOPtr = reinterpret_cast<void**>(gameState->_saveFile);
	if (!gameStateIOPtr)
	{
		return !(gameState->_cloudGameStatePath.empty() || gameState->_cloudRerunStatePath.empty());
	}

	return *gameStateIOPtr == &SteamCloudFile_vftable;
}

static std::string get_state_file_name(GameState* gameState, uint32_t saveSlot, bool isRerun)
{
	return (is_remote_save(gameState) ? "remote" : "local") + std::string("_") + (isRerun ? "rerunstate" : "gamestate") + std::to_string(saveSlot);
}

static uint32_t get_state_slot(GameState* gameState, GameStateIO* io, bool isRerun)
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

static std::string get_state_file_name(GameState* gameState, GameStateIO* io, bool isRerun)
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

#pragma region Lua

static void lua_call_game_state_slot_callback(int fnIdx, int slot);
static void lua_call_mod_data_callback(int fnIdx, const GameStateSaveInfo& gameStateInfo, int checksum);
static void LuaStoreGameState(int slot);
static void LuaRestoreGameState(int slot);
static void LuaClearGameState(int slot);
static void LuaSerializeGameState(const GameStateSaveInfo& gameStateInfo, int checksum);
static void LuaDeserializeGameState(const GameStateSaveInfo& gameStateInfo, int checksum);
static void LuaGameStateDelete(const GameStateSaveInfo& gameStateInfo);
static void LuaCleanupState();

static void lua_call_game_state_slot_callback(int fnIdx, int slot)
{
	lua_State* L = g_LuaEngine->_state;

	lua_rawgeti(L, LUA_REGISTRYINDEX, fnIdx);
	lua_pushinteger(L, slot);

	if (lua_pcall(L, 1, 0, 0) != LUA_OK)
	{
		ZHL::Log(LOG_ERROR_HEADER "%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

static void lua_call_mod_data_callback(int fnIdx, const GameStateSaveInfo& gameStateInfo, int checksum)
{
	lua_State* L = g_LuaEngine->_state;

	lua_rawgeti(L, LUA_REGISTRYINDEX, fnIdx);

	lua_createtable(L, 0, 2);
	{
		lua_pushinteger(L, checksum);
		lua_setfield(L, -2, "checksum");	
	
		lua_pushlstring(L, gameStateInfo.fileName.c_str(), gameStateInfo.fileName.size());
		lua_setfield(L, -2, "fileName");
	}

	if (lua_pcall(L, 1, 0, 0) != LUA_OK)
	{
		ZHL::Log(LOG_ERROR_HEADER "%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

static void LuaStoreGameState(int slot)
{
	lua_call_game_state_slot_callback(s_luaCallbacks.storeGameState, slot);
}

static void LuaRestoreGameState(int slot)
{
	lua_call_game_state_slot_callback(s_luaCallbacks.restoreGameState, slot);
}

static void LuaClearGameState(int slot)
{
	lua_call_game_state_slot_callback(s_luaCallbacks.clearGameState, slot);
}

static void LuaSerializeGameState(const GameStateSaveInfo& gameStateInfo, int checksum)
{
	lua_call_mod_data_callback(s_luaCallbacks.serializeGameState, gameStateInfo, checksum);
}

static void LuaDeserializeGameState(const GameStateSaveInfo& gameStateInfo, int checksum)
{
	lua_call_mod_data_callback(s_luaCallbacks.deserializeGameState, gameStateInfo, checksum);
}

static void LuaGameStateDelete(const GameStateSaveInfo& gameStateInfo)
{
	lua_State* L = g_LuaEngine->_state;

	lua_rawgeti(L, LUA_REGISTRYINDEX, s_luaCallbacks.deleteGameState);
	lua_pushlstring(L, gameStateInfo.fileName.c_str(), gameStateInfo.fileName.size());

	if (lua_pcall(L, 1, 0, 0) != LUA_OK)
	{
		ZHL::Log(LOG_ERROR_HEADER "%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

static void LuaCleanupState()
{
    lua_State* L = g_LuaEngine->_state;

    lua_rawgeti(L, LUA_REGISTRYINDEX, s_luaCallbacks.cleanupState);

    if (lua_pcall(L, 0, 0, 0) != LUA_OK)
    {
        ZHL::Log(LOG_ERROR_HEADER "%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

#pragma endregion

static inline void save_game_state(uint32_t slot)
{
	ItemPoolManager::__SaveState(slot);
}

static inline void restore_game_state(uint32_t slot, bool startGame)
{
	ItemPoolManager::__RestoreState(slot);
}

static inline void clear_game_state(uint32_t slot)
{
	ItemPoolManager::__ClearSaveState(slot);
}

static inline bool write_save(GameState& gameState, const GameStateSaveInfo& info, bool isRerun)
{
	const std::string& fileName = info.fileName;

	if (!VirtualRoomSetManager::detail::WriteSave(gameState, info, isRerun))
	{
		return false;
	}

	ItemPoolManager::__SaveToDisk(fileName, isRerun);
	return true;
}

// Return false to invalidate the game state
static inline bool read_save(GameState& gameState, const GameStateSaveInfo& info, bool isRerun)
{
	const std::string& fileName = info.fileName;

	if (!VirtualRoomSetManager::detail::ReadSave(gameState, info, isRerun))
	{
		return false;
	}

	ItemPoolManager::__LoadFromDisk(fileName, isRerun);
	return true;
}

static inline void delete_save(const GameStateSaveInfo& info, bool isRerun)
{
	const std::string& fileName = info.fileName;
	uint32_t slot = info.saveSlot;

	ItemPoolManager::__DeleteGameState(fileName);
	VirtualRoomSetManager::detail::DeleteSave(info, isRerun);
	VirtualGridRooms::DeleteGameState(fileName);
	ESSM::detail::SaveData::DeleteGameState(fileName);
}

void SaveStateManager::detail::BindLuaCallbacks(lua_State *L, int tbl)
{
	assert(L == g_LuaEngine->_state);

    int tblIdx = lua_absindex(L, tbl);
    if (!lua_istable(L, tblIdx))
    {
        ZHL::Log(LOG_ERROR_HEADER "BindLuaCallbacks: expected a table\n");
        LuaInternals::RaiseInitError();
        assert(false);
        return;
    }

    auto BindCallback = [](lua_State* L, int tblIdx, const char* fieldName, int& outRef)
    {
        // reset ref
        luaL_unref(L, LUA_REGISTRYINDEX, outRef);
        outRef = LUA_NOREF;

        // bind callback to ref
        lua_getfield(L, tblIdx, fieldName);
        if (!lua_isfunction(L, -1))
        {
            ZHL::Log(LOG_ERROR_HEADER "BindLuaCallbacks: Expected '%s' to be a function\n", fieldName);
            LuaInternals::RaiseInitError();
            lua_pop(L, 1);
            assert(false);
            return;
        }

        outRef = luaL_ref(L, LUA_REGISTRYINDEX);
    };

    BindCallback(L, tblIdx, "StoreGameState", s_luaCallbacks.storeGameState);
    BindCallback(L, tblIdx, "RestoreGameState", s_luaCallbacks.restoreGameState);
    BindCallback(L, tblIdx, "ClearGameState", s_luaCallbacks.clearGameState);
    BindCallback(L, tblIdx, "SerializeGameState", s_luaCallbacks.serializeGameState);
    BindCallback(L, tblIdx, "DeserializeGameState", s_luaCallbacks.deserializeGameState);
    BindCallback(L, tblIdx, "DeleteGameState", s_luaCallbacks.deleteGameState);
    BindCallback(L, tblIdx, "CleanupState", s_luaCallbacks.cleanupState);
}

#pragma region Hooks

HOOK_METHOD(Game, SaveState, (GameState* state) -> void)
{
	auto slot = GetGameStateSlot(state);
	if (slot == GameStateSlot::NULL_SLOT)
	{
		super(state);
		ZHL::Log("[INFO] [GameStateManagement] could not determine Game State slot, skipping save.\n");
		return;
	}

	bool isGlowingHourglass = IsGlowingHourglassSlot(slot);

	CustomCallbacks::PRE_GAME_STATE_SAVE();
	if (isGlowingHourglass)
	{
		CustomCallbacks::PRE_GLOWING_HOURGLASS_SAVE(slot - GameStateSlot::GLOWING_HOURGLASS_1);
	}

	super(state);
	save_game_state(slot);

	CustomCallbacks::POST_GAME_STATE_SAVE();
	if (isGlowingHourglass)
	{
		CustomCallbacks::POST_GLOWING_HOURGLASS_SAVE(slot - GameStateSlot::GLOWING_HOURGLASS_1);
	}

	LuaStoreGameState(slot);
}

HOOK_METHOD(Game, RestoreState, (GameState* state, bool startGame) -> void)
{
	auto slot = GetGameStateSlot(state);
	if (slot == GameStateSlot::NULL_SLOT)
	{
		ZHL::Log("[INFO] [GameStateManagement] could not determine Game State slot, skipping restore.\n");
		super(state, startGame);
		return;
	}

	LuaRestoreGameState(slot);

	bool isGlowingHourglass = IsGlowingHourglassSlot(slot);

	CustomCallbacks::PRE_GAME_STATE_RESTORE();
	if (isGlowingHourglass)
	{
		CustomCallbacks::PRE_GLOWING_HOURGLASS_LOAD(slot - GameStateSlot::GLOWING_HOURGLASS_1);
	}

	super(state, startGame);
	restore_game_state(slot, startGame);

	CustomCallbacks::POST_GAME_STATE_RESTORE();
	if (isGlowingHourglass)
	{
		CustomCallbacks::POST_GLOWING_HOURGLASS_LOAD(slot - GameStateSlot::GLOWING_HOURGLASS_1);
	}
}

HOOK_METHOD(GameState, Clear, () -> void)
{
	super();

	auto slot = GetGameStateSlot(this);
	if (slot == GameStateSlot::NULL_SLOT)
	{
		ZHL::Log("[INFO] [GameStateManagement] could not determine Game State slot, skipping clear.\n");
		return;
	}

	clear_game_state(slot);
	LuaClearGameState(slot);
}

HOOK_METHOD(GameState, write, (GameStateIO** io) -> bool)
{
	bool isRerun = false;
	GameStateSaveInfo info = get_info(this, *io, isRerun);
	if (info.IsUnknown())
	{
		ZHL::Log("[INFO] [GameStateManagement] skipping GameState write.\n");
		return super(io);
	}

	const std::string& fileName = info.fileName;

	ESSM::detail::SaveData::WriteState state = ESSM::detail::SaveData::WriteGameState();
	if (GetGameStateSlot(this) == GameStateSlot::SAVE_FILE) {
		VirtualGridRooms::WriteGameState(fileName);
	}
	VirtualRoomSetManager::detail::PreWriteGameState(*this, info);

	bool success = super(io);

	ESSM::detail::SaveData::RestoreWrittenStates(state);
	VirtualRoomSetManager::detail::PostWriteGameState(*this, info);

	if (!success)
	{
		return false;
	}

	ESSM::detail::SaveData::Serialize(fileName, state);
	success = write_save(*this, info, isRerun);
	if (!success)
	{
		return false;
	}

	int checksum = g_Manager->_gamestate._checksum;
	LuaSerializeGameState(info, checksum);
	return success;
}

HOOK_METHOD(GameState, write_rerun, (GameStateIO** io) -> bool)
{
	bool isRerun = true;
	GameStateSaveInfo info = get_info(this, *io, isRerun);
	const std::string& fileName = info.fileName;

	ESSM::detail::SaveData::WriteState state = ESSM::detail::SaveData::WriteGameState();
	VirtualGridRooms::WriteGameState(fileName);
	VirtualRoomSetManager::detail::ClearDB(info);

	bool success = super(io);

	ESSM::detail::SaveData::RestoreWrittenStates(state);

	if (!success)
	{
		return false;
	}

	if (info.IsUnknown())
	{
		ZHL::Log("[INFO] [GameStateManagement] skipping GameState write.\n");
		return true;
	}

	ESSM::detail::SaveData::Serialize(info.fileName, state);
	success = write_save(*this, info, isRerun);
	if (!success)
	{
		return false;
	}

	int checksum = g_Manager->_gamestate._checksum;
	LuaSerializeGameState(info, checksum);
	return success;
}

HOOK_METHOD(GameState, read, (GameStateIO** io, bool isLocalRun) -> bool)
{
	bool isRerun = false;
	GameStateSaveInfo info = get_info(this, *io, isRerun);

	bool originalSuccess = super(io, isLocalRun);

	VirtualGridRooms::ReadGameState(info.fileName);
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

	bool needsHandling = ESSM::detail::SaveData::NeedsHandling(essmReadState) ||
		VirtualRoomSetManager::detail::GameStateNeedsHandling(*this);

	if (!isLocalRun) // This occurs when loading a game state upon joining an already existing match.
	{
		ZHL::Log("[INFO] [GameStateManagement] reading non save file GameState, skipping read.\n");
		return !needsHandling;
	}

	if (info.IsUnknown())
	{
		ZHL::Log("[INFO] [GameStateManagement] skipping GameState read.\n");
		return !needsHandling;
	}

	if (!ESSM::detail::SaveData::Deserialize(info.fileName, essmReadState))
	{
		return false;
	}

	success = read_save(*this, info, isRerun);
	if (!success)
	{
		return false;
	}

	int checksum = g_Manager->_gamestate._checksum;
	LuaDeserializeGameState(info, checksum);
	return success;
}

HOOK_METHOD(GameState, read_rerun, (GameStateIO** io) -> bool)
{
	bool isRerun = true;
	GameStateSaveInfo info = get_info(this, *io, isRerun);

	bool originalSuccess = super(io);

	VirtualGridRooms::ReadGameState(info.fileName);
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
	if (info.IsUnknown())
	{
		ZHL::Log("[INFO] [GameStateManagement] skipping GameState read.\n");
		return !needsHandling;
	}

	if (!ESSM::detail::SaveData::Deserialize(info.fileName, essmReadState))
	{
		return false;
	}

	success = read_save(*this, info, isRerun);
	if (!success)
	{
		return false;
	}

	int checksum = g_Manager->_gamestate._checksum;
	LuaDeserializeGameState(info, checksum);
	return success;
}

HOOK_METHOD(GameState, Delete, () -> void)
{
	bool isRerun = false;
	GameStateSaveInfo info = get_info(this, this->_saveFile, false);

	super();

	if (info.IsUnknown())
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown game state, skipping delete.\n");
		return;
	}

	delete_save(info, isRerun);
	LuaGameStateDelete(info);
}

HOOK_METHOD(GameState, DeleteRerun, () -> void)
{
	bool isRerun = true;
	GameStateSaveInfo info = get_info(this, this->_saveFile, false);

	super();

	if (info.IsUnknown())
	{
		ZHL::Log("[INFO] [GameStateManagement] Unknown game state, skipping delete.\n");
		return;
	}

	delete_save(info, isRerun);
	LuaGameStateDelete(info);
}

HOOK_METHOD(Manager, cleanup_current_state, () -> void)
{
    super();
    LuaCleanupState();
}

#pragma endregion