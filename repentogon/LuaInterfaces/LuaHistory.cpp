#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PlayerGetHistory)
{
	Entity_Player* player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	History** toLua = (History**)lua_newuserdata(L, sizeof(History*));
	*toLua = player->GetHistory();
	luaL_setmetatable(L, lua::metatables::HistoryMT);
	return 1;
}

LUA_FUNCTION(Lua_HistoryRemoveHistoryItemByIndex)
{
	History* history = *lua::GetRawUserdata<History**>(L, 1, lua::metatables::HistoryMT);
	int index = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, history->RemoveHistoryItemByIndex(index));
	return 1;
}

LUA_FUNCTION(Lua_HistoryGetCollectiblesHistory)
{
	History* history = *lua::GetRawUserdata<History**>(L, 1, lua::metatables::HistoryMT);

	lua_newtable(L);
	int idx = 1;
	for (const History_HistoryItem& item : history->_historyItems) {
		History_HistoryItem* ud = (History_HistoryItem*)lua_newuserdata(L, sizeof(History_HistoryItem));
		*ud = item;
		luaL_setmetatable(L, lua::metatables::HistoryItemMT);
		lua_rawseti(L, -2, idx);
		idx++;
	}
	return 1;
}

LUALIB_API int SearchHistory(lua_State* L, const bool trinkets) {
	History* history = *lua::GetRawUserdata<History**>(L, 1, lua::metatables::HistoryMT);

	std::set<int> filterIDs;

	if (lua_type(L, 2) == LUA_TTABLE) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isinteger(L, -1)) {
				filterIDs.insert((int)lua_tointeger(L, -1));
				int id = (int)lua_tointeger(L, -1);
				if (trinkets) {
					id &= TRINKET_ID_MASK;
				}
				filterIDs.insert(id);
			}
			lua_pop(L, 1);
		}
	} else if (!lua_isnoneornil(L, 2)) {
		int id = (int)luaL_checkinteger(L, 2);
		if (trinkets) {
			id &= TRINKET_ID_MASK;
		}
		filterIDs.insert(id);
	}

	lua_newtable(L);
	int idx = 1;
	for (const History_HistoryItem& item : history->_historyItems) {
		int id = item._itemID;
		if (item._isTrinket) {
			id &= TRINKET_ID_MASK;
		}
		if (trinkets == item._isTrinket && (filterIDs.empty() || filterIDs.find(id) != filterIDs.end())) {
			History_HistoryItem* ud = (History_HistoryItem*)lua_newuserdata(L, sizeof(History_HistoryItem));
			*ud = item;
			luaL_setmetatable(L, lua::metatables::HistoryItemMT);
			lua_rawseti(L, -2, idx);
			idx++;
		}
	}
	return 1;
}
LUA_FUNCTION(Lua_HistorySearchCollectibles) {
	return SearchHistory(L, false);
}
LUA_FUNCTION(Lua_HistorySearchTrinkets) {
	return SearchHistory(L, true);
}

static void RegisterHistory(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY_PLAYER, "GetHistory", Lua_PlayerGetHistory);

	luaL_Reg functions[] = {
		{ "RemoveHistoryItemByIndex", Lua_HistoryRemoveHistoryItemByIndex },
		{ "GetCollectiblesHistory", Lua_HistoryGetCollectiblesHistory },
		{ "SearchCollectibles", Lua_HistorySearchCollectibles },
		{ "SearchTrinkets", Lua_HistorySearchTrinkets },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::HistoryMT, lua::metatables::HistoryMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterHistory(_state);
}