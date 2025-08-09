#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "SaveFile.h"

namespace SaveSyncing {

// Global toggle for whether REPENTOGON should use a separate save file from vanilla.
// Temporary measure to test this functionality in a more controlled environment.
// Long term, we need to use a separate save file as a precaution against future game updates.
static constexpr bool USE_SEPARATE_REPENTOGON_SAVE_FILES = true;

// Holds checksums for the most recent successful synchronizations.
// Allows us to identify if the saves are out of sync or not.
// Contents are saved and loaded from json.
class SyncStatus {
  public:
	bool HasChecksum(const std::string& key) {
		return _checksums.find(key) != _checksums.end();
	}
	bool ChecksumMatches(const std::string& key, const uint32_t checksum) {
		return HasChecksum(key) && _checksums.at(key) == checksum;
	}
	void UpdateChecksum(const std::string& key, const uint32_t checksum) {
		_checksums[key] = checksum;
	}
	void ClearChecksum(const std::string& key) {
		_checksums.erase(key);
	}

	bool IsLoaded() { return _loaded; }
	bool IsEnabled() { return _autoSyncEnabled; }
	bool* GetEnabledPtr() { return &_autoSyncEnabled; }

	static std::string GetKey(const int slot, const bool isRepentogon);
	static std::string SyncStatus::GetJsonPath();

	bool SaveToJson();
	bool LoadFromJson();

  private:
	std::map<std::string, uint32_t> _checksums;
	bool _loaded = false;
	bool _autoSyncEnabled = true;
};

extern SyncStatus syncStatus;

enum GameVersion {
	UNKNOWN = 0,
	REBIRTH = 1,
	AFTERBIRTH = 2,
	AFTERBIRTH_PLUS = 3,
	REPENTANCE = 4,
	REPENTANCE_PLUS = 5,
	REPENTOGON = 6,
};

// Imports any missing achievements/progress to the REPENTOGON save file from another version of the game.
// By default can only add/update missing/lesser values, will not remove anything.
bool ImportFrom(GameVersion srcVersion, int slot, SaveSyncMode mode = SAVE_SYNC_TAKE_MAX);

// Copies achievements/progress from the REPENTOGON save file over to the save from another version of the game.
// By default can only add/update missing/lesser values, will not remove anything.
bool ExportTo(GameVersion dstVersion, int slot, SaveSyncMode mode = SAVE_SYNC_TAKE_MAX);

// Runs a synchronization between the REPENTOGON save file and the corresponding vanilla Repentance+ save file.
// If previously synced, carries over save data changes from whichever file was updated over to the other one.
// Otherwise, (re)establishes synchronization by merging the achievements/progress from both files onto each other.
bool PerformVanillaSaveSynchronization(const bool isStartup);

// Applies ASM patches that cause the game to read the REPENTOGON PersistentGameData files.
void ASMPatchesForSaveSyncing();

}  // namespace SaveSyncing
