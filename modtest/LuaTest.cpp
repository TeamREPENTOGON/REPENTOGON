#include <cstdio>

#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include <iostream>
#include <sstream>
#include <string>

static std::map<std::string, std::vector<std::pair<std::string, void*>>> _functions;

template<typename T>
T ud_to_data(void* ud) {
	return *(T*)((char*)ud + 4);
}

static int Room_GetRailType(lua_State* L) {

}

static int Room_SetRailType(lua_State* L) {

}

static int Room_Test(lua_State* L) {
	lua_pushinteger(L, 12);
	return 1;
}

static int LuaDumpRegistry(lua_State* L) {
	int top = lua_gettop(L);
	lua_newtable(L);
	lua_pushnil(L);
	while (lua_next(L, LUA_REGISTRYINDEX) != 0) {
		lua_pushvalue(L, -2);
		lua_pushvalue(L, -2);
		lua_rawset(L, -5);
		lua_pop(L, 1);
	}

	int newtop = lua_gettop(L);
	if (newtop != top + 1) {
		FILE* f = fopen("repentogon.log", "a");
		fprintf(f, "top = %d, newtop = %d\n", top, newtop);
		fclose(f);
		exit(-1);
	}
	return 1;
}

static int LuaExtractFunctions(lua_State* L) {
	/* int n = lua_gettop(L);
	std::string path;
	if (n == 0) {
		path = "lua_functions.log";
	} else {
		if (lua_type(L, 1) == LUA_TSTRING) {
			path = lua_tostring(L, 1);
		} else {
			path = "lua_functions.log";
		}
	} */

	lua_newtable(L); // t
	int i = 0;
	for (auto iter = _functions.begin(); iter != _functions.end(); ++iter, ++i) {
		lua_newtable(L); // t t

		lua_pushstring(L, "type"); // t t type 
		lua_pushstring(L, iter->first.c_str()); // t t type name
		lua_rawset(L, -3); // t t

		lua_pushstring(L, "functions"); // t t functions
		lua_newtable(L); // t t functions t

		for (auto fn_iter = iter->second.begin(); fn_iter != iter->second.end(); ++fn_iter) {
			lua_pushstring(L, fn_iter->first.c_str()); // t t functions t fnname
			lua_pushlightuserdata(L, fn_iter->second); // t t functions t fname addr
			lua_rawset(L, -3); // t t functions t
		}

		lua_rawset(L, -3); // t t

		lua_rawseti(L, -2, i); // t
	}

	return 1;
}


