#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

LUA_FUNCTION(Lua_EntityToEntitySlot) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	if (*entity->GetType() == 6) { //TODO: enum
		Entity_Slot* ud = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));

		luaL_setmetatable(L, lua::metatables::EntitySlotMT);
	}
	else
		lua_pushnil(L);
	return 1;
}

LUA_FUNCTION(Lua_CreateDropsFromExplosion) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	slot->CreateDropsFromExplosion();
	return 0;
}

LUA_FUNCTION(Lua_GetPrizeCollectible) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *slot->GetPrizeCollectible());
	return 1;
}

LUA_FUNCTION(Lua_SetPrizeCollectible) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	int collectible = (int)luaL_checkinteger(L, 2);
	slot->SetPrizeCollectible(collectible);
	return 0;
}

LUA_FUNCTION(Lua_RandomCoinJamAnim) {
	//this is such a trivial reimplementation that like, who cares
	const char* CoinJamAnims[4] = { "CoinJam", "CoinJam2", "CoinJam3", "CoinJam4" };
	const char* CoinJamAnim = CoinJamAnims[Isaac::Random(4)];
	lua_pushstring(L, CoinJamAnim);
	return 1;
}

/*LUA_FUNCTION(Lua_GetSlotRNG) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));

	RNG* toLua = lua::luabridge::UserdataValue<RNG>::place(L, lua::GetMetatableKey(lua::Metatables::RNG));
	*toLua = *slot->GetSlotRNG();
	return 1;
}
*/

LUA_FUNCTION(Lua_GetState) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *slot->GetState());
	return 1;
}

LUA_FUNCTION(Lua_SetState) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	*slot->GetState() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_GetPrizeType) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *slot->GetPrizeType());
	return 1;
}

LUA_FUNCTION(Lua_SetPrizeType) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	*slot->GetPrizeType() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_GetShellGameAnimationIndex) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	lua_pushinteger(L, slot->_shellGameAnimationIndex);

	return 1;
}

LUA_FUNCTION(Lua_SetShellGameAnimationIndex) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	slot->_shellGameAnimationIndex = (short)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_GetDonationValue) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *slot->GetDonationValue());
	return 1;
}

LUA_FUNCTION(Lua_SetDonationValue) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	*slot->GetDonationValue() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_GetTriggerTimer) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	lua_pushinteger(L, slot->_triggerTimer);

	return 1;
}

LUA_FUNCTION(Lua_SetTriggerTimer) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	slot->_triggerTimer = (unsigned int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_GetTimeout) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *slot->GetTimeout());
	return 1;
}

LUA_FUNCTION(Lua_SetTimeout) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	*slot->GetTimeout() = (unsigned short)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_GetTouch) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	lua_pushinteger(L, *slot->GetTouch());
	return 1;
}

LUA_FUNCTION(Lua_SetTouch) {
	luaL_checkudata(L, 1, lua::metatables::EntitySlotMT);
	Entity_Slot* slot = lua::UserdataToData<Entity_Slot*>(lua_touserdata(L, 1));
	*slot->GetTouch() = (unsigned short)luaL_checkinteger(L, 2);
	return 0;
}

void RegisterSlotMetatable(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY, "ToSlot", Lua_EntityToEntitySlot);

	luaL_newmetatable(L, lua::metatables::EntitySlotMT); // meta

	lua_pushstring(L, "__index"); // meta __index
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod); // meta __index metamethod
	lua_rawset(L, -3); // meta

	lua_pushstring(L, "__newindex"); // meta __newindex
	lua_pushcfunction(L, lua::luabridge::newIndexMetaMethod); // meta __newindex metamethod
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
		{ "GetPrizeCollectible", Lua_GetPrizeCollectible },
		{ "SetPrizeCollectible", Lua_SetPrizeCollectible },
		{ "RandomCoinJamAnim", Lua_RandomCoinJamAnim },
		//{ "GetSlotRNG", Lua_GetSlotRNG },
		{ "GetState", Lua_GetState },
		{ "SetState", Lua_SetState },
		{ "GetPrizeType", Lua_GetPrizeType },
		{ "SetPrizeType", Lua_SetPrizeType },
		{ "GetShellGameAnimationIndex", Lua_GetShellGameAnimationIndex },
		{ "SetShellGameAnimationIndex", Lua_SetShellGameAnimationIndex },
		{ "GetDonationValue", Lua_GetDonationValue },
		{ "SetDonationValue", Lua_SetDonationValue },
		{ "GetTriggerTimerNum", Lua_GetTriggerTimer },
		{ "SetTriggerTimerNum", Lua_SetTriggerTimer },
		{ "GetTimeout", Lua_GetTimeout },
		{ "SetTimeout", Lua_SetTimeout },
		{ "GetTouch", Lua_GetTouch },
		{ "SetTouch", Lua_SetTouch },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1); // clean stack
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterSlotMetatable(_state);
}