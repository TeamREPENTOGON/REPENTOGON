#include "IsaacRepentance.h"
#include "Log.h"
#include "../../VirtualRoomConfig/VirtualRoomSetManager.h"
#include "../../Patches/Stages/StageManager.h"

MOD_EXPORT RoomConfig_Room* L_RoomConfig_GetRoomByStageTypeAndVariant(uint32_t stage, uint32_t type, uint32_t variant, int mode) {
	return VirtualRoomSetManager::GetRoomByStageTypeAndVariant_IgnoreStageOverride(stage, type, variant, mode);
}

MOD_EXPORT RoomConfig_Room* L_RoomConfig_GetRandomRoom(unsigned int seed, bool reduceWeight, int stage, int type, int shape, unsigned int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, unsigned int doors, int subtype, int mode) {
	return VirtualRoomSetManager::GetRandomRoom_IgnoreStageOverride(seed, reduceWeight, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, &doors, subtype, mode);
}

MOD_EXPORT int L_RoomConfig_GetNumStages() {
	return StageManager::GetNumStages();
}

MOD_EXPORT RoomConfig_Stage* L_RoomConfig_GetStage(int stageid) {
	auto* stage = StageManager::GetStage(stageid);
	if (!stage) {
		if (stageid >= 0 && stageid < NUM_STB) {
			return &g_Game->GetRoomConfig()->_stages[stageid];
		}
		return nullptr;
	}
	return &stage->GetConfigStage();
}

MOD_EXPORT unsigned int L_RoomConfig_GetRoomSetID(uint32_t stage, int mode) {
	if (mode == -1) {
		mode = g_Game->IsGreedMode() ? 1 : 0;
	}
	return VirtualRoomSetManager::detail::GetId(VirtualRoomSetManager::GetSet(stage, mode));
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
