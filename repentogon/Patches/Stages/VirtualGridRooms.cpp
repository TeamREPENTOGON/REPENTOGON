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

// Active set of custom off-grid rooms.
// TODO: Need to finalize how these are stored. Contiguous IDs are good, but haven't yet decided if mods should be able to "register" a specific index persistently, or only add rooms individually.
// Also, its possible for Data to end up null after quitting and continuing if stuff shifts around. Stuff to consider.
// Another note: Off-grid rooms do not set the "previous" room index, so using a devil card in an off-grid room will return you back to your last on-grid room when you leave.
// This is desirable for most off-grid rooms, however, so I'm glad custom indices are inheriting that, but worth keeping in mind since it may not be desirable for all usecases.
std::vector<RoomDescriptor> s_Rooms;

// Save state slots for custom off-grid rooms (the primary save data and the two glowing hourglass slots).
VirtualGridRoomSaveState s_SaveStates[3];

std::vector<RoomDescriptor>& VirtualGridRooms::GetVirtualRoomDescs() {
    return s_Rooms;
}

// Initializes a custom off-grid room at the next available index (starts at -21 currently).
// TODO: While the game does not actually use it as a room index, supposedly -100 and -101 are used for the mirror/mineshaft DOORS. Should look into this and avoid potential conflicts.
RoomDescriptor* AddVirtualRoomDesc(RoomConfig_Room* roomData, RNG& rng) {
    int gridIndex = -1 * (1 + NUM_OFF_GRID_ROOMS + s_Rooms.size());
    RoomDescriptor* newRoomDesc = &s_Rooms.emplace_back();

    newRoomDesc->GridIndex = gridIndex;
    newRoomDesc->InitSeeds(&rng);
    newRoomDesc->Data = roomData;

    return newRoomDesc;
}

void StoreVirtualGridRoomsState(GameState& state) {
	int slot = GetGameStateSlot(&state);

	ZHL::Log("[VirtualGridRooms] Storing slot %d\n", slot);

	s_SaveStates[slot].rooms.clear();

	for (const auto& roomDesc : s_Rooms) {
		VirtualGridRoomSaveState::Room& roomState = s_SaveStates[slot].rooms.emplace_back();
		if (roomDesc.Data) {
			Level::write_room_config(roomDesc.Data, &roomState.conf, nullptr);
		} else {
			ZHL::Log("[VirtualGridRooms] Can't properly store room with no Data\n");
		}
		roomState.desc = roomDesc;
		roomState.desc.Data = nullptr;
		roomState.desc.OverrideData = nullptr;
	}
}

void RestoreVirtualGridRoomsState(GameState& state) {
	int slot = GetGameStateSlot(&state);

	ZHL::Log("[VirtualGridRooms] Restoring slot %d\n", slot);

	if (!s_Rooms.empty()) {
		ZHL::Log("[VirtualGridRooms] ERROR: Rooms list was not cleared prior to restore!\n");
		s_Rooms.clear();
	}

	for (auto& roomState : s_SaveStates[slot].rooms) {
		RoomConfig_Room* room = Level::read_room_config(&roomState.conf, nullptr);
		if (room) {
			roomState.desc.Data = room;
		} else {
			ZHL::Log("[VirtualGridRooms] Failed to restore a room (stage %d, mode %d, type %d, variant %d)\n", roomState.conf.GetStageID(), roomState.conf.GetMode(), roomState.conf._type, roomState.conf._variant);
		}
		s_Rooms.emplace_back(roomState.desc);
		roomState.desc.Data = nullptr;
	}
}

// Validates a custom off-grid room index. If invalid, repairs with provided backup values.
template<typename T>
void SafetyCheckOffGridRoomIdx(int& roomIdx, T& dimension, const int backupRoomIdx, const int backupDimension) {
	if (roomIdx < -20 && g_Game->GetRoomByIdx(roomIdx, dimension)->Data == nullptr) {
		roomIdx = backupRoomIdx;
		dimension = backupDimension;
	}
}

// In Level::RestoreGameState, we need to restore custom off-grid rooms after reset_room_list, but before the game tried to access the current room.
// Doing it contextually in reset_room_list was an easy way to do that and also has the benefit of also being early enough to "fix" invalid indexes,
// just in case the write/read of the rooms failed somehow. If this ever needed to be replaced it might need to be an ASM patch.

GameState* s_RestoringGameState = nullptr;

