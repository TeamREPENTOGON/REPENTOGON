#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EffectCreateLight)
{
	Entity_Effect* effect = lua::GetUserdata<Entity_Effect*>(L, 1, lua::Metatables::ENTITY_EFFECT, "EntityEffect");
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

	ColorMod color;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		color = *lua::GetUserdata<ColorMod*>(L, 3, lua::Metatables::COLOR, "Color");
	}
	int unk1 = (int)luaL_checkinteger(L, 4);
	int unk2 = (int)luaL_checkinteger(L, 5);

	effect->CreateLight(pos, &color, unk1, unk2);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "CreateLight", Lua_EffectCreateLight },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_EFFECT, functions);
}