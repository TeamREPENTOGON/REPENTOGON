#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EffectCreateLight)
{
	Vector* pos = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	float scale = (float)luaL_optnumber(L, 2, (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
	int lifespan = (int)luaL_optinteger(L, 3, -1);
	int state = (int)luaL_optinteger(L, 4, 6);
	ColorMod color;
	if (lua_type(L, 5) == LUA_TUSERDATA) {
		color = *lua::GetUserdata<ColorMod*>(L, 5, lua::Metatables::COLOR, "Color");
	}

	if (lifespan < 1) {
		lifespan = -1;
	}
	if (state < 1) {
		state = 6;
	}

	Entity_Effect* effect = (Entity_Effect*)g_Game->Spawn(1000, 121, *pos, Vector(0, 0), nullptr, 0, Isaac::genrand_int32(), 0);
	if (!effect) {
		lua_pushnil(L);
	}
	else {
		effect->_state = state;
		effect->_timeout = lifespan;
		effect->_lifespan = lifespan;
		effect->SetColor(&color, -1, 255, false, false);
		effect->_sprite._scale *= scale;

		lua::luabridge::UserdataPtr::push(L, effect, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
	}

	return 1;
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