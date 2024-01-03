#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EntityColorConstructor) {
	EntityColor color;
	color._color = *lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::COLOR, "Color");
	color._priority = (int)luaL_checkinteger(L, 2);
	color._duration1 = (int)luaL_checkinteger(L, 3);
	color._duration2 = (int)luaL_checkinteger(L, 3);
	color._fadeout = lua::luaL_checkboolean(L, 4);
	color._shared = lua::luaL_checkboolean(L, 5);
	EntityColor* toLua = (EntityColor*)lua_newuserdata(L, sizeof(EntityColor));
	luaL_setmetatable(L, lua::metatables::EntityColorMT);
	*toLua = color;
	return 1;
}

LUA_FUNCTION(Lua_EntityColorGetColorMod)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = color->_color;

	return 1;
}

LUA_FUNCTION(Lua_EntityColorSetColorMod)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	color->_color = *lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");

	return 0;
}

LUA_FUNCTION(Lua_EntityColorGetPriority)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	lua_pushinteger(L, color->_priority);

	return 1;
}

LUA_FUNCTION(Lua_EntityColorSetPriority)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	color->_priority = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_EntityColorGetDuration)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	lua_pushinteger(L, color->_duration1);

	return 1;
}

LUA_FUNCTION(Lua_EntityColorSetDuration)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	color->_duration1 = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_EntityColorGetLifespan)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	lua_pushinteger(L, color->_duration2);

	return 1;
}

LUA_FUNCTION(Lua_EntityColorSetLifespan)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	color->_duration2 = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_EntityColorGetFadeout)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	lua_pushboolean(L, color->_fadeout);

	return 1;
}

LUA_FUNCTION(Lua_EntityColorSetFadeout)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	color->_fadeout = lua::luaL_checkboolean(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_EntityColorGetShared)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	lua_pushboolean(L, color->_shared);

	return 1;
}

LUA_FUNCTION(Lua_EntityColorSetShared)
{
	EntityColor* color = lua::GetUserdata<EntityColor*>(L, 1, lua::metatables::EntityColorMT);
	color->_shared = lua::luaL_checkboolean(L, 2);

	return 0;
}

static void RegisterEntityColor(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetColor", Lua_EntityColorGetColorMod },
		{ "SetColor", Lua_EntityColorSetColorMod },
		{ "GetDuration", Lua_EntityColorGetDuration },
		{ "SetDuration", Lua_EntityColorSetDuration },
		{ "GetLifespan", Lua_EntityColorGetLifespan },
		{ "SetLifespan", Lua_EntityColorSetLifespan },
		{ "GetFadeout", Lua_EntityColorGetFadeout },
		{ "SetFadeout", Lua_EntityColorSetFadeout },
		{ "GetShared", Lua_EntityColorGetShared },
		{ "SetShared", Lua_EntityColorSetShared },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::EntityColorMT, lua::metatables::EntityColorMT, functions);
	lua_register(L, lua::metatables::EntityColorMT, Lua_EntityColorConstructor);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterEntityColor(state);
}