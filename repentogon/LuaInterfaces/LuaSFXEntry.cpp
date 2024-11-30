#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_SFXManager_GetEntry)
{
	SoundEffects* sound = lua::GetUserdata<SoundEffects*>(L, 1, lua::Metatables::SFX_MANAGER, "SFXManager");
	int soundId = (int)luaL_checkinteger(L, 2);
	int max;

	if (!sound->ValidateSoundID(soundId, max)) {
		return luaL_error(L, "Invalid sound ID %d. Min = 0, Max = %d", soundId, max - 1);
	}

	SoundEffect* toLua = &sound->_sounds[soundId];
	SoundEffect** luaSoundEntry = (SoundEffect**)lua_newuserdata(L, sizeof(SoundEffect*));
	*luaSoundEntry = toLua;
	luaL_setmetatable(L, lua::metatables::SFXEntryMT);
	return 1;
}

LUA_FUNCTION(Lua_SFXEntryGetLastFramePlayed)
{
	SoundEffect* entry = *lua::GetUserdata<SoundEffect**>(L, 1, lua::metatables::SFXEntryMT);
	lua_pushinteger(L, entry->_framePlayed);
	return 1;
}

LUA_FUNCTION(Lua_SFXEntryGetNextPlayableFrame)
{
	SoundEffect* entry = *lua::GetUserdata<SoundEffect**>(L, 1, lua::metatables::SFXEntryMT);
	lua_pushinteger(L, entry->_nextPlayableFrame);
	return 1;
}

LUA_FUNCTION(Lua_SFXEntryGetVolume)
{
	SoundEffect* entry = *lua::GetUserdata<SoundEffect**>(L, 1, lua::metatables::SFXEntryMT);
	lua_pushnumber(L, entry->_volume);
	return 1;
}

LUA_FUNCTION(Lua_SFXEntryGetLoaded)
{
	SoundEffect* entry = *lua::GetUserdata<SoundEffect**>(L, 1, lua::metatables::SFXEntryMT);
	lua_pushboolean(L, entry->_loaded);
	return 1;
}

LUA_FUNCTION(Lua_SFXEntryGetPreload)
{
	SoundEffect* entry = *lua::GetUserdata<SoundEffect**>(L, 1, lua::metatables::SFXEntryMT);
	lua_pushboolean(L, entry->_shouldPreload);
	return 1;
}

LUA_FUNCTION(Lua_SFXEntryGetName)
{
	SoundEffect* entry = *lua::GetUserdata<SoundEffect**>(L, 1, lua::metatables::SFXEntryMT);
	lua_pushstring(L, entry->_name.empty() ? "" : entry->_name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_SFXEntryGetSampleCount)
{
	SoundEffect* entry = *lua::GetUserdata<SoundEffect**>(L, 1, lua::metatables::SFXEntryMT);
	lua_pushinteger(L, entry->_sampleCount);
	return 1;
}

LUA_FUNCTION(Lua_SFXEntryGetSample)
{
	SoundEffect* entry = *lua::GetUserdata<SoundEffect**>(L, 1, lua::metatables::SFXEntryMT);
	int id = (int)luaL_checkinteger(L, 2);

	if (id < 0 || id > entry->_sampleCount-1) {
		return luaL_error(L, "Invalid sample ID %d. Min = 0, Max = %d", id, entry->_sampleCount - 1);
	}

	SoundEffectSample* toLua = &entry->_samples[id];
	SoundEffectSample** luaSoundSample = (SoundEffectSample**)lua_newuserdata(L, sizeof(SoundEffectSample*));
	*luaSoundSample = toLua;
	luaL_setmetatable(L, lua::metatables::SFXEntrySampleMT);
	return 1;
}

LUA_FUNCTION(Lua_SFXEntrySampleGetFilename)
{
	SoundEffectSample* sample = *lua::GetUserdata<SoundEffectSample**>(L, 1, lua::metatables::SFXEntrySampleMT);
	lua_pushstring(L, sample->_filename.empty() ? "" : sample->_filename.c_str());
	return 1;
}

LUA_FUNCTION(Lua_SFXEntrySampleGetActor)
{
	SoundEffectSample* sample = *lua::GetUserdata<SoundEffectSample**>(L, 1, lua::metatables::SFXEntrySampleMT);
	SoundActor* toLua = &sample->_actor;
	SoundActor** luaSoundActor = (SoundActor**)lua_newuserdata(L, sizeof(SoundActor*));
	*luaSoundActor = toLua;
	luaL_setmetatable(L, lua::metatables::SoundActorMT);
	return 1;
}

LUA_FUNCTION(Lua_SFXEntrySampleGetWeight)
{
	SoundEffectSample* sample = *lua::GetUserdata<SoundEffectSample**>(L, 1, lua::metatables::SFXEntrySampleMT);
	lua_pushinteger(L, sample->_weight);
	return 1;
}

static void RegisterSFXEntry(lua_State* L)
{
	lua::RegisterFunction(L, lua::Metatables::SFX_MANAGER, "GetEntry", Lua_SFXManager_GetEntry);

	luaL_Reg entryFunctions[] = {
		{ "Get", Lua_SFXEntryGetName},
		{ "GetLastFramePlayed", Lua_SFXEntryGetLastFramePlayed},
		{ "GetNextPlayableFrame", Lua_SFXEntryGetNextPlayableFrame},
		{ "GetVolume", Lua_SFXEntryGetVolume},
		{ "GetLoaded", Lua_SFXEntryGetLoaded},
		{ "GetPreload", Lua_SFXEntryGetPreload},
		{ "GetSampleCount", Lua_SFXEntryGetSampleCount},
		{ "GetSample", Lua_SFXEntryGetSample},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::SFXEntryMT, lua::metatables::SFXEntryMT, entryFunctions);

	luaL_Reg sampleFunctions[] = {
	{ "GetFilename", Lua_SFXEntrySampleGetFilename},
	{ "GetActor", Lua_SFXEntrySampleGetActor},
	{ "GetWeight", Lua_SFXEntrySampleGetWeight},
	{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::SFXEntrySampleMT, lua::metatables::SFXEntrySampleMT, sampleFunctions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterSFXEntry(_state);
}