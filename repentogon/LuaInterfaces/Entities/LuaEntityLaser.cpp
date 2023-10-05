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

static int Lua_EntityLaserGetHomingType(lua_State* L)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	lua_pushinteger(L, *laser->GetHomingType());

	return 1;
}

static int Lua_EntityLaserSetHomingType(lua_State* L)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	*laser->GetHomingType() = (uint32_t)luaL_checkinteger(L, 2);

	return 0;
}

static void RegisterHomingTypeFix(lua_State* L) {
	lua::LuaStackProtector protector(L);

	lua::PushMetatable(L, lua::Metatables::ENTITY_LASER);
	lua_pushstring(L, "__propget");
	lua_rawget(L, -2);

	lua_pushstring(L, "HomingType");
	lua_pushcfunction(L, Lua_EntityLaserGetHomingType);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	lua_pushstring(L, "__propset");
	lua_rawget(L, -2);

	lua_pushstring(L, "HomingType");
	lua_pushcfunction(L, Lua_EntityLaserSetHomingType);
	lua_rawset(L, -3);
	lua_pop(L, 2);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ENTITY_LASER;
	lua::RegisterFunction(state, mt, "GetScale", Lua_EntityLaserGetScale);
	lua::RegisterFunction(state, mt, "SetScale", Lua_EntityLaserSetScale);
	lua::RegisterFunction(state, mt, "ResetSpriteScale", Lua_EntityLaserResetSpriteScale);
	RegisterHomingTypeFix(state);
}