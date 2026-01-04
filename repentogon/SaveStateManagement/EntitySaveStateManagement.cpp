/* SYSTEM OVERVIEW:
*  This system is meant to provide a way to extend the various save state structures.
*  This is done by assigning an id to each active structure, which can then be used by outside systems to correlate to the main structure.
*  Because there isn't always free space in every type of structures, it was necessary to hijack the structures and replace one of the fields with the id.
*    A second field is also used to confirm that the structure has been properly hijacked and, additionally, as a general marker to assist other operations.
*    The fields that are chosen to be hijacked should be ones that are not actively read in operations that are not a Restore of the entity state.
*      Additionally, for the marker, the original field should be one that has a high likelihood of not being naturally set to one of the marker values.
*  Due to this it is necessary to store a copy of the original state of the non-hijacked values that is then restored prior to an entity Restore.
*  
*  The core operations of the system are:
*  - Hijacking: Which involdes generating a new id copying the original values into a separate structure, then replacing the fields with the Id and the Hijacked Marker.
*      Called on Save State Creation (or if preallocated when considered "active").
*  - Storing: Which usually coincides with Hijacking, if not then it is the same as hijacking minus generating a new id.
*  - Restoring: Replacing the marker and id value to their original non-hijacked values before calling the vanilla restore function, then restoring the marker and id.
*  - Clearing: Deleting the id of the structure
*      Called when the Save State is deleted (or if preallocated when going from "active" to "inactive").
*  - Copy: Which involves generating a new id and copying the hijacked values from the source.
*      Given that the id replaces one of the original field, the game copies that value for us, allowing us to not have to know where the source actually comes from, meaning
*      we can simply read the current id as the id of the source, then replacing it with the newly generated one.
*  - Serialize: We generate a new id (local to the serialization process) for each written structure in the game state, which will then be serialized by the game.
*    We then serialize the non-hijacked values on our end, tying it to the local id. Then restore the original ids of the written structure
*  - Deserialize: We read each id that was written on serialization then act as if all the read structures are newly created and Hijack each of them, taking the original
*    non-hijacked values from our own save file.
*      If we cannot find an associated id from our own save file then we make the game fail deserialization.
*/

/* IMPLEMENTATION NOTES:
*  - There is no single point where the Level's rooms are reset or partially reset; each case must be found and handled individually.
*  - The main operators (like operator= or the destructor) for EntitySaveState have not been used as hook points, since many temporary states are created, and we do not need to signal their creation, copy or destruction.
*  - Some places manually create and insert EntitySaveStates into a container, without using a constructor; each of these must be found and handled.
*  - Due to this, manual work is required when save states created, copied or cleared by the game. As such, all points where an EntitySaveState is used must be reviewed.
*    - The only exception are SmartPointers, which have a single point of destruction, but their initialization and copy must still be handled manually.
*    - Because the EntitySaveState destructor always attempts to call an unused global Notify function, references to that function's memory address can be used to locate uses of EntitySaveState.
*    - There are unhandled cases where EntitySaveState and GameStatePlayer are used, but all of these are related to online play and have not been addressed.
*  - There is no mechanism that checks if an ID is cleared multiple times. We must ensure this never occurs.
*    - A possible improvement could involve marking cleared EntitySaveStates with a special marker to make detection safer. However, this would not prevent issues caused by duplicates, which are the main cause of multiple clears.
*  - Because of SmartPointers, it is not currently possible to fully clear the ID generator, as some references would persist after reset, causing incorrect clears.
*    - Full handling of SmartPointers would be required to safely enable full resets.
*    - Enabling this would make the system resilient to “memory leaks”, but might still allow incorrect clears.
*    - The addition of the Stability checker has kind of rendered this point meaningless as it is capable of detecting "memory leaks" and properly fix them.
*  - It is assumed that all Entity Stores are handled by Room::save_entity().
*  - It is assumed that all Entity restores are handled by Room::restore_entity(), except for Entity_Pickup::TryFlipState.
*  - It is assumed that GameState is in a cleared state when it is read.
*  - It is assumed that every time a GameState is cleared, GameState::Clear() is called.
*  - Confirm that Level::RestoreGameState does not reset the LilPortalRoom when calling reset_room_list, since we currently have to perform that clear manually.
*  - It is assumed that every time a player's state is cleared, GameStatePlayer::Init() is called.
*  - It is assumed that all player and familiar saves are handled by Entity_Player::StoreGameState().
*  - It is assumed that all player restores are handled by Entity_Player::RestoreGameState().
*  - It is assumed that all familiar restores are handled by Entity_Player::RestoreGameState_PostLevelInit().
*  - There is one known case where familiar data is modified manually outside the designated function; ensure all such cases are identified and handled.
*    - This specific case occurs when restoring backup players in PlayerManager::RestoreGameState(), which copies data from the save state into the unlisted save state.
*  - To avoid having to iterate over non "initialized" RoomDescriptors in BackwardsStage, when resetting the game's state, we diverge from the game's behavior by actually clearing the ids.
*    - In contrast, the game keeps the vector around and simply sets the room count to 0, only properly clearing it when saving a new RoomDescriptor.
*    - Hopefully this doesn't cause any issues, but, if we were to allow access to BackwardsStage, we still shouldn't grant access to the vector when the room technically doesn't exist.
*/

#include "EntitySaveStateManagement.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"
#include "Log.h"
#include "../MiscFunctions.h"
#include "../LuaInterfaces/LuaEntitySaveState.h"
#include "../LuaInterfaces/_Internals.h"
#include "../ImGuiFeatures/CustomImGui.h"
#include "../Patches/ExtraRenderSteps.h"

#include <fstream>
#include <algorithm>

#undef max
#undef ERROR_INVALID_STATE

#define DEBUG_MSG false
#define LOG_DEBUG_HEADER "[DEBUG] [ESSM] - "
#define LOG_INFO_HEADER "[INFO] [ESSM] - "
#define LOG_ERROR_HEADER "[ERROR] [ESSM] - "

using ClearedIds = std::vector<uint32_t>;
using CopiedIds = std::vector<std::pair<uint32_t, uint32_t>>;
using StatePtr = std::variant<GameStatePlayer*, FamiliarData*, EntitySaveState*>;
using CollectedStates = std::vector<StatePtr>;
namespace ErrorDisplay = ExtraRenderSteps::ErrorDisplay;

const size_t DEFAULT_COLLECT_RESERVE = (16'000 / sizeof(StatePtr)); // 16 KB should be enough to cover all cases.

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

// Data
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
    private:
        int m_storeEntity = LUA_NOREF;
        int m_restoreEntity = LUA_NOREF;
        int m_clearSaveStates = LUA_NOREF;
        int m_copySaveStates = LUA_NOREF;
        int m_serialize = LUA_NOREF;
        int m_preDeserialize = LUA_NOREF;
        int m_deserialize = LUA_NOREF;

    public:
        void BindLuaCallbacks(lua_State* L, int tblIdx);
        void StoreEntity(const Entity& entity, uint32_t saveStateId);
        void RestoreEntity(const Entity& entity, uint32_t saveStateId);
        void ClearStates(const std::vector<uint32_t>& saveStateIds);
        void CopyStates(const std::vector<std::pair<uint32_t, uint32_t>>& copyIds);
        void Serialize(const detail::SaveData::_WriteState& writeState, const std::string& filename, uint32_t checksum);
        void PreDeserialize(const std::string& filename);
        void Deserialize(const detail::SaveData::_ReadState& readState, const std::string& filename, uint32_t checksum);
    };

    static Data s_systemData;
    static LuaCallbacks s_luaCallbacks;
    static bool s_checkerErrorShown;
}

// forward declarations
namespace ESSM
{
    namespace LuaFunctions
    {
        static std::filesystem::path GetLuaSaveDataPath(const ModReference& mod, const std::string& filename);
    }
}

namespace ESSM::detail::Init
{
    void BindLuaCallbacks(lua_State* L, int tblIdx)
    {
        s_luaCallbacks.BindLuaCallbacks(L, tblIdx);
    }
}

namespace ESSM::PlayerIterators
{
    namespace InGame
    {
        template <typename Func, typename... Args>
        static void ForEach(Func&& func, Args&&... args)
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

            const size_t NUM_ESAU_JR_STATES = std::extent_v<decltype(playerManager._esauJrState)>;
            for (size_t i = 0; i < NUM_ESAU_JR_STATES; i++)
            {
                handlePlayer(playerManager._esauJrState[i]);
            }
        }
    }

    namespace InGameState
    {
        template <typename Func, typename... Args>
        static void ForEach(GameState& gameState, Func&& func, Args&&... args)
        {
            GameStatePlayer* players = gameState._players;
            size_t playerCount = gameState._playerCount;

            for (size_t i = 0; i < playerCount; i++)
            {
                func(players[i], std::forward<Args>(args)...);
            }

            const size_t NUM_ESAU_JR_STATES = std::extent_v<decltype(gameState._esauJrStates)>;
            GameStatePlayer* esauJrStates = gameState._esauJrStates;

            for (size_t i = 0; i < NUM_ESAU_JR_STATES; i++)
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
        static void ForEach(Func&& func, Args&&... args)
        {
            auto lambda = [&](GameStatePlayer& player) {
                for (auto& data : player._familiarData)
                {
                    func(data, std::forward<Args>(args)...);
                }
            };

            ESSM::PlayerIterators::InGame::ForEach(lambda);
        }
    }

    namespace InGameState
    {
        template <typename Func, typename... Args>
        static void ForEach(GameState& gameState, Func&& func, Args&&... args)
        {
            auto lambda = [&](GameStatePlayer& player) {
                for (auto& data : player._familiarData)
                {
                    func(data, std::forward<Args>(args)...);
                }
            };

            ESSM::PlayerIterators::InGameState::ForEach(gameState, lambda);
        }
    }
}

namespace ESSM::EntityIterators
{
    namespace detail
    {
        template <typename Func, typename... Args>
        static void iterate_vector(std::vector<EntitySaveState>& vec, Func&& func, Args&&... args)
        {
            for (auto& entity : vec)
            {
                func(entity, std::forward<Args>(args)...);
            }
        }
    }

    namespace Structure
    {
        template <typename Func, typename... Args>
        static void ForEachBackwardsStage(BackwardsStageDesc& backwardsStage, Func&& func, Args&&... args)
        {
            for (size_t i = 0; i < backwardsStage._bossRoomsCount; i++)
            {
                detail::iterate_vector(backwardsStage._bossRooms[i].SavedEntities, func, std::forward<Args>(args)...);
            }

            for (size_t i = 0; i < backwardsStage._treasureRoomsCount; i++)
            {
                detail::iterate_vector(backwardsStage._treasureRooms[i].SavedEntities, func, std::forward<Args>(args)...);
            }
        }
    }
    
