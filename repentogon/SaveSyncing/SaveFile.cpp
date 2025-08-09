#include "SaveFile.h"

#include <string>
#include <filesystem>
#include <vector>

#include "IsaacRepentance.h"
#include "Log.h"
#include <optional>

namespace {

// Sync the raw data between two files for a given chunk and write to the dstFile, interpreting the chunk data elements as type T.
template<typename T>
bool SyncData(SaveFile& dstFile, const SaveFile& srcFile, const SaveChunk chunkID, const SaveSyncMode mode) {
	const SaveChunkDesc* srcChunk = srcFile.GetSaveChunkDesc(chunkID);
	const SaveChunkDesc* dstChunk = dstFile.GetSaveChunkDesc(chunkID);

	if (!srcChunk || !dstChunk || !srcChunk->IsValid() || !dstChunk->IsValid()) {
		ZHL::Log("[SaveFile] Sync error: Cannot sync data for save chunk %d - SaveChunkDesc not properly initialized!\n", chunkID);
		return false;
	}

	if (srcChunk->actualSize != srcChunk->numElements * sizeof(T) || dstChunk->actualSize != dstChunk->numElements * sizeof(T)) {
		ZHL::Log("[SaveFile] Sync error: Cannot interpret data of chunk %d as type %s!\n", chunkID, typeid(T).name());
		return false;
	}

	// Sync only the elements shared on both sides.
	const int32_t sharedElements = std::min(srcChunk->numElements, dstChunk->numElements);

	if (!srcFile.Seek(srcChunk->dataLocation, SEEK_SET) || !dstFile.Seek(dstChunk->dataLocation, SEEK_SET)) {
		ZHL::Log("[SaveFile] Failed to seek to start of data for chunk %d.\n", chunkID);
		return false;
	}

	std::vector<T> data;
	data.reserve(sharedElements);

	for (int i = 0; i < sharedElements; i++) {
		T srcVal = 0;
		T dstVal = 0;
		if (!srcFile.Read(&srcVal) || !dstFile.Read(&dstVal)) {
			ZHL::Log("[SaveFile] Read error while syncing save data for chunk %d.\n", chunkID);
			return false;
		}
		if (mode == SAVE_SYNC_OVERWRITE) {
			data.push_back(srcVal);
		} else if (mode == SAVE_SYNC_TAKE_MAX) {
			data.push_back(std::max(srcVal, dstVal));
		} else {
			ZHL::Log("[SaveFile] Sync error: Unrecognized SaveSyncMode %d\n", mode);
			return false;
		}
	}

	if (data.size() != sharedElements) {
		ZHL::Log("[SaveFile] Sync error: Did not read expected number of shared elements for chunk %d!\n", chunkID);
		return false;
	}

	if (chunkID == SAVE_CHUNK_EVENT_COUNTERS) {
		// Make sure syncing didn't give a streak and a negative streak at the same time.
		// If this happens, the positive streak takes priority.
		constexpr int streakCounter = 22;
		constexpr int negativeStreakCounter = 113;
		if (data.size() > negativeStreakCounter && data[streakCounter] > 0 && data[negativeStreakCounter] > 0) {
			data[negativeStreakCounter] = 0;
		}
	}

	if (!dstFile.Seek(dstChunk->dataLocation, SEEK_SET)) {
		ZHL::Log("[SaveFile] Failed to seek before writing sync result for chunk %d.\n", chunkID);
		return false;
	}
	if (!dstFile.Write<T>(data.data(), sharedElements)) {
		ZHL::Log("[SaveFile] Failed to write sync result for chunk %d.\n", chunkID);
		return false;
	}

	return true;
}

}  // namespace

bool SaveFile::SetPosition(const long pos) const {
	_rawDataPosition = pos;
	if (_rawDataPosition > _rawDataSize) {
		_rawDataPosition = _rawDataSize;
		return false;
	} else if (_rawDataPosition < 0) {
		_rawDataPosition = 0;
		return false;
	}
	return true;
}

