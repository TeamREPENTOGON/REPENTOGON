#include "RoomPlacement.h"

#include "IsaacRepentance.h"

constexpr bool DEBUG = false;

void RoomPlacementDebug(const std::string& str) {
	if (DEBUG) {
		KAGE::_LogMessage(0, (str + "\n").c_str());
		g_Game->GetConsole()->Print(str + "\n", 0xFFFFFFFF, 60);
	}
}

DoorSourceTarget GetDoorSourceTarget(const int gridIndex, const int roomShape, const int doorSlot, const bool normalizeRoomShape) {
	int normalizedRoomShape = roomShape;

	if (normalizeRoomShape) {
		// Treat thin rooms as their less-thin counterparts.
		if (roomShape == 2 || roomShape == 3) {  // IH / IV -> 1x1
			normalizedRoomShape = 1;
		}
		else if (roomShape == 5) {  // IIV -> 1x2
			normalizedRoomShape = 4;
		}
		else if (roomShape == 7) {  // IIH -> 2x1
			normalizedRoomShape = 6;
		}
	}

	XY roomPos = RoomIndexToCoords(gridIndex);
	DoorSourceTarget doorSourceTarget;

	LevelGenerator::get_door_source_position(&doorSourceTarget.source, &roomPos, normalizedRoomShape, doorSlot);
	LevelGenerator::get_door_target_position(&doorSourceTarget.target, &roomPos, normalizedRoomShape, doorSlot, false);

	return doorSourceTarget;
}

std::map<int, RoomDescriptor*> GetNeighboringRooms(const int gridIndex, const int roomShape, const int dimension) {
	std::map<int, RoomDescriptor*> neighbors;

	for (int doorSlot = 0; doorSlot < 8; doorSlot++) {
		const DoorSourceTarget doorSourceTarget = GetDoorSourceTarget(gridIndex, roomShape, doorSlot, true);
		if (doorSourceTarget.IsValid()) {
			RoomDescriptor* neighborDesc = g_Game->GetRoomByIdx(doorSourceTarget.GetTargetIdx(), dimension);
			if (neighborDesc && neighborDesc->Data) {
				neighbors[doorSlot] = neighborDesc;
			}
		}
	}

	return neighbors;
}

