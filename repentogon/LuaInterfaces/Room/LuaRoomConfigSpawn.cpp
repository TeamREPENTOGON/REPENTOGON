#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) RoomEntry* L_RoomConfigSpawn_PickEntry(RoomSpawn* spawn, float r) {
		return spawn->PickEntry(r);
	}
}
