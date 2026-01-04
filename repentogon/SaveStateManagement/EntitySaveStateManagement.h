#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"

namespace EntitySaveStateManagement
{
    // Unhijacked values.
    short& EntitySaveState_GetGridSpawnIdx(EntitySaveState& data);
    uint32_t& EntitySaveState_GetI7(EntitySaveState& data);

    uint32_t& GameStatePlayer_GetImmaculateConceptionState(GameStatePlayer& data);
    uint32_t& GameStatePlayer_GetCambionConceptionState(GameStatePlayer& data);

    uint32_t& FamiliarData_GetState(FamiliarData& data);
    uint32_t& FamiliarData_GetRoomClearCount(FamiliarData& data);

    void EntitySaveState_ClearBatch(const std::vector<EntitySaveState>& vector);

    void RegisterSaveState(EntitySaveState& saveState);
}

namespace EntitySaveStateManagement::detail::Init
{
    void RegisterLuaInternals(lua_State* L, int tbl);
    void BindLuaCallbacks(lua_State* L, int tbl);
}

namespace EntitySaveStateManagement::detail::Patches
{
    void ApplyPatches();
}

namespace EntitySaveStateManagement::detail::SaveData
{
    using StatePtr = std::variant<GameStatePlayer*, FamiliarData*, EntitySaveState*>;

    struct WrittenEntityState
    {
        StatePtr state;
        uint32_t originalId;

        WrittenEntityState(StatePtr state, uint32_t originalId) : state(state), originalId(originalId) {}
    };

    struct SerializeIdPair
    {
        uint32_t serializeId;
        uint32_t realId;

        SerializeIdPair(uint32_t serializeId, uint32_t realId) : serializeId(serializeId), realId(realId) {}
    };

    struct _WriteState
    {
        std::vector<WrittenEntityState> writtenEntitySaveStates;
        std::vector<SerializeIdPair> writeEntityIdPairs; // for Lua Callbacks

        // Free up memory after we are done
        public: void FreeMemory()
        {
            this->writtenEntitySaveStates = {};
            this->writeEntityIdPairs = {};
        }

        private: _WriteState() = default;

        friend struct WriteState;
    };

    struct _ReadState
    {
        std::vector<StatePtr> readEntitySaveStates;
        std::vector<SerializeIdPair> restoreEntityIdPairs; // for Lua Callbacks
        uint32_t minReadEntities = 0;
        std::bitset<3> errors;

        // Free up memory after we are done
        public: void FreeMemory()
        {
            this->readEntitySaveStates = {};
            this->restoreEntityIdPairs = {};
        }

        private: _ReadState() = default;

        friend struct ReadState;
    };

    struct WriteState
    {
        private: _WriteState m_writeState;

        friend WriteState WriteGameState();
        friend void Serialize(const std::string& fileName, WriteState& writeState);
        friend void RestoreWrittenStates(WriteState& writeState);
    };

    struct ReadState
    {
        private: _ReadState m_readState;

        friend ReadState ReadGameState();
        friend bool CheckErrors(const ReadState& readState);
        friend bool NeedsHandling(const ReadState& readState);
        friend bool Deserialize(const std::string& fileName, ReadState& readState);
    };

    WriteState WriteGameState();
    void Serialize(const std::string& fileName, WriteState& writeState);
    void RestoreWrittenStates(WriteState& writeState);

    /** Collects EntitySaveStates to be restored, also fixes them so GameState::Clear doesn't create problems.
     *  This should always run after a GameState is read, and should not be skipped.
    */
    ReadState ReadGameState();
    bool CheckErrors(const ReadState& readState);
    bool NeedsHandling(const ReadState& readState);
    bool Deserialize(const std::string& fileName, ReadState& readState);

    void DeleteGameState(const std::string& fileName);
}