/* FOR MAINTAINERS:
*  - There is no single point where the Level's rooms are reset or partially reset; each case must be found and handled individually.
*  - The main operators (like operator= or the destructor) for EntitySaveState have not been used as hook points, since many temporary states are created, and we do not need to signal their creation, copy or destruction.
*  - Some places manually create and insert EntitySaveStates into a container, without using a constructor; each of these must be found and handled.
*  - Due to the unreliable nature of the EntitySaveState methods, manual work is required when save states are copied or cleared by the game. As such, all points where EntitySaveState is used must be reviewed.
*    - The only exception is SmartPointers, which have a single point of destruction, but their initialization and copy must still be handled manually.
*    - Because the EntitySaveState destructor always attempts to call an unused global Notify function, references to that function's memory address can be used to locate uses of EntitySaveState.
*    - There are unhandled cases where EntitySaveState and GameStatePlayer are used, but all of these are related to online play and have not been addressed.
*  - There is no mechanism that checks if an ID is cleared multiple times. We must ensure this never occurs.
*    - A possible improvement could involve marking cleared EntitySaveStates with a special flag to make detection safer. However, this would not prevent issues caused by duplicate, which are the main cause of multiple clears.
*  - Because of SmartPointers, it is not currently possible to fully clear the ID generator, as some references would persist after reset, causing incorrect clears.
*    - Full handling of SmartPointers would be required to safely enable full resets.
*    - Enabling this would make the system resilient to “memory leaks”, but might still allow incorrect clears.
*  - It is assumed that all Entity saves are handled by Room::save_entity().
*  - It is assumed that every time Room::save_entity() is called with 'isSavingMinecartEntity' set to true, it is immediately followed by another call to Room::save_entity(), with no interruptions.
*  - It is assumed that all Entity restores are handled by Room::restore_entity(), except for Entity_Pickup::TryFlipState.
*  - It is assumed that GameState::SaveState's initial state is cleared.
*  - It is assumed that GameState is in a cleared state when it is read.
*  - It is assumed that every time a GameState is cleared, GameState::Clear() is called.
*  - Confirm that Level::RestoreGameState does not reset the LilPortalRoom when calling reset_room_list, since we currently have to perform that clear manually.
*  - It is assumed that Entity_Player::StoreGameState's initial state is equivalent to an initialized or newly constructed state.
*  - It is assumed that every time a player's state is cleared, GameStatePlayer::Init() is called.
*  - It is assumed that all player and familiar saves are handled by Entity_Player::StoreGameState().
*  - It is assumed that all player restores are handled by Entity_Player::RestoreGameState().
*  - It is assumed that all familiar restores are handled by Entity_Player::RestoreGameState_PostLevelInit().
*  - There is one known case where familiar data is modified manually outside the designated function; ensure all such cases are identified and handled.
*    - This specific case occurs when restoring backup players in PlayerManager::RestoreGameState(), which copies data from the save state into the unlisted save state.
*  - To avoid having to iterate over non "initialized" RoomDescriptors in BackwardsStage, when resetting the game's state, we diverge from the game's behavior by actually clearing the ids.
*    - In contrast, the game keeps the vector around and simply sets the room count to 0, only properly clearing it when saving a new RoomDescriptor.
*/

#include "EntitySaveStateManagement.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"
#include "Log.h"
#include "../MiscFunctions.h"
#include "../LuaInterfaces/LuaEntitySaveState.h"

#include <fstream>
#include <algorithm>

#undef max
#undef ERROR_INVALID_STATE
#define DEBUG_MSG true

#define __LOG_DEBUG_HEADER__ "[DEBUG] [ESSM] - "
#define __LOG_INFO_HEADER__ "[INFO] [ESSM] - "
#define __LOG_ERROR_HEADER__ "[ERROR] [ESSM] - "

namespace {
    struct HijackedState {
        int id = 0;
        int marker = 0;
    };
};

#if DEBUG_MSG
    static void LogDebug(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        std::string message = REPENTOGON::StringFormat(format, args);
        va_end(args);

        KAGE::_LogMessage(0, message.c_str());
    }
#else
    static void LogDebug(const char* format, ...) {};
#endif

#define ESSM EntitySaveStateManagement

namespace ESSM
{
    struct Data
    {
        std::vector<uint32_t> reusableIds;
        uint32_t totalIds = 0;
        std::vector<HijackedState> hijackedStates; 
    };

    class LuaCallbacks
    {
        private: int m_storeEntity = LUA_NOREF;
        private: int m_restoreEntity = LUA_NOREF;
        private: int m_clearSaveStates = LUA_NOREF;
        private: int m_copySaveStates = LUA_NOREF;
        private: int m_serialize = LUA_NOREF;
        private: int m_deserialize = LUA_NOREF;

        public: void BindLuaCallbacks(lua_State* L, int tblIdx);
        public: void StoreEntity(const Entity& entity, uint32_t saveStateId);
        public: void RestoreEntity(const Entity& entity, uint32_t saveStateId);
        public: void ClearStates(const std::vector<uint32_t>& saveStateIds);
        public: void CopyStates(const std::vector<std::pair<uint32_t, uint32_t>> copyIds);
        public: void Serialize(const SaveData::_WriteState& writeState, const std::string& filename, uint32_t checksum);
        public: void Deserialize(const SaveData::_ReadState& readState, const std::string& filename, uint32_t checksum);
    };

    static Data s_systemData;
    static LuaCallbacks s_luaCallbacks;
}

namespace ESSM::Init
{
    void BindLuaCallbacks(lua_State* L, int tblIdx)
    {
        s_luaCallbacks.BindLuaCallbacks(L, tblIdx);
    }
}

constexpr size_t ESAU_JR_PLAYERS = 4;

namespace ESSM::PlayerIterators
{
    namespace InGame
    {
        template <typename Func, typename... Args>
        static void All(Func&& func, Args&&... args)
        {
            Game* game = g_Game;
            // Player data
            const PlayerManager& playerManager = game->_playerManager;
            const auto& players = playerManager._playerList;

            auto handlePlayer = [&](Entity_Player* player)
            {
                if (!player)
                {
                    return;
                }

                Entity_Player* backupPlayer = player->_backupPlayer;
                if (backupPlayer)
                {
                    func(backupPlayer->_unlistedRestoreState, std::forward<Args>(args)...);
                }

                func(player->_unlistedRestoreState, std::forward<Args>(args)...);
            };

            for (Entity_Player* player : players)
            {
                handlePlayer(player);
            }

            for (size_t i = 0; i < 4; i++)
            {
                handlePlayer(playerManager._esauJrState[i]);
            }
        }
    }

    namespace InGameState
    {
        template <typename Func, typename... Args>
        static void All(GameState& gameState, Func&& func, Args&&... args)
        {
            GameStatePlayer* players = gameState._players;
            size_t playerCount = gameState._playerCount;

            for (size_t i = 0; i < playerCount; i++)
            {
                func(players[i], std::forward<Args>(args)...);
            }

            GameStatePlayer* esauJrStates = gameState._esauJrStates;

            for (size_t i = 0; i < ESAU_JR_PLAYERS; i++)
            {
                GameStatePlayer& player = esauJrStates[i];
                if (player._playerType == -1)
                {
                    continue;
                }

                func(player, std::forward<Args>(args)...);
            }
        }
    }
}

namespace ESSM::FamiliarIterators
{
    namespace InGame
    {
        template <typename Func, typename... Args>
        static void All(Func&& func, Args&&... args)
        {
            auto lambda = [&](GameStatePlayer& player) {
                func(player._familiarData, std::forward<Args>(args)...);
            };

            ESSM::PlayerIterators::InGame::All(lambda);
        }
    }

    namespace InGameState
    {
        template <typename Func, typename... Args>
        static void All(GameState& gameState, Func&& func, Args&&... args)
        {
            auto lambda = [&](GameStatePlayer& player) {
                func(player._familiarData, std::forward<Args>(args)...);
            };

            ESSM::PlayerIterators::InGameState::All(gameState, lambda);
        }
    }
}

namespace ESSM::EntityIterators
{
    namespace Structure
    {
        template <typename Func, typename... Args>
        static void BackwardsStage(BackwardsStageDesc& backwardsStage, Func&& func, Args&&... args)
        {
            for (size_t i = 0; i < backwardsStage._bossRoomsCount; i++)
            {
                RoomDescriptor& room = backwardsStage._bossRooms[i];
                func(room.SavedEntities, std::forward<Args>(args)...);
            }

            for (size_t i = 0; i < backwardsStage._treasureRoomsCount; i++)
            {
                RoomDescriptor& room = backwardsStage._treasureRooms[i];
                func(room.SavedEntities, std::forward<Args>(args)...);
            }
        }
    }
    
    namespace InGame
    {
        template <typename Func, typename... Args>
        static void AllRooms(Func&& func, Args&&... args)
        {
            Game* game = g_Game;
            RoomDescriptor* roomList = game->_gridRooms;
        
            for (size_t i = 0; i < MAX_ROOMS; i++)
            {
                func(roomList[i].SavedEntities, std::forward<Args>(args)...);
            }
        }

        template <typename Func, typename... Args>
        static void AllBackwardsStages(Func&& func, Args&&... args)
        {
            Game* game = g_Game;
            BackwardsStageDesc* backwardsStages = game->_backwardsStages;
        
            for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
            {
                BackwardsStageDesc& backwardsStage = backwardsStages[i];
                ESSM::EntityIterators::Structure::BackwardsStage(backwardsStage, func, std::forward<Args>(args)...);
            }
        }

        template <typename Func, typename... Args>
        static void AllPlayers(Func&& func, Args&&... args)
        {
            Game* game = g_Game;
            // Player data
            const PlayerManager& playerManager = game->_playerManager;
            const auto& players = playerManager._playerList;
        
            auto handlePlayer = [&](Entity_Player* player)
            {
                if (!player)
                {
                    return;
                }
        
                func(player->_movingBoxContents, std::forward<Args>(args)...);
        
                Entity_Player* backupPlayer = player->_backupPlayer;
                if (backupPlayer)
                {
                    func(backupPlayer->_movingBoxContents, std::forward<Args>(args)...);
                    func(backupPlayer->_unlistedRestoreState._movingBoxContents, std::forward<Args>(args)...);
                }
        
                func(player->_unlistedRestoreState._movingBoxContents, std::forward<Args>(args)...);
            };
        
            for (Entity_Player* player : players)
            {
                handlePlayer(player);
            }
        
            for (size_t i = 0; i < 4; i++)
            {
                handlePlayer(playerManager._esauJrState[i]);
            }
        }

        template <typename Func, typename... Args>
        static void All(Func&& func, Args&&... args)
        {
            Game* game = g_Game;
            AllRooms(std::forward<Func>(func), std::forward<Args>(args)...);
            func(game->_myosotisPickups, std::forward<Args>(args)...);
            AllBackwardsStages(std::forward<Func>(func), std::forward<Args>(args)...);
            AllPlayers(std::forward<Func>(func), std::forward<Args>(args)...);
        }
    }
    
    namespace InGameState
    {
        template <typename Func, typename... Args>
        static void AllRooms(GameState& gameState, Func&& func, Args&&... args)
        {
            RoomDescriptor* rooms = gameState._rooms;
        
            for (size_t i = 0; i < MAX_ROOMS; i++)
            {
                RoomDescriptor& room = rooms[i];
                func(room.SavedEntities, std::forward<Args>(args)...);
            }
        }

        template <typename Func, typename... Args>
        static void AllBackwardsStages(GameState& gameState, Func&& func, Args&&... args)
        {
            BackwardsStageDesc* backwardsStages = gameState._backwardsStages;
        
            for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
            {
                BackwardsStageDesc& backwardsStage = backwardsStages[i];
                ESSM::EntityIterators::Structure::BackwardsStage(backwardsStage, func, std::forward<Args>(args)...);
            }
        }

        template <typename Func, typename... Args>
        static void AllPlayers(GameState& gameState, Func&& func, Args&&... args)
        {
            GameStatePlayer* players = gameState._players;
            size_t playerCount = gameState._playerCount;
        
            for (size_t i = 0; i < playerCount; i++)
            {
                GameStatePlayer& player = players[i];
                func(player._movingBoxContents, std::forward<Args>(args)...);
            }
        
            GameStatePlayer* esauJrStates = gameState._esauJrStates;
        
            for (size_t i = 0; i < ESAU_JR_PLAYERS; i++)
            {
                GameStatePlayer& player = esauJrStates[i];
                if (player._playerType == -1)
                {
                    continue;
                }
        
                func(player._movingBoxContents, std::forward<Args>(args)...);
            }
        }

