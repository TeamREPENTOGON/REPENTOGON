#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "LuaWeapon.h"

Weapon* WeaponData::GetWeaponFromLua(lua_State* L, int idx) {
	return lua::GetRawUserdata<WeaponData*>(L, idx, lua::metatables::WeaponMT)->weapon;
}

LUA_FUNCTION(Lua_CreateWeapon) {
	WeaponData* ud = new (lua_newuserdata(L, sizeof(WeaponData))) WeaponData;
	int wepType = (int)luaL_checkinteger(L, 1);
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");

	if (!(WEAPON_NULL <= wepType && wepType < NUM_WEAPON_TYPES))
	{
		return luaL_argerror(L, 1, "Invalid WeaponType");
	}

	ud->weapon = Isaac::CreateWeapon((WeaponType)wepType, ent);
	luaL_setmetatable(L, lua::metatables::WeaponMT);
	return 1;
}

LUA_FUNCTION(Lua_DestoryWeapon) {
	WeaponData* weapon = lua::GetRawUserdata<WeaponData*>(L, 1, lua::metatables::WeaponMT);
	if (std::holds_alternative<Entity_Player*>(weapon->owner)) {
		Entity_Player* owner = std::get<Entity_Player*>(weapon->owner);
		Isaac::DestoryWeapon(owner->GetWeapon(weapon->slot));
	}
	else if (std::holds_alternative<Entity_Familiar*>(weapon->owner)) {
		Entity_Familiar* owner = std::get<Entity_Familiar*>(weapon->owner);
		Isaac::DestoryWeapon(owner->GetWeapon());
	}
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetWeapon) {
	Entity_Player* player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int index = (int)luaL_checkinteger(L, 2);
	if (index < 0 || index > 4) {
		return luaL_argerror(L, 2, "Index must be between 0 and 4");
	}
	WeaponData* ud = (WeaponData*)lua_newuserdata(L, sizeof(WeaponData));
	ud->weapon = *player->GetWeapon(index);
	if (!ud->weapon) {
		lua_pop(L, 1); // Remove the userdata as it is useless
		lua_pushnil(L);
		return 1;
	}
	ud->owner = player;
	ud->slot = index;
	luaL_setmetatable(L, lua::metatables::WeaponMT);
	return 1;
}

LUA_FUNCTION(Lua_FanGetWeapon) {
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	WeaponData* ud = (WeaponData*)lua_newuserdata(L, sizeof(WeaponData));
	ud->weapon = *fam->GetWeapon();
	if (!ud->weapon) {
		lua_pop(L, 1); // Remove the userdata as it is useless
		lua_pushnil(L);
		return 1;
	}
	ud->owner = fam;
	luaL_setmetatable(L, lua::metatables::WeaponMT);
	return 1;
}

LUA_FUNCTION(Lua_WeaponGetOwner) {
	WeaponData* weapon = lua::GetRawUserdata<WeaponData*>(L, 1, lua::metatables::WeaponMT);
	Entity* ent = weapon->weapon->GetOwner();
	if (!ent) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
	}
	return 1;
}

LUA_FUNCTION(Lua_WeaponGetFireDelay) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushnumber(L, *weapon->GetFireDelay());
	return 1;
}

LUA_FUNCTION(Lua_WeaponSetFireDelay) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	float delay = (float)luaL_checknumber(L, 2);
	*weapon->GetFireDelay() = delay;
	return 0;
}

LUA_FUNCTION(Lua_WeaponGetMaxFireDelay) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushnumber(L, weapon->GetMaxFireDelay());
	return 1;
}

LUA_FUNCTION(Lua_WeaponGetCharge) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushnumber(L, *weapon->GetCharge());
	return 1;
}

LUA_FUNCTION(Lua_WeaponSetCharge) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	float charge = (float)luaL_checknumber(L, 2);
	*weapon->GetCharge() = charge;
	return 0;
}

LUA_FUNCTION(Lua_WeaponGetDirection)
{
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *weapon->GetDirection();

	return 1;
}

