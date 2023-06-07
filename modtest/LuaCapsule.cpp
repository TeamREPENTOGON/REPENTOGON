#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* CapsuleMT = "Capsule";

static int Lua_EntityGetNullCapsule(lua_State* L) {
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	const char* str = luaL_checkstring(L, 2);
	Capsule res;
	res = *ent->GetNullCapsule(&res, str);

	Capsule** ud = (Capsule**)lua_newuserdata(L, sizeof(Capsule*));
	*ud = &res;
	luaL_setmetatable(L, CapsuleMT);
	return 1;
};

static int Lua_EntityGetCollisionCapsule(lua_State* L) {
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	const Vector* vec = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Capsule res;
	res = *ent->GetCollisionCapsule(&res, *vec);

	Capsule* ud = (Capsule*)lua_newuserdata(L, sizeof(Capsule));
	*ud = res; //lol
	luaL_setmetatable(L, CapsuleMT);
	return 1;
};

static int Lua_CapsuleGetVec1(lua_State* L)
{
	Capsule* capsule = *lua::GetUserdata<Capsule**>(L, 1, CapsuleMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *capsule->GetVec1();

	return 1;
}

static int Lua_CapsuleGetSus(lua_State* L)
{
	Capsule* capsule = *lua::GetUserdata<Capsule**>(L, 1, CapsuleMT);
	lua_pushnumber(L, *capsule->GetSus());

	return 1;
}

static void RegisterCapsule(lua_State* L) {

	luaL_newmetatable(L, CapsuleMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetVec1", Lua_CapsuleGetVec1 },
		//{ "GetVec2", Lua_AmbushStartChallenge },
		{ "GetVar", Lua_CapsuleGetSus },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterCapsule(state);
	lua::RegisterFunction(state, lua::Metatables::ENTITY, "GetNullCapsule", Lua_EntityGetNullCapsule);
	lua::RegisterFunction(state, lua::Metatables::ENTITY, "GetCollisionCapsule", Lua_EntityGetCollisionCapsule);
}