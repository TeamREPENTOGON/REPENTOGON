#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_LRoomTileDesc_GetRandomTile(LRoomTileDesc* desc, unsigned int seed, int out[2]) {
		XY ret;
		desc->GetRandomTile(&ret, seed);
		out[0] = ret.x;
		out[1] = ret.y;
	}
}