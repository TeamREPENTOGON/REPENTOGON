#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EntityLaserGetHomingLaser)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY_LASER, "EntityLaser");
	HomingLaser* toLua = &laser->_homingLaser;
	if (toLua == nullptr) {
		lua_pushnil(L);
		return 1;
	}
	HomingLaser** ud = (HomingLaser**)lua_newuserdata(L, sizeof(HomingLaser*));
	*ud = toLua;
	luaL_setmetatable(L, lua::metatables::HomingLaserMT);
	return 1;
}

LUA_FUNCTION(Lua_HomingLaserApplyHook)
{
	HomingLaser* homing = *lua::GetUserdata<HomingLaser**>(L, 1, lua::metatables::HomingLaserMT);
	float scale = luaL_checknumber(L, 2);
	float sinOffset = luaL_checknumber(L, 3);

	homing->ApplyHook(scale, sinOffset);

	return 0;
}

static void RegisterHomingLaser(lua_State* L)
{
	lua::RegisterFunction(L, lua::Metatables::ENTITY_LASER, "GetHomingLaser", Lua_EntityLaserGetHomingLaser);

	lua::RegisterVariableGetter(L, lua::Metatables::ENTITY_LASER, "HomingLaser", Lua_EntityLaserGetHomingLaser);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_ENTITY_LASER, "HomingLaser", Lua_EntityLaserGetHomingLaser);

	luaL_Reg functions[] = {
		{ "ApplyHook", Lua_HomingLaserApplyHook},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::HomingLaserMT, lua::metatables::HomingLaserMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterHomingLaser(_state);
}