    namespace InGame
    {
        template <typename Func, typename... Args>
        static void ForEachInRooms(Func&& func, Args&&... args)
        {
            Game* game = g_Game;
            RoomDescriptor* roomList = game->_gridRooms;
        
            for (size_t i = 0; i < MAX_ROOMS; i++)
            {
                detail::iterate_vector(roomList[i].SavedEntities, func, std::forward<Args>(args)...);
            }
        }

        template <typename Func, typename... Args>
        static void ForEachInBackwardsStages(Func&& func, Args&&... args)
        {
            Game* game = g_Game;
            BackwardsStageDesc* backwardsStages = game->_backwardsStages;
        
            for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
            {
                BackwardsStageDesc& backwardsStage = backwardsStages[i];
                ESSM::EntityIterators::Structure::ForEachBackwardsStage(backwardsStage, func, std::forward<Args>(args)...);
            }
        }

        template <typename Func, typename... Args>
        static void ForEachInPlayers(Func&& func, Args&&... args)
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

                detail::iterate_vector(player->_movingBoxContents, func, std::forward<Args>(args)...);

                Entity_Player* backupPlayer = player->_backupPlayer;
                if (backupPlayer)
                {
                    detail::iterate_vector(backupPlayer->_movingBoxContents, func, std::forward<Args>(args)...);
                    detail::iterate_vector(backupPlayer->_unlistedRestoreState._movingBoxContents, func, std::forward<Args>(args)...);
                }

                detail::iterate_vector(player->_unlistedRestoreState._movingBoxContents, func, std::forward<Args>(args)...);
            };
        
            for (Entity_Player* player : players)
            {
                handlePlayer(player);
            }
        
