#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

static void* VectorKey = nullptr;

LUA_FUNCTION(Lua_VectorT_Constructor) {
	float x = (float) luaL_checknumber(L, 2);
	float y = (float) luaL_checknumber(L, 3);

	Vector vec(x, y);
	lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, vec);
	return 1;
}

static Vector Zero(0, 0), One(1, 1);

LUA_FUNCTION(Lua_VectorT_FromAngle) {
	float angle = (float) (luaL_checknumber(L, 1) * M_PI / 180);
	Vector vec(cos(angle), sin(angle));
	lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, vec);
	return 1;
}

LUA_FUNCTION(Lua_VectorT_Globals) {
	const char* name = luaL_checkstring(L, 2);
	if (!strcmp(name, "Zero")) {
		lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, Zero);
	}
	else if (!strcmp(name, "One")) {
		lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, One);
	}
	else {
		lua_pushnil(L);
	}

	return 1;
}

LUA_FUNCTION(Lua_VectorT_NewIndex) {
	return luaL_error(L, "Vector table content's is read only");
}

LUA_FUNCTION(Lua_VectorUD_metamul) {
	int t1 = lua_type(L, 1);
	int t2 = lua_type(L, 2);

	if (t1 == LUA_TNUMBER) {
		float mult = (float) luaL_checknumber(L, 1);
		Vector* vector = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");
		Vector result(mult * vector->x, mult * vector->y);
		lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, result);
	}
	else if (t2 == LUA_TNUMBER) {
		float mult = (float) luaL_checknumber(L, 2);
		Vector* vector = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");
		Vector result(mult * vector->x, mult * vector->y);
		lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, result);
	}
	else if (t1 == t2 && t1 == LUA_TUSERDATA) {
		Vector* a = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");
		Vector* b = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");
		Vector result(a->x * b->x, a->y * b->y);
		lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, result);
	}
	else {
		luaL_error(L, "Invalid types for multiplication: %s * %s\n", lua_typename(L, t1), lua_typename(L, t2));
	}

	return 1;
}

LUA_FUNCTION(Lua_VectorUD_metadiv) {
	int t1 = lua_type(L, 1);
	int t2 = lua_type(L, 2);

	if (t1 == LUA_TNUMBER) {
		return luaL_error(L, "Cannot left multiply a Vector with a number");
	}
	else if (t2 == LUA_TNUMBER) {
		float div = (float) luaL_checknumber(L, 2);
		if (div == 0) {
			return luaL_error(L, "Divide by zero");
		}
		Vector* vector = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");
		Vector result(vector->x / div, vector->y / div);
		lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, result);
	}
	else if (t1 == t2 && t1 == LUA_TUSERDATA) {
		Vector* a = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");
		Vector* b = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");

		if (b->x == 0 || b->y == 0) {
			return luaL_error(L, "Divide by zero");
		}
		Vector result(a->x / b->x, a->y / b->y);
		lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, result);
	}
	else {
		luaL_error(L, "Invalid types for division: %s * %s\n", lua_typename(L, t1), lua_typename(L, t2));
	}

	return 1;
}

LUA_FUNCTION(Lua_VectorUD_metaadd) {
	Vector* v1 = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");
	Vector* v2 = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");

	Vector result(v1->x + v2->x, v1->y + v2->y);
	lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, result);
	return 1;
}

LUA_FUNCTION(Lua_VectorUD_metasub) {
	Vector* v1 = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");
	Vector* v2 = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR, "Vector");

	Vector result(v1->x - v2->x, v1->y - v2->y);
	lua::luabridge::UserdataValue<Vector>::push(L, VectorKey, result);
	return 1;
}

/*HOOK_METHOD(LuaEngine, Init, (bool debug) -> void) {
	super(debug);

	VectorKey = lua::GetMetatableKey(lua::Metatables::VECTOR);

	lua_State* L = _state;
	lua::LuaStackProtector protector(L);
	lua_newtable(L); // Vector
	// Metatable
	lua_newtable(L); // Vector's metatable
	// Restore the class field of the global Vector table's metatable
	lua_pushstring(L, "__class");
	lua::PushMetatable(L, lua::Metatables::VECTOR);
	lua_rawset(L, -3);
	luaL_Reg funcs[] = {
		{ "__call", Lua_VectorT_Constructor },
		{ "__index", Lua_VectorT_Globals },
		{ "__newindex", Lua_VectorT_NewIndex },
		{ NULL, NULL }
	};
	luaL_setfuncs(L, funcs, 0);
	lua_setmetatable(L, -2);
	lua_pushstring(L, "FromAngle");
	lua_pushcfunction(L, Lua_VectorT_FromAngle);
	lua_rawset(L, -3);
	lua_setglobal(L, "Vector");

	lua::Metatables metas[2] = { lua::Metatables::VECTOR, lua::Metatables::CONST_VECTOR };
	for (lua::Metatables meta : metas) {
		lua::PushMetatable(L, meta);
		lua_pushstring(L, "__name");
		lua_pushstring(L, "Vector");
		lua_rawset(L, -3);
		lua_pushstring(L, "__mul");
		lua_pushcfunction(L, Lua_VectorUD_metamul);
		lua_rawset(L, -3);
		lua_pushstring(L, "__div");
		lua_pushcfunction(L, Lua_VectorUD_metadiv);
		lua_rawset(L, -3);
		lua_pushstring(L, "__add");
		lua_pushcfunction(L, Lua_VectorUD_metaadd);
		lua_rawset(L, -3);
		lua_pushstring(L, "__sub");
		lua_pushcfunction(L, Lua_VectorUD_metasub);
		lua_rawset(L, -3);
		lua_pop(L, 1);
	}
}*/