        template <typename Func, typename... Args>
        static void All(GameState& gameState, Func&& func, Args&&... args)
        {
            AllRooms(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
            AllBackwardsStages(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
            AllPlayers(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
        }
    }
}

namespace ESSM::IdManager
{
    static uint32_t NewId()
    {
        if (s_systemData.reusableIds.size() > 0)
        {
            uint32_t id = s_systemData.reusableIds.back();
            s_systemData.reusableIds.pop_back();
            LogDebug(__LOG_DEBUG_HEADER__ "New ID: %u\n", id);
            return id;
        }
    
        uint32_t id = s_systemData.totalIds;
        s_systemData.totalIds++;
        s_systemData.hijackedStates.emplace_back();
        LogDebug(__LOG_DEBUG_HEADER__ "New ID: %u\n", id);
        return id;
    }

    static void ClearId(uint32_t id)
    {
        s_systemData.reusableIds.push_back(id);
        LogDebug(__LOG_DEBUG_HEADER__ "Cleared ID: %u\n", id);
    }
}

namespace ESSM::EntityHijackManager
{
    constexpr short READ_MARKER = 0x5248;
    constexpr short HIJACK_MARKER = 0x5247;
    constexpr short WRITTEN_MARKER = 0x5246;

    const short& get_marker_value(const EntitySaveState& data)
    {
        return *(short*)&data.gridSpawnIdx;
    }
    
    const uint32_t& get_id_value(const EntitySaveState& data)
    {
        return *(uint32_t*)&data._intStorage7;
    }

    // only used if unable to get a HijackedState
    static void DefaultRestore(EntitySaveState& data)
    {
        data.gridSpawnIdx = (short)-1;
        data._intStorage7 = 0;
    }
    
    short& get_marker_value(EntitySaveState& data)
    { 
        return const_cast<short&>(get_marker_value(std::as_const(data)));
    }

    uint32_t& get_id_value(EntitySaveState& data)
    { 
        return const_cast<uint32_t&>(get_id_value(std::as_const(data)));
    }

    static bool IsHijacked(const EntitySaveState& data)
    {
        return get_marker_value(data) == HIJACK_MARKER;
    }
    
    static bool IsWritten(const EntitySaveState& data)
    {
        return get_marker_value(data) == WRITTEN_MARKER;
    }
    
    static bool IsRead(const EntitySaveState& data)
    {
        return get_marker_value(data) == READ_MARKER;
    }
    
    static bool HasMarker(const EntitySaveState& data)
    {
        return IsHijacked(data) || IsWritten(data) || IsRead(data);
    }

    static void SetHijacked(EntitySaveState& data)
    {
        get_marker_value(data) = HIJACK_MARKER;
    }
    
    static void SetWritten(EntitySaveState& data)
    {
        get_marker_value(data) = WRITTEN_MARKER;
    }
    
    static void SetRead(EntitySaveState& data)
    {
        get_marker_value(data) = READ_MARKER;
    }
    
    static uint32_t GetId(const EntitySaveState& data)
    {
        return get_id_value(data);
    }
    
    static void SetId(EntitySaveState& data, uint32_t id)
    {
        get_id_value(data) = id;
    }
    
    static void Hijack(EntitySaveState& data, uint32_t id)
    {
        HijackedState& hijackedState = s_systemData.hijackedStates[id];
        hijackedState.marker = get_marker_value(data);
        hijackedState.id = get_id_value(data);
        SetHijacked(data);
        SetId(data, id);
    }
    
    static uint32_t NewHijack(EntitySaveState& data)
    {
        assert(!IsHijacked(data));
        uint32_t id = ESSM::IdManager::NewId();
        LogDebug(__LOG_DEBUG_HEADER__ "Saved %d, %d, %d\n", data.type, data.variant, data.subtype);
        Hijack(data, id);
        return id;
    }

    static void RestoreHijack(EntitySaveState& data, HijackedState& hijackedState)
    {
        get_marker_value(data) = hijackedState.marker;
        get_id_value(data) = hijackedState.id;
    }
    
    static uint32_t UnHijack(EntitySaveState& data)
    {
        assert(IsHijacked(data));
        uint32_t id = GetId(data);
        RestoreHijack(data, s_systemData.hijackedStates[id]);
        return id;
    }
}

namespace ESSM::PlayerHijackManager
{
    constexpr short CLEARED_MARKER = 0x5249;
    constexpr short READ_MARKER = 0x5248;
    constexpr short HIJACK_MARKER = 0x5247;
    constexpr short WRITTEN_MARKER = 0x5246;

    const uint32_t& get_marker_value(const GameStatePlayer& data)
    {
        return *(uint32_t*)&data._immaculateConceptionState;
    }
    
    const uint32_t& get_id_value(const GameStatePlayer& data)
    {
        return *(uint32_t*)&data._cambionConceptionState;
    }

    // only used if unable to get a HijackedState
    static void DefaultRestore(GameStatePlayer& data)
    {
        data._immaculateConceptionState = 0;
        data._cambionConceptionState = 0;
    }
    
    uint32_t& get_marker_value(GameStatePlayer& data)
    { 
        return const_cast<uint32_t&>(get_marker_value(std::as_const(data)));
    }

    uint32_t& get_id_value(GameStatePlayer& data)
    { 
        return const_cast<uint32_t&>(get_id_value(std::as_const(data)));
    }

    static bool IsHijacked(const GameStatePlayer& data)
    {
        return get_marker_value(data) == HIJACK_MARKER;
    }
    
    static bool IsWritten(const GameStatePlayer& data)
    {
        return get_marker_value(data) == WRITTEN_MARKER;
    }
    
    static bool IsRead(const GameStatePlayer& data)
    {
        return get_marker_value(data) == READ_MARKER;
    }

    static bool IsCleared(const GameStatePlayer& data)
    {
        return get_marker_value(data) == CLEARED_MARKER;
    }
    
    static bool HasMarker(const GameStatePlayer& data)
    {
        return IsHijacked(data) || IsWritten(data) || IsRead(data) || IsCleared(data);
    }

    static void SetHijacked(GameStatePlayer& data)
    {
        get_marker_value(data) = HIJACK_MARKER;
    }
    
    static void SetWritten(GameStatePlayer& data)
    {
        get_marker_value(data) = WRITTEN_MARKER;
    }
    
    static void SetRead(GameStatePlayer& data)
    {
        get_marker_value(data) = READ_MARKER;
    }

    static void SetCleared(GameStatePlayer& data)
    {
        get_marker_value(data) = CLEARED_MARKER;
    }
    
    static uint32_t GetId(const GameStatePlayer& data)
    {
        return get_id_value(data);
    }
    
    static void SetId(GameStatePlayer& data, uint32_t id)
    {
        get_id_value(data) = id;
    }
    
    static void Hijack(GameStatePlayer& data, uint32_t id)
    {
        HijackedState& hijackedState = s_systemData.hijackedStates[id];
        hijackedState.marker = get_marker_value(data);
        hijackedState.id = get_id_value(data);
        SetHijacked(data);
        SetId(data, id);
    }
    
    static uint32_t NewHijack(GameStatePlayer& data)
    {
        assert(!IsHijacked(data));
        uint32_t id = ESSM::IdManager::NewId();
        LogDebug(__LOG_DEBUG_HEADER__ "Player Saved %d\n", data._playerType);
        Hijack(data, id);
        return id;
    }

    static void RestoreHijack(GameStatePlayer& data, HijackedState& hijackedState)
    {
        get_marker_value(data) = hijackedState.marker;
        get_id_value(data) = hijackedState.id;
    }
    
    static uint32_t UnHijack(GameStatePlayer& data)
    {
        assert(IsHijacked(data));
        uint32_t id = GetId(data);
        RestoreHijack(data, s_systemData.hijackedStates[id]);
        return id;
    }
}

namespace ESSM::FamiliarHijackManager
{
    constexpr short READ_MARKER = 0x5248;
    constexpr short HIJACK_MARKER = 0x5247;
    constexpr short WRITTEN_MARKER = 0x5246;

    const uint32_t& get_marker_value(const FamiliarData& data)
    {
        return *(uint32_t*)&data._state;
    }
    
    const uint32_t& get_id_value(const FamiliarData& data)
    {
        return *(uint32_t*)&data._roomClearCount;
    }

    // only used if unable to get a HijackedState
    static void DefaultRestore(FamiliarData& data)
    {
        data._state = 0;
        data._roomClearCount = 0;
    }
    
    uint32_t& get_marker_value(FamiliarData& data)
    { 
        return const_cast<uint32_t&>(get_marker_value(std::as_const(data)));
    }

    uint32_t& get_id_value(FamiliarData& data)
    { 
        return const_cast<uint32_t&>(get_id_value(std::as_const(data)));
    }

    static bool IsHijacked(const FamiliarData& data)
    {
        return get_marker_value(data) == HIJACK_MARKER;
    }
    
    static bool IsWritten(const FamiliarData& data)
    {
        return get_marker_value(data) == WRITTEN_MARKER;
    }
    
    static bool IsRead(const FamiliarData& data)
    {
        return get_marker_value(data) == READ_MARKER;
    }
    
    static bool HasMarker(const FamiliarData& data)
    {
        return IsHijacked(data) || IsWritten(data) || IsRead(data);
    }

    static void SetHijacked(FamiliarData& data)
    {
        get_marker_value(data) = HIJACK_MARKER;
    }
    
    static void SetWritten(FamiliarData& data)
    {
        get_marker_value(data) = WRITTEN_MARKER;
    }
    
    static void SetRead(FamiliarData& data)
    {
        get_marker_value(data) = READ_MARKER;
    }
    
    static uint32_t GetId(const FamiliarData& data)
    {
        return get_id_value(data);
    }
    
    static void SetId(FamiliarData& data, uint32_t id)
    {
        get_id_value(data) = id;
    }
    
    static void Hijack(FamiliarData& data, uint32_t id)
    {
        HijackedState& hijackedState = s_systemData.hijackedStates[id];
        hijackedState.marker = get_marker_value(data);
        hijackedState.id = get_id_value(data);
        SetHijacked(data);
        SetId(data, id);
    }
    
    static uint32_t NewHijack(FamiliarData& data)
    {
        assert(!IsHijacked(data));
        uint32_t id = ESSM::IdManager::NewId();
        LogDebug(__LOG_DEBUG_HEADER__ "Familiar Saved %d, %d\n", data._variant, data._subtype);
        Hijack(data, id);
        return id;
    }

    static void RestoreHijack(FamiliarData& data, HijackedState& hijackedState)
    {
        get_marker_value(data) = hijackedState.marker;
        get_id_value(data) = hijackedState.id;
    }
    
    static uint32_t UnHijack(FamiliarData& data)
    {
        assert(IsHijacked(data));
        uint32_t id = GetId(data);
        RestoreHijack(data, s_systemData.hijackedStates[id]);
        return id;
    }
}

// Lua Callbacks
namespace ESSM
{
    void LuaCallbacks::BindLuaCallbacks(lua_State* L, int tblIdx)
    {
        assert(L == g_LuaEngine->_state);
    
        tblIdx = lua_absindex(L, tblIdx);
        if (!lua_istable(L, tblIdx))
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "ESSM::BindLuaCallbacks: expected a table\n");
            assert(false);
            return;
        }
    
        luaL_unref(L, LUA_REGISTRYINDEX, s_luaCallbacks.m_storeEntity);
        luaL_unref(L, LUA_REGISTRYINDEX, s_luaCallbacks.m_restoreEntity);
        luaL_unref(L, LUA_REGISTRYINDEX, s_luaCallbacks.m_clearSaveStates);
        luaL_unref(L, LUA_REGISTRYINDEX, s_luaCallbacks.m_copySaveStates);
        luaL_unref(L, LUA_REGISTRYINDEX, s_luaCallbacks.m_serialize);
        luaL_unref(L, LUA_REGISTRYINDEX, s_luaCallbacks.m_deserialize);
    
        s_luaCallbacks.m_storeEntity = LUA_NOREF; 
        s_luaCallbacks.m_restoreEntity = LUA_NOREF; 
        s_luaCallbacks.m_clearSaveStates = LUA_NOREF; 
        s_luaCallbacks.m_copySaveStates = LUA_NOREF; 
        s_luaCallbacks.m_serialize = LUA_NOREF; 
        s_luaCallbacks.m_deserialize = LUA_NOREF;
    
        auto bindCallback = [](lua_State* L, int tblIdx, const char* fieldName, int& outRef)
        {
            lua_getfield(L, tblIdx, fieldName);
            if (!lua_isfunction(L, -1))
            {
                ZHL::Log(__LOG_ERROR_HEADER__ "ESSM::BindLuaCallbacks: Expected '%s' to be a function\n", fieldName);
                lua_pop(L, 1);
                assert(false);
                return;
            }
    
            outRef = luaL_ref(L, LUA_REGISTRYINDEX);
        };
    
        bindCallback(L, tblIdx, "StoreEntity", s_luaCallbacks.m_storeEntity);
        bindCallback(L, tblIdx, "RestoreEntity", s_luaCallbacks.m_restoreEntity);
        bindCallback(L, tblIdx, "ClearStates", s_luaCallbacks.m_clearSaveStates);
        bindCallback(L, tblIdx, "CopyStates", s_luaCallbacks.m_copySaveStates);
        bindCallback(L, tblIdx, "Serialize", s_luaCallbacks.m_serialize);
        bindCallback(L, tblIdx, "Deserialize", s_luaCallbacks.m_deserialize);
    }
    
