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

#include <fstream>
#include <algorithm>

#undef max
#undef ERROR_INVALID_STATE
#define DEBUG_MSG true

namespace {
    struct HijackedState {
        int intStorage7 = 0;
        short gridSpawnIdx = 0;
    };
};

#if DEBUG_MSG
    static void LogDebug(const char* format, ...) noexcept
    {
        va_list args;
        va_start(args, format);
        std::string message = REPENTOGON::StringFormat(format, args);
        va_end(args);

        KAGE::_LogMessage(0, message.c_str());
    }
#else
    static void LogDebug(const char* format, ...) noexcept {};
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

    static Data s_systemData;
}

namespace ESSM::EntityIterators
{
    constexpr size_t ESAU_JR_PLAYERS = 4;

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
        
                if (player->_backupPlayer)
                {
                    func(player->_backupPlayer->_movingBoxContents, std::forward<Args>(args)...);
                }
        
                if (player->_hasUnlistedRestoreState)
                {
                    func(player->_unlistedRestoreState._movingBoxContents, std::forward<Args>(args)...);
                }
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
    static uint32_t NewId() noexcept
    {
        if (s_systemData.reusableIds.size() > 0)
        {
            uint32_t id = s_systemData.reusableIds.back();
            s_systemData.reusableIds.pop_back();
            LogDebug("[ESSM] New ID: %u\n", id);
            return id;
        }
    
        uint32_t id = s_systemData.totalIds;
        s_systemData.totalIds++;
        s_systemData.hijackedStates.emplace_back();
        LogDebug("[ESSM] New ID: %u\n", id);
        return id;
    }

    static void ClearId(uint32_t id) noexcept
    {
        s_systemData.reusableIds.push_back(id);
        LogDebug("[ESSM] Cleared ID: %u\n", id);
    }
}

namespace ESSM::HijackManager
{
    constexpr short READ_MARKER = 0x5248;
    constexpr short HIJACK_MARKER = 0x5247;
    constexpr short WRITTEN_MARKER = 0x5246;

    static bool IsHijacked(const EntitySaveState& data) noexcept
    {
        return data.gridSpawnIdx == HIJACK_MARKER;
    }
    
    static bool IsWritten(const EntitySaveState& data) noexcept
    {
        return data.gridSpawnIdx == WRITTEN_MARKER;
    }
    
    static bool IsRead(const EntitySaveState& data) noexcept
    {
        return data.gridSpawnIdx == READ_MARKER;
    }
    
    static void SetHijacked(EntitySaveState& data) noexcept
    {
        data.gridSpawnIdx = HIJACK_MARKER;
    }
    
    static void SetWritten(EntitySaveState& data) noexcept
    {
        data.gridSpawnIdx = WRITTEN_MARKER;
    }
    
    static void SetRead(EntitySaveState& data) noexcept
    {
        data.gridSpawnIdx = READ_MARKER;
    }
    
    static uint32_t GetId(const EntitySaveState& data) noexcept
    {
        return data._intStorage7;
    }
    
    static void SetId(EntitySaveState& data, uint32_t id) noexcept
    {
        data._intStorage7 = id;
    }
    
    static void Hijack(EntitySaveState& data, uint32_t id) noexcept
    {
        HijackedState& hijackedState = s_systemData.hijackedStates[id];
        hijackedState.gridSpawnIdx = data.gridSpawnIdx;
        data.gridSpawnIdx = HIJACK_MARKER;
        hijackedState.intStorage7 = data._intStorage7;
        data._intStorage7 = id;
    }
    
    static uint32_t NewHijack(EntitySaveState& data) noexcept
    {
        assert(!IsHijacked(data));
        uint32_t id = ESSM::IdManager::NewId();
        LogDebug("[ESSM] New ID: %u\n", id);
        LogDebug("[ESSM] Saved %d, %d, %d\n", data.type, data.variant, data.subtype);
        Hijack(data, id);
        return id;
    }

    static void RestoreHijack(EntitySaveState& data, HijackedState& hijackedState) noexcept
    {
        data.gridSpawnIdx = hijackedState.gridSpawnIdx;
        data._intStorage7 = hijackedState.intStorage7;
    }
    
    static uint32_t UnHijack(EntitySaveState& data) noexcept
    {
        assert(IsHijacked(data));
        uint32_t id = GetId(data);
        RestoreHijack(data, s_systemData.hijackedStates[id]);
        return id;
    }
}

