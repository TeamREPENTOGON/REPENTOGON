#include "IsaacRepentance.h"
#include "Log.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"

#include "RoomPlacement.h"

extern "C" {
	__declspec(dllexport) void L_RoomDescriptor_AddRestrictedGridIndex(RoomDescriptor* descriptor, int gridIdx) {
		std::set<int>& ents = descriptor->RestrictedGridIndexes;
		ents.insert(gridIdx);
	}

	__declspec(dllexport) int L_RoomDescriptor_GetErrorTrinketEffect(RoomDescriptor* descriptor) {
		return descriptor->GetErrorTrinketEffect();
	}

	__declspec(dllexport) int L_RoomDescriptor_GetNeighboringRooms(RoomDescriptor* roomDesc, int* outSlots, RoomDescriptor** outRooms) {
		const std::map<int, RoomDescriptor*> neighbors =
			GetNeighboringRooms(roomDesc->GridIndex,
				roomDesc->Data->Shape,
				roomDesc->Dimension);

		int count = 0;
		for (const auto& [doorSlot, neighborDesc] : neighbors) {
			outSlots[count] = doorSlot;
			outRooms[count] = neighborDesc;
			++count;
		}
		return count;
	}

	__declspec(dllexport) int L_RoomDescriptor_GetRestrictedGridIndexesCount(RoomDescriptor* descriptor) {
		return static_cast<int>(descriptor->RestrictedGridIndexes.size());
	}

	__declspec(dllexport) int L_RoomDescriptor_GetRestrictedGridIndexes(RoomDescriptor* descriptor, int* outIndexes, int maxCount) {
		int count = 0;
		for (int gridIdx : descriptor->RestrictedGridIndexes) {
			if (count >= maxCount) break;
			outIndexes[count++] = gridIdx;
		}
		return count;
	}


	__declspec(dllexport) int L_RoomDescriptor_GetValidNeighborPlacementLocationsCount(RoomDescriptor* descriptor, int roomShape, int doorMask, bool allowMultipleDoors, bool allowSpecialNeighbors) {
		return FindValidNeighborPlacementLocations(descriptor, roomShape, doorMask, allowMultipleDoors, allowSpecialNeighbors).size();
	}

	__declspec(dllexport) int L_RoomDescriptor_GetValidNeighborPlacementLocations(RoomDescriptor* descriptor, int roomShape, int doorMask, bool allowMultipleDoors, bool allowSpecialNeighbors, int* outLocations, int maxCount)
	{
		const std::set<int> validLocations = FindValidNeighborPlacementLocations(descriptor, roomShape, doorMask, allowMultipleDoors, allowSpecialNeighbors);

		int count = 0;
		for (const int gridIndex : validLocations) {
			if (count >= maxCount) break;
			outLocations[count++] = gridIndex;
		}
		return count;
	}
};

MOD_EXPORT void* L_RoomDescriptor_GetDecoSaveState(RoomDescriptor* descriptor) {
	return &descriptor->SavedEffects;
}

MOD_EXPORT void* L_RoomDescriptor_GetEntitiesSaveState(RoomDescriptor* descriptor) {
	return &descriptor->SavedEntities;
}

MOD_EXPORT void* L_RoomDescriptor_GetGridEntitiesSaveState(RoomDescriptor* descriptor) {
	return &descriptor->SavedGridEntities;
}

MOD_EXPORT void L_RoomDescriptor_InitSeeds(RoomDescriptor* descriptor, RNG* rng) {
	descriptor->InitSeeds(rng);
}
