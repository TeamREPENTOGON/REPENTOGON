#include "SaveSyncing.h"

#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <regex>

#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "IsaacRepentance.h"
#include "Log.h"
#include "HookSystem.h"
#include "LuaCore.h"
#include "SaveFile.h"
#include "SigScan.h"

#include "../Patches/JsonSavedata.h"
#include "../MiscFunctions.h"
#include "../REPENTOGONOptions.h"

namespace SaveSyncing {

// REPENTOGON uses a separate save file that we automatically keep synced with the vanilla save file.
// Under normal circumstances, this process should be invisible.
//
// The reason this functionality exists at all is due to the possibility of a Repentance+ update
// introducing new achievements, event counters, or making any other change to the save files.
// Since REPENTOGON may continue to run on an older version of the game, we need to handle this.
// 
// By maintaining our own save file, we make it much less likely for us to cause the user to lose
// progress or corrupt their save file in such a situation. Synchronization can be performed even
// if the two saves have different counts of achievements/counters/etc - only the shared ones are synced.
//
// Though otherwise we do have strict validations in place on the expected structure of the save file.
// If anything does not meet our expectations, we will not perform syncronization, and will need to
// investigate the changes to the save files.
//
// If we are ever in a situation where synchronization cannot be performed, rgon can continue to
// use its separate save file until the sync can be restored. The user should never lose progress,
// as when we restore the sync between diverged saves we join and take the maximum values of both
// saves. At worst, this could restore a lost streak or donation machine contents.
//
// Under normal circumstances, we can just keep the two saves in sync by copying the more recent state.

struct SaveDataPathInfo {
	const char* name;
	const char* localPath;
	const char* cloudPath;
};

static const std::unordered_map<GameVersion, SaveDataPathInfo> SAVE_DATA_PATHS = {
	{ REBIRTH,			{ "Rebirth",		"%s../Binding of Isaac Rebirth/persistentgamedata%d.dat",		"persistentgamedata%d.dat",				} },
	{ AFTERBIRTH,		{ "Afterbirth",		"%s../Binding of Isaac Afterbirth/persistentgamedata%d.dat",	"ab_persistentgamedata%d.dat",			} },
	{ AFTERBIRTH_PLUS,	{ "Afterbirth+",	"%s../Binding of Isaac Afterbirth+/persistentgamedata%d.dat",	"abp_persistentgamedata%d.dat",			} },
	{ REPENTANCE,		{ "Repentance",		"%s../Binding of Isaac Repentance/persistentgamedata%d.dat",	"rep_persistentgamedata%d.dat",			} },
	{ REPENTANCE_PLUS,	{ "Repentance+",	"%spersistentgamedata%d.dat",									"rep+persistentgamedata%d.dat",			} },
	{ REPENTOGON,		{ "REPENTOGON",		"%sRepentogon/rgon_persistentgamedata%d.dat",					"rgon_steam_persistentgamedata%d.dat",	} },
};

// Given a PersistentGameData path to the REPENTOGON save file, returns the corresponding vanilla Repentance+ save file path.
std::string ConvertRgonSavePathToVanilla(const std::string rgonSavePath) {
	return std::regex_replace(rgonSavePath, std::regex(R"((Repentogon[\/\\]+rgon_)|(rgon_steam_))"), "");
}

// Shared paths for GameState. At the moment, REPENTOGON still uses the same GameState file.
constexpr char GAMESTATE_PATH[] = "%sgamestate%d.dat";
constexpr char GAMESTATE_PATH_STEAMCLOUD[] = "rep+gamestate%d.dat";

constexpr int PATH_BUFFER_SIZE = 1024;

std::string GetPersistentGameDataPath(const GameVersion version, const int slot, const bool steamCloud) {
	if (SAVE_DATA_PATHS.find(version) == SAVE_DATA_PATHS.end()) {
		return "";
	}

	auto pathBuffer = std::make_unique<char[]>(PATH_BUFFER_SIZE);

	if (steamCloud) {
		snprintf(pathBuffer.get(), PATH_BUFFER_SIZE, SAVE_DATA_PATHS.at(version).cloudPath, slot);
		return std::string(pathBuffer.get());
	}
	
	snprintf(pathBuffer.get(), PATH_BUFFER_SIZE, SAVE_DATA_PATHS.at(version).localPath, &g_SaveDataPath, slot);
	// Normalize the path string. Replace backslashes with forward slashes because this game is awesome
	return std::regex_replace(std::filesystem::weakly_canonical(pathBuffer.get()).string(), std::regex(R"(\\)"), "/");
}

bool ImportSlot(const GameVersion srcVersion, const GameVersion dstVersion, const int slot, const SaveSyncMode mode) {
	if (slot < 1 || slot > 3) {
		ZHL::Log("[SaveSyncing] Cannot import unknown slot %d\n", slot);
		return false;
	}
	ZHL::Log("[SaveSyncing] Attempting to import slot %d from %s to %s\n", slot, SAVE_DATA_PATHS.at(srcVersion).name, SAVE_DATA_PATHS.at(dstVersion).name);

	const bool steamCloudEnabled = g_Manager->IsSteamCloudEnabled();

	const std::string srcPath = GetPersistentGameDataPath(srcVersion, slot, steamCloudEnabled);
	const std::string dstPath = GetPersistentGameDataPath(dstVersion, slot, steamCloudEnabled);

	ZHL::Log("[SaveSyncing] Source: %s\n", srcPath.c_str());
	ZHL::Log("[SaveSyncing] Destination: %s\n", dstPath.c_str());

	const SaveFile srcSave(srcPath.c_str(), steamCloudEnabled);
	if (!srcSave.OpenedFile()) {
		ZHL::Log("[SaveSyncing] Failed to open %s\n", srcPath.c_str());
		return false;
	}
	if (!srcSave.IsValid()) {
		ZHL::Log("[SaveSyncing] Failed to validate %s\n", srcPath.c_str());
		return false;
	}

	SaveFile dstSave(dstPath.c_str(), steamCloudEnabled);
	if (!dstSave.OpenedFile()) {
		ZHL::Log("[SaveSyncing] Failed to open %s\n", dstPath.c_str());
		return false;
	}
	if (!dstSave.IsValid()) {
		ZHL::Log("[SaveSyncing] Failed to validate %s\n", dstPath.c_str());
		return false;
	}

	if (!dstSave.Sync(srcSave, mode)) {
		ZHL::Log("[SaveSyncing] Import failed.\n");
		return false;
	}
	if (!dstSave.Save()) {
		ZHL::Log("[SaveSyncing] Failed to write modified save %s\n", dstPath.c_str());
		return false;
	}

	ZHL::Log("[SaveSyncing] Import successful!\n");
	return true;
}

bool ImportAll(const GameVersion srcVersion, const GameVersion dstVersion, const SaveSyncMode mode) {
	for (int slot = 1; slot <= 3; slot++) {
		if (!ImportSlot(srcVersion, dstVersion, slot, mode)) {
			return false;
		}
	}
	return true;
}

bool ImportFrom(const GameVersion srcVersion, const int slot, const SaveSyncMode mode) {
	if (slot == 0) {
		return ImportAll(srcVersion, REPENTOGON, mode);
	}
	return ImportSlot(srcVersion, REPENTOGON, slot, mode);
}

bool ExportTo(const GameVersion dstVersion, const int slot, const SaveSyncMode mode) {
	if (slot == 0) {
		return ImportAll(REPENTOGON, dstVersion, mode);
	}
	return ImportSlot(REPENTOGON, dstVersion, slot, mode);
}

// ----------------------------------------------------------------------------------------------------
// -- ASM Patches to replace the file path formats used for the save files

// Replaces the filepath used for local saves.
void ASMPatchLocalSaveFileName() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SetSaveSlotLocalFilePath);

	printf("[REPENTOGON] Patching Manager::SetSaveSlot for the local save file name @ %p\n", addr);

	const char* ptr = SAVE_DATA_PATHS.at(REPENTOGON).localPath;

	ASMPatch patch;
	patch.AddBytes("\x68").AddBytes(ByteBuffer().AddAny((char*)&ptr, 4))
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// Replaces the filename used for remote (steam cloud) saves.
void ASMPatchRemoteSaveFileName() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::SetSaveSlotSteamCloudFileName);

	printf("[REPENTOGON] Patching Manager::SetSaveSlot for the steam cloud save file name @ %p\n", addr);

	const char* ptr = SAVE_DATA_PATHS.at(REPENTOGON).cloudPath;

	ASMPatch patch;
	patch.AddBytes("\x68").AddBytes(ByteBuffer().AddAny((char*)&ptr, 4))
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForSaveSyncing() {
	if (USE_SEPARATE_REPENTOGON_SAVE_FILES) {
		ASMPatchLocalSaveFileName();
		ASMPatchRemoteSaveFileName();
	}
}

