#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* WeaponMT = "WeaponMT";

static int Lua_CreateWeapon(lua_State* L) {
	Weapon** ud = (Weapon**)lua_newuserdata(L, sizeof(Weapon*));
	int wepType = luaL_checkinteger(L, 2);
	Entity* ent = lua::GetUserdata<Entity*>(L, 3, lua::Metatables::ENTITY, "Entity");
	*ud = Isaac::CreateWeapon((WeaponType)wepType, ent);
	luaL_setmetatable(L, WeaponMT);
	return 1;
}

static int Lua_PlayerGetWeapon(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Weapon* toLua = player->GetWeapon();
	if (toLua == nullptr) {
		lua_pushnil(L);
		return 1;
	}
	Weapon** luaWeapon = (Weapon**)lua_newuserdata(L, sizeof(Weapon*));
	*luaWeapon = toLua;
	luaL_setmetatable(L, WeaponMT);
	return 1;
}

//static int Lua_WeaponGetFireDelay(lua_State* L) {
//	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, WeaponMT);
//	lua_pushnumber(L, weapon->GetFireDelay());
//	return 1;
//}

static int Lua_WeaponGetMaxFireDelay(lua_State* L) {
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, WeaponMT);
	lua_pushnumber(L, weapon->GetMaxFireDelay());
	return 1;
}

static int Lua_WeaponGetWeaponType(lua_State* L) {
	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, WeaponMT);
	lua_pushinteger(L, weapon->GetWeaponType());
	return 1;
}

//static int Lua_WeaponGetWeaponModifiers(lua_State* L) {
//	Weapon* weapon = *lua::GetUserdata<Weapon**>(L, 1, WeaponMT);
//	lua_pushinteger(L, weapon->GetWeaponModifiers());
//	return 1;
//}

static void RegisterWeapon(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "CreateWeapon");
	lua_pushcfunction(L, Lua_CreateWeapon);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, WeaponMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		//{ "GetFireDelay", Lua_WeaponGetFireDelay },
		{ "GetMaxFireDelay", Lua_WeaponGetMaxFireDelay },
		{ "GetWeaponType", Lua_WeaponGetWeaponType },
		//{ "GetWeaponModifiers", Lua_WeaponGetWeaponModifiers },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ENTITY_PLAYER;
	lua::RegisterFunction(state, mt, "GetWeapon", Lua_PlayerGetWeapon);
	RegisterWeapon(state);
}