namespace ESSM
{
    class StabilityChecker
    {
        private: enum BasicErrors
        {
            ERROR_DOUBLE_CLEAR = 0,
            ERROR_INVALID_STATE = 1,
            ERROR_INVALID_ID = 2,
            ERROR_DUPLICATE = 3,
            ERROR_INCORRECT_CLEAR = 4,
            ERROR_LEAK = 5,

            NUM_ERRORS
        };

        private: struct CheckData
        {
            private: std::vector<uintptr_t> m_checks;
            private: std::vector<EntitySaveState*> m_invalidStates;
            private: std::vector<EntitySaveState*> m_invalidId;
            private: std::vector<EntitySaveState*> m_duplicates;
            private: std::vector<EntitySaveState*> m_incorrectClears;
            private: std::vector<uint32_t> m_leakedIds;
            private: std::bitset<NUM_ERRORS> m_errors;

            friend StabilityChecker;
        };

        private: static void check_save_state(CheckData& checks, EntitySaveState& saveState) noexcept
        {
            bool validState = HijackManager::IsHijacked(saveState);
            assert(validState); // written and read states are treated as invalid, because they should only be marked as such in an isolated moment.
            if (!validState)
            {
                checks.m_invalidStates.emplace_back(&saveState);
                checks.m_errors.set(BasicErrors::ERROR_INVALID_STATE, true);
                return;
            }

            uint32_t id = HijackManager::GetId(saveState);
            bool validId = id < s_systemData.totalIds;
            assert(validId); // this is really messed up
            if (!validId)
            {
                checks.m_invalidId.emplace_back(&saveState);
                checks.m_errors.set(BasicErrors::ERROR_INVALID_ID, true);
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
                    checks.m_incorrectClears.emplace_back(&saveState);
                    checks.m_errors.set(BasicErrors::ERROR_INCORRECT_CLEAR, true);
                }
                else
                {
                    checks.m_duplicates.emplace_back(&saveState);
                    checks.m_errors.set(BasicErrors::ERROR_DUPLICATE, true);
                }
                return;
            }

            checks.m_checks[id] = saveStatePtr;

            EntitySaveState* innerSaveState = saveState.entitySaveState.saveState;
            if (innerSaveState)
            {
                check_save_state(checks, *innerSaveState);
            }
        }

        private: static void check_save_state_vector(CheckData& checks, std::vector<EntitySaveState>& saveStates) noexcept
        {
            for (EntitySaveState& saveState : saveStates)
            {
                check_save_state(checks, saveState);
            }
        }

        private: static constexpr auto check_vector_lambda = [](std::vector<EntitySaveState>& vec, CheckData& checks) noexcept(noexcept(check_save_state_vector(checks, vec))) { check_save_state_vector(checks, vec); };

        private: static constexpr size_t MAX_PICKUPS = 512;
        private: static void check_entity_list_stability(CheckData& checks) noexcept
        {
            const EntityFactory* factory = g_Game->_entityFactory;
            const Entity_Pickup* pickups = factory->_pickup;

            for (size_t i = 0; i < MAX_PICKUPS; i++)
            {
                const Entity_Pickup& pickup = pickups[i];
                EntitySaveState* flipState = pickup._flipSaveState.saveState;
                if (flipState)
                {
                    check_save_state(checks, *flipState);
                }
            }
        }

        private: static void check_run_stability(CheckData& checks) noexcept
        {
            ESSM::EntityIterators::InGame::All(check_vector_lambda, checks);
        }

        private: static void check_game_state_stability(GameState& gameState, CheckData& checks) noexcept
        {
            ESSM::EntityIterators::InGameState::All(gameState, check_vector_lambda, checks);
        }

        public: static void CheckStability() noexcept
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
                    checks.m_errors.set(BasicErrors::ERROR_DOUBLE_CLEAR, true);
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
                    checks.m_errors.set(BasicErrors::ERROR_LEAK, true);
                }
            }

            // TODO: Handle errors
        }
    };
}

namespace ESSM::BaseOperations
{
    static void ClearSaveState(const EntitySaveState& saveState) noexcept
    {
        assert(HijackManager::IsHijacked(saveState));
        uint32_t id = HijackManager::GetId(saveState);
        IdManager::ClearId(id);
        // clear entity (either store entity id and defer it for later or directly clear it)
    }
    
    static void ClearVector(const std::vector<EntitySaveState>& saveEntities) noexcept
    {
        for (const EntitySaveState& saveState : saveEntities)
        {
            ClearSaveState(saveState);
        }
    }