// ----------------------------------------------------------------------------------------------------
// -- JSON file maintained with the checksums of the most recent successful save syncs

SyncStatus syncStatus;  // `extern SaveSync::SyncStatus syncStatus` defined in header

std::string SyncStatus::GetKey(const int slot, const bool isRepentogon) {
	return std::string(isRepentogon ? "REPENTOGON." : "Vanilla.") + std::to_string(slot);
}

std::string SyncStatus::GetJsonPath() {
	if (g_Manager->IsSteamCloudEnabled()) {
		return "rgon_savesyncstatus.json";
	}
	return std::string(REPENTOGON::GetRepentogonDataPath()) + "savesyncstatus.json";
}

bool SyncStatus::SaveToJson() {
	if (!_loaded) {
		ZHL::Log("[SaveSyncing] Cannot save SyncStatus - not initialized!\n");
		return false;
	}

	rapidjson::Document doc;
	doc.SetObject();
	doc.AddMember(rapidjson::StringRef("AutoSyncingEnabled"), _autoSyncEnabled, doc.GetAllocator());
	ArrayToJson(&doc, "Checksums", _checksums);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	const std::string jsonPath = GetJsonPath();

	if (g_Manager->IsSteamCloudEnabled()) {
		SteamCloudFile file;
		if (!file.PushToSteamCloud(jsonPath.c_str(), buffer.GetSize()) || !file.IsOpen()) {
			ZHL::Log("[SaveSyncing] Failed to write %s to the steam cloud!\n", jsonPath.c_str());
			return false;
		}
		file.Write(buffer.GetString(), 1, buffer.GetSize());
		file.Close();
	} else {
		KAGE_Filesys_File file;
		if (!file.OpenWrite(jsonPath.c_str()) || !file.IsOpen()) {
			ZHL::Log("[SaveSyncing] Failed to write %s!\n", jsonPath.c_str());
			return false;
		}
		file.Write(buffer.GetString(), 1, buffer.GetSize());
		file.Close();
	}

	ZHL::Log("[SaveSyncing] Successfully wrote %s\n", jsonPath.c_str());
	return true;
}

