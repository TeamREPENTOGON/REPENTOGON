#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_SoundStreamGetActor)
{
	SoundStream* stream = *lua::GetUserdata<SoundStream**>(L, 1, lua::metatables::SoundStreamMT);
	SoundActor* toLua = &stream->_actor;
	SoundActor** luaSoundActor = (SoundActor**)lua_newuserdata(L, sizeof(SoundActor*));
	*luaSoundActor = toLua;
	luaL_setmetatable(L, lua::metatables::SoundActorMT);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamGetVolume)
{
	SoundStream* stream = *lua::GetUserdata<SoundStream**>(L, 1, lua::metatables::SoundStreamMT);
	lua_pushnumber(L, stream->_volume);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamGetTargetVolume)
{
	SoundStream* stream = *lua::GetUserdata<SoundStream**>(L, 1, lua::metatables::SoundStreamMT);
	lua_pushnumber(L, stream->_targetVolume);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamGetVolumeModifier)
{
	SoundStream* stream = *lua::GetUserdata<SoundStream**>(L, 1, lua::metatables::SoundStreamMT);
	lua_pushnumber(L, stream->_volumeMod);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamGetFadeRate)
{
	SoundStream* stream = *lua::GetUserdata<SoundStream**>(L, 1, lua::metatables::SoundStreamMT);
	lua_pushnumber(L, stream->_volume);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamIsPlaying)
{
	SoundStream* stream = *lua::GetUserdata<SoundStream**>(L, 1, lua::metatables::SoundStreamMT);
	lua_pushboolean(L, stream->_playing);
	return 1;
}

LUA_FUNCTION(Lua_SoundStreamGetFrameCount)
{
	SoundStream* stream = *lua::GetUserdata<SoundStream**>(L, 1, lua::metatables::SoundStreamMT);
	lua_pushinteger(L, stream->_frameCount);
	return 1;
}

static void RegisterSoundStreamSet(lua_State* L)
{
	luaL_Reg functions[] = {
		{ "GetActor", Lua_SoundStreamGetActor},
		{ "GetVolume", Lua_SoundStreamGetVolume},
		{ "GetTargetVolume", Lua_SoundStreamGetTargetVolume},
		{ "GetVolumeModifier", Lua_SoundStreamGetVolumeModifier},
		{ "GetFadeRate", Lua_SoundStreamGetFadeRate},
		{ "IsPlaying", Lua_SoundStreamIsPlaying},
		{ "GetFrameCount", Lua_SoundStreamGetFrameCount},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::SoundStreamSetMT, lua::metatables::SoundStreamSetMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterSoundStreamSet(_state);
}