#include "IsaacRepentance.h"

// Converts level GridIndex to level coordinates.
inline XY RoomIndexToCoords(const int gridIndex) {
	return XY(gridIndex % 13, (int)std::floor(gridIndex / 13));
}

// Converts level coordinates to level GridIndex.
inline int RoomCoordsToIndex(const int column, const int line) {
	return line * 13 + column;
}
inline int RoomCoordsToIndex(const XY& coords) {
	return RoomCoordsToIndex(coords.x, coords.y);
}

// Converts DoorSlot to DoorMask.
inline int DoorSlotToMask(const int doorSlot) {
	return 1 << doorSlot;
	//return 1 << ((doorSlot & 31) & 0xFFFF);  // Why was I doing it like this???
}

// Given a DoorSlot, returns a vector containing the two DoorSlots that it may connect to in an adjacent room.
// For example, RIGHT1 could connect to either LEFT0 or LEFT1.
inline std::vector<int> GetPossibleReturnDoorSlots(const int doorSlot) {
	const int baseReturnDoorSlot = (doorSlot + 2) % 4;
	return { baseReturnDoorSlot, baseReturnDoorSlot + 4 };
}

// Represents a connection between two doors at two different (adjacent) locations.
struct DoorSourceTarget {
	XY source = XY(-1, -1);
	XY target = XY(-1, -1);

	// Converts the respective coordinates to a (level) GridIndex.
	int GetSourceIdx() const {
		if (!IsValid()) return -1;
		return RoomCoordsToIndex(source);
	}
	int GetTargetIdx() const {
		if (!IsValid()) return -1;
		return RoomCoordsToIndex(target);
	}

	// Returns true if `source` and `target` are populated as a valid connection.
	bool IsValid() const {
		return source.x > -1 && source.x < 13 && source.y > -1 && source.y < 13
			&& target.x > -1 && target.x < 13 && target.y > -1 && target.y < 13
			&& (source.x == target.x || source.y == target.y)
			&& (std::abs(source.x - target.x) == 1 || std::abs(source.y - target.y) == 1);
	}

	// Returns true if the provided DoorSlotTarget is the inverse of this one (ie, the same connection from the other direction).
	bool ConnectsTo(const DoorSourceTarget& other) const {
		return IsValid() && other.IsValid() && GetSourceIdx() == other.GetTargetIdx() && GetTargetIdx() == other.GetSourceIdx();
	}
};

// Returns a DoorSourceTarget struct that represents a possible connection between two doors, based on a room at the given GridIndex with the given RoomShape, via the given DoorSlot.
// If `normalizeRoomShape` is set, "thin" rooms will be treated as their non-thin variants (this is useful for determining if a room will fit at the given location).
DoorSourceTarget GetDoorSourceTarget(const int gridIndex, const int roomShape, const int doorSlot, const bool normalizeRoomShape);

// Returns a map of DoorSlot->Neighboring RoomDescriptor, for a room of the given shape at the given GridIndex.
std::map<int, RoomDescriptor*> GetNeighboringRooms(const int gridIndex, const int roomShape, const int dimension);

// allowMultipleDoors - If false, placement is only valid if the room has only one door (like special rooms). Connections to regular secret rooms don't count as a door.
// allowSpecialNeighbors - If false, the room cannot be placed with any special rooms as neighbors (respecting the general rule of special room placement). Regular secret rooms don't count.
// allowNoNeighbors - If true, allows the room to be placed out in the void with no connections to other rooms.

// Returns true if the room could be placed at the specified location.
bool CanPlaceRoom(RoomConfig_Room* room, const int column, const int line, const int dimension, const bool allowMultipleDoors, const bool allowSpecialNeighbors, const bool allowNoNeighbors);

// Attempt to place a room at the specified location. The room must fit and be capable of connecting doors to all adjacent rooms.
// Returns a pointer to the new RoomDescriptor if successful.
RoomDescriptor* TryPlaceRoom(RoomConfig_Room* roomConfig, const int column, const int line, const int dimension, const uint32_t seed, const bool allowMultipleDoors, const bool allowSpecialNeighbors, const bool allowNoNeighbors);

// Returns true if the room could be placed connected to the provided existing RoomDescriptor via the given door.
bool CanPlaceRoomAtDoor(RoomConfig_Room* roomConfig, RoomDescriptor* existingRoom, const int doorSlot, const bool allowMultipleDoors, const bool allowSpecialNeighbors);

// Attempt to place a room connected to the provided existing RoomDescriptor via the given door.
// Returns a pointer to the new RoomDescriptor if successful.
RoomDescriptor* TryPlaceRoomAtDoor(RoomConfig_Room* roomConfig, RoomDescriptor* existingRoom, const int doorSlot, const uint32_t seed, const bool allowMultipleDoors, const bool allowSpecialNeighbors);

// Returns a set of GridIndexes that would be valid locations to place this room.
std::set<int> FindValidRoomPlacementLocations(RoomConfig_Room* roomConfig, const int dimension, const bool allowMultipleDoors, const bool allowSpecialNeighbors);
