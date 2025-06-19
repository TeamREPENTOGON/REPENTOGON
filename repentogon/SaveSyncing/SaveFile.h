#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <memory>
#include <filesystem>

#include "IsaacRepentance.h"
#include "Log.h"

/*
* Isaac's PersistentGameData save files have a straightforward enough format, mostly just being integers and such written directly as bytes with c file io functions.
* 
* The save file always begins with the 16-byte string like "ISAACNGSAVE09R  ", an identifier of the format of the save file.
* - AB+/REP/REP+: ISAACNGSAVE09R
* - AB: ISAACNGSAVE08R
* - REBIRTH: ISAACNGSAVE06R
* [ISAACNGSAVE09R  ]
* 
* If the save file had an ongoing run, the next value here is a KAGE checksum for that run. If the file has no ongoing run, this is just 00000000.
* The data for the run itself is stored in a different file, this checksum is just to validate it.
* If validation fails, or something like modding state changes, the game just deletes the run.
* [gameStateChecksum]
* 
* Next, bulk of the save file are individual save data "chunks". For example, chunk 1 is achievements, chunk 2 is event counters, etc.
* The chunks appear to always be in order. There are currently 11 different chunks.
* Each chunk starts with its ID, the ""size"" of the chunk data in bytes, and the number of elements in the chunk data.
* Note that the "chunkSize" seems to be inaccurate for most of the boolean-type chunks, being written as numElements*4 as if they were integers.
* I think this is an oversight, as the "size" is not used to read the save file, only the number of elements.
* [chunkID][chunkSize][chunkNumElements][chunkData] x11
* 
* Chunk data is typically just a raw list of booleans (as 00 or 01 bytes) or a raw list of integers. The bestiary has a unique format, and is the
* only chunk that has a variable size, since monsters are only added to the bestiary data once encountered, and modded enemies can be tracked as well.
* The bestiary chunk data is essentially divided into 4 "sections", written in this order: 4 (deaths) -> 2 (kills) -> 3 (hits) -> 1 (encounters)
* bestiaryChunkData = [bestiarySectionID][bestiarySectionSize][bestiarySectionData] x4
* bestiarySectionSize seems to be the number of monsters logged in the section times 4, even though each monster's entry is 8 bytes...
* Might be a similar case like above where boolean sections had their sizes written as if they were 4 bytes each instead of 1.
* Each entry in bestiarySectionData is 8 bytes, where the first 4 bytes is a compressed representation of the entity's Type+Variant, and the second 4 bytes is the data value (ie, number of kills).
* 
* The second last 4-byte value in the file, and the last value covered by the checksum below.
* Seems to increment by 2 every time you load the save file. Some sort of counter of how many times the save file has been opened?
* [counter]
* 
* The very last thing written to the save file is a KAGE checksum generated from the contents of the save file, exluding the header string ("ISAACNGSAVE09R  ") and the checksum itself.
* The game recalculates the same checksum when reading the save file too and will consider the file corrupted if it does not match the checksum written to the file.
* [checksum]
*/

// Known number of save chunks.
// If a save file ever has more or less than this amount, do not perform synchronization, as more investigation will be necessary.
constexpr int32_t NUM_SAVE_CHUNKS = 11;

// At time of writing, attempting to write a save file bigger than this to the steam cloud seems to crash the game in vanilla (though it does actually successfully save to the cloud beforehand).
//  - The game only allocates 0x10000 bytes when saving PersistentGameData to the steam cloud (hard coded param to SteamCloudFile::PushToSteamCloud).
//  - SteamCloudFile::Write() has logic to resize its buffer, but it does not update SteamCloudFile._bufferSize when it does so? That might lead to the crash. ("55 8B EC 53 56 8B F1 57 8B 7D ? 0F AF 7D")
// Granted, this is ~65.5 KB, which while not massive, would probably require some abuse of the Bestiary to occur naturally (saves are typically <10KB, though I've seen ~30KB with extensive mod usage).
// For our purposes I'll just refuse to deal with such files for now. If we find a file bigger than this, don't perform synchronization.
// It may be possible to "fix" this limitation in the future, but honestly something else must have went wrong if this ever comes up.
constexpr int32_t MAX_SAVE_FILE_SIZE = 0x10000;