HOOK_METHOD(Level, reset_room_list, (bool resetLilPortalRoom) -> void) {
	super(resetLilPortalRoom);
	s_Rooms.clear();
	if (GameState* state = s_RestoringGameState) {
		RestoreVirtualGridRoomsState(*state);

		if (state->_dimension == 2) {  // Only for Death Certificate dimension
			SafetyCheckOffGridRoomIdx(state->_backupLastRoomIdx, state->_backupLastDimension, g_Game->_startingRoomIdx, 0);
			SafetyCheckOffGridRoomIdx(state->_backupRoomIdx, state->_backupDimension, state->_backupLastRoomIdx, state->_backupLastDimension);
		}
		SafetyCheckOffGridRoomIdx(state->_lastRoomIdx, state->_lastDimension, g_Game->_startingRoomIdx, 0);
		SafetyCheckOffGridRoomIdx(state->_roomIdx, state->_dimension, state->_lastRoomIdx, state->_lastDimension);
	}
}

HOOK_METHOD(Level, RestoreGameState, (GameState* state) -> void) {
	s_RestoringGameState = state;
	super(state);
	s_RestoringGameState = nullptr;
}

HOOK_METHOD(Level, StoreGameState, (GameState* state) -> void) {
	StoreVirtualGridRoomsState(*state);
	super(state);
}

// Add a testing room: a random error room at index -21
// TODO: Remove this
HOOK_METHOD(Level, generate_dungeon, (RNG* rng) -> void) {
    super(rng);
    if (s_Rooms.empty()) {
        unsigned int doors = 0;
        RoomConfig_Room* room = g_Game->GetRoomConfig()->GetRandomRoom(rng->_seed, true, STB_SPECIAL_ROOMS, ROOM_ERROR, ROOMSHAPE_1x1, 0, -1, 0, 10, &doors, -1, -1);
        if (room) {
			AddVirtualRoomDesc(room, *rng);
        }
    }
}

// Make GetRoomByIdx return custom off-grid rooms, if applicable for indices <= -21
HOOK_METHOD(Level, GetRoomByIdx, (int32_t idx, int32_t dimension) -> RoomDescriptor*) {
    if (idx < -NUM_OFF_GRID_ROOMS) {
        int index = ((-idx) - NUM_OFF_GRID_ROOMS) - 1;
        if (index < s_Rooms.size()) {
            return &s_Rooms[index];
        } else {
			ZHL::Log("[VirtualGridRooms::GetRoomByIdx] Attempt to access invalid index %d\n", idx);
        }
    }
    return super(idx, dimension);
}

// If ChangeRoom manages to get an invalid index, the game will crash.
// Redirect to the starting room instead, since... anything's better than crashing.
// TODO: Make ChangeRoom and similar functions ERROR at the lua API level.
HOOK_METHOD(Level, ChangeRoom, (int targetRoomIdx, int dimension) -> void) {
	if (g_Game->GetRoomByIdx(targetRoomIdx, dimension)->Data == nullptr) {
		const std::string err = REPENTOGON::StringFormat("ERROR: Invalid room index %d passed to ChangeRoom - redirecting to starting room to avoid a crash!\n", targetRoomIdx);
		ZHL::Log("[VirtualGridRooms::ChangeRoom] %s", err.c_str());
		g_Game->GetConsole()->PrintError(err);
		targetRoomIdx = g_Game->_startingRoomIdx;
		dimension = 0;
	}
	super(targetRoomIdx, dimension);
}

std::vector<VirtualGridRoomSaveState::Room>& VirtualGridRooms::GetVirtualRoomDescsFromGameState(GameState& state) {
	int slot = GetGameStateSlot(&state);
	if (slot == GameStateSlot::NULL_SLOT) {
		static std::vector<VirtualGridRoomSaveState::Room> s_EmptyRoomsList;
		return s_EmptyRoomsList;
	}
	return s_SaveStates[slot].rooms;
}

HOOK_METHOD(GameState, Clear, () -> void) {
	super();

	int slot = GetGameStateSlot(this);
	if (slot != GameStateSlot::NULL_SLOT) {
		ZHL::Log("[VirtualGridRooms] Clearing state for slot: %d\n", slot);
		s_SaveStates[slot].rooms.clear();
	}
}

static inline std::string GetSaveFilePath(const std::string& filename) {
    return REPENTOGON::StringFormat("%sVirtualGridRooms/%s.dat", REPENTOGON::GetRepentogonDataPath(), filename.c_str());
}

// Helper class wrapped around the functions to read/write room data to a file.
class VirtualGridRoomsFileHelper{
public:
	VirtualGridRoomsFileHelper() = delete;
	VirtualGridRoomsFileHelper(KAGE_Filesys_File& file) {
		// TODO: Need to clean up some stuff related to GameStateIO
		fakeGameState._saveFile = (GameStateIO*)&file;
		fakeGameState._gameStateVersion = ISAAC_GAME_STATE_VERSION;

		gameStateIO._file = &file;
		gameStateIO._version = ISAAC_GAME_STATE_VERSION;
	}

