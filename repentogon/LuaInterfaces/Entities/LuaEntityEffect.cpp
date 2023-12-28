#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EffectCreateLight)
{
	Entity_Effect* effect = lua::GetUserdata<Entity_Effect*>(L, 1, lua::Metatables::ENTITY_EFFECT, "EntityEffect");
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	int lifespan = (int)luaL_checkinteger(L, 3);
	int state = (int)luaL_optinteger(L, 4, 0);
	ColorMod color;
	if (lua_type(L, 5) == LUA_TUSERDATA) {
		color = *lua::GetUserdata<ColorMod*>(L, 5, lua::Metatables::COLOR, "Color");
	}

	effect->CreateLight(pos, &color, lifespan, state);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	/*
	luaL_Reg functions[] = {
		{ "CreateLight", Lua_EffectCreateLight },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_EFFECT, functions);
	*/

	lua::RegisterGlobalClassFunction(_state, "EntityEffect", "CreateLight", Lua_EffectCreateLight);
}