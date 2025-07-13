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

LUA_FUNCTION(Lua_Options_GetOnlineHUD) {
	lua_pushinteger(L, g_Manager->GetOptions()->_onlineHud);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetOnlineHUD) {
	int value = luaL_checkinteger(L, 1);
	value = std::min(std::max(0, value), 3);
	g_Manager->GetOptions()->_onlineHud = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetStreamerMode) {
	lua_pushboolean(L, g_Manager->GetOptions()->_streamerMode);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetStreamerMode) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_streamerMode = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetOnlineChatEnabled) {
	lua_pushboolean(L, g_Manager->GetOptions()->_onlineChatEnabled);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetOnlineChatEnabled) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_onlineChatEnabled = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetOnlinePlayerVolume) {
	lua_pushinteger(L, g_Manager->GetOptions()->_onlinePlayerVolume);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetOnlinePlayerVolume) {
	int value = luaL_checkinteger(L, 1);
	value = std::min(std::max(0, value), 10);
	g_Manager->GetOptions()->_onlinePlayerVolume = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetOnlinePlayerOpacity) {
	lua_pushinteger(L, g_Manager->GetOptions()->_onlinePlayerOpacity);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetOnlinePlayerOpacity) {
	int value = luaL_checkinteger(L, 1);
	value = std::min(std::max(0, value), 10);
	g_Manager->GetOptions()->_onlinePlayerOpacity = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetOnlineColorSet) {
	lua_pushinteger(L, g_Manager->GetOptions()->_onlineColorSet);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetOnlineColorSet) {
	int value = luaL_checkinteger(L, 1);
	value = std::min(std::max(0, value), 3);
	g_Manager->GetOptions()->_onlineColorSet = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetOnlineInputDelay) {
	lua_pushinteger(L, g_Manager->GetOptions()->_onlineInputDelay);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetOnlineInputDelay) {
	int value = luaL_checkinteger(L, 1);
	value = std::min(std::max(1, value), 5);
	g_Manager->GetOptions()->_onlineInputDelay = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetExposure) {
	lua_pushnumber(L, (float)g_Manager->GetOptions()->_exposure);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetExposure) {
	float value = (float)luaL_checknumber(L, 1);
	g_Manager->GetOptions()->_exposure = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetBrightness) {
	lua_pushnumber(L, (float)g_Manager->GetOptions()->_brightness);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetBrightness) {
	float value = (float)luaL_checknumber(L, 1);
	g_Manager->GetOptions()->_brightness = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetContrast) {
	lua_pushnumber(L, (float)g_Manager->GetOptions()->_contrast);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetContrast) {
	float value = (float)luaL_checknumber(L, 1);
	g_Manager->GetOptions()->_contrast = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetControllerHotplugEnabled) {
	lua_pushboolean(L, g_Manager->GetOptions()->_controlHotplugEnabled);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetControllerHotplugEnabled) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_controlHotplugEnabled = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetTryImportSave) {
	lua_pushboolean(L, g_Manager->GetOptions()->_tryImportSave);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetTryImportSave) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_tryImportSave = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetBossHPOnBottom) {
	lua_pushboolean(L, g_Manager->GetOptions()->_bossHpOnBottom);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetBossHPOnBottom) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_bossHpOnBottom = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetModsEnabled) {
	lua_pushboolean(L, g_Manager->GetOptions()->_enableMods);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetModsEnabled) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_enableMods = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetTouchMode) {
	lua_pushinteger(L, g_Manager->GetOptions()->_touchMode);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetTouchMode) {
	int value = luaL_checkinteger(L, 1);
	g_Manager->GetOptions()->_touchMode = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetAimLockEnabled) {
	lua_pushboolean(L, g_Manager->GetOptions()->_enableAimLock);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetAimLockEnabled) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_enableAimLock = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetAscentVoiceOver) {
	lua_pushboolean(L, g_Manager->GetOptions()->_ascentVoiceover);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetAscentVoiceOver) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_ascentVoiceover = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetOnlineChatFilterEnabled) {
	lua_pushboolean(L, g_Manager->GetOptions()->_onlineFilterEnabled);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetOnlineChatFilterEnabled) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_onlineFilterEnabled = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetAcceptedModDisclaimer) {
	lua_pushboolean(L, g_Manager->GetOptions()->_acceptedModDisclaimer);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetAcceptedModDisclaimer) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_acceptedModDisclaimer = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetAcceptedDataCollectionDisclaimer) {
	lua_pushboolean(L, g_Manager->GetOptions()->_acceptedDataCollectionDisclaimer);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetAcceptedDataCollectionDisclaimer) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_acceptedDataCollectionDisclaimer = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetAcceptedPublicBeta) {
	lua_pushboolean(L, g_Manager->GetOptions()->_acceptedPublicBeta_VERSION);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetAcceptedPublicBeta) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_acceptedPublicBeta_VERSION = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetSteamCloudEnabled) {
	lua_pushboolean(L, g_Manager->GetOptions()->_enableSteamCloud);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetSteamCloudEnabled) {
	bool value = lua::luaL_checkboolean(L, 1);
	g_Manager->GetOptions()->_enableSteamCloud = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetWindowWidth) {
	lua_pushinteger(L, g_Manager->GetOptions()->_windowWidth);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetWindowWidth) {
	int value = luaL_checkinteger(L, 1);
	g_Manager->GetOptions()->_windowWidth = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetWindowHeight) {
	lua_pushinteger(L, g_Manager->GetOptions()->_windowHeight);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetWindowHeight) {
	int value = luaL_checkinteger(L, 1);
	g_Manager->GetOptions()->_windowHeight = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetWindowPosX) {
	lua_pushinteger(L, g_Manager->GetOptions()->_windowPosX);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetWindowPosX) {
	int value = luaL_checkinteger(L, 1);
	value = std::min(std::max(8, value), 65535);
	g_Manager->GetOptions()->_windowPosX = value;
	return 0;
}