bool SyncStatus::LoadFromJson() {
	if (_loaded) {
		ZHL::Log("[SaveSyncing] Cannot load SyncStatus - already loaded!\n");
		return false;
	}

	const std::string jsonPath = GetJsonPath();

	const bool steamCloudEnabled = g_Manager->IsSteamCloudEnabled();

	std::unique_ptr<KAGE_Filesys_FileStream> file;

	if (steamCloudEnabled) {
		file = std::make_unique<SteamCloudFile>();
	} else {
		file = std::make_unique<KAGE_Filesys_File>();
	}

	ZHL::Log("[SaveSyncing] Reading SyncStatus from json @ %s\n", jsonPath.c_str());

	if (!file->OpenRead(jsonPath.c_str()) || !file->IsOpen()) {
		if (steamCloudEnabled || !std::filesystem::exists(jsonPath)) {
			ZHL::Log("[SaveSyncing] SyncStatus json file does not exist yet. Using defaults.\n");
			_loaded = true;
			return true;
		}
		ZHL::Log("[SaveSyncing] Failed to open SyncStatus json file.\n");
		return false;
	}

	const long size = file->GetSize();
	auto content = std::make_unique<char[]>(size + 1);
	content[size] = '\0';

	file->Read(content.get(), 1, size);

	rapidjson::Document doc;
	ZHL::Log("[SaveSyncing] Parsing SyncStatus json...\n");
	doc.Parse(content.get());
	if (doc.IsObject()) {
		if (doc.HasMember("AutoSyncingEnabled")) {
			_autoSyncEnabled = doc["AutoSyncingEnabled"].GetBool();
		}
		if (doc.HasMember("Checksums")) {
			JsonToArray(doc["Checksums"], _checksums);
		}
		ZHL::Log("[SaveSyncing] ...success!\n");
	} else {
		ZHL::Log("[SaveSyncing] ...failed! Using defaults.\n");
	}
	
	_loaded = true;
	return true;
}

// ----------------------------------------------------------------------------------------------------
// -- Core Save Synchronization

