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
        struct _WriteState
        {
            std::vector<std::pair<GameStatePlayer*, uint32_t>> writtenPlayerStates;
            std::vector<std::pair<FamiliarData*, uint32_t>> writtenFamiliarData;
            std::vector<std::pair<EntitySaveState*, uint32_t>> writtenEntitySaveStates;
            std::vector<std::pair<uint32_t, uint32_t>> writeEntityIdPairs; // write, id (for Lua Callbacks)
            std::vector<std::pair<uint32_t, uint32_t>> writePlayerIdPairs; // write, id (for Lua Callbacks)
            std::vector<std::pair<uint32_t, uint32_t>> writeFamiliarIdPairs; // write, id (for Lua Callbacks)

            // Free up memory after we are done
            public: void FreeMemory()
            {
                this->writtenPlayerStates.clear();
                this->writtenFamiliarData.clear();
                this->writtenEntitySaveStates.clear();
                this->writeEntityIdPairs.clear();
                this->writePlayerIdPairs.clear();
                this->writeFamiliarIdPairs.clear();
            }

            private: _WriteState() = default;

            friend struct WriteState;
        };

        struct _ReadState
        {
            std::vector<EntitySaveState*> readEntitySaveStates;
            uint32_t minReadEntities = 0;
            std::vector<GameStatePlayer*> readPlayerStates;
            uint32_t minReadPlayers = 0;
            std::vector<FamiliarData*> readFamiliarData;
            uint32_t minReadFamiliars = 0;
            std::vector<std::pair<uint32_t, uint32_t>> restoreEntityIdPairs; // readId, id (for Lua Callbacks)
            std::vector<std::pair<uint32_t, uint32_t>> restorePlayerIdPairs; // readId, id (for Lua Callbacks)
            std::vector<std::pair<uint32_t, uint32_t>> restoreFamiliarIdPairs; // readId, id (for Lua Callbacks)
            std::bitset<3> errors;

            // Free up memory after we are done
            public: void FreeMemory()
            {
                this->readEntitySaveStates.clear();
                this->readPlayerStates.clear();
                this->readFamiliarData.clear();
                this->restoreEntityIdPairs.clear();
                this->restorePlayerIdPairs.clear();
                this->restoreFamiliarIdPairs.clear();
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

    void ApplyPatches();
}