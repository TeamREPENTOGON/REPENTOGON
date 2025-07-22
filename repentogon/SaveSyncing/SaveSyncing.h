#pragma once

#include <map>
#include <string>

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

	bool IsLoaded() { return _loaded; }
	bool IsEnabled() { return _enabled; }
	bool* GetEnabledPtr() { return &_enabled; }

	static std::string GetKey(const int slot, const bool isRepentogon);
	static std::string SyncStatus::GetJsonPath();

	bool SaveToJson();
	bool LoadFromJson();

  private:
	std::map<std::string, uint32_t> _checksums;
	bool _loaded = false;
	bool _enabled = true;
};

extern SyncStatus syncStatus;

void PerformSaveSynchronization();

void ASMPatchesForSaveSyncing();

}  // namespace SaveSyncing