// Checks if a GameState file exists for this slot, and if so, generates & returns the checksum for it.
// We update the GameState checksum when syncing so that switching between RGON and vanilla does not always delete it.
uint32_t GetGamestateChecksum(const int slot, const bool steamCloud) {
	auto gamestatePathBuffer = std::make_unique<char[]>(PATH_BUFFER_SIZE);
	if (steamCloud) {
		snprintf(gamestatePathBuffer.get(), PATH_BUFFER_SIZE, GAMESTATE_PATH_STEAMCLOUD, slot);
	} else {
		snprintf(gamestatePathBuffer.get(), PATH_BUFFER_SIZE, GAMESTATE_PATH, &g_SaveDataPath, slot);
	}
	ZHL::Log("[SaveSyncing] Reading GameState from: %s\n", gamestatePathBuffer.get());
	std::unique_ptr<KAGE_Filesys_FileStream> gamestateFile;
	if (steamCloud) {
		gamestateFile = std::make_unique<SteamCloudFile>();
	} else {
		gamestateFile = std::make_unique<KAGE_Filesys_File>();
	}
	if (gamestateFile->OpenRead(gamestatePathBuffer.get()) && gamestateFile->IsOpen()) {
		return Checksum::Generate(0, gamestateFile.get(), 16, 4);
	}
	return 0;
}

bool SaveFileExists(const std::string path, const bool steamCloud) {
	if (steamCloud) {
		SteamCloudFile file;
		if (file.OpenRead(path.c_str())) {
			file.Close();
			return true;
		}
		return false;
	}
	return std::filesystem::exists(path.c_str());
}

void TryInitRepentogonSaveFile(const int slot) {
	if (slot < 1 || slot > 3) return;

	if (!USE_SEPARATE_REPENTOGON_SAVE_FILES) {
		ZHL::Log("[SaveSyncing] Separate REPENTOGON save file is disabled.\n");
		return;
	}

	const bool steamCloudEnabled = g_Manager->IsSteamCloudEnabled();

	const std::string rgonPath = GetPersistentGameDataPath(REPENTOGON, slot, steamCloudEnabled);

	if (SaveFileExists(rgonPath, steamCloudEnabled)) {
		// Already exists.
		return;
	}

	syncStatus.ClearChecksum(SyncStatus::GetKey(slot, /*isRepentogon=*/true));

	ZHL::Log("[SaveSyncing] Initializing REPENTOGON save file for slot %d @ %s\n", slot, rgonPath);
	
	// Start by initializing the PersistentGameData normally, importing from a previous DLC if available (Repentance, AB+, etc)
	PersistentGameData pgd;
	if (steamCloudEnabled) {
		pgd.steamcloudfilepath = rgonPath;
		pgd.LoadFromSteamCloud();
		pgd.SaveToSteamCloud();
	} else {
		pgd.Load(rgonPath.c_str());
		pgd.SaveLocally();
	}

	// Attempt to overwrite with achievements/progress from the corresponding vanilla Rep+ save file.
	ZHL::Log("[SaveSyncing] Attempting to import progress from Repentance+...\n");
	if (!ImportFrom(REPENTANCE_PLUS, slot, SAVE_SYNC_OVERWRITE)) {
		ZHL::Log("[SaveSyncing] Could not import progress from Repentance+.\n");
	}

	ZHL::Log("[SaveSyncing] Finished initializing REPENTOGON save file.\n");
}

// Checks if the REPENTOGON save files exist yet, and if they don't, initializes them.
void TryInitRepentogonSaveFiles() {
	for (int slot = 1; slot <= 3; slot++) {
		TryInitRepentogonSaveFile(slot);
	}
}

// Make sure that the REPENTOGON save file is properly initialized.
HOOK_METHOD(Manager, SetSaveSlot, (unsigned int slot) -> void) {
	TryInitRepentogonSaveFile(slot);
	super(slot);
}