bool SaveFile::MovePosition(const long offset) const {
	return SetPosition(_rawDataPosition + offset);
}

bool SaveFile::Seek(const long offset, const int origin) const {
	if (origin == SEEK_SET) {
		return SetPosition(offset);
	} else if (origin == SEEK_CUR) {
		return MovePosition(offset);
	} else if (origin == SEEK_END) {
		return SetPosition(_rawDataSize + offset);
	}
	return false;
}

uint32_t SaveFile::GenerateChecksum(const bool ignoreGamestateAndCounter) const {
	// Checksum::Flush is inlined and I was having trouble recreating it, so I use Checksum::Generate instead, which takes in a KAGE stream.
	// Using a fake SteamCloudFile like this is a little hacky but it works perfectly fine.
	SteamCloudFile file;
	file._buffer = _rawData.get();
	file._bufferSize = _rawDataSize;
	file._size = _rawDataSize;
	file._status = 0;

	// Normally the checksum starts after the 16-byte header, and ends before where the actual checksum is written.
	// For save synchronization tracking we can also ignore the gamestate checksum and the counter.
	const int start = ignoreGamestateAndCounter ? 20 : 16;
	const int endOffset = ignoreGamestateAndCounter ? 8 : 4;
	const uint32_t checksum = Checksum::Generate(0, &file, start, endOffset);

	// Clear the SteamCloudFile to prevent the destructor from doing anything weird.
	file._buffer = nullptr;
	file._bufferSize = 0;
	file._size = 0;
	file._status = 2;

	return checksum;
}

bool SaveFile::UpdateChecksum() {
	const uint32_t checksum = GenerateChecksum(false);
	if (!Seek(-4, SEEK_END)) {
		ZHL::Log("[SaveFile] Failed to seek to checksum.\n");
		return false;
	}
	if (!Write(&checksum)) {
		ZHL::Log("[SaveFile] Failed to write updated checksum.\n");
		return false;
	}
	ZHL::Log("[SaveFile] New checksum: %u\n", checksum);
	return true;
}

void SaveFile::ClearRawDataBuffer() {
	_rawData = nullptr;
	_rawDataSize = 0;
	_rawDataPosition = 0;
	_isValid = false;
}

void SaveFile::InitRawDataBuffer(uint32_t size) {
	ClearRawDataBuffer();
	_rawDataSize = size;
	_rawData = std::make_unique<char[]>(_rawDataSize + 1);
	_rawData[_rawDataSize] = '\0';
}

bool SaveFile::ReadSaveFile() {
	_openedFile = false;
	ClearRawDataBuffer();

	if (_filePath.empty()) {
		ZHL::Log("[SaveFile] Cannot read save file - filePath is empty!\n");
		return false;
	}
	ZHL::Log("[SaveFile] Reading save file @ %s\n", _filePath.c_str());

	std::unique_ptr<KAGE_Filesys_FileStream> file;

	if (_steamCloud) {
		file = std::make_unique<SteamCloudFile>();
	} else {
		file = std::make_unique<KAGE_Filesys_File>();
	}

	_openedFile = file->OpenRead(_filePath.c_str()) && file->IsOpen();

	if (!_openedFile) {
		ZHL::Log("[SaveFile] Failed to open save file for reading.\n");
		return false;
	}

	const long size = file->GetSize();

	if (size > MAX_SAVE_FILE_SIZE) {
		ZHL::Log("[SaveFile] Attempted to read a save file that is %ld bytes. What have you done? Aborting.", size);
		return false;
	}

	InitRawDataBuffer(size);
	file->Read(_rawData.get(), 1, _rawDataSize);

	return ParseAndValidateData(true);
}

