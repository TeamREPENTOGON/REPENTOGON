#include <array>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

LUA_FUNCTION(Lua_ItemConfigPill_EffectClass_propget) {
	ItemConfigPill* config = lua::GetUserdata<ItemConfigPill*>(L, 1, lua::Metatables::CONST_PILL_EFFECT, "PillEffect");
	lua_pushinteger(L, config->effectClass);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigPill_EffectSubClass_propget) {
	ItemConfigPill* config = lua::GetUserdata<ItemConfigPill*>(L, 1, lua::Metatables::CONST_PILL_EFFECT, "PillEffect");
	lua_pushinteger(L, config->effectSubClass);
	return 1;
}

static void FixItemConfigPillEffects(lua_State* L) {
	lua::LuaStackProtector protector(L);

	std::array<lua::Metatables, 2> metatables = {
		lua::Metatables::PILL_EFFECT,
		lua::Metatables::CONST_PILL_EFFECT
	};

	for (lua::Metatables metatable : metatables) {
		lua::PushMetatable(L, metatable);
		lua_pushstring(L, "__propget");
		lua_rawget(L, -2);

		lua_pushstring(L, "EffectClass");
		lua_pushcfunction(L, Lua_ItemConfigPill_EffectClass_propget);
		lua_rawset(L, -3);

		lua_pushstring(L, "EffectSubClass");
		lua_pushcfunction(L, Lua_ItemConfigPill_EffectSubClass_propget);
		lua_rawset(L, -3);

		lua_pop(L, 2);
	}
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	FixItemConfigPillEffects(_state);
}