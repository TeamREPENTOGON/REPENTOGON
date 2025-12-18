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

    // Clears all ids from the passed vector
    // DOES NOT CLEAR THE VECTOR, IT MUST BE MANUALLY CLEARED AFTERWARDS!!!
    void EntitySaveState_ClearVector(std::vector<EntitySaveState>& vector);
}

namespace EntitySaveStateManagement::detail
{
    namespace Init
    {
        void RegisterLuaInternals(lua_State* L);
        void BindLuaCallbacks(lua_State* L, int tbl);
    }

    namespace SaveData
    {
        using StatePtr = std::variant<GameStatePlayer*, FamiliarData*, EntitySaveState*>;

        struct _WriteState
        {
            std::vector<std::pair<StatePtr, uint32_t>> writtenEntitySaveStates;
            std::vector<std::pair<uint32_t, uint32_t>> writeEntityIdPairs; // write, id (for Lua Callbacks)

            // Free up memory after we are done
            public: void FreeMemory()
            {
                this->writtenEntitySaveStates.clear();
                this->writeEntityIdPairs.clear();
            }

            private: _WriteState() = default;

            friend struct WriteState;
        };

        struct _ReadState
        {
            std::vector<StatePtr> readEntitySaveStates;
            std::vector<std::pair<uint32_t, uint32_t>> restoreEntityIdPairs; // readId, id (for Lua Callbacks)
            uint32_t minReadEntities = 0;
            std::bitset<3> errors;

            // Free up memory after we are done
            public: void FreeMemory()
            {
                this->readEntitySaveStates.clear();
                this->restoreEntityIdPairs.clear();
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

    namespace Patches
    {
        void ApplyPatches();
    }
}