LUA_FUNCTION(Lua_WeaponGetWeaponType) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushinteger(L, weapon->GetWeaponType());
	return 1;
}

LUA_FUNCTION(Lua_WeaponGetModifiers) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushinteger(L, *weapon->GetModifiers());
	return 1;
}

LUA_FUNCTION(Lua_WeaponSetModifiers) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	const unsigned int modifiers = (unsigned int)luaL_checkinteger(L, 2);
	*weapon->GetModifiers() |= modifiers;
	return 1;
}

LUA_FUNCTION(Lua_WeaponGetNumFired) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushinteger(L, weapon->GetNumFired());
	return 1;
}

LUA_FUNCTION(Lua_WeaponPlayItemAnim) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	unsigned int itemID = (unsigned int)luaL_checkinteger(L, 2);
	int anim = (int)luaL_checkinteger(L, 3);
	const Vector* position = lua::GetLuabridgeUserdata<Vector*>(L, 4, lua::Metatables::VECTOR, "Vector");
	float charge = (float)luaL_checknumber(L, 5);
	weapon->PlayItemAnim(itemID, anim, *position, charge);
	return 0;
}

LUA_FUNCTION(Lua_WeaponIsAxisAligned) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushboolean(L, weapon->IsAxisAligned());
	return 1;
}

LUA_FUNCTION(Lua_WeaponIsItemAnimFinished) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	unsigned int item = (unsigned int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, weapon->IsItemAnimFinished(item));
	return 1;
}

LUA_FUNCTION(Lua_WeaponClearItemAnim) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	unsigned int item = (unsigned int)luaL_checkinteger(L, 2);
	weapon->ClearItemAnim(item);
	return 0;
}

LUA_FUNCTION(Lua_WeaponSetHeadLockTime) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	unsigned int time = (unsigned int)luaL_checkinteger(L, 2);
	weapon->SetHeadLockTime(time);
	return 0;
}

LUA_FUNCTION(Lua_WeaponGetMainEntity) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua::luabridge::UserdataPtr::push(L, weapon->GetMainEntity(), lua::GetMetatableKey(lua::Metatables::ENTITY));
	return 1;
}

static void RegisterWeapon(lua_State* L) {
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "CreateWeapon", Lua_CreateWeapon);
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "DestroyWeapon", Lua_DestoryWeapon);

	lua::RegisterFunction(L, lua::Metatables::ENTITY_PLAYER, "GetWeapon", Lua_PlayerGetWeapon);
	lua::RegisterFunction(L, lua::Metatables::ENTITY_FAMILIAR, "GetWeapon", Lua_FanGetWeapon);

	luaL_Reg functions[] = {
		{ "GetOwner", Lua_WeaponGetOwner },
		{ "GetFireDelay", Lua_WeaponGetFireDelay },
		{ "SetFireDelay", Lua_WeaponSetFireDelay },
		{ "GetMaxFireDelay", Lua_WeaponGetMaxFireDelay },
		{ "GetCharge", Lua_WeaponGetCharge },
		{ "SetCharge", Lua_WeaponSetCharge },
		{ "GetDirection", Lua_WeaponGetDirection },
		{ "GetWeaponType", Lua_WeaponGetWeaponType },
		{ "GetModifiers", Lua_WeaponGetModifiers },
		{ "SetModifiers", Lua_WeaponSetModifiers },
		{ "GetNumFired", Lua_WeaponGetNumFired },
		{ "PlayItemAnim", Lua_WeaponPlayItemAnim },
		{ "IsAxisAligned", Lua_WeaponIsAxisAligned },
		{ "IsItemAnimFinished", Lua_WeaponIsItemAnimFinished },
		{ "ClearItemAnim", Lua_WeaponClearItemAnim },
		{ "SetHeadLockTime", Lua_WeaponSetHeadLockTime },
		{ "GetMainEntity", Lua_WeaponGetMainEntity },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::WeaponMT, lua::metatables::WeaponMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterWeapon(_state);
}