bool SaveFile::ParseAndValidateData(const bool isInitialRead) {
	ZHL::Log("[SaveFile] Attempting to parse and validate save file...\n");

	_isValid = false;
	_saveChunkDescs.clear();
	_bestiarySectionDescs.clear();

	if (!_openedFile || _rawDataSize <= 0 || !_rawData) {
		ZHL::Log("[SaveFile] Cannot validate - no data!\n");
		return false;
	}

	if (!Seek(0, SEEK_SET)) {
		ZHL::Log("[SaveFile] Failed to seek to start of save file.\n");
		return false;
	}

	char header[0x11];
	if (!Read(header, 0x10)) {
		ZHL::Log("[SaveFile] Failed to read save file header.\n");
		return false;
	}
	header[0x10] = '\0';
	if (isInitialRead) ZHL::Log("[SaveFile] Save header: %s\n", header);

	_header = std::string(header);

	if (strcmp(header, EXPECTED_SAVE_HEADER) != 0) {
		ZHL::Log("[SaveFile] Unrecognized save file header (expected \"%s\", got \"%s\")\n", EXPECTED_SAVE_HEADER, header);
		return false;
	}

	uint32_t gameStateChecksum;
	if (!Read(&gameStateChecksum)) {
		ZHL::Log("[SaveFile] Failed to read GameState checksum.\n");
		return false;
	}
	if (isInitialRead) ZHL::Log("[SaveFile] GameState checksum: %u\n", gameStateChecksum);

	for (int i = 1; i <= NUM_SAVE_CHUNKS; i++) {
		int rawChunkID = 0;
		if (!Read(&rawChunkID)) {
			ZHL::Log("[SaveFile] Failed to read chunk ID (was expecting %d)\n", i);
			return false;
		}
		if (rawChunkID != i || !IsValidSaveChunkID(rawChunkID)) {
			ZHL::Log("[SaveFile] Unexpected chunk ID %d (was expecting %d)\n", rawChunkID, i);
			return false;
		}
		const SaveChunk chunkID = (SaveChunk)rawChunkID;

		if (chunkID != i) {
			ZHL::Log("[SaveFile] Chunk %d read out of expected iteration order (was expecting %d)\n", chunkID, i);
			return false;
		}

		const SaveChunkConfig* chunkConfig = &SAVE_CHUNK_CONFIGS.at(chunkID);

		if (isInitialRead) ZHL::Log("[SaveFile] Parsing save chunk %s (%d) @ %ld...\n", chunkConfig->name, chunkID, GetPosition() + 8);

		SaveChunkDesc& chunkDesc = _saveChunkDescs[chunkID];

		if (!Read(&chunkDesc.sizeWritten)) {
			ZHL::Log("[SaveFile] Failed to read chunk sizeWritten\n");
			return false;
		}
		if (chunkDesc.sizeWritten <= 0 && chunkID != SAVE_CHUNK_BESTIARY) {
			ZHL::Log("[SaveFile] Unexpected value for sizeWritten %d\n", chunkDesc.sizeWritten);
			return false;
		}

		if (!Read(&chunkDesc.numElements)) {
			ZHL::Log("[SaveFile] Failed to read numElements\n");
			return false;
		}
		if (chunkDesc.numElements <= 0) {
			ZHL::Log("[SaveFile] Unexpected value for numElements %d\n", chunkDesc.numElements);
			return false;
		}

		// Store the position of the chunk's data within the file.
		chunkDesc.dataLocation = GetPosition();

		if (chunkID == SAVE_CHUNK_BESTIARY) {
			// The bestiary's format is more complicated and does not have a fixed size, so we'll need to parse it.
			chunkDesc.actualSize = 0;
			uint32_t bestiarySectionDataSizeSum = 0;

			// 5 is written here in AB+, but there is still only 4 bestiary sections.
			// If nicalis were to somehow add another bestiary section in the future, we'd fail our validations elsewhere anyway, so no big deal.
			if (chunkDesc.numElements != 4 && chunkDesc.numElements != 5) {
				ZHL::Log("[SaveFile] Read unexpected value for the number of bestiary sections (4 or 5 expected, got %d)\n", chunkDesc.numElements);
				return false;
			}

			for (const int32_t j : EXPECTED_BESTIARY_SECTION_ORDER) {
				int32_t bestiarySectionID = 0;
				if (!Read(&bestiarySectionID)) {
					ZHL::Log("[SaveFile] Failed to read bestiary section ID\n");
					return false;
				}
				if (bestiarySectionID != j) {
					ZHL::Log("[SaveFile] Expected bestiary section ID %d, got %d\n", j, bestiarySectionID);
					return false;
				}

				// The value written here is the numEntries*4, even though the actual data size is numEntries*8.
				int32_t bestiarySectionSizeWritten = 0;
				if (!Read(&bestiarySectionSizeWritten)) {
					ZHL::Log("[SaveFile] Failed to read bestiarySectionSize\n");
					return false;
				}
				if (bestiarySectionSizeWritten % 4 != 0) {
					ZHL::Log("[SaveFile] Error: bestiarySectionSizeWritten is not divisible by 4\n");
					return false;
				}

				const int32_t numBestiarySectionEntries = bestiarySectionSizeWritten / 4;
				const int32_t actualBestiarySectionSize = numBestiarySectionEntries * 8;

				BestiarySectionDesc& bestiarySectionDesc = _bestiarySectionDescs[bestiarySectionID];
				bestiarySectionDesc.sizeWritten = bestiarySectionSizeWritten;
				bestiarySectionDesc.actualSize = actualBestiarySectionSize;
				bestiarySectionDesc.numElements = numBestiarySectionEntries;
				bestiarySectionDesc.dataLocation = GetPosition();

				// The section can be empty if no monsters have been encountered/killed/etc.
				for (int b = 0; b < numBestiarySectionEntries; b++) {
					int32_t key = 0;
					if (!Read(&key)) {
						ZHL::Log("[SaveFile] Failed to read bestiary entity key\n");
						return false;
					}
					int32_t value = 0;
					if (!Read(&value)) {
						ZHL::Log("[SaveFile] Failed to read bestiary entity value\n");
						return false;
					}
					bestiarySectionDesc.entries[key] = value;
				}
				if (isInitialRead) ZHL::Log("[SaveFile] Read bestiary section %d with size %d (%d written in file)\n", bestiarySectionID, actualBestiarySectionSize, bestiarySectionSizeWritten);
				chunkDesc.actualSize += actualBestiarySectionSize + 8;
				bestiarySectionDataSizeSum += actualBestiarySectionSize;
			}

			if (chunkDesc.sizeWritten * 2 != bestiarySectionDataSizeSum) {
				ZHL::Log("[SaveFile] Bestiary sizeWritten %d expected to be half of section data sum %d, but it is not!\n", chunkDesc.sizeWritten, bestiarySectionDataSizeSum);
				return false;
			}
		} else {
			const int32_t dataElementSize = chunkConfig->dataType == SAVE_CHUNK_DATA_BOOL ? 1 : 4;
			chunkDesc.actualSize = chunkDesc.numElements * dataElementSize;
			const int32_t expectedSizeWritten = chunkConfig->totalSizeInSaveIsWrong ? (chunkDesc.numElements * 4) : chunkDesc.actualSize;

			if (chunkDesc.sizeWritten != expectedSizeWritten) {
				ZHL::Log("[SaveFile] Unexpected chunk size written in file (expected %d, got %d)\n", expectedSizeWritten, chunkDesc.sizeWritten);
				return false;
			}

			if (chunkDesc.actualSize / chunkDesc.numElements != dataElementSize) {
				ZHL::Log("[SaveFile] Unexpected result dividing true total chunk size by the number of elements (expected %d, got %d)\n", dataElementSize, chunkDesc.actualSize / chunkDesc.numElements);
				return false;
			}

			if (!Seek(chunkDesc.actualSize, SEEK_CUR)) {
				ZHL::Log("[SaveFile] Failed to seek past chunk data.\n");
				return false;
			}
		}

		if (!chunkDesc.IsValid()) {
			ZHL::Log("[SaveFile] Failed to parse chunk\n");
			return false;
		}

		if (isInitialRead) ZHL::Log("[SaveFile] - sizeWritten %d, actualSize %d bytes, numElements %d\n", chunkDesc.sizeWritten, chunkDesc.actualSize, chunkDesc.numElements);
	}

	uint32_t counter = 0;
	if (!Read(&counter)) {
		ZHL::Log("[SaveFile] Failed to read the counter at the end of the file.\n");
		return false;
	}
	if (isInitialRead) ZHL::Log("[SaveFile] Counter: %u\n", counter);

	const uint32_t checksumLocation = GetPosition();
	uint32_t checksum = 0;
	if (!Read(&checksum)) {
		ZHL::Log("[SaveFile] Failed to read checksum.\n");
		return false;
	}
	const uint32_t expectedChecksum = GenerateChecksum(false);
	if (checksum != expectedChecksum) {
		ZHL::Log("[SaveFile] Checksum in file %u does not match expected value %u\n", checksum, expectedChecksum);
		return false;
	}
	if (isInitialRead) ZHL::Log("[SaveFile] Checksum: %u\n", checksum);

	if (!AtEndOfFile()) {
		ZHL::Log("[SaveFile] Expected end of file, instead %d bytes remaining!\n", GetSize() - GetPosition());
		return false;
	}

	ZHL::Log("[SaveFile] Save file successfully parsed & validated!\n");
	_isValid = true;
	return true;
}

