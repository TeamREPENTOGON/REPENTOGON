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
	Vector offsetVec;
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		offsetVec = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	}
	else {
		offsetVec = Vector(0, 0);
	}

	Capsule* ud = (Capsule*)lua_newuserdata(L, sizeof(Capsule));
	*ud = ent->GetCollisionCapsule(&offsetVec);
	luaL_setmetatable(L, lua::metatables::CapsuleMT);
	return 1;
};

LUA_FUNCTION(Lua_CapsuleGetPosition)
{
	Capsule* capsule = lua::GetRawUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *capsule->GetPosition();

	return 1;
}

LUA_FUNCTION(Lua_CapsuleGetStartPoint)
{
	Capsule* capsule = lua::GetRawUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = capsule->_vec2;

	return 1;
}
LUA_FUNCTION(Lua_CapsuleGetEndPoint)
{
	Capsule* capsule = lua::GetRawUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = capsule->_vec3;

	return 1;
}

LUA_FUNCTION(Lua_CapsuleGetDirection)
{
	Capsule* capsule = lua::GetRawUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = capsule->_direction;

	return 1;
}

LUA_FUNCTION(Lua_CapsuleGetSize)
{
	Capsule* capsule = lua::GetRawUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	lua_pushnumber(L, capsule->_unkFloat1);

	return 1;
}

LUA_FUNCTION(Lua_CapsuleGetSizeDifference)
{
	Capsule* capsule = lua::GetRawUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	lua_pushnumber(L, capsule->_unkFloat2);

	return 1;
}

LUA_FUNCTION(Lua_CapsuleCollide)
{
	Capsule* cap1 = lua::GetRawUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	Capsule* cap2 = lua::GetRawUserdata<Capsule*>(L, 2, lua::metatables::CapsuleMT);
	Vector* vec = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	lua_pushboolean(L, cap1->Collide(cap1, cap2, vec));
	return 1;
}

LUA_FUNCTION(Lua_Capsule__gc) {
	Capsule* capsule = lua::GetRawUserdata<Capsule*>(L, 1, lua::metatables::CapsuleMT);
	capsule->~Capsule();

	return 0;
}

static void RegisterCapsule(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY, "GetNullCapsule", Lua_EntityGetNullCapsule);
	lua::RegisterFunction(L, lua::Metatables::ENTITY, "GetCollisionCapsule", Lua_EntityGetCollisionCapsule);

	luaL_Reg functions[] = {
		{ "GetPosition", Lua_CapsuleGetPosition },
		{ "GetStartPoint", Lua_CapsuleGetStartPoint },
		{ "GetEndPoint", Lua_CapsuleGetEndPoint },
		{ "GetDirection", Lua_CapsuleGetDirection },
		{ "GetSize", Lua_CapsuleGetSize },
		{ "GetSizeDifference", Lua_CapsuleGetSizeDifference },
		{ "Collide", Lua_CapsuleCollide },

		// Deprecated methods
		{ "GetVec2", Lua_CapsuleGetStartPoint },
		{ "GetVec3", Lua_CapsuleGetEndPoint },
		{ "GetF1", Lua_CapsuleGetSize },
		{ "GetF2", Lua_CapsuleGetSizeDifference },

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