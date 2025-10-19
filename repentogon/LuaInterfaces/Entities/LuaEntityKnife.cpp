#include <algorithm>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../Patches/ASMPatches/ASMSplitTears.h"
#include "../../Patches/EntityPlus.h"

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

LUA_FUNCTION(Lua_GetHitboxParentKnife) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	EntityKnifePlus* entityPlus = GetEntityKnifePlus(knife);
	Entity* parentKnife = entityPlus ? entityPlus->hitboxSource.GetReference() : nullptr;
	if (parentKnife && parentKnife->_type == ENTITY_KNIFE) {
		lua::luabridge::UserdataPtr::push(L, parentKnife, lua::Metatables::ENTITY_KNIFE);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

LUA_FUNCTION(Lua_SetHitboxParentKnife) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	Entity_Knife* parentKnife = !lua_isnoneornil(L, 2) ? lua::GetLuabridgeUserdata<Entity_Knife*>(L, 2, lua::Metatables::ENTITY_KNIFE, "EntityKnife") : nullptr;
	EntityKnifePlus* entityPlus = GetEntityKnifePlus(knife);
	if (entityPlus) {
		entityPlus->hitboxSource.SetReference(parentKnife);
	}
	return 0;
}

LUA_FUNCTION(Lua_KnifeRemoveFromHitList) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	auto hitList = knife->GetHitEntities();

	auto iterator = std::find(hitList->begin(), hitList->end(), entity->GetHitListIndex());

	if (iterator != hitList->end()) {
		std::swap(*iterator, hitList->back());
		hitList->pop_back();
	}

	return 0;
}

LUA_FUNCTION(Lua_KnifeAddToHitList) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	int hitListIndex = entity->GetHitListIndex();
	auto hitList = knife->GetHitEntities();
	bool found = std::find(hitList->begin(), hitList->end(), hitListIndex) != hitList->end();

	if (!found) {
		hitList->push_back(hitListIndex);
	}

	return 0;
}

LUA_FUNCTION(Lua_KnifeInHitList) {
	Entity_Knife* knife = lua::GetLuabridgeUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	int hitListIndex = entity->GetHitListIndex();
	auto hitList = knife->GetHitEntities();
	bool found = std::find(hitList->begin(), hitList->end(), hitListIndex) != hitList->end();

	lua_pushboolean(L, found);

	return 1;
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
		{ "GetHitboxParentKnife", Lua_GetHitboxParentKnife },
		{ "SetHitboxParentKnife", Lua_SetHitboxParentKnife },
		{ "RemoveFromHitList", Lua_KnifeRemoveFromHitList },
		{ "AddToHitList", Lua_KnifeAddToHitList },
		{ "InHitList", Lua_KnifeInHitList },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_KNIFE, functions);
}