bool SaveFile::HasValidSaveChunk(const SaveChunk chunkID) const {
	if (IsValidSaveChunkID(chunkID) && _saveChunkDescs.find(chunkID) != _saveChunkDescs.end()) {
		return _saveChunkDescs.at(chunkID).IsValid();
	}
	return false;
}
const SaveChunkDesc* SaveFile::GetSaveChunkDesc(const SaveChunk chunkID) const {
	if (HasValidSaveChunk(chunkID)) {
		return &_saveChunkDescs.at(chunkID);
	}
	return nullptr;
}

bool SaveFile::HasBestiarySectionDesc(const int32_t bestiarySectionID) const {
	return bestiarySectionID >= 1 && bestiarySectionID <= 4 && _bestiarySectionDescs.find(bestiarySectionID) != _bestiarySectionDescs.end();
}
const BestiarySectionDesc* SaveFile::GetBestiarySectionDesc(const int32_t bestiarySectionID) const {
	if (HasBestiarySectionDesc(bestiarySectionID)) {
		return &_bestiarySectionDescs.at(bestiarySectionID);
	}
	return nullptr;
}

bool SaveFile::SyncBestiaryData(const SaveFile& srcFile, const SaveSyncMode mode) {
	if (!HasValidSaveChunk(SAVE_CHUNK_BESTIARY) || !srcFile.HasValidSaveChunk(SAVE_CHUNK_BESTIARY)) {
		ZHL::Log("[SaveFile] Bestiary sync error: SaveChunkDesc not properly initialized!\n");
		return false;
	}

	// Sync the contents of this save's bestiary data maps with those from the other save file.

	for (const int32_t bestiarySectionID : EXPECTED_BESTIARY_SECTION_ORDER) {
		if (!HasBestiarySectionDesc(bestiarySectionID) || !srcFile.HasBestiarySectionDesc(bestiarySectionID)) {
			ZHL::Log("[SaveFile] Bestiary sync error: Section %d not found!\n", bestiarySectionID);
			return false;
		}

		const BestiarySectionDesc* src = srcFile.GetBestiarySectionDesc(bestiarySectionID);
		BestiarySectionDesc& dst = _bestiarySectionDescs[bestiarySectionID];

		if (!dst.IsValid() || !src || !src->IsValid()) {
			ZHL::Log("[SaveFile] Bestiary sync error: Invalid bestiarySectionDesc!\n");
			return false;
		}

		if (mode == SAVE_SYNC_OVERWRITE) {
			// Just copy the entire contents.
			dst.entries = src->entries;
		} else if (mode == SAVE_SYNC_TAKE_MAX) {
			// Merge the contents of both bestiaries together, taking the max values.
			for (const auto& [key, value] : src->entries) {
				if (dst.entries.find(key) != dst.entries.end()) {
					dst.entries[key] = std::max(dst.entries[key], value);
				} else {
					dst.entries[key] = value;
				}
			}
		} else {
			ZHL::Log("[SaveFile] Bestiary sync error: Unrecognized SaveSyncMode: %d\n", mode);
			return false;
		}
	}

	// Now we need to write the new bestiary contents to the raw data.
	// This is the only save chunk with variable size, so we'll need to resize the buffer.

	SaveChunkDesc& bestiaryDesc = _saveChunkDescs[SAVE_CHUNK_BESTIARY];
	const uint32_t bestiaryDataLocation = bestiaryDesc.dataLocation;

	uint32_t totalBestiarySectionDataSize = 0;
	for (const int32_t bestiarySectionID : EXPECTED_BESTIARY_SECTION_ORDER) {
		totalBestiarySectionDataSize += _bestiarySectionDescs[bestiarySectionID].entries.size() * 8;  // Key and value ints for each monster.
	}

	// "Size" written for the bestiary chunk is half of the summed size of the section data, excluding their own IDs and sizes.
	bestiaryDesc.sizeWritten = totalBestiarySectionDataSize / 2;
	// Include the ID/size values for each section in the true total size of the bestiary chunk data.
	bestiaryDesc.actualSize = totalBestiarySectionDataSize + EXPECTED_BESTIARY_SECTION_ORDER.size() * 8;

	// Snatch the old raw data buffer.
	std::unique_ptr<char[]> oldBuffer = std::move(_rawData);
	const uint32_t oldBufferSize = _rawDataSize;

	// Initialize a new buffer with the appropriate size.
	InitRawDataBuffer(bestiaryDataLocation + bestiaryDesc.actualSize + 8);

	// Write the old buffer's contents into the new buffer, up to the bestiary data.
	if (!Write(oldBuffer.get(), bestiaryDataLocation - 8)) {
		ZHL::Log("[SaveFile] Bestiary sync error: Failed to write old buffer contents to new buffer.\n");
		return false;
	}

	// Write the updated size for the bestiary chunk.
	if (!Write(&bestiaryDesc.sizeWritten) || !Write(&bestiaryDesc.numElements)) {
		ZHL::Log("[SaveFile] Bestiary sync error: Failed to write bestiary chunk size/numElements.\n");
		return false;
	}

	// Write the bestiary sections.
	for (const int bestiarySectionID : EXPECTED_BESTIARY_SECTION_ORDER) {
		int sectionSizeToWrite = _bestiarySectionDescs[bestiarySectionID].entries.size() * 4;
		if (!Write(&bestiarySectionID) || !Write(&sectionSizeToWrite)) {
			ZHL::Log("[SaveFile] Bestiary sync error: Failed to write bestiary section ID/size.\n");
			return false;
		}
		for (const auto& [key, value] : _bestiarySectionDescs[bestiarySectionID].entries) {
			if (!Write(&key) || !Write(&value)) {
				ZHL::Log("[SaveFile] Bestiary sync error: Failed to write monster key/value.\n");
				return false;
			}
		}
	}

	// Write the counter/checksum from the old buffer.
	if (!Write(oldBuffer.get() + oldBufferSize - 8, 8)) {
		ZHL::Log("[SaveFile] Bestiary sync error: Failed to write counter/checksum from old buffer.\n");
		return false;
	}

	if (!AtEndOfFile()) {
		ZHL::Log("[SaveFile] Bestiary sync error: Expected end of file, instead %d bytes remaining!\n", GetSize() - GetPosition());
		return false;
	}

	return true;
}