    void LuaCallbacks::StoreEntity(const Entity& entity, uint32_t saveStateId)
    {
        LuaEngine* lua = g_LuaEngine;
        lua_State* L = lua->_state;
    
        uint32_t entityId = lua->GetMaskedPointer((uintptr_t)&entity);
    
        lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_storeEntity);
        lua_pushinteger(L, entityId);
        lua_pushinteger(L, saveStateId);
    
        if (lua_pcall(L, 2, 0, 0) != LUA_OK)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "LuaCallbacks::StoreEntity: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    
    void LuaCallbacks::RestoreEntity(const Entity& entity, uint32_t saveStateId)
    {
        LuaEngine* lua = g_LuaEngine;
        lua_State* L = lua->_state;
    
        uint32_t entityId = lua->GetMaskedPointer((uintptr_t)&entity);
    
        lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_restoreEntity);
        lua_pushinteger(L, entityId);
        lua_pushinteger(L, saveStateId);
    
        if (lua_pcall(L, 2, 0, 0) != LUA_OK)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "LuaCallbacks::RestoreEntity: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    
    void LuaCallbacks::ClearStates(const std::vector<uint32_t>& saveStateIds)
    {
        lua_State* L = g_LuaEngine->_state;
    
        lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_clearSaveStates);
    
        // push_vector
        size_t size = saveStateIds.size();
        lua_createtable(L, size, 0);
        for (size_t i = 0; i < size; i++)
        {
            lua_pushinteger(L, saveStateIds[i]);
            lua_rawseti(L, -2, i + 1);
        }
    
        if (lua_pcall(L, 1, 0, 0) != LUA_OK)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "LuaCallbacks::ClearStates: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    
    void LuaCallbacks::CopyStates(const std::vector<std::pair<uint32_t, uint32_t>> copyIds)
    {
        lua_State* L = g_LuaEngine->_state;
    
        lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_copySaveStates);
    
        // push sourceIds and destIds
        size_t size = copyIds.size();
        lua_createtable(L, size, 0); // sourceIds
        lua_createtable(L, size, 0); // destIds
    
        int sourceIds = lua_absindex(L, -2);
        int destIds = lua_absindex(L, -1);
    
        for (size_t i = 0; i < size; ++i)
        {
            const auto& copyId = copyIds[i];
            lua_pushinteger(L, copyId.first);
            lua_rawseti(L, sourceIds, i + 1);
    
            lua_pushinteger(L, copyId.second);
            lua_rawseti(L, destIds, i + 1);
        }
    
        if (lua_pcall(L, 2, 0, 0) != LUA_OK)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "LuaCallbacks::CopyStates: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    
    void LuaCallbacks::Serialize(const SaveData::_WriteState& writeState, const std::string& filename, uint32_t checksum)
    {
        lua_State* L = g_LuaEngine->_state;

        const auto& entityStates = writeState.writeEntityIdPairs;
        
        lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_serialize);

        /* ASSUMPTION: All written ids are unique.
        *  This should, currently, always be true as all ids are based on the size of the vector before push.
        */

        // push IdMap
        size_t size = entityStates.size();
        lua_createtable(L, size, 0);
        int idMap = lua_absindex(L, -1);

        for (size_t i = 0; i < entityStates.size(); ++i)
        {
            const auto& id = entityStates[i];
            lua_pushinteger(L, id.second);
            lua_rawseti(L, idMap, id.first + 1);
        }

        lua_pushlstring(L, filename.c_str(), filename.size());
        lua_pushinteger(L, checksum);

        if (lua_pcall(L, 3, 0, 0) != LUA_OK)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "LuaCallbacks::Serialize: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    
    void LuaCallbacks::Deserialize(const SaveData::_ReadState& readState, const std::string& filename, uint32_t checksum)
    {
        lua_State* L = g_LuaEngine->_state;

        const auto& entityStates = readState.restoreEntityIdPairs;

        lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_deserialize);

        // push serializedIds and destIds
        size_t size = entityStates.size();
        lua_createtable(L, size, 0); // serializedIds
        lua_createtable(L, size, 0); // destIds

        int serializedIds = lua_absindex(L, -2);
        int destIds = lua_absindex(L, -1);

        for (size_t i = 0; i < entityStates.size(); ++i)
        {
            const auto& id = entityStates[i];
            lua_pushinteger(L, id.first);
            lua_rawseti(L, serializedIds, i + 1);

            lua_pushinteger(L, id.second);
            lua_rawseti(L, destIds, i + 1);
        }

        if (lua_pcall(L, 4, 0, 0) != LUA_OK)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "LuaCallbacks::Deserialize: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
}

// These are base operations, signals should not be sent from here
namespace ESSM::PlayerOperations
{
    static void ClearSaveState(GameStatePlayer& saveState)
    {
        assert(PlayerHijackManager::IsHijacked(saveState));
        uint32_t id = PlayerHijackManager::GetId(saveState);
        IdManager::ClearId(id);
        // TODO: Callers should send signals
    }

    static void CopySaveState(GameStatePlayer& saveState)
    {
        assert(PlayerHijackManager::IsHijacked(saveState));
        uint32_t sourceId = PlayerHijackManager::GetId(saveState);
        uint32_t targetId = IdManager::NewId();
        PlayerHijackManager::SetId(saveState, targetId);
        s_systemData.hijackedStates[targetId] = s_systemData.hijackedStates[sourceId];
        LogDebug(__LOG_DEBUG_HEADER__ "Copied Player %d -> %d\n", sourceId, targetId);
        // TODO: Callers should send signals
    }
}

namespace ESSM::FamiliarOperations
{
    static void ClearSaveState(FamiliarData& saveState)
    {
        assert(FamiliarHijackManager::IsHijacked(saveState));
        uint32_t id = FamiliarHijackManager::GetId(saveState);
        IdManager::ClearId(id);
        // TODO: Callers should send signals
    }
    
    static void ClearVector(std::vector<FamiliarData>& saveEntities)
    {
        for (FamiliarData& saveState : saveEntities)
        {
            ClearSaveState(saveState);
        }
    }

    static void CopySaveState(FamiliarData& saveState)
    {
        assert(FamiliarHijackManager::IsHijacked(saveState));
        uint32_t sourceId = FamiliarHijackManager::GetId(saveState);
        uint32_t targetId = IdManager::NewId();
        FamiliarHijackManager::SetId(saveState, targetId);
        s_systemData.hijackedStates[targetId] = s_systemData.hijackedStates[sourceId];
        LogDebug(__LOG_DEBUG_HEADER__ "Copied Familiar %d -> %d\n", sourceId, targetId);
        // TODO: Callers should send signals
    }

    static void CopyVector(std::vector<FamiliarData>& saveEntities)
    {
        for (FamiliarData& saveState : saveEntities)
        {
            CopySaveState(saveState);
        }
    }

    static auto ClearVectorLambda = [](std::vector<FamiliarData>& vec) { ClearVector(vec); };
    static auto CopyVectorLambda = [](std::vector<FamiliarData>& vec) { CopyVector(vec); };
}

namespace ESSM::EntityOperations
{
    static void ClearSaveState(EntitySaveState& saveState)
    {
        assert(EntityHijackManager::IsHijacked(saveState));
        uint32_t id = EntityHijackManager::GetId(saveState);
        IdManager::ClearId(id);
        // TODO: Callers should send signals
    }
    
    static void ClearVector(std::vector<EntitySaveState>& saveEntities)
    {
        for (EntitySaveState& saveState : saveEntities)
        {
            ClearSaveState(saveState);
        }
    }

    static void CopySaveState(EntitySaveState& saveState)
    {
        assert(EntityHijackManager::IsHijacked(saveState));
        uint32_t sourceId = EntityHijackManager::GetId(saveState);
        uint32_t targetId = IdManager::NewId();
        EntityHijackManager::SetId(saveState, targetId);
        s_systemData.hijackedStates[targetId] = s_systemData.hijackedStates[sourceId];
        LogDebug(__LOG_DEBUG_HEADER__ "Copied Entity %d -> %d\n", sourceId, targetId);
        // TODO: Callers should send signals
    }

    static void CopyVector(std::vector<EntitySaveState>& saveEntities)
    {
        for (EntitySaveState& saveState : saveEntities)
        {
            CopySaveState(saveState);
        }
    }

    static auto ClearVectorLambda = [](std::vector<EntitySaveState>& vec) { ClearVector(vec); };
    static auto CopyVectorLambda = [](std::vector<EntitySaveState>& vec) { CopyVector(vec); };
}

namespace ESSM
{
    class StabilityChecker
    {
        private: enum eErrors
        {
            ERROR_DOUBLE_CLEAR = 0,
            ERROR_INVALID_STATE = 1,
            ERROR_INVALID_ID = 2,
            ERROR_DUPLICATE = 3,
            ERROR_INCORRECT_CLEAR = 4,
            ERROR_LEAK = 5,

            NUM_ERRORS
        };

        private: struct InvalidEntities
        {
            private: std::vector<EntitySaveState*> m_invalidStates;
            private: std::vector<EntitySaveState*> m_invalidId;
            private: std::vector<EntitySaveState*> m_duplicates;
            private: std::vector<EntitySaveState*> m_incorrectClears;

            friend StabilityChecker;
        };

        private: struct InvalidPlayers
        {
            private: std::vector<GameStatePlayer*> m_invalidStates;
            private: std::vector<GameStatePlayer*> m_invalidId;
            private: std::vector<GameStatePlayer*> m_duplicates;
            private: std::vector<GameStatePlayer*> m_incorrectClears;

            friend StabilityChecker;
        };

        private: struct InvalidFamiliars
        {
            private: std::vector<FamiliarData*> m_invalidStates;
            private: std::vector<FamiliarData*> m_invalidId;
            private: std::vector<FamiliarData*> m_duplicates;
            private: std::vector<FamiliarData*> m_incorrectClears;

            friend StabilityChecker;
        };

        private: struct CheckData
        {
            private: std::vector<uintptr_t> m_checks;
            private: InvalidEntities m_invalidEntities;
            private: InvalidPlayers m_invalidPlayers;
            private: InvalidFamiliars m_invalidFamiliars;
            private: std::vector<uint32_t> m_leakedIds;
            private: std::bitset<NUM_ERRORS> m_errors;

            friend StabilityChecker;
        };

        private: static void check_entity_save_state(CheckData& checks, EntitySaveState& saveState)
        {
            InvalidEntities& invalidEntities = checks.m_invalidEntities;
            bool validState = EntityHijackManager::IsHijacked(saveState);
            assert(validState); // written and read states are treated as invalid, because they should only be marked as such in an isolated moment.
            if (!validState)
            {
                invalidEntities.m_invalidStates.emplace_back(&saveState);
                checks.m_errors.set(eErrors::ERROR_INVALID_STATE, true);
                return;
            }

            uint32_t id = EntityHijackManager::GetId(saveState);
            bool validId = id < s_systemData.totalIds;
            assert(validId); // this is really messed up
            if (!validId)
            {
                invalidEntities.m_invalidId.emplace_back(&saveState);
                checks.m_errors.set(eErrors::ERROR_INVALID_ID, true);
                return;
            }

            uintptr_t checkPtr = checks.m_checks[id];
            uintptr_t saveStatePtr = (uintptr_t)&saveState;
            if (checkPtr == saveStatePtr) // because of smartPointers we might visit the same state twice, so this is not an error
            {
                return;
            }

            bool isUnique = checkPtr == 0;
            assert(isUnique); // there is an unaccounted duplicate
            if (!isUnique)
            {
                if (checkPtr == 1) // is a cleared id
                {
                    invalidEntities.m_incorrectClears.emplace_back(&saveState);
                    checks.m_errors.set(eErrors::ERROR_INCORRECT_CLEAR, true);
                }
                else
                {
                    invalidEntities.m_duplicates.emplace_back(&saveState);
                    checks.m_errors.set(eErrors::ERROR_DUPLICATE, true);
                }
                return;
            }

            checks.m_checks[id] = saveStatePtr;

            EntitySaveState* innerSaveState = saveState.entitySaveState.saveState;
            if (innerSaveState)
            {
                check_entity_save_state(checks, *innerSaveState);
            }
        }

