#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_LootListConstructor) {
    // Initializes a new LootList (std::deque) under lua ownership and pushes a reference to the stack.
	lua::place<LootList>(L, lua::metatables::LootListMT);
	return 1;
}

LUA_FUNCTION(Lua_LootListPushEntry) {
	LootList* lootList = lua::GetRawUserdata<LootList*>(L, 1, lua::metatables::LootListMT);
	const unsigned int type = (unsigned int)luaL_checkinteger(L, 2);
	const unsigned int variant = (unsigned int)luaL_checkinteger(L, 3);
	const unsigned int subType = (unsigned int)luaL_checkinteger(L, 4);
	const unsigned int seed = (unsigned int)luaL_optinteger(L, 5, Random());
	RNG* rng = nullptr;

	if (lua_type(L, 6) == LUA_TUSERDATA) {
		rng = lua::GetLuabridgeUserdata<RNG*>(L, 6, lua::Metatables::RNG, "RNG");
	}

	lootList->push_back({ type, variant, subType, seed, rng });

	return 0;
}

LUA_FUNCTION(Lua_PickupGetLootList) {
	Entity_Pickup* pickup = lua::GetLuabridgeUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	bool shouldAdvance = lua::luaL_optboolean(L, 2, false);

	LootList* toLua = (LootList*)lua_newuserdata(L, sizeof(LootList));
    luaL_setmetatable(L, lua::metatables::LootListMT);
    // Initialize a new LootList (std::deque) in the memory allocated for the userdata, using "placement new".
    // Using the move constructor, we can "steal" the contents of the deque returned by the function without having to copy everything.
    // If we just copied the raw bytes of the deque, the allocated contents would still be freed alongside the original deque.
    // This allows lua to properly take ownership of the deque. Destroying the deque later is handled by `Lua_LootList__gc`.
    new (toLua) LootList(std::move(pickup->GetLootList(shouldAdvance, nullptr)));

	return 1;
}

LUA_FUNCTION(Lua_NPCGetFireplaceLoot) {
	auto* npc = lua::GetLuabridgeUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	bool shouldAdvance = lua::luaL_optboolean(L, 2, false);

    LootList* toLua = (LootList*)lua_newuserdata(L, sizeof(LootList));
    luaL_setmetatable(L, lua::metatables::LootListMT);
    new (toLua) LootList(std::move(npc->fireplace_get_loot(shouldAdvance)));

	return 1;
}

LUA_FUNCTION(Lua_NPCGetShopkeeperLoot) {
	auto* npc = lua::GetLuabridgeUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	bool shouldAdvance = lua::luaL_optboolean(L, 2, false);

    LootList* toLua = (LootList*)lua_newuserdata(L, sizeof(LootList));
    luaL_setmetatable(L, lua::metatables::LootListMT);
    new (toLua) LootList(std::move(npc->shopkeeper_get_loot(shouldAdvance)));

	return 1;
}

LUA_FUNCTION(Lua_LootListGetEntries)
{
	LootList* lootList = lua::GetRawUserdata<LootList*>(L, 1, lua::metatables::LootListMT);

	lua_newtable(L);
	int idx = 1;
	for (LootListEntry& item : *lootList) {
		LootListEntry** ud = (LootListEntry**)lua_newuserdata(L, sizeof(LootListEntry*));
		*ud = &item;
		luaL_setmetatable(L, lua::metatables::LootListEntryMT);
		lua_rawseti(L, -2, idx);
		idx++;
	}

	return 1;
}

LUA_FUNCTION(Lua_LootList__gc) {
    if (LootList* lootlist = lua::GetRawUserdata<LootList*>(L, 1, lua::metatables::LootListMT)) {
        lootlist->~LootList();
    }
	return 0;
}

static void RegisterLootList(lua_State* L) {

	lua::RegisterFunction(L, lua::Metatables::ENTITY_PICKUP, "GetLootList", Lua_PickupGetLootList);

	lua::RegisterFunction(L, lua::Metatables::ENTITY_NPC, "GetFireplaceLoot", Lua_NPCGetFireplaceLoot);
	lua::RegisterFunction(L, lua::Metatables::ENTITY_NPC, "GetShopkeeperLoot", Lua_NPCGetShopkeeperLoot);

	luaL_Reg functions[] = {
		{ "GetEntries", Lua_LootListGetEntries },
		{ "PushEntry", Lua_LootListPushEntry },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::LootListMT, lua::metatables::LootListMT, functions, Lua_LootList__gc);
	lua_register(L, lua::metatables::LootListMT, Lua_LootListConstructor);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterLootList(_state);
}