bool SaveFile::SyncChunk(const SaveFile& srcFile, const SaveChunk chunkID, const SaveSyncMode mode) {
	if (!IsValidSaveChunkID(chunkID)) {
		ZHL::Log("[SaveFile] Cannot sync data for unknown chunk ID: %d\n", chunkID);
		return false;
	}

	const SaveChunkConfig* chunkConfig = &SAVE_CHUNK_CONFIGS.at(chunkID);

	if (!chunkConfig->allowSyncing && mode != SAVE_SYNC_OVERWRITE) {
		ZHL::Log("[SaveFile] Skipping %s...\n", chunkConfig->name);
		return true;
	}
	ZHL::Log("[SaveFile] Syncing %s...\n", chunkConfig->name);

	if (chunkConfig->dataType == SAVE_CHUNK_DATA_BESTIARY) {
		return SyncBestiaryData(srcFile, mode);
	} else if (chunkConfig->dataType == SAVE_CHUNK_DATA_INT) {
		return SyncData<int32_t>(*this, srcFile, chunkID, mode);
	} else if (chunkConfig->dataType == SAVE_CHUNK_DATA_BOOL) {
		return SyncData<int8_t>(*this, srcFile, chunkID, mode);
	}

	return false;
}

bool SaveFile::ReadGamestateChecksum(uint32_t* out) const {
	if (!Seek(16, SEEK_SET)) {
		ZHL::Log("[SaveFile] Failed to seek to gamestate checksum for reading.\n");
		return false;
	}
	if (!Read(out)) {
		ZHL::Log("[SaveFile] Failed to read gamestate checksum.\n");
		return false;
	}
	return true;
}

