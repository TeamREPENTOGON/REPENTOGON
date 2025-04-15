#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"


LUA_FUNCTION(Lua_PEGetConditionType) {
	ProceduralEffect* pi = *lua::GetRawUserdata<ProceduralEffect**>(L, 1, lua::metatables::ProceduralEffectMT);
	lua_pushinteger(L, pi->effectConditionType);
	return 1;
}

LUA_FUNCTION(Lua_PEGetActionType) {
	ProceduralEffect* pi = *lua::GetRawUserdata<ProceduralEffect**>(L, 1, lua::metatables::ProceduralEffectMT);
	lua_pushinteger(L, pi->effectActionType);
	return 1;
}

LUA_FUNCTION(Lua_PEGetConditionProperty) {
	ProceduralEffect* pi = *lua::GetRawUserdata<ProceduralEffect**>(L, 1, lua::metatables::ProceduralEffectMT);
	lua_newtable(L);
	switch (pi->effectConditionType) {
	case ProceduralEffect::CONDITION_ENTITY_SPAWN:
		lua_pushnumber(L, pi->GetConditionData()->entitySpawn.type);
		lua_setfield(L, -2, "type");
		lua_pushnumber(L, pi->GetConditionData()->entitySpawn.variant);
		lua_setfield(L, -2, "variant");
		break;
	}
	return 1;
}

LUA_FUNCTION(Lua_PEGetActionProperty) {
	ProceduralEffect* pi = *lua::GetRawUserdata<ProceduralEffect**>(L, 1, lua::metatables::ProceduralEffectMT);
	lua_newtable(L);
	switch (pi->effectActionType)
	{
	case ProceduralEffect::ACTION_USE_ACTIVE_ITEM:
		lua_pushinteger(L, pi->GetActionData()->useActiveItem.collectibleId);
		lua_setfield(L, -2, "id");
		break;
	case ProceduralEffect::ACTION_ADD_TEMPRORY_EFFECT:
		lua_pushinteger(L, pi->GetActionData()->addTempraryEffect.collectibleId);
		lua_setfield(L, -2, "id");
		break;
	case ProceduralEffect::ACTION_SPAWN_ENTITY:
		lua_pushinteger(L, pi->GetActionData()->spawnEntity.type);
		lua_setfield(L, -2, "type");
		lua_pushinteger(L, pi->GetActionData()->spawnEntity.variant);
		lua_setfield(L, -2, "variant");
		break;
	case ProceduralEffect::ACTION_CONVERT_ENTITIES:
		lua_pushinteger(L, pi->GetActionData()->convertEntities.fromType);
		lua_setfield(L, -2, "fromType");
		lua_pushinteger(L, pi->GetActionData()->convertEntities.fromVariant);
		lua_setfield(L, -2, "fromVariant");
		lua_pushinteger(L, pi->GetActionData()->convertEntities.toType);
		lua_setfield(L, -2, "toType");
		lua_pushinteger(L, pi->GetActionData()->convertEntities.toVariant);
		lua_setfield(L, -2, "toVariant");
		break;
	case ProceduralEffect::ACTION_AREA_DAMAGE:
		lua_pushnumber(L, pi->GetActionData()->areaDamage.radius);
		lua_setfield(L, -2, "radius");
		lua_pushnumber(L, pi->GetActionData()->areaDamage.damage);
		lua_setfield(L, -2, "damage");
		lua_pushinteger(L, pi->GetActionData()->areaDamage.flags1);
		lua_setfield(L, -2, "flags1");
		lua_pushinteger(L, pi->GetActionData()->areaDamage.flags2);
		lua_setfield(L, -2, "flags2");
		break;
	case ProceduralEffect::ACTION_FART:
		lua_pushnumber(L, pi->GetActionData()->fart.fartScale);
		lua_setfield(L, -2, "scale");
		lua_pushnumber(L, pi->GetActionData()->fart.radius * 6.f / 85.f);
		lua_setfield(L, -2, "radius");
		break;
	}
	return 1;
}


LUA_FUNCTION(Lua_PEGetTriggerChance) {
	ProceduralEffect* pi = *lua::GetRawUserdata<ProceduralEffect**>(L, 1, lua::metatables::ProceduralEffectMT);
	float chance = pi->triggerChance;
	//manually fix the chance from 0 ~ 1 to the actual rate.
	switch (pi->effectConditionType)
	{
	case ProceduralEffect::CONDITION_TEAR_FIRE:
	case ProceduralEffect::CONDITION_ENEMY_HIT:
	case ProceduralEffect::CONDITION_ENTITY_SPAWN:
		chance *= 0.05f;
		break;
	case ProceduralEffect::CONDITION_ENEMY_KILL:
		chance *= 0.2f;
		break;
	default:
		break;
	}
	lua_pushnumber(L, chance);
	return 1;
}

static void RegisterProceduralItem(lua_State* L) {
	luaL_Reg functions[] = {
		{"GetConditionType", Lua_PEGetConditionType},
		{"GetActionType", Lua_PEGetActionType},
		{"GetConditionProperty", Lua_PEGetConditionProperty},
		{"GetActionProperty", Lua_PEGetActionProperty},
		{"GetTriggerChance", Lua_PEGetTriggerChance},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::ProceduralEffectMT, lua::metatables::ProceduralEffectMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterProceduralItem(_state);
}