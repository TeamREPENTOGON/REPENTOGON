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

LUA_FUNCTION(Lua_ItemConfig_CanRerollCollectible) {
	int id = (int)luaL_checkinteger(L, 1);
	lua_pushboolean(Isaac::CanRerollCollectible(id));
	return 1;
}

LUA_FUNCTION(Lua_ItemConfig_GetTaggedItems) {
	ItemConfig* config = lua::GetUserdata<ItemConfig*>(L, 1, lua::Metatables::CONFIG, "ItemConfig");
	unsigned int tags = (unsigned int)luaL_checkinteger(L, 2);

	std::vector<ItemConfig_Item>& items = config->GetTaggedItems(tags);

	lua_newtable(L);
	for (size_t i = 0; i < items.size(); ++i) {
		lua_pushinteger(L, i + 1);
		lua::luabridge::UserdataPtr::push(L, &items[i], lua::GetMetatableKey(lua::Metatables::ITEM));
		lua_rawset(L, -3);
	}

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

	luaL_Reg functions[] = {
		{ "GetTaggedItems", Lua_ItemConfig_GetTaggedItems },
		{ "CanRerollCollectible", Lua_ItemConfig_CanRerollCollectible },
		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::CONFIG, functions);

	FixItemConfigPillEffects(_state);
}