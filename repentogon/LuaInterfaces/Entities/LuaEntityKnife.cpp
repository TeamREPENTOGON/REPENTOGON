#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_KnifeGetHitList) {
	Entity_Knife * knife = lua::GetUserdata<Entity_Knife*>(L, 1, lua::Metatables::ENTITY_KNIFE, "EntityKnife");
	std::vector<unsigned int> hitList = *knife->GetHitEntities();

	lua_newtable(L);
	int idx = 1;
	for (int index : hitList) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, index);
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetHitList", Lua_KnifeGetHitList },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_KNIFE, functions);
}