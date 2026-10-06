#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../LuaClasses.h"
#include "LuaEntityBridge.h"
#include "../REPENTOGONDelirium.h"

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

LUA_FUNCTION(Lua_Entity_PushClass) {
	void* pointer = (void*)(uintptr_t)luaL_checknumber(L, 1);
	switch ((int)luaL_checkinteger(L, 2)) {
	case 1: LuaEntityPlayer::PushPtr(L, (Entity_Player*)pointer); break;
	case 3: LuaEntityFamiliar::PushPtr(L, (Entity_Familiar*)pointer); break;
	case 5: LuaEntityPickup::PushPtr(L, (Entity_Pickup*)pointer); break;
	case 6: LuaEntitySlot::PushPtr(L, (Entity_Slot*)pointer); break;
	case 100: {
		Entity* entity = (Entity*)pointer;
		Entity_NPC* npc = entity->ToNPC();
		if (entity->_type == delirium::ENTITY_DELIRIUM || (npc && *npc->GetDeliriumBossType() == npc->_type && *npc->GetDeliriumBossVariant() == npc->_variant)) {
			LuaEntityNPC::PushPtr(L, (Entity_NPC*)pointer);
			luaL_setmetatable(L, lua::metatables::DeliriumMetatable);
		}
		else {
			lua_pushnil(L);
		}
		break;
	}
	default: return luaL_error(L, "No FFI-less class for this entity");
	}
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
	lua_register(_state, "__Lua_Entity_PushPlayer", Lua_Entity_PushPlayer);
	lua_register(_state, "__Lua_Entity_PushClass", Lua_Entity_PushClass);
	lua_register(_state, "__Lua_Entity_PushNPC", Lua_Entity_PushNPC);
	lua_register(_state, "__Lua_Entity_PushEffect", Lua_Entity_PushEffect);
	lua_register(_state, "__Lua_Entity_PushResults", Lua_Entity_PushResults);

	super();
}
