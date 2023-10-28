#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetNightmareScene) {
	Manager* manager = g_Manager;
	NightmareScene** ud = (NightmareScene**)lua_newuserdata(L, sizeof(NightmareScene*));
	*ud = manager->GetNightmareScene();
	luaL_setmetatable(L, lua::metatables::NightmareSceneMT);
	return 1;
}

/*LUA_FUNCTION(Lua_NightmareSceneShow)
{
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	bool unk = lua_toboolean(L, 2);
	nc->Show(unk);

	return 0;
}*/

LUA_FUNCTION(Lua_NightmareSceneGetBackgroundSprite)
{
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	ANM2* anm2 = nc->GetBackgroundSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_NightmareSceneGetBubbleSprite)
{
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	ANM2* anm2 = nc->GetBubbleSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_NightmareSceneGetProgressBarSprite)
{
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	ANM2* anm2 = nc->GetProgressBarSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_NightmareSceneGetProgressBarMap) {
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

LUA_FUNCTION(Lua_NightmareSceneIsDogmaNightmare) {
	NightmareScene* nc = *lua::GetUserdata<NightmareScene**>(L, 1, lua::metatables::NightmareSceneMT);
	lua_pushboolean(L, *nc->IsDogmaNightmare());
	return 1;
}

static void RegisterNightmareScene(lua_State* L)
{
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "GetNightmareScene", Lua_GetNightmareScene);

	luaL_Reg functions[] = {
		//{ "Show", Lua_NightmareSceneShow },
		{ "GetBackgroundSprite", Lua_NightmareSceneGetBackgroundSprite },
		{ "GetBubbleSprite", Lua_NightmareSceneGetBubbleSprite },
		{ "GetProgressBarSprite", Lua_NightmareSceneGetProgressBarSprite },
		{ "GetProgressBarMap", Lua_NightmareSceneGetProgressBarMap },
		{ "IsDogmaNightmare", Lua_NightmareSceneIsDogmaNightmare },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::NightmareSceneMT, lua::metatables::NightmareSceneMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterNightmareScene(_state);
}