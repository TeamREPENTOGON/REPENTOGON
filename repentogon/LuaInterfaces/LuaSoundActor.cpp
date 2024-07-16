#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

bool IsSourceOpen(SoundActor* actor) {
	return (actor->_source != nullptr && actor->_source->IsOpen());
}

LUA_FUNCTION(Lua_SoundActorPlay) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (IsSourceOpen(actor))
	{
		actor->_source->Play(true);
	}
	else
		return luaL_error(L, "SoundSource is not open!");
	return 0;
}

LUA_FUNCTION(Lua_SoundActorPause) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (IsSourceOpen(actor))
	{
		actor->_source->Pause();
		actor->_paused = true;
		actor->_playing = false;
	}
	else
		return luaL_error(L, "SoundSource is not open!");
	return 0;
}

LUA_FUNCTION(Lua_SoundActorResume) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (IsSourceOpen(actor))
	{
		actor->_source->Resume();
		actor->_paused = false;
		actor->_playing = true;
	}
	else
		return luaL_error(L, "SoundSource is not open!");
	return 0;
}

LUA_FUNCTION(Lua_SoundActorStop) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (actor->_source == nullptr)
		return luaL_error(L, "SoundSource is uninitialized!");
	actor->_source->Stop();
	actor->_playing = false;
	return 0;
}

LUA_FUNCTION(Lua_SoundActorGetPitch) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (actor->_source == nullptr)
		return luaL_error(L, "SoundSource is uninitialized!");
	lua_pushnumber(L, actor->_source->GetPitch());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorSetPitch) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	float pitch = luaL_checknumber(L, 2);
	if (IsSourceOpen(actor))
		actor->_source->SetPitch(pitch);
	else
		return luaL_error(L, "SoundSource is not open!");
	return 0;
}

LUA_FUNCTION(Lua_SoundActorGetPan) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (actor->_source == nullptr)
		return luaL_error(L, "SoundSource is uninitialized!");
	lua_pushnumber(L, actor->_source->GetPan());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorSetPan) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	float pan = luaL_checknumber(L, 2);
	if (IsSourceOpen(actor))
		actor->_source->SetPan(pan);
	else
		return luaL_error(L, "SoundSource is not open!");
	return 0;
}

LUA_FUNCTION(Lua_SoundActorGetVolume) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (IsSourceOpen(actor))
		lua_pushnumber(L, actor->_source->GetVolume());
	else
		return luaL_error(L, "SoundSource is not open!");
	return 1;
}

LUA_FUNCTION(Lua_SoundActorSetVolume) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	float volume = luaL_checknumber(L, 2);
	if (IsSourceOpen(actor))
		actor->_source->SetVolume(volume);
	else
		return luaL_error(L, "SoundSource is not open!");
	return 0;
}

LUA_FUNCTION(Lua_SoundActorGetLooping) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (actor->_source == nullptr)
		return luaL_error(L, "SoundSource is uninitialized!");
	lua_pushboolean(L, actor->_source->IsLooping());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorSetLooping) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	bool loop = lua::luaL_checkboolean(L, 2);
	if (IsSourceOpen(actor))
		actor->_source->SetLooping(loop);
	else
		return luaL_error(L, "SoundSource is not open!");
	return 0;
}

LUA_FUNCTION(Lua_SoundActorGetNumChannels) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (actor->_source == nullptr)
		return luaL_error(L, "SoundSource is uninitialized!");
	lua_pushinteger(L, actor->_source->GetNumChannels());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorGetSampleRate) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (actor->_source == nullptr)
		return luaL_error(L, "SoundSource is uninitialized!");
	lua_pushinteger(L, actor->_source->GetSampleRate());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorGetSourceFormat) {
	SoundActor* actor = *lua::GetUserdata<SoundActor**>(L, 1, lua::metatables::SoundActorMT);
	if (actor->_source == nullptr)
		return luaL_error(L, "SoundSource is uninitialized!");
	lua_pushinteger(L, actor->_source->GetSourceFormat());
	return 1;
}

static void RegisterSoundActor(lua_State* L)
{
	luaL_Reg functions[] = {
		{ "Play", Lua_SoundActorPlay},
		{ "Pause", Lua_SoundActorPause},
		{ "Resume", Lua_SoundActorResume},
		{ "Stop", Lua_SoundActorStop},
		{ "GetPitch", Lua_SoundActorGetPitch},
		{ "GetPan", Lua_SoundActorGetPan},
		{ "GetVolume", Lua_SoundActorGetVolume},
		{ "GetLooping", Lua_SoundActorGetLooping},
		{ "GetNumChannels", Lua_SoundActorGetNumChannels},
		{ "GetSampleRate", Lua_SoundActorGetSampleRate},
		{ "GetSourceFormat", Lua_SoundActorGetSourceFormat},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::SoundActorMT, lua::metatables::SoundActorMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterSoundActor(_state);
}