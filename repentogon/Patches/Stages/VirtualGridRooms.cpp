#include "VirtualGridRooms.h"

#include <Log.h>
#include <stdint.h>
#include <vector>
#include <memory>

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "../../SaveStateManagement/GameStateManagement.h"
#include "../../MiscFunctions.h"
#include "../../SaveStateManagement/EntitySaveStateManagement.h"

std::vector<RoomDescriptor> s_Rooms;

VirtualGridRoomSaveState s_SaveStates[3];

std::vector<RoomDescriptor>& VirtualGridRooms::GetVirtualRoomDescs() {
    return s_Rooms;
}

RoomDescriptor* AddVirtualRoomDesc(RoomConfig_Room* roomData, RNG& rng) {
    int gridIndex = -1 * (1 + NUM_OFF_GRID_ROOMS + s_Rooms.size());
    //g_Game->GetConsole()->Print("Added virtual off-grid room @ " + std::to_string(gridIndex) + "\n", 0xFFFFFFFF, 60);
    RoomDescriptor* newRoomDesc = &s_Rooms.emplace_back();

    newRoomDesc->GridIndex = gridIndex;
    newRoomDesc->InitSeeds(&rng);
    newRoomDesc->Data = roomData;

    return newRoomDesc;
}

GameState* s_RestoringGameState = nullptr;

HOOK_METHOD(Level, reset_room_list, (bool resetLilPortalRoom) -> void) {
	super(resetLilPortalRoom);
	s_Rooms.clear();
	if (s_RestoringGameState) {
		auto slot = GetGameStateSlot(s_RestoringGameState);
		if (slot == GameStateSlot::NULL_SLOT) {
			ZHL::Log("[INFO] [VirtualGridRooms] could not determine Game State slot, skipping restore.\n");
		} else {
			ZHL::Log("[INFO] [VirtualGridRooms] REstore game state\n");
			VirtualGridRooms::RestoreGameState(slot);
		}
	}
}

HOOK_METHOD(Level, RestoreGameState, (GameState* state) -> void) {
	s_RestoringGameState = state;
	super(state);
	s_RestoringGameState = nullptr;
}

HOOK_METHOD(Level, StoreGameState, (GameState* state) -> void) {
	auto slot = GetGameStateSlot(state);
	if (slot == GameStateSlot::NULL_SLOT) {
		ZHL::Log("[INFO] [VirtualGridRooms] could not determine Game State slot, skipping store.\n");
	} else {
		ZHL::Log("[INFO] [VirtualGridRooms] store game state\n");
		VirtualGridRooms::StoreGameState(slot);
	}

    super(state);
}

// Testing
HOOK_METHOD(Level, generate_dungeon, (RNG* rng) -> void) {
    super(rng);
    if (s_Rooms.empty()) {
        unsigned int doors = 0;
        RoomConfig_Room* room = g_Game->GetRoomConfig()->GetRandomRoom(rng->_seed, true, STB_SPECIAL_ROOMS, ROOM_ERROR, ROOMSHAPE_1x1, 0, -1, 0, 10, &doors, -1, -1);
        if (!room) {
            //g_Game->GetConsole()->Print("buhhh its null\n", 0xFFFFFFFF, 60);
        }
        AddVirtualRoomDesc(room, *rng);
    }
}

HOOK_METHOD(Level, GetRoomByIdx, (int32_t idx, int32_t dimension) -> RoomDescriptor*) {
    if (idx < -NUM_OFF_GRID_ROOMS) {
        int index = ((-idx) - NUM_OFF_GRID_ROOMS) - 1;
        if (s_Rooms.size() > index) {
            //if (g_Game->GetCurrentRoomDesc() != &s_Rooms[index]) {
                //g_Game->GetConsole()->Print("Accessing virtual off-grid room @ " + std::to_string(index) + "\n", 0xFFFFFFFF, 60);
            //}
            return &s_Rooms[index];
        } else {
            //g_Game->GetConsole()->Print("AAAAAAAGHGHHEHAGH @ " + std::to_string(idx) + "\n", 0xFFFFFFFF, 60);
        }
    }
    return super(idx, dimension);
}

HOOK_METHOD(Level, ChangeRoom, (int targetRoomIdx, int dimension) -> void) {
	if (targetRoomIdx < -NUM_OFF_GRID_ROOMS) {
		int index = ((-targetRoomIdx) - NUM_OFF_GRID_ROOMS) - 1;
		if (index >= s_Rooms.size()) {
			g_Game->GetConsole()->Print("AAAAAAAGHGHHEHAGH @ " + std::to_string(targetRoomIdx) + "\n", 0xFFFFFFFF, 60);
			targetRoomIdx = g_Game->_startingRoomIdx;
		}
	}
	super(targetRoomIdx, dimension);
}

std::vector<VirtualGridRoomSaveState::Room> s_EmptyList;

