#include "IsaacRepentance.h"

MOD_EXPORT bool L_RoomConfigStage_ValidateMusicID(int id, int* max) {
	return g_Manager->_musicmanager.ValidateMusicID(id, *max);
}

MOD_EXPORT void L_RoomConfigStage_SetDisplayName(RoomConfig_Stage* stage, const char* value) {
	stage->_displayName = value;
}

MOD_EXPORT void L_RoomConfigStage_SetPlayerSpot(RoomConfig_Stage* stage, const char* value) {
	stage->_playerSpot = value;
}

MOD_EXPORT void L_RoomConfigStage_SetBossSpot(RoomConfig_Stage* stage, const char* value) {
	stage->_bossSpot = value;
}

MOD_EXPORT void L_RoomConfigStage_SetSuffix(RoomConfig_Stage* stage, const char* value) {
	stage->_suffix = value;
}

MOD_EXPORT const char* L_RoomConfigStage_GetXMLName(RoomConfig_Stage* stage) {
	const std::string& filepath = stage->_rooms[0]._filepath;
	return filepath.size() >= 6 ? filepath.c_str() + 6 : "";
}

MOD_EXPORT void L_RoomConfigStage_SetXMLName(RoomConfig_Stage* stage, const char* name) {
	stage->_rooms[0]._filepath = std::string("rooms/") + name;
	stage->_rooms[1]._filepath = std::string("rooms/greed/") + name;
}

MOD_EXPORT void L_RoomConfigStage_LoadRoomSet(RoomConfig_Stage* stage, int mode) {
	if (!stage->_rooms[mode]._loaded) {
		g_Game->GetRoomConfig()->LoadStageBinary(stage->_id, mode);
	}
}