// The first time we perform synchronization, we perform a "full" sync by taking the maximum values for all
// shared achievements/counters/etc from both save files and merging them together, writing that to both saves.
//
// Upon successful sync, we write the checksums of the files at the time of the sync to a json file.
//
// For following syncs, we can refer to the checksums to see which files changed. If only one of the two
// save files has been modified since the last successful sync, we can simply copy the data from the
// modified save file to the other one. This is better than always taking the maximum values since we need
// to account for things like decreasing event counters (streaks being lost, donation machines, etc) as well
// as the ability for REPENTOGON mods to lock achievements, unclear challenges, decrement event counters, etc.
//
// However, if both saves have been modified since the last successful sync, then we'll have to perform a "full"
// sync again, as the progress on the two saves may have diverged. In theory, this should only be necessary if
// some issue causes synchronization to fail while the user is switching between playing on REPENTOGON and vanilla.
// Possible issues include Nicalis making a change to the Rep+ save format, persistent crashing, or perhaps some
// niche edge case with the save data contents that we did not account for.
bool TrySyncSaveSlot(const int slot, const bool isStartup) {
	if (!USE_SEPARATE_REPENTOGON_SAVE_FILES) {
		ZHL::Log("[SaveSyncing] Separate REPENTOGON save file is disabled.\n");
		return false;
	}

	const bool steamCloudEnabled = g_Manager->IsSteamCloudEnabled();

	const std::string vanillaPathStr = GetPersistentGameDataPath(REPENTANCE_PLUS, slot, steamCloudEnabled);
	const std::string rgonPathStr = GetPersistentGameDataPath(REPENTOGON, slot, steamCloudEnabled);

	const char* vanillaPath = vanillaPathStr.c_str();
	const char* rgonPath = rgonPathStr.c_str();

	const std::string vanillaSyncKey = SyncStatus::GetKey(slot, /*isRepentogon=*/false);
	const std::string rgonSyncKey = SyncStatus::GetKey(slot, /*isRepentogon=*/true);

	ZHL::Log("[SaveSyncing] Beginning sync for %s saves in slot %d\n", steamCloudEnabled ? "cloud" : "local", slot);
	ZHL::Log("[SaveSyncing] Vanilla save path: %s\n", vanillaPath);
	ZHL::Log("[SaveSyncing] REPENTOGON save path: %s\n", rgonPath);

	const uint32_t gamestateChecksum = GetGamestateChecksum(slot, steamCloudEnabled);
	if (gamestateChecksum > 0) {
		ZHL::Log("[SaveSyncing] Current GameState checksum: %u\n", gamestateChecksum);
	} else {
		ZHL::Log("[SaveSyncing] No GameState found.\n");
	}

	// Read the REPENTOGON save file.
	SaveFile rgonSave(rgonPath, steamCloudEnabled);
	if (!rgonSave.OpenedFile()) {
		ZHL::Log("[SaveSyncing] REPENTOGON save file not initialized for slot %d @ %s\n", slot, rgonPath);
		return false;
	}
	if (!rgonSave.IsValid()) {
		ZHL::Log("[SaveSyncing] Failed to validate rgon save file for slot %d @ %s\n", slot, rgonPath);
		return false;
	}
	uint32_t rgonGamestateChecksum = 0;
	if (!rgonSave.ReadGamestateChecksum(&rgonGamestateChecksum)) {
		ZHL::Log("[SaveSyncing] Failed to read gamestate checksum from rgon save for slot %d @ %s\n", slot, rgonPath);
		return false;
	}
	const bool updateRgonGamestateChecksum = rgonGamestateChecksum != gamestateChecksum;
	uint32_t rgonChecksum = rgonSave.GenerateChecksum(true);

	// Read the vanilla save file.
	SaveFile vanillaSave(vanillaPath, steamCloudEnabled);
	if (!vanillaSave.OpenedFile()) {
		// We can't perform synchronization without a vanilla save file, but don't consider this an error.
		syncStatus.ClearChecksum(vanillaSyncKey);
		ZHL::Log("[SaveSyncing] No vanilla save file found for slot %d @ %s\n", slot, vanillaPath);
		return true;
	}
	if (!vanillaSave.IsValid()) {
		ZHL::Log("[SaveSyncing] Failed to validate vanilla save file for slot %d @ %s\n", slot, vanillaPath);
		return false;
	}
	uint32_t vanillaGamestateChecksum = 0;
	if (!vanillaSave.ReadGamestateChecksum(&vanillaGamestateChecksum)) {
		ZHL::Log("[SaveSyncing] Failed to read gamestate checksum from vanilla save for slot %d @ %s\n", slot, vanillaPath);
		return false;
	}
	const bool updateVanillaGamestateChecksum = vanillaGamestateChecksum != gamestateChecksum;
	uint32_t vanillaChecksum = vanillaSave.GenerateChecksum(true);

	// Check if each save has changed since the last successful sync (OR no successful sync is logged).
	const bool previouslySynced = syncStatus.HasChecksum(vanillaSyncKey) && syncStatus.HasChecksum(rgonSyncKey);
	const bool vanillaChanged = !previouslySynced || !syncStatus.ChecksumMatches(vanillaSyncKey, vanillaChecksum);
	const bool rgonChanged = !previouslySynced || !syncStatus.ChecksumMatches(rgonSyncKey, rgonChecksum);

	// As an extra (possibly unnecessary) safety check, if the REPENTOGON save file is found to have been modified during startup, trigger a full sync.
	// This case could imply the save file was modified externally or some other issue occured that caused the shutdown sync to fail.
	const bool unexpectedRgonModification = previouslySynced && rgonChanged && isStartup;

	const bool fullSync = !previouslySynced || (vanillaChanged && rgonChanged) || unexpectedRgonModification;
	const bool syncVanilla = fullSync || (!vanillaChanged && rgonChanged);
	const bool syncRgon = fullSync || (vanillaChanged && !rgonChanged);

	const SaveSyncMode syncMode = fullSync ? SAVE_SYNC_TAKE_MAX : SAVE_SYNC_OVERWRITE;

	if (!syncVanilla && !syncRgon) {
		ZHL::Log("[SaveSyncing] No synchronization required for slot %d\n", slot);
		if (!updateVanillaGamestateChecksum && !updateRgonGamestateChecksum) {
			return true;
		}
	} else if (!fullSync) {
		ZHL::Log("[SaveSyncing] Only one save has been modified since the last successful sync. Updating sync...\n");
	} else if (!previouslySynced) {
		ZHL::Log("[SaveSyncing] No previous synchronization found. Performing a full synchronization...\n");
	} else if (unexpectedRgonModification) {
		ZHL::Log("[SaveSyncing] Detected potential external REPENTOGON save file modification. Performing a full synchronization...\n");
	} else {
		ZHL::Log("[SaveSyncing] Both saves have been modified and are potentially diverged. Performing a full synchronization...\n");
	}

	if (syncRgon || updateRgonGamestateChecksum) {
		if (syncRgon) {
			ZHL::Log("[SaveSyncing] Syncing REPENTOGON save...\n");
			if (!rgonSave.Sync(vanillaSave, syncMode)) {
				ZHL::Log("[SaveSyncing] Failed to sync rgon save with vanilla save for slot %d\n", slot);
				return false;
			}
		}

		if (updateRgonGamestateChecksum) {
			ZHL::Log("[SaveSyncing] Updating REPENTOGON save's gamestate checksum...\n");
			if (!rgonSave.SetGamestateChecksum(gamestateChecksum)) {
				ZHL::Log("[SaveSyncing] Failed to update rgon save's gamestate checksum for slot %d\n", slot);
				return false;
			}
		}

		if (!rgonSave.Save()) {
			ZHL::Log("[SaveSyncing] Failed to write updated rgon save for slot %d\n", slot);
			return false;
		}
	}

	if (syncVanilla || updateVanillaGamestateChecksum) {
		if (syncVanilla) {
			ZHL::Log("[SaveSyncing] Syncing vanilla save...\n");
			if (!vanillaSave.Sync(rgonSave, syncMode)) {
				ZHL::Log("[SaveSyncing] Failed to sync vanilla save with rgon save for slot %d\n", slot);
				return false;
			}
		}

		if (updateVanillaGamestateChecksum) {
			ZHL::Log("[SaveSyncing] Updating vanilla save's gamestate checksum...\n");
			if (!vanillaSave.SetGamestateChecksum(gamestateChecksum)) {
				ZHL::Log("[SaveSyncing] Failed to update vanilla save's gamestate checksum for slot %d\n", slot);
				return false;
			}
		}

		if (!vanillaSave.Save()) {
			ZHL::Log("[SaveSyncing] Failed to write updated vanilla save for slot %d\n", slot);
			return false;
		}
	}

	// Update checksums for last successful sync.
	syncStatus.UpdateChecksum(vanillaSyncKey, vanillaSave.GenerateChecksum(true));
	syncStatus.UpdateChecksum(rgonSyncKey, rgonSave.GenerateChecksum(true));

	ZHL::Log("[SaveSyncing] Completed synchronization for slot %d\n", slot);
	return true;
}

