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

	MusicEntry* toLua = &music->_entries[musicId];
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

LUA_FUNCTION(Lua_MusicEntryGetName)
{
	MusicEntry* entry = *lua::GetUserdata<MusicEntry**>(L, 1, lua::metatables::MusicEntryMT);
	lua_pushstring(L, entry->_name.empty() ? "" : entry->_name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_MusicEntryGetIntroFilename)
{
	MusicEntry* entry = *lua::GetUserdata<MusicEntry**>(L, 1, lua::metatables::MusicEntryMT);
	lua_pushstring(L, entry->_introFilename.empty() ? "" : entry->_introFilename.c_str());
	return 1;
}

LUA_FUNCTION(Lua_MusicEntryGetFilename)
{
	MusicEntry* entry = *lua::GetUserdata<MusicEntry**>(L, 1, lua::metatables::MusicEntryMT);
	lua_pushstring(L, entry->_filename.empty() ? "" : entry->_filename.c_str());
	return 1;
}

LUA_FUNCTION(Lua_MusicEntryGetLoop)
{
	MusicEntry* entry = *lua::GetUserdata<MusicEntry**>(L, 1, lua::metatables::MusicEntryMT);
	lua_pushboolean(L, entry->_loop);
	return 1;
}

LUA_FUNCTION(Lua_MusicEntryGetLayerMode)
{
	MusicEntry* entry = *lua::GetUserdata<MusicEntry**>(L, 1, lua::metatables::MusicEntryMT);
	lua_pushinteger(L, entry->_layerMode);
	return 1;
}

LUA_FUNCTION(Lua_MusicEntryGetMultiplier)
{
	MusicEntry* entry = *lua::GetUserdata<MusicEntry**>(L, 1, lua::metatables::MusicEntryMT);
	lua_pushnumber(L, entry->_multiplier);
	return 1;
}

LUA_FUNCTION(Lua_MusicEntryGetLayerFadeRate)
{
	MusicEntry* entry = *lua::GetUserdata<MusicEntry**>(L, 1, lua::metatables::MusicEntryMT);
	lua_pushnumber(L, entry->_layerFadeRate);
	return 1;
}

LUA_FUNCTION(Lua_MusicEntryGetLayerCount)
{
	MusicEntry* entry = *lua::GetUserdata<MusicEntry**>(L, 1, lua::metatables::MusicEntryMT);
	lua_pushinteger(L, entry->_numLayers);
	return 1;
}

LUA_FUNCTION(Lua_MusicEntryGetLayer)
{
	MusicEntry* entry = *lua::GetUserdata<MusicEntry**>(L, 1, lua::metatables::MusicEntryMT);
	int id = (int)luaL_checkinteger(L, 2);
	if (id < 0 || id > entry->_numLayers-1) {
		return luaL_error(L, "Invalid layer ID %d. Min = 0, Max = %d", id, entry->_numLayers - 1);
	}
	MusicLayer* toLua = &entry->_layers[id];
	MusicLayer** luaMusicLayer = (MusicLayer**)lua_newuserdata(L, sizeof(MusicLayer*));
	*luaMusicLayer = toLua;
	luaL_setmetatable(L, lua::metatables::MusicLayerMT);
	return 1;
}

LUA_FUNCTION(Lua_MusicLayerGetIntroFilename)
{
	MusicLayer* layer = *lua::GetUserdata<MusicLayer**>(L, 1, lua::metatables::MusicLayerMT);
	lua_pushstring(L, layer->_intro.empty() ? "" : layer->_intro.c_str());
	return 1;
}

LUA_FUNCTION(Lua_MusicLayerGetFilename)
{
	MusicLayer* layer = *lua::GetUserdata<MusicLayer**>(L, 1, lua::metatables::MusicLayerMT);
	lua_pushstring(L, layer->_path.empty() ? "" : layer->_path.c_str());
	return 1;
}

LUA_FUNCTION(Lua_MusicLayerGetMultiplier)
{
	MusicLayer* layer = *lua::GetUserdata<MusicLayer**>(L, 1, lua::metatables::MusicLayerMT);
	lua_pushnumber(L, layer->_mul);
	return 1;
}

static void RegisterMusicEntry(lua_State* L)
{
	lua::RegisterFunction(L, lua::Metatables::MUSIC_MANAGER, "GetEntry", Lua_MusicManager_GetEntry);

	luaL_Reg entryFunctions[] = {
		{ "GetId", Lua_MusicEntryGetID},
		{ "GetName", Lua_MusicEntryGetID},
		{ "GetIntroFilename", Lua_MusicEntryGetIntroFilename},
		{ "GetFilename", Lua_MusicEntryGetFilename},
		{ "GetLoop", Lua_MusicEntryGetLoop},
		{ "GetLayerMode", Lua_MusicEntryGetLayerMode},
		{ "GetMultiplier", Lua_MusicEntryGetMultiplier},
		{ "GetLayerFadeRate", Lua_MusicEntryGetLayerFadeRate},
		{ "GetLayerCount", Lua_MusicEntryGetLayerCount},
		{ "GetLayer", Lua_MusicEntryGetLayer},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::MusicEntryMT, lua::metatables::MusicEntryMT, entryFunctions);

	luaL_Reg layerFunctions[] = {
	{ "GetIntroFilename", Lua_MusicLayerGetIntroFilename},
	{ "GetFilename", Lua_MusicLayerGetFilename},
	{ "GetMultiplier", Lua_MusicLayerGetMultiplier},
	{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::MusicLayerMT, lua::metatables::MusicLayerMT, layerFunctions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterMusicEntry(_state);
}