bool CanPlaceRoom(const int roomShape, int doorMask, const int column, const int line, const int dimension, const bool allowMultipleDoors, const bool allowSpecialNeighbors, const bool allowNoNeighbors) {
	if (column < 0 || column > 12 || line < 0 || line > 12 || roomShape < 1 || roomShape > 12) return false;

	// This function provides a pointer to a raw array of integer pairs that represent the level grid offsets
	// that the room shape occupies. For example, for a 1x2 room, it would be (0,0) and (0,1). The list ends with (-1, -1).
	int* offsetsArray = *LevelGenerator::get_room_placement_offsets(roomShape);

	for (int i = 0; i < 4; i++) {
		const int x = offsetsArray[i * 2];
		const int y = offsetsArray[i * 2 + 1];
		if (x == -1 || y == -1) {
			break;
		}
		if (x < 0 || y < 0 || x > 1 || y > 1) {
			RoomPlacementDebug("Found an unexpected room placement offset value! Aborting. (scary)");
			return false;
		}
		const int newCol = column + x;
		const int newLine = line + y;
		if (newCol > 12 || newLine > 12) {
			RoomPlacementDebug("Room would go out of bounds!");
			return false;
		}
		const int offsetIdx = RoomCoordsToIndex(newCol, newLine);
		RoomDescriptor* roomDesc = g_Game->GetRoomByIdx(offsetIdx, dimension);
		if (roomDesc && roomDesc->Data) {
			RoomPlacementDebug("Room would overlap with an existing room!");
			return false;
		}
	}

	const int gridIndex = RoomCoordsToIndex(column, line);
	const std::map<int, RoomDescriptor*> neighbors = GetNeighboringRooms(gridIndex, roomShape, dimension);

	if (!allowNoNeighbors && neighbors.empty()) {
		RoomPlacementDebug("Placement with no neighbors not allowed!");
		return false;
	}

	if (doorMask < 0) {
		// Assume all DoorSlots for this RoomShape are available.
		doorMask = 0;
		for (int doorSlot = 0; doorSlot < 8; doorSlot++) {
			if (LevelGenerator::has_shape_slot(roomShape, doorSlot, false)) {
				doorMask = doorMask | DoorSlotToMask(doorSlot);
			}
		}
	}

	int doors = 0;
	bool hasSecretNeighbor = false;
	bool hasNonSecretNeighbor = false;

	for (const auto& [doorSlot, neighborDesc] : neighbors) {
		if ((doorMask & DoorSlotToMask(doorSlot)) == 0) {
			RoomPlacementDebug("Cannot connect to neighbor: Missing DoorSlot!");
			return false;
		}

		if (neighborDesc->Data->Type == 5) {
			RoomPlacementDebug("Can't connect to a boss room!");
			return false;
		}

		if (!allowSpecialNeighbors && neighborDesc->Data->Type > 1 && neighborDesc->Data->Type != 7) {
			RoomPlacementDebug("Placement would violate special room rules!");
			return false;
		}

		const DoorSourceTarget doorSourceTarget = GetDoorSourceTarget(gridIndex, roomShape, doorSlot, true);
		bool canConnect = false;

		for (const int returnDoorSlot : GetPossibleReturnDoorSlots(doorSlot)) {
			if ((neighborDesc->Data->Doors & DoorSlotToMask(returnDoorSlot)) != 0) {
				const DoorSourceTarget returnDoorSourceTarget = GetDoorSourceTarget(neighborDesc->GridIndex, neighborDesc->Data->Shape, returnDoorSlot, true);
				if (doorSourceTarget.ConnectsTo(returnDoorSourceTarget)) {
					// Don't allow use of DoorSlot.UP0 in the starting room of Chest/Dark Room, or if the Depths 2 photo door is present.
					const bool forbiddenSlot = returnDoorSlot == 1 && neighborDesc->GridIndex == g_Game->_startingRoomIdx && (g_Game->LevelHasPhotoDoor() || g_Game->_stage == 11);
					if (!forbiddenSlot) {
						canConnect = true;
					}
					break;
				}
			}
		}

		if (!canConnect) {
			RoomPlacementDebug("Neighbor does not have an appropriate DoorSlot available!");
			return false;
		}

		if (neighborDesc->Data->Type == 7 || neighborDesc->Data->Type == 8) {
			// Don't count secret rooms as a door.
			hasSecretNeighbor = true;
		} else {
			hasNonSecretNeighbor = true;
			doors++;
		}
	}

	if (!allowMultipleDoors && doors > 1) {
		RoomPlacementDebug("Placement with multiple doors not allowed!");
		return false;
	}

	if (hasSecretNeighbor && !hasNonSecretNeighbor && !allowSpecialNeighbors) {
		RoomPlacementDebug("Placement with a secret room as the only neighbor is not allowed if allowSpecialNeighbors=false!");
		return false;
	}

	return true;
}

bool CanPlaceRoom(RoomConfig_Room* roomConfig, const int column, const int line, const int dimension, const bool allowMultipleDoors, const bool allowSpecialNeighbors, const bool allowNoNeighbors) {
	return roomConfig && CanPlaceRoom(roomConfig->Shape, roomConfig->Doors, column, line, dimension, allowMultipleDoors, allowSpecialNeighbors, allowNoNeighbors);
}