bool SaveFile::SetGamestateChecksum(const uint32_t value) {
	if (!Seek(16, SEEK_SET)) {
		ZHL::Log("[SaveFile] Failed to seek to gamestate checksum for writing.\n");
		return false;
	}
	if (!Write(&value)) {
		ZHL::Log("[SaveFile] Failed to write updated gamestate checksum.\n");
		return false;
	}
	// Update the save checksum and re-run validations.
	if (!UpdateChecksum()) {
		ZHL::Log("[SaveFile] Failed to update save checksum after updating GameState checksum!\n");
		return false;
	}
	if (!ParseAndValidateData(false)) {
		ZHL::Log("[SaveFile] Post-GameState checksum update validation failed!\n");
		return false;
	}
	return true;
}

bool SaveFile::ReadCounter(uint32_t* out) const {
	if (!Seek(-8, SEEK_END)) {
		ZHL::Log("[SaveFile] Failed to seek to counter for reading.\n");
		return false;
	}
	if (!Read(out)) {
		ZHL::Log("[SaveFile] Failed to read counter.\n");
		return false;
	}
	return true;
}

bool SaveFile::SetCounter(const uint32_t value) {
	if (!Seek(-8, SEEK_END)) {
		ZHL::Log("[SaveFile] Failed to seek to counter for writing.\n");
		return false;
	}
	if (!Write(&value)) {
		ZHL::Log("[SaveFile] Failed to write updated counter.\n");
		return false;
	}
	return true;
}