// PersistentGameData saves begin with this 16-character string. This identifies the format of the save file. Note that just having different achievement counts doesn't necessitate a different "format".
//  - AB+/REP/REP+: ISAACNGSAVE09R
//  - AB: ISAACNGSAVE08R
//  - REBIRTH : ISAACNGSAVE06R
constexpr char EXPECTED_SAVE_HEADER[] = "ISAACNGSAVE09R  ";

// The bestiary sections are written in this order (deaths, kills, hits, encounters).
static const std::vector<int32_t> EXPECTED_BESTIARY_SECTION_ORDER = { 4, 2, 3, 1 };

static bool IsValidSaveChunkID(const int32_t i) {
	return i > 0 && i <= NUM_SAVE_CHUNKS;
}

enum SaveChunk {
	SAVE_CHUNK_UNKNOWN = 0,
	SAVE_CHUNK_ACHIEVEMENTS = 1,
	SAVE_CHUNK_EVENT_COUNTERS = 2,
	SAVE_CHUNK_LEVEL_COUNTERS = 3,
	SAVE_CHUNK_COLLECTIBLES = 4,
	SAVE_CHUNK_MINIBOSSES = 5,
	SAVE_CHUNK_BOSSES = 6,
	SAVE_CHUNK_CHALLENGES = 7,
	SAVE_CHUNK_CUTSCENES = 8,
	SAVE_CHUNK_GAME_SETTINGS = 9,
	SAVE_CHUNK_SPECIAL_SEEDS = 10,
	SAVE_CHUNK_BESTIARY = 11,
};

enum SaveChunkDataType {
	SAVE_CHUNK_DATA_TYPE_UNKNOWN = 0,
	SAVE_CHUNK_DATA_BOOL = 1,
	SAVE_CHUNK_DATA_INT = 2,
	SAVE_CHUNK_DATA_BESTIARY = 3,
};

// Static information about a save chunk in general.
struct SaveChunkConfig {
	const char* name;
	const SaveChunkDataType dataType;
	const bool allowSyncing;
	// Some of the boolean sections have their "size" written to the file as numElements*4 instead of their actual size (numElements*1).
	const bool totalSizeInSaveIsWrong;
};
static const std::unordered_map<SaveChunk, SaveChunkConfig> SAVE_CHUNK_CONFIGS = {
	{ SAVE_CHUNK_ACHIEVEMENTS,		{ "Achievements",	SAVE_CHUNK_DATA_BOOL,		/*allowSyncing=*/true,	/*totalSizeInSaveIsWrong=*/false	} },
	{ SAVE_CHUNK_EVENT_COUNTERS,	{ "Event Counters",	SAVE_CHUNK_DATA_INT,		/*allowSyncing=*/true,	/*totalSizeInSaveIsWrong=*/false	} },
	{ SAVE_CHUNK_LEVEL_COUNTERS,	{ "Level Counters",	SAVE_CHUNK_DATA_INT,		/*allowSyncing=*/true,	/*totalSizeInSaveIsWrong=*/true		} },
	{ SAVE_CHUNK_COLLECTIBLES,		{ "Collectibles",	SAVE_CHUNK_DATA_BOOL,		/*allowSyncing=*/true,	/*totalSizeInSaveIsWrong=*/true		} },
	{ SAVE_CHUNK_MINIBOSSES,		{ "Minibosses",		SAVE_CHUNK_DATA_BOOL,		/*allowSyncing=*/true,	/*totalSizeInSaveIsWrong=*/true		} },
	{ SAVE_CHUNK_BOSSES,			{ "Bosses",			SAVE_CHUNK_DATA_BOOL,		/*allowSyncing=*/true,	/*totalSizeInSaveIsWrong=*/true		} },
	{ SAVE_CHUNK_CHALLENGES,		{ "Challenges",		SAVE_CHUNK_DATA_BOOL,		/*allowSyncing=*/true,	/*totalSizeInSaveIsWrong=*/true		} },
	{ SAVE_CHUNK_CUTSCENES,			{ "Cutscenes",		SAVE_CHUNK_DATA_INT,		/*allowSyncing=*/true,	/*totalSizeInSaveIsWrong=*/false	} },
	{ SAVE_CHUNK_GAME_SETTINGS,		{ "Settings",		SAVE_CHUNK_DATA_INT,		/*allowSyncing=*/false,	/*totalSizeInSaveIsWrong=*/false	} },
	{ SAVE_CHUNK_SPECIAL_SEEDS,		{ "Special Seeds",	SAVE_CHUNK_DATA_BOOL,		/*allowSyncing=*/true,	/*totalSizeInSaveIsWrong=*/true		} },
	{ SAVE_CHUNK_BESTIARY,			{ "Bestiary",		SAVE_CHUNK_DATA_BESTIARY,	/*allowSyncing=*/true,	/*totalSizeInSaveIsWrong=*/false	} },
};