        private: static void check_player_save_state(CheckData& checks, GameStatePlayer& saveState)
        {
            InvalidPlayers& invalidPlayers = checks.m_invalidPlayers;
            bool cleared = PlayerHijackManager::IsCleared(saveState);
            bool validState = cleared || PlayerHijackManager::IsHijacked(saveState);
            assert(validState); // written and read states are treated as invalid, because they should only be marked as such in an isolated moment.
            if (!validState)
            {
                invalidPlayers.m_invalidStates.emplace_back(&saveState);
                checks.m_errors.set(eErrors::ERROR_INVALID_STATE, true);
                return;
            }

            if (cleared)
            {
                return;
            }

            uint32_t id = PlayerHijackManager::GetId(saveState);
            bool validId = id < s_systemData.totalIds;
            assert(validId); // this is really messed up
            if (!validId)
            {
                invalidPlayers.m_invalidId.emplace_back(&saveState);
                checks.m_errors.set(eErrors::ERROR_INVALID_ID, true);
                return;
            }

            uintptr_t checkPtr = checks.m_checks[id];
            uintptr_t saveStatePtr = (uintptr_t)&saveState;
            if (checkPtr == saveStatePtr) // we should never iterate over them twice, but this would be a game's issue not our own, unless the iterators themselves are broken
            {
                return;
            }

            bool isUnique = checkPtr == 0;
            assert(isUnique); // there is an unaccounted duplicate
            if (!isUnique)
            {
                if (checkPtr == 1) // is a cleared id
                {
                    invalidPlayers.m_incorrectClears.emplace_back(&saveState);
                    checks.m_errors.set(eErrors::ERROR_INCORRECT_CLEAR, true);
                }
                else
                {
                    invalidPlayers.m_duplicates.emplace_back(&saveState);
                    checks.m_errors.set(eErrors::ERROR_DUPLICATE, true);
                }
                return;
            }

            checks.m_checks[id] = saveStatePtr;
        }

        private: static void check_familiar_data(CheckData& checks, FamiliarData& saveState)
        {
            InvalidFamiliars& invalidFamiliars = checks.m_invalidFamiliars;
            bool validState = FamiliarHijackManager::IsHijacked(saveState);
            assert(validState); // written and read states are treated as invalid, because they should only be marked as such in an isolated moment.
            if (!validState)
            {
                invalidFamiliars.m_invalidStates.emplace_back(&saveState);
                checks.m_errors.set(eErrors::ERROR_INVALID_STATE, true);
                return;
            }

            uint32_t id = FamiliarHijackManager::GetId(saveState);
            bool validId = id < s_systemData.totalIds;
            assert(validId); // this is really messed up
            if (!validId)
            {
                invalidFamiliars.m_invalidId.emplace_back(&saveState);
                checks.m_errors.set(eErrors::ERROR_INVALID_ID, true);
                return;
            }

            uintptr_t checkPtr = checks.m_checks[id];
            uintptr_t saveStatePtr = (uintptr_t)&saveState;
            if (checkPtr == saveStatePtr) // we should never iterate over them twice, but this would be a game's issue not our own, unless the iterators themselves are broken
            {
                return;
            }

            bool isUnique = checkPtr == 0;
            assert(isUnique); // there is an unaccounted duplicate
            if (!isUnique)
            {
                if (checkPtr == 1) // is a cleared id
                {
                    invalidFamiliars.m_incorrectClears.emplace_back(&saveState);
                    checks.m_errors.set(eErrors::ERROR_INCORRECT_CLEAR, true);
                }
                else
                {
                    invalidFamiliars.m_duplicates.emplace_back(&saveState);
                    checks.m_errors.set(eErrors::ERROR_DUPLICATE, true);
                }
                return;
            }

            checks.m_checks[id] = saveStatePtr;
        }

        private: static void check_entity_save_state_vector(CheckData& checks, std::vector<EntitySaveState>& saveStates)
        {
            for (EntitySaveState& saveState : saveStates)
            {
                check_entity_save_state(checks, saveState);
            }
        }

        private: static void check_familiar_data_vector(CheckData& checks, std::vector<FamiliarData>& saveStates)
        {
            for (FamiliarData& saveState : saveStates)
            {
                check_familiar_data(checks, saveState);
            }
        }

        private: static constexpr auto check_player_lambda = [](GameStatePlayer& state, CheckData& checks) { check_player_save_state(checks, state); };
        private: static constexpr auto check_familiar_vector_lambda = [](std::vector<FamiliarData>& vec, CheckData& checks) { check_familiar_data_vector(checks, vec); };
        private: static constexpr auto check_entity_vector_lambda = [](std::vector<EntitySaveState>& vec, CheckData& checks) { check_entity_save_state_vector(checks, vec); };

        private: static constexpr size_t MAX_PICKUPS = 512;
        private: static void check_entity_list_stability(CheckData& checks)
        {
            const EntityFactory* factory = g_Game->_entityFactory;
            const Entity_Pickup* pickups = factory->_pickup;

            for (size_t i = 0; i < MAX_PICKUPS; i++)
            {
                const Entity_Pickup& pickup = pickups[i];
                EntitySaveState* flipState = pickup._flipSaveState.saveState;
                if (flipState)
                {
                    check_entity_save_state(checks, *flipState);
                }
            }
        }

        private: static void check_run_stability(CheckData& checks)
        {
            ESSM::PlayerIterators::InGame::All(check_player_lambda, checks);
            ESSM::FamiliarIterators::InGame::All(check_familiar_vector_lambda, checks);
            ESSM::EntityIterators::InGame::All(check_entity_vector_lambda, checks);
        }

        private: static void check_game_state_stability(GameState& gameState, CheckData& checks)
        {
            ESSM::PlayerIterators::InGameState::All(gameState, check_player_lambda, checks);
            ESSM::FamiliarIterators::InGameState::All(gameState, check_familiar_vector_lambda, checks);
            ESSM::EntityIterators::InGameState::All(gameState, check_entity_vector_lambda, checks);
        }

        private: static void handle_errors(CheckData& checks)
        {
            InvalidPlayers& players = checks.m_invalidPlayers;
            InvalidFamiliars& familiars = checks.m_invalidFamiliars;
            InvalidEntities& entities = checks.m_invalidEntities;

            // TODO: print error message for console
            if (checks.m_errors.test(eErrors::ERROR_DOUBLE_CLEAR))
            {
                // double clears are handled during iterations
            }

            if (checks.m_errors.test(eErrors::ERROR_INVALID_STATE))
            {
                for (auto* state : players.m_invalidStates)
                {
                    if (PlayerHijackManager::HasMarker(*state))
                    {
                        PlayerHijackManager::DefaultRestore(*state);
                    }
                    PlayerHijackManager::NewHijack(*state);
                }
    
                for (auto* state : familiars.m_invalidStates)
                {
                    if (FamiliarHijackManager::HasMarker(*state))
                    {
                        FamiliarHijackManager::DefaultRestore(*state);
                    }
                    FamiliarHijackManager::NewHijack(*state);
                }
    
                for (auto* state : entities.m_invalidStates)
                {
                    if (EntityHijackManager::HasMarker(*state))
                    {
                        EntityHijackManager::DefaultRestore(*state);
                    }
                    EntityHijackManager::NewHijack(*state);
                }
            }

            if (checks.m_errors.test(eErrors::ERROR_INVALID_ID))
            {
                // Original Data is lost forever, there is no way to get it back
                for (auto* state : players.m_invalidId)
                {
                    PlayerHijackManager::DefaultRestore(*state);
                    PlayerHijackManager::NewHijack(*state);
                }

                for (auto* state : familiars.m_invalidId)
                {
                    FamiliarHijackManager::DefaultRestore(*state);
                    FamiliarHijackManager::NewHijack(*state);
                }

                for (auto* state : entities.m_invalidId)
                {
                    EntityHijackManager::DefaultRestore(*state);
                    EntityHijackManager::NewHijack(*state);
                }
            }

            if (checks.m_errors.test(eErrors::ERROR_INCORRECT_CLEAR))
            {
                // since the id is not reused we just need to copy
                // should not send a copy signal since a clear was issued on that id.
                for (auto* state : players.m_incorrectClears)
                {
                    PlayerOperations::CopySaveState(*state);
                }

                for (auto* state : familiars.m_incorrectClears)
                {
                    FamiliarOperations::CopySaveState(*state);
                }

                for (auto* state : entities.m_incorrectClears)
                {
                    EntityOperations::CopySaveState(*state);
                }
            }

            if (checks.m_errors.test(eErrors::ERROR_DUPLICATE))
            {
                // TODO: Send copy signals
                for (auto* state : players.m_incorrectClears)
                {
                    PlayerOperations::CopySaveState(*state);
                }

                for (auto* state : familiars.m_incorrectClears)
                {
                    FamiliarOperations::CopySaveState(*state);
                }

                for (auto* state : entities.m_incorrectClears)
                {
                    EntityOperations::CopySaveState(*state);
                }
            }

            if (checks.m_errors.test(eErrors::ERROR_LEAK))
            {
                // TODO: Send clear signal
                for (uint32_t id : checks.m_leakedIds)
                {
                    IdManager::ClearId(id);
                }
            }
        }

        public: static void CheckStability()
        {
            CheckData checks = CheckData();
            checks.m_checks.resize(s_systemData.totalIds, 0);

            std::vector<uint32_t>& reusableIds = s_systemData.reusableIds;
            for (size_t i = reusableIds.size(); i != 0; i--)
            {
                size_t it = i - 1;
                uint32_t reusableId = reusableIds[it];

                bool doubleClear = checks.m_checks[reusableId] != 0;
                assert(!doubleClear); // An id was cleared twice
                if (doubleClear)
                {
                    reusableIds[it] = reusableIds.back();
                    reusableIds.pop_back();
                    checks.m_errors.set(eErrors::ERROR_DOUBLE_CLEAR, true);
                }

                checks.m_checks[reusableId] = 1; // using 1 as it can't be a valid pointer
            }

            Manager* manager = g_Manager;
            Game* game = g_Game;

            check_entity_list_stability(checks);
            check_run_stability(checks);

            check_game_state_stability(manager->_gamestate, checks);
            check_game_state_stability(game->_glowingHourglassStates[0]._gameState, checks);
            check_game_state_stability(game->_glowingHourglassStates[1]._gameState, checks);

            size_t checksCount = checks.m_checks.size();
            for (size_t i = 0; i < checksCount; i++)
            {
                uintptr_t check = checks.m_checks[i];
                bool leaked = check == 0;
                assert(!leaked); // one of the ids was not accounted for (Usually caused by missed Clear)
                if (leaked)
                {
                    checks.m_leakedIds.push_back(i);
                    checks.m_errors.set(eErrors::ERROR_LEAK, true);
                }
            }

            handle_errors(checks);
        }
    };
}

static void SaveEntity(Entity& entity, EntitySaveState& data, uint32_t id)
{
    // TODO: save entity
}

static void RestoreEntity(Entity& entity, EntitySaveState& data, uint32_t id)
{
    // TODO: restore entity
}

namespace ESSM::SaveData
{
    constexpr uint32_t SAVE_VERSION = 1;

    struct SaveFile
    {
        std::vector<HijackedState> entities;
        std::vector<HijackedState> players;
        std::vector<HijackedState> familiars;
    };

    namespace Section
    {
        constexpr uint32_t ENTITY_START = 0x73454E54; // sENT
        constexpr uint32_t ENTITY_END = 0x65454E54; // eENT
        constexpr uint32_t ENTITY_SIZE = 4 + 4;
        constexpr uint32_t MAX_ENTITY_COUNT = 0x1FFFF;
    }

    enum ReadErrors
    {
        ERROR_INVALID_STATE = 0,
        ERROR_DUPLICATE_ID = 1,
        TRIGGERED_EXCEPTION = 2,

        NUM_ERRORS
    };

    template<typename T> struct bitset_size;
    template<std::size_t N> struct bitset_size<std::bitset<N>> {
        static constexpr std::size_t value = N;
    };

    static_assert(bitset_size<decltype(_ReadState::errors)>::value == NUM_ERRORS, "NUM_ERRORS doesn't match _ReadState errors");