bool PerformVanillaSaveSynchronization(const bool isStartup) {
	if (!USE_SEPARATE_REPENTOGON_SAVE_FILES) {
		ZHL::Log("[SaveSyncing] Separate REPENTOGON save file is disabled.\n");
		return false;
	}

	if (!syncStatus.IsLoaded()) {
		ZHL::Log("[SaveSyncing] Cannot sync - SyncStatus not loaded.\n");
		return false;
	}

	bool syncingFailed = false;

	for (int slot = 1; slot <= 3; slot++) {
		if (!TrySyncSaveSlot(slot, isStartup)) {
			syncingFailed = true;
		}
	}

	if (!syncStatus.SaveToJson()) {
		syncingFailed = true;
	}

	return !syncingFailed;
}

static bool startupSyncWasSuccessful = false;

bool PerformAutomaticVanillaSaveSynchronization(const bool isStartup) {
	if (!USE_SEPARATE_REPENTOGON_SAVE_FILES) {
		ZHL::Log("[SaveSyncing] Separate REPENTOGON save file is disabled.\n");
		return false;
	}

	if (!syncStatus.IsEnabled()) {
		ZHL::Log("[SaveSyncing] Automatic save syncing is disabled.\n");
		return false;
	}

	if (!isStartup && !startupSyncWasSuccessful) {
		ZHL::Log("[SaveSyncing] Skipping automatic sync - startup sync was not performed / not successful!\n");
		return false;
	}

	if (!PerformVanillaSaveSynchronization(isStartup)) {
		MessageBox(0, LANG.ERROR_SAVE_SYNC_FAILED, "REPENTOGON", MB_ICONERROR);
		return false;
	}

	if (isStartup) {
		startupSyncWasSuccessful = true;
	}
	return true;
}

