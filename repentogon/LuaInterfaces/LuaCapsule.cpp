#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EntityGetNullCapsule) {
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	const char* str = luaL_checkstring(L, 2);
	Capsule res = ent->GetNullCapsule(str);

	Capsule* ud = (Capsule*)lua_newuserdata(L, sizeof(Capsule));
	*ud = res;
	luaL_setmetatable(L, lua::metatables::CapsuleMT);
	return 1;
};

LUA_FUNCTION(Lua_EntityGetCollisionCapsule) {
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector* vec = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

	Capsule* ud = (Capsule*)lua_newuserdata(L, sizeof(Capsule));
	*ud = ent->GetCollisionCapsule(vec);
	luaL_setmetatable(L, lua::metatables::CapsuleMT);
	return 1;
};

LUA_FUNCTION(Lua_CapsuleGetVec1)
{
	Capsule* capsule = lua::GetUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *capsule->GetVec1();

	return 1;
}

LUA_FUNCTION(Lua_CapsuleCollide)
{
	Capsule* cap1 = lua::GetUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Capsule* cap2 = lua::GetUserdata<Capsule*>(L, 2, lua::metatables::CapsuleMT);
	Vector* vec = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	lua_pushboolean(L, cap1->Collide(cap1, cap2, vec));
	return 1;
}

static void RegisterCapsule(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY, "GetNullCapsule", Lua_EntityGetNullCapsule);
	lua::RegisterFunction(L, lua::Metatables::ENTITY, "GetCollisionCapsule", Lua_EntityGetCollisionCapsule);

	luaL_Reg functions[] = {
		{ "GetVec1", Lua_CapsuleGetVec1 },
		{ "Collide", Lua_CapsuleCollide },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::CapsuleMT, lua::metatables::CapsuleMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterCapsule(_state);
}