    namespace Operations
    {
        static void ClearEntitySaveVector(std::vector<EntitySaveState>& saveEntities)
        {
            for (EntitySaveState& saveState : saveEntities)
            {
                if (ESSM::EntityHijackManager::IsHijacked(saveState))
                {
                    ESSM::EntityOperations::ClearSaveState(saveState);
                }
            }
        }

        static void ClearPlayerSave(GameStatePlayer& saveState)
        {
            if (ESSM::PlayerHijackManager::IsHijacked(saveState))
            {
                ESSM::PlayerOperations::ClearSaveState(saveState);
            }
        }

        static void ClearFamiliarSaveVector(std::vector<FamiliarData>& saveEntities)
        {
            for (FamiliarData& saveState : saveEntities)
            {
                if (ESSM::FamiliarHijackManager::IsHijacked(saveState))
                {
                    ESSM::FamiliarOperations::ClearSaveState(saveState);
                }
            }
        }

        static void write_entity_smart_pointer(EntitySaveState& data, _WriteState& writeState)
        {
            assert(EntityHijackManager::IsHijacked(data) || EntityHijackManager::IsWritten(data));
            if (EntityHijackManager::IsHijacked(data))
            {
                auto& writtenSaveStates = writeState.writtenEntitySaveStates;
                uint32_t writeId = writtenSaveStates.size();
    
                uint32_t id = EntityHijackManager::GetId(data);
                EntityHijackManager::SetWritten(data);
                EntityHijackManager::SetId(data, writeId);
    
                writtenSaveStates.emplace_back(&data, id);
            }
            else if (!EntityHijackManager::IsWritten(data))
            {
                throw std::runtime_error("invalid EntitySaveState state during write");
            }
    
            EntitySaveState* innerSmartPointer = data.entitySaveState.saveState;
            if (innerSmartPointer)
            {
                write_entity_smart_pointer(*innerSmartPointer, writeState);
            }
        }
    
        static void write_entity(EntitySaveState& data, _WriteState& writeState)
        {
            assert(EntityHijackManager::IsHijacked(data));
            if (!EntityHijackManager::IsHijacked(data))
            {
                throw std::runtime_error("invalid EntitySaveState state during write");
            }

            auto& writtenSaveStates = writeState.writtenEntitySaveStates;
            uint32_t writeId = writtenSaveStates.size();
    
            uint32_t id = EntityHijackManager::GetId(data);
            EntityHijackManager::SetWritten(data);
            EntityHijackManager::SetId(data, writeId);
    
            writtenSaveStates.emplace_back(&data, id);
    
            EntitySaveState* smartPointer = data.entitySaveState.saveState;
            if (smartPointer)
            {
                write_entity_smart_pointer(*smartPointer, writeState);
            }
        }

        static void write_player(GameStatePlayer& data, _WriteState& writeState)
        {
            assert(PlayerHijackManager::IsHijacked(data));
            if (!PlayerHijackManager::IsHijacked(data))
            {
                throw std::runtime_error("invalid GameStatePlayer state during write");
            }

            auto& writtenSaveStates = writeState.writtenEntitySaveStates;
            uint32_t writeId = writtenSaveStates.size();

            uint32_t id = PlayerHijackManager::GetId(data);
            PlayerHijackManager::SetWritten(data);
            PlayerHijackManager::SetId(data, writeId);

            writtenSaveStates.emplace_back(&data, id);
        }

        static void write_familiar(FamiliarData& data, _WriteState& writeState)
        {
            assert(FamiliarHijackManager::IsHijacked(data));
            if (!FamiliarHijackManager::IsHijacked(data))
            {
                throw std::runtime_error("invalid FamiliarData state during write");
            }

            auto& writtenSaveStates = writeState.writtenEntitySaveStates;
            uint32_t writeId = writtenSaveStates.size();

            uint32_t id = FamiliarHijackManager::GetId(data);
            FamiliarHijackManager::SetWritten(data);
            FamiliarHijackManager::SetId(data, writeId);

            writtenSaveStates.emplace_back(&data, id);
        }
    
        static void read_entity(EntitySaveState& data, _ReadState& readState)
        {
            if (!EntityHijackManager::IsWritten(data))
            {
                if (!EntityHijackManager::IsRead(data))
                {
                    readState.errors.set(ReadErrors::ERROR_INVALID_STATE); // invalid EntitySaveState state during read
                }
            }
            else
            {
                // Due to how flip state are implemented, there can be duplicate ids. As such finding a duplicate here is not an error.
                uint32_t readId = EntityHijackManager::GetId(data);
                readState.minReadEntities = std::max(readState.minReadEntities, readId + 1);
                readState.readEntitySaveStates.push_back(&data);
            }
            
            EntityHijackManager::SetRead(data);
            EntitySaveState* smartPointer = data.entitySaveState.saveState;
            if (smartPointer)
            {
                read_entity(*smartPointer, readState);
            }
        }

        static void read_player(GameStatePlayer& data, _ReadState& readState)
        {
            if (!PlayerHijackManager::IsWritten(data))
            {
                if (!PlayerHijackManager::IsRead(data))
                {
                    readState.errors.set(ReadErrors::ERROR_INVALID_STATE); // invalid GameStatePlayer state during read
                }
            }
            else
            {
                // IMPROVEMENT: we may want to detect if there is a duplicate id, however this currently does not cause problems
                uint32_t readId = PlayerHijackManager::GetId(data);
                readState.minReadEntities = std::max(readState.minReadEntities, readId + 1);
                readState.readEntitySaveStates.push_back(&data);
            }

            PlayerHijackManager::SetRead(data);
        }

        static void read_familiar(FamiliarData& data, _ReadState& readState)
        {
            if (!FamiliarHijackManager::IsWritten(data))
            {
                if (!FamiliarHijackManager::IsRead(data))
                {
                    readState.errors.set(ReadErrors::ERROR_INVALID_STATE); // invalid FamiliarData state during read
                }
            }
            else
            {
                // IMPROVEMENT: we may want to detect if there is a duplicate id, however this currently does not cause problems
                uint32_t readId = FamiliarHijackManager::GetId(data);
                readState.minReadEntities = std::max(readState.minReadEntities, readId + 1);
                readState.readEntitySaveStates.push_back(&data);
            }

            FamiliarHijackManager::SetRead(data);
        }
    
        static void write_entity_vector(std::vector<EntitySaveState>& saveEntities, _WriteState& writeState)
        {
            for (EntitySaveState& saveState : saveEntities)
            {
                write_entity(saveState, writeState);
            }
        }
    
        static void read_entity_vector(std::vector<EntitySaveState>& saveEntities, _ReadState& readState)
        {
            for (EntitySaveState& saveState : saveEntities)
            {
                read_entity(saveState, readState);
            }
        }

        static void write_familiar_vector(std::vector<FamiliarData>& saveEntities, _WriteState& writeState)
        {
            for (FamiliarData& saveState : saveEntities)
            {
                write_familiar(saveState, writeState);
            }
        }

        static void read_familiar_vector(std::vector<FamiliarData>& saveEntities, _ReadState& readState)
        {
            for (FamiliarData& saveState : saveEntities)
            {
                read_familiar(saveState, readState);
            }
        }
    
        static auto ClearEntitySaveVectorLambda = [](std::vector<EntitySaveState>& vec) { ClearEntitySaveVector(vec); };
        static auto WriteEntityVectorLambda = [](std::vector<EntitySaveState>& vec, _WriteState& writeState) { write_entity_vector(vec, writeState); };
        static auto WriteFamiliarVectorLambda = [](std::vector<FamiliarData>& vec, _WriteState& writeState) { write_familiar_vector(vec, writeState); };
        static auto WritePlayerLambda = [](GameStatePlayer& player, _WriteState& writeState) { write_player(player, writeState); };
        static auto ReadEntityVectorLambda = [](std::vector<EntitySaveState>& vec, _ReadState& readState) { read_entity_vector(vec, readState); };
        static auto ReadFamiliarVectorLambda = [](std::vector<FamiliarData>& vec, _ReadState& readState) { read_familiar_vector(vec, readState); };
        static auto ReadPlayerLambda = [](GameStatePlayer& player, _ReadState& readState) { read_player(player, readState); };
    }

    static void resolve_read_errors(_ReadState& readState)
    {
        if (readState.errors.test(ReadErrors::ERROR_INVALID_STATE))
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "invalid EntitySaveState state during read\n");
        }