    static void CopySaveState(EntitySaveState& saveState) noexcept
    {
        assert(HijackManager::IsHijacked(saveState));
        uint32_t sourceId = HijackManager::GetId(saveState);
        uint32_t targetId = IdManager::NewId();
        HijackManager::SetId(saveState, targetId);
        s_systemData.hijackedStates[targetId] = s_systemData.hijackedStates[sourceId];
        LogDebug("[ESSM] New ID: %u\n", targetId);
        LogDebug("[ESSM] Copied %d -> %d\n", sourceId, targetId);
    }

    static void CopyVector(std::vector<EntitySaveState>& saveEntities) noexcept
    {
        for (EntitySaveState& saveState : saveEntities)
        {
            CopySaveState(saveState);
            // copy entity (either store entity id and defer it for later or directly clear it)
        }
    }

    static auto ClearVectorLambda = [](const std::vector<EntitySaveState>& vec) noexcept(noexcept(ClearVector(vec))) { ClearVector(vec); };
    static auto CopyVectorLambda = [](std::vector<EntitySaveState>& vec) noexcept(noexcept(CopyVector(vec))) { CopyVector(vec); };
}

static void SaveEntity(Entity& entity, EntitySaveState& data, uint32_t id) noexcept
{
    // TODO: save entity
}

static void RestoreEntity(Entity& entity, EntitySaveState& data, uint32_t id) noexcept
{
    // TODO: restore entity
}

namespace ESSM::SaveData
{
    constexpr uint32_t SAVE_VERSION = 1;

    struct SaveFile
    {
        std::vector<HijackedState> entities;
    };

    namespace Section
    {
        constexpr uint32_t ENTITY_START = 0x73454E54; // sENT
        constexpr uint32_t ENTITY_END = 0x65454E54; // eENT
        constexpr uint32_t ENTITY_SIZE = 6;
        constexpr uint32_t MAX_ENTITY_COUNT = 0xFFFF;
    }

    enum ReadErrors
    {
        ERROR_INVALID_STATE = 0,
        TRIGGERED_EXCEPTION = 1,

        NUM_ERRORS
    };

    template<typename T> struct bitset_size;
    template<std::size_t N> struct bitset_size<std::bitset<N>> {
        static constexpr std::size_t value = N;
    };

    static_assert(bitset_size<decltype(ReadState::errors)>::value == NUM_ERRORS, "NUM_ERRORS doesn't match ReadState errors");

    namespace Operations
    {
        static void ClearSaveVector(const std::vector<EntitySaveState>& saveEntities) noexcept
        {
            for (const EntitySaveState& saveState : saveEntities)
            {
                if (ESSM::HijackManager::IsHijacked(saveState))
                {
                    ESSM::BaseOperations::ClearSaveState(saveState);
                }
            }
        }

        static void write_entity_smart_pointer(EntitySaveState& data, WriteState& writeState)
        {
            assert(HijackManager::IsHijacked(data) || HijackManager::IsWritten(data));
            if (HijackManager::IsHijacked(data))
            {
                auto& writtenSaveStates = writeState.writtenSaveStates;
                uint32_t writeId = writtenSaveStates.size();
    
                uint32_t id = HijackManager::GetId(data);
                HijackManager::SetWritten(data);
                HijackManager::SetId(data, writeId);
    
                writtenSaveStates.emplace_back(&data, id);
            }
            else if (!HijackManager::IsWritten(data))
            {
                throw std::runtime_error("invalid EntitySaveState state during write");
            }
    
            EntitySaveState* innerSmartPointer = data.entitySaveState.saveState;
            if (innerSmartPointer)
            {
                write_entity_smart_pointer(*innerSmartPointer, writeState);
            }
        }
    
        static void write_save_state(EntitySaveState& data, WriteState& writeState)
        {
            assert(HijackManager::IsHijacked(data));
            if (!HijackManager::IsHijacked(data))
            {
                throw std::runtime_error("invalid EntitySaveState state during write");
            }

            auto& writtenSaveStates = writeState.writtenSaveStates;
            uint32_t writeId = writtenSaveStates.size();
    
            uint32_t id = HijackManager::GetId(data);
            HijackManager::SetWritten(data);
            HijackManager::SetId(data, writeId);
    
            writtenSaveStates.emplace_back(&data, id);
    
            EntitySaveState* smartPointer = data.entitySaveState.saveState;
            if (smartPointer)
            {
                write_entity_smart_pointer(*smartPointer, writeState);
            }
        }
    
