#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "IsaacRepentance.h"

struct VirtualGridRoomSaveState {
    struct Room {
        RoomDescriptor desc;
        GameStateRoomConfig conf;
    };

    std::vector<Room> rooms;
};

namespace VirtualGridRooms {
    void StoreGameState(uint32_t slot);
    void RestoreGameState(uint32_t slot);
    void ClearGameState(uint32_t slot);
    void ClearGameState(GameState& state);

    std::vector<RoomDescriptor>& GetVirtualRoomDescs();
    std::vector<VirtualGridRoomSaveState::Room>& GetVirtualRoomDescsFromGameState(GameState& state);
    std::vector<VirtualGridRoomSaveState::Room>& GetVirtualRoomDescsFromGameState(uint32_t slot);

    void WriteGameState(const std::string& fileName);
    void ReadGameState(const std::string& fileName);
    void DeleteGameState(const std::string& fileName);

	void ApplyPatches();
}  // namespace VirtualGridRooms
