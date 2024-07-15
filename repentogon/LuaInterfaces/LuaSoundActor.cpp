#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_SoundActorPlay) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	char* unk;
	actor->_source->Play(unk);
	return 0;
}

LUA_FUNCTION(Lua_SoundActorPause) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	actor->_source->Pause();
	return 0;
}

LUA_FUNCTION(Lua_SoundActorResume) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	actor->_source->Resume();
	return 0;
}

LUA_FUNCTION(Lua_SoundActorStop) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	actor->_source->Stop();
	return 0;
}

LUA_FUNCTION(Lua_SoundActorGetPitch) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	lua_pushnumber(L, actor->_source->GetPitch());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorSetPitch) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	float pitch = luaL_checknumber(L, 2);
	actor->_source->SetPitch(pitch);
	return 0;
}

LUA_FUNCTION(Lua_SoundActorGetPan) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	lua_pushnumber(L, actor->_source->GetPan());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorSetPan) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	float pan = luaL_checknumber(L, 2);
	actor->_source->SetPan(pan);
	return 0;
}

LUA_FUNCTION(Lua_SoundActorGetVolume) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	lua_pushnumber(L, actor->_source->GetVolume());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorSetVolume) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	float volume = luaL_checknumber(L, 2);
	actor->_source->SetVolume(volume);
	return 0;
}

LUA_FUNCTION(Lua_SoundActorGetLooping) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	lua_pushboolean(L, actor->_source->IsLooping());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorSetLooping) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	bool loop = lua::luaL_checkboolean(L, 2);
	actor->_source->SetLooping(loop);
	return 0;
}

LUA_FUNCTION(Lua_SoundActorGetNumChannels) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	lua_pushinteger(L, actor->_source->GetNumChannels());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorGetSampleRate) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
	lua_pushinteger(L, actor->_source->GetSampleRate());
	return 1;
}

LUA_FUNCTION(Lua_SoundActorGetSourceFormat) {
	SoundActor* actor = lua::GetUserdata<SoundActor*>(L, 1, lua::metatables::SoundActorMT);
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