        static void read_save_state(EntitySaveState& data, ReadState& readState) noexcept
        {
            if (!HijackManager::IsWritten(data))
            {
                if (!HijackManager::IsRead(data))
                {
                    readState.errors.set(ReadErrors::ERROR_INVALID_STATE); // invalid EntitySaveState state during read
                }
            }
            else
            {
                // Due to how flip state are implemented, there can be duplicate ids. As such finding a duplicate here is not an error.
                uint32_t readId = HijackManager::GetId(data);
                readState.maxId = std::max(readState.maxId, readId);
                readState.readSaveStates.push_back(&data);
            }
            
            HijackManager::SetRead(data);
            EntitySaveState* smartPointer = data.entitySaveState.saveState;
            if (smartPointer)
            {
                read_save_state(*smartPointer, readState);
            }
        }
    
        static void write_vector(std::vector<EntitySaveState>& saveEntities, WriteState& writeState) noexcept(noexcept(write_save_state(EntitySaveState(), writeState)))
        {
            for (EntitySaveState& saveState : saveEntities)
            {
                write_save_state(saveState, writeState);
            }
        }
    
        static void read_vector(std::vector<EntitySaveState>& saveEntities, ReadState& readState) noexcept(noexcept(read_save_state(EntitySaveState(), readState)))
        {
            for (EntitySaveState& saveState : saveEntities)
            {
                read_save_state(saveState, readState);
            }
        }
    
        static auto ClearSaveVectorLambda = [](const std::vector<EntitySaveState>& vec) noexcept(noexcept(ClearSaveVector(vec))) { ClearSaveVector(vec); };
        static auto WriteVectorLambda = [](std::vector<EntitySaveState>& vec, WriteState& writeState) noexcept(noexcept(write_vector(vec, writeState))) { write_vector(vec, writeState); };
        static auto ReadVectorLambda = [](std::vector<EntitySaveState>& vec, ReadState& readState) noexcept(noexcept(read_vector(vec, readState))) { read_vector(vec, readState); };
    }

    static void resolve_read_errors(ReadState& readState) noexcept
    {
        if (readState.errors.test(ReadErrors::ERROR_INVALID_STATE))
        {
            ZHL::Log("[ERROR] [ESSM] - invalid EntitySaveState state during read\n");
        }
    }

