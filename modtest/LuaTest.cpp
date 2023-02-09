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


extern "C" int Lua_GetMultiShotPositionVelocity(lua_State *L) // This *should* be in the API, but magically vanished some point after 1.7.8.
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

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	freopen("CONOUT$", "w", stdout);
	printf("[REPENTOGON WAS HERE] (flame everywhere woah gif modding of isaac sticker)\n");
	lua_State *state = g_LuaEngine->_state; //Soon.
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
};
