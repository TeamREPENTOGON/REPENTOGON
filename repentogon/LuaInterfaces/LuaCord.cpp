#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "../Reimplementations/Rope.hpp"

LUA_FUNCTION(Lua_CreateCord) {
	printf("lua: creating cord\n");
	int numPoints = (int)luaL_optinteger(L, 1, 2);
	float timestep = (float)luaL_optnumber(L, 2, 0.01);
	int iterations = (int)luaL_optinteger(L, 3, 50);
	float stretchHeight = (float)luaL_optnumber(L, 4, 1.0);
	float stretchWidth = (float)luaL_optnumber(L, 5, 1.0);
	Rope* toLua = lua::place<Rope>(L, lua::metatables::CordMT, numPoints, timestep, iterations, stretchHeight, stretchWidth);
	luaL_setmetatable(L, lua::metatables::CordMT);
	return 1;
}

LUA_FUNCTION(Lua_CordInit) {
	Rope* rope = lua::GetUserdata<Rope*>(L, 1, lua::metatables::CordMT);
	rope->Init();
	return 0;
}

LUA_FUNCTION(Lua_CordUpdate) {
	Rope* rope = lua::GetUserdata<Rope*>(L, 1, lua::metatables::CordMT);
	rope->Update();
	return 0;
}

LUA_FUNCTION(Lua_CordRender) {
	Rope* rope = lua::GetUserdata<Rope*>(L, 1, lua::metatables::CordMT);
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 2, lua::Metatables::SPRITE, "Sprite");
	int layerID = (int)luaL_checkinteger(L, 3);
	bool useOverlay = lua::luaL_checkboolean(L, 4);
	bool unk = lua::luaL_optboolean(L, 5, false);
	rope->Render(anm2, layerID, useOverlay, unk);
	return 0;
}

LUA_FUNCTION(Lua_CordGetPoints) {
	printf("lua: cord getting pointdeque\n");
	Rope* rope = lua::GetUserdata<Rope*>(L, 1, lua::metatables::CordMT);
	PointDeque** luaDeque = (PointDeque**)lua_newuserdata(L, sizeof(PointDeque*));
	*luaDeque = rope->GetPointDeque();
	luaL_setmetatable(L, lua::metatables::PointDequeMT);
	return 1;
}

LUA_FUNCTION(Lua_CordGetTimestep) {
	Rope* rope = lua::GetUserdata<Rope*>(L, 1, lua::metatables::CordMT);
	lua_pushnumber(L, rope->_timestep);
	return 1;
}

LUA_FUNCTION(Lua_CordSetTimestep) {
	Rope* rope = lua::GetUserdata<Rope*>(L, 1, lua::metatables::CordMT);
	rope->_timestep = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_CordGetIterations) {
	Rope* rope = lua::GetUserdata<Rope*>(L, 1, lua::metatables::CordMT);
	lua_pushinteger(L, rope->_iterations);
	return 1;
}

LUA_FUNCTION(Lua_CordSetIterations) {
	Rope* rope = lua::GetUserdata<Rope*>(L, 1, lua::metatables::CordMT);
	rope->_iterations = (int)luaL_checkinteger(L, 2);
	return 0;
}



LUA_FUNCTION(Lua_Cord__gc) {
	Rope* rope = lua::GetUserdata<Rope*>(L, 1, lua::metatables::CordMT);
	rope->~Rope();
	return 0;
}

static void RegisterCord(lua_State* L) {
	luaL_Reg functions[] = {
		{ "Init", Lua_CordInit},
		{ "Update", Lua_CordUpdate},
		{ "Render", Lua_CordRender},
		{ "GetTimestep", Lua_CordGetTimestep},
		{ "SetTimestep", Lua_CordSetTimestep},
		{ "GetIterations", Lua_CordGetIterations},
		{ "SetIterations", Lua_CordSetIterations},
		{ "GetPoints", Lua_CordGetPoints},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::CordMT, lua::metatables::CordMT, functions, Lua_Cord__gc);
	lua_register(L, lua::metatables::CordMT, Lua_CreateCord);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterCord(_state);
}