#include "StageManager.h"

#include <unordered_map>

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../../LuaInterfaces/CustomCallbacks.h"

namespace StageManager {

namespace {

static std::vector<StageConfig*> s_Stages;
static StageOverride s_StageOverrides[4]{};

}  // namespace

size_t GetNumStages() {
	size_t size = s_Stages.size();
	// If called when the vector hasn't been initialized yet.
	if (size < NUM_STB) {
		return NUM_STB;
	}
	return s_Stages.size();
}

StageConfig* GetStage(uint32_t id) {
	if (id < 0 || id >= s_Stages.size()) {
		return nullptr;
	}
	return s_Stages[id];
}

StageConfig* GetStageByName(std::string name) {
	for (StageConfig* stage : s_Stages) {
		if (stage && stage->GetName() == name) {
			return stage;
		}
	}
	return nullptr;
}

void StageConfig::PostLoadStages() {
	for (const auto& [id, _] : XMLStuff.StageData->nodes) {
		if (id >= s_Stages.size()) {
			s_Stages.resize(id + 1);
		}
		s_Stages[id] = new StageConfig(id);
	}

	for (int stageType = STAGETYPE_ORIGINAL; stageType <= STAGETYPE_REPENTANCE_B; stageType++) {
		if (stageType == STAGETYPE_GREEDMODE) continue;
		for (int levelStage = STAGE1_1; levelStage < NUM_STAGES; levelStage++) {
			if (levelStage >= STAGE4_3 && stageType > STAGETYPE_WOTL) break;
			uint32_t stageid = g_Game->GetRoomConfig()->GetStageID(levelStage, stageType, 0);
			if (StageConfig* stage = GetStage(stageid)) {
				if (stage->_levelStage == -1) {
					stage->_levelStage = levelStage;
				}
				if (stage->_stageType == -1) {
					stage->_stageType = stageType;
				}
			}
		}
		for (int levelStage = STAGE1_GREED; levelStage <= STAGE7_GREED; levelStage++) {
			if (levelStage >= STAGE6_GREED && stageType > STAGETYPE_ORIGINAL) break;
			uint32_t stageid = g_Game->GetRoomConfig()->GetStageID(levelStage, stageType, 1);
			if (StageConfig* stage = GetStage(stageid)) {
				if (stage->_greedLevelStage == -1) {
					stage->_greedLevelStage = levelStage;
				}
			}
		}
	}

	for (StageConfig* stage : s_Stages) {
		if (!stage) continue;
		if (stage->IsCustom()) {
			if (StageConfig* baseStage = GetStage(stage->GetBaseStageId())) {
				stage->_stageType = baseStage->_stageType;
				stage->_levelStage = baseStage->_levelStage;
				stage->_greedLevelStage = baseStage->_greedLevelStage;
			}
		}
		if (stage->_stageType == -1)
			stage->_stageType = 0;
		if (stage->_levelStage == -1)
			stage->_levelStage = 0;
		if (stage->_greedLevelStage == -1)
			stage->_greedLevelStage = 0;
	}
}

std::vector<StageConfig*> StageManager::GetStagesByLevel(int levelStage, bool greedMode) {
	std::vector<StageConfig*> stages;
	for (StageConfig* stage : s_Stages) {
		if (!stage) continue;
		if (stage->GetLevelStage(false, greedMode) == levelStage || stage->GetLevelStage(true, greedMode) == levelStage) {
			stages.push_back(stage);
		}
	}
	return stages;
}

std::vector<StageConfig*> GetEligibleAltStages(int levelStage, int stageType) {
	bool greedMode = g_Game->IsGreedMode();
	std::vector<StageConfig*> stages;
	for (StageConfig* stage : s_Stages) {
		if (!stage) continue;

		int achievement = stage->GetAchievement();
		if (achievement > 0 && !g_Manager->GetPersistentGameData()->Unlocked(achievement)) {
			continue;
		}

		// Repentance (alt path) StageTypes cannot mix with non-Repentance StageTypes.
		bool altPath = stageType == STAGETYPE_REPENTANCE || stageType == STAGETYPE_REPENTANCE_B;
		if (altPath != stage->IsAltPath()) {
			continue;
		}

		// StageType must match exactly after Womb (no mixing Cathedral alts with Sheol alts).
		if (levelStage >= STAGE4_3 && stage->GetStageType() != stageType) {
			continue;
		}

		// Check if this LevelStage matches this stage (as either I or II)
		if (stage->GetLevelStage(false, greedMode) == levelStage || stage->GetLevelStage(true, greedMode) == levelStage) {
			stages.push_back(stage);
		}
	}
	return stages;
}

StageOverride& GetOverride(int slot) {
	if (slot < 1 || slot > 3) {
		slot = 0;
	}
	return s_StageOverrides[slot];
}

StageOverride& GetCurrentOverride() {
	return GetOverride(g_Manager->_currentSaveSlot);
}

StageConfig* GetCurrentCustomStage() {
	if (Isaac::IsInGame() && GetCurrentOverride().HasOverride()) {
		return GetStage(GetCurrentOverride().GetCustomStageID());
	}
	return nullptr;
}

bool DisableVanillaQuests() {
	if (auto* stage = GetCurrentCustomStage()) {
		return stage->DisableQuests();
	}
	return false;
}

static bool s_SettingNextStage = false;
static uint32_t s_SettingCustomStage = 0;

HOOK_METHOD(Level, SetNextStage, () -> void) {
	s_SettingNextStage = true;
	super();
	s_SettingNextStage = false;
}

void StageConfig::GotoStage(bool second) const {
	int levelStage = GetLevelStage(second, g_Game->IsGreedMode());
	int stageType = GetStageType();
	if (IsCustom()) {
		s_SettingCustomStage = GetStageId();
	}
	g_Game->GetLevel()->SetStage(levelStage, stageType);
	s_SettingCustomStage = 0;
	g_Game->GetLevel()->Init(true);
	g_Game->GetLevel()->Update();
	g_Game->GetPlayerManager()->TriggerNewRoom_TemporaryEffects();
}

// TODO: Consider moving validations to lualand
void ProcessLevelSelectCallbackResult(lua_State* L, lua::LuaResults& results, int& levelStageOut, int& stageTypeOut, uint32_t& customStageOut) {
	if (!results && lua_istable(L, -1)) {
		lua_getfield(L, -1, "StageID");
		uint32_t stageID = (uint32_t)lua_tointeger(L, -1);
		lua_pop(L, 1);

		if (stageID > 0) {
			// New StageID+Floor# format, supports custom stages.

			lua_getfield(L, -1, "Floor");
			int floor = (int)lua_tointeger(L, -1);
			lua_pop(L, 1);

			if (StageConfig* stage = GetStage(stageID)) {
				levelStageOut = stage->GetLevelStage(floor >= 2);
				stageTypeOut = stage->GetStageType();
				if (stage->IsCustom()) {
					customStageOut = stageID;
				} else {
					customStageOut = 0;
				}
				return;
			}
			g_Game->GetConsole()->PrintError(REPENTOGON::StringFormat("MC_PRE_LEVEL_SELECT: Invalid return value, %u is not a valid Stage ID!\n", stageID));
			return;
		}
		
		// Original (vanilla only) LevelStage+StageType format.

		lua_len(L, -1);
		int len = (int)lua_tointeger(L, -1);
		lua_pop(L, 1);

		if (len != 2) {
			g_Game->GetConsole()->PrintError(REPENTOGON::StringFormat("MC_PRE_LEVEL_SELECT: Invalid return value, table contains %d elements, expected 2\n", len));
			return;
		}

		lua_rawgeti(L, -1, 1);
		int levelStage = (int)lua_tointeger(L, -1);
		lua_pop(L, 1);

		if (g_Game->IsGreedMode()) {
			if (levelStage < 1 || levelStage > 7) {
				g_Game->GetConsole()->PrintError(REPENTOGON::StringFormat("MC_PRE_LEVEL_SELECT: Invalid level stage, received %d, should be in range [1; 7] (detected greed(ier) mode)\n", levelStage));
				return;
			}
		} else {
			if (levelStage < 1 || levelStage > 14) {
				g_Game->GetConsole()->PrintError(REPENTOGON::StringFormat("MC_PRE_LEVEL_SELECT: Invalid level stage, received %d, should be in range [1; 14] (detected normal/hard mode)\n", levelStage));
				return;
			}
		}

		lua_rawgeti(L, -1, 2);
		int stageType = (int)lua_tointeger(L, -1);
		lua_pop(L, 1);

		if (stageType < 0 || stageType == 3 || stageType > 5) {
			g_Game->GetConsole()->PrintError(REPENTOGON::StringFormat("MC_PRE_LEVEL_SELECT: Invalid stage type, received value %d, expected 0, 1, 2, 4 or 5\n", stageType));
			return;
		}

		// Passed validations
		levelStageOut = levelStage;
		stageTypeOut = stageType;
	}
}

HOOK_METHOD(Level, SetStage, (int levelStage, int stageType)-> void) {
	uint32_t vanillaStageId = g_Game->GetRoomConfig()->GetStageID(levelStage, stageType, g_Game->GetMode());
	uint32_t customStageId = s_SettingCustomStage;
	s_SettingCustomStage = 0;

	bool isNatural = false;

	if (s_SettingNextStage) {
		s_SettingNextStage = false;
		isNatural = true;

		// This selection logic might not be perfect but should be good enough for now.
		// The game has called SetNextStage and made a decision (for example, STAGE3_2+STAGETYPE_WOTL for Necropolis II)
		// Here we gather all the current eligible "alts" (Depths, Necropolis, Dank Depths, and all custom stages based on them) and randomly pick one.
		// If we pick a custom stage, override the current stage with that one. If we pick a vanilla one, just use the original selection.
		// We may want to reimplement SetNextStage to enable some greater customization (like a callback).

		WeightedOutcomePicker picker;
		KAGE::_LogMessage(0, "[StageManager] Selecting next stage...\n");
		for (const StageConfig* stage : GetEligibleAltStages(levelStage, stageType)) {
			KAGE::_LogMessage(0, "[StageManager] ... added %s\n", stage->GetName().c_str());
			picker.AddOutcomeWeight(WeightedOutcomePicker_Outcome{ (uint32_t)stage->GetStageId(), 1 }, false);
		}

		uint32_t seed = g_Game->_seedEffects._stageSeeds[levelStage];
		RNG rng;
		rng.SetSeed(seed, 35);
		uint32_t choice = picker.PickOutcome(rng);

		if (const StageConfig* chosenStage = GetStage(choice)) {
			KAGE::_LogMessage(0, "[StageManager] ...... chose %s\n", chosenStage->GetName().c_str());
			// Ignore vanilla stages that get picked, since whatever vanilla picked is OK at that point.
			if (chosenStage->IsCustom()) {
				KAGE::_LogMessage(0, "[StageManager] Selected custom stage. Loading it...\n");
				stageType = chosenStage->GetStageType();
				customStageId = chosenStage->GetStageId();
			} else {
				KAGE::_LogMessage(0, "[StageManager] Selected vanilla stage. Ignoring and going with vanilla SetNextStage's choice instead.\n");
			}
		}
	}

	// TODO: Finalize updates to MC_PRE_LEVEL_SELECT (ie, new return format to support custom stages), decide whether to introduce a separate callback for "natural" selections.

	// MC_PRE_LEVEL_SELECT (1104)
	const int callbackId = 1104;
	if (CallbackState.test(callbackId - 1000)) {
		int stageid = customStageId > 0 ? customStageId : vanillaStageId;
		int floor = 1;
		if (StageConfig* stage = GetStage(stageid)) {
			floor = stage->GetFloor(levelStage);
		}

		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.push(levelStage)
			.push(stageType)
			.push(stageid)
			.push(floor)
			.push(isNatural)
			.call(1);

		ProcessLevelSelectCallbackResult(L, results, levelStage, stageType, customStageId);
	}

	if (customStageId > 0) {
		GetCurrentOverride().Load(customStageId);
	} else {
		GetCurrentOverride().Flush();
	}

	super(levelStage, stageType);

	if (customStageId > 0) {
		int progressIdx = levelStage + (stageType == STAGETYPE_REPENTANCE || stageType == STAGETYPE_REPENTANCE_B);
		g_Manager->_nightmareScene._progressBar_stageFrame[progressIdx] = customStageId;
	}
}

HOOK_METHOD_PRIORITY(RoomConfig, LoadStageBinary, -1, (uint32_t stage, uint32_t mode) -> void) {
	super(stage, mode);
	if (GetCurrentOverride().HasOverride()) {
		s_Stages[GetCurrentOverride().GetCustomStageID()]->Load();
	}
}

HOOK_METHOD(Manager, SetSaveSlot, (uint32_t slot)->void) {
	if (GetCurrentOverride().HasOverride()) {
		s_Stages[GetCurrentOverride().GetVanillaStageID()]->Load();
	}
	super(slot);
	if (GetCurrentOverride().HasOverride()) {
		s_Stages[GetCurrentOverride().GetCustomStageID()]->Load();
	}
}

}  // namespace StageManager
