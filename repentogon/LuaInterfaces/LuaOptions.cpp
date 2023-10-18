#include "LuaCore.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"

#include "../REPENTOGONOptions.h"

LUA_FUNCTION(Lua_Options_GetVoidGeneration) {
	lua_pushboolean(L, repentogonOptions.betterVoidGeneration);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetVoidGeneration) {
	bool value = lua_toboolean(L, 1);
	repentogonOptions.betterVoidGeneration = value;
	repentogonOptions.Write("VanillaTweaks", "BetterVoidGeneration", value == true ? "1" : "0");
	return 0;
}

LUA_FUNCTION(Lua_Options_GetHushFix) {
	lua_pushboolean(L, repentogonOptions.hushPanicStateFix);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetHushFix) {
	bool value = lua_toboolean(L, 1);
	repentogonOptions.hushPanicStateFix = value;
	repentogonOptions.Write("VanillaTweaks", "HushPanicStateFix", value == true ? "1" : "0");
	return 0;
}

LUA_FUNCTION(Lua_Options_GetKeyMasterDealChance) {
	lua_pushboolean(L, repentogonOptions.keyMasterDealChance);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetKeyMasterDealChance) {
	bool value = lua_toboolean(L, 1);
	repentogonOptions.keyMasterDealChance = value;
	repentogonOptions.Write("VanillaTweaks", "KeyMasterDealChance", value == true ? "1" : "0");
	return 0;
}

LUA_FUNCTION(Lua_Options_GetPreventModUpdates) {
	lua_pushboolean(L, repentogonOptions.preventModUpdates);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetPreventModUpdates) {
	bool value = lua_toboolean(L, 1);
	repentogonOptions.preventModUpdates = value;
	repentogonOptions.Write("VanillaTweaks", "PreventModUpdates", value == true ? "1" : "0");
	return 0;
}

LUA_FUNCTION(Lua_Options_Fix_SetSFXVolume) {
	float value = lua_tonumber(L, 1);
	value = (min(max(0.0f, value), 1.0f)) * 10; // clamp, then multiply in preparation for rounding
	value = std::round(value) / 10; // round, then divide back down to first decimal
	g_Manager->GetOptions()->_sfxVolume = value;
	g_Manager->_sfxManager.ClearVolumeModifier();
	return 0;
}

static void RegisterLuaOptions(lua_State* L) {
	lua::LuaStackProtector protector(L);

	lua_getglobal(L, "Options");
	lua_pushstring(L, "__propget");
	lua_rawget(L, -2);

	lua_pushstring(L, "BetterVoidGeneration");
	lua_pushcfunction(L, Lua_Options_GetVoidGeneration);
	lua_rawset(L, -3);

	lua_pushstring(L, "HushPanicStateFix");
	lua_pushcfunction(L, Lua_Options_GetHushFix);
	lua_rawset(L, -3);

	lua_pushstring(L, "KeyMasterDealChance");
	lua_pushcfunction(L, Lua_Options_GetKeyMasterDealChance);
	lua_rawset(L, -3);

	lua_pushstring(L, "PreventModUpdates");
	lua_pushcfunction(L, Lua_Options_GetKeyMasterDealChance);
	lua_rawset(L, -3);

	lua_pop(L, 1);

	lua_pushstring(L, "__propset");
	lua_rawget(L, -2);

	lua_pushstring(L, "BetterVoidGeneration");
	lua_pushcfunction(L, Lua_Options_SetVoidGeneration);
	lua_rawset(L, -3);

	lua_pushstring(L, "HushPanicStateFix");
	lua_pushcfunction(L, Lua_Options_SetHushFix);
	lua_rawset(L, -3);

	lua_pushstring(L, "KeyMasterDealChance");
	lua_pushcfunction(L, Lua_Options_SetKeyMasterDealChance);
	lua_rawset(L, -3);

	lua_pushstring(L, "PreventModUpdates");
	lua_pushcfunction(L, Lua_Options_GetKeyMasterDealChance);
	lua_rawset(L, -3);

	lua_pushstring(L, "SFXVolume");
	lua_pushcfunction(L, Lua_Options_Fix_SetSFXVolume);
	lua_rawset(L, -3);

	lua_pop(L, 2);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	RegisterLuaOptions(_state);
}