RoomDescriptor* TryPlaceRoom(RoomConfig_Room* roomConfig, const int column, const int line, const int dimension, const uint32_t seed, const bool allowMultipleDoors, const bool allowSpecialNeighbors, const bool allowNoNeighbors) {
	if (!CanPlaceRoom(roomConfig, column, line, dimension, allowMultipleDoors, allowSpecialNeighbors, allowNoNeighbors)) {
		RoomPlacementDebug("Failed to place room: Doesn't fit!");
		return nullptr;
	}

	LevelGenerator_Room levelgenroom;
	levelgenroom.constructor(column, line, roomConfig->Shape);

	// No, the last argument for PlaceRoom is not the dimension. Or at least it does not appear to work.
	// So we cheat!
	const int currentDimension = g_Game->_currentDimensionIdx;
	if (dimension > -1) {
		g_Game->_currentDimensionIdx = dimension;
	}
	const bool placeRoomSuccess = g_Game->PlaceRoom(&levelgenroom, roomConfig, seed, 0);
	if (dimension > -1) {
		g_Game->_currentDimensionIdx = currentDimension;
	}

	if (!placeRoomSuccess) {
		RoomPlacementDebug("Failed to place room: Call to PlaceRoom failed!");
		return nullptr;
	}

	RoomDescriptor* roomDesc = &g_Game->_gridRooms[g_Game->_nbRooms - 1];
	if (!roomDesc->Data) {
		RoomPlacementDebug("Failed to place room: I DON'T KNOW WHY THOUGH (scary)");
		return nullptr;
	}

	// We'll populate this with only the doors that are actually present.
	roomDesc->AllowedDoors = 0;

	Room* currentRoom = g_Game->GetCurrentRoom();

	const std::map<int, RoomDescriptor*> neighbors = GetNeighboringRooms(roomDesc->GridIndex, roomDesc->Data->Shape, dimension);

	for (const auto& [doorSlot, neighborDesc] : neighbors) {
		const int doorSlotFlag = DoorSlotToMask(doorSlot);
		const DoorSourceTarget doorSourceTarget = GetDoorSourceTarget(roomDesc->GridIndex, roomDesc->Data->Shape, doorSlot, false);

		if ((roomDesc->Data->Doors & doorSlotFlag) == 0 || !doorSourceTarget.IsValid()) {
			RoomPlacementDebug("Something weird happened, placed a room but it doesn't seem like it should have been allowed! (scary)");
			continue;
		}

		for (const int returnDoorSlot : GetPossibleReturnDoorSlots(doorSlot)) {
			const DoorSourceTarget returnSourceTarget = GetDoorSourceTarget(neighborDesc->GridIndex, neighborDesc->Data->Shape, returnDoorSlot, false);

			if (returnSourceTarget.ConnectsTo(doorSourceTarget)) {
				roomDesc->Doors[doorSlot] = doorSourceTarget.GetTargetIdx();
				roomDesc->AllowedDoors = roomDesc->AllowedDoors | doorSlotFlag;

				neighborDesc->Doors[returnDoorSlot] = returnSourceTarget.GetTargetIdx();
				neighborDesc->AllowedDoors = neighborDesc->AllowedDoors | DoorSlotToMask(returnDoorSlot);

				if (neighborDesc == g_Game->GetCurrentRoomDesc() && !currentRoom->GetDoor(returnDoorSlot)) {
					// The room is adjacent to the player's current room, so we need to spawn the door.
					currentRoom->MakeDoor(returnDoorSlot);
					GridEntity_Door* door = currentRoom->GetDoor(returnDoorSlot);
					if (door) {
						door->_targetRoomIdx = roomDesc->SafeGridIndex;
						door->SetRoomTypes(neighborDesc->Data->Type, roomDesc->Data->Type);
						if (currentRoom->GetFrameCount() > 0) {
							door->play_animation();
						}
						// Remove red key outlines if any are present.
						Entity_Effect* outline = currentRoom->_redDoorOutlines[returnDoorSlot];
						if (outline) {
							outline->Remove();
							currentRoom->_redDoorOutlines[returnDoorSlot] = nullptr;
						}
					} else {
						RoomPlacementDebug("Failed to spawn connecting door in player's current room. Weird!!");
					}
				}
			}
		}
	}

	// Update the visibility of the new room appropriately.
	const int roomType = roomDesc->Data->Type;
	const bool special = (roomType > 1);
	const bool secret = (roomType == 7);
	const bool superSecret = (roomType == 8);
	const bool ultraSecret = (roomType == 29);

	const bool compass = g_Game->GetLevelStateFlag(7);  // LevelStateFlag.STATE_COMPASS_EFFECT
	const bool map = g_Game->GetLevelStateFlag(8);  // LevelStateFlag.STATE_MAP_EFFECT
	const bool blueMap = g_Game->GetLevelStateFlag(9);  // LevelStateFlag.STATE_BLUE_MAP_EFFECT
	const bool fullMap = g_Game->GetLevelStateFlag(10);  // LevelStateFlag.STATE_FULL_MAP_EFFECT

	if (fullMap || (blueMap && (secret || superSecret)) || (compass && special && !secret && !superSecret && !ultraSecret)) {
		roomDesc->DisplayFlags = roomDesc->DisplayFlags | 5;  // Make room + icon visible.
	} else if (map) {
		roomDesc->DisplayFlags = roomDesc->DisplayFlags | 1;  // Make room visible.
	}

	g_Game->UpdateVisibility();

	return roomDesc;
}

