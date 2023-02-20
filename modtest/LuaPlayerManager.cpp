#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* PlayerManagerMT = "PlayerManager";

static int Lua_GetPlayerManager(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	void** ud = (void**)lua_newuserdata(L, sizeof(void*));
	*ud = (char*)game + 0x1AB40;
	luaL_setmetatable(L, PlayerManagerMT);
	return 1;
}

int Lua_FirstCollectibleOwner(lua_State* L)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, PlayerManagerMT);
	int collectible = luaL_checkinteger(L, 2);
	RNG* rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");
	bool unk = lua_toboolean(L, 4);

	Entity_Player* player = playerManager->FirstCollectibleOwner(collectible, *rng, unk);
	if (!player) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	return 1;
}

int Lua_SpawnCoPlayer2(lua_State* L)
{
	PlayerManager* playerManager = lua::GetUserdata<PlayerManager*>(L, 1, PlayerManagerMT);
	int playerType = luaL_checkinteger(L, 2);

	Entity_Player* player = playerManager->SpawnCoPlayer2(playerType);
	lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));

	return 1;
}

static void RegisterPlayerManager(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetPlayerManager");
	lua_pushcfunction(L, Lua_GetPlayerManager);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, PlayerManagerMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "FirstCollectibleOwner", Lua_FirstCollectibleOwner },
		{ "SpawnCoPlayer2", Lua_SpawnCoPlayer2 },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterPlayerManager(state);
}