#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_GetNightmareScene(lua_State* L) {
	Manager* manager = g_Manager;
	NightmareScene** ud = (NightmareScene**)lua_newuserdata(L, sizeof(NightmareScene*));
	*ud = manager->GetNightmareScene();
	luaL_setmetatable(L, lua::metatables::NightmareSceneMT);
	return 1;
}

int Lua_NightmareSceneShow(lua_State* L)
{
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	bool unk = lua_toboolean(L, 2);
	nc->Show(unk);

	return 0;
}

int Lua_NightmareSceneGetBackgroundSprite(lua_State* L)
{
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	ANM2* anm2 = nc->GetBackgroundSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

int Lua_NightmareSceneGetBubbleSprite(lua_State* L)
{
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	ANM2* anm2 = nc->GetBubbleSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

int Lua_NightmareSceneGetProgressBarSprite(lua_State* L)
{
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	ANM2* anm2 = nc->GetProgressBarSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

int Lua_NightmareSceneGetProgressBarMap(lua_State* L) {
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	lua_newtable(L);
	int* content = nc->GetProgressBarMap();
	for (int i = 0; i < 14; ++i) {
		lua_pushinteger(L, i + 1);
		lua_pushinteger(L, *content++);
		lua_rawset(L, -3);
	}
	return 1;
}

int Lua_NightmareSceneIsDogmaNightmare(lua_State* L) {
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	lua_pushboolean(L, *nc->IsDogmaNightmare());
	return 1;
}

static void RegisterNightmareScene(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetNightmareScene");
	lua_pushcfunction(L, Lua_GetNightmareScene);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, lua::metatables::NightmareSceneMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		//{ "Show", Lua_NightmareSceneShow },
		{ "GetBackgroundSprite", Lua_NightmareSceneGetBackgroundSprite },
		{ "GetBubbleSprite", Lua_NightmareSceneGetBubbleSprite },
		{ "GetProgressBarSprite", Lua_NightmareSceneGetProgressBarSprite },
		{ "GetProgressBarMap", Lua_NightmareSceneGetProgressBarMap },
		{ "IsDogmaNightmare", Lua_NightmareSceneIsDogmaNightmare },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterNightmareScene(state);
}