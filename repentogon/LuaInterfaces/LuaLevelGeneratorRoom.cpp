#include "IsaacRepentance.h"

MOD_EXPORT unsigned int L_LevelGeneratorRoom_GetNeighborCount(LevelGenerator_Room* room) {
	return room->_neighbors.size();
}

MOD_EXPORT void L_LevelGeneratorRoom_GetNeighbors(LevelGenerator_Room* room, int* out) {
	for (int idx : room->_neighbors) {
		*out++ = idx;
	}
}