        if (readState.errors.test(ReadErrors::ERROR_DUPLICATE_ID))
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "duplicate id found during read\n");
        }
    }

    static void clear_write_state(_WriteState& writeState)
    {
        writeState.writtenEntitySaveStates.clear();
    }

    bool CheckErrors(const ReadState& readState)
    {
        return readState.m_readState.errors.any();
    }

    static bool needs_handling(const _ReadState& readState)
    {
        return !readState.readEntitySaveStates.empty();
    }

    bool NeedsHandling(const ReadState& readState)
    {
        return needs_handling(readState.m_readState);
    }

    template <typename T>
    static void write_binary(std::ofstream& ofs, const T& value)
    {
        static_assert(std::is_trivially_copyable_v<T>, "write_binary can only serialize trivially copyable types");
        ofs.write(reinterpret_cast<const char*>(&value), sizeof(T));

        if (!ofs)
        {
            throw std::runtime_error("Failed to write data");
        }
    }

    template <typename T>
    static void read_binary(std::ifstream& ifs, T& value)
    {
        static_assert(std::is_trivially_copyable_v<T>, "read_binary can only deserialize trivially copyable types");
        ifs.read(reinterpret_cast<char*>(&value), sizeof(value));;

        if (!ifs)
        {
            throw std::runtime_error("Failed to read data");
        }
    }

    static std::streamoff validate_file_array_size(std::ifstream& file, uint32_t fileCount)
    {
        // validate file array size
        std::streamoff startPosition = file.tellg();
        file.seekg(0, std::ios::end);
        std::streamoff fileSize = file.tellg();
        file.seekg(startPosition); // restore

        std::streamoff endPosition = std::streamoff(startPosition) + std::streamoff(fileCount) * Section::ENTITY_SIZE;
        if (fileSize < endPosition) // + footer
        {
            throw std::runtime_error("File truncated or corrupt");
        }

        return endPosition;
    }

    static std::string get_save_data_path(const std::string& fileName)
    {
        return REPENTOGON::StringFormat("%s/EntitySaveStateManagement/%s.dat", REPENTOGON::GetRepentogonDataPath(), fileName.c_str());
    }

    static bool guarantee_parent_path_exists(std::filesystem::path filePath)
    {
        std::filesystem::path directory = filePath.parent_path();

        std::error_code errorCode;
        if (!directory.empty() && !std::filesystem::exists(directory, errorCode))
        {
            if (errorCode)
            {
                return false;
            }

            std::filesystem::create_directories(directory, errorCode);
            if (errorCode)
            {
                return false;
            }
        }

        return true;
    }

    static void remove_file(const std::filesystem::path& filePath)
    {
        std::error_code errorCode;
        std::filesystem::remove(filePath, errorCode);
        if (errorCode)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to delete file at \"%s\"\n", filePath.string().c_str());
        }
    }

    WriteState WriteGameState()
    {
        Manager* manager = g_Manager;
        GameState& gameState = manager->_gamestate;
    
        WriteState writeState = WriteState();
        _WriteState& _writeState = writeState.m_writeState;
        try
        {
            ESSM::EntityIterators::InGameState::All(gameState, Operations::WriteEntityVectorLambda, _writeState);
            ESSM::PlayerIterators::InGameState::All(gameState, Operations::WritePlayerLambda, _writeState);
            ESSM::FamiliarIterators::InGameState::All(gameState, Operations::WriteFamiliarVectorLambda, _writeState);
        }
        catch(const std::runtime_error& e)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to write game state : %s\n", e.what());
            clear_write_state(_writeState);
            return writeState;
        }

        // serialize entities from write State
        return writeState;
    }

    ReadState ReadGameState()
    {
        Manager* manager = g_Manager;
        GameState& gameState = manager->_gamestate;

        ReadState readState = ReadState();
        _ReadState& _readState = readState.m_readState;

        try
        {
            ESSM::EntityIterators::InGameState::All(gameState, Operations::ReadEntityVectorLambda, _readState);
            ESSM::PlayerIterators::InGameState::All(gameState, Operations::ReadPlayerLambda, _readState);
            ESSM::FamiliarIterators::InGameState::All(gameState, Operations::ReadFamiliarVectorLambda, _readState);
        }
        catch(const std::runtime_error& e)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to read game state : %s\n", e.what());
            _readState.errors.set(ReadErrors::TRIGGERED_EXCEPTION);
        }

        resolve_read_errors(_readState);
        return readState;
    }

    void RestoreWrittenStates(WriteState& writeState)
    {
        _WriteState& _writeState = writeState.m_writeState;

        _writeState.writeEntityIdPairs.reserve(_writeState.writtenEntitySaveStates.size());
        for (auto& writtenSaveState : _writeState.writtenEntitySaveStates)
        {
            StatePtr& saveState = writtenSaveState.first;
            uint32_t id = writtenSaveState.second;
    
            uint32_t writeId;
            std::visit([&](auto* ptr)
            {
                using T = std::decay_t<decltype(ptr)>;
                constexpr bool Type_Entity = std::is_same_v<T, EntitySaveState*>;
                constexpr bool Type_Player = std::is_same_v<T, GameStatePlayer*>;
                constexpr bool Type_Familiar = std::is_same_v<T, FamiliarData*>;

                static_assert(Type_Entity || Type_Player || Type_Familiar, "Unhandled variant");

                if constexpr (Type_Entity) {
                    assert(EntityHijackManager::IsWritten(*ptr));
                    EntityHijackManager::SetHijacked(*ptr);
                    writeId = EntityHijackManager::GetId(*ptr);
                    EntityHijackManager::SetId(*ptr, id);
                }
                else if constexpr (Type_Player) {
                    assert(PlayerHijackManager::IsWritten(*ptr));
                    PlayerHijackManager::SetHijacked(*ptr);
                    writeId = PlayerHijackManager::GetId(*ptr);
                    PlayerHijackManager::SetId(*ptr, id);
                }
                else if constexpr (Type_Familiar) {
                    assert(FamiliarHijackManager::IsWritten(*ptr));
                    FamiliarHijackManager::SetHijacked(*ptr);
                    writeId = FamiliarHijackManager::GetId(*ptr);
                    FamiliarHijackManager::SetId(*ptr, id);
                }
            }, saveState);
            
            _writeState.writeEntityIdPairs.emplace_back(writeId, id);
        }
    }

    void hijack_read_entity_states(_ReadState& readState, SaveFile& saveFile)
    {
        std::vector<HijackedState>& entities = saveFile.entities;
        int maxId = entities.size() - 1;

        for (StatePtr& readSaveState : readState.readEntitySaveStates)
        {
            uint32_t id = ESSM::IdManager::NewId();
            uint32_t readId;

            std::visit([&](auto* ptr)
            {
                using T = std::decay_t<decltype(ptr)>;
                constexpr bool Type_Entity = std::is_same_v<T, EntitySaveState*>;
                constexpr bool Type_Player = std::is_same_v<T, GameStatePlayer*>;
                constexpr bool Type_Familiar = std::is_same_v<T, FamiliarData*>;

                static_assert(Type_Entity || Type_Player || Type_Familiar, "Unhandled variant");

                if constexpr (Type_Entity) {
                    assert(EntityHijackManager::IsRead(*ptr));
                    readId = EntityHijackManager::GetId(*ptr);
                    // all previous steps should have guaranteed this
                    assert((int)readId <= maxId);

                    EntityHijackManager::SetHijacked(*ptr);
                    EntityHijackManager::SetId(*ptr, id);
                }
                else if constexpr (Type_Player) {
                    assert(PlayerHijackManager::IsRead(*ptr));
                    readId = PlayerHijackManager::GetId(*ptr);
                    // all previous steps should have guaranteed this
                    assert((int)readId <= maxId);

                    PlayerHijackManager::SetHijacked(*ptr);
                    PlayerHijackManager::SetId(*ptr, id);
                }
                else if constexpr (Type_Familiar) {
                    assert(FamiliarHijackManager::IsRead(*ptr));
                    readId = FamiliarHijackManager::GetId(*ptr);
                    // all previous steps should have guaranteed this
                    assert((int)readId <= maxId);

                    FamiliarHijackManager::SetHijacked(*ptr);
                    FamiliarHijackManager::SetId(*ptr, id);
                }
            }, readSaveState);

            s_systemData.hijackedStates[id] = entities[readId];
            readState.restoreEntityIdPairs.emplace_back(readId, id);
        }
    }

    static void serialize_entities(std::ofstream& file, const _WriteState& writeState)
    {
        uint32_t size = static_cast<uint32_t>(writeState.writtenEntitySaveStates.size());
        if (size > Section::MAX_ENTITY_COUNT)
        {
            throw std::runtime_error("Too many entities to serialize");
        }

        write_binary(file, Section::ENTITY_START);
        write_binary(file, size);

        for (auto& saveState : writeState.writtenEntitySaveStates)
        {
            uint32_t id = saveState.second;
            HijackedState& hijackedState = s_systemData.hijackedStates[id];

            write_binary(file, hijackedState.id); // UNHIJACKED_ID
            write_binary(file, hijackedState.marker); // UNHIJACKED_MARKER
        }

        write_binary(file, Section::ENTITY_END);
    }

    static void deserialize_entities(std::ifstream& file, _ReadState& readState, SaveFile& saveFile)
    {
        std::vector<HijackedState>& entities = saveFile.entities;

        uint32_t sectionHeader = 0;
        read_binary(file, sectionHeader);
        if (sectionHeader != Section::ENTITY_START)
        {
            throw std::runtime_error("Incorrect section header");
        }

        uint32_t fileCount = 0;
        read_binary(file, fileCount);

        std::streamoff endPosition = validate_file_array_size(file, fileCount);

        if (fileCount < readState.minReadEntities)
        {
            throw std::runtime_error("Not enough entities in save data");
        }

        if (fileCount > Section::MAX_ENTITY_COUNT)
        {
            throw std::runtime_error("Entity count too large");
        }

        uint32_t neededCount = std::max(fileCount, readState.minReadEntities);
        try {
            entities.reserve(neededCount);
        }
        catch (const std::bad_alloc&) {
            throw std::runtime_error("Out of memory");
        }

        for (uint32_t i = 0; i < neededCount; ++i)
        {
            HijackedState& hijackedState = entities.emplace_back();
            read_binary(file, hijackedState.id); // UNHIJACKED_ID
            read_binary(file, hijackedState.marker); // UNHIJACKED_MARKER
        }

        file.seekg(endPosition);
        if (!file)
        {
            throw std::runtime_error("Failed to skip to end of entity data");
        }

        uint32_t sectionFooter = 0;
        read_binary(file, sectionFooter);
        if (sectionFooter != Section::ENTITY_END)
        {
            throw std::runtime_error("Incorrect section footer");
        }
    }

    static void serialize_save(std::ofstream& file, const _WriteState& writeState)
    {
        uint32_t gameChecksum = g_Manager->_gamestate._checksum;

        write_binary(file, SAVE_VERSION);
        write_binary(file, gameChecksum);

        serialize_entities(file, writeState);
    }

    static void deserialize_save(std::ifstream& file, _ReadState& readState, SaveFile& saveFile)
    {
        uint32_t gameChecksum = g_Manager->_gamestate._checksum;

        uint32_t saveVersion = 0;
        read_binary(file, saveVersion);
        if (saveVersion != SAVE_VERSION)
        {
            throw std::runtime_error(REPENTOGON::StringFormat("unsupported save version %u\n", saveVersion));
        }

        uint32_t saveChecksum = 0;
        read_binary(file, saveChecksum);
        if (saveChecksum != gameChecksum)
        {
            throw std::runtime_error("save checksum mismatch\n");
        }

        deserialize_entities(file, readState, saveFile);
    }

    static void serialize(const std::string& filename, const _WriteState& writeState)
    {
        std::filesystem::path filePath = get_save_data_path(filename);
        bool exists = guarantee_parent_path_exists(filePath);
        if (!exists)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to create directory for save file at \"%s\"\n", filePath.string().c_str());
            return;
        }

        std::ofstream file = std::ofstream(filePath, std::ios::binary);
        if (!file.is_open())
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to open save file for writing at \"%s\"\n", filePath.string().c_str());
            return;
        }

        try
        {
            serialize_save(file, writeState);
            file.flush();

            if (!file)
            {
                throw std::runtime_error("flush failed");
            }
        }
        catch(const std::runtime_error& e)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to serialize save file at \"%s\": %s\n", filePath.string().c_str(), e.what());
            file.close();
            remove_file(filePath);
            return;
        }

        // TODO: Send Serialize Signal
        ZHL::Log(__LOG_INFO_HEADER__ "successfully saved %s to \"%s\"\n", filename.c_str(), filePath.string().c_str());
    }

    void Serialize(const std::string& fileName, WriteState& writeState)
    {
        _WriteState& _writeState = writeState.m_writeState;
        serialize(fileName, _writeState);

        _writeState.FreeMemory();
    }

    static bool deserialize(const std::string& fileName, _ReadState& readState)
    {
        // we should have never entered with errors here in the first place
        if (readState.errors.any())
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "pre-existing errors in read state when deserializing\n");
            return false;
        }

        if (!needs_handling(readState))
        {
            return true;
        }

        std::filesystem::path filePath = get_save_data_path(fileName);
        std::error_code errorCode;
        if (!std::filesystem::exists(filePath, errorCode))
        {
            return false;
        }

        std::ifstream file = std::ifstream(filePath, std::ios::binary);
        if (!file.is_open())
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to open save file for reading at \"%s\"\n", filePath.string().c_str());
            return false;
        }

        SaveFile saveFile;
        try
        {
            deserialize_save(file, readState, saveFile);
        }
        catch(const std::runtime_error& e)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to deserialize save file at \"%s\": %s\n", filePath.string().c_str(), e.what());
            return false;
        }

        file.close();
        hijack_read_entity_states(readState, saveFile);
        ZHL::Log(__LOG_INFO_HEADER__ "successfully loaded %s from \"%s\"\n", fileName.c_str(), filePath.string().c_str());
        // TODO: Send Deserialize Event
        return true;
    }

    bool Deserialize(const std::string& fileName, ReadState& readState)
    {
        _ReadState& _readState = readState.m_readState;
        bool success = deserialize(fileName, _readState);

        // free the memory since we no longer need it
        _readState.FreeMemory();
        return success;
    }

    void DeleteGameState(const std::string& fileName)
    {
        std::filesystem::path filePath = get_save_data_path(fileName);
        std::error_code errorCode;
        if (!std::filesystem::exists(filePath, errorCode))
        {
            return;
        }

        std::filesystem::remove(filePath, errorCode);
        if (errorCode)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to delete file at \"%s\"\n", filePath.string().c_str());
        }
        else
        {
            ZHL::Log(__LOG_INFO_HEADER__ "deleted file at \"%s\"\n", filePath.string().c_str());
        }
    }
}

static std::pair<Entity*, EntitySaveState*> s_minecartEntity = {nullptr, nullptr};

HOOK_METHOD(Room, save_entity, (Entity* entity, EntitySaveState* data, bool savingMinecartEntity) -> bool)
{
    std::pair<Entity*, EntitySaveState*> minecartEntity = s_minecartEntity;
    s_minecartEntity.first = nullptr; s_minecartEntity.second = nullptr;

    assert(!ESSM::EntityHijackManager::IsHijacked(*data)); // these are always newly created so they should not be already hijacked
    bool saved = super(entity, data, savingMinecartEntity);

    if (!saved)
    {
        if (minecartEntity.first)
        {
            assert(entity->_type == eEntityType::ENTITY_MINECART); // the next thing after a minecart entity save must always be a minecart
            EntitySaveState* flipState = EntitySaveState::Pickup::GetFlipSaveState(*minecartEntity.second);
            if (flipState)
            {
                ESSM::EntityHijackManager::UnHijack(*flipState);
            }
        }

        return false;
    }

    if (savingMinecartEntity)
    {
        s_minecartEntity.first = entity;
        s_minecartEntity.second = data;
        return true;
    }

    if (minecartEntity.first)
    {
        assert(entity->_type == eEntityType::ENTITY_MINECART); // the next thing after a minecart entity save must always be a minecart
        EntitySaveState* flipState = EntitySaveState::Pickup::GetFlipSaveState(*minecartEntity.second);
        if (flipState)
        {
            ESSM::EntityOperations::CopySaveState(*flipState);
        }

        uint32_t id = ESSM::EntityHijackManager::NewHijack(*minecartEntity.second);
        SaveEntity(*minecartEntity.first, *minecartEntity.second, id);
    }

    EntitySaveState* flipState = EntitySaveState::Pickup::GetFlipSaveState(*data);
    if (flipState)
    {
        ESSM::EntityOperations::CopySaveState(*flipState);
    }

    uint32_t id = ESSM::EntityHijackManager::NewHijack(*data);
    SaveEntity(*entity, *data, id);
    return saved;
}