LUA_FUNCTION(Lua_Options_GetWindowPosY) {
	lua_pushinteger(L, g_Manager->GetOptions()->_windowPosY);
	return 1;
}

LUA_FUNCTION(Lua_Options_SetWindowPosY) {
	int value = luaL_checkinteger(L, 1);
	value = std::min(std::max(8, value), 65535);
	g_Manager->GetOptions()->_windowPosY = value;
	return 0;
}

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
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "OnlineHUD", Lua_Options_GetOnlineHUD, NULL);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "StreamerMode", Lua_Options_GetStreamerMode, NULL);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "OnlineChatEnabled", Lua_Options_GetOnlineChatEnabled, NULL);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "OnlinePlayerVolume", Lua_Options_GetOnlinePlayerVolume, NULL);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "OnlinePlayerOpacity", Lua_Options_GetOnlinePlayerOpacity, NULL);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "OnlineColorSet", Lua_Options_GetOnlineColorSet, NULL);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "OnlineInputDelay", Lua_Options_GetOnlineInputDelay, NULL);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "Exposure", Lua_Options_GetExposure, Lua_Options_SetExposure);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "Brightness", Lua_Options_GetBrightness, Lua_Options_SetBrightness);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "Contrast", Lua_Options_GetContrast, Lua_Options_SetContrast);
	// lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "ControllerHotplugEnabled", Lua_Options_GetControllerHotplugEnabled, Lua_Options_SetControllerHotplugEnabled);
	// lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "TryImportSave", Lua_Options_GetTryImportSave, Lua_Options_SetTryImportSave);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "BossHPOnBottom", Lua_Options_GetBossHPOnBottom, Lua_Options_SetBossHPOnBottom);
	// lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "ModsEnabled", Lua_Options_GetModsEnabled, Lua_Options_SetModsEnabled);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "TouchMode", Lua_Options_GetTouchMode, Lua_Options_SetTouchMode);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "AimLockEnabled", Lua_Options_GetAimLockEnabled, Lua_Options_SetAimLockEnabled);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "AscentVoiceOver", Lua_Options_GetAscentVoiceOver, Lua_Options_SetAscentVoiceOver);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "OnlineChatFilterEnabled", Lua_Options_GetOnlineChatFilterEnabled, NULL);
	// lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "AcceptedModDisclaimer", Lua_Options_GetAcceptedModDisclaimer, Lua_Options_SetAcceptedModDisclaimer);
	// lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "AcceptedDataCollectionDisclaimer", Lua_Options_GetAcceptedDataCollectionDisclaimer, Lua_Options_SetAcceptedDataCollectionDisclaimer);
	// lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "AcceptedPublicBeta", Lua_Options_GetAcceptedPublicBeta, Lua_Options_SetAcceptedPublicBeta);
	// lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "SteamCloudEnabled", Lua_Options_GetSteamCloudEnabled, Lua_Options_SetSteamCloudEnabled);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "WindowWidth", Lua_Options_GetWindowWidth, Lua_Options_SetWindowWidth);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "WindowHeight", Lua_Options_GetWindowHeight, Lua_Options_SetWindowHeight);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "WindowPosX", Lua_Options_GetWindowPosX, Lua_Options_SetWindowPosX);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "WindowPosY", Lua_Options_GetWindowPosY, Lua_Options_SetWindowPosY);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "BetterVoidGeneration", Lua_Options_GetVoidGeneration, Lua_Options_SetVoidGeneration);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "HushPanicStateFix", Lua_Options_GetHushFix, Lua_Options_SetHushFix);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "StatHUDPlanetarium", Lua_Options_GetStatHUDPlanetarium, Lua_Options_SetStatHUDPlanetarium);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "QuickRoomClear", Lua_Options_GetQuickRoomClear, Lua_Options_SetQuickRoomClear);
	lua::RegisterGlobalClassVariable(L, lua::GlobalClasses::Options, "PreventModUpdates", Lua_Options_GetPreventModUpdates, NULL);
	lua::RegisterGlobalClassVariableSetter(L, lua::GlobalClasses::Options, "SFXVolume", Lua_Options_Fix_SetSFXVolume);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterLuaOptions(_state);
}