// Sync on startup, before the game has read any save files.
HOOK_GLOBAL(IsaacStartup, (int param1, int param2, int param3) -> void, _X86_) {
	super(param1, param2, param3);
	if (USE_SEPARATE_REPENTOGON_SAVE_FILES) {
		syncStatus.LoadFromJson();
		TryInitRepentogonSaveFiles();
		PerformAutomaticVanillaSaveSynchronization(true);
	}
}

// Sync on shutdown, after the game is already finished with save-related stuff.
HOOK_METHOD(Manager, destructor, () -> void) {
	if (USE_SEPARATE_REPENTOGON_SAVE_FILES) {
		PerformAutomaticVanillaSaveSynchronization(false);
	}
	super();
}

// The functions for importing saves from previous versions expect to be passed the vanilla save path,
// and they copy/modify it to obtain the path for the legacy save. These don't work at all when passed
// the REPENTOGOIN save data paths, so here we fix them to still start from the vanilla paths.
#define FIX_LOCAL_SAVE_IMPORT(_func) \
HOOK_METHOD(PersistentGameData, _func, (const char* path) -> bool) { \
	if (!USE_SEPARATE_REPENTOGON_SAVE_FILES) return super(path); \
	std::string vanillaPath = ConvertRgonSavePathToVanilla(path); \
	return super(vanillaPath.c_str()); \
}
FIX_LOCAL_SAVE_IMPORT(TryImportRepLocalSave);
FIX_LOCAL_SAVE_IMPORT(TryImportABPLocalSave);
FIX_LOCAL_SAVE_IMPORT(TryImportABLocalSave);
FIX_LOCAL_SAVE_IMPORT(TryImportRebirthLocalSave);

// Same as above for the steam cloud files, though these read from `steamcloudfilepath` instead of taking a parameter.
// Note there is no function to import a rebirth save from the cloud, for some reason!!
#define FIX_STEAM_CLOUD_SAVE_IMPORT(_func) \
HOOK_METHOD(PersistentGameData, _func, () -> bool) { \
	if (!USE_SEPARATE_REPENTOGON_SAVE_FILES) return super(); \
	const std::string rgonPath = this->steamcloudfilepath; \
	this->steamcloudfilepath = ConvertRgonSavePathToVanilla(rgonPath); \
	const bool result = super(); \
	this->steamcloudfilepath = rgonPath; \
	return result; \
}
FIX_STEAM_CLOUD_SAVE_IMPORT(TryImportRepCloudSave);
FIX_STEAM_CLOUD_SAVE_IMPORT(TryImportABPCloudSave);
FIX_STEAM_CLOUD_SAVE_IMPORT(TryImportABCloudSave);

// ----------------------------------------------------------------------------------------------------
// -- Testing Utils

#ifndef NDEBUG

LUA_FUNCTION(Lua_ValidateLocalSave) {
	std::string path = luaL_checkstring(L, 1);
	SaveFile saveFile(path.c_str(), false);
	lua_pushboolean(L, saveFile.IsValid());
	return 1;
}