    static void clear_write_state(WriteState& writeState) noexcept
    {
        writeState.writtenSaveStates.clear();
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

    static std::string get_save_data_path(const std::string& fileName) noexcept
    {
        return REPENTOGON::StringFormat("%s/EntitySaveStateManagement/%s.dat", REPENTOGON::GetRepentogonDataPath(), fileName.c_str());
    }

    static bool guarantee_parent_path_exists(std::filesystem::path filePath) noexcept
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

    static void remove_file(const std::filesystem::path& filePath) noexcept
    {
        std::error_code errorCode;
        std::filesystem::remove(filePath, errorCode);
        if (errorCode)
        {
            ZHL::Log("[ERROR] [ESSM] - unable to delete file at \"%s\"\n", filePath.string().c_str());
        }
    }

    WriteState WriteGameState() noexcept
    {
        Manager* manager = g_Manager;
        GameState& gameState = manager->_gamestate;
    
        WriteState writeState = WriteState();
        try
        {
            ESSM::EntityIterators::InGameState::All(gameState, Operations::WriteVectorLambda, writeState);
        }
        catch(const std::runtime_error& e)
        {
            ZHL::Log("[ERROR] [ESSM] - unable to write game state : %s\n", e.what());
            clear_write_state(writeState);
            return writeState;
        }

        // serialize entities from write State
        return writeState;
    }

    ReadState ReadGameState() noexcept
    {
        Manager* manager = g_Manager;
        GameState& gameState = manager->_gamestate;
    
        ReadState readState = ReadState();
        try
        {
            ESSM::EntityIterators::InGameState::All(gameState, Operations::ReadVectorLambda, readState);
        }
        catch(const std::runtime_error& e)
        {
            ZHL::Log("[ERROR] [ESSM] - unable to read game state : %s\n", e.what());
            readState.errors.set(ReadErrors::TRIGGERED_EXCEPTION);
        }

        resolve_read_errors(readState);
        return readState;
    }

    void RestoreWrittenStates(WriteState& writeState) noexcept
    {
        for (std::pair<EntitySaveState*, uint32_t> writtenSaveState : writeState.writtenSaveStates)
        {
            EntitySaveState& saveState = *writtenSaveState.first;
            uint32_t id = writtenSaveState.second;
    
            assert(HijackManager::IsWritten(saveState));
            HijackManager::SetHijacked(saveState);
            HijackManager::SetId(saveState, id);
        }
    }

    void hijack_read_states(ReadState& readState, SaveFile& saveFile) noexcept
    {
        std::vector<HijackedState>& entities = saveFile.entities;
        uint32_t maxId = entities.size() - 1;

        for (EntitySaveState* readSaveState : readState.readSaveStates)
        {
            EntitySaveState& saveState = *readSaveState;

            assert(HijackManager::IsRead(saveState));
            uint32_t readId = HijackManager::GetId(saveState);
            // all previous steps should have guaranteed this
            assert(readId <= maxId);
            
            uint32_t id = ESSM::IdManager::NewId();
            HijackManager::SetHijacked(saveState);
            HijackManager::SetId(saveState, id);
            s_systemData.hijackedStates[id] = entities[readId];

            readState.restoredSaveStates.emplace_back(readId, id);
        }
    }

    static void serialize_entities(std::ofstream& file, const WriteState& writeState)
    {
        uint32_t size = static_cast<uint32_t>(writeState.writtenSaveStates.size());
        if (size > Section::MAX_ENTITY_COUNT)
        {
            throw std::runtime_error("Too many entities to serialize");
        }

        write_binary(file, Section::ENTITY_START);
        write_binary(file, size);

        for (auto& saveState : writeState.writtenSaveStates)
        {
            uint32_t id = saveState.second;
            HijackedState& hijackedState = s_systemData.hijackedStates[id];

            write_binary(file, hijackedState.intStorage7); // UNHIJACKED_ID
            write_binary(file, hijackedState.gridSpawnIdx); // UNHIJACKED_MARKER
        }

        write_binary(file, Section::ENTITY_END);
    }

    static void deserialize_entities(std::ifstream& file, ReadState& readState, SaveFile& saveFile)
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

        if (fileCount <= readState.maxId)
        {
            throw std::runtime_error("Not enough entities in save data");
        }

        if (fileCount > Section::MAX_ENTITY_COUNT)
        {
            throw std::runtime_error("Entity count too large");
        }

        // validate file array size
        auto pos = file.tellg();
        file.seekg(0, std::ios::end);
        std::streamoff fileSize = file.tellg();
        file.seekg(pos); // restore

        std::streamoff requiredSize = std::streamoff(pos) + std::streamoff(fileCount) * Section::ENTITY_SIZE + 4; // + footer
        if (fileSize < requiredSize)
        {
            throw std::runtime_error("File truncated or corrupt");
        }

        uint32_t neededCount = std::max(fileCount, readState.maxId + 1);
        try {
            entities.reserve(neededCount);
        }
        catch (const std::bad_alloc&) {
            throw std::runtime_error("Out of memory");
        }

        uint32_t remaining = fileCount - neededCount;

        for (uint32_t i = 0; i < neededCount; ++i)
        {
            HijackedState& hijackedState = entities.emplace_back();
            read_binary(file, hijackedState.intStorage7); // UNHIJACKED_ID
            read_binary(file, hijackedState.gridSpawnIdx); // UNHIJACKED_MARKER
        }

        file.seekg(remaining * Section::ENTITY_SIZE, std::ios::cur);
        if (!file)
        {
            throw std::runtime_error("Failed to skip extra entity data");
        }

        uint32_t sectionFooter = 0;
        read_binary(file, sectionFooter);
        if (sectionFooter != Section::ENTITY_END)
        {
            throw std::runtime_error("Incorrect section footer");
        }
    }

    static void serialize_save(std::ofstream& file, const WriteState& writeState)
    {
        uint32_t gameChecksum = g_Manager->_gamestate._checksum;

        write_binary(file, SAVE_VERSION);
        write_binary(file, gameChecksum);

        serialize_entities(file, writeState);
    }

    static void deserialize_save(std::ifstream& file, ReadState& readState, SaveFile& saveFile)
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

    void Serialize(const std::string& fileName, const WriteState& writeState) noexcept
    {
        std::filesystem::path filePath = get_save_data_path(fileName);
        bool exists = guarantee_parent_path_exists(filePath);
        if (!exists)
        {
            ZHL::Log("[ERROR] [ESSM] - unable to create directory for save file at \"%s\"\n", filePath.string().c_str());
            return;
        }

        std::ofstream file = std::ofstream(filePath, std::ios::binary);
        if (!file.is_open())
        {
            ZHL::Log("[ERROR] [ESSM] - unable to open save file for writing at \"%s\"\n", filePath.string().c_str());
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
            ZHL::Log("[ERROR] [ESSM] - unable to serialize save file at \"%s\": %s\n", filePath.string().c_str(), e.what());
            file.close();
            remove_file(filePath);
            return;
        }

        ZHL::Log("[INFO] [ESSM] - successfully saved %s to \"%s\"\n", fileName.c_str(), filePath.string().c_str());
    }

