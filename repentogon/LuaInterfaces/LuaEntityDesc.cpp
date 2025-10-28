#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUALIB_API unsigned int LuaCheckEntityType(lua_State* L, int arg) {
	const unsigned int type = (unsigned int)luaL_optinteger(L, arg, 0);
	if (type < 0 || (type >= 1 && type <= 9) || type >= 1000) {
		std::string error("Invalid Entity Type ");
		error.append(std::to_string(type));
		return luaL_argerror(L, arg, error.c_str());
	}
	return type;
}

LUALIB_API int LuaCheckChampionId(lua_State* L, int arg) {
	const int id = luaL_optinteger(L, arg, -1);
	if (id < -1 || id > 25) {
		return luaL_argerror(L, arg, "ChampionId must be between -1 and 25");
	}
	return id;
}

LUA_FUNCTION(Lua_EntityDescConstructor) {
	const unsigned int type = LuaCheckEntityType(L, 1);
	const unsigned int variant = (unsigned int)luaL_optinteger(L, 2, 0);
	const unsigned int subtype = (unsigned int)luaL_optinteger(L, 3, 0);
	const int championId = (int)LuaCheckChampionId(L, 4);
	const float health = (float)luaL_optnumber(L, 5, .0f);
	const float maxHealth = (float)luaL_optnumber(L, 6, .0f);
	const bool playerControlled = lua::luaL_optboolean(L, 7, false);

	EntityDesc* desc = nullptr;
	desc = lua::place<EntityDesc>(L, lua::metatables::EntityDescMT, type, variant, subtype, championId, health, maxHealth, playerControlled);

	return 1;
}

LUA_FUNCTION(Lua_EntityDescGetType) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	lua_pushinteger(L, desc->_type);
	return 1;
}

LUA_FUNCTION(Lua_EntityDescSetType) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	desc->_type = LuaCheckEntityType(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityDescGetVariant) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	lua_pushinteger(L, desc->_variant);
	return 1;
}

LUA_FUNCTION(Lua_EntityDescSetVariant) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	desc->_variant = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityDescGetSubtype) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	lua_pushinteger(L, desc->_subtype);
	return 1;
}

LUA_FUNCTION(Lua_EntityDescSetSubtype) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	desc->_subtype = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityDescGetChampionId) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	lua_pushinteger(L, desc->_championId);
	return 1;
}

LUA_FUNCTION(Lua_EntityDescSetChampionId) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	desc->_championId = LuaCheckChampionId(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityDescGetHealth) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	lua_pushnumber(L, desc->_health);
	return 1;
}

LUA_FUNCTION(Lua_EntityDescSetHealth) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	desc->_health = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityDescGetMaxHealth) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	lua_pushnumber(L, desc->_maxHealth);
	return 1;
}

LUA_FUNCTION(Lua_EntityDescSetMaxHealth) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	desc->_maxHealth = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityDescIsPlayerControlled) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	lua_pushboolean(L, desc->_playerControlled);
	return 1;
}

LUA_FUNCTION(Lua_EntityDescSetPlayerControlled) {
	EntityDesc* desc = lua::GetRawUserdata<EntityDesc*>(L, 1, lua::metatables::EntityDescMT);
	desc->_playerControlled = lua_toboolean(L, 2);
	return 0;
}

static void RegisterEntityDesc(lua_State* L) {

	luaL_Reg functions[] = {
		{ "GetType", Lua_EntityDescGetType},
		{ "SetType", Lua_EntityDescSetType },
		{ "GetVariant", Lua_EntityDescGetVariant },
		{ "SetVariant", Lua_EntityDescSetVariant },
		{ "GetSubtype", Lua_EntityDescGetSubtype },
		{ "SetSubtype", Lua_EntityDescSetSubtype },
		{ "GetChampionId", Lua_EntityDescGetChampionId },
		{ "SetChampionId", Lua_EntityDescSetChampionId },
		{ "GetHealth", Lua_EntityDescGetHealth },
		{ "SetHealth", Lua_EntityDescSetHealth },
		{ "GetMaxHealth", Lua_EntityDescGetMaxHealth },
		{ "SetMaxHealth", Lua_EntityDescSetMaxHealth },
		{ "IsPlayerControlled", Lua_EntityDescIsPlayerControlled },
		{ "SetPlayerControlled", Lua_EntityDescSetPlayerControlled },

		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::EntityDescMT, lua::metatables::EntityDescMT, functions);
	lua_register(L, lua::metatables::EntityDescMT, Lua_EntityDescConstructor);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterEntityDesc(_state);
}