            const size_t NUM_ESAU_JR_STATES = std::extent_v<decltype(playerManager._esauJrState)>;
            for (size_t i = 0; i < NUM_ESAU_JR_STATES; i++)
            {
                handlePlayer(playerManager._esauJrState[i]);
            }
        }

        template <typename Func, typename... Args>
        static void ForEach(Func&& func, Args&&... args)
        {
            Game* game = g_Game;
            ForEachInRooms(std::forward<Func>(func), std::forward<Args>(args)...);
            detail::iterate_vector(game->_myosotisPickups, func, std::forward<Args>(args)...);
            ForEachInBackwardsStages(std::forward<Func>(func), std::forward<Args>(args)...);
            ForEachInPlayers(std::forward<Func>(func), std::forward<Args>(args)...);
        }
    }
    
    namespace InGameState
    {
        template <typename Func, typename... Args>
        static void ForEachInRooms(GameState& gameState, Func&& func, Args&&... args)
        {
            RoomDescriptor* rooms = gameState._rooms;
        
            for (size_t i = 0; i < MAX_ROOMS; i++)
            {
                RoomDescriptor& room = rooms[i];
                detail::iterate_vector(room.SavedEntities, func, std::forward<Args>(args)...);
            }
        }

        template <typename Func, typename... Args>
        static void ForEachInBackwardsStages(GameState& gameState, Func&& func, Args&&... args)
        {
            BackwardsStageDesc* backwardsStages = gameState._backwardsStages;
        
            for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
            {
                BackwardsStageDesc& backwardsStage = backwardsStages[i];
                ESSM::EntityIterators::Structure::ForEachBackwardsStage(backwardsStage, func, std::forward<Args>(args)...);
            }
        }

        template <typename Func, typename... Args>
        static void ForEachInPlayers(GameState& gameState, Func&& func, Args&&... args)
        {
            GameStatePlayer* players = gameState._players;
            size_t playerCount = gameState._playerCount;
        
            for (size_t i = 0; i < playerCount; i++)
            {
                GameStatePlayer& player = players[i];
                detail::iterate_vector(player._movingBoxContents, func, std::forward<Args>(args)...);
            }
        
            const size_t NUM_ESAU_JR_STATES = std::extent_v<decltype(gameState._esauJrStates)>;
            GameStatePlayer* esauJrStates = gameState._esauJrStates;
        
            for (size_t i = 0; i < NUM_ESAU_JR_STATES; i++)
            {
                GameStatePlayer& player = esauJrStates[i];
                if (player._playerType == -1)
                {
                    continue;
                }

                detail::iterate_vector(player._movingBoxContents, func, std::forward<Args>(args)...);
            }
        }

        template <typename Func, typename... Args>
        static void ForEach(GameState& gameState, Func&& func, Args&&... args)
        {
            ForEachInRooms(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
            ForEachInBackwardsStages(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
            ForEachInPlayers(gameState, std::forward<Func>(func), std::forward<Args>(args)...);
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
            LogDebug(LOG_DEBUG_HEADER "New ID: %u\n", id);
            return id;
        }
    
        uint32_t id = s_systemData.totalIds;
        s_systemData.totalIds++;
        s_systemData.hijackedStates.emplace_back();
        LogDebug(LOG_DEBUG_HEADER "New ID: %u\n", id);
        return id;
    }

    static void ClearId(uint32_t id)
    {
        s_systemData.reusableIds.push_back(id);
        LogDebug(LOG_DEBUG_HEADER "Cleared ID: %u\n", id);
    }
}

// Hijack Manager
namespace ESSM
{
    template <typename Traits>
    class HijackManager
    {
        using Data = typename Traits::DataType;
        using Marker = typename Traits::MarkerType;
        using Id = typename Traits::IdType;

        static_assert(std::is_integral_v<Marker> && sizeof(Marker) >= 2, "Chosen MarkerType cannot be used.");

    private:
        static const Marker CLEARED_MARKER = 0x5249;
        static const Marker READ_MARKER = 0x5248;
        static const Marker HIJACK_MARKER = 0x5247;
        static const Marker WRITTEN_MARKER = 0x5246;

    public:
        static void DefaultRestore(Data& data)
        {
            Traits::DefaultRestore(data);
        }

        static bool IsHijacked(const Data& data)
        {
            return Traits::GetMarker(data) == HIJACK_MARKER;
        }

        static bool IsWritten(const Data& data)
        {
            return Traits::GetMarker(data) == WRITTEN_MARKER;
        }
        
        static bool IsRead(const Data& data)
        {
            return Traits::GetMarker(data) == READ_MARKER;
        }

        static bool IsCleared(const Data& data)
        {
            return Traits::GetMarker(data) == CLEARED_MARKER;
        }

        static bool HasMarker(const Data& data)
        {
            return IsHijacked(data) || IsWritten(data) || IsRead(data) || IsCleared(data);
        }

        static void SetHijacked(Data& data)
        {
            Traits::SetMarker(data, HIJACK_MARKER);
        }

        static void SetWritten(Data& data)
        {
            Traits::SetMarker(data, WRITTEN_MARKER);
        }

        static void SetRead(Data& data)
        {
            Traits::SetMarker(data, READ_MARKER);
        }

        static void SetCleared(Data& data)
        {
            Traits::SetMarker(data, CLEARED_MARKER);
        }

        static size_t GetId(const Data& data)
        {
            return Traits::GetId(data);
        }

        static void SetId(Data& data, size_t id)
        {
            Traits::SetId(data, id);
        }

        static void Hijack(Data& data, size_t id)
        {
            HijackedState& hijackedState = s_systemData.hijackedStates[id];
            hijackedState.marker = Traits::GetMarker(data);
            hijackedState.id = Traits::GetId(data);
            SetHijacked(data);
            SetId(data, id);
        }

        static size_t NewHijack(Data& data)
        {
            assert(!IsHijacked(data));
            size_t id = ESSM::IdManager::NewId();
            Hijack(data, id);
            return id;
        }

        static void RestoreHijack(Data& data, HijackedState& hijackedState)
        {
            Traits::SetMarker(data, hijackedState.marker);
            Traits::SetId(data, hijackedState.id);
        }

        static size_t UnHijack(Data& data)
        {
            assert(IsHijacked(data));
            size_t id = GetId(data);
            RestoreHijack(data, s_systemData.hijackedStates[id]);
            return id;
        }
    };

    struct EntityTraits {
        using DataType = EntitySaveState;
        using MarkerType = short;
        using IdType = uint32_t;

        static short GetMarker(const EntitySaveState& data) { return data.gridSpawnIdx; }
        static void SetMarker(EntitySaveState& data, short value) { data.gridSpawnIdx = value; }
        static int GetId(const EntitySaveState& data) { return data._intStorage7; }
        static void SetId(EntitySaveState& data, int value) { data._intStorage7 = value; }

        static void DefaultRestore(EntitySaveState& data)
        {
            data.gridSpawnIdx = -1;
            data._intStorage7 = 0;
        }
    };

    struct PlayerTraits {
        using DataType = GameStatePlayer;
        using MarkerType = int;
        using IdType = int;

        static int GetMarker(const GameStatePlayer& data) { return data._immaculateConceptionState; }
        static void SetMarker(GameStatePlayer& data, int value) { data._immaculateConceptionState = value; }
        static int GetId(const GameStatePlayer& data) { return data._cambionConceptionState; }
        static void SetId(GameStatePlayer& data, int value) { data._cambionConceptionState = value; }

        static void DefaultRestore(GameStatePlayer& data)
        {
            data._immaculateConceptionState = 0;
            data._cambionConceptionState = 0;
        }
    };

    struct FamiliarTraits {
        using DataType = FamiliarData;
        using MarkerType = int;
        using IdType = int;

        static int GetMarker(const FamiliarData& data) { return data._state; }
        static void SetMarker(FamiliarData& data, int value) { data._state = value; }
        static int GetId(const FamiliarData& data) { return data._roomClearCount; }
        static void SetId(FamiliarData& data, int value) { data._roomClearCount = value; }

        static void DefaultRestore(FamiliarData& data)
        {
            data._state = 0;
            data._roomClearCount = 0;
        }
    };

    using EntityHijackManager = HijackManager<EntityTraits>;
    using PlayerHijackManager = HijackManager<PlayerTraits>;
    using FamiliarHijackManager = HijackManager<FamiliarTraits>;

    template <typename T>
    struct HijackManagerFor;

    template <>
    struct HijackManagerFor<EntitySaveState> {
        using type = EntityHijackManager;
    };

    template <>
    struct HijackManagerFor<GameStatePlayer> {
        using type = PlayerHijackManager;
    };

    template <>
    struct HijackManagerFor<FamiliarData> {
        using type = FamiliarHijackManager;
    };
}

namespace ESSM::Traits
{
    template<typename T>
    struct TraitsFor;

    struct EntityTraits
    {
        using Hijack = EntityHijackManager;
        static constexpr bool can_be_cleared = false;
        static constexpr bool has_recursive_child = true;

        static EntitySaveState* GetRecursiveChild(EntitySaveState& s)
        {
            return s.entitySaveState.saveState;
        }
    };

    struct PlayerTraits
    {
        using Hijack = PlayerHijackManager;
        static constexpr bool can_be_cleared = true;
        static constexpr bool has_recursive_child = false;
    };

    struct FamiliarTraits
    {
        using Hijack = FamiliarHijackManager;
        static constexpr bool can_be_cleared = false;
        static constexpr bool has_recursive_child = false;
    };

    template<>
    struct TraitsFor<EntitySaveState> : EntityTraits {};
    template<>
    struct TraitsFor<GameStatePlayer> : PlayerTraits {};
    template<>
    struct TraitsFor<FamiliarData> : FamiliarTraits {};
}

// Unhijacked API
namespace ESSM
{
    short& EntitySaveState_GetGridSpawnIdx(EntitySaveState& data)
    {
        assert(EntityHijackManager::IsHijacked(data));
        uint32_t id = EntityHijackManager::GetId(data);
        return *(short*)&s_systemData.hijackedStates[id].marker;
    }

    uint32_t& EntitySaveState_GetI7(EntitySaveState& data)
    {
        assert(EntityHijackManager::IsHijacked(data));
        uint32_t id = EntityHijackManager::GetId(data);
        return *(uint32_t*)&s_systemData.hijackedStates[id].id;
    }

    uint32_t& GameStatePlayer_GetImmaculateConceptionState(GameStatePlayer& data)
    {
        assert(PlayerHijackManager::IsHijacked(data));
        uint32_t id = PlayerHijackManager::GetId(data);
        return *(uint32_t*)&s_systemData.hijackedStates[id].marker;
    }

    uint32_t& GameStatePlayer_GetCambionConceptionState(GameStatePlayer& data)
    {
        assert(PlayerHijackManager::IsHijacked(data));
        uint32_t id = PlayerHijackManager::GetId(data);
        return *(uint32_t*)&s_systemData.hijackedStates[id].id;
    }

    uint32_t& FamiliarData_GetState(FamiliarData& data)
    {
        assert(FamiliarHijackManager::IsHijacked(data));
        uint32_t id = FamiliarHijackManager::GetId(data);
        return *(uint32_t*)&s_systemData.hijackedStates[id].marker;
    }

    uint32_t& FamiliarData_GetRoomClearCount(FamiliarData& data)
    {
        assert(FamiliarHijackManager::IsHijacked(data));
        uint32_t id = FamiliarHijackManager::GetId(data);
        return *(uint32_t*)&s_systemData.hijackedStates[id].id;
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
            ZHL::Log(LOG_ERROR_HEADER "ESSM::BindLuaCallbacks: expected a table\n");
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
                ZHL::Log(LOG_ERROR_HEADER "ESSM::BindLuaCallbacks: Expected '%s' to be a function\n", fieldName);
                lua_pop(L, 1);
                LuaInternals::RaiseInitError();
                assert(false);
                return;
            }
    
            outRef = luaL_ref(L, LUA_REGISTRYINDEX);
        };

        BindCallback(L, tblIdx, "StoreEntity", this->m_storeEntity);
        BindCallback(L, tblIdx, "RestoreEntity", this->m_restoreEntity);
        BindCallback(L, tblIdx, "ClearStates", this->m_clearSaveStates);
        BindCallback(L, tblIdx, "CopyStates", this->m_copySaveStates);
        BindCallback(L, tblIdx, "Serialize", this->m_serialize);
        BindCallback(L, tblIdx, "PreDeserialize", this->m_preDeserialize);
        BindCallback(L, tblIdx, "Deserialize", this->m_deserialize);
    }
    
    void LuaCallbacks::StoreEntity(const Entity& entity, uint32_t saveStateId)
    {
        LuaEngine* lua = g_LuaEngine;
        lua_State* L = lua->_state;
    
        uint32_t entityId = lua->GetMaskedPointer((uintptr_t)&entity);
    
        lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_storeEntity);
        lua_pushinteger(L, entityId);
        lua_pushinteger(L, saveStateId + 1); // Use 1-based indexing for Lua.
    
        if (lua_pcall(L, 2, 0, 0) != LUA_OK)
        {
            ZHL::Log(LOG_ERROR_HEADER "LuaCallbacks::StoreEntity: %s\n", lua_tostring(L, -1));
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
        lua_pushinteger(L, saveStateId + 1); // Use 1-based indexing for Lua.
    
        if (lua_pcall(L, 2, 0, 0) != LUA_OK)
        {
            ZHL::Log(LOG_ERROR_HEADER "LuaCallbacks::RestoreEntity: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    
    void LuaCallbacks::ClearStates(const std::vector<uint32_t>& saveStateIds)
    {
        if (saveStateIds.size() == 0)
        {
            return;
        }

        lua_State* L = g_LuaEngine->_state;
    
        lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_clearSaveStates);
    
        // push_vector
        size_t size = saveStateIds.size();
        lua_createtable(L, size, 0);
        for (size_t i = 0; i < size; i++)
        {
            lua_pushinteger(L, saveStateIds[i] + 1); // Use 1-based indexing for Lua.
            lua_rawseti(L, -2, i + 1);
        }
    
        if (lua_pcall(L, 1, 0, 0) != LUA_OK)
        {
            ZHL::Log(LOG_ERROR_HEADER "LuaCallbacks::ClearStates: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    
    void LuaCallbacks::CopyStates(const std::vector<std::pair<uint32_t, uint32_t>>& copyIds)
    {
        if (copyIds.size() == 0)
        {
            return;
        }

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
            lua_pushinteger(L, copyId.first + 1); // Use 1-based indexing for Lua.
            lua_rawseti(L, sourceIds, i + 1);

            lua_pushinteger(L, copyId.second + 1); // Use 1-based indexing for Lua.
            lua_rawseti(L, destIds, i + 1);
        }
    
        if (lua_pcall(L, 2, 0, 0) != LUA_OK)
        {
            ZHL::Log(LOG_ERROR_HEADER "LuaCallbacks::CopyStates: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    
    void LuaCallbacks::Serialize(const detail::SaveData::_WriteState& writeState, const std::string& filename, uint32_t checksum)
    {
        lua_State* L = g_LuaEngine->_state;

        const auto& entityStates = writeState.writeEntityIdPairs;
        
        lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_serialize);

        /* ASSUMPTION: All serialize ids are unique.
        *  This should, currently, always be true as all ids are based on the size of the vector before push.
        */

        // push IdMap
        size_t size = entityStates.size();
        lua_createtable(L, size, 0);
        int idMap = lua_absindex(L, -1);

        for (size_t i = 0; i < entityStates.size(); ++i)
        {
            const auto& ids = entityStates[i];
            lua_pushinteger(L, ids.realId + 1); // Use 1-based indexing for Lua.
            lua_rawseti(L, idMap, ids.serializeId + 1);
        }

        lua_pushlstring(L, filename.c_str(), filename.size());
        lua_pushinteger(L, checksum);

        if (lua_pcall(L, 3, 0, 0) != LUA_OK)
        {
            ZHL::Log(LOG_ERROR_HEADER "LuaCallbacks::Serialize: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    
    void LuaCallbacks::PreDeserialize(const std::string& filename)
    {
        lua_State* L = g_LuaEngine->_state;

        lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_preDeserialize);

        std::list<ModReference>& modReferences = g_Mods;

        size_t size = modReferences.size();
        lua_createtable(L, size, 0); // mods
        lua_createtable(L, size, 0); // modIds

        int mods = lua_absindex(L, -2);
        int modIds = lua_absindex(L, -1);

        size_t i = 0;
        for (ModReference& modRef : modReferences)
        {
            std::filesystem::path filePath = LuaFunctions::GetLuaSaveDataPath(modRef, filename);
            std::error_code errorCode;
            if (!std::filesystem::exists(filePath, errorCode))
            {
                continue;
            }

            lua_rawgeti(L, LUA_REGISTRYINDEX, modRef._luaTableRef->_ref);
            lua_rawseti(L, mods, i + 1);

            const std::string& id = modRef._mainLuaPath;
            lua_pushlstring(L, id.c_str(), id.size());
            lua_rawseti(L, modIds, i + 1);

            ++i;
        }
        
        if (lua_pcall(L, 2, 0, 0) != LUA_OK)
        {
            ZHL::Log(LOG_ERROR_HEADER "LuaCallbacks::PreDeserialize: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }

    void LuaCallbacks::Deserialize(const detail::SaveData::_ReadState& readState, const std::string& filename, uint32_t checksum)
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
            const auto& ids = entityStates[i];
            lua_pushinteger(L, ids.serializeId + 1); // Use 1-based indexing for Lua.
            lua_rawseti(L, serializedIds, i + 1);

            lua_pushinteger(L, ids.realId + 1); // Use 1-based indexing for Lua.
            lua_rawseti(L, destIds, i + 1);
        }

        lua_pushlstring(L, filename.c_str(), filename.size());
        lua_pushinteger(L, checksum);
            
        if (lua_pcall(L, 4, 0, 0) != LUA_OK)
        {
            ZHL::Log(LOG_ERROR_HEADER "LuaCallbacks::Deserialize: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
}

namespace ESSM::Utils
{
    static auto CollectLambda = [](auto& state, CollectedStates& collection) { collection.emplace_back(&state); };

    // Collects all entities of a specific type
    template<typename T>
    static void CollectStates(std::vector<T>& states, CollectedStates& collection)
    {
        for (auto& state : states)
        {
            CollectLambda(state, collection);
        }
    }
}

namespace ESSM::Core
{
    static auto ClearState = [](const auto* obj, ClearedIds& clearedIds)
    {
        using T = std::remove_cv_t<std::remove_pointer_t<decltype(obj)>>;
        using Traits = ESSM::Traits::TraitsFor<T>;
        using Manager = typename Traits::Hijack;

        assert(Manager::IsHijacked(*obj));
        uint32_t id = Manager::GetId(*obj);
        IdManager::ClearId(id);

        clearedIds.emplace_back(id);
    };

    static auto ClearState_HijackedOnly = [](const auto* obj, ClearedIds& clearedIds)
    {
        using T = std::remove_cv_t<std::remove_pointer_t<decltype(obj)>>;
        using Traits = ESSM::Traits::TraitsFor<T>;
        using Manager = typename Traits::Hijack;

        if (Manager::IsHijacked(*obj))
        {
            ClearState(obj, clearedIds);
        }
    };

    static auto CopyState_NoBatch = [](auto* obj) -> std::pair<uint32_t, uint32_t>
    {
        using T = std::remove_cv_t<std::remove_pointer_t<decltype(obj)>>;
        using Traits = ESSM::Traits::TraitsFor<T>;
        using Manager = typename Traits::Hijack;

        assert(Manager::IsHijacked(*obj));
        uint32_t sourceId = Manager::GetId(*obj);
        uint32_t targetId = IdManager::NewId();
        Manager::SetId(*obj, targetId);
        s_systemData.hijackedStates[targetId] = s_systemData.hijackedStates[sourceId];
        LogDebug(LOG_DEBUG_HEADER "Copied Entity %d -> %d\n", sourceId, targetId);

        return std::make_pair(sourceId, targetId);
    };

    static auto CopyState = [](auto* obj, CopiedIds& copiedIds)
    {
        copiedIds.emplace_back(CopyState_NoBatch(obj));
    };

    static void ClearSaveStates(CollectedStates& collectedSaves)
    {
        ClearedIds clearedIds;
        clearedIds.reserve(collectedSaves.size());
        for (auto& entity : collectedSaves)
        {
            std::visit([&](auto* obj) { ClearState(obj, clearedIds); }, entity);
        }
        s_luaCallbacks.ClearStates(clearedIds);
    }

    static void ClearSaveStates_HijackedOnly(CollectedStates& collectedSaves)
    {
        ClearedIds clearedIds;
        clearedIds.reserve(collectedSaves.size());
        for (auto& entity : collectedSaves)
        {
            std::visit([&](auto* obj) { ClearState_HijackedOnly(obj, clearedIds); }, entity);
        }
        s_luaCallbacks.ClearStates(clearedIds);
    }

    static void CopySaveStates(CollectedStates& collectedSaves)
    {
        CopiedIds copiedIds;
        copiedIds.reserve(collectedSaves.size());
        for (auto& entity : collectedSaves)
        {
            std::visit([&](auto* obj) { CopyState(obj, copiedIds); }, entity);
        }
        s_luaCallbacks.CopyStates(copiedIds);
    }

    static void SaveEntity(Entity& entity, uint32_t id)
    {
        s_luaCallbacks.StoreEntity(entity, id);
    }

    static void RestoreEntity(Entity& entity, uint32_t id)
    {
        s_luaCallbacks.RestoreEntity(entity, id);
    }
}

// Public API
namespace ESSM
{
    void EntitySaveState_ClearBatch(const std::vector<EntitySaveState>& vector)
    {
        ClearedIds clearedIds;
        clearedIds.reserve(vector.size());
        for (auto& entity : vector)
        {
            Core::ClearState(&entity, clearedIds);
        }
        s_luaCallbacks.ClearStates(clearedIds);
    }

    void RegisterSaveState(EntitySaveState& saveState)
    {
        assert(!EntityHijackManager::HasMarker(saveState));
        EntityHijackManager::NewHijack(saveState);
    }
}

// Stability Checker
namespace ESSM
{
    class StabilityChecker
    {
    private:
        enum eErrors
        {
            ERROR_DOUBLE_CLEAR = 0,
            ERROR_INVALID_STATE = 1,
            ERROR_INVALID_ID = 2,
            ERROR_DUPLICATE = 3,
            ERROR_INCORRECT_CLEAR = 4,
            ERROR_LEAK = 5,

            NUM_ERRORS
        };

        struct CheckData
        {
        private:
            std::vector<uintptr_t> m_checks;
            std::vector<StatePtr> m_invalidStates;
            std::vector<StatePtr> m_invalidIds;
            std::vector<StatePtr> m_duplicates;
            std::vector<StatePtr> m_incorrectClears;
            std::vector<uint32_t> m_leakedIds;
            std::bitset<NUM_ERRORS> m_errors;

            friend StabilityChecker;
        };

        template <typename T>
        struct CheckTraits;

        template <>
        struct CheckTraits<EntitySaveState> : Traits::EntityTraits
        {
            static bool is_valid_state(EntitySaveState& s)
            {
                return Hijack::IsHijacked(s);
            }
        };

        template <>
        struct CheckTraits<GameStatePlayer> : Traits::PlayerTraits
        {
            static bool is_valid_state(GameStatePlayer& s)
            {
                return Hijack::IsCleared(s) || Hijack::IsHijacked(s);
            }
        };

        template <>
        struct CheckTraits<FamiliarData> : Traits::FamiliarTraits
        {
            static bool is_valid_state(FamiliarData& s)
            {
                return Hijack::IsHijacked(s);
            }
        };

        template <typename T>
        static void check_save_state(CheckData& checks, T& saveState)
        {
            using Traits  = CheckTraits<T>;
            using Hijack = typename Traits::Hijack;

            bool validState = Traits::is_valid_state(saveState);
            assert(validState);
            if (!validState)
            {
                checks.m_invalidStates.emplace_back(&saveState);
                checks.m_errors.set(eErrors::ERROR_INVALID_STATE, true);
                return;
            }

            // not every entity makes use of the cleared marker
            if constexpr (Traits::can_be_cleared)
            {
                if (Hijack::IsCleared(saveState))
                    return;
            }

            uint32_t id = Hijack::GetId(saveState);
            bool validId = id < s_systemData.totalIds;
            assert(validId); // this is really messed up
            if (!validId)
            {
                checks.m_invalidIds.emplace_back(&saveState);
                checks.m_errors.set(eErrors::ERROR_INVALID_ID, true);
                return;
            }

            uintptr_t checkPtr = checks.m_checks[id];
            uintptr_t savePtr = reinterpret_cast<uintptr_t>(&saveState);
            if (checkPtr == savePtr) // because of smartPointers we might visit the same state twice, so this is not an error, and even so it would either be an error with the iterator or with the game.
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
                    checks.m_errors.set(eErrors::ERROR_INCORRECT_CLEAR, true);
                }
                else
                {
                    checks.m_duplicates.emplace_back(&saveState);
                    checks.m_errors.set(eErrors::ERROR_DUPLICATE, true);
                }
                return;
            }

            checks.m_checks[id] = savePtr;
            // not every entity has a recursive child
            if constexpr (Traits::has_recursive_child)
            {
                if (auto* recursiveChild = Traits::GetRecursiveChild(saveState))
                {
                    check_save_state(checks, *recursiveChild);
                }
            }
        }

        static void collect_from_removed_players(CollectedStates& collection)
        {
            const EntityList_EL& persistentEL = *g_Game->_room->_entityList.GetPersistentEL();
            for (size_t i = 0; i < persistentEL._size; ++i)
            {
                Entity* entity = persistentEL._data[i];
                if (!entity || entity->_type != ENTITY_PLAYER)
                {
                    continue;
                }

                // the game always makes this assumption, so if this entity is not an actual Entity_Player there would be problems regardless.
                Entity_Player* player = (Entity_Player*)entity;
                if (!(!player->_exists && player->_valid && player->_playerIndex < 0))
                {
                    continue;
                }

                ESSM::Utils::CollectStates(player->_movingBoxContents, collection);
                GameStatePlayer& saveState = player->_unlistedRestoreState;
                collection.push_back(&saveState);
                ESSM::Utils::CollectStates(saveState._familiarData, collection);
                ESSM::Utils::CollectStates(saveState._movingBoxContents, collection);
            }
        }

        static void collect_from_entity_list(CollectedStates& collection)
        {
            const EntityFactory* factory = g_Game->_entityFactory;
            const size_t MAX_PICKUPS = std::extent_v<decltype(factory->_pickup)>;
            const Entity_Pickup* pickups = factory->_pickup;

            for (size_t i = 0; i < MAX_PICKUPS; i++)
            {
                const Entity_Pickup& pickup = pickups[i];
                EntitySaveState* flipState = pickup._flipSaveState.saveState;
                if (flipState)
                {
                    Utils::CollectLambda(*flipState, collection);
                }
            }
        }

        static void collect_from_run(CollectedStates& collection)
        {
            ESSM::PlayerIterators::InGame::ForEach(Utils::CollectLambda, collection);
            ESSM::FamiliarIterators::InGame::ForEach(Utils::CollectLambda, collection);
            ESSM::EntityIterators::InGame::ForEach(Utils::CollectLambda, collection);
        }

        static void collect_from_game_state(GameState& gameState, CollectedStates& collection)
        {
            ESSM::PlayerIterators::InGameState::ForEach(gameState, Utils::CollectLambda, collection);
            ESSM::FamiliarIterators::InGameState::ForEach(gameState, Utils::CollectLambda, collection);
            ESSM::EntityIterators::InGameState::ForEach(gameState, Utils::CollectLambda, collection);
        }

        static void print_errors(const std::array<const char*, eErrors::NUM_ERRORS>& strings, size_t numStrings)
        {
            const char CONSOLE_ERROR_HEADER[] = LOG_ERROR_HEADER;
            const char HEADER[] = "Stability Checker:\n";
            const char INDENTATION[] = "  ";
            const char FOOTER[] = "end\n";
            const size_t CONSOLE_ERROR_HEADER_SIZE = sizeof(CONSOLE_ERROR_HEADER) - 1;
            const size_t HEADER_SIZE = sizeof(HEADER) - 1;
            const size_t INDENTATION_SIZE = sizeof(INDENTATION) - 1;
            const size_t FOOTER_SIZE = sizeof(FOOTER) - 1;

            size_t stringSize = CONSOLE_ERROR_HEADER_SIZE;
            stringSize += HEADER_SIZE;
            size_t stringSizes[eErrors::NUM_ERRORS] = {0};

            for (size_t i = 0; i < numStrings; i++)
            {
                stringSizes[i] = std::strlen(strings[i]);
                stringSize += INDENTATION_SIZE + stringSizes[i] + 1; // newline character
            }

            stringSize += FOOTER_SIZE;
 
            std::string consoleString;
            consoleString.reserve(stringSize);

            consoleString.assign(CONSOLE_ERROR_HEADER, CONSOLE_ERROR_HEADER_SIZE);
            consoleString.append(HEADER, HEADER_SIZE);

            for (size_t i = 0; i < numStrings; i++)
            {
                consoleString.append(INDENTATION, INDENTATION_SIZE);
                consoleString.append(strings[i], stringSizes[i]);
                // add newline
                consoleString.push_back('\n');
            }
            
            consoleString.append(FOOTER, FOOTER_SIZE);
            assert(consoleString.size() == stringSize); // sanity check

            if (!s_checkerErrorShown)
            {
                ErrorDisplay::RaiseError("A core REPENTOGON system is failing!", ErrorDisplay::Priority::REPENTOGON_MEDIUM);
                // may want to create an ImGui display for REPENTOGON internals.
                s_checkerErrorShown = true;
            }

            g_Game->GetConsole()->PrintError(consoleString.c_str());

            // Log in ZHL differently, otherwise timestamps won't appear on other lines
            ZHL::Log(LOG_ERROR_HEADER "%s", HEADER);
            for (size_t i = 0; i < numStrings; i++)
            {
                ZHL::Log(LOG_ERROR_HEADER "  %s\n", strings[i]);
            }
            ZHL::Log(LOG_ERROR_HEADER "%s", FOOTER);
        }

        static void handle_errors(CheckData& checks)
        {
            if (checks.m_errors.none())
            {
                return;
            }

            struct ErrorStrings
            {
                std::array<const char*, eErrors::NUM_ERRORS> strings = {};
                size_t numStrings = 0;

                void AddString(const char* string)
                {
                    assert(numStrings < eErrors::NUM_ERRORS);
                    strings[numStrings] = string;
                    numStrings++;
                }
            };

            ErrorStrings errorStrings;

            // TODO: print error message for console
            if (checks.m_errors.test(eErrors::ERROR_DOUBLE_CLEAR))
            {
                errorStrings.AddString("ERROR_DOUBLE_CLEAR: An id was cleared more than once.");
            }

            if (checks.m_errors.test(eErrors::ERROR_INVALID_STATE))
            {
                for (StatePtr state : checks.m_invalidStates)
                {
                    std::visit([](auto* s)
                    {
                        using T = std::remove_pointer_t<decltype(s)>;
                        using Manager = typename Traits::TraitsFor<T>::Hijack;

                        // If the marker is not in a valid state then the hijacked data is lost forever, otherwise the data is correct.
                        if (Manager::HasMarker(*s))
                        {
                            Manager::DefaultRestore(*s);
                        }
                        Manager::NewHijack(*s);
                    }, state);
                }

                errorStrings.AddString("ERROR_INVALID_STATE: A state has not been properly handled.");
            }

            if (checks.m_errors.test(eErrors::ERROR_INVALID_ID))
            {
                // Original Data is lost forever, there is no way to get it back
                for (StatePtr state : checks.m_invalidIds)
                {
                    std::visit([](auto* s)
                    {
                        using T = std::remove_pointer_t<decltype(s)>;
                        using Manager = typename Traits::TraitsFor<T>::Hijack;

                        Manager::DefaultRestore(*s);
                        Manager::NewHijack(*s);
                    }, state);
                }

                errorStrings.AddString("ERROR_INVALID_ID: A non existent id has been used.");
            }

            if (checks.m_errors.test(eErrors::ERROR_INCORRECT_CLEAR))
            {
                // since the id is not reused we just need to copy
                // should not send a copy signal since a clear was issued on that id.
                for (StatePtr state : checks.m_incorrectClears)
                {
                    std::visit([](auto* s)
                    {
                        using T = std::remove_pointer_t<decltype(s)>;
                        using Manager = typename Traits::TraitsFor<T>::Hijack;

                        Core::CopyState_NoBatch(s);
                    }, state);
                }

                errorStrings.AddString("ERROR_INCORRECT_CLEAR: An id was cleared incorrectly.");
            }

            if (checks.m_errors.test(eErrors::ERROR_DUPLICATE))
            {
                CopiedIds copiedIds;
                copiedIds.reserve(checks.m_duplicates.size());
                Core::CopySaveStates(checks.m_duplicates);
                errorStrings.AddString("ERROR_DUPLICATE: Two or more states have the same id.");
            }

            if (checks.m_errors.test(eErrors::ERROR_LEAK))
            {
                for (uint32_t id : checks.m_leakedIds)
                {
                    IdManager::ClearId(id);
                }

                // send callback just to be sure that lua states are also cleared
                s_luaCallbacks.ClearStates(checks.m_leakedIds);
                errorStrings.AddString("ERROR_LEAK: An active id was not found.");
            }

            if (errorStrings.numStrings == 0)
            {
                errorStrings.AddString("ERROR_UNKNOWN: An error occurred but no handler was found.");
                assert(false);
            }

            print_errors(errorStrings.strings, errorStrings.numStrings);
        }

    public:
        static void CheckStability()
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

            CollectedStates collection;
            collect_from_removed_players(collection);
            collect_from_entity_list(collection);
            collect_from_run(collection);
            collect_from_game_state(manager->_gamestate, collection);
            collect_from_game_state(game->_glowingHourglassStates[0]._gameState, collection);
            collect_from_game_state(game->_glowingHourglassStates[1]._gameState, collection);

            for (auto& statePtr : collection)
            {
                std::visit([&](auto* s) { check_save_state(checks, *s); }, statePtr);
            }

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

namespace ESSM::detail::SaveData
{
    const uint32_t SAVE_VERSION = 1;

    struct SaveFile
    {
        std::vector<HijackedState> entities;
        std::vector<HijackedState> players;
        std::vector<HijackedState> familiars;
    };

    namespace Section
    {
        const uint32_t ENTITY_START = 0x73454E54; // sENT
        const uint32_t ENTITY_END = 0x65454E54; // eENT
        const uint32_t ENTITY_SIZE = 4 + 4;
        const uint32_t MAX_ENTITY_COUNT = 0x1FFFF;
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
        template <typename T, bool RecursiveChild>
        struct _WriteTraits;

        template <>
        struct _WriteTraits<EntitySaveState, false> : Traits::EntityTraits
        {
            static bool is_valid_state(EntitySaveState& s)
            {
                return Hijack::IsHijacked(s);
            }
        };

        template <>
        struct _WriteTraits<EntitySaveState, true> : Traits::EntityTraits
        {
            static bool is_valid_state(EntitySaveState& s)
            {
                return Hijack::IsHijacked(s) || Hijack::IsWritten(s);
            }
        };

        template <>
        struct _WriteTraits<GameStatePlayer, false> : Traits::PlayerTraits
        {
            static bool is_valid_state(GameStatePlayer& s)
            {
                return Hijack::IsHijacked(s);
            }
        };

        template <>
        struct _WriteTraits<FamiliarData, false> : Traits::FamiliarTraits
        {
            static bool is_valid_state(FamiliarData& s)
            {
                return Hijack::IsHijacked(s);
            }
        };

        template <typename T, bool RecursiveChild>
        void write_save_state(T& data, _WriteState& writeState)
        {
            using Traits  = _WriteTraits<T, RecursiveChild>;
            using Hijack = Traits::Hijack;

            bool isValid = Traits::is_valid_state(data);
            assert(isValid);
            if (!isValid)
            {
                throw std::runtime_error("invalid save state during write");
            }

            if (!Hijack::IsHijacked(data))
            {
                return;
            }

            auto& writtenSaveStates = writeState.writtenEntitySaveStates;
            uint32_t writeId = writtenSaveStates.size();

            uint32_t id = Hijack::GetId(data);
            Hijack::SetWritten(data);
            Hijack::SetId(data, writeId);

            writtenSaveStates.emplace_back(&data, id);

            if constexpr (Traits::has_recursive_child)
            {
                if (auto* inner = Traits::GetRecursiveChild(data))
                {
                    write_save_state<T, true>(*inner, writeState);
                }
            }
        }

        template <typename T>
        void read_save_state(T& data, _ReadState& readState)
        {
            using Traits  = ESSM::Traits::TraitsFor<T>;
            using Hijack = typename Traits::Hijack;

            if (!Hijack::IsWritten(data))
            {
                if (!Hijack::IsRead(data))
                {
                    readState.errors.set(ReadErrors::ERROR_INVALID_STATE);
                }
            }
            else
            {
                // Due to how flip state are implemented, there can be duplicate ids. As such finding a duplicate here is not an error.
                // this shouldn't happen for other types but it's not harmful and if anything it would be a mistake on the game's part.
                uint32_t readId = Hijack::GetId(data);
                readState.minReadEntities = std::max(readState.minReadEntities, readId + 1);
                readState.readEntitySaveStates.push_back(&data);
            }

            Hijack::SetRead(data);

            if constexpr (Traits::has_recursive_child)
            {
                if (auto* inner = Traits::GetRecursiveChild(data))
                {
                    read_save_state<T>(*inner, readState);
                }
            }
        }
    }

    static void resolve_read_errors(_ReadState& readState)
    {
        if (readState.errors.test(ReadErrors::ERROR_INVALID_STATE))
        {
            ZHL::Log(LOG_ERROR_HEADER "invalid save state during read\n");
        }

        if (readState.errors.test(ReadErrors::ERROR_DUPLICATE_ID))
        {
            ZHL::Log(LOG_ERROR_HEADER "duplicate id found during read\n");
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

    static void remove_file(const std::filesystem::path& filePath)
    {
        std::error_code errorCode;
        std::filesystem::remove(filePath, errorCode);
        if (errorCode)
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to delete file at \"%s\"\n", filePath.string().c_str());
        }
    }

    static CollectedStates collect_save_entities(GameState& gameState)
    {
        CollectedStates collection;
        ESSM::EntityIterators::InGameState::ForEach(gameState, Utils::CollectLambda, collection);
        ESSM::PlayerIterators::InGameState::ForEach(gameState, Utils::CollectLambda, collection);
        ESSM::FamiliarIterators::InGameState::ForEach(gameState, Utils::CollectLambda, collection);

        return collection;
    }

    WriteState WriteGameState()
    {
        Manager* manager = g_Manager;
        GameState& gameState = manager->_gamestate;
    
        WriteState writeState = WriteState();
        _WriteState& _writeState = writeState.m_writeState;
        CollectedStates collection = collect_save_entities(gameState);

        try
        {
            for (StatePtr statePtr : collection)
            {
                std::visit([&](auto* s) {
                    using T = std::remove_cv_t<std::remove_reference_t<decltype(*s)>>;
                    Operations::write_save_state<T, false>(*s, _writeState);
                }, statePtr);
            }
        }
        catch(const std::runtime_error& e)
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to write game state : %s\n", e.what());
            clear_write_state(_writeState);
            return writeState;
        }

        // Entities will be serialized later using the WriteState.
        return writeState;
    }

    ReadState ReadGameState()
    {
        Manager* manager = g_Manager;
        GameState& gameState = manager->_gamestate;

        ReadState readState = ReadState();
        _ReadState& _readState = readState.m_readState;
        CollectedStates collection = collect_save_entities(gameState);

        try
        {
            for (auto& statePtr : collection)
            {
                std::visit([&](auto* s) { Operations::read_save_state(*s, _readState); }, statePtr);
            }
        }
        catch(const std::runtime_error& e)
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to read game state : %s\n", e.what());
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
            StatePtr& saveState = writtenSaveState.state;
            uint32_t id = writtenSaveState.originalId;
    
            uint32_t writeId;
            std::visit([&](auto* ptr)
            {
                using T = std::remove_cv_t<std::remove_pointer_t<decltype(ptr)>>;
                using Traits  = ESSM::Traits::TraitsFor<T>;
                using Hijack = typename Traits::Hijack;

                assert(Hijack::IsWritten(*ptr));
                Hijack::SetHijacked(*ptr);
                writeId = Hijack::GetId(*ptr);
                Hijack::SetId(*ptr, id);
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
                using T = std::remove_cv_t<std::remove_pointer_t<decltype(ptr)>>;
                using Traits  = ESSM::Traits::TraitsFor<T>;
                using Hijack = typename Traits::Hijack;

                assert(Hijack::IsRead(*ptr));
                readId = Hijack::GetId(*ptr);
                // all previous steps should have guaranteed this
                assert((int)readId <= maxId);

                Hijack::SetHijacked(*ptr);
                Hijack::SetId(*ptr, id);
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
            uint32_t id = saveState.originalId;
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
        bool exists = REPENTOGON::EnsureParentPathExists(filePath);
        if (!exists)
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to create directory for save file at \"%s\"\n", filePath.string().c_str());
            return;
        }

        std::ofstream file = std::ofstream(filePath, std::ios::binary);
        if (!file.is_open())
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to open save file for writing at \"%s\"\n", filePath.string().c_str());
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
            ZHL::Log(LOG_ERROR_HEADER "unable to serialize save file at \"%s\": %s\n", filePath.string().c_str(), e.what());
            file.close();
            remove_file(filePath);
            return;
        }

        ZHL::Log(LOG_INFO_HEADER "successfully saved %s to \"%s\"\n", filename.c_str(), filePath.string().c_str());

        uint32_t gameChecksum = g_Manager->_gamestate._checksum;
        s_luaCallbacks.Serialize(writeState, filename, gameChecksum);
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
            ZHL::Log(LOG_ERROR_HEADER "pre-existing errors in read state when deserializing\n");
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
            ZHL::Log(LOG_ERROR_HEADER "unable to open save file for reading at \"%s\"\n", filePath.string().c_str());
            return false;
        }

        SaveFile saveFile;
        try
        {
            deserialize_save(file, readState, saveFile);
        }
        catch(const std::runtime_error& e)
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to deserialize save file at \"%s\": %s\n", filePath.string().c_str(), e.what());
            return false;
        }

        file.close();
        hijack_read_entity_states(readState, saveFile);
        ZHL::Log(LOG_INFO_HEADER "successfully loaded %s from \"%s\"\n", fileName.c_str(), filePath.string().c_str());
        
        uint32_t gameChecksum = g_Manager->_gamestate._checksum;
        s_luaCallbacks.PreDeserialize(fileName);
        s_luaCallbacks.Deserialize(readState, fileName, gameChecksum);
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
            ZHL::Log(LOG_ERROR_HEADER "unable to delete file at \"%s\"\n", filePath.string().c_str());
        }
        else
        {
            ZHL::Log(LOG_INFO_HEADER "deleted file at \"%s\"\n", filePath.string().c_str());
        }
    }
}

#pragma region Hooks

static std::pair<Entity*, EntitySaveState*> s_minecartEntity = {nullptr, nullptr};

// ASSUMPTION: It is assumed that every time a save_entity with 'isSavingMinecartEntity' set to true is successful, it is immediately followed by another call to save_entity,
// with the saved entity being the minecart that holds the previous saved entity.
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
            EntitySaveState* flipState = minecartEntity.second->entitySaveState.saveState;
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
        EntitySaveState* flipState = minecartEntity.second->entitySaveState.saveState;
        if (flipState)
        {
            CollectedStates collection = {flipState};
            ESSM::Core::CopySaveStates(collection);
        }

        uint32_t id = ESSM::EntityHijackManager::NewHijack(*minecartEntity.second);
        ESSM::Core::SaveEntity(*minecartEntity.first, id);
    }

    EntitySaveState* flipState = data->entitySaveState.saveState;
    if (flipState)
    {
        CollectedStates collection = {flipState};
        ESSM::Core::CopySaveStates(collection);
    }

    uint32_t id = ESSM::EntityHijackManager::NewHijack(*data);
    ESSM::Core::SaveEntity(*entity, id);
    return saved;
}

HOOK_METHOD(Room, restore_entity, (Entity* entity, EntitySaveState* data) -> void)
{
    uint32_t id = ESSM::EntityHijackManager::UnHijack(*data);

    super(entity, data);

    ESSM::EntityHijackManager::Hijack(*data, id);
    ESSM::Core::RestoreEntity(*entity, id);
}

HOOK_METHOD(Level, Init, (bool resetLilPortalRoom) -> void)
{
    auto& myosotisPickups = g_Game->_myosotisPickups;

    CollectedStates collection;
    collection.reserve(myosotisPickups.size());
    ESSM::Utils::CollectStates(myosotisPickups, collection);
    ESSM::Core::ClearSaveStates(collection);

    super(resetLilPortalRoom);
}

HOOK_METHOD(Level, reset_room_list, (bool resetLilPortalRoom) -> void)
{
    Game* game = g_Game;
    RoomDescriptor* roomList = game->_gridRooms;
    const size_t LIL_PORTAL_IDX = MAX_GRID_ROOMS + (-eGridRooms::ROOM_LIL_PORTAL_IDX) - 1;

    CollectedStates collection;
    collection.reserve(DEFAULT_COLLECT_RESERVE);

    for (size_t i = 0; i < MAX_ROOMS; i++)
    {
        if (i == LIL_PORTAL_IDX && !(resetLilPortalRoom))
        {
            continue;
        }

        RoomDescriptor& room = roomList[i];
        ESSM::Utils::CollectStates(room.SavedEntities, collection);
    }

    ESSM::Core::ClearSaveStates(collection);
    super(resetLilPortalRoom);
}

static void reset_single_room(RoomDescriptor& room)
{
    CollectedStates collection;
    collection.reserve(room.SavedEntities.size());
    ESSM::Utils::CollectStates(room.SavedEntities, collection);
    ESSM::Core::ClearSaveStates(collection);
}

HOOK_METHOD(Level, DEBUG_goto_room, (RoomConfig_Room * roomConfig) -> void)
{
    const size_t LIST_IDX = eGridRooms::MAX_GRID_ROOMS + (-eGridRooms::ROOM_DEBUG_IDX) - 1;

    Game* game = g_Game;
    RoomDescriptor& room = game->_gridRooms[LIST_IDX];
    reset_single_room(room);

    super(roomConfig);
}

HOOK_METHOD(Level, InitializeGenesisRoom, () -> void)
{
    const size_t LIST_IDX = eGridRooms::MAX_GRID_ROOMS + (-eGridRooms::ROOM_GENESIS_IDX) - 1;

    Game* game = g_Game;
    RoomDescriptor& room = game->_gridRooms[LIST_IDX];
    reset_single_room(room);

    super();
}

HOOK_METHOD(Level, TryInitializeBlueRoom, (int currentIdx, int destinationIdx, int direction) -> bool)
{
    const uint32_t FLAG_BLUE_REDIRECT = 1 << 18;
    const size_t LIST_IDX = eGridRooms::MAX_GRID_ROOMS + (-eGridRooms::ROOM_BLUE_ROOM_IDX) - 1;

    if ((direction != -1 && currentIdx >= 0) && (g_Game->GetRoomByIdx(destinationIdx, -1)->Flags & FLAG_BLUE_REDIRECT) != 0)
    {
        Game* game = g_Game;
        RoomDescriptor& room = game->_gridRooms[LIST_IDX];
        reset_single_room(room);
    }

    return super(currentIdx, destinationIdx, direction);
}

HOOK_METHOD(Game, SaveBackwardsStage, (int stage) -> void)
{
    BackwardsStageDesc& backwardsStage = g_Game->_backwardsStages[stage - 1];
    CollectedStates collection;
    collection.reserve(DEFAULT_COLLECT_RESERVE);
    ESSM::EntityIterators::Structure::ForEachBackwardsStage(backwardsStage, ESSM::Utils::CollectLambda, collection);
    ESSM::Core::ClearSaveStates(collection);

	super(stage);

    collection.clear();
    ESSM::EntityIterators::Structure::ForEachBackwardsStage(backwardsStage, ESSM::Utils::CollectLambda, collection);
    ESSM::Core::CopySaveStates(collection);
}

// Clear backwards stage save state, even though the game simply sets room count to 0, to avoid having to iterate "uninitialized" RoomDescriptors in the BackwardsStage. 
HOOK_METHOD(Game, ResetState, () -> void)
{
    CollectedStates collection;
    collection.reserve(DEFAULT_COLLECT_RESERVE);
    ESSM::EntityIterators::InGame::ForEachInBackwardsStages(ESSM::Utils::CollectLambda, collection);
    ESSM::Core::ClearSaveStates(collection);

	super();
}

HOOK_METHOD(GameState, Clear, () -> void)
{
    LogDebug(LOG_DEBUG_HEADER "Start GameState::Clear\n");

    CollectedStates collection;
    collection.reserve(DEFAULT_COLLECT_RESERVE);
    ESSM::EntityIterators::InGameState::ForEachInRooms(*this, ESSM::Utils::CollectLambda, collection);
    ESSM::EntityIterators::InGameState::ForEachInBackwardsStages(*this, ESSM::Utils::CollectLambda, collection);
    ESSM::Core::ClearSaveStates_HijackedOnly(collection);

    // player save states are handled by GameStatePlayer::Init
    super();

    LogDebug(LOG_DEBUG_HEADER "End GameState::Clear\n");
}

HOOK_METHOD(Game, SaveState, (GameState* state) -> void)
{
    LogDebug(LOG_DEBUG_HEADER "Start Game::SaveState\n");

    // ASSUMPTION: It is assumed that the state has already been cleared.
    // This is because SaveState always calls GameState::Clear before saving
	super(state);
    
    CollectedStates collection;
    collection.reserve(DEFAULT_COLLECT_RESERVE);
    ESSM::EntityIterators::InGameState::ForEachInRooms(*state, ESSM::Utils::CollectLambda, collection);
    ESSM::EntityIterators::InGameState::ForEachInBackwardsStages(*state, ESSM::Utils::CollectLambda, collection);
    ESSM::Core::CopySaveStates(collection);

    // players are handled by Entity_Player::Init

    LogDebug(LOG_DEBUG_HEADER "End Game::SaveState\n");
}

HOOK_METHOD(Game, RestoreState, (GameState* state, bool startGame) -> void)
{
    LogDebug(LOG_DEBUG_HEADER "Start Game::RestoreState\n");

    CollectedStates collection;
    collection.reserve(DEFAULT_COLLECT_RESERVE);
    ESSM::EntityIterators::InGame::ForEachInBackwardsStages(ESSM::Utils::CollectLambda, collection);
    ESSM::Core::ClearSaveStates(collection);

    super(state, startGame);
    // copy is in a separate patch as copying it here might cause problems due to callbacks running in the mean time.

    LogDebug(LOG_DEBUG_HEADER "End Game::RestoreState\n");
}

HOOK_METHOD(Level, RestoreGameState, (GameState* state) -> void)
{
    // ASSUMPTION: Confirm that the function does not reset the LilPortalRoom when calling reset_room_list (through params), since we currently have to perform that clear manually.
    const size_t LIL_PORTAL_IDX = (-eGridRooms::ROOM_LIL_PORTAL_IDX) - 1;
    reset_single_room(g_Game->_negativeGridRooms[LIL_PORTAL_IDX]);
    super(state);
}

HOOK_METHOD(GameStatePlayer, Init, () -> void)
{
    CollectedStates collection;
    collection.reserve(this->_movingBoxContents.size() + this->_familiarData.size() + 1);
    ESSM::Utils::CollectStates(this->_movingBoxContents, collection);
    ESSM::Utils::CollectStates(this->_familiarData, collection);
    collection.emplace_back(this);
    // We can't guarantee that the GameStatePlayer is in a hijacked state before Init.
    ESSM::Core::ClearSaveStates_HijackedOnly(collection);

    super();
    // Doing this here since it makes the most sense, also means we won't have problems if a GameStatePlayer is accessed in between an Init and a Restore
    ESSM::PlayerHijackManager::SetCleared(*this);
}

HOOK_METHOD(Entity_Player, constructor, () -> void)
{
    super();
    ESSM::PlayerHijackManager::DefaultRestore(this->_unlistedRestoreState);
}

HOOK_METHOD(Entity_Player, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void)
{
    super(type, variant, subtype, initSeed);

    assert(!ESSM::PlayerHijackManager::HasMarker(this->_unlistedRestoreState));
    ESSM::PlayerHijackManager::SetCleared(this->_unlistedRestoreState);
}

HOOK_METHOD(Entity_Player, destructor, () -> void)
{
    GameStatePlayer& playerState = this->_unlistedRestoreState;

    CollectedStates collection;
    collection.reserve(this->_movingBoxContents.size() + playerState._movingBoxContents.size() + playerState._familiarData.size() + 1);
    ESSM::Utils::CollectStates(this->_movingBoxContents, collection);
    ESSM::Utils::CollectStates(playerState._movingBoxContents, collection);
    ESSM::Utils::CollectStates(playerState._familiarData, collection);

    if (!ESSM::PlayerHijackManager::IsCleared(playerState))
    {
        collection.emplace_back(&playerState);
    }

    ESSM::Core::ClearSaveStates(collection);

    super();
}

HOOK_METHOD(Entity_Player, StoreGameState, (GameStatePlayer* saveState, bool saveTemporaryFamiliars) -> void)
{
    // ASSUMPTION: It is assumed that the initial state is equivalent to an initialized or newly constructed state.
    assert(ESSM::PlayerHijackManager::IsCleared(*saveState));

    // We don't need to clear the marker or the state, as the store will override them anyway with the correct values
    super(saveState, saveTemporaryFamiliars);

    CollectedStates collection;
    collection.reserve(this->_movingBoxContents.size());
    ESSM::Utils::CollectStates(saveState->_movingBoxContents, collection);
    ESSM::Core::CopySaveStates(collection);

    // Familiar data is handled in separate hooks
    uint32_t id = ESSM::PlayerHijackManager::NewHijack(*saveState);
    ESSM::Core::SaveEntity(*this, id);
}

HOOK_METHOD(Entity_Player, RestoreGameState, (GameStatePlayer * saveState) -> void)
{
    // if this player was replaced then don't restore the state
    if (this->_replacedPlayer)
    {
        return super(saveState);
    }

    // Familiars are handled by Entity_Familiar::RestoreState (which is in RestoreGameState_PostLevelInit anyway)
    CollectedStates collection;
    collection.reserve(this->_movingBoxContents.size());
    ESSM::Utils::CollectStates(this->_movingBoxContents, collection);
    ESSM::Core::ClearSaveStates(collection);

    uint32_t id = ESSM::PlayerHijackManager::UnHijack(*saveState);
    super(saveState);
    ESSM::PlayerHijackManager::Hijack(*saveState, id);

    collection.clear();
    collection.reserve(this->_movingBoxContents.size());
    ESSM::Utils::CollectStates(this->_movingBoxContents, collection);
    ESSM::Core::CopySaveStates(collection);

    ESSM::Core::RestoreEntity(*this, id);
}

HOOK_METHOD(Entity_Familiar, RestoreState, (FamiliarData* saveState) -> void)
{
    uint32_t id = ESSM::FamiliarHijackManager::UnHijack(*saveState);
    super(saveState);
    ESSM::FamiliarHijackManager::Hijack(*saveState, id);
    ESSM::Core::RestoreEntity(*this, ESSM::FamiliarHijackManager::GetId(*saveState));
}

static void __stdcall asm_restore_game_state_backwards_rooms()
{
    CollectedStates collection;
    collection.reserve(DEFAULT_COLLECT_RESERVE);
    ESSM::EntityIterators::InGame::ForEachInBackwardsStages(ESSM::Utils::CollectLambda, collection);
    ESSM::Core::CopySaveStates(collection);
}

static void Patch_GameRestoreState_PostBackwardsStageDescRestore()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Game_RestoreState_PostBackwardsStageDescRestore);
    ZHL::Log("[REPENTOGON] Patching Game::RestoreState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    const size_t RESTORED_BYTES = 6;

    patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_restore_game_state_backwards_rooms)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_room_saved_entities()
{
    RoomDescriptor& room = *g_Game->_room->_descriptor;

    CollectedStates collection;
    collection.reserve(room.SavedEntities.size());
    ESSM::Utils::CollectStates(room.SavedEntities, collection);
    ESSM::Core::ClearSaveStates(collection);
}

static void common_clear_room_saved_entities_patch(const char* id, const char* logIdentifier)
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(id);
    ZHL::Log("[REPENTOGON] Patching %s for SaveStateManagement at %p\n", logIdentifier, addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    const size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_clear_room_saved_entities)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void Patch_RoomSaveState_ClearSavedEntities()
{
    common_clear_room_saved_entities_patch(&AsmDefinitions::Room_SaveState_ClearSavedEntities, "Room::SaveState");
}

static void Patch_RoomRestoreState_ClearSavedEntities()
{
    common_clear_room_saved_entities_patch(&AsmDefinitions::Room_RestoreState_ClearSavedEntities, "Room::RestoreState");
}

static void __stdcall asm_copy_myosotis_pickups()
{
    auto& vector = g_Game->_myosotisPickups;

    CollectedStates collection;
    collection.reserve(vector.size());
    ESSM::Utils::CollectStates(vector, collection);
    ESSM::Core::CopySaveStates(collection);
}

static void Patch_LevelInit_PostMyosotisEffect()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_Init_PostMyosotisEffect);
    ZHL::Log("[REPENTOGON] Patching Level::Init for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    const size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_copy_myosotis_pickups)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_restore_level_game_state()
{
    CollectedStates collection;
    collection.reserve(DEFAULT_COLLECT_RESERVE);
    ESSM::EntityIterators::InGame::ForEachInRooms(ESSM::Utils::CollectLambda, collection);
    ESSM::Core::CopySaveStates(collection);
}

static void Patch_LevelRestoreGameState_PreRoomLoad()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_RestoreGameState_PreRoomLoad);
    ZHL::Log("[REPENTOGON] Patching Level::RestoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    const size_t RESTORED_BYTES = 6;

    patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_restore_level_game_state)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_vector_pre_backwards_assign(std::vector<EntitySaveState>* saveStateVector)
{
    auto& vector = *saveStateVector;

    CollectedStates collection;
    collection.reserve(vector.size());
    ESSM::Utils::CollectStates(vector, collection);
    ESSM::Core::ClearSaveStates(collection);
}

