#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"
#include "Delirium.h"

static void* ud_to_data(void* udata) {
	return *(void**)((char*)udata + 4);
}

template<typename T>
int lua_ReadAt(lua_State* L);

template<typename T>
int lua_ReadAt<std::enable_if_t<std::is_integral_v<T>, T>>(lua_State* L) {
	if (int n = lua_gettop(L); n != 1) {
		return luaL_error(L, "lua_ReadAt: expected 1 parameter, got %d\n", n);
	}

	lua_Integer addr = luaL_checkinteger(L, 1);
	void* ptr = (void*)addr;

	lua_pushinteger(L, *(T*)ptr);
	return 1;
}

template<>
int lua_ReadAt<float>(lua_State* L) {
	if (int n = lua_gettop(L); n != 1) {
		return luaL_error(L, "lua_ReadAt: expected 1 parameter, got %d\n", n);
	}

	lua_Integer addr = luaL_checkinteger(L, 1);
	void* ptr = (void*)addr;

	lua_pushnumber(L, *(float*)ptr);
	return 1;
}

template<>
int lua_ReadAt<void*>(lua_State* L) {
	if (int n = lua_gettop(L); n != 1) {
		return luaL_error(L, "lua_ReadAt: expected 1 parameter, got %d\n", n);
	}

	lua_Integer addr = luaL_checkinteger(L, 1);
	void* ptr = (void*)addr;

	lua_pushnumber(L, (uintptr_t) * (void**)ptr);
	return 1;
}

template<typename T>
int lua_ReadFromUDataAt(lua_State* L);

template<typename T>
int lua_ReadFromUDataAt<std::enable_if_t<std::is_integral_v<T>, T>>(lua_State* L) {
	if (int n = lua_gettop(L); n != 2) {
		return luaL_error(L, "lua_ReadFromUDataAt: expected 2 parameters, got %d\n", n);
	}

	void* udata = lua_touserdata(L, 1);
	lua_Integer offset = luaL_checkinteger(L, 2);

	void* ptr = ud_to_data(udata);
	lua_pushinteger(L, *(T*)((char*)ptr + offset));
	return 1;
}

template<>
int lua_ReadFromUDataAt<float>(lua_State* L) {
	if (int n = lua_gettop(L); n != 2) {
		return luaL_error(L, "lua_ReadFromUDataAt: expected 2 parameters, got %d\n", n);
	}

	void* udata = lua_touserdata(L, 1);
	lua_Integer offset = luaL_checkinteger(L, 2);

	void* ptr = ud_to_data(udata);
	lua_pushinteger(L, *(float*)((char*)ptr + offset));
	return 1;
}

template<>
int lua_ReadFromUDataAt<void*>(lua_State* L) {
	if (int n = lua_gettop(L); n != 2) {
		return luaL_error(L, "lua_ReadFromUDataAt: expected 2 parameters, got %d\n", n);
	}

	void* udata = lua_touserdata(L, 1);
	lua_Integer offset = luaL_checkinteger(L, 2);

	void* ptr = ud_to_data(udata);
	lua_pushinteger(L, (uintptr_t) * (void**)((char*)ptr + offset));
	return 1;
}

static int lua_IntRepr(lua_State* L) {
	if (int n = lua_gettop(L); n != 1) {
		return luaL_error(L, "Expected one argument, got %d\n", n);
	}

	lua_Number number = luaL_checknumber(L, 1);
	float asFloat = (float)number;

	lua_pushinteger(L, *(int*)&asFloat);
	return 1;
}

static void RegisterMemoryFunctions(lua_State* L) {
	lua_newtable(L);
	luaL_Reg functions[] = {
		{ "ReadInt8", lua_ReadAt<int8_t> },
		{ "ReadInt16", lua_ReadAt<int16_t> },
		{ "ReadInt32", lua_ReadAt<int32_t> },
		{ "ReadUInt8", lua_ReadAt<uint8_t> },
		{ "ReadUInt16", lua_ReadAt<uint16_t> },
		{ "ReadUInt32", lua_ReadAt<uint32_t> },
		{ "ReadFloat", lua_ReadAt<float> },
		{ "ReadAddr", lua_ReadAt<void*> },
		{ "ReadUDInt8", lua_ReadFromUDataAt<int8_t> },
		{ "ReadUDInt16", lua_ReadFromUDataAt<int16_t> },
		{ "ReadUDInt32", lua_ReadFromUDataAt<int32_t> },
		{ "ReadUDUInt8", lua_ReadFromUDataAt<uint8_t> },
		{ "ReadUDUInt16", lua_ReadFromUDataAt<uint16_t> },
		{ "ReadUDUInt32", lua_ReadFromUDataAt<uint32_t> },
		{ "ReadUDFloat", lua_ReadFromUDataAt<float> },
		{ "ReadUDAddr", lua_ReadFromUDataAt<void*> },
		{ "IntRepr", lua_IntRepr },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_setglobal(L, "Memory");
}

static int lua_EntityToDelirium(lua_State* L) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "EntityNPC");
	if (entity->_type == delirium::ENTITY_DELIRIUM) {
		Entity_NPC* npc = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
		luaL_setmetatable(L, delirium::DeliriumMetatable);
	}
	else if (Entity_NPC* npc = entity->ToNPC()) {
		if (*npc->GetDeliriumBossType() == npc->_type &&
			*npc->GetDeliriumBossVariant() == npc->_variant) {
			Entity_NPC* npc2 = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
			luaL_setmetatable(L, delirium::DeliriumMetatable);
		}
		else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}

	return 1;
}

