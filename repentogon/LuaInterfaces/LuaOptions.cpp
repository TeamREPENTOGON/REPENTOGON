#include <algorithm>

#include "LuaCore.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"

#include "../REPENTOGONOptions.h"

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif 

LUA_FUNCTION(Lua_Options_GetVoidGeneration) {
	lua_pushboolean(L, repentogonOptions.betterVoidGeneration);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetVoidGeneration) {
	bool value = lua::luaL_checkboolean(L, 1);
	repentogonOptions.betterVoidGeneration = value;
	//repentogonOptions.Write("VanillaTweaks", "BetterVoidGeneration", value);
	return 0;
}

LUA_FUNCTION(Lua_Options_GetHushFix) {
	lua_pushboolean(L, repentogonOptions.hushLaserSpeedFix);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetHushFix) {
	bool value = lua::luaL_checkboolean(L, 1);
	repentogonOptions.hushLaserSpeedFix = value;
	//repentogonOptions.Write("VanillaTweaks", "HushPanicStateFix", value);
	return 0;
}

LUA_FUNCTION(Lua_Options_GetKeyMasterDealChance) {
	lua_pushboolean(L, repentogonOptions.keyMasterDealChance);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetKeyMasterDealChance) {
	bool value = lua::luaL_checkboolean(L, 1);
	repentogonOptions.keyMasterDealChance = value;
	//repentogonOptions.Write("VanillaTweaks", "KeyMasterDealChance", value);
	return 0;
}

LUA_FUNCTION(Lua_Options_GetStatHUDPlanetarium) {
	lua_pushboolean(L, repentogonOptions.statHUDPlanetarium);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetStatHUDPlanetarium) {
	bool value = lua::luaL_checkboolean(L, 1);
	repentogonOptions.statHUDPlanetarium = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetQuickRoomClear) {
	lua_pushboolean(L, repentogonOptions.quickRoomClear);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetQuickRoomClear) {
	bool value = lua::luaL_checkboolean(L, 1);
	repentogonOptions.quickRoomClear = value;
	//repentogonOptions.Write("VanillaTweaks", "QuickRoomClear", value);
	return 0;
}

LUA_FUNCTION(Lua_Options_GetPreventModUpdates) {
	lua_pushboolean(L, repentogonOptions.preventModUpdates);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetPreventModUpdates) {
	bool value = lua::luaL_checkboolean(L, 1);
	repentogonOptions.preventModUpdates = value;
	//repentogonOptions.Write("VanillaTweaks", "PreventModUpdates", value);
	return 0;
}

LUA_FUNCTION(Lua_Options_Fix_SetSFXVolume) {
	float value = (float)luaL_checknumber(L, 1);
	value = (std::min(std::max(0.0f, value), 1.0f)) * 10; // clamp, then multiply in preparation for rounding
	value = std::round(value) / 10; // round, then divide back down to first decimal
	g_Manager->GetOptions()->_sfxVolume = value;
	g_Manager->_sfxManager.ClearVolumeModifier();
	return 0;
}

static void RegisterLuaOptions(lua_State* L) {
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "BetterVoidGeneration", Lua_Options_GetVoidGeneration, Lua_Options_SetVoidGeneration);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "HushPanicStateFix", Lua_Options_GetHushFix, Lua_Options_SetHushFix);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "KeyMasterDealChance", Lua_Options_GetKeyMasterDealChance, Lua_Options_SetKeyMasterDealChance);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "StatHUDPlanetarium", Lua_Options_GetStatHUDPlanetarium, Lua_Options_SetStatHUDPlanetarium);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "QuickRoomClear", Lua_Options_GetQuickRoomClear, Lua_Options_SetQuickRoomClear);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "PreventModUpdates", Lua_Options_GetPreventModUpdates, Lua_Options_SetPreventModUpdates);
	lua::RegisterGlobalClassVariableSetter(L, lua::GlobalClasses::Options, "SFXVolume", Lua_Options_Fix_SetSFXVolume);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterLuaOptions(_state);
}