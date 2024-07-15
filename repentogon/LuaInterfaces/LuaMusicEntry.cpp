#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_MusicManager_GetEntry)
{
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	int musicId = (int)luaL_checkinteger(L, 2);
	int max;

	if (!music->ValidateMusicID(musicId, max)) {
		return luaL_error(L, "Invalid music ID %d. Min = 0, Max = %d", musicId, max - 1);
	}

	MusicEntry* toLua = music->_entries[musicId];
	MusicEntry** luaMusicEntry = (MusicEntry**)lua_newuserdata(L, sizeof(MusicEntry*));
	*luaMusicEntry = toLua;
	luaL_setmetatable(L, lua::metatables::MusicEntryMT);
	return 1;
}

LUA_FUNCTION(Lua_MusicEntryGetID)
{
	MusicEntry* entry = *lua::GetUserdata<MusicEntry**>(L, 1, lua::metatables::MusicEntryMT);
	lua_pushinteger(L, entry->_id);
	return 1;
}

static void RegisterMusicEntry(lua_State* L)
{
	lua::RegisterFunction(L, lua::Metatables::MUSIC_MANAGER, "GetEntry", Lua_MusicManager_GetEntry);

	luaL_Reg functions[] = {
		{ "GetId", Lua_MusicEntryGetID},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::NullFrameMT, lua::metatables::NullFrameMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterMusicEntry(_state);
}