std::vector<VirtualGridRoomSaveState::Room>& VirtualGridRooms::GetVirtualRoomDescsFromGameState(uint32_t slot) {
    if (slot == GameStateSlot::NULL_SLOT) {
        ZHL::Log("[CONNOR ERROR] [VirtualGridRooms] GetVirtualRoomDescsFromGameState called with invalid slot\n");
        return s_EmptyList;
    }
    return s_SaveStates[slot].rooms;
}

std::vector<VirtualGridRoomSaveState::Room>& VirtualGridRooms::GetVirtualRoomDescsFromGameState(GameState& state) {
    return GetVirtualRoomDescsFromGameState(GetGameStateSlot(&state));
}

void VirtualGridRooms::StoreGameState(uint32_t slot) {
    //g_Game->GetConsole()->Print("StoreGameState @ " + std::to_string(slot) + "\n", 0xFFFFFFFF, 60);
   // g_Game->GetConsole()->Print("rooms @ " + std::to_string(virtualRoomDescs.size()) + "\n", 0xFFFFFFFF, 60);

    // Bound check
	s_SaveStates[slot].rooms.clear();

    ZHL::Log("[CONNOR] [VirtualGridRooms] StoreGameState %d\n", slot);

    for (auto& roomDesc : s_Rooms) {  // conste
       // g_Game->GetConsole()->Print("Storing virtual off-grid room @ " + std::to_string(m_SaveStates[slot].rooms.size()) + "\n", 0xFFFFFFFF, 60);
        VirtualGridRoomSaveState::Room& roomState = s_SaveStates[slot].rooms.emplace_back();
        if (roomDesc.Data) {
            Level::write_room_config(roomDesc.Data, &roomState.conf, nullptr);
        } else {
            //g_Game->GetConsole()->Print("NULL ROOM???\n", 0xFFFFFFFF, 60);
        }
        roomState.desc = roomDesc;
        roomState.desc.Data = nullptr;
        roomState.desc.OverrideData = nullptr;
    }
}

void VirtualGridRooms::RestoreGameState(uint32_t slot) {
    //g_Game->GetConsole()->Print("RestoreGameState @ " + std::to_string(slot) + "\n", 0xFFFFFFFF, 60);
    //g_Game->GetConsole()->Print("rooms @ " + std::to_string(m_SaveStates[slot].rooms.size()) + "\n", 0xFFFFFFFF, 60);

    ZHL::Log("[CONNOR] [VirtualGridRooms] Level RestoreGameState %d\n", slot);

	ZHL::Log("AAAAA: VGD RESTORE: %u\n", slot);

    for (RoomDescriptor& room : s_Rooms) {
        //EntitySaveStateManagement::ResetRoom(room);
    }
	s_Rooms.clear();

    for (auto& roomState : s_SaveStates[slot].rooms) {
        //unsigned int doors = 0; RoomConfig_Room* room = g_Game->GetRoomConfig()->GetRandomRoom(1234, true, STB_SPECIAL_ROOMS, ROOM_ERROR, ROOMSHAPE_1x1, 0, -1, 0, 10, &doors, -1, -1);
        g_Game->GetConsole()->Print("Restoring virtual off-grid room @ " + std::to_string(s_Rooms.size()) + "\n", 0xFFFFFFFF, 60);
        RoomConfig_Room* room = Level::read_room_config(&roomState.conf, nullptr);
        roomState.desc.Data = room;
        //virtualRoomDescs.push_back(roomState.desc);
		s_Rooms.emplace_back(roomState.desc);
        roomState.desc.Data = nullptr;
        // Handle null room, id shift
        //RoomConfig_Room* room = Level::read_room_config(&roomState.conf, nullptr);
        
        //virtualRoomDescs.back().Data = room;
        //AddVirtualRoomDesc(room, g_Game->_generationRNG);
    }
    /*
    virtualRoomDescs.clear();

    unsigned int doors = 0;
    RoomConfig_Room* room = g_Game->GetRoomConfig()->GetRandomRoom(1234, true, STB_SPECIAL_ROOMS, ROOM_ERROR, ROOMSHAPE_1x1, 0, -1, 0, 10, &doors, -1, -1);
    if (!room) {
        g_Game->GetConsole()->Print("buhhh its null\n", 0xFFFFFFFF, 60);
    }
    AddVirtualRoomDesc(room, g_Game->_generationRNG);*/
}

void VirtualGridRooms::ClearGameState(uint32_t slot) {
    if (slot == GameStateSlot::NULL_SLOT) {
        ZHL::Log("[CONNOR] [VirtualGridRooms] could not determine Game State slot, skipping clear.\n");
    } else {
        ZHL::Log("[CONNOR] GameState clear: %d\n", slot);
		//for (auto& room : m_SaveStates[slot].rooms) {
			//EntitySaveStateManagement::ResetRoom(room.desc);
		//}
		s_SaveStates[slot].rooms.clear();
    }
}
void VirtualGridRooms::ClearGameState(GameState& state) {
    ClearGameState(GetGameStateSlot(&state));
}

static inline std::string GetSaveFilePath(const std::string& filename) {
    return REPENTOGON::StringFormat("%sVirtualGridRooms/%s.dat", REPENTOGON::GetRepentogonDataPath(), filename.c_str());
}