static void RegisterRailFunctions(lua_State* L) {
	// Get metatable of Room object
	lua::PushMetatable(L, lua::Metatables::ROOM);
	lua_pushstring(L, "Test");
	lua_pushcfunction(L, Room_Test);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void ExtractGameFunctions(lua_State* L, std::vector<std::pair<std::string, void*>>& functions, FILE* f) {
	lua_pushnil(L);
	while (lua_next(L, -3)) {
		if (lua_type(L, -2) == LUA_TSTRING && lua_tostring(L, -2)[0] != '_') {
			const char* name = lua_tostring(L, -2);
			if (lua_type(L, -1) == LUA_TFUNCTION) {
				const void* addr = lua_topointer(L, -1);
				unsigned char nupvalues = *(unsigned char*)((char*)addr + 0x6);

				if (nupvalues == 1) {
					lua_TValue* upvalue = (lua_TValue*)((char*)addr + 0x10);
					if ((upvalue->tt_ & 0xF) == LUA_TUSERDATA) {
						Udata* closure_udata = (Udata*)upvalue->value_.p;
						void* fn_addr = *(void**)((char*)closure_udata + 0x18);
						// fprintf(f, "Found addr of %s at %p\n", name, fn_addr);
						functions.push_back(std::make_pair(name, fn_addr));
					}
				}
			}
		}
		lua_pop(L, 1);
	}
}

static void RegisterMetatables(lua_State* L) {
	FILE* f = fopen("repentogon.log", "a");
	fprintf(f, "Dumping Lua registry\n");

	lua_pushnil(L);
	std::map<std::string, void*> metatables;

	while (lua_next(L, LUA_REGISTRYINDEX)) {
		if (lua_type(L, -2) == LUA_TLIGHTUSERDATA) {
			if (lua_type(L, -1) == LUA_TTABLE) {
				lua_pushstring(L, "__type");
				int __type = lua_rawget(L, -2);

				if (__type == LUA_TSTRING) {
					std::string type(lua_tostring(L, -1));
					void* addr = lua_touserdata(L, -3);

					if (type == "Room" || type == "const Room") {
						lua_pushstring(L, "GetBossID");
						int bossID = lua_rawget(L, -3);

						if (bossID == LUA_TNIL) {
							if (type.find("const") != std::string::npos) {
								type = "const RoomConfig_Room";
							}
							else {
								type = "RoomConfig_Room";
							}
						}

						lua_pop(L, 1);
					}
					else if (type == "RoomDescriptor" || type == "const RoomDescriptor") {
						lua_pushstring(L, "Get");
						int get = lua_rawget(L, -3);

						if (get != LUA_TNIL) {
							if (type.find("const") != std::string::npos) {
								type = "const ArrayProxy_RoomDescriptor";
							}
							else {
								type = "ArrayProxy_RoomDescriptor";
							}
						}

						lua_pop(L, 1);
					}

					lua::RegisterMetatable(lua::GetMetatableIdxFromName(type), addr);
					ExtractGameFunctions(L, _functions[type.c_str()], f);
				}

				lua_pop(L, 1);
			}
		}

		lua_pop(L, 1);
	}

	fprintf(f, "Done dumping Lua registry\n");
	fclose(f);
}


int Lua_GetMultiShotPositionVelocity(lua_State *L) // This *should* be in the API, but magically vanished some point after 1.7.8.
{
	Entity_Player* player = *(Entity_Player**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer") + 4);
	int loopIndex = luaL_checkinteger(L, 2);
	WeaponType weaponType = (WeaponType)luaL_checkinteger(L, 3);
	Vector* shotDirection = *(Vector**)((char*)lua::CheckUserdata(L, 4, lua::Metatables::VECTOR, "Vector") + 4);
	float shotSpeed = luaL_checknumber(L, 5);
	Weapon_MultiShotParams* multiShotParams = *(Weapon_MultiShotParams**)((char*)lua_touserdata(L, 6) + 4); // no metatable for this in the API (yet)! :compressed_torvalds:

	if (multiShotParams->numTears < loopIndex) {
		luaL_argerror(L, 2, "LoopIndex cannot be higher than MultiShotParams.numTears");
	};

	PosVel* toLua = lua::luabridge::UserdataValue<PosVel>::place(L, lua::GetMetatableKey(lua::Metatables::POS_VEL));
	*toLua = player->GetMultiShotPositionVelocity(loopIndex, weaponType, *shotDirection, shotSpeed, *multiShotParams);

	return 1;
}

static void RegisterMultiShotPositionVelocity(lua_State *L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "GetMultiShotPositionVelocity");
	lua_pushcfunction(L, Lua_GetMultiShotPositionVelocity);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_GameAchievementUnlocksDisallowed(lua_State * L)
{
	Game* game = *(Game**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::GAME, "Game") + 4);
	lua_pushboolean(L, game->AchievementUnlocksDisallowed());

	return 1;
}

static void RegisterAchievementUnlocksDisallowed(lua_State *L)
{
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "AchievementUnlocksDisallowed");
	lua_pushcfunction(L, Lua_GameAchievementUnlocksDisallowed);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static constexpr const char* RoomConfigHolderMT = "RoomConfigHolder";
static constexpr const char* RoomPlacerMT = "RoomPlacer";
static constexpr const char* RoomDescriptorDoors = "RoomDescriptorDoors";
static constexpr const char* RoomDescriptorDoorsConst = "RoomDescriptorDoorsConst";
static constexpr const char* PlayerManagerMT = "PlayerManager";
static constexpr const char* PersistentGameDataMT = "PersistentGameData";

static int Lua_GameGetRoomConfigHolder(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	void** ud = (void**)lua_newuserdata(L, sizeof(void*));
	*ud = (char*)game + 0x1879C;
	// lua_pushlightuserdata(L, (char*)game + 0x1879C);
	luaL_setmetatable(L, RoomConfigHolderMT);
	return 1;
}

static int Lua_RoomConfigHolderGetRoomByStageTypeAndVariant(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 5) {
		return luaL_error(L, "Expected five parameters, got %d\n", n);
	}

	RoomConfigHolder* holder = *lua::GetUserdata<RoomConfigHolder**>(L, 1, RoomConfigHolderMT);
	int stage = luaL_checkinteger(L, 2);
	
	if (stage < 0 || stage > 36) {
		return luaL_error(L, "StageID must be between 0 and 36 (both inclusive), got %d\n", stage);
	}

	int type = luaL_checkinteger(L, 3);
	if (type < 1 || type > 29) {
		return luaL_error(L, "Type must be between 1 and 29 (both inclusive), got %d\n", type);
	}

	int variant = luaL_checkinteger(L, 4);
	int difficulty = luaL_checkinteger(L, 5);
	if (difficulty < -2 || difficulty > 1) {
		difficulty = -1;
	}

	RoomConfig* config = holder->GetRoomByStageTypeAndVariant(stage, type, variant, difficulty);
	if (!config) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, config, lua::GetMetatableKey(lua::Metatables::CONST_ROOM_CONFIG_ROOM));
	}

	return 1;
}

