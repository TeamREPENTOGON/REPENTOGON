#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"

namespace EntitySaveStateManagement
{
    namespace Init
    {
        void RegisterLuaInternals(lua_State* L);
        void BindLuaCallbacks(lua_State* L, int tbl);
    }

    namespace SaveData
    {
        struct WriteState
        {
            std::vector<std::pair<GameStatePlayer*, uint32_t>> writtenPlayerStates;
            std::vector<std::pair<FamiliarData*, uint32_t>> writtenFamiliarData;
            std::vector<std::pair<EntitySaveState*, uint32_t>> writtenEntitySaveStates;
        };

        struct ReadState
        {
            std::vector<EntitySaveState*> readEntitySaveStates;
            std::vector<std::pair<uint32_t, uint32_t>> restoredEntitySaveStates; // readId, id
            uint32_t minReadEntities = 0;
            std::vector<GameStatePlayer*> readPlayerStates;
            std::vector<std::pair<uint32_t, uint32_t>> restoredPlayerStates; // readId, id
            uint32_t minReadPlayers = 0;
            std::vector<FamiliarData*> readFamiliarData;
            std::vector<std::pair<uint32_t, uint32_t>> restoredFamiliarData; // readId, id
            uint32_t minReadFamiliars = 0;
            std::bitset<3> errors;
        };

        WriteState WriteGameState();
        void Serialize(const std::string& fileName, const WriteState& writeState);
        void RestoreWrittenStates(WriteState& writeState);

        /** Collects EntitySaveStates to be restored, also fixes them so GameState::Clear doesn't create problems.
         *  This should always run after a GameState is read, and should not be skipped.
        */
        ReadState ReadGameState();
        inline bool CheckErrors(const ReadState& readState) { return readState.errors.any(); }
        inline bool NeedsHandling(const ReadState& readState) { return !readState.readEntitySaveStates.empty() || !readState.readPlayerStates.empty() || !readState.readFamiliarData.empty();  }
        bool Deserialize(const std::string& fileName, ReadState& readState);

        void DeleteGameState(const std::string& fileName);
    }

    void ApplyPatches();
}