static void __stdcall asm_copy_vector_post_backwards_assign(std::vector<EntitySaveState>* saveStateVector)
{
    auto& vector = *saveStateVector;

    CollectedStates collection;
    collection.reserve(vector.size());
    ESSM::Utils::CollectStates(vector, collection);
    ESSM::Core::CopySaveStates(collection);
}

static void Patch_LevelPlaceRoomsBackwards_Boss_AssignEntitySaveStateVector()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_place_rooms_backwards_Boss_AssignEntitySaveStateVector);
    ZHL::Log("[REPENTOGON] Patching Level::place_rooms_backwards for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t callAddr = addr + 8;
    intptr_t resumeAddr = callAddr + 5;
    const size_t RESTORED_BYTES = 8;
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
    const size_t RESTORED_BYTES = 10;
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

static void __fastcall asm_hijack_dark_closet_collectible(EntitySaveState& saveState)
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
    const size_t RESTORED_BYTES = 7;
    const int32_t FIELD_OFFSET = offsetof(EntitySaveState, _intStorage7);
    
    int8_t disp8 = *(int8_t*)disp8Addr;
    int32_t saveStateOffset_EBP = disp8 - FIELD_OFFSET;

    patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .PreserveRegisters(savedRegisters)
        .LoadEffectiveAddress(ASMPatch::Registers::EBP, saveStateOffset_EBP, ASMPatch::Registers::ECX)
        .AddInternalCall(asm_hijack_dark_closet_collectible)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_moving_box_vector(Entity_Player& player)
{
    auto& vector = player._movingBoxContents;

    CollectedStates collection;
    collection.reserve(vector.size());
    ESSM::Utils::CollectStates(vector, collection);
    ESSM::Core::ClearSaveStates(collection);
}

static void Patch_PlayerUseActiveItem_MovingBoxClearVector()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseActiveItem_MovingBox_ClearVector);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::UseActiveItem for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    const size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EDI) // player
        .AddInternalCall(asm_clear_moving_box_vector)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall asm_store_familiar_data(FamiliarData& saveState, size_t updateELIndex)
{
    EntityList_EL& updateEL = *g_Game->_room->_entityList.GetUpdateEL();
    assert(updateELIndex < updateEL._size);
    Entity* entity = updateEL._data[updateELIndex];
    assert(entity);

    uint32_t id = ESSM::FamiliarHijackManager::NewHijack(saveState);
    ESSM::Core::SaveEntity(*entity, id);
}

