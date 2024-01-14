#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_CapsuleConstructor) {
	Vector* position = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	Vector* multiplier = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	const float f1 = (float)luaL_checknumber(L, 3);

	Capsule* capsule = nullptr;
	if (lua_type(L, 4) == LUA_TNUMBER) {
		const float f2 = (float)luaL_checknumber(L, 4);
		capsule = lua::place<Capsule>(L, lua::metatables::CapsuleMT, position, multiplier, f1, f2);
	}
	else {
		capsule = lua::place<Capsule>(L, lua::metatables::CapsuleMT, position, multiplier, f1);
	}

	return 1;
}

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

LUA_FUNCTION(Lua_CapsuleGetPosition)
{
	Capsule* capsule = lua::GetUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *capsule->GetPosition();

	return 1;
}

LUA_FUNCTION(Lua_CapsuleGetVec2)
{
	Capsule* capsule = lua::GetUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = capsule->_vec2;

	return 1;
}
LUA_FUNCTION(Lua_CapsuleGetVec3)
{
	Capsule* capsule = lua::GetUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = capsule->_vec3;

	return 1;
}

LUA_FUNCTION(Lua_CapsuleGetDirection)
{
	Capsule* capsule = lua::GetUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = capsule->_direction;

	return 1;
}

LUA_FUNCTION(Lua_CapsuleGetUnkF1)
{
	Capsule* capsule = lua::GetUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	lua_pushnumber(L, capsule->_unkFloat1);

	return 1;
}

LUA_FUNCTION(Lua_CapsuleGetUnkF2)
{
	Capsule* capsule = lua::GetUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	lua_pushnumber(L, capsule->_unkFloat2);

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

LUA_FUNCTION(Lua_Capsule__gc) {
	Capsule* capsule = lua::GetUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	capsule->~Capsule();

	return 0;
}

static void RegisterCapsule(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY, "GetNullCapsule", Lua_EntityGetNullCapsule);
	lua::RegisterFunction(L, lua::Metatables::ENTITY, "GetCollisionCapsule", Lua_EntityGetCollisionCapsule);

	luaL_Reg functions[] = {
		{ "GetPosition", Lua_CapsuleGetPosition },
		{ "GetVec2", Lua_CapsuleGetVec2 },
		{ "GetVec3", Lua_CapsuleGetVec3 },
		{ "GetDirection", Lua_CapsuleGetDirection },
		{ "GetF1", Lua_CapsuleGetUnkF1 },
		{ "GetF2", Lua_CapsuleGetUnkF2 },
		{ "Collide", Lua_CapsuleCollide },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::CapsuleMT, lua::metatables::CapsuleMT, functions, Lua_Capsule__gc);
	lua_register(L, lua::metatables::CapsuleMT, Lua_CapsuleConstructor);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterCapsule(_state);
}