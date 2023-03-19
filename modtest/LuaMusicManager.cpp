#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

static int lua_MusicManager_Play(lua_State* L) {
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	int musicId = luaL_checkinteger(L, 2);
	if (musicId < 0) {
		return luaL_error(L, "%d is not a valid music identifier", musicId);
	}

	void* ecx = *(void**)((char*)music + 0x320);
	void* edi = *(void**)((char*)music + 0x31C);

	ptrdiff_t length = (ptrdiff_t)ecx - (ptrdiff_t)edi;
	int bound = length / 96; // Size of the content of the array

	if (musicId >= bound) {
		return luaL_error(L, "%d is not a valid music identifier (max = %d)", musicId, bound - 1);
	}
	
	float volume = luaL_optnumber(L, 3, -1);
	music->Play(musicId, volume);
	return 0;
}

static void FixMusicManagerPlay(lua_State* L) {
	lua::LuaStackProtector protector(L);
	lua::PushMetatable(L, lua::Metatables::MUSIC_MANAGER);
	lua_pushstring(L, "Play");
	lua_pushcfunction(L, lua_MusicManager_Play);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	FixMusicManagerPlay(g_LuaEngine->_state);
}