    static bool deserialize(const std::string& fileName, ReadState& readState) noexcept
    {
        // we should have never entered with errors here in the first place
        if (readState.errors.any())
        {
            ZHL::Log("[ERROR] [ESSM] - pre-existing errors in read state when deserializing\n");
            return false;
        }

        if (!NeedsHandling(readState))
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
            ZHL::Log("[ERROR] [ESSM] - unable to open save file for reading at \"%s\"\n", filePath.string().c_str());
            return false;
        }

        SaveFile saveFile;
        try
        {
            deserialize_save(file, readState, saveFile);
        }
        catch(const std::runtime_error& e)
        {
            ZHL::Log("[ERROR] [ESSM] - unable to deserialize save file at \"%s\": %s\n", filePath.string().c_str(), e.what());
            return false;
        }

        file.close();
        hijack_read_states(readState, saveFile);
        // TODO: Send Deserialize Event
        ZHL::Log("[INFO] [ESSM] - successfully loaded %s from \"%s\"\n", fileName.c_str(), filePath.string().c_str());
        return true;
    }

    bool Deserialize(const std::string& fileName, ReadState& readState) noexcept
    {
        bool success = deserialize(fileName, readState);

        // free the memory since we no longer need it
        readState.readSaveStates.clear();

        return success;
    }
}

static std::pair<Entity*, EntitySaveState*> s_minecartEntity = {nullptr, nullptr};

HOOK_METHOD(Room, save_entity, (Entity* entity, EntitySaveState* data, bool savingMinecartEntity) -> bool)
{
    std::pair<Entity*, EntitySaveState*> minecartEntity = s_minecartEntity;
    s_minecartEntity.first = nullptr; s_minecartEntity.second = nullptr;

    assert(!ESSM::HijackManager::IsHijacked(*data)); // these are always newly created so they should not be already hijacked
    bool saved = super(entity, data, savingMinecartEntity);

    if (!saved)
    {
        if (minecartEntity.first)
        {
            assert(entity->_type == eEntityType::ENTITY_MINECART); // the next thing after a minecart entity save must always be a minecart
            EntitySaveState* flipState = EntitySaveState::Pickup::GetFlipSaveState(*minecartEntity.second);
            if (flipState)
            {
                ESSM::HijackManager::UnHijack(*flipState);
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
            ESSM::BaseOperations::CopySaveState(*flipState);
        }

        uint32_t id = ESSM::HijackManager::NewHijack(*minecartEntity.second);
        SaveEntity(*minecartEntity.first, *minecartEntity.second, id);
    }

    EntitySaveState* flipState = EntitySaveState::Pickup::GetFlipSaveState(*data);
    if (flipState)
    {
        ESSM::BaseOperations::CopySaveState(*flipState);
    }

    uint32_t id = ESSM::HijackManager::NewHijack(*data);
    SaveEntity(*entity, *data, id);
    return saved;
}

HOOK_METHOD(Room, restore_entity, (Entity* entity, EntitySaveState* data) -> void)
{
    uint32_t id = ESSM::HijackManager::UnHijack(*data);

    super(entity, data);

    ESSM::HijackManager::Hijack(*data, id);
    RestoreEntity(*entity, *data, id);
}

HOOK_METHOD(Level, Init, (bool resetLilPortalRoom) -> void)
{
    ESSM::BaseOperations::ClearVector(g_Game->_myosotisPickups);
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

        const RoomDescriptor& room = roomList[i];
        ESSM::BaseOperations::ClearVector(room.SavedEntities);
    }

    super(resetLilPortalRoom);
}

HOOK_METHOD(Level, DEBUG_goto_room, (RoomConfig_Room * roomConfig) -> void)
{
    constexpr size_t LIST_IDX = eGridRooms::MAX_GRID_ROOMS + (-eGridRooms::ROOM_DEBUG_IDX) - 1;

    Game* game = g_Game;
    RoomDescriptor& room = game->_gridRooms[LIST_IDX];
    ESSM::BaseOperations::ClearVector(room.SavedEntities);

    super(roomConfig);
}