// Information about a save chunk as it exists in an actual save file.
struct SaveChunkDesc {
	int32_t sizeWritten = -1;  // The "size" of the chunk's data written into the save file. May be inaccurate depending on the chunk.
	int32_t actualSize = -1;  // The true size of the chunk's data, in bytes.
	int32_t numElements = -1;  // The number of elements stored in the chunk data (ie, the number of achievements/counters).
	uint32_t dataLocation = 0;  // Offset location of the start of the chunk's data, to easily seek to it later.

	bool IsValid() const {
		return sizeWritten > -1 && actualSize > -1 && numElements > -1 && dataLocation > 0;
	}
};
struct BestiarySectionDesc : public SaveChunkDesc {
	std::map<int32_t, int32_t> entries;
};

enum SaveSyncMode {
	SAVE_SYNC_NONE = 0,
	// For each achievement/counter etc shared between the two saves, take the maximum of the two values.
	SAVE_SYNC_TAKE_MAX = 1,
	// For each achievement/counter etc shared between the two saves, overwrite the values in the "destination" save with those from the "source" save.
	SAVE_SYNC_OVERWRITE = 2,
};

// Class used for validating and syncing PersistentGameData save files.
class SaveFile {
  public:
	SaveFile() = delete;

	SaveFile(const char* path, const bool steamCloud) : _filePath(path), _steamCloud(steamCloud) {
		ReadSaveFile();
	}

	// Set the current read/write position for the buffer.
	// Returns false if the attempt would go out of bounds.
	bool SetPosition(long pos) const;

	// Move the current read/write position for the buffer by the given amount.
	// Returns false if the attempt would go out of bounds.
	bool MovePosition(long offset) const;

	// Moves the read/write position akin to fseek.
	// Returns false if the attempt would go out of bounds.
	// origin: SEEK_SET, SEEK_CUR, SEEK_END
	bool Seek(long offset, int origin) const;

	// Returns the current read/write position for the buffer.
	inline uint32_t GetPosition() const {
		return _rawDataPosition;
	}

	// Returns true if the read/write position is at the end of the buffer.
	inline bool AtEndOfFile() const {
		return _rawDataPosition == _rawDataSize;
	}

	// Generic template to read bytes from the buffer, from the current position.
	template<typename T>
	bool Read(T* out, const int32_t num = 1) const {
		const uint32_t numBytes = num * sizeof(T);
		if (_rawDataPosition + numBytes > _rawDataSize) {
			ZHL::Log("[SaveFile] Attempt to read outside the bounds of the buffer!\n");
			return false;
		}
		if (num > 0) {
			memcpy(out, _rawData.get() + _rawDataPosition, numBytes);
			SetPosition(_rawDataPosition + numBytes);
		}
		return true;
	}

	// Generic template to write bytes into the buffer, from the current position.
	template<typename T>
	bool Write(T* in, const int32_t num = 1) {
		const uint32_t numBytes = num * sizeof(T);
		if (_rawDataPosition + numBytes > _rawDataSize) {
			ZHL::Log("[SaveFile] Attempt to write outside the bounds of the buffer!\n");
			return false;
		}
		if (num > 0) {
			_isValid = false;
			memcpy(_rawData.get() + _rawDataPosition, in, numBytes);
			SetPosition(_rawDataPosition + numBytes);
		}
		return true;
	}

	// Generates a KAGE checksum for the current contents of the raw data buffer.
	// ignoreCounter excludes the "counter" from the checksum, as its safe to ignore it for the purposes of determining sync status.
	uint32_t GenerateChecksum(bool ignoreCounter) const;

	// Regenerates the checksum for the contents of the file, then writes the updated checksum into the raw data.
	// Returns false if writing the checksum fails.
	bool UpdateChecksum();

	// Reads the save at _filePath and loads the contents into _rawData.
	bool ReadSaveFile();

