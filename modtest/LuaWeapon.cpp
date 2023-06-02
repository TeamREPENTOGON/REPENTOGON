#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_CreateWeapon(lua_State* L) {
	Weapon** ud = (Weapon**)lua_newuserdata(L, sizeof(Weapon*));
	int wepType = luaL_checkinteger(L, 1);
	Entity* ent = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	*ud = Isaac::CreateWeapon((WeaponType)wepType, ent);
	luaL_setmetatable(L, lua::metatables::WeaponMT);
	return 1;
}

static int Lua_DestoryWeapon(lua_State* L) {
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, lua::metatables::WeaponMT);
	Isaac::DestoryWeapon(&weapon);
	return 0;
}

static int Lua_PlayerGetWeapon(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int index = luaL_checkinteger(L, 2);
	if (index < 0 || index > 4) {
		return luaL_argerror(L, 2, "Index must be between 0 and 4");
	}
	Weapon** ud = (Weapon**)lua_newuserdata(L, sizeof(Weapon*));
	*ud = *player->GetWeapon(index);
	if (*ud == NULL) {
		lua_pushnil(L);
		return 1;
	}
	luaL_setmetatable(L, lua::metatables::WeaponMT);
	return 1;
}

static int Lua_FanGetWeapon(lua_State* L) {
	Entity_Familiar* fam = lua::GetUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	Weapon** ud = (Weapon**)lua_newuserdata(L, sizeof(Weapon*));
	*ud = *fam->GetWeapon();
	if (*ud == NULL) {
		lua_pushnil(L);
		return 1;
	}
	luaL_setmetatable(L, lua::metatables::WeaponMT);
	return 1;
}

static int Lua_WeaponGetOwner(lua_State* L) {
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, lua::metatables::WeaponMT);
	Entity* ent = weapon->GetOwner();
	if (!ent) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, ent, lua::GetMetatableKey(lua::Metatables::ENTITY));
	}
	return 1;
}

static int Lua_WeaponGetFireDelay(lua_State* L) {
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, lua::metatables::WeaponMT);
	lua_pushnumber(L, weapon->GetFireDelay());
	return 1;
}

static int Lua_WeaponGetMaxFireDelay(lua_State* L) {
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, lua::metatables::WeaponMT);
	lua_pushnumber(L, weapon->GetMaxFireDelay());
	return 1;
}

static int Lua_WeaponGetCharge(lua_State* L) {
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, lua::metatables::WeaponMT);
	lua_pushnumber(L, weapon->GetCharge());
	return 1;
}

static int Lua_WeaponGetDirection(lua_State* L)
{
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, lua::metatables::WeaponMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *weapon->GetDirection();

	return 1;
}

static int Lua_WeaponGetWeaponType(lua_State* L) {
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, lua::metatables::WeaponMT);
	lua_pushinteger(L, weapon->GetWeaponType());
	return 1;
}

static int Lua_WeaponGetModifiers(lua_State* L) {
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, lua::metatables::WeaponMT);
	lua_pushinteger(L, weapon->GetModifiers());
	return 1;
}

static int Lua_WeaponGetNumFired(lua_State* L) {
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, lua::metatables::WeaponMT);
	lua_pushinteger(L, weapon->GetNumFired());
	return 1;
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
		{ "GetMaxFireDelay", Lua_WeaponGetMaxFireDelay },
		{ "GetCharge", Lua_WeaponGetCharge },
		{ "GetDirection", Lua_WeaponGetDirection },
		{ "GetWeaponType", Lua_WeaponGetWeaponType },
		{ "GetModifiers", Lua_WeaponGetModifiers },
		{ "GetNumFired", Lua_WeaponGetNumFired },
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