GameState fakeGameState;

struct Something {
    KAGE_Filesys_IFile* io = nullptr;
    char* filname = nullptr;
    uint32_t version = 163;
    Checksum checksum;
};

bool TryWriteGameState(Something& something) {
    KAGE_Filesys_IFile* io = something.io;

    VirtualGridRoomSaveState& state = s_SaveStates[0];  // const
    int numRooms = state.rooms.size();
    io->Write(&numRooms, 4, 1);
    for (auto& room : state.rooms) {
        if (!fakeGameState.write_room(&room.conf, &room.desc, (GameStateIO*)&something)) {
            ZHL::Log("TryWriteGameState ERROR: write_room failed\n");
            return false;
        }
    }
    int checksum = 0;
    io->Write(&checksum, 4, 1);

    return true;
}

void VirtualGridRooms::WriteGameState(const std::string& fileName) {
    const std::string filePath = GetSaveFilePath(fileName);

    std::filesystem::create_directories(REPENTOGON::StringFormat("%s/VirtualGridRooms/", REPENTOGON::GetRepentogonDataPath()));

    ZHL::Log("WriteGameState: %s\n", filePath.c_str());

    //auto* io = g_SaveDataManager->OpenSaveFileForWriting(filePath.c_str());
    KAGE_Filesys_File file;
    file.OpenWrite(filePath.c_str());

    if (!file.IsOpen()) {
        ZHL::Log("WriteGameState file not open\n");
        return;
    }

    Something something;
    something.io = &file;

    //fakeGameState._saveFile = (GameStateIO*)&file;
    fakeGameState._saveFile = (GameStateIO*)something.io;
    fakeGameState._gameStateVersion = 163;
    bool success = TryWriteGameState(something);
    fakeGameState._saveFile = nullptr;

    file.Close();
    //io->Close();
    //io->Free(true);

    if (!success) {
        ZHL::Log("WriteGameState FAILED\n");
    }
}

bool TryReadGameState(Something& something) {
    KAGE_Filesys_IFile* io = something.io;

    int numRooms = -1;
    io->Read(&numRooms, 4, 1);
    if (numRooms < 0) {
        ZHL::Log("TryReadGameState ERROR: Failed to read numRooms\n");
        return false;
    }

    for (int slot = 0; slot < 3; slot++) {
        VirtualGridRoomSaveState& state = s_SaveStates[slot];
        for (auto& room : state.rooms) {
            //EntitySaveStateManagement::ResetRoom(room.desc);
        }
        state.rooms.clear();
    }

    VirtualGridRoomSaveState& state = s_SaveStates[0];

    for (int i = 0; i < numRooms; i++) {
        VirtualGridRoomSaveState::Room& room = state.rooms.emplace_back();
        ZHL::Log("TryReadGameState: Starting read_room...\n");
        if (io->IsEof()) {
            ZHL::Log("TryReadGameState: pre-EOF\n");
        }
        if (!fakeGameState.read_room(&room.conf, &room.desc, (GameStateIO*)&something)) {
            ZHL::Log("TryReadGameState ERROR: read_room failed\n");
            if (io->IsEof()) {
                ZHL::Log("TryReadGameState: post-EOF\n");
            }
            return false;
        }
        if (io->IsEof()) {
            ZHL::Log("TryReadGameState: ok post-EOF\n");
        }
    }
    int checksum = -1;
    io->Read(&checksum, 4, 1);

    if (checksum != 0) {
        ZHL::Log("TryReadGameState ERROR: Checksum mismatch\n");
        return false;
    }

    if (!io->IsEof()) {
        ZHL::Log("TryReadGameState ERROR: Not at end of file???\n");
    }
    
    return true;
}

void VirtualGridRooms::ReadGameState(const std::string& fileName) {
    const std::string filePath = GetSaveFilePath(fileName);
    //auto* io = g_SaveDataManager->OpenSaveFileForReading(filePath.c_str());
    //GameStateIO* io = nullptr;

    KAGE_Filesys_File file;
    file.OpenRead(filePath.c_str());

    if (!file.IsOpen()) {
        ZHL::Log("ReadGameState file not found\n");
        return;
    }

    Something something;
    something.io = &file;

    fakeGameState._saveFile = (GameStateIO*)something.io;
    fakeGameState._gameStateVersion = 163;
    bool success = TryReadGameState(something);
    fakeGameState._saveFile = nullptr;
    
    file.Close();
    //io->Close();
    //io->Free(true);

    if (!success) {
        ZHL::Log("ReadGameState FAILED\n");
        //std::filesystem::remove(filePath);
    }
}

void VirtualGridRooms::DeleteGameState(const std::string& fileName) {
    const std::string filePath = GetSaveFilePath(fileName);
    std::filesystem::remove(filePath);
}

void VirtualGridRooms::ApplyPatches() {
	// Allows "invalid" negative GridRoom indices to pass a check in ChangeRoom.
	sASMPatcher.FlatPatch(sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_ChangeRoom_NegativeGridRoomCheck), "\x90\x90\x90", 3);
}