bool SaveFile::SyncCounter(const SaveFile& srcFile, const SaveSyncMode mode) {
	uint32_t srcCounter = 0;
	if (!srcFile.ReadCounter(&srcCounter)) {
		ZHL::Log("[SaveFile] Counter sync error: Failed to read counter from source file.\n");
		return false;
	}

	uint32_t dstCounter = 0;
	if (!ReadCounter(&dstCounter)) {
		ZHL::Log("[SaveFile] Counter sync error: Failed to read counter from destination file.\n");
		return false;
	}

	uint32_t newCounter = 0;

	if (mode == SAVE_SYNC_OVERWRITE) {
		newCounter = srcCounter;
	} else if (mode == SAVE_SYNC_TAKE_MAX) {
		newCounter = std::max(dstCounter, srcCounter);
	} else {
		ZHL::Log("[SaveFile] Counter sync error: Unrecognized SaveSyncMode: %d\n", mode);
		return false;
	}

	if (!SetCounter(newCounter)) {
		ZHL::Log("[SaveFile] Counter sync error: Failed to update counter.\n");
		return false;
	}

	return true;
}

bool SaveFile::Sync(const SaveFile& srcFile, const SaveSyncMode mode) {
	if (!IsValid() || !srcFile.IsValid()) {
		ZHL::Log("[SaveFile] Cannot sync saves - validation failed.\n");
		return false;
	}

	if (GetHeader() != srcFile.GetHeader()) {
		ZHL::Log("[SaveFile] Cannot sync saves - header mismatch.\n");
		return false;
	}

	ZHL::Log("[SaveFile] Syncing `%s` with `%s`...\n", GetFilePath().c_str(), srcFile.GetFilePath().c_str());

	// If syncing fails midway, the raw data is most likely rendered invalid.
	_isValid = false;

	// Sync the main save chunks.
	for (int i = 1; i <= NUM_SAVE_CHUNKS; i++) {
		if (!SyncChunk(srcFile, (SaveChunk)i, mode)) {
			return false;
		}
	}

	// Sync the "counter" value.
	if (!SyncCounter(srcFile, mode)) {
		return false;
	}

	// Now that everything else is updated, generate a new checksum and write it into the data.
	if (!UpdateChecksum()) {
		ZHL::Log("[SaveFile] Failed to update save checksum after sync!\n");
		return false;
	}

	// Run validations on the updated data.
	if (!ParseAndValidateData(false)) {
		ZHL::Log("[SaveFile] Post-sync validation failed!\n");
		return false;
	}

	ZHL::Log("[SaveFile] Sync completed!\n");
	return true;
}

