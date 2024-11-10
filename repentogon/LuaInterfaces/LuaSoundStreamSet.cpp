#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

float musicVolumes[7] = { 1, 1, 1, 1, 1, 1, 1 };

HOOK_METHOD(SoundStreamSet, game_constructor, () -> SoundStreamSet*) {
	SoundStreamSet* set = super();

	for (size_t i = 0; i < 7; i++) {
		set->_streams[i]._unused = (uint8_t)i;
	}

	return set;
}

HOOK_METHOD(SoundStream, Update, (bool checkIfPlaying) -> void) {
	// exclude jingle stream
	if (this->_setRef) {
		// main stream target volume isn't constantly recalculated like layers are
		float target = this->_unused == 0 ? g_Manager->GetOptions()->_musicVolume : this->_targetVolume;
		this->_targetVolume = target * musicVolumes[this->_unused];
	}

	super(checkIfPlaying);
}

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

LUA_FUNCTION(Lua_SoundStreamSetGetStream)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);
	int streamId = (int)luaL_checkinteger(L, 2);

	if (streamId < 0 || streamId > 6) {
		return luaL_error(L, "Invalid music ID %d. Min = 0, Max = 6", streamId);
	}

	SoundStream* toLua = &set->_streams[streamId];
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

LUA_FUNCTION(Lua_SoundStreamSetGetTargetVolumeMultiplier)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);

	int streamId = (int)luaL_checkinteger(L, 2);

	if (streamId < 0 || streamId > 6) {
		return luaL_error(L, "Invalid music ID %d. Min = 0, Max = 6", streamId);
	}
	lua_pushnumber(L, musicVolumes[streamId]);

	return 1;
}

LUA_FUNCTION(Lua_SoundStreamSetSetTargetVolumeMultiplier)
{
	SoundStreamSet* set = *lua::GetUserdata<SoundStreamSet**>(L, 1, lua::metatables::SoundStreamSetMT);

	int streamId = (int)luaL_checkinteger(L, 2);

	if (streamId < 0 || streamId > 6) {
		return luaL_error(L, "Invalid music ID %d. Min = 0, Max = 6", streamId);
	}
	
	musicVolumes[streamId] = luaL_checknumber(L, 3);

	return 0;
}

static void RegisterSoundStreamSet(lua_State* L)
{
	lua::RegisterFunction(L, lua::Metatables::MUSIC_MANAGER, "GetStreamSet", Lua_MusicManager_GetStreamSet);
	lua::RegisterFunction(L, lua::Metatables::MUSIC_MANAGER, "GetCurrentStreamSet", Lua_MusicManager_GetCurrentStreamSet);
	lua::RegisterFunction(L, lua::Metatables::MUSIC_MANAGER, "GetCurrentStreamSetId", Lua_MusicManager_GetCurrentStreamSetId);

	luaL_Reg functions[] = {
		{ "GetStream", Lua_SoundStreamSetGetStream},
		{ "GetActiveLayers", Lua_SoundStreamSetGetActiveLayers},
		{ "GetLayerMode", Lua_SoundStreamSetGetVolumeMode},
		{ "GetVolume", Lua_SoundStreamSetGetVolume},
		{ "GetTargetVolume", Lua_SoundStreamSetGetTargetVolume},
		{ "GetFadeRate", Lua_SoundStreamSetGetFadeRate},
		{ "IsPlaying", Lua_SoundStreamSetIsPlaying},
		{ "GetTargetVolumeMultiplier", Lua_SoundStreamSetGetTargetVolumeMultiplier},
		{ "SetTargetVolumeMultiplier", Lua_SoundStreamSetSetTargetVolumeMultiplier},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::SoundStreamSetMT, lua::metatables::SoundStreamSetMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterSoundStreamSet(_state);
}