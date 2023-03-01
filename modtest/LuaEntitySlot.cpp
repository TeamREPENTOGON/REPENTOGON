#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

static int Lua_EntityToEntitySlot(lua_State* L) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	if (*entity->GetType() == 6) { //TODO: enum
		Entity_Slot* ud = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));

		luaL_setmetatable(L, lua::metatables::EntitySlotMT);
	}
	else
		lua_pushnil(L);
	return 1;
}

static int Lua_CreateDropsFromExplosion(lua_State* L) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	slot->CreateDropsFromExplosion();
	return 0;
}

static int Lua_SetPrizeCollectible(lua_State* L) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	int collectible = luaL_checkinteger(L, 2);
	slot->SetPrizeCollectible(collectible);
	return 0;
}

static int Lua_RandomCoinJamAnim(lua_State* L) {
	//this is such a trivial reimplementation that like, who cares
	const char* CoinJamAnims[4] = { "CoinJam", "CoinJam2", "CoinJam3", "CoinJam4" };
	const char* CoinJamAnim = CoinJamAnims[Isaac::Random(4)];
	lua_pushstring(L, CoinJamAnim);
	return 1;
}

void RegisterSlotMetatable(lua_State* L) {
	lua::LuaStackProtector protector(L);

	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "ToSlot");
	lua_pushcfunction(L, Lua_EntityToEntitySlot);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, lua::metatables::EntitySlotMT); // meta

	lua_pushstring(L, "__index"); // meta __index
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod); // meta __index metamethod
	lua_rawset(L, -3); // meta

	lua_pushboolean(L, true); // meta true
	lua_rawsetp(L, -2, lua::luabridge::getIdentityKey()); // meta

	lua_pushstring(L, "__parent"); // meta __parent
	lua::PushMetatable(L, lua::Metatables::ENTITY); // meta __parent meta_entity
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__propget"); // meta __propget
	lua_newtable(L); // meta __proget table
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__propset"); // meta __propset
	lua_newtable(L); // meta __propset table
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__const"); // meta __const
	lua_newtable(L); // meta __const table
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__type"); // meta __type
	lua_pushstring(L, "EntitySlot"); // meta __type EntitySlot
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__class"); // meta __class 
	lua_pushstring(L, "EntitySlot"); // meta __class EntitySlot
	lua_rawset(L, -3); // meta

	luaL_Reg funcs[] = {
		{ "CreateDropsFromExplosion", Lua_CreateDropsFromExplosion },
		{ "SetPrizeCollectible", Lua_SetPrizeCollectible },
		{ "RandomCoinJamAnim", Lua_RandomCoinJamAnim },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1); // clean stack
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	RegisterSlotMetatable(g_LuaEngine->_state);
}