#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PIGetItem) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	lua::luabridge::UserdataPtr::push(L, pi->item, lua::Metatables::CONST_ITEM);
	return 1;
}

LUA_FUNCTION(Lua_PIGetTargetItem) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	if (pi->targetItem) {
		lua::luabridge::UserdataPtr::push(L, pi->targetItem, lua::Metatables::CONST_ITEM);
	}
	else {
		lua_pushnil(L);
	}
	return 1;
}

LUA_FUNCTION(Lua_PIGetID) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	lua_pushinteger(L, pi->_id);
	return 1;
}

LUA_FUNCTION(Lua_PIGetDamage) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	lua_pushnumber(L, pi->damage);
	return 1;
}
LUA_FUNCTION(Lua_PIGetFireDelay) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	lua_pushnumber(L, pi->firedelay);
	return 1;
}
LUA_FUNCTION(Lua_PIGetSpeed) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	lua_pushnumber(L, pi->speed);
	return 1;
}
LUA_FUNCTION(Lua_PIGetRange) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	lua_pushnumber(L, pi->range);
	return 1;
}
LUA_FUNCTION(Lua_PIGetShotSpeed) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	lua_pushnumber(L, pi->shotspeed);
	return 1;
}
LUA_FUNCTION(Lua_PIGetLuck) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	lua_pushnumber(L, pi->luck);
	return 1;
}

LUA_FUNCTION(Lua_PIGetEffectCount) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	std::vector<ProceduralEffect*>* effect = pi->GetEffects();
	lua_pushinteger(L, effect->size());
	return 1;
}

LUA_FUNCTION(Lua_PIGetEffect) {
	ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, lua::metatables::ProceduralItemMT);
	std::vector<ProceduralEffect*>* effect = pi->GetEffects();
	int index = (int)luaL_checkinteger(L, 2);
	if (index >= 0 && (unsigned int)index < effect->size()) {
		ProceduralEffect** pe = (ProceduralEffect**)lua_newuserdata(L, sizeof(void*));
		*pe = (*effect)[index];
		luaL_setmetatable(L, "ProceduralEffect");
	}
	else {
		lua_pushnil(L);
	}
	return 1;
}

static void RegisterProceduralItem(lua_State* L) {
	luaL_Reg functions[] = {
				{ "GetItem", Lua_PIGetItem},
				{ "GetTargetItem", Lua_PIGetTargetItem},
				{ "GetEffectCount", Lua_PIGetEffectCount},
				{ "GetEffect", Lua_PIGetEffect},
				{ "GetID", Lua_PIGetID},
				{ "GetDamage", Lua_PIGetDamage},
				{ "GetFireDelay", Lua_PIGetFireDelay},
				{ "GetSpeed", Lua_PIGetSpeed},
				{ "GetRange", Lua_PIGetRange},
				{ "GetShotSpeed", Lua_PIGetShotSpeed},
				{ "GetLuck", Lua_PIGetLuck},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::ProceduralItemMT, lua::metatables::ProceduralItemMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterProceduralItem(_state);
}