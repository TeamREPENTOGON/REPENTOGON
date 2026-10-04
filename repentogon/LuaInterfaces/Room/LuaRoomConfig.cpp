#include "IsaacRepentance.h"
#include "Log.h"
#include "../../VirtualRoomConfig/VirtualRoomSetManager.h"

MOD_EXPORT RoomConfig_Room* L_RoomConfig_GetRoomByStageTypeAndVariant(uint32_t stage, uint32_t type, uint32_t variant, int mode) {
	return g_Game->GetRoomConfig()->GetRoomByStageTypeAndVariant(stage, type, variant, mode);
}

MOD_EXPORT RoomConfig_Room* L_RoomConfig_GetRandomRoom(unsigned int seed, bool reduceWeight, int stage, int type, int shape, unsigned int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, unsigned int doors, int subtype, int mode) {
	return g_Game->GetRoomConfig()->GetRandomRoom(seed, reduceWeight, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, &doors, subtype, mode);
}

MOD_EXPORT RoomConfig_Stage* L_RoomConfig_GetStage(int stage) {
	return &g_Game->GetRoomConfig()->_stages[stage];
}

MOD_EXPORT unsigned int L_RoomConfig_GetVanillaSetID(uint32_t stage, int mode) {
	if (mode == -1) {
		mode = g_Game->IsGreedMode() ? 1 : 0;
	}
	return VirtualRoomSetManager::detail::GetId(VirtualRoomSetManager::GetVanillaSet(stage, mode));
}

MOD_EXPORT bool L_RoomConfig_HasShapeSlot(int shape, unsigned int slot) {
	return LevelGenerator::has_shape_slot(shape, slot, false);
}

MOD_EXPORT int L_RoomConfig_GetDoorFromPosition(int16_t x, int16_t y, int shape) {
	return RoomConfig::get_door_from_position(x, y, shape);
}

MOD_EXPORT void L_RoomConfig_Log(const char* message) {
	ZHL::Log("%s", message);
}
