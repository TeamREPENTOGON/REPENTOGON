#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_EntityLaserGetScale(lua_State* L)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	lua_pushnumber(L, *laser->GetScale());

	return 1;
}

static int Lua_EntityLaserSetScale(lua_State* L)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	*laser->GetScale() = (float)luaL_checknumber(L, 2);
	laser->ResetSpriteScale();

	return 0;
}

static int Lua_EntityLaserResetSpriteScale(lua_State* L)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	laser->ResetSpriteScale();

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ENTITY_LASER;
	lua::RegisterFunction(state, mt, "GetScale", Lua_EntityLaserGetScale);
	lua::RegisterFunction(state, mt, "SetScale", Lua_EntityLaserSetScale);
	lua::RegisterFunction(state, mt, "ResetSpriteScale", Lua_EntityLaserResetSpriteScale);
}