HOOK_METHOD(Level, InitializeGenesisRoom, () -> void)
{
    constexpr size_t LIST_IDX = eGridRooms::MAX_GRID_ROOMS + (-eGridRooms::ROOM_GENESIS_IDX) - 1;

    Game* game = g_Game;
    RoomDescriptor& room = game->_gridRooms[LIST_IDX];
    ESSM::BaseOperations::ClearVector(room.SavedEntities);

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
        ESSM::BaseOperations::ClearVector(room.SavedEntities);
    }

    return super(currentIdx, destinationIdx, direction);
}

HOOK_METHOD(Game, SaveBackwardsStage, (int stage) -> void)
{
    BackwardsStageDesc& backwardsStage = g_Game->_backwardsStages[stage - 1];
    ESSM::EntityIterators::Structure::BackwardsStage(backwardsStage, ESSM::BaseOperations::ClearVectorLambda);

	super(stage);

    ESSM::EntityIterators::Structure::BackwardsStage(backwardsStage, ESSM::BaseOperations::CopyVectorLambda);
}

// Clear backwards stage save state, even though the game simply sets room count to 0, to avoid having to iterate "uninitialized" RoomDescriptors in the BackwardsStage. 
HOOK_METHOD(Game, ResetState, () -> void)
{
    ESSM::EntityIterators::InGame::AllBackwardsStages(ESSM::BaseOperations::ClearVectorLambda);
	super();
}

HOOK_METHOD(GameState, Clear, () -> void)
{
    LogDebug("[ESSM] Start GameState::Clear\n");

    ESSM::EntityIterators::InGameState::AllRooms(*this, ESSM::SaveData::Operations::ClearSaveVectorLambda);
    ESSM::EntityIterators::InGameState::AllBackwardsStages(*this, ESSM::SaveData::Operations::ClearSaveVectorLambda);
    // player methods are handled by GameStatePlayer::Init
    super();

    LogDebug("[ESSM] End GameState::Clear\n");
}

HOOK_METHOD(Game, SaveState, (GameState* state) -> void)
{
    LogDebug("[ESSM] Start Game::SaveState\n");

    // there is no need to clear the state since SaveState always calls Clear before saving
	super(state);
    ESSM::EntityIterators::InGameState::AllRooms(*state, ESSM::BaseOperations::CopyVectorLambda);
    ESSM::EntityIterators::InGameState::AllBackwardsStages(*state, ESSM::BaseOperations::CopyVectorLambda);
    // players are handled in their appropriate methods, since we would be clearing twice otherwise

    LogDebug("[ESSM] End Game::SaveState\n");
}

HOOK_METHOD(Game, RestoreState, (GameState* state, bool startGame) -> void)
{
    LogDebug("[ESSM] Start Game::RestoreState\n");

    ESSM::EntityIterators::InGame::AllBackwardsStages(ESSM::BaseOperations::ClearVectorLambda);
    super(state, startGame);
    // copy is in a separate patch as copying it here might cause problems due to callbacks running in the mean time.

    LogDebug("[ESSM] End Game::RestoreState\n");
}

HOOK_METHOD(Level, RestoreGameState, (GameState* state) -> void)
{
    // Manual clear the lil portal room
    constexpr size_t LIL_PORTAL_IDX = (-eGridRooms::ROOM_LIL_PORTAL_IDX) - 1;
    ESSM::BaseOperations::ClearVector(g_Game->_negativeGridRooms[LIL_PORTAL_IDX].SavedEntities);
    super(state);
}

HOOK_METHOD(GameStatePlayer, Init, () -> void)
{
    ESSM::SaveData::Operations::ClearSaveVector(this->_movingBoxContents);
    super();
}

HOOK_METHOD(Entity_Player, StoreGameState, (GameStatePlayer* saveState, bool saveTemporaryFamiliars) -> void)
{
    super(saveState, saveTemporaryFamiliars);
    ESSM::BaseOperations::CopyVector(saveState->_movingBoxContents);
}

HOOK_METHOD(Entity_Player, RestoreGameState, (GameStatePlayer * saveState) -> void)
{
    ESSM::BaseOperations::ClearVector(this->_movingBoxContents);
    super(saveState);
    ESSM::BaseOperations::CopyVector(this->_movingBoxContents);
}

static void __stdcall restore_game_state_backwards_rooms() noexcept
{
    ESSM::EntityIterators::InGame::AllBackwardsStages(ESSM::BaseOperations::CopyVectorLambda);
}

static void Patch_GameRestoreState_PostBackwardsStageDescRestore() noexcept
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

static void __stdcall asm_clear_room_vector() noexcept
{
    RoomDescriptor& room = *g_Game->_room->_descriptor;
    ESSM::BaseOperations::ClearVector(room.SavedEntities);
}