static void Patch_PlayerStoreGameState_FamiliarStoreState()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_StoreGameState_FamiliarStoreState);
    ZHL::Log("[REPENTOGON] Patching EntityPlayer::StoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    const size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .MoveFromMemory(ASMPatch::Registers::EBP, -0x28, ASMPatch::Registers::EDX) // roomELIndex
        .LoadEffectiveAddress(ASMPatch::Registers::EBP, -0x90, ASMPatch::Registers::ECX) // saveState
        .AddInternalCall(asm_store_familiar_data)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_clear_familiar_vector(std::vector<FamiliarData>* saveStateVector)
{
    auto& vector = *saveStateVector;

    CollectedStates collection;
    collection.reserve(vector.size());
    ESSM::Utils::CollectStates(vector, collection);
    ESSM::Core::ClearSaveStates(collection);
}

static void __stdcall asm_copy_familiar_vector(std::vector<FamiliarData>* saveStateVector)
{
    auto& vector = *saveStateVector;

    CollectedStates collection;
    collection.reserve(vector.size());
    ESSM::Utils::CollectStates(vector, collection);
    ESSM::Core::CopySaveStates(collection);
}

static void Patch_PlayerStoreGameState_AssignUnlistedFamiliarData()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_StoreGameState_AssignUnlistedFamiliarData);
    ZHL::Log("[REPENTOGON] Patching Entity_Player::StoreGameState for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t callAddr = addr + 8;
    intptr_t resumeAddr = callAddr + 5;
    const size_t RESTORED_BYTES = 8;
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
    const size_t RESTORED_BYTES = 15;
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

        CollectedStates collection = {saveState};
        ESSM::Core::ClearSaveStates(collection);

        LogDebug(LOG_DEBUG_HEADER "Smart pointer Cleared: %u\n", id);
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
    LogDebug(LOG_DEBUG_HEADER "New Flip State: %u\n", id);
}

