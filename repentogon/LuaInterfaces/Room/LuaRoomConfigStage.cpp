#include "IsaacRepentance.h"
#include "../../Patches/Stages/StageManager.h"

MOD_EXPORT bool L_RoomConfigStage_ValidateMusicID(int id, int* max) {
	return g_Manager->_musicmanager.ValidateMusicID(id, *max);
}

// The custom stage support (StageManager) introduces some quirks for this end of the API.
// The RoomConfig_Stage* array is of a fixed size, so we maintain our own separate stage data under StageManager, which includes RoomConfig_Stage* objects (minus RoomSets).
// When a custom stage is loaded, it overwrites the vanilla RoomConfig_Stage attributes (name, spots, backdrop, music, suffix) so that many of those behaviours don't require patches.
// The RoomConfig_Stage objects we provide to the lua API are the ones owned by the StageManager, not the ones in the vanilla array.
// So if values are modified, we may need to "update" the values stored in the vanilla array too.
// Additionally, the StageManager does not hold the vanilla RoomSets, so we still need to refer back to the vanilla array for those.

static void CheckLoadedStage(RoomConfig_Stage* stage) {
	if (auto* managerStage = StageManager::GetStage(stage->_id)) {
		if (managerStage->IsVanilla() && !StageManager::GetCurrentOverride().IsOverridden(stage->_id)) {
			managerStage->Load();
		}
		if (managerStage->IsCustom() && StageManager::GetCurrentOverride().GetCustomStageID() == stage->_id) {
			managerStage->Load();
		}
	}
}

MOD_EXPORT void L_RoomConfigStage_SetDisplayName(RoomConfig_Stage* stage, const char* value) {
	stage->_displayName = value;
	CheckLoadedStage(stage);
}

MOD_EXPORT void L_RoomConfigStage_SetPlayerSpot(RoomConfig_Stage* stage, const char* value) {
	stage->_playerSpot = value;
	CheckLoadedStage(stage);
}

MOD_EXPORT void L_RoomConfigStage_SetBossSpot(RoomConfig_Stage* stage, const char* value) {
	stage->_bossSpot = value;
	CheckLoadedStage(stage);
}

MOD_EXPORT void L_RoomConfigStage_SetSuffix(RoomConfig_Stage* stage, const char* value) {
	stage->_suffix = value;
	CheckLoadedStage(stage);
}

MOD_EXPORT void L_RoomConfigStage_SetBackdrop(RoomConfig_Stage* stage, int value) {
	stage->_backdrop = value;
	CheckLoadedStage(stage);
}

MOD_EXPORT void L_RoomConfigStage_SetMusic(RoomConfig_Stage* stage, int value) {
	stage->_musicId = value;
	CheckLoadedStage(stage);
}

MOD_EXPORT const char* L_RoomConfigStage_GetXMLName(RoomConfig_Stage* stage) {
	if (stage->_id >= NUM_STB) return "";

	const std::string& filepath = stage->_rooms[0]._filepath;
	return filepath.size() >= 6 ? filepath.c_str() + 6 : "";
}

MOD_EXPORT void L_RoomConfigStage_SetXMLName(RoomConfig_Stage* stage, const char* name) {
	if (stage->_id >= NUM_STB) return;
	
	RoomConfig_Stage* vanillaStage = &g_Game->GetRoomConfig()->_stages[stage->_id];
	vanillaStage->_rooms[0]._filepath = std::string("rooms/") + name;
	vanillaStage->_rooms[1]._filepath = std::string("rooms/greed/") + name;
}

MOD_EXPORT RoomSet* L_RoomConfigStage_LoadVanillaRoomSet(RoomConfig_Stage* stage, int mode) {
	if (stage->_id >= NUM_STB) return nullptr;

	RoomConfig_Stage* vanillaStage = &g_Game->GetRoomConfig()->_stages[stage->_id];
	if (!vanillaStage->_rooms[mode]._loaded) {
		g_Game->GetRoomConfig()->LoadStageBinary(stage->_id, mode);
	}

	return &g_Game->GetRoomConfig()->_stages[stage->_id]._rooms[mode];
}

MOD_EXPORT RoomSet* L_RoomConfigStage_GetVanillaRoomSet(RoomConfig_Stage* stage, int mode) {
	if (stage->_id >= NUM_STB) return nullptr;

	return &g_Game->GetRoomConfig()->_stages[stage->_id]._rooms[mode];
}
