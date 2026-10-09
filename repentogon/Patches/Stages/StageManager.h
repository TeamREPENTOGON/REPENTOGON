#pragma once

#include <string>
#include <unordered_map>

#include "IsaacRepentance.h"
#include "../XMLData.h"
#include "../../VirtualRoomConfig/VirtualRoomSetManager.h"

namespace StageManager {

static const std::unordered_map<int, int> s_VanillaStageAchievements = {
	{ STB_CELLAR, 86 },
	{ STB_CATACOMBS, 87 },
	{ STB_NECROPOLIS, 88 },
	{ STB_BURNING_BASEMENT, 342 },
	{ STB_FLOODED_CAVES, 343 },
	{ STB_DANK_DEPTHS, 344 },
	{ STB_SCARRED_WOMB, 345 },
	{ STB_DROSS, 412 },
	{ STB_ASHPIT, 413 },
	{ STB_GEHENNA, 414 },
};

class StageConfig {
public:
	static void PostLoadStages();

	StageConfig() = delete;

	StageConfig(uint32_t stageId) {
		_stageId = stageId;
		XMLAttributes xmlData = XMLStuff.StageData->GetNodeById(stageId);
		XMLAttributes baseXmlData = xmlData;

		if (stageId >= NUM_STB) {
			// Custom Stage
			int baseStage = std::atoi(xmlData["basestage"].c_str());
			if (baseStage <= STB_SPECIAL_ROOMS || (baseStage >= STB_UNUSED1 && baseStage <= STB_ULTRA_GREED) || baseStage >= NUM_STB) {
				g_Game->GetConsole()->PrintError("stages.xml ERROR: Invalid basestage " + std::to_string(baseStage) + " defined for custom stage: " + xmlData["name"]);
				baseStage = 1;
				_hidden = true;
			} else {
				_hidden = xmlData["hidden"] == "true";
			}
			baseXmlData = XMLStuff.StageData->GetNodeById(baseStage);
			_baseStageId = baseStage;

			_achievement = std::atoi(xmlData["achievement"].c_str());
		} else {
			// Vanilla Stage
			_baseStageId = _stageId;
			_hidden = stageId == STB_MORTIS;

			if (s_VanillaStageAchievements.count(stageId)) {
				_achievement = s_VanillaStageAchievements.at(stageId);
			} else {
				_achievement = 0;
			}
		}

		std::string root = xmlData["root"];
		if (root.empty()) {
			root = "rooms/";
		}

		std::string greedRoot = xmlData["greedroot"];
		if (greedRoot.empty()) {
			greedRoot = "rooms/greed/";
		}

		_binary = root + xmlData["path"];
		_greedBinary = greedRoot + xmlData["path"];

		std::string bossGfxRoot = xmlData["bossgfxroot"];
		if (bossGfxRoot.empty()) {
			bossGfxRoot = "gfx/ui/boss/";
		}

		std::string playerSpot = xmlData["playerspot"];
		if (playerSpot.empty()) {
			playerSpot = baseXmlData["playerspot"];
		}
		_playerSpot = bossGfxRoot + playerSpot;

		std::string bossSpot = xmlData["bossspot"];
		if (bossSpot.empty()) {
			bossSpot = baseXmlData["bossspot"];
		}
		_bossSpot = bossGfxRoot + bossSpot;
		
		_displayName = xmlData["untranslatedname"];
		if (_displayName.empty()) {
			_displayName = xmlData["name"];
		}

		_suffix = xmlData["suffix"];
		if (!_suffix.empty() && _suffix.front() != '_') {
			_suffix = '_' + _suffix;
		}
		_musicId = std::atoi(xmlData["music"].c_str());
		_backdrop = std::atoi(xmlData["backdrop"].c_str());

		if (stageId >= NUM_STB) {
			VirtualRoomSetManager::TryInitializeSet(stageId);
			if (!_binary.empty()) {
				VirtualRoomSetManager::detail::AddStbRooms(stageId, 0, _binary);
			}
			if (!_greedBinary.empty()) {
				VirtualRoomSetManager::detail::AddStbRooms(stageId, 1, _greedBinary);
			}
		}
	}

	void GotoStage(bool second) const;

	inline void Load() const {
		RoomConfig_Stage& stage = g_Game->GetRoomConfig()->_stages[_baseStageId];
		stage._displayName = _displayName;
		stage._playerSpot = _playerSpot;
		stage._bossSpot = _bossSpot;
		stage._suffix = _suffix;
		stage._musicId = _musicId;
		stage._backdrop = _backdrop;
	}

	inline int GetStageId() const {
		return _stageId;
	}

	inline int GetBaseStageId() const {
		return _baseStageId;
	}

	inline const std::string& GetName() const {
		return _displayName;
	}

	inline bool IsVanilla() const {
		return _stageId < NUM_STB;
	}

	inline bool IsCustom() const {
		return !IsVanilla();
	}

	inline bool IsHidden() const {
		return _hidden;
	}

	inline int GetAchievement() const {
		return _achievement;
	}

	inline int GetStageType() const {
		return _stageType;
	}

	inline bool IsAltPath() const {
		return _stageType == STAGETYPE_REPENTANCE || _stageType == STAGETYPE_REPENTANCE_B;
	}

	inline int GetLevelStage(bool second, bool greedMode) const {
		if (greedMode) {
			return _greedLevelStage;
		} else if (second && _levelStage > 0 && _levelStage < STAGE4_2) {
			return _levelStage + 1;
		}
		return _levelStage;
	}

protected:
	int _stageType = -1;
	int _levelStage = -1;
	int _greedLevelStage = -1;

private:
	uint32_t _stageId;
	uint32_t _baseStageId;

	std::string _binary;
	std::string _greedBinary;
	
	// RoomConfig_Stage attributes
	std::string _displayName;
	std::string _playerSpot;
	std::string _bossSpot;
	std::string _suffix;
	int _musicId;
	int _backdrop;

	// Custom stage attributes
	bool _hidden;
	bool _achievement;
};

size_t GetNumStages();
StageConfig* GetStage(uint32_t id);
StageConfig* GetStageByName(std::string name);
std::vector<StageConfig*> GetStagesByLevel(int levelStage, bool greedMode);

class StageOverride {
public:
	inline bool Flush() {
		if (_customStageId > 0) {
			if (StageConfig* customStage = GetStage(_customStageId)) {
				if (StageConfig* vanillaStage = GetStage(customStage->GetBaseStageId())) {
					vanillaStage->Load();
				}
			}
			_customStageId = 0;
			return true;
		}
		return false;
	}

	inline void Load(const StageConfig& customStage) {
		Flush();
		if (customStage.IsCustom()) {
			_customStageId = customStage.GetStageId();
			customStage.Load();
		}
	}
	inline void Load(const uint32_t customStageId) {
		if (StageConfig* customStage = GetStage(customStageId)) {
			Load(*customStage);
		} else {
			Flush();
		}
	}

	inline bool HasOverride() {
		return _customStageId > 0;
	}

	inline uint32_t GetVanillaStageID() {
		if (_customStageId > 0) {
			if (StageConfig* stage = GetStage(_customStageId)) {
				return stage->GetBaseStageId();
			}
		}
		return 0;
	}

	inline uint32_t GetCustomStageID() {
		return _customStageId;
	}

	inline bool IsOverridden(const uint32_t vanillaStageId) {
		return GetVanillaStageID() == vanillaStageId;
	}

private:
	uint32_t _customStageId = 0;
};

StageOverride& GetOverride(int slot);
StageOverride& GetCurrentOverride();

}  // namespace StageManager