	~VirtualGridRoomsFileHelper() {
		// Not necessary atm since these don't have destructors, but still wipe these for good measure.
		// This class doesn't own the file.
		fakeGameState._saveFile = nullptr;
		gameStateIO._file = nullptr;
	}

	bool WriteRoom(const VirtualGridRoomSaveState::Room& room) {
		return fakeGameState.write_room(&room.conf, &room.desc, &gameStateIO);
	}
	
	bool ReadRoom(VirtualGridRoomSaveState::Room& room) {
		return fakeGameState.read_room(&room.conf, &room.desc, &gameStateIO);
	}

	uint32_t GetChecksum() {
		return gameStateIO._checksum._checksum;
	}

private:
	GameState fakeGameState{};
	GameStateIO gameStateIO{};
};

bool TryWriteGameState(KAGE_Filesys_File& file) {
	const VirtualGridRoomSaveState& state = s_SaveStates[0];
	int numRooms = state.rooms.size();
	file.Write(&numRooms, 4, 1);

	auto helper = std::make_unique<VirtualGridRoomsFileHelper>(file);

	for (const VirtualGridRoomSaveState::Room& room : state.rooms) {
		if (!helper->WriteRoom(room)) {
			ZHL::Log("[VirtualGridRooms::WriteGameState] ERROR: write_room failed\n");
			return false;
		}
	}
	uint32_t checksum = helper->GetChecksum();
	file.Write(&checksum, 4, 1);

	file.Close();
	return true;
}

void VirtualGridRooms::WriteGameState(const std::string& fileName) {
    const std::string filePath = GetSaveFilePath(fileName);

    std::filesystem::create_directories(REPENTOGON::StringFormat("%s/VirtualGridRooms/", REPENTOGON::GetRepentogonDataPath()));

	ZHL::Log("[VirtualGridRooms::WriteGameState] Writing state to %s\n", filePath.c_str());

    KAGE_Filesys_File file;
    file.OpenWrite(filePath.c_str());

    if (!file.IsOpen()) {
		ZHL::Log("[VirtualGridRooms::WriteGameState] Failed to open file for writing.\n");
        return;
    }

    if (!TryWriteGameState(file)) {
		ZHL::Log("[VirtualGridRooms::WriteGameState] ERROR: Write failed, rooms may not be persisted.\n");
    }
}

bool TryReadGameState(KAGE_Filesys_File& file) {
    int numRooms = -1;
	file.Read(&numRooms, 4, 1);
    if (numRooms < 0) {
        ZHL::Log("[VirtualGridRooms::ReadGameState] ERROR: Failed to read numRooms\n");
        return false;
    }

    VirtualGridRoomSaveState& state = s_SaveStates[0];
	// The GameState is always expected to have been cleared before reading, but just in case.
	if (!state.rooms.empty()) {
		ZHL::Log("[VirtualGridRooms::ReadGameState] ERROR: State was not cleared!\n");
		state.rooms.clear();
	}

	auto helper = std::make_unique<VirtualGridRoomsFileHelper>(file);

    for (int i = 0; i < numRooms; i++) {
        VirtualGridRoomSaveState::Room& room = state.rooms.emplace_back();
        if (!helper->ReadRoom(room)) {
            ZHL::Log("[VirtualGridRooms::ReadGameState] ERROR: read_room failed\n");
            return false;
        }
    }

	uint32_t checksum = 0xFFFFFFFF;
	file.Read(&checksum, 4, 1);

	uint32_t expectedChecksum = helper->GetChecksum();

    if (checksum != expectedChecksum) {
        ZHL::Log("[VirtualGridRooms::ReadGameState] ERROR: Checksum mismatch\n");
        return false;
    }

    if (!file.IsEof()) {
		ZHL::Log("[VirtualGridRooms::ReadGameState] ERROR: Didn't reach end of file???\n");
    }

	file.Close();
    return true;
}

void VirtualGridRooms::ReadGameState(const std::string& fileName) {
    const std::string filePath = GetSaveFilePath(fileName);

	if (!std::filesystem::exists(filePath)) {
		return;
	}

	ZHL::Log("[VirtualGridRooms::ReadGameState] Reading state from %s\n", filePath.c_str());

    KAGE_Filesys_File file;
    file.OpenRead(filePath.c_str());

    if (!file.IsOpen()) {
		ZHL::Log("[VirtualGridRooms::ReadGameState] Failed to open file: %s\n", filePath.c_str());
        return;
    }

    if (!TryReadGameState(file)) {
        ZHL::Log("[VirtualGridRooms::ReadGameState] ERROR: Read failed, rooms will be cleared.\n");
		// In case we read incomplete state.
		s_SaveStates[0].rooms.clear();
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
