#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_MusicManager_GetCurrentStreamSetId)
{
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	lua_pushinteger(L, music->_currentStream);
	return 1;
}

LUA_FUNCTION(Lua_MusicManager_GetCurrentStreamSet)
{
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	SoundStreamSet* toLua = &music->_streamSets[music->_currentStream];
	SoundStreamSet** luaStreamSet = (SoundStreamSet**)lua_newuserdata(L, sizeof(SoundStreamSet*));
	*luaStreamSet = toLua;
	luaL_setmetatable(L, lua::metatables::SoundStreamSetMT);
	return 1;
}

LUA_FUNCTION(Lua_MusicManager_GetStreamSet)
{
	Music* music = lua::GetUserdata<Music*>(L, 1, lua::Metatables::MUSIC_MANAGER, "MusicManager");
	int setId = (int)luaL_checkinteger(L, 2);

	if (setId < 0 || setId > 1) {
		return luaL_error(L, "Invalid set ID %d. Min = 0, Max = 1", setId);
	}

	SoundStreamSet* toLua = &music->_streamSets[setId];
	SoundStreamSet** luaStreamSet = (SoundStreamSet**)lua_newuserdata(L, sizeof(SoundStreamSet*));
	*luaStreamSet = toLua;
	luaL_setmetatable(L, lua::metatables::SoundStreamSetMT);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamSetGetPrimaryStream)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);
	SoundStream* toLua = &set->_stream;
	SoundStream** luaStream = (SoundStream**)lua_newuserdata(L, sizeof(SoundStream*));
	*luaStream = toLua;
	luaL_setmetatable(L, lua::metatables::SoundStreamMT);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamSetGetLayerStream)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);
	int streamId = (int)luaL_checkinteger(L, 2);

	if (streamId < 0 || streamId > 1) {
		return luaL_error(L, "Invalid music ID %d. Min = 0, Max = 1", streamId);
	}

	SoundStream* toLua = &set->_stream;
	SoundStream** luaStream = (SoundStream**)lua_newuserdata(L, sizeof(SoundStream*));
	*luaStream = toLua;
	luaL_setmetatable(L, lua::metatables::SoundStreamMT);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamSetGetActiveLayers)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);
	lua_pushinteger(L, set->_layerBitfield);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamSetGetVolumeMode)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);
	lua_pushinteger(L, set->_volumeMode);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamSetGetVolume)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);
	lua_pushnumber(L, set->_volume);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamSetGetTargetVolume)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);
	lua_pushnumber(L, set->_targetVolume);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamSetGetFadeRate)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);
	lua_pushnumber(L, set->_fadeRate);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamSetIsPlaying)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);
	lua_pushboolean(L, set->_playing);
	return 1;
}

static void RegisterSoundStreamSet(lua_State* L)
{
	lua::RegisterFunction(L, lua::Metatables::MUSIC_MANAGER, "GetStreamSet", Lua_MusicManager_GetStreamSet);
	lua::RegisterFunction(L, lua::Metatables::MUSIC_MANAGER, "GetCurrentStreamSet", Lua_MusicManager_GetCurrentStreamSet);
	lua::RegisterFunction(L, lua::Metatables::MUSIC_MANAGER, "GetCurrentStreamSetId", Lua_MusicManager_GetCurrentStreamSetId);

	luaL_Reg functions[] = {
		{ "GetPrimaryStream", Lua_SoundStreamSetGetPrimaryStream},
		{ "GetLayerStream", Lua_SoundStreamSetGetLayerStream},
		{ "GetActiveLayers", Lua_SoundStreamSetGetActiveLayers},
		{ "GetVolumeMode", Lua_SoundStreamSetGetVolumeMode},
		{ "GetVolume", Lua_SoundStreamSetGetVolume},
		{ "GetTargetVolume", Lua_SoundStreamSetGetTargetVolume},
		{ "GetFadeRate", Lua_SoundStreamSetGetFadeRate},
		{ "IsPlaying", Lua_SoundStreamSetIsPlaying},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::SoundStreamSetMT, lua::metatables::SoundStreamSetMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterSoundStreamSet(_state);
}