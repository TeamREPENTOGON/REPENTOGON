#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../Patches/ASMPatches/ASMSplitTears.h"

LUA_FUNCTION(Lua_KnifeGetHitList) {
	Entity_Knife * knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	std::vector<unsigned int> hitList = *knife->GetHitEntities();

	lua_newtable(L);
	int idx = 1;
	for (int index : hitList) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, index);
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

LUA_FUNCTION(Lua_GetIsSwinging) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	lua_pushboolean(L, knife->_isSwinging);
	return 1;
}

LUA_FUNCTION(Lua_SetIsSwinging) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	knife->_isSwinging = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_GetIsSpinAttack) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	lua_pushboolean(L, knife->_isSpinAttack);
	return 1;
}

LUA_FUNCTION(Lua_SetIsSpinAttack) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	knife->_isSpinAttack = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_IsMultidimensionalTouched) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	lua_pushboolean(L, knife->_multidimensionalApplied);
	return 1;
}

LUA_FUNCTION(Lua_SetMultidimensionalTouched) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	knife->_multidimensionalApplied = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_IsPrismTouched) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	lua_pushboolean(L, knife->_prismApplied);
	return 1;
}

LUA_FUNCTION(Lua_SetPrismTouched) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	knife->_prismApplied = lua::luaL_checkboolean(L, 2);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetHitList", Lua_KnifeGetHitList },
		{ "GetIsSwinging", Lua_GetIsSwinging },
		{ "SetIsSwinging", Lua_SetIsSwinging },
		{ "GetIsSpinAttack", Lua_GetIsSpinAttack },
		{ "SetIsSpinAttack", Lua_SetIsSpinAttack },
		{ "IsMultidimensionalTouched", Lua_IsMultidimensionalTouched },
		{ "SetMultidimensionalTouched", Lua_SetMultidimensionalTouched },
		{ "IsPrismTouched", Lua_IsPrismTouched },
		{ "SetPrismTouched", Lua_SetPrismTouched },
		{ "FireSplitTear", SplitTears::Lua_FireSplitTear },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_KNIFE, functions);
}