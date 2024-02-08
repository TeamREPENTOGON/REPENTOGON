#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

static bool ValidateMusicID(Music* music, int id, int& max) {
	void* ecx = *(void**)((char*)music + 0x320);
	void* edi = *(void**)((char*)music + 0x31C);

	ptrdiff_t length = (ptrdiff_t)ecx - (ptrdiff_t)edi;
	max = length / 96; // Size of the content of the array

	return id >= 0 && id < max;
}

LUA_FUNCTION(Lua_MusicManager_Play) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	int musicId = (int)luaL_checkinteger(L, 2);
	int max;

	if (!ValidateMusicID(music, musicId, max)) {
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

	if (!ValidateMusicID(music, musicId, max)) {
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

	if (!ValidateMusicID(music, musicId, max)) {
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
	music->PlayJingle(musicId, 140, false);

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