HOOK_METHOD(Room, restore_entity, (Entity* entity, EntitySaveState* data) -> void)
{
    uint32_t id = ESSM::EntityHijackManager::UnHijack(*data);

    super(entity, data);

    ESSM::EntityHijackManager::Hijack(*data, id);
    RestoreEntity(*entity, *data, id);
}

HOOK_METHOD(Level, Init, (bool resetLilPortalRoom) -> void)
{
    ESSM::EntityOperations::ClearVector(g_Game->_myosotisPickups);
    super(resetLilPortalRoom);
}

HOOK_METHOD(Level, reset_room_list, (bool resetLilPortalRoom) -> void)
{
    Game* game = g_Game;
    RoomDescriptor* roomList = game->_gridRooms;
    constexpr size_t LIL_PORTAL_IDX = MAX_GRID_ROOMS + (-eGridRooms::ROOM_LIL_PORTAL_IDX) - 1;

    for (size_t i = 0; i < MAX_ROOMS; i++)
    {
        if (i == LIL_PORTAL_IDX && !(resetLilPortalRoom))
        {
            continue;
        }

        RoomDescriptor& room = roomList[i];
        ESSM::EntityOperations::ClearVector(room.SavedEntities);
    }

    super(resetLilPortalRoom);
}

HOOK_METHOD(Level, DEBUG_goto_room, (RoomConfig_Room * roomConfig) -> void)
{
    constexpr size_t LIST_IDX = eGridRooms::MAX_GRID_ROOMS + (-eGridRooms::ROOM_DEBUG_IDX) - 1;

    Game* game = g_Game;
    RoomDescriptor& room = game->_gridRooms[LIST_IDX];
    ESSM::EntityOperations::ClearVector(room.SavedEntities);

    super(roomConfig);
}

HOOK_METHOD(Level, InitializeGenesisRoom, () -> void)
{
    constexpr size_t LIST_IDX = eGridRooms::MAX_GRID_ROOMS + (-eGridRooms::ROOM_GENESIS_IDX) - 1;

    Game* game = g_Game;
    RoomDescriptor& room = game->_gridRooms[LIST_IDX];
    ESSM::EntityOperations::ClearVector(room.SavedEntities);

    super();
}

HOOK_METHOD(Level, TryInitializeBlueRoom, (int currentIdx, int destinationIdx, int direction) -> bool)
{
    constexpr uint32_t FLAG_BLUE_REDIRECT = 1 << 18;
    constexpr size_t LIST_IDX = eGridRooms::MAX_GRID_ROOMS + (-eGridRooms::ROOM_BLUE_ROOM_IDX) - 1;

    if ((direction != -1 && currentIdx >= 0) && (g_Game->GetRoomByIdx(destinationIdx, -1)->Flags & FLAG_BLUE_REDIRECT) != 0)
    {
        Game* game = g_Game;
        RoomDescriptor& room = game->_gridRooms[LIST_IDX];
        ESSM::EntityOperations::ClearVector(room.SavedEntities);
    }

    return super(currentIdx, destinationIdx, direction);
}

HOOK_METHOD(Game, SaveBackwardsStage, (int stage) -> void)
{
    BackwardsStageDesc& backwardsStage = g_Game->_backwardsStages[stage - 1];
    ESSM::EntityIterators::Structure::BackwardsStage(backwardsStage, ESSM::EntityOperations::ClearVectorLambda);

	super(stage);

    ESSM::EntityIterators::Structure::BackwardsStage(backwardsStage, ESSM::EntityOperations::CopyVectorLambda);
}

// Clear backwards stage save state, even though the game simply sets room count to 0, to avoid having to iterate "uninitialized" RoomDescriptors in the BackwardsStage. 
HOOK_METHOD(Game, ResetState, () -> void)
{
    ESSM::EntityIterators::InGame::AllBackwardsStages(ESSM::EntityOperations::ClearVectorLambda);
	super();
}

HOOK_METHOD(GameState, Clear, () -> void)
{
    LogDebug(__LOG_DEBUG_HEADER__ "Start GameState::Clear\n");

    ESSM::EntityIterators::InGameState::AllRooms(*this, ESSM::SaveData::Operations::ClearEntitySaveVectorLambda);
    ESSM::EntityIterators::InGameState::AllBackwardsStages(*this, ESSM::SaveData::Operations::ClearEntitySaveVectorLambda);
    // player save states are handled by GameStatePlayer::Init
    super();

    LogDebug(__LOG_DEBUG_HEADER__ "End GameState::Clear\n");
}

HOOK_METHOD(Game, SaveState, (GameState* state) -> void)
{
    LogDebug(__LOG_DEBUG_HEADER__ "Start Game::SaveState\n");

    // There is no need to clear the state since SaveState always calls GameState::Clear before saving
	super(state);
    ESSM::EntityIterators::InGameState::AllRooms(*state, ESSM::EntityOperations::CopyVectorLambda);
    ESSM::EntityIterators::InGameState::AllBackwardsStages(*state, ESSM::EntityOperations::CopyVectorLambda);
    // players are handled by Entity_Player::Init

    LogDebug(__LOG_DEBUG_HEADER__ "End Game::SaveState\n");
}

HOOK_METHOD(Game, RestoreState, (GameState* state, bool startGame) -> void)
{
    LogDebug(__LOG_DEBUG_HEADER__ "Start Game::RestoreState\n");

    ESSM::EntityIterators::InGame::AllBackwardsStages(ESSM::EntityOperations::ClearVectorLambda);
    super(state, startGame);
    // copy is in a separate patch as copying it here might cause problems due to callbacks running in the mean time.

    LogDebug(__LOG_DEBUG_HEADER__ "End Game::RestoreState\n");
}

HOOK_METHOD(Level, RestoreGameState, (GameState* state) -> void)
{
    // Manual clear the lil portal room
    constexpr size_t LIL_PORTAL_IDX = (-eGridRooms::ROOM_LIL_PORTAL_IDX) - 1;
    ESSM::EntityOperations::ClearVector(g_Game->_negativeGridRooms[LIL_PORTAL_IDX].SavedEntities);
    super(state);
}

HOOK_METHOD(GameStatePlayer, Init, () -> void)
{
    ESSM::SaveData::Operations::ClearEntitySaveVector(this->_movingBoxContents);
    ESSM::SaveData::Operations::ClearFamiliarSaveVector(this->_familiarData);
    ESSM::SaveData::Operations::ClearPlayerSave(*this);
    super();
    // Doing this here since it makes the most sense, also means we won't have problems if a GameStatePlayer is accessed in between an Init and a Restore
    ESSM::PlayerHijackManager::SetCleared(*this);
}

HOOK_METHOD(Entity_Player, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void)
{
    super(type, variant, subtype, initSeed);

    assert(!ESSM::PlayerHijackManager::HasMarker(this->_unlistedRestoreState));
    ESSM::PlayerHijackManager::SetCleared(this->_unlistedRestoreState);
}

HOOK_METHOD(Entity_Player, destructor, () -> void)
{
    ESSM::EntityOperations::ClearVector(this->_movingBoxContents);

    GameStatePlayer& playerState = this->_unlistedRestoreState;
    ESSM::EntityOperations::ClearVector(playerState._movingBoxContents);
    ESSM::FamiliarOperations::ClearVector(playerState._familiarData);

    if (!ESSM::PlayerHijackManager::IsCleared(playerState))
    {
        ESSM::PlayerOperations::ClearSaveState(playerState);
    }

    super();
}

HOOK_METHOD(Entity_Player, StoreGameState, (GameStatePlayer* saveState, bool saveTemporaryFamiliars) -> void)
{
    assert(ESSM::PlayerHijackManager::IsCleared(*saveState));
    // ASSUMPTION: We don't need to clear the marker or the state, as the store will override them anyway with the correct values

    super(saveState, saveTemporaryFamiliars);

    ESSM::EntityOperations::CopyVector(saveState->_movingBoxContents);
    // Familiar data is handled in separate hooks
    ESSM::PlayerHijackManager::NewHijack(*saveState);
}

HOOK_METHOD(Entity_Player, RestoreGameState, (GameStatePlayer * saveState) -> void)
{
    // if this player was replaced then don't restore the state
    if (this->_replacedPlayer)
    {

        return super(saveState);
    }

    // Familiars are handled by Entity_Familiar::RestoreState (which is in RestoreGameState_PostLevelInit anyway)
    // TODO: Send Player restore signal
    ESSM::EntityOperations::ClearVector(this->_movingBoxContents);
    uint32_t id = ESSM::PlayerHijackManager::UnHijack(*saveState);
    super(saveState);
    ESSM::PlayerHijackManager::Hijack(*saveState, id);
    ESSM::EntityOperations::CopyVector(this->_movingBoxContents);
}

HOOK_METHOD(Entity_Familiar, RestoreState, (FamiliarData* saveState) -> void)
{
    // TODO: Send Familiar restore signal
    uint32_t id = ESSM::FamiliarHijackManager::UnHijack(*saveState);
    super(saveState);
    ESSM::FamiliarHijackManager::Hijack(*saveState, id);
}

static void __stdcall restore_game_state_backwards_rooms()
{
    ESSM::EntityIterators::InGame::AllBackwardsStages(ESSM::EntityOperations::CopyVectorLambda);
}

static void Patch_GameRestoreState_PostBackwardsStageDescRestore()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Game_RestoreState_PostBackwardsStageDescRestore);
    ZHL::Log("[REPENTOGON] Patching Game::RestoreState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .PreserveRegisters(savedRegisters)
        .AddInternalCall(restore_game_state_backwards_rooms)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_room_vector()
{
    RoomDescriptor& room = *g_Game->_room->_descriptor;
    ESSM::EntityOperations::ClearVector(room.SavedEntities);
}

static void Patch_RoomSaveState_ClearVector()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_SaveState_ClearVector);
    ZHL::Log("[REPENTOGON] Patching Room::SaveState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_clear_room_vector)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void Patch_RoomRestoreState_ClearVector()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_RestoreState_ClearVector);
    ZHL::Log("[REPENTOGON] Patching Room::RestoreState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_clear_room_vector)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_copy_myosotis_pickups()
{
    ESSM::EntityOperations::CopyVector(g_Game->_myosotisPickups);
}

static void Patch_LevelInit_PostMyosotisEffect()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_Init_PostMyosotisEffect);
    ZHL::Log("[REPENTOGON] Patching Level::Init for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_copy_myosotis_pickups)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall restore_level_game_state()
{
    ESSM::EntityIterators::InGame::AllRooms(ESSM::EntityOperations::CopyVectorLambda);
}

static void Patch_LevelRestoreGameState_PreRoomLoad()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_RestoreGameState_PreRoomLoad);
    ZHL::Log("[REPENTOGON] Patching Level::RestoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .PreserveRegisters(savedRegisters)
        .AddInternalCall(restore_level_game_state)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_vector_pre_backwards_assign(std::vector<EntitySaveState>* saveStateVector)
{
    ESSM::EntityOperations::ClearVector(*saveStateVector);
}

static void __stdcall asm_copy_vector_post_backwards_assign(std::vector<EntitySaveState>* saveStateVector)
{
    ESSM::EntityOperations::CopyVector(*saveStateVector);
}

static void Patch_LevelPlaceRoomsBackwards_Boss_AssignEntitySaveStateVector()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_place_rooms_backwards_Boss_AssignEntitySaveStateVector);
    ZHL::Log("[REPENTOGON] Patching Level::place_rooms_backwards for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t callAddr = addr + 8;
    intptr_t resumeAddr = callAddr + 5;
    constexpr size_t RESTORED_BYTES = 8;
    int32_t call_rel32 = *(int32_t*)(callAddr + 1);
    intptr_t calleeAddress = callAddr + 5 + call_rel32;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::ECX) // saveStateVector
        .AddInternalCall(asm_clear_vector_pre_backwards_assign)
        .RestoreRegisters(savedRegisters) 
        .Push(ASMPatch::Registers::ECX) // saveStateVector (for asm_copy_vector_post_backwards_assign)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddInternalCall((void*)calleeAddress) // restore call to assign
        .AddInternalCall((void*)asm_copy_vector_post_backwards_assign)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void Patch_LevelPlaceRoomsBackwards_Treasure_AssignEntitySaveStateVector()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_place_rooms_backwards_Treasure_AssignEntitySaveStateVector);
    ZHL::Log("[REPENTOGON] Patching Level::place_rooms_backwards for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t callAddr = addr + 10;
    intptr_t resumeAddr = callAddr + 5;
    constexpr size_t RESTORED_BYTES = 10;
    int32_t call_rel32 = *(int32_t*)(callAddr + 1);
    intptr_t calleeAddress = callAddr + 5 + call_rel32;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EDX) // saveStateVector
        .AddInternalCall(asm_clear_vector_pre_backwards_assign)
        .RestoreRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EDX) // saveStateVector (for asm_copy_vector_post_backwards_assign)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddInternalCall((void*)calleeAddress) // restore call to assign
        .AddInternalCall((void*)asm_copy_vector_post_backwards_assign)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall hijack_dark_closet_collectible(EntitySaveState& saveState)
{
    ESSM::EntityHijackManager::NewHijack(saveState);
}

