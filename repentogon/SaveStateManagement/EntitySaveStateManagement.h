#pragma once

#include "IsaacRepentance.h"

namespace EntitySaveStateManagement
{
    namespace SaveData
    {
        struct WriteState
        {
            std::vector<std::pair<EntitySaveState*, uint32_t>> writtenSaveStates;
        };

        struct ReadState
        {
            std::vector<EntitySaveState*> readSaveStates;
            std::vector<std::pair<uint32_t, uint32_t>> restoredSaveStates; // readId, id
            uint32_t maxId = 0;
            std::bitset<2> errors;
        };

        WriteState WriteGameState() noexcept;
        void Serialize(const std::string& fileName, const WriteState& writeState) noexcept;
        void RestoreWrittenStates(WriteState& writeState) noexcept;

        /** Collects EntitySaveStates to be restored, also fixes them so GameState::Clear doesn't create problems.
         *  This should always run after a GameState is read, and should not be skipped.
        */
        ReadState ReadGameState() noexcept;
        inline bool CheckErrors(const ReadState& readState) noexcept { return readState.errors.any(); }
        inline bool NeedsHandling(const ReadState& readState) noexcept { return !readState.readSaveStates.empty(); }
        bool Deserialize(const std::string& fileName, ReadState& readState) noexcept;
    }

    void ApplyPatches() noexcept;
}