static void RegisterRoomConfigHolder(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetRoomConfigHolder");
	lua_pushcfunction(L, Lua_GameGetRoomConfigHolder);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, RoomConfigHolderMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetRoomByStageTypeAndVariant", Lua_RoomConfigHolderGetRoomByStageTypeAndVariant },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

static RoomDescriptor* GetLeftRoom(RoomDescriptor* source) {
	return nullptr;
}

static int Lua_LevelPlaceRoom(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Game");
	LevelGenerator_Room* room = lua::GetUserdata<LevelGenerator_Room*>(L, 2, RoomPlacerMT);
	RoomConfig* config = lua::GetUserdata<RoomConfig*>(L, 3, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
	uint64_t seed = luaL_checkinteger(L, 4);

	bool result = game->PlaceRoom(room, config, seed, 0);
	lua_pushboolean(L, result);

	if (result) {
		// Fix doors for other rooms
		uint32_t roomIdx = room->_gridLineIdx * 13 + room->_gridColIdx;
		RoomDescriptor* descriptor = game->GetRoomByIdx(roomIdx, -1);
		
		switch (descriptor->Data->Shape) {
		case ROOMSHAPE_1x1:
			break;
		}
	}

	return 1;
}

static int Lua_RoomPlacerSetColIdx(lua_State* L) {
	LevelGenerator_Room* room = lua::GetUserdata<LevelGenerator_Room*>(L, 1, RoomPlacerMT);
	room->_gridColIdx = luaL_checkinteger(L, 2);
	return 0;
}

static int Lua_RoomPlacerSetLineIdx(lua_State* L) {
	LevelGenerator_Room* room = lua::GetUserdata<LevelGenerator_Room*>(L, 1, RoomPlacerMT);
	room->_gridLineIdx = luaL_checkinteger(L, 2);
	return 0;
}

static int Lua_RoomPlacerSetAllowedDoors(lua_State* L) {
	LevelGenerator_Room* room = lua::GetUserdata<LevelGenerator_Room*>(L, 1, RoomPlacerMT);
	room->_doors = luaL_checkinteger(L, 2);
	return 0;
}

static int Lua_LevelGeneratorEntry(lua_State* L) {
	LevelGenerator_Room* ud = (LevelGenerator_Room*)lua_newuserdata(L, sizeof(LevelGenerator_Room));
	luaL_setmetatable(L, RoomPlacerMT);
	return 1;
}

void RegisterRoomAdder(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::LEVEL);
	lua_pushstring(L, "PlaceRoom");
	lua_pushcfunction(L, Lua_LevelPlaceRoom);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, RoomPlacerMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "SetColIdx", Lua_RoomPlacerSetColIdx },
		{ "SetLineIdx", Lua_RoomPlacerSetLineIdx },
		{ "SetAllowedDoors", Lua_RoomPlacerSetAllowedDoors },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "LevelGeneratorEntry");
	lua_pushcfunction(L, Lua_LevelGeneratorEntry);
	lua_rawset(L, -3);
}

static void RoomDescriptorGetAllowedDoors(lua_State* L, RoomDescriptor* descriptor) {
	lua_pushinteger(L, descriptor->AllowedDoors);
}

static int Lua_RoomDescriptorGetAllowedDoors(lua_State* L) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	RoomDescriptorGetAllowedDoors(L, descriptor);
	return 1;
}