static void Patch_PickupInitFlipState_CreateSaveState()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPickup_InitFlipState_PostSaveStateConstructor);
    ZHL::Log("[REPENTOGON] Patching Entity_Pickup::InitFlipState for SaveStateManagent at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    const size_t RESTORED_BYTES = 6;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EAX) // arg
        .AddInternalCall(asm_hijack_new_flip_state)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall asm_flip_restore(Entity& entity, EntitySaveState& saveState)
{
    ESSM::Core::RestoreEntity(entity, ESSM::EntityHijackManager::GetId(saveState));
}

static void Patch_PickupTryFlip_RestoreFlipState()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPickup_TryFlip_PostRestoreFlipState);
    ZHL::Log("[REPENTOGON] Patching Entity_Pickup::TryFlip for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 6;
    const size_t RESTORED_BYTES = 4;
    const size_t JNZ_OP_CODE_SIZE = 2;
    const intptr_t JNZ_OFFSET = 4;
    const size_t SMART_PTR_SAVE_STATE_OFFSET = offsetof(KAGE_SmartPointer_EntitySaveState, saveState);
    intptr_t relativeJmpAddr = addr + (intptr_t)RESTORED_BYTES + (intptr_t)JNZ_OP_CODE_SIZE + JNZ_OFFSET;

    // EDI is a reference to the FlipState SmartPointer
    patch.PreserveRegisters(savedRegisters)
        .MoveFromMemory(ASMPatch::Registers::EDI, SMART_PTR_SAVE_STATE_OFFSET, ASMPatch::Registers::EDX) // saveState
        .CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::ESI) // entity
        .AddInternalCall(asm_flip_restore)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (void*)relativeJmpAddr)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_change_mineshaft_room(size_t listIdx)
{
    Game* game = g_Game;
    RoomDescriptor& room = game->_gridRooms[listIdx];
    auto& vector = room.SavedEntities;

    CollectedStates collection;
    collection.reserve(vector.size());
    ESSM::Utils::CollectStates(vector, collection);
    ESSM::Core::ClearSaveStates(collection);
}

