#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

LUA_FUNCTION(Lua_ItemConfigPill_EffectClass_propget) {
	ItemConfig_Pill* config = lua::GetUserdata<ItemConfig_Pill*>(L, 1, lua::Metatables::CONST_PILL_EFFECT, "PillEffect");
	lua_pushinteger(L, config->effectClass);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigPill_EffectSubClass_propget) {
	ItemConfig_Pill* config = lua::GetUserdata<ItemConfig_Pill*>(L, 1, lua::Metatables::CONST_PILL_EFFECT, "PillEffect");
	lua_pushinteger(L, config->effectSubClass);
	return 1;
}

static void FixItemConfigPillEffects(lua_State* L) {
	lua::RegisterVariableGetter(L, lua::Metatables::PILL_EFFECT, "EffectClass", Lua_ItemConfigPill_EffectClass_propget);
	lua::RegisterVariableGetter(L, lua::Metatables::PILL_EFFECT, "EffectSubClass", Lua_ItemConfigPill_EffectSubClass_propget);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_PILL_EFFECT, "EffectClass", Lua_ItemConfigPill_EffectClass_propget);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_PILL_EFFECT, "EffectSubClass", Lua_ItemConfigPill_EffectSubClass_propget);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	FixItemConfigPillEffects(_state);
}