LUA_FUNCTION(Lua_ValidateCloudSave) {
	std::string path = luaL_checkstring(L, 1);
	SaveFile saveFile(path.c_str(), true);
	lua_pushboolean(L, saveFile.IsValid());
	return 1;
}

LUA_FUNCTION(Lua_TestSync) {
	// Based on ModdingDataPath because this is used for tests that load sample saves from the REPENTOGON Test Mod.
	std::filesystem::path basePath(&g_ModdingDataPath);

	std::filesystem::path dstFilePath = basePath / std::filesystem::path(luaL_checkstring(L, 1));
	std::filesystem::path srcFilePath = basePath / std::filesystem::path(luaL_checkstring(L, 2));
	std::filesystem::path outputPath = basePath / std::filesystem::path(luaL_checkstring(L, 3));
	const bool overwrite = lua::luaL_optboolean(L, 4, false);

	if (!std::filesystem::exists(dstFilePath) || !std::filesystem::exists(srcFilePath)) {
		ZHL::Log("[SaveFilesTest.TestSync] Input file(s) not found.\n");
		lua_pushboolean(L, false);
		return 1;
	}

	const std::string dstFilePathStr = dstFilePath.string();
	const std::string srcFilePathStr = srcFilePath.string();
	const std::string outputPathStr = outputPath.string();

	SaveFile dstFile(dstFilePathStr.c_str(), false);
	SaveFile srcFile(srcFilePathStr.c_str(), false);

	if (!dstFile.Sync(srcFile, overwrite ? SAVE_SYNC_OVERWRITE : SAVE_SYNC_TAKE_MAX)) {
		ZHL::Log("[SaveFilesTest.TestSync] Failed to sync\n");
		lua_pushboolean(L, false);
		return 1;
	}

	const bool saved = dstFile.SaveLocal(outputPathStr);
	if (!saved) {
		ZHL::Log("[SaveFilesTest.TestSync] Failed to save synced output\n");
	}
	lua_pushboolean(L, saved);
	return 1;
}

LUA_FUNCTION(Lua_CompareSaves) {
	std::filesystem::path basePath(&g_ModdingDataPath);
	const std::string first = (basePath / std::filesystem::path(luaL_checkstring(L, 1))).string();
	const std::string second = (basePath / std::filesystem::path(luaL_checkstring(L, 2))).string();
	const bool ignoreGamestateAndCounter = lua::luaL_optboolean(L, 3, false);

	if (!std::filesystem::exists(first) || !std::filesystem::exists(second)) {
		ZHL::Log("[SaveFilesTest.CompareSaves] Input file(s) not found!\n");
		lua_pushboolean(L, false);
		return 1;
	}

	SaveFile firstFile(first.c_str(), false);
	SaveFile secondFile(second.c_str(), false);

	if (!firstFile.IsValid() || !secondFile.IsValid()) {
		ZHL::Log("[SaveFilesTest.CompareSaves] Save file is invalid!\n");
		lua_pushboolean(L, false);
		return 1;
	}

	if (firstFile.GetSize() != secondFile.GetSize()) {
		ZHL::Log("[SaveFilesTest.CompareSaves] Save files are different sizes!\n");
		lua_pushboolean(L, false);
		return 1;
	}

	if (firstFile.GenerateChecksum(ignoreGamestateAndCounter) != secondFile.GenerateChecksum(ignoreGamestateAndCounter)) {
		ZHL::Log("[SaveFilesTest.CompareSaves] Save file contents are different!\n");
		lua_pushboolean(L, false);
		return 1;
	}

	lua_pushboolean(L, true);
	return 1;
}

static void RegisterSaveManagementTestUtils(lua_State* L)
{
	lua_getglobal(L, "UnitTests");
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
		lua_setglobal(L, "UnitTests");
		lua_getglobal(L, "UnitTests");
	}
	lua_newtable(L);
	lua_setfield(L, -2, "SaveUtility");
	lua_getfield(L, -1, "SaveUtility");

	lua::TableAssoc(L, "ValidateLocalSave", Lua_ValidateLocalSave);
	lua::TableAssoc(L, "ValidateCloudSave", Lua_ValidateCloudSave);

	lua::TableAssoc(L, "TestSync", Lua_TestSync);
	lua::TableAssoc(L, "CompareSaves", Lua_CompareSaves);

	lua_pop(L, 2);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterSaveManagementTestUtils(_state);
}

#endif // !NDEBUG

}  // namespace SaveSyncing