std::vector<XY> FindPlacementsForRoomAtDoor(const int roomShape, RoomDescriptor* existingRoom, const int doorSlot) {
	std::vector<XY> out;
	if (existingRoom && existingRoom->Data && (existingRoom->Data->Doors & DoorSlotToMask(doorSlot)) != 0) {
		const DoorSourceTarget doorSourceTarget = GetDoorSourceTarget(existingRoom->GridIndex, existingRoom->Data->Shape, doorSlot, false);
		if (doorSourceTarget.IsValid()) {
			for (const int returnDoorSlot : GetPossibleReturnDoorSlots(doorSlot)) {
				const DoorSourceTarget returnSourceTarget = GetDoorSourceTarget(doorSourceTarget.GetTargetIdx(), roomShape, returnDoorSlot, false);
				if (returnSourceTarget.IsValid()) {
					const XY offset = doorSourceTarget.target - returnSourceTarget.source;
					const XY placementPos = doorSourceTarget.target + offset;
					out.push_back(placementPos);
				}
			}
		}
	}
	return out;
}

bool CanPlaceRoomAtDoor(const int roomShape, const int doorMask, RoomDescriptor* existingRoom, const int doorSlot, const bool allowMultipleDoors, const bool allowSpecialNeighbors) {
	const std::vector<XY> possiblePlacements = FindPlacementsForRoomAtDoor(roomShape, existingRoom, doorSlot);
	for (const XY& coords : possiblePlacements) {
		if (CanPlaceRoom(roomShape, doorMask, coords.x, coords.y, existingRoom->Dimension, allowMultipleDoors, allowSpecialNeighbors, /*allowNoNeighbors=*/false)) {
			return true;
		}
	}
	return false;
}

bool CanPlaceRoomAtDoor(RoomConfig_Room* roomConfig, RoomDescriptor* existingRoom, const int doorSlot, const bool allowMultipleDoors, const bool allowSpecialNeighbors) {
	return roomConfig && CanPlaceRoomAtDoor(roomConfig->Shape, roomConfig->Doors, existingRoom, doorSlot, allowMultipleDoors, allowSpecialNeighbors);
}

RoomDescriptor* TryPlaceRoomAtDoor(RoomConfig_Room* roomConfig, RoomDescriptor* existingRoom, const int doorSlot, const uint32_t seed, const bool allowMultipleDoors, const bool allowSpecialNeighbors) {
	if (!roomConfig) return nullptr;

	std::vector<XY> placements = FindPlacementsForRoomAtDoor(roomConfig->Shape, existingRoom, doorSlot);
	if (seed % 2 == 0 && placements.size() > 1) {
		std::swap(placements[0], placements[1]);
	}
	for (const XY& placementPos : placements) {
		RoomDescriptor* newRoom = TryPlaceRoom(roomConfig, placementPos.x, placementPos.y, existingRoom->Dimension, seed, allowMultipleDoors, allowSpecialNeighbors, /*allowNoNeighbors=*/false);
		if (newRoom) {
			return newRoom;
		}
	}

	return nullptr;
}

std::set<int> FindValidRoomPlacementLocations(const int roomShape, const int doorMask, const int dimension, const bool allowMultipleDoors, const bool allowSpecialNeighbors) {
	const int trueDimension = dimension > -1 ? dimension : g_Game->GetDimension();

	std::set<int> out;

	for (uint32_t i = 0; i < g_Game->_nbRooms; i++) {
		RoomDescriptor* roomDesc = &g_Game->_gridRooms[i];
		if (roomDesc && roomDesc->Data && roomDesc->Dimension == trueDimension) {
			for (int doorSlot = 0; doorSlot < 8; doorSlot++) {
				const std::vector<XY> possiblePlacements = FindPlacementsForRoomAtDoor(roomShape, roomDesc, doorSlot);
				for (const XY& coords : possiblePlacements) {
					if (CanPlaceRoom(roomShape, doorMask, coords.x, coords.y, dimension, allowMultipleDoors, allowSpecialNeighbors, false)) {
						out.insert(RoomCoordsToIndex(coords.x, coords.y));
					}
				}
			}
		}
	}

	return out;
}

std::set<int> FindValidRoomPlacementLocations(RoomConfig_Room* roomConfig, const int dimension, const bool allowMultipleDoors, const bool allowSpecialNeighbors) {
	if (!roomConfig) {
		return std::set<int>();
	}
	return FindValidRoomPlacementLocations(roomConfig->Shape, roomConfig->Doors, dimension, allowMultipleDoors, allowSpecialNeighbors);
}
