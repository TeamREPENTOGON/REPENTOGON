#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../LuaClasses.h"
#include "LuaEntityBridge.h"

static std::vector<Entity*> s_entityResults;

void StoreEntityResults(Entity** data, unsigned int size) {
	s_entityResults.assign(data, data + size);
}

LUA_FUNCTION(Lua_Entity_EntityAddress) {
	lua_pushnumber(L, (lua_Number)(uintptr_t)LuaEntity::Get(L, 1));
	return 1;
}

LUA_FUNCTION(Lua_Entity_PlayerAddress) {
	lua_pushnumber(L, (lua_Number)(uintptr_t)LuaEntityPlayer::Get(L, 1));
	return 1;
}

LUA_FUNCTION(Lua_Entity_PushEntity) {
	LuaEntity::PushPtr(L, (Entity*)(uintptr_t)luaL_checknumber(L, 1));
	return 1;
}

LUA_FUNCTION(Lua_Entity_PushPlayer) {
	LuaEntityPlayer::PushPtr(L, (Entity_Player*)(uintptr_t)luaL_checknumber(L, 1));
	return 1;
}

LUA_FUNCTION(Lua_Entity_PushNPC) {
	LuaEntityNPC::PushPtr(L, (Entity_NPC*)(uintptr_t)luaL_checknumber(L, 1));
	return 1;
}

LUA_FUNCTION(Lua_Entity_PushEffect) {
	LuaEntityEffect::PushPtr(L, (Entity_Effect*)(uintptr_t)luaL_checknumber(L, 1));
	return 1;
}

LUA_FUNCTION(Lua_Entity_PushResults) {
	lua_createtable(L, (int)s_entityResults.size(), 0);
	for (size_t i = 0; i < s_entityResults.size(); i++) {
		LuaEntity::PushPtr(L, s_entityResults[i]);
		lua_rawseti(L, -2, (int)i + 1);
	}
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	lua_register(_state, "__Lua_Entity_EntityAddress", Lua_Entity_EntityAddress);
	lua_register(_state, "__Lua_Entity_PlayerAddress", Lua_Entity_PlayerAddress);
	lua_register(_state, "__Lua_Entity_PushEntity", Lua_Entity_PushEntity);
	lua_register(_state, "__Lua_Entity_PushPlayer", Lua_Entity_PushPlayer);
	lua_register(_state, "__Lua_Entity_PushNPC", Lua_Entity_PushNPC);
	lua_register(_state, "__Lua_Entity_PushEffect", Lua_Entity_PushEffect);
	lua_register(_state, "__Lua_Entity_PushResults", Lua_Entity_PushResults);

	super();
}
