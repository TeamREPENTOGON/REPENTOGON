#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"
#include "Delirium.h"

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

namespace delirium {
	LUA_FUNCTION(Lua_Delirium_Transform) {
		Entity_NPC* delirium = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityDelirium");;
		int type = luaL_checkinteger(L, 2);
		if (type < 10) { // ENTITY_GAPER
			return luaL_error(L, "Invalid EntityType %d for Delirium\n", type);
		}
		int variant = luaL_optinteger(L, 3, 0);
		bool callback = lua::luaL_optboolean(L, 4, false);

		delirium::ForcedTransformations[delirium] = std::make_tuple(type, variant, callback);
		*delirium->GetDeliriumTransformationTimer() = 1;
		return 0;
	}
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
		{ "Transform", delirium::Lua_Delirium_Transform },
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

HOOK_METHOD_PRIORITY(LuaEngine, Init, 100,  (bool debug) -> void) {
	super(debug);
	this->RunBundledScript("resources-delirium/scripts/enums.lua");
}

extern "C" __declspec(dllexport) int ModInit(int argc, char** argv) {
	delirium::PatchSkipFrames();
	delirium::PatchCompanion();
	// delirium::AddPreTransformationCallback();
	delirium::AddTransformationCallback();
	delirium::PatchVadeRetro();
	return 0;
}