static void Patch_EntityNPCAiMothersShadow_ChangeMineshaftRoom()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_ai_mothers_shadow_ChangeMineshaftRoom);
    ZHL::Log("[REPENTOGON] Patching Entity_NPC::ai_mothers_shadow for SaveStateManagement at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 5;
    const size_t RESTORED_BYTES = 5;

    patch.PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::EBP, -0x48) // listIdx
        .AddInternalCall(asm_change_mineshaft_room)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

void ESSM::detail::Patches::ApplyPatches()
{
    Patch_ReferenceCount_EntitySaveStateDestructor();
    Patch_LevelInit_PostMyosotisEffect();
    Patch_LevelRestoreGameState_PreRoomLoad();
    Patch_LevelPlaceRoomsBackwards_Boss_AssignEntitySaveStateVector();
    Patch_LevelPlaceRoomsBackwards_Treasure_AssignEntitySaveStateVector();
    Patch_LevelGenerateDarkCloset_PostGenerateCollectibleSaveState();
    Patch_RoomSaveState_ClearSavedEntities();
    Patch_RoomRestoreState_ClearSavedEntities();
    Patch_GameRestoreState_PostBackwardsStageDescRestore();
    Patch_PlayerManagerRestoreGameState_AssignBackupFamiliarData();
    Patch_PlayerStoreGameState_FamiliarStoreState();
    Patch_PlayerStoreGameState_AssignUnlistedFamiliarData();
    Patch_PlayerUseActiveItem_MovingBoxClearVector();
    Patch_PickupInitFlipState_CreateSaveState();
    Patch_PickupTryFlip_RestoreFlipState();
    Patch_EntityNPCAiMothersShadow_ChangeMineshaftRoom();
}

