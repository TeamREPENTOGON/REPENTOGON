#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "IsaacRepentance.h"

struct VirtualGridRoomSaveState {
    struct Room {
		RoomDescriptor desc{};
		GameStateRoomConfig conf{};
    };

    std::vector<Room> rooms;
};

namespace VirtualGridRooms {
    std::vector<RoomDescriptor>& GetVirtualRoomDescs();
    std::vector<VirtualGridRoomSaveState::Room>& GetVirtualRoomDescsFromGameState(GameState& state);

    void WriteGameState(const std::string& fileName);
    void ReadGameState(const std::string& fileName);
    void DeleteGameState(const std::string& fileName);

	void ApplyPatches();
}  // namespace VirtualGridRooms
