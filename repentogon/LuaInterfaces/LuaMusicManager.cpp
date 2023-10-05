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

static int lua_MusicManager_Play(lua_State* L) {
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

static int lua_MusicManager_Crossfade(lua_State* L) {
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

static int lua_MusicManager_Fadein(lua_State* L) {
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

static int Lua_MusicManager_PlayJingle(lua_State* L) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	int musicId = (int)luaL_checkinteger(L, 2);
	music->PlayJingle(musicId, 140, false);

	return 0;
}

static int Lua_MusicManager_StopJingle(lua_State* L) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	music->StopJingle();

	return 0;
}

static void FixMusicManagerFunctions(lua_State* L) {
	lua::LuaStackProtector protector(L);
	lua::PushMetatable(L, lua::Metatables::MUSIC_MANAGER);
	lua_pushstring(L, "Play");
	lua_pushcfunction(L, lua_MusicManager_Play);
	lua_rawset(L, -3);
	lua_pushstring(L, "Crossfade");
	lua_pushcfunction(L, lua_MusicManager_Crossfade);
	lua_rawset(L, -3);
	lua_pushstring(L, "Fadein");
	lua_pushcfunction(L, lua_MusicManager_Fadein);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::Metatables mt = lua::Metatables::MUSIC_MANAGER;
	FixMusicManagerFunctions(g_LuaEngine->_state);
	lua::RegisterFunction(state, mt, "PlayJingle", Lua_MusicManager_PlayJingle);
	lua::RegisterFunction(state, mt, "StopJingle", Lua_MusicManager_StopJingle);
}