static void Patch_RoomSaveState_ClearVector() noexcept
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

static void Patch_RoomRestoreState_ClearVector() noexcept
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

static void __stdcall asm_copy_myosotis_pickups() noexcept
{
    ESSM::BaseOperations::CopyVector(g_Game->_myosotisPickups);
}

static void Patch_LevelInit_PostMyosotisEffect() noexcept
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

static void __stdcall restore_level_game_state() noexcept
{
    ESSM::EntityIterators::InGame::AllRooms(ESSM::BaseOperations::CopyVectorLambda);
}

static void Patch_LevelRestoreGameState_PreRoomLoad() noexcept
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

static void __stdcall asm_clear_vector_pre_backwards_assign(std::vector<EntitySaveState>* saveStateVector) noexcept
{
    ESSM::BaseOperations::ClearVector(*saveStateVector);
}

static void __stdcall asm_copy_vector_post_backwards_assign(std::vector<EntitySaveState>* saveStateVector) noexcept
{
    ESSM::BaseOperations::CopyVector(*saveStateVector);
}

static void Patch_LevelPlaceRoomsBackwards_Boss_AssignEntitySaveStateVector() noexcept
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

static void Patch_LevelPlaceRoomsBackwards_Treasure_AssignEntitySaveStateVector() noexcept
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

static void __stdcall asm_clear_moving_box_vector(Entity_Player& player) noexcept
{
    ESSM::BaseOperations::ClearVector(player._movingBoxContents);
}

static void Patch_PlayerUseActiveItem_MovingBoxClearVector() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseActiveItem_MovingBox_ClearVector);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::UseActiveItem for  at %p\n", addr);

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

// use fastcall to emulate thiscall
static void __fastcall asm_clear_smart_pointer(EntitySaveState* saveState) noexcept
{
    if (ESSM::HijackManager::IsHijacked(*saveState))
    {
        uint32_t id = ESSM::HijackManager::GetId(*saveState);
        ESSM::BaseOperations::ClearSaveState(*saveState);
        LogDebug("[ESSM] Smart pointer Cleared: %u\n", id);
    }

    saveState->destructor();
}

static void Patch_ReferenceCount_EntitySaveStateDestructor() noexcept
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::KAGE_ReferenceCount_EntitySaveState_DecrementReference_SaveStateDestructor);
    ZHL::Log("[REPENTOGON] Patching KAGE::ReferenceCount<EntitySaveState>::DecrementReference for SaveStateManagement at %p\n", addr);

    ASMPatch patch;
    patch.AddInternalCall(asm_clear_smart_pointer);

    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void __stdcall asm_hijack_new_flip_state(EntitySaveState& saveState) noexcept
{
    uint32_t id = ESSM::HijackManager::NewHijack(saveState);
    LogDebug("[ESSM] New Flip State: %u\n", id);
}

static void Patch_PickupInitFlipState_CreateSaveState() noexcept
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

static void __stdcall asm_flip_restore(Entity& entity, EntitySaveState& saveState) noexcept
{
    RestoreEntity(entity, saveState, ESSM::HijackManager::GetId(saveState));
}

static void Patch_PickupTryFlip_RestoreFlipState() noexcept
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

static void __stdcall change_mineshaft_room(size_t listIdx) noexcept
{
    Game* game = g_Game;
    RoomDescriptor& room = game->_gridRooms[listIdx];
    ESSM::BaseOperations::ClearVector(room.SavedEntities);
}

static void Patch_EntityNPCAiMothersShadow_ChangeMineshaftRoom() noexcept
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

void ESSM::ApplyPatches() noexcept
{
    Patch_ReferenceCount_EntitySaveStateDestructor();
    Patch_LevelInit_PostMyosotisEffect();
    Patch_LevelRestoreGameState_PreRoomLoad();
    Patch_LevelPlaceRoomsBackwards_Boss_AssignEntitySaveStateVector();
    Patch_LevelPlaceRoomsBackwards_Treasure_AssignEntitySaveStateVector();
    Patch_RoomSaveState_ClearVector();
    Patch_RoomRestoreState_ClearVector();
    Patch_GameRestoreState_PostBackwardsStageDescRestore();
    Patch_PlayerUseActiveItem_MovingBoxClearVector();
    Patch_PickupInitFlipState_CreateSaveState();
    Patch_PickupTryFlip_RestoreFlipState();
    Patch_EntityNPCAiMothersShadow_ChangeMineshaftRoom();
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