static int Lua_RoomDescriptorGetAllowedDoorsConst(lua_State* L) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::CONST_ROOM_DESCRIPTOR, "const RoomDescriptor");
	RoomDescriptorGetAllowedDoors(L, descriptor);
	return 1;
}

static int Lua_RoomDescriptorSetAllowedDoors(lua_State* L) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	int doors = luaL_checkinteger(L, 2);
	descriptor->AllowedDoors = doors;
	return 0;
}

static int Lua_RoomDescriptorSetDoor(lua_State* L) {
	return 0;
}

static void RoomDescriptorGetDoors(lua_State* L, RoomDescriptor* descriptor, bool allowSet) {
	RoomDescriptor** ptr = (RoomDescriptor**)lua_newuserdata(L, sizeof(RoomDescriptor*));
	*ptr = descriptor;
	if (allowSet) {
		luaL_setmetatable(L, RoomDescriptorDoors);
	} else {
		luaL_setmetatable(L, RoomDescriptorDoorsConst);
	}
}

static int Lua_RoomDescriptorGetDoors(lua_State* L) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	RoomDescriptorGetDoors(L, descriptor, true);
	return 1;
}

static int Lua_RoomDescriptorGetDoorsConst(lua_State* L) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::CONST_ROOM_DESCRIPTOR, "const RoomDescriptor");
	RoomDescriptorGetDoors(L, descriptor, false);
	return 1;
}

static void _RoomDescriptorDoorsGet(lua_State* L, RoomDescriptor* descriptor) {
	int slot = luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		luaL_error(L, "Invalid door slot %d\n", slot);
		return;
	}

	lua_pushinteger(L, descriptor->Doors[slot]);
}

static int Lua_RoomDescriptorDoorsGet(lua_State* L) {
	RoomDescriptor** ptr = lua::GetUserdata<RoomDescriptor**>(L, 1, RoomDescriptorDoors);
	_RoomDescriptorDoorsGet(L, *ptr);
	return 1;
}

static int Lua_RoomDescriptorDoorsGetConst(lua_State* L) {
	RoomDescriptor** ptr = lua::GetUserdata<RoomDescriptor**>(L, 1, RoomDescriptorDoorsConst);
	_RoomDescriptorDoorsGet(L, *ptr);
	return 1;
}

static int Lua_RoomDescriptorDoorsSet(lua_State* L) {
	RoomDescriptor** ptr = lua::GetUserdata<RoomDescriptor**>(L, 1, RoomDescriptorDoors);
	RoomDescriptor* descriptor = *ptr;

	int slot = luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		return luaL_error(L, "Invalid door slot %d\n", slot);
	}

	int value = luaL_checkinteger(L, 3);
	if (value < -1 || value > 255) {
		return luaL_error(L, "Invalid door mask %d\n", value);
	}

	descriptor->Doors[slot] = value;
	return 0;
}

static void FixRoomDescriptorProperties(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ROOM_DESCRIPTOR);
	lua_pushstring(L, "__propget");
	int type = lua_rawget(L, -2);

	if (type != LUA_TTABLE) {
		lua_pop(L, 2);
		printf("__propget is not a table\n");
		return;
	}

	lua_pushstring(L, "AllowedDoors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetAllowedDoors);
	lua_rawset(L, -3);

	lua_pushstring(L, "Doors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetDoors);
	lua_rawset(L, -3);

	lua_pop(L, 2);

	lua::PushMetatable(L, lua::Metatables::CONST_ROOM_DESCRIPTOR);
	lua_pushstring(L, "__propget");
	type = lua_rawget(L, -2);

	if (type != LUA_TTABLE) {
		lua_pop(L, 2);
		printf("__propget is not a table\n");
		return;
	}

	lua_pushstring(L, "AllowedDoors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetAllowedDoorsConst);
	lua_rawset(L, -3);

	lua_pushstring(L, "Doors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetDoorsConst);
	lua_rawset(L, -3);

	lua_pop(L, 2);

	lua::PushMetatable(L, lua::Metatables::ROOM_DESCRIPTOR);
	lua_pushstring(L, "__propset");
	type = lua_rawget(L, -2);

	if (type != LUA_TTABLE) {
		lua_pop(L, 2);
		printf("__propset is not a table\n");
		return;
	}

	lua_pushstring(L, "AllowedDoors");
	lua_pushcfunction(L, Lua_RoomDescriptorSetAllowedDoors);

	lua_rawset(L, -3);
	lua_pop(L, 2);

	luaL_newmetatable(L, RoomDescriptorDoors);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, Lua_RoomDescriptorDoorsGet);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, Lua_RoomDescriptorDoorsSet);
	lua_rawset(L, -5);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, RoomDescriptorDoorsConst);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, Lua_RoomDescriptorDoorsGetConst);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static int Lua_GetPlayerManager(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	void** ud = (void**)lua_newuserdata(L, sizeof(void*));
	*ud = (char*)game + 0x1AB40;
	luaL_setmetatable(L, PlayerManagerMT);
	return 1;
}

