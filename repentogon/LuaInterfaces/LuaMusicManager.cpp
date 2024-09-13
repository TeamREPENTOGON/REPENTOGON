#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

LUA_FUNCTION(Lua_MusicManager_Play) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	int musicId = (int)luaL_checkinteger(L, 2);
	int max;

	if (!music->ValidateMusicID(musicId, max)) {
		return luaL_error(L, "Invalid music ID %d. Min = 0, Max = %d", musicId, max - 1);
	}

	float volume = (float)luaL_optnumber(L, 3, -1);
	music->Play(musicId, volume);
	return 0;
}

LUA_FUNCTION(Lua_MusicManager_Crossfade) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	int musicId = (int)luaL_checkinteger(L, 2);
	int max;

	if (!music->ValidateMusicID(musicId, max)) {
		return luaL_error(L, "Invalid music ID %d. Min = 0, Max = %d", musicId, max - 1);
	}

	float faderate = (float)luaL_optnumber(L, 3, 0.08);
	music->Crossfade(musicId, faderate);
	return 0;
}

LUA_FUNCTION(Lua_MusicManager_Fadein) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	unsigned int musicId = (unsigned int)luaL_checkinteger(L, 2);
	int max;

	if (!music->ValidateMusicID(musicId, max)) {
		return luaL_error(L, "Invalid music ID %d. Min = 0, Max = %d", musicId, max - 1);
	}

	float volume = (float)luaL_optnumber(L, 3, 1);
	float faderate = (float)luaL_optnumber(L, 4, 0.08);
	music->Fadein(musicId, volume, faderate);
	return 0;
}

LUA_FUNCTION(Lua_MusicManager_PlayJingle) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	int musicId = (int)luaL_checkinteger(L, 2);
	int duration = (int)luaL_optinteger(L, 3, 140);
	if (duration < 0)
		return luaL_argerror(L, 3, "Duration must be greater than zero!");
	music->PlayJingle(musicId, 140, false);

	//duration was inlined and (at least most) calls to the func had it stripped from the args, just set it ourselves
	music->_jingleCountdownMaybe = duration;

	return 0;
}

LUA_FUNCTION(Lua_MusicManager_StopJingle) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	music->StopJingle();

	return 0;
}

LUA_FUNCTION(Lua_MusicManager_GetCurrentPitch) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	lua_pushnumber(L, music->_pitch);
	return 1;
}

LUA_FUNCTION(Lua_MusicManager_SetCurrentPitch) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	music->_pitch = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MusicManager_DisableLayer) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	int id = (int)luaL_checkinteger(L, 2);
	if (id < 0 || id > 5)
		return luaL_error(L, "bad argument #2 to 'DisableLayer' (Invalid layer id %d)", id);
	music->DisableLayer(id);
	return 0;
}

LUA_FUNCTION(Lua_MusicManager_EnableLayer) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	int id = (int)luaL_checkinteger(L, 2);
	bool instant = lua::luaL_optboolean(L, 3, false);
	if (id < 0 || id > 5)
		return luaL_error(L, "bad argument #2 to 'EnableLayer' (Invalid layer id %d)", id);
	music->EnableLayer(id, instant);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua::LuaStackProtector protector(_state);

	// Fix existing functions
	lua::RegisterFunction(_state, lua::Metatables::MUSIC_MANAGER, "Play", Lua_MusicManager_Play);
	lua::RegisterFunction(_state, lua::Metatables::MUSIC_MANAGER, "Crossfade", Lua_MusicManager_Crossfade);
	lua::RegisterFunction(_state, lua::Metatables::MUSIC_MANAGER, "Fadein", Lua_MusicManager_Fadein);

	// New Functions
	lua::RegisterFunction(_state, lua::Metatables::MUSIC_MANAGER, "PlayJingle", Lua_MusicManager_PlayJingle);
	lua::RegisterFunction(_state, lua::Metatables::MUSIC_MANAGER, "StopJingle", Lua_MusicManager_StopJingle);
	lua::RegisterFunction(_state, lua::Metatables::MUSIC_MANAGER, "GetCurrentPitch", Lua_MusicManager_GetCurrentPitch);
	lua::RegisterFunction(_state, lua::Metatables::MUSIC_MANAGER, "SetCurrentPitch", Lua_MusicManager_SetCurrentPitch);
}
