#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "LuaWeapon.h"

Weapon* WeaponData::GetWeaponFromLua(lua_State* L, int idx) {
	return lua::GetUserdata<WeaponData*>(L, idx, lua::metatables::WeaponMT)->weapon;
}

static int Lua_CreateWeapon(lua_State* L) {
	WeaponData* ud = new (lua_newuserdata(L, sizeof(WeaponData))) WeaponData;
	int wepType = (int)luaL_checkinteger(L, 1);
	Entity* ent = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	ud->weapon = Isaac::CreateWeapon((WeaponType)wepType, ent);
	luaL_setmetatable(L, lua::metatables::WeaponMT);
	return 1;
}

static int Lua_DestoryWeapon(lua_State* L) {
	WeaponData* weapon = lua::GetUserdata<WeaponData*>(L, 1, lua::metatables::WeaponMT);
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

static int Lua_PlayerGetWeapon(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
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

static int Lua_FanGetWeapon(lua_State* L) {
	Entity_Familiar* fam = lua::GetUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
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

static int Lua_WeaponGetOwner(lua_State* L) {
	WeaponData* weapon = lua::GetUserdata<WeaponData*>(L, 1, lua::metatables::WeaponMT);
	Entity* ent = weapon->weapon->GetOwner();
	if (!ent) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
	}
	return 1;
}

static int Lua_WeaponGetFireDelay(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushnumber(L, *weapon->GetFireDelay());
	return 1;
}

static int Lua_WeaponSetFireDelay(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	float delay = (float)luaL_checknumber(L, 2);
	*weapon->GetCharge() = delay;
	return 0;
}

static int Lua_WeaponGetMaxFireDelay(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushnumber(L, weapon->GetMaxFireDelay());
	return 1;
}

static int Lua_WeaponGetCharge(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushnumber(L, *weapon->GetCharge());
	return 1;
}

static int Lua_WeaponSetCharge(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	float charge = (float)luaL_checknumber(L, 2);
	*weapon->GetCharge() = charge;
	return 0;
}

static int Lua_WeaponGetDirection(lua_State* L)
{
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *weapon->GetDirection();

	return 1;
}

static int Lua_WeaponGetWeaponType(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushinteger(L, weapon->GetWeaponType());
	return 1;
}

static int Lua_WeaponGetModifiers(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushinteger(L, weapon->GetModifiers());
	return 1;
}

static int Lua_WeaponGetNumFired(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushinteger(L, weapon->GetNumFired());
	return 1;
}

static int Lua_WeaponPlayItemAnim(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	unsigned int itemID = (unsigned int)luaL_checkinteger(L, 2);
	int anim = (int)luaL_checkinteger(L, 3);
	const Vector* position = lua::GetUserdata<Vector*>(L, 4, lua::Metatables::VECTOR, "Vector");
	float charge = (float)luaL_checknumber(L, 5);
	weapon->PlayItemAnim(itemID, anim, *position, charge);
	return 0;
}

static int Lua_WeaponIsAxisAligned(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	lua_pushboolean(L, weapon->IsAxisAligned());
	return 1;
}

static int Lua_WeaponIsItemAnimFinished(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	unsigned int item = (unsigned int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, weapon->IsItemAnimFinished(item));
	return 1;
}

static int Lua_WeaponClearItemAnim(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	unsigned int item = (unsigned int)luaL_checkinteger(L, 2);
	weapon->ClearItemAnim(item);
	return 0;
}

static int Lua_WeaponSetHeadLockTime(lua_State* L) {
	Weapon* weapon = WeaponData::GetWeaponFromLua(L, 1);
	unsigned int time = (unsigned int)luaL_checkinteger(L, 2);
	weapon->SetHeadLockTime(time);
	return 0;
}

static void RegisterWeapon(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "CreateWeapon");
	lua_pushcfunction(L, Lua_CreateWeapon);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "DestroyWeapon");
	lua_pushcfunction(L, Lua_DestoryWeapon);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, lua::metatables::WeaponMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

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
		{ "GetNumFired", Lua_WeaponGetNumFired },
		{ "PlayItemAnim", Lua_WeaponPlayItemAnim },
		{ "IsAxisAligned", Lua_WeaponIsAxisAligned },
		{ "IsItemAnimFinished", Lua_WeaponIsItemAnimFinished },
		{ "ClearItemAnim", Lua_WeaponClearItemAnim },
		{ "SetHeadLockTime", Lua_WeaponSetHeadLockTime },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterWeapon(state);
	lua::Metatables mt = lua::Metatables::ENTITY_PLAYER;
	lua::RegisterFunction(state, mt, "GetWeapon", Lua_PlayerGetWeapon);
	lua::RegisterFunction(state, lua::Metatables::ENTITY_FAMILIAR, "GetWeapon", Lua_FanGetWeapon);
}