#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) const char* L_RoomConfigRoom_GetName(RoomConfig_Room* room) {
		return room->Name.c_str();
	}
}