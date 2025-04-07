#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_BossPoolGetPool) {
	const int stbType = (int)luaL_checkinteger(L, 1);

	if (stbType >= 0 && stbType <= 36) {
		auto& pool = g_Game->_bossPool._pool[stbType];
		auto** ud = (BossPool_Pool**)lua_newuserdata(L, sizeof(BossPool_Pool*));

		*ud = &pool;
		luaL_setmetatable(L, lua::metatables::BossPoolMT);
		return 1;
	}
	else {
		return luaL_error(L, "[BossPoolManager.GetPool] : Invalid STB type!");
	}
}

LUA_FUNCTION(Lua_PoolGetName) {
	auto* pool = *lua::GetRawUserdata<BossPool_Pool**>(L, 1, lua::metatables::BossPoolMT);
	lua_pushstring(L, pool->_name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_PoolGetEntries) {
	auto* pool = *lua::GetRawUserdata<BossPool_Pool**>(L, 1, lua::metatables::BossPoolMT);
	auto& entrys = pool->_entries;

	lua_newtable(L);
	int idx = 1;

	for (const auto& entry : entrys) {
		lua_newtable(L);

		lua_pushstring(L, "bossID");
		lua_pushinteger(L, entry._id);
		lua_rawset(L, -3);

		lua_pushstring(L, "initialWeight");
		lua_pushnumber(L, entry._initialWeight);
		lua_rawset(L, -3);

		lua_pushstring(L, "weight");
		lua_pushnumber(L, entry._weight);
		lua_rawset(L, -3);

		lua_pushstring(L, "weightAlt");
		lua_pushnumber(L, entry._weight);
		lua_rawset(L, -3);

		lua_pushstring(L, "achievementID");
		lua_pushinteger(L, entry._achievementID);
		lua_rawset(L, -3);

		lua_rawseti(L, -2, idx);
		idx++;
	}

	return 1;
}

LUA_FUNCTION(Lua_PoolGetTotalWeight) {
	auto* pool = *lua::GetRawUserdata<BossPool_Pool**>(L, 1, lua::metatables::BossPoolMT);
	lua_pushnumber(L, pool->_totalWeight);
	return 1;
}

LUA_FUNCTION(Lua_PoolGetRNG) {
	auto* pool = *lua::GetRawUserdata<BossPool_Pool**>(L, 1, lua::metatables::BossPoolMT);
	lua::luabridge::UserdataPtr::push(L, &pool->_rng, lua::GetMetatableKey(lua::Metatables::RNG));
	return 1;
}

LUA_FUNCTION(Lua_PoolGetDoubleTroubleRoomVariantStart) {
	auto* pool = *lua::GetRawUserdata<BossPool_Pool**>(L, 1, lua::metatables::BossPoolMT);
	lua_pushinteger(L, pool->_doubleTroubleRoomVariantStart);
	return 1;
}

static void RegisterBossPoolPool(lua_State* L) {

	luaL_Reg functions[] = {
		{ "GetName", Lua_PoolGetName },
		{ "GetEntries", Lua_PoolGetEntries },
		{ "GetWeight", Lua_PoolGetTotalWeight },
		{ "GetRNG", Lua_PoolGetRNG },
		{ "GetDoubleTroubleRoomVariantStart", Lua_PoolGetDoubleTroubleRoomVariantStart },
		{ "GetDoubleTroubleRoomID", Lua_PoolGetDoubleTroubleRoomVariantStart },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::BossPoolMT, lua::metatables::BossPoolMT, functions);
}

LUA_FUNCTION(Lua_BossPoolGetRemovedBosses) {
	auto& bossesEntries = g_Game->_bossPool._removedBosses;

	lua_newtable(L);

	for (size_t i = 1; i < bossesEntries.size(); i++) {
		lua_pushinteger(L, i);
		lua_pushboolean(L, bossesEntries[i]);
		lua_rawset(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_BossPoolGetLevelBlacklist) {
	auto& bossesEntries = g_Game->_bossPool._levelBlacklist;

	lua_newtable(L);

	for (size_t i = 1; i < bossesEntries.size(); i++) {
		lua_pushinteger(L, i);
		lua_pushboolean(L, bossesEntries[i]);
		lua_rawset(L, -3);
	}

	return 1;
}

static void RegisterBossPool(lua_State* L) {
	lua_newtable(L);

	lua::TableAssoc(L, "GetPool", Lua_BossPoolGetPool);
	lua::TableAssoc(L, "GetRemovedBosses", Lua_BossPoolGetRemovedBosses);
	lua::TableAssoc(L, "GetLevelBlacklist", Lua_BossPoolGetLevelBlacklist);
	lua::TableAssoc(L, "GetRemovedSpecialBosses", Lua_BossPoolGetLevelBlacklist);
	
	lua_setglobal(L, "BossPoolManager");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(_state);
	RegisterBossPoolPool(_state);
	RegisterBossPool(_state);
}