#pragma endregion // Hooks

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

        // lua ids are sent over as if they were 1-indexed
        assert(EntityHijackManager::IsHijacked(state));
        lua_pushinteger(L, EntityHijackManager::GetId(state) + 1);
        return 1;
    }

    static std::filesystem::path GetLuaSaveDataPath(const ModReference& mod, const std::string& filename)
    {
        return std::filesystem::path(mod._dataDirectory) / "EntitySaveStateManagement" / (filename + ".dat");
    }

    LUA_FUNCTION(SaveData)
    {
        ModReference& modReference = get_mod_reference(L, 1);
        std::string filename = luaL_checkstring(L, 2);
        size_t len;
        // there is no need to copy the string
        const char* data = luaL_checklstring(L, 3, &len);

        std::filesystem::path filePath = GetLuaSaveDataPath(modReference, filename);
        bool exists = REPENTOGON::EnsureParentPathExists(filePath);
        if (!exists)
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to create directory for Mod save file at \"%s\"\n", filePath.string().c_str());
            return 0;
        }

        std::ofstream file = std::ofstream(filePath, std::ios::binary);
        if (!file)
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to open Mod file for writing at \"%s\"\n", filePath.string().c_str());
            return 0;
        }

        file.write(data, len);
        return 0;
    }

    LUA_FUNCTION(LoadData)
    {
        ModReference& modReference = get_mod_reference(L, 1);
        std::string filename = luaL_checkstring(L, 2);

        std::filesystem::path filePath = GetLuaSaveDataPath(modReference, filename);
        std::error_code errorCode;
        if (!std::filesystem::exists(filePath, errorCode))
        {
            lua_pushnil(L);
            return 1;
        }

        std::ifstream file = std::ifstream(filePath, std::ios::binary);
        if (!file)
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to open Mod file for reading at \"%s\"\n", filePath.string().c_str());
            lua_pushnil(L);
            return 1;
        }

        file.seekg(0, std::ios::end);
        std::streamoff lenoff = file.tellg();

        if (lenoff < 0)
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to get the length of Mod file at \"%s\"\n", filePath.string().c_str());
            lua_pushnil(L);
            return 1;
        }

        size_t len = static_cast<size_t>(lenoff);
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

        std::filesystem::path filePath = GetLuaSaveDataPath(modReference, filename);
        std::error_code errorCode;
        if (!std::filesystem::exists(filePath, errorCode))
        {
            return 0;
        }

        std::filesystem::remove(filePath, errorCode);

        if (errorCode)
        {
            ZHL::Log(LOG_ERROR_HEADER "unable to delete Mod file at \"%s\"\n", filePath.string().c_str());
        }

        return 0;
    }
}

void ESSM::detail::Init::RegisterLuaInternals(lua_State *L, int tbl)
{
    lua_pushvalue(L, tbl);
    lua::TableAssoc(L, "GetEntitySaveStateId", LuaFunctions::GetEntitySaveStateId);
    lua::TableAssoc(L, "SaveData", LuaFunctions::SaveData);
    lua::TableAssoc(L, "LoadData", LuaFunctions::LoadData);
    lua::TableAssoc(L, "DeleteData", LuaFunctions::DeleteData);
    lua_pop(L, 1);
}

// make it high priority so that it only checks stability after everything is done
HOOK_METHOD_PRIORITY(Room, Init, -9999, (int param_1, RoomDescriptor * descriptor) -> void)
{
    super(param_1, descriptor);
    ESSM::StabilityChecker::CheckStability();
}

#ifndef NDEBUG

HOOK_STATIC_PRIORITY(Manager, Update, -9999, (bool unk) -> void, __stdcall)
{
    super(unk);
    ESSM::StabilityChecker::CheckStability();
}

#endif