static int lua_DeliriumGetBossType(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *delirium->GetDeliriumBossType());
	return 1;
}

static int lua_DeliriumGetBossVariant(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *delirium->GetDeliriumBossVariant());
	return 1;
}

static int lua_DeliriumGetTransformationTimer(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *delirium->GetDeliriumTransformationTimer());
	return 1;
}

static int lua_DeliriumGetRemainingAttacks(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *delirium->GetDeliriumRemainingAttacks());
	return 1;
}

static int lua_DeliriumGetAttackID(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *delirium->GetDeliriumAttackID());
	return 1;
}

static int lua_DeliriumGetCycle(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *delirium->GetDeliriumCycleID());
	return 1;
}

static int lua_DeliriumGetState(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *delirium->GetDeliriumState());
	return 1;
}

static int lua_DeliriumGetAngle(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *delirium->GetDeliriumAttackAngle());
	return 1;
}

static int lua_DeliriumSetTransformationTimer(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	uint16_t timer = luaL_checkinteger(L, 2);
	*delirium->GetDeliriumTransformationTimer() = timer;
	return 0;
}

static int lua_DeliriumSetRemainingAttacks(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	uint8_t remainingAttacks = luaL_checkinteger(L, 2);
	*delirium->GetDeliriumRemainingAttacks() = remainingAttacks;
	return 0;
}

static int lua_DeliriumSetAttackID(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	uint32_t attackId = luaL_checkinteger(L, 2);
	*delirium->GetDeliriumAttackID() = attackId;
	return 0;
}

static int lua_DeliriumSetCycle(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	uint32_t cycle = luaL_checkinteger(L, 2);
	*delirium->GetDeliriumCycleID() = cycle;
	return 0;
}

static int lua_DeliriumSetState(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	uint8_t state = luaL_checkinteger(L, 2);
	*delirium->GetDeliriumState() = state;
	return 0;
}

static int lua_DeliriumSetAngle(lua_State* L) {
	luaL_checkudata(L, 1, delirium::DeliriumMetatable);
	Entity_NPC* delirium = lua::UserdataToData<Entity_NPC*>(lua_touserdata(L, 1));
	uint8_t angle = luaL_checkinteger(L, 2);
	*delirium->GetDeliriumAttackAngle() = angle;
	return 0;
}

static void CreateDeliriumGetters(lua_State* L) {
	lua_pushstring(L, "__propget");
	lua_newtable(L);
	luaL_Reg functions[] = {
		{ "BossType", lua_DeliriumGetBossType },
		{ "BossVariant", lua_DeliriumGetBossVariant },
		{ "TransformationTimer", lua_DeliriumGetTransformationTimer },
		{ "RemainingAttacks", lua_DeliriumGetRemainingAttacks },
		{ "AttackID", lua_DeliriumGetAttackID },
		{ "Cycle", lua_DeliriumGetCycle },
		{ "State", lua_DeliriumGetState },
		{ "Angle", lua_DeliriumGetAngle },
		{ NULL, NULL }
	};
	luaL_setfuncs(L, functions, 0);
	lua_rawset(L, -3);
}

static void CreateDeliriumSetters(lua_State* L) {
	lua_pushstring(L, "__propset");
	lua_newtable(L);
	luaL_Reg functions[] = {
		{ "TransformationTimer", lua_DeliriumSetTransformationTimer },
		{ "RemainingAttacks", lua_DeliriumSetRemainingAttacks },
		{ "AttackID", lua_DeliriumSetAttackID },
		{ "Cycle", lua_DeliriumSetCycle },
		{ "State", lua_DeliriumSetState },
		{ "Angle", lua_DeliriumSetAngle },
		{ NULL, NULL }
	};
	luaL_setfuncs(L, functions, 0);
	lua_rawset(L, -3);
}

static void RegisterDeliriumFunctions(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "ToDelirium");
	lua_pushcfunction(L, lua_EntityToDelirium);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, delirium::DeliriumMetatable);

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, lua::luabridge::newIndexMetaMethod);
	lua_rawset(L, -3);

	lua_pushboolean(L, true);
	lua_rawsetp(L, -2, lua::luabridge::getIdentityKey());

	lua_pushstring(L, "__parent");
	lua::PushMetatable(L, lua::Metatables::ENTITY_NPC);
	lua_rawset(L, -3);

	lua_pushstring(L, "__type");
	lua_pushstring(L, "EntityDelirium");
	lua_rawset(L, -3);

	lua_pushstring(L, "__class");
	lua_pushstring(L, "EntityDelirium");
	lua_rawset(L, -3);

	CreateDeliriumGetters(L);
	CreateDeliriumSetters(L);

	lua_pushstring(L, "__const");
	lua_newtable(L);
	lua_rawset(L, -3);

	luaL_Reg functions[] = {
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);
	// RegisterMemoryFunctions(L);
	RegisterDeliriumFunctions(L);
}

extern "C" __declspec(dllexport) int ModInit(int argc, char** argv) {
	delirium::PatchSkipFrames();
	delirium::PatchCompanion();
	delirium::AddPreTransformationCallback();
	delirium::AddTransformationCallback();
	return 0;
}