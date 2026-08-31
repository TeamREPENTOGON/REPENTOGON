#include <cstdio>
#include <chrono>

#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Log.h"
#include "HookSystem.h"
#include "MiscFunctions.h"
#include "CrashHandler.h"
#include "Patches/EntityManager.h"
#include "SaveStateManagement/EntitySaveStateManagement.h"
#include "LuaInterfaces/_Internals.h"
#include <iostream>
#include <random>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

#include "LuaInit.h"

static std::map<std::string, std::vector<std::pair<std::string, void*>>> _functions;

int LuaKeys::runCallbackWithTwoParams = LUA_NOREF;

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
		ZHL::Log("top = %d, newtop = %d\n", top, newtop);
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

static void ExtractGameFunctions(lua_State* L, std::vector<std::pair<std::string, void*>>& functions, FILE* f) {
	lua_pushnil(L);
	while (lua_next(L, -3)) {
		if (lua_type(L, -2) == LUA_TSTRING && lua_tostring(L, -2)[0] != '_') {
			const char* name = lua_tostring(L, -2);
			if (lua_type(L, -1) == LUA_TFUNCTION) {
				const void* addr = lua_topointer(L, -1);
				unsigned char nupvalues = *(unsigned char*)((char*)addr + 0x6);

				if (nupvalues == 1) {
					TValue* upvalue = (TValue*)((char*)addr + 0x10);
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
	ZHL::Logger logger;
	logger.Log("Dumping Lua registry\n");

	lua_pushnil(L);
	std::map<std::string, void*> metatables;

	while (lua_next(L, LUA_REGISTRYINDEX)) {
		if (lua_type(L, -2) == LUA_TLIGHTUSERDATA) { // key value
			if (lua_type(L, -1) == LUA_TTABLE) { // key value
				lua_pushstring(L, "__type"); // key value (table) __type
				int __type = lua_rawget(L, -2); // key table table["__type"]

				if (__type == LUA_TSTRING) {
					std::string type(lua_tostring(L, -1));
					void* addr = lua_touserdata(L, -3);

					if (type == "Room" || type == "const Room") {
						lua_pushstring(L, "GetBossID"); // key table table["__type"] GetBossID
						int bossID = lua_rawget(L, -3); // key table table["__type"] table["GetBossID"]

						if (bossID == LUA_TNIL) {
							if (type.find("const") != std::string::npos) {
								type = "const RoomConfig_Room";
							}
							else {
								type = "RoomConfig_Room";
							}
						}

						lua_pop(L, 1); // key table table["__type"]
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
					ExtractGameFunctions(L, _functions[type.c_str()], logger.GetFile());
				}

				lua_pop(L, 1); // key table
			}
		}

		lua_pop(L, 1); // key
	}

	logger.Log("Done dumping Lua registry\n");
}

void NukeConstMetatables(lua_State* L) {
	int diff = (int)lua::Metatables::BEGIN_CONST + 1 - ((int)lua::Metatables::BEGIN_NORMAL + 1);
	for (int i = (int)lua::Metatables::BEGIN_CONST + 1; i < (int)lua::Metatables::METATABLES_MAX; ++i) {
		void* key = lua::GetMetatableKey((lua::Metatables)i);
		lua::PushMetatable(L, (lua::Metatables)(i - diff));
		lua_rawsetp(L, LUA_REGISTRYINDEX, key);
	}
}

static void bind_lua_internals(lua_State* L, int tblIdx)
{
	int stackTop = lua_gettop(L);
	tblIdx = lua_absindex(L, tblIdx);
	if (!lua_istable(L, tblIdx))
	{
		ZHL::Log("[ERROR] _LuaBindings is not a table.\n");
		LuaInternals::RaiseInitError();
		assert(false);

		return;
	}

	{
		lua::LuaStackProtector protector(L);
		lua_getfield(L, tblIdx, "EntityManager");
		EntityManager::detail::Init::BindLuaCallbacks(L, -1);
		lua_pop(L, 1);
	}

	{
		lua::LuaStackProtector protector(L);
		lua_getfield(L, tblIdx, "ESSM");
		EntitySaveStateManagement::detail::Init::BindLuaCallbacks(L, -1);
		lua_pop(L, 1);
	}
}

HOOK_METHOD(LuaEngine, Init, (bool Debug) -> void) {
	const char* C_BINDINGS_NAME = "_CBindings";
	const char* LUA_BINDINGS_NAME = "_LuaBindings";

	super(Debug);

	lua_State* L = g_LuaEngine->_state;
	luaL_requiref(L, "debug", luaopen_debug, 1);
	lua_pop(L, 1);
	luaL_requiref(L, "os", luaopen_os, 1);
	lua_pop(L, 1);

	lua_newtable(L);
	LuaInternals::RegisterInternals(L);
	lua_setglobal(L, C_BINDINGS_NAME);

	lua_State* state = g_LuaEngine->runCallbackRegistry->state;
	this->RunBundledScript("resources/scripts/enums_ex.lua");
	this->RunBundledScript("resources/scripts/main_ex.lua");

	luaL_unref(state, LUA_REGISTRYINDEX, g_LuaEngine->_unloadModFuncRef->_ref);
	lua_getglobal(state, "_UnloadMod");
	g_LuaEngine->_unloadModFuncRef->_ref = luaL_ref(state, LUA_REGISTRYINDEX);

	luaL_unref(state, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
	lua_getglobal(state, "_RunCallback");
	g_LuaEngine->runCallbackRegistry->key = luaL_ref(state, LUA_REGISTRYINDEX);

	{
		lua::LuaStackProtector protector(L);
		lua_getglobal(L, LUA_BINDINGS_NAME);
		bind_lua_internals(L, -1);
		lua_pop(L, 1);
	}

	// "delete" Lua Bindings
	lua_pushnil(L);
	lua_setglobal(L, LUA_BINDINGS_NAME);

	// "delete" C Bindings
	lua_pushnil(L);
	lua_setglobal(L, C_BINDINGS_NAME);

    lua_getglobal(state, "_RunCallbackWithTwoParams");
    LuaKeys::runCallbackWithTwoParams = luaL_ref(state, LUA_REGISTRYINDEX);

	NukeConstMetatables(_state);
	REPENTOGON::UpdateProgressDisplay("LuaEngine Initialized");
}

static int LuaBenchmark(lua_State* L) {
	if (int n = lua_gettop(L); n != 1) {
		return luaL_error(L, "Expected one parameter, got %d", n);
	}

	const char* s = lua_tostring(L, 1);
	if (!s) {
		return luaL_error(L, "Invalid parameter");
	}

	if (luaL_dostring(L, s)) {
		const char* err = lua_tostring(L, 1);
		std::string copy(err);
		lua_pop(L, 1);
		return luaL_error(L, "Invalid error: %s", copy.c_str());
	}

	bool running = lua_gc(L, LUA_GCISRUNNING, 0);
	if (running) {
		lua_gc(L, LUA_GCSTOP, 0);
	}

	auto begin = std::chrono::steady_clock::now();
	luaL_dostring(L, s);
	auto end = std::chrono::steady_clock::now();

	auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	lua_pushinteger(L, diff);

	if (running) {
		lua_gc(L, LUA_GCRESTART, 0);
	}

	return 1;
}

static void DumpTable(lua_State* L, FILE* f) {
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		fprintf(f, "%s - %s\n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
		lua_pop(L, 1); // Pop value
	}
}

HOOK_STATIC(Isaac, GetRoomEntities, (void* holder) -> void*, __cdecl) {
	void* res = super(holder);
	lua_Integer* key = (lua_Integer*)((char*)holder + 4);
	lua_State* L = g_LuaEngine->_state;
	lua_rawgeti(L, LUA_REGISTRYINDEX, *key);
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		lua_pop(L, 1); // Pop value
		lua_pushvalue(L, -1); // Copy key to keep it on the stack after the set
		lua_pushnil(L); // t key key nil
		lua_rawset(L, -4); // t key
	}
	/* lua_pushinteger(L, 1);
	lua_pushstring(L, "toto");
	lua_rawset(L, -3); */
	lua_pop(L, 1); 
	luaL_unref(L, LUA_REGISTRYINDEX, (int)*key);
	lua_createtable(L, 0, 0);
	*key = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_rawgeti(L, LUA_REGISTRYINDEX, *key);
	lua_pushinteger(L, 12);
	luaL_ref(L, -2);
	lua_settop(L, -2);
	return res;
}

static std::uniform_real_distribution<float> _distrib(-1, 1);
static std::random_device rd;
static std::mt19937 gen(rd());

LUA_FUNCTION(Lua_RandomFloat) {
	lua_pushnumber(L, _distrib(gen));
	return 1;
}

LUA_FUNCTION(Lua_ToRadians) {
	float angle = (float) luaL_checknumber(L, 1);
	lua_pushnumber(L, angle * M_PI / 180.f);
	return 1;
}

LUA_FUNCTION(Lua_ToDegrees) {
	float angle = (float) luaL_checknumber(L, 1);
	lua_pushnumber(L, angle * 180.f / M_PI);
	return 1;
}

// [ONLINE-DETERMINISM] Give Lua an RNG that both machines will agree on.
//
// Lua's stock math.random is seeded per process. Two players running byte-identical mod code
// on the same run seed still get different numbers out of it, so any mod that reaches for
// math.random instead of the game's RNG desyncs an online game every time - and nothing in
// the mod looks wrong, which is what makes it hard to find.
//
// The fix is not to route these calls into the game's own RNG: that stream is part of the
// game state, and consuming from it would change what every *other* roll returns. Instead
// this is a separate generator seeded from the run seed. Both machines start a run with the
// same seed and execute the same mod code, so they draw the same sequence, while the game's
// own stream is left exactly as it was.
//
// Re-seeding is lazy rather than hooked to run start: the seed is compared on every draw and
// the generator restarts whenever the run seed changes. That covers continues and restarts
// without needing to catch every path into a new run.
static unsigned int s_luaRngState = 0;
static unsigned int s_luaRngSeed = 0;
static bool s_luaRngReseedWarned = false;

// Any non-zero constant works to keep xorshift out of its fixed point; this is the usual one.
static const unsigned int LUA_RNG_FALLBACK = 0x9E3779B9u;

static void Lua_SyncRngToRun() {
	unsigned int runSeed = g_Manager ? g_Manager->_gamestate._seeds._gameStartSeed : 0u;
	if (runSeed != s_luaRngSeed) {
		s_luaRngSeed = runSeed;
		s_luaRngState = runSeed ? runSeed : LUA_RNG_FALLBACK;
	}
}

static unsigned int Lua_NextRandom() {
	Lua_SyncRngToRun();
	unsigned int x = s_luaRngState;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	s_luaRngState = x ? x : LUA_RNG_FALLBACK;
	return s_luaRngState;
}

LUA_FUNCTION(Lua_DeterministicRandom) {
	int argc = lua_gettop(L);
	unsigned int raw = Lua_NextRandom();

	if (argc == 0) {
		// Stock math.random() returns a float in [0,1). Dividing by 2^32 keeps that range
		// without ever reaching 1.0.
		lua_pushnumber(L, (lua_Number)raw / 4294967296.0);
		return 1;
	}

	lua_Integer lower = 1;
	lua_Integer upper = 0;
	if (argc == 1) {
		upper = luaL_checkinteger(L, 1);
		// Lua 5.3 gives math.random(0) its own meaning: an integer with every bit random,
		// rather than a range. Treating it as the range [1,0] would raise "interval is
		// empty" on a call that is legal and works in stock Lua.
		if (upper == 0) {
			unsigned long long high = (unsigned long long)Lua_NextRandom() << 32;
			lua_pushinteger(L, (lua_Integer)(high | (unsigned long long)Lua_NextRandom()));
			return 1;
		}
	} else {
		lower = luaL_checkinteger(L, 1);
		upper = luaL_checkinteger(L, 2);
	}
	if (lower > upper) {
		return luaL_error(L, "bad argument to 'random' (interval is empty)");
	}

	// Modulo over the span rather than over the raw value, so the bounds are respected.
	// The bias this leaves is the same one stock Lua 5.3 has here and is irrelevant next to
	// the determinism this exists to provide.
	unsigned long long span = (unsigned long long)(upper - lower) + 1ull;
	lua_pushinteger(L, lower + (lua_Integer)(raw % span));
	return 1;
}

LUA_FUNCTION(Lua_DeterministicRandomSeed) {
	// A mod re-seeding by hand is the exact problem this replaces - math.randomseed(os.time())
	// is the classic form and guarantees the two machines diverge. The call is accepted and
	// ignored so nothing breaks, but it is reported once because it means some mod is trying
	// to control this sequence and will not get what it expects.
	if (!s_luaRngReseedWarned) {
		s_luaRngReseedWarned = true;
		ZHL::Log("[ONLINE-DETERMINISM] a mod called math.randomseed; ignoring it, since an "
			"explicit seed would break agreement between players. The sequence stays tied "
			"to the run seed.\n");
	}
	return 0;
}

// [ONLINE-DETERMINISM] Make pairs() iterate in an order both machines will agree on.
//
// pairs() has no defined order. Lua walks the hash part in whatever arrangement the table's
// internal layout produced, which depends on insertion history and on pointer values, so the
// same table built the same way can iterate differently in two processes. A mod that takes
// the first match out of a pairs() loop, or accumulates floats across one, then produces
// different results on each machine from identical code, identical mods and an identical
// seed. Nothing in the mod looks wrong - this is the subtlest of the desync causes.
//
// Keys are collected, ordered by type and value, and walked through a closure. Numbers and
// strings sort by their own value and so are fully machine-independent. Keys of any other
// type (tables, functions, userdata) have nothing stable to sort on - their identity *is* an
// address - so they keep discovery order after the sortable ones and are reported once,
// because such a loop cannot be made deterministic from here.
// Off by default. This is the only one of the determinism measures that changes behaviour
// for *every* pairs() call in every mod and in REPENTOGON's own Lua, which makes it both the
// most useful and the most dangerous of them. A machine running these mods died with a stack
// buffer overrun and this is the change with the widest blast radius, so it is opt-in until
// that is understood: SetDeterministicPairs(true) enables it, which also makes it possible
// to tell whether a crash is caused by this or by something else.
static bool s_deterministicPairs = false;
static bool s_unsortableKeyWarned = false;

namespace {
	struct PairsKey {
		int rank;              // 0 = number, 1 = string, 2 = anything else
		double number;
		std::string text;
		int slot;              // position in the collected key table, 1-based

		bool operator<(const PairsKey& other) const {
			if (rank != other.rank) {
				return rank < other.rank;
			}
			if (rank == 0) {
				return number < other.number;
			}
			if (rank == 1) {
				return text < other.text;
			}
			// Unsortable: hold discovery order rather than inventing one.
			return slot < other.slot;
		}
	};
}

LUA_FUNCTION(Lua_SortedPairsIterator) {
	lua_Integer index = lua_tointeger(L, lua_upvalueindex(3)) + 1;
	lua_pushinteger(L, index);
	lua_replace(L, lua_upvalueindex(3));

	lua_rawgeti(L, lua_upvalueindex(2), (int)index);   // the key, or nil past the end
	if (lua_isnil(L, -1)) {
		return 1;
	}
	lua_pushvalue(L, -1);                              // key for the lookup
	lua_gettable(L, lua_upvalueindex(1));              // its current value
	return 2;
}

LUA_FUNCTION(Lua_DeterministicPairs) {
	luaL_checktype(L, 1, LUA_TTABLE);

	// __pairs must still win, or metatable-driven iteration silently stops working.
	if (luaL_getmetafield(L, 1, "__pairs") != LUA_TNIL) {
		lua_pushvalue(L, 1);
		lua_call(L, 1, 3);
		return 3;
	}

	if (!s_deterministicPairs) {
		lua_getglobal(L, "next");
		lua_pushvalue(L, 1);
		lua_pushnil(L);
		return 3;
	}

	lua_newtable(L);                                   // collected keys
	int keysIndex = lua_gettop(L);
	std::vector<PairsKey> order;
	int count = 0;

	lua_pushnil(L);
	while (lua_next(L, 1) != 0) {
		lua_pop(L, 1);                                 // value; only keys matter here
		PairsKey key;
		key.slot = ++count;
		int type = lua_type(L, -1);
		if (type == LUA_TNUMBER) {
			key.rank = 0;
			key.number = (double)lua_tonumber(L, -1);
		} else if (type == LUA_TSTRING) {
			key.rank = 1;
			// Safe only because the type was just checked: lua_tolstring converts numbers
			// in place, and doing that to a key mid-traversal would corrupt lua_next.
			size_t length = 0;
			const char* text = lua_tolstring(L, -1, &length);
			key.text.assign(text, length);
		} else {
			key.rank = 2;
			key.number = 0.0;
			if (!s_unsortableKeyWarned) {
				s_unsortableKeyWarned = true;
				ZHL::Log("[ONLINE-DETERMINISM] pairs() saw a key that is neither number nor "
					"string; its order cannot be made machine-independent, so that loop "
					"stays a desync risk.\n");
			}
		}
		order.push_back(key);

		lua_pushvalue(L, -1);                          // duplicate key to store...
		lua_rawseti(L, keysIndex, count);              // ...into the key table
		// the original key stays on the stack as lua_next's cursor
	}

	std::stable_sort(order.begin(), order.end());

	lua_newtable(L);                                   // keys, now in order
	int sortedIndex = lua_gettop(L);
	for (size_t i = 0; i < order.size(); ++i) {
		lua_rawgeti(L, keysIndex, order[i].slot);
		lua_rawseti(L, sortedIndex, (int)i + 1);
	}

	lua_pushvalue(L, 1);                               // upvalue 1: the table
	lua_pushvalue(L, sortedIndex);                     // upvalue 2: ordered keys
	lua_pushinteger(L, 0);                             // upvalue 3: cursor
	lua_pushcclosure(L, Lua_SortedPairsIterator, 3);
	lua_pushvalue(L, 1);
	lua_pushnil(L);
	return 3;
}

LUA_FUNCTION(Lua_SetDeterministicPairs) {
	s_deterministicPairs = lua_toboolean(L, 1) != 0;
	ZHL::Log("[ONLINE-DETERMINISM] deterministic pairs() %s\n",
		s_deterministicPairs ? "enabled" : "disabled");
	return 0;
}

HOOK_METHOD_PRIORITY(LuaEngine, RegisterClasses, 100, () -> void) {
	super();
	ZHL::Log("[REPENTOGON] Registering Lua functions and metatables\n");
	lua_State *state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	// luaL_openlibs(state);
	lua_register(state, "DumpRegistry", LuaDumpRegistry);
	lua::UnloadMetatables();
	RegisterMetatables(state);
	lua_register(state, "ExtractFunctions", LuaExtractFunctions);
	lua_register(state, "Benchmark", LuaBenchmark);
	lua_register(state, "RandomFloat", Lua_RandomFloat);
	lua_register(state, "ToRadians", Lua_ToRadians);
	lua_register(state, "ToDegrees", Lua_ToDegrees);

	// Swap the two entry points into Lua's per-process RNG for run-seeded equivalents.
	// Done here rather than at a lower priority so mod code never sees the stock versions.
	lua_getglobal(state, "math");
	if (lua_istable(state, -1)) {
		lua_pushcfunction(state, Lua_DeterministicRandom);
		lua_setfield(state, -2, "random");
		lua_pushcfunction(state, Lua_DeterministicRandomSeed);
		lua_setfield(state, -2, "randomseed");
		ZHL::Log("[ONLINE-DETERMINISM] math.random/math.randomseed now follow the run seed\n");
	} else {
		ZHL::Log("[ONLINE-DETERMINISM] no global 'math' table; leaving Lua's RNG alone\n");
	}
	lua_pop(state, 1);

	lua_register(state, "pairs", Lua_DeterministicPairs);
	lua_register(state, "SetDeterministicPairs", Lua_SetDeterministicPairs);
	ZHL::Log("[ONLINE-DETERMINISM] sorted pairs() is installed but OFF; "
		"SetDeterministicPairs(true) enables it\n");
}

HOOK_METHOD_PRIORITY(LuaEngine, RegisterClasses, 9999, () -> void) {
	REPENTOGON::UpdateProgressDisplay("Do LuaEngine::RegisterClasses");
	super();
}

HOOK_METHOD(ModManager, ListMods, () -> void) {
	REPENTOGON::UpdateProgressDisplay("Do ModManager::ListMods");
	super();
}

HOOK_METHOD_PRIORITY(Manager, LoadConfigs, 9999, () -> void) {
	ZHL::SetExceptionHandlerEnabled(false);
	REPENTOGON::UpdateProgressDisplay("Do Manager::LoadConfigs");
	super();
	ZHL::SetExceptionHandlerEnabled(true);
}

HOOK_METHOD_PRIORITY(ModManager, UpdateWorkshopMods, 9999, () -> void) {
	REPENTOGON::UpdateProgressDisplay("Do ModManager::UpdateWorkshopMods");
	super();
}

HOOK_METHOD_PRIORITY(ModManager, UpdateWorkshopMods, -9999, () -> void) {
	super();
	REPENTOGON::FinishProgressDisplay(); // UpdateWorkshopMods is pretty much the last big function called in IsaacRepentance::IsaacStartup()
}

HOOK_METHOD(RoomConfig, LoadStages, (char* xmlpath) -> void) {
	ZHL::SetExceptionHandlerEnabled(false);
	REPENTOGON::UpdateProgressDisplay("Do RoomConfig::LoadStages");
	super(xmlpath);
	ZHL::SetExceptionHandlerEnabled(true);
}