bool SaveFile::SaveLocal(const std::string& filepath) {
	if (!IsValid()) {
		ZHL::Log("[SaveFile] Cannot save invalid save data!\n");
		return false;
	}
	KAGE_Filesys_File file;
	file.OpenWrite(filepath.c_str());
	if (!file.IsOpen()) {
		ZHL::Log("[SaveFile] Failed to open local save file for writing.\n");
		return false;
	}
	const uint32_t written = file.Write(_rawData.get(), 1, _rawDataSize);
	file.Close();
	if (written != _rawDataSize) {
		ZHL::Log("[SaveFile] Didn't write the expected number of bytes to local save file!\n");
		return false;
	}
	return true;
}

bool SaveFile::SaveToSteamCloud(const std::string& filename) {
	if (!IsValid()) {
		ZHL::Log("[SaveFile] Cannot save invalid save data to the steam cloud!\n");
		return false;
	}
	SteamCloudFile file;
	file.PushToSteamCloud(filename.c_str(), _rawDataSize);
	if (!file.IsOpen()) {
		ZHL::Log("[SaveFile] Failed to open steam cloud save file for writing.\n");
		return false;
	}
	const uint32_t written = file.Write(_rawData.get(), 1, _rawDataSize);
	file.Close();
	if (written != _rawDataSize) {
		ZHL::Log("[SaveFile] Didn't write the expected number of bytes to steam cloud save file!\n");
		return false;
	}
	return true;
}

bool SaveFile::Save() {
	if (_steamCloud) {
		return SaveToSteamCloud(_filePath);
	}
	return SaveLocal(_filePath);
}
