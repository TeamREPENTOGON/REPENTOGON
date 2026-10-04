#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../LuaClasses.h"

MOD_EXPORT float L_Weapon_GetMaxCharge(Weapon* weapon) {
	return weapon->GetMaxCharge();
}

MOD_EXPORT void L_Weapon_PlayItemAnim(Weapon* weapon, unsigned int itemID, int anim, Vector* position, float charge) {
	weapon->PlayItemAnim(itemID, anim, *position, charge);
}

MOD_EXPORT bool L_Weapon_IsAxisAligned(Weapon* weapon) {
	return weapon->IsAxisAligned();
}

MOD_EXPORT bool L_Weapon_IsItemAnimFinished(Weapon* weapon, unsigned int itemID) {
	return weapon->IsItemAnimFinished(itemID);
}

MOD_EXPORT void L_Weapon_ClearItemAnim(Weapon* weapon, unsigned int itemID) {
	weapon->ClearItemAnim(itemID);
}

MOD_EXPORT void L_Weapon_SetHeadLockTime(Weapon* weapon, int time) {
	weapon->SetHeadLockTime(time);
}

LUA_FUNCTION(Lua_WeaponGetOwner) {
	Weapon* weapon = LuaWeapon::Get(L, 1);
	Entity* ent = weapon->GetOwner();
	if (!ent) {
		lua_pushnil(L);
	}
	else {
		LuaEntity::PushPtr(L, ent);
	}
	return 1;
}

LUA_FUNCTION(Lua_WeaponGetMainEntity) {
	Weapon* weapon = LuaWeapon::Get(L, 1);
	Entity* ent = weapon->GetMainEntity();
	if (!ent) {
		lua_pushnil(L);
	}
	else {
		LuaEntity::PushPtr(L, ent);
	}
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	lua_register(_state, "__Lua_Weapon_GetOwner", Lua_WeaponGetOwner);
	lua_register(_state, "__Lua_Weapon_GetMainEntity", Lua_WeaponGetMainEntity);

	super();
}