static void Patch_LevelGenerateDarkCloset_PostGenerateCollectibleSaveState()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_generate_dark_closet_PostGenerateCollectibleSaveState);
    ZHL::Log("[REPENTOGON] Patching Level::generate_dark_closet for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 7;
    intptr_t disp8Addr = addr + 2;
    constexpr size_t RESTORED_BYTES = 7;
    constexpr int32_t FIELD_OFFSET = offsetof(EntitySaveState, _intStorage7);
    
    int8_t disp8 = *(int8_t*)disp8Addr;
    int32_t saveStateOffset_EBP = disp8 - FIELD_OFFSET;

    patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .PreserveRegisters(savedRegisters)
        .LoadEffectiveAddress(ASMPatch::Registers::EBP, saveStateOffset_EBP, ASMPatch::Registers::ECX) // LEA ECX, [EBP + saveStateOffset_EBP]
        .AddInternalCall(hijack_dark_closet_collectible)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_moving_box_vector(Entity_Player& player)
{
    ESSM::EntityOperations::ClearVector(player._movingBoxContents);
}

static void Patch_PlayerUseActiveItem_MovingBoxClearVector()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseActiveItem_MovingBox_ClearVector);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::UseActiveItem for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EDI) // player
        .AddInternalCall(asm_clear_moving_box_vector)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall store_familiar_data(FamiliarData& saveState, size_t persistentELIndex)
{
    ESSM::FamiliarHijackManager::NewHijack(saveState);
    // TODO: Send Store command
}

static void Patch_PlayerStoreGameState_FamiliarStoreState()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_StoreGameState_FamiliarStoreState);
    ZHL::Log("[REPENTOGON] Patching EntityPlayer::StoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .MoveFromMemory(ASMPatch::Registers::EBP, -0x28, ASMPatch::Registers::EDX)
        .LoadEffectiveAddress(ASMPatch::Registers::EBP, -0x90, ASMPatch::Registers::ECX)
        .AddInternalCall(store_familiar_data)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_familiar_vector(std::vector<FamiliarData>* saveStateVector)
{
    ESSM::FamiliarOperations::ClearVector(*saveStateVector);
}

static void __stdcall asm_copy_familiar_vector(std::vector<FamiliarData>* saveStateVector)
{
    ESSM::FamiliarOperations::CopyVector(*saveStateVector);
}

static void Patch_PlayerStoreGameState_AssignUnlistedFamiliarData()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_StoreGameState_AssignUnlistedFamiliarData);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::StoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t callAddr = addr + 8;
    intptr_t resumeAddr = callAddr + 5;
    constexpr size_t RESTORED_BYTES = 8;
    int32_t call_rel32 = *(int32_t*)(callAddr + 1);
    intptr_t calleeAddress = callAddr + 5 + call_rel32;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::ECX) // saveStateVector
        .AddInternalCall(asm_clear_familiar_vector)
        .RestoreRegisters(savedRegisters) 
        .Push(ASMPatch::Registers::ECX) // saveStateVector (for asm_copy_familiar_vector)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddInternalCall((void*)calleeAddress) // restore call to assign
        .AddInternalCall((void*)asm_copy_familiar_vector)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void Patch_PlayerManagerRestoreGameState_AssignBackupFamiliarData()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PlayerManager_RestoreGameState_AssignBackupFamiliarData);
    ZHL::Log("[REPENTOGON] Patching PlayerManager::RestoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t callAddr = addr + 15;
    intptr_t resumeAddr = callAddr + 5;
    constexpr size_t RESTORED_BYTES = 15;
    int32_t call_rel32 = *(int32_t*)(callAddr + 1);
    intptr_t calleeAddress = callAddr + 5 + call_rel32;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::ECX) // saveStateVector
        .AddInternalCall(asm_clear_familiar_vector)
        .RestoreRegisters(savedRegisters) 
        .Push(ASMPatch::Registers::ECX) // saveStateVector (for asm_copy_familiar_vector)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddInternalCall((void*)calleeAddress) // restore call to assign
        .AddInternalCall((void*)asm_copy_familiar_vector)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

// use fastcall to emulate thiscall
static void __fastcall asm_clear_smart_pointer(EntitySaveState* saveState)
{
    if (ESSM::EntityHijackManager::IsHijacked(*saveState))
    {
        uint32_t id = ESSM::EntityHijackManager::GetId(*saveState);
        ESSM::EntityOperations::ClearSaveState(*saveState);
        LogDebug(__LOG_DEBUG_HEADER__ "Smart pointer Cleared: %u\n", id);
    }

    saveState->destructor();
}

static void Patch_ReferenceCount_EntitySaveStateDestructor()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::KAGE_ReferenceCount_EntitySaveState_DecrementReference_SaveStateDestructor);
    ZHL::Log("[REPENTOGON] Patching KAGE::ReferenceCount<EntitySaveState>::DecrementReference for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(asm_clear_smart_pointer);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void __stdcall asm_hijack_new_flip_state(EntitySaveState& saveState)
{
    uint32_t id = ESSM::EntityHijackManager::NewHijack(saveState);
    LogDebug(__LOG_DEBUG_HEADER__ "New Flip State: %u\n", id);
}

static void Patch_PickupInitFlipState_CreateSaveState()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPickup_InitFlipState_PostSaveStateConstructor);
    ZHL::Log("[REPENTOGON] Patching Entity_Pickup::InitFlipState for SaveStateManagent at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EAX) // arg
        .AddInternalCall(asm_hijack_new_flip_state)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_flip_restore(Entity& entity, EntitySaveState& saveState)
{
    RestoreEntity(entity, saveState, ESSM::EntityHijackManager::GetId(saveState));
}

static void Patch_PickupTryFlip_RestoreFlipState()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPickup_TryFlip_PostRestoreFlipState);
    ZHL::Log("[REPENTOGON] Patching Entity_Pickup::TryFlip for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    constexpr size_t RESTORED_BYTES = 4;
    constexpr size_t JNZ_OP_CODE_SIZE = 2;
    constexpr intptr_t JNZ_OFFSET = 4;
    intptr_t relativeJmpAddr = addr + (intptr_t)RESTORED_BYTES + (intptr_t)JNZ_OP_CODE_SIZE + JNZ_OFFSET;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EDI) // saveState
        .Push(ASMPatch::Registers::ESI) // entity
        .AddInternalCall(asm_flip_restore)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (void*)relativeJmpAddr)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall change_mineshaft_room(size_t listIdx)
{
    Game* game = g_Game;
    RoomDescriptor& room = game->_gridRooms[listIdx];
    ESSM::EntityOperations::ClearVector(room.SavedEntities);
}

static void Patch_EntityNPCAiMothersShadow_ChangeMineshaftRoom()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_ai_mothers_shadow_ChangeMineshaftRoom);
    ZHL::Log("[REPENTOGON] Patching Entity_NPC::ai_mothers_shadow for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 5;
    constexpr size_t RESTORED_BYTES = 5;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EBP, -0x48) // listIdx
        .AddInternalCall(change_mineshaft_room)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

void ESSM::ApplyPatches()
{
    Patch_ReferenceCount_EntitySaveStateDestructor();
    Patch_LevelInit_PostMyosotisEffect();
    Patch_LevelRestoreGameState_PreRoomLoad();
    Patch_LevelPlaceRoomsBackwards_Boss_AssignEntitySaveStateVector();
    Patch_LevelPlaceRoomsBackwards_Treasure_AssignEntitySaveStateVector();
    Patch_LevelGenerateDarkCloset_PostGenerateCollectibleSaveState();
    Patch_RoomSaveState_ClearVector();
    Patch_RoomRestoreState_ClearVector();
    Patch_GameRestoreState_PostBackwardsStageDescRestore();
    Patch_PlayerManagerRestoreGameState_AssignBackupFamiliarData();
    Patch_PlayerStoreGameState_FamiliarStoreState();
    Patch_PlayerStoreGameState_AssignUnlistedFamiliarData();
    Patch_PlayerUseActiveItem_MovingBoxClearVector();
    Patch_PickupInitFlipState_CreateSaveState();
    Patch_PickupTryFlip_RestoreFlipState();
    Patch_EntityNPCAiMothersShadow_ChangeMineshaftRoom();
}

namespace ESSM::LuaFunctions
{
    static ModReference& get_mod_reference(lua_State* L, int idx)
    {
        if (!lua_istable(L, idx))
        {
            luaL_argerror(L, idx, "Expected a table.");
        }

        ModReference* modRef = g_LuaEngine->GetModRefByTable(idx);
        if (!modRef)
        {
            luaL_argerror(L, idx, "Invalid ModReference.");
        }

        return *modRef;
    }

    LUA_FUNCTION(GetEntitySaveStateId)
    {
        EntitySaveState& state = Lua_EntitySaveState::GetEntitySaveState(L, 1);

        lua_pushinteger(L, EntityHijackManager::GetId(state));
        return 1;
    }

    static std::filesystem::path get_save_data_path(const ModReference& mod, const std::string& filename)
    {
        return std::filesystem::path(mod._dataDirectory) / "EntitySaveStateManagement" / filename;
    }

    LUA_FUNCTION(SaveData)
    {
        ModReference& modReference = get_mod_reference(L, 1);
        std::string filename = luaL_checkstring(L, 2);
        size_t len;
        // there is no need to copy the string
        const char* data = luaL_checklstring(L, 3, &len);

        std::filesystem::path filePath = get_save_data_path(modReference, filename);
        if (!std::filesystem::exists(filePath))
        {
            return 0;
        }

        std::ofstream file = std::ofstream(filePath, std::ios::binary);
        if (!file)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to open Mod file for writing at \"%s\"\n", filePath.string().c_str());
            return 0;
        }

        file.write(data, len);
        return 0;
    }

    LUA_FUNCTION(LoadData)
    {
        ModReference& modReference = get_mod_reference(L, 1);
        std::string filename = luaL_checkstring(L, 2);

        std::filesystem::path filePath = get_save_data_path(modReference, filename);
        if (!std::filesystem::exists(filePath))
        {
            lua_pushnil(L);
            return 1;
        }

        std::ifstream file = std::ifstream(filePath, std::ios::binary);
        if (!file)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to open Mod file for reading at \"%s\"\n", filePath.string().c_str());
            lua_pushnil(L);
            return 1;
        }

        file.seekg(0, std::ios::end);
        size_t len = file.tellg();
        file.seekg(0);

        std::string data(len, '\0');
        file.read(data.data(), len);

        lua_pushlstring(L, data.data(), data.size());
        return 1;
    }

    LUA_FUNCTION(DeleteData)
    {
        ModReference& modReference = get_mod_reference(L, 1);
        std::string filename = luaL_checkstring(L, 2);

        std::filesystem::path filePath = get_save_data_path(modReference, filename);
        if (!std::filesystem::exists(filePath))
        {
            return 0;
        }

        std::error_code errorCode;
        std::filesystem::remove(filePath, errorCode);

        if (errorCode)
        {
            ZHL::Log(__LOG_ERROR_HEADER__ "unable to delete Mod file at \"%s\"\n", filePath.string().c_str());
        }

        return 0;
    }
}

void ESSM::Init::RegisterLuaInternals(lua_State *L)
{
    lua::TableAssoc(L, "GetEntitySaveStateId", LuaFunctions::GetEntitySaveStateId);
    lua::TableAssoc(L, "SaveData", LuaFunctions::SaveData);
    lua::TableAssoc(L, "LoadData", LuaFunctions::LoadData);
    lua::TableAssoc(L, "DeleteData", LuaFunctions::DeleteData);
}

#ifndef NDEBUG

#include "chrono"

HOOK_STATIC(Manager, Update, (bool unk) -> void, __stdcall)
{
    super(unk);

    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();
    ESSM::StabilityChecker::CheckStability();
    auto end = clock::now();

    unsigned long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    //KAGE::_LogMessage(0, "check_stability time taken: %llu\n", duration);
}

#endif