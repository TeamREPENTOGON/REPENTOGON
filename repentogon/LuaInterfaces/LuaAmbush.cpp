#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int ambushWaves = 3;

extern "C" {

	//todo
	/*
	* GetMaxChallnegeWaves
	* SetMaxChallengeWaves
	* IsActive
	* GetNextWave
	* GetNextWaves
	*/

	void L_Ambush_StartChallenge() {
		g_Game->GetAmbush()->StartChallenge();
	}

	void L_Ambush_SpawnBossrushWave() {
		g_Game->GetAmbush()->SpawnBossrushWave();
	}

	void L_Ambush_SpawnWave() {
		g_Game->GetAmbush()->SpawnWave();
	}

	int L_Ambush_GetMaxBossrushWaves() {
		return *g_Game->GetAmbush()->GetMaxBossrushWaves();
	}

	void L_Ambush_SetMaxBossrushWaves(int amount) {
		*g_Game->GetAmbush()->GetMaxBossrushWaves() = amount;
	}

	int L_Ambush_GetMaxChallengeWaves() {
		return 0; //todo
	}

	void L_Ambush_SetMaxChallengeWaves(int amount) {
		return; //todo
	}

	int L_Ambush_GetCurrentWave() {
		return *g_Game->GetAmbush()->GetCurrentWave();
	}

	bool L_Ambush_IsActive() {
		return false; //todo
	}

	RoomConfig* L_Ambush_GetNextWave() {
		return nullptr; //todo
	}

	void L_Ambush_GetNextWaves() {
		 //todo
	}

}


LUA_FUNCTION(Lua_GetMaxChallengeWaves)
{
	Ambush* ambush = g_Game->GetAmbush();
	lua_pushinteger(L, ambushWaves);
	return 1;
}

LUA_FUNCTION(Lua_SetMaxChallengeWaves)
{
	Ambush* ambush = g_Game->GetAmbush();
	int amount = (int)luaL_checkinteger(L, 1);

	ambushWaves = amount;

	return 0;
}

LUA_FUNCTION(Lua_Ambush_IsActive) {
	Ambush* ambush = g_Game->GetAmbush();
	// lua_pushboolean(L, ambush->isActive);
	return 1;
}

static int AmbushDifficulty[4] = { 1, 5, 10, 15 };

#undef min
#undef max

void SetupAmbushData(lua_State* L, Ambush* ambush, RNG* rng, int* index, int* subtype) {
	RoomDescriptor* descriptor = g_Game->_room->_descriptor;
	RoomConfig* config = descriptor->Data;

	if (config->Type != 11) {
		luaL_error(L, "Cannot get Ambush wave information outside of a (boss) challenge room");
	}

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

LUA_FUNCTION(Lua_Ambush_GetNextWave) {
	Ambush* ambush = g_Game->GetAmbush();
	RoomConfig* currentRoom = g_Game->_room->_descriptor->Data;
	RNG rng;
	int index;
	int subtype;

	SetupAmbushData(L, ambush, &rng, &index, &subtype);

	int spawnCount = 0;
	RoomConfig* config = nullptr;

	int i = 0;
	do {
		unsigned int requiredDoors = 0;
		rng.Next();
		config = g_Game->GetRoomConfigHolder()->GetRandomRoom(rng._seed, false,
			g_Game->GetRoomConfig()->GetStageID(g_Game->_stage, g_Game->_stageType, -1),
			11 /* Challenge room */, currentRoom->Shape, 0, -1, AmbushDifficulty[index] /* ebp - 60 */,
			AmbushDifficulty[index] /* ebp - 60 */, &requiredDoors/* ebp - 4C */, subtype /* ebp - 64 */, -1);

		if (!config) {
			lua_pushnil(L);
			return 1;
		}

		spawnCount = config->SpawnCount;
		++i;
	} while (spawnCount == 0 && i < 10);

	if (config && i != 10) {
		lua::luabridge::UserdataPtr::push(L, config, lua::Metatables::ROOM_CONFIG_ROOM);
	}
	else {
		lua_pushnil(L);
	}

	return 1;
}

LUA_FUNCTION(Lua_Ambush_GetNextWaves) {
	Ambush* ambush = g_Game->GetAmbush();
	RoomDescriptor* descriptor = g_Game->_room->_descriptor;
	RoomConfig* currentRoom = descriptor->Data;

	RNG rng;
	int index;
	int subtype;
	unsigned int doors = 0;

	SetupAmbushData(L, ambush, &rng, &index, &subtype);

	std::vector<std::tuple<RoomConfig*, float, float>> configs;
	lua_newtable(L);
	int limit = ambushWaves;
	if (currentRoom->Subtype == 1) {
		limit = ambush->maxBossWaves;
	}

	for (int i = ambush->currentWave; i < ambushWaves; ++i) {
		int spawnCount = 0;
		int j = 0;
		do {
			rng.Next();

			int stage = g_Game->GetRoomConfig()->GetStageID(g_Game->_stage, g_Game->_stageType, -1);
			// Draw the room a first time without reducing its weight.
			// We need to determine what its current weight is in order to reduce it during 
			// a second draw. 
			RoomConfig* config = g_Game->GetRoomConfigHolder()->GetRandomRoom(rng._seed, false, stage, 11, currentRoom->Shape, 0, -1, AmbushDifficulty[index], AmbushDifficulty[index], &doors, subtype, -1);
			if (!config) {
				return 1;
			}

			float weight = config->Weight;
			float initial = config->InitialWeight;
			config = g_Game->GetRoomConfigHolder()->GetRandomRoom(rng._seed, true, stage, 11, currentRoom->Shape, 0, -1, AmbushDifficulty[index], AmbushDifficulty[index], &doors, subtype, -1);

			spawnCount = config->SpawnCount;
			if (spawnCount != 0) {
				configs.push_back(std::make_tuple(config, initial, weight));

				for (int k = 0; k < 3 * spawnCount; ++k) {
					// One call for PickEntry, one for FixEntry, one for SpawnWrapper.
					rng.Next();
				}
			}

			++j;
		} while (spawnCount == 0 && j < 10);

		index = std::min(index + 1, 3);
	}

	for (auto [config, initial, weight] : configs) {
		config->InitialWeight = initial;
		config->Weight = weight;
	}

	for (unsigned int i = 0; i < configs.size(); ++i) {
		lua_pushinteger(L, i + 1);
		lua::luabridge::UserdataPtr::push(L, std::get<0>(configs[i]), lua::Metatables::ROOM_CONFIG_ROOM);
		lua_rawset(L, -3);
	}

	return 1;
}