	// Returns true if a file was successfully opened, even if some other issue occurred afterwards.
	inline bool OpenedFile() {
		return _openedFile;
	}

	// Parses the raw save data to validate the structure of it meets all expectations,
	// and (re)populates stuff like _saveChunkDescs and _bestiarySectionDescs.
	bool ParseAndValidateData(bool isInitialRead);

	// Returns true if the save file passed validations the last time they were run.
	// Validations are typically run after first reading the file, after syncing data, and right before saving.
	inline bool IsValid() const {
		return _isValid;
	}

	// Returns true if a save chunk with the provided ID has been successfully parsed.
	bool HasValidSaveChunk(SaveChunk chunkID) const;

	// Returns the desired SaveChunkDesc, or nullptr if none exists.
	const SaveChunkDesc* GetSaveChunkDesc(SaveChunk chunkID) const;

	// Returns true if data has been parsed for the indiciated bestiary section.
	bool HasBestiarySectionDesc(int32_t bestiarySectionID) const;

	// Returns the desired BestiarySectionDesc, or nullptr if none exists.
	const BestiarySectionDesc* GetBestiarySectionDesc(int32_t bestiarySectionID) const;

	// Reads the current value of the "counter" towards the end of the save file.
	// Seemingly tracks how many times the file was loaded by the game.
	bool ReadCounter(uint32_t* out) const;

	// Sync achievements/counters/etc from another save file into this one, with the behaviour indicated by the SaveSyncMode.
	// Only achievements/counters/etc that exist in both files are synced, so syncing files with different amounts of achievements is safe.
	// Returns false if synchronization fails.
	bool Sync(const SaveFile& srcFile, SaveSyncMode mode, const std::optional<uint32_t> overrideGamestateChecksum = std::nullopt);

	// Save a file to the specified location, overwriting any existing file.
	// Returns false if writing fails.
	bool SaveLocal(const std::string& filepath);

	// Save to the steam cloud with the specified filename.
	// Return false if the steam cloud is not connected, or writing fails.
	bool SaveToSteamCloud(const std::string& filename);

	// Save to the same file/location that the save was originally read from.
	bool Save();

	// Returns the path of the original file that was loaded.
	inline const std::string& GetFilePath() const {
		return _filePath;
	}

	// Returns the header string from the save file (ie, "ISAACNGSAVE09R  ").
	inline const std::string& GetHeader() const {
		return _header;
	}

	// Returns a pointer to the raw data read from the save file (potentially modified by syncing).
	inline const char* GetRawData() const {
		return _rawData.get();
	}

	// Returns the size of the current raw save data in bytes.
	inline uint32_t GetSize() const {
		return _rawDataSize;
	}

  private:
	void ClearRawDataBuffer();

	// Initializes the _rawData buffer to the specified size.
	void InitRawDataBuffer(uint32_t size);

	// Unique syncing logic for the Bestiary data.
	bool SyncBestiaryData(const SaveFile& src, SaveSyncMode mode);

	// Syncs the data for a particular chunk.
	bool SyncChunk(const SaveFile& srcFile, SaveChunk chunkID, SaveSyncMode mode);

	// Sets the gamestate checksum to the given value.
	bool SetGamestateChecksum(uint32_t value);

	// Modify the counter value near the end of the raw data.
	bool SetCounter(uint32_t value);

	// Syncs the counter value between two saves.
	bool SyncCounter(const SaveFile& srcFile, const SaveSyncMode mode);


	// The path of the input file read.
	const std::string _filePath;

	// Whether the path corresponds to a steam cloud file.
	// Steam cloud file paths are not full file paths - they are typically just the name of the file.
	const bool _steamCloud;

	// The raw data read from the save file, to be potentially modified and then saved back to disk.
	std::unique_ptr<char[]> _rawData;
	uint32_t _rawDataSize = 0;
	mutable uint32_t _rawDataPosition = 0;

	// Some save information parsed from the raw data during ParseAndValidateData.
	std::string _header;
	std::unordered_map<SaveChunk, SaveChunkDesc> _saveChunkDescs;
	std::unordered_map<uint32_t, BestiarySectionDesc> _bestiarySectionDescs;

	// Set to true if a file was successfully opened, regardless of validations.
	bool _openedFile = false;

	// Set to true if all validations passed last time ParseAndValidateData ran.
	// Writes and failed syncs set this back to false.
	bool _isValid = false;
};
