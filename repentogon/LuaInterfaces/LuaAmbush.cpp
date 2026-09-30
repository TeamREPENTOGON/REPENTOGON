#include "IsaacRepentance.h"

int ambushWaves = 3;
int bossAmbushWaves = 2;

MOD_EXPORT int L_Ambush_GetCurrentWave() {
	return g_Game->_ambush.currentWave;
}

MOD_EXPORT int L_Ambush_GetMaxBossChallengeWaves() {
	return bossAmbushWaves;
}

MOD_EXPORT int L_Ambush_GetMaxBossrushWaves() {
	return g_Game->_ambush.maxBossWaves;
}

MOD_EXPORT int L_Ambush_GetMaxChallengeWaves() {
	return ambushWaves;
}

void SetupAmbushData(Ambush* ambush, RNG* rng, int* index, int* subtype) {
	RoomDescriptor* descriptor = g_Game->_room->_descriptor;
	RoomConfig_Room* config = descriptor->Data;

	memcpy(rng, &ambush->rng, sizeof(*rng));

	if (ambush->currentWave == 0) {
		rng->_seed = descriptor->SpawnSeed;
	}

	*index = ambush->currentWave;
	if (g_Game->_difficulty == 1) {
		++*index;
	}

	*subtype = 10;
	if (config->Subtype == 1) {
		++*subtype;
	}

	*index = std::min(*index, 3);
}

static int AmbushDifficulty[4] = { 1, 5, 10, 15 };

#undef min
#undef max

MOD_EXPORT RoomConfig_Room* L_Ambush_GetNextWave() {
	Ambush* ambush = &g_Game->_ambush;
	RoomConfig_Room* currentRoom = g_Game->_room->_descriptor->Data;
	RNG rng;
	int index;
	int subtype;

	SetupAmbushData(ambush, &rng, &index, &subtype);

	int spawnCount = 0;
	RoomConfig_Room* config = nullptr;

	int i = 0;
	do {
		unsigned int requiredDoors = 0;
		rng.Next();
		config = g_Game->GetRoomConfig()->GetRandomRoom(rng._seed, false,
			g_Game->GetRoomConfig()->GetStageID(g_Game->_stage, g_Game->_stageType, -1),
			11 /* Challenge room */, currentRoom->Shape, 0, -1, AmbushDifficulty[index] /* ebp - 60 */,
			AmbushDifficulty[index] /* ebp - 60 */, &requiredDoors/* ebp - 4C */, subtype /* ebp - 64 */, -1);

		if (!config) {
			return nullptr;
		}

		spawnCount = config->SpawnCount;
		++i;
	} while (spawnCount == 0 && i < 10);

	if (config && i != 10) {
		return config;
	}
	else {
		return nullptr;
	}
}

MOD_EXPORT int L_Ambush_GetRemainingWaves() {
	return std::max(ambushWaves - g_Game->_ambush.currentWave, 0);
}

MOD_EXPORT int L_Ambush_GetNextWaves(RoomConfig_Room** out, int maxCount) {
	Ambush* ambush = &g_Game->_ambush;
	RoomDescriptor* descriptor = g_Game->_room->_descriptor;
	RoomConfig_Room* currentRoom = descriptor->Data;

	RNG rng;
	int index;
	int subtype;
	unsigned int doors = 0;

	SetupAmbushData(ambush, &rng, &index, &subtype);

	std::vector<std::tuple<RoomConfig_Room*, float, float>> configs;
	for (int i = ambush->currentWave; i < ambushWaves; ++i) {
		int spawnCount = 0;
		int j = 0;
		do {
			rng.Next();
			int stage = g_Game->GetRoomConfig()->GetStageID(g_Game->_stage, g_Game->_stageType, -1);	
			// Draw the room a first time without reducing its weight.
			// We need to determine what its current weight is in order to reduce it during 
			// a second draw. 
			RoomConfig_Room* config = g_Game->GetRoomConfig()->GetRandomRoom(rng._seed, false, stage, 11, currentRoom->Shape, 0, -1, AmbushDifficulty[index], AmbushDifficulty[index], &doors, subtype, -1);
			if (!config) {
				return 0;
			}
			float weight = config->Weight;
			float initial = config->InitialWeight;
			config = g_Game->GetRoomConfig()->GetRandomRoom(rng._seed, true, stage, 11, currentRoom->Shape, 0, -1, AmbushDifficulty[index], AmbushDifficulty[index], &doors, subtype, -1);
			spawnCount = config->SpawnCount;
			if (spawnCount != 0) {
				configs.push_back(std::make_tuple(config, initial, weight));
				for (int k = 0; k < 3 * spawnCount; ++k)	
					// One call for PickEntry, one for FixEntry, one for SpawnWrapper.
					rng.Next();
			}
			++j;
		} while (spawnCount == 0 && j < 10);
		index = std::min(index + 1, 3);
	}

	for (auto [config, initial, weight] : configs) {
		config->InitialWeight = initial;
		config->Weight = weight;
	}

	int count = std::min((int)configs.size(), maxCount);
	for (int i = 0; i < count; ++i)
		out[i] = std::get<0>(configs[i]);
	return count;
}

MOD_EXPORT void L_Ambush_SetMaxBossChallengeWaves(int waves) {
	bossAmbushWaves = waves;
}

MOD_EXPORT void L_Ambush_SetMaxBossrushWaves(int waves) {
	g_Game->_ambush.maxBossWaves = waves;
}

MOD_EXPORT void L_Ambush_SetMaxChallengeWaves(int waves) {
	ambushWaves = waves;
}

MOD_EXPORT void L_Ambush_SpawnBossrushWave() {
	g_Game->_ambush.SpawnBossrushWave();
}

MOD_EXPORT void L_Ambush_SpawnWave() {
	g_Game->_ambush.SpawnWave();
}

MOD_EXPORT void L_Ambush_StartChallenge() {
	g_Game->_ambush.StartChallenge();
}