int Lua_FirstCollectibleOwner(lua_State* L)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, PlayerManagerMT);
	int collectible = luaL_checkinteger(L, 2);
	RNG* rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");
	bool unk = lua_toboolean(L, 4);

	Entity_Player* player = playerManager->FirstCollectibleOwner(collectible, *rng, unk);
	if (!player) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	return 1;
}

int Lua_SpawnCoPlayer2(lua_State* L)
{
	PlayerManager* playerManager = lua::GetUserdata<PlayerManager*>(L, 1, PlayerManagerMT);
	int playerType = luaL_checkinteger(L, 2);

	Entity_Player* player = playerManager->SpawnCoPlayer2(playerType);
	lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));

	return 1;
}

static void RegisterPlayerManager(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetPlayerManager");
	lua_pushcfunction(L, Lua_GetPlayerManager);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, PlayerManagerMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "FirstCollectibleOwner", Lua_FirstCollectibleOwner },
		{ "SpawnCoPlayer2", Lua_SpawnCoPlayer2 },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}
int Lua_InitTwin(lua_State* L)
{
	Entity_Player* player = *(Entity_Player**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer") + 4);
	int playerType = luaL_checkinteger(L, 2);

	Entity_Player* toLua = lua::luabridge::UserdataValue<Entity_Player>::place(L, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	toLua = player->InitTwin(playerType);

	return 1;

}

static void RegisterInitTwin(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "InitTwin");
	lua_pushcfunction(L, Lua_InitTwin);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static int Lua_GetPersistentGameData(lua_State* L) {
	Manager* manager = g_Manager;
	void** ud = (void**)lua_newuserdata(L, sizeof(void*));
	*ud = (char*)manager + 0x14;
	luaL_setmetatable(L, PersistentGameDataMT);
	return 1;
}

int Lua_PGDTryUnlock(lua_State* L)
{
	PersistentGameData* pgd = lua::GetUserdata<PersistentGameData*>(L, 1, PersistentGameDataMT);
	int unlock = luaL_checkinteger(L, 2);

	bool success = pgd->TryUnlock(unlock);
	lua_pushboolean(L, success);
	return 1;
}

static void RegisterPersistentGameData(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetPersistentGameData");
	lua_pushcfunction(L, Lua_GetPersistentGameData);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, PersistentGameDataMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "TryUnlock", Lua_PGDTryUnlock },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, Init, (bool Debug) -> void) {
	super(Debug);
	this->RunBundledScript("resources/scripts/enums_ex.lua");
	this->RunBundledScript("resources/scripts/main_ex.lua");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	printf("[REPENTOGON WAS HERE] (flame everywhere woah gif modding of isaac sticker)\n");
	lua_State *state = g_LuaEngine->_state;
	// luaL_openlibs(state);
	printf("repentogonning all over the place\n");
	lua_register(state, "DumpRegistry", LuaDumpRegistry);
	printf("i'm repeotogonnning!!!!!!\n");
	lua::UnloadMetatables();
	RegisterMetatables(state);
	RegisterRailFunctions(state);

	lua_register(state, "ExtractFunctions", LuaExtractFunctions);
	RegisterMultiShotPositionVelocity(state);
	RegisterAchievementUnlocksDisallowed(state);
	RegisterRoomConfigHolder(state);
	RegisterRoomAdder(state);
	RegisterPlayerManager(state);
	FixRoomDescriptorProperties(state);
	RegisterInitTwin(state);
	RegisterPersistentGameData(state);
}
