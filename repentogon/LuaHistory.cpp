#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* HistoryItemMT = "HistoryItem";

static int Lua_PlayerGetHistory(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	History** toLua = (History**)lua_newuserdata(L, sizeof(History*));
	*toLua = player->GetHistory();
	luaL_setmetatable(L, lua::metatables::HistoryMT);
	return 1;
}

static int Lua_HistoryRemoveHistoryItemByIndex(lua_State* L)
{
	History* history = *lua::GetUserdata<History**>(L, 1, lua::metatables::HistoryMT);
	int index = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, history->RemoveHistoryItemByIndex(index));
	return 1;
}

static int Lua_HistoryGetCollectiblesHistory(lua_State* L)
{
	History* history = *lua::GetUserdata<History**>(L, 1, lua::metatables::HistoryMT);
	std::vector<History_HistoryItem> historyItems = history->_historyItems;

	lua_newtable(L);
	int idx = 1;
	for (History_HistoryItem& item : historyItems) {
		History_HistoryItem* ud = (History_HistoryItem*)lua_newuserdata(L, sizeof(History_HistoryItem));
		*ud = item;
		luaL_setmetatable(L, HistoryItemMT);
		lua_rawseti(L, -2, idx);
		idx++;
	}
	return 1;
}

static void RegisterHistory(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "GetHistory");
	lua_pushcfunction(L, Lua_PlayerGetHistory);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, lua::metatables::HistoryMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {
		{ "RemoveHistoryItemByIndex", Lua_HistoryRemoveHistoryItemByIndex },
		{ "GetCollectiblesHistory", Lua_HistoryGetCollectiblesHistory },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1);
}

static int Lua_HistoryItemGetItemID(lua_State* L)
{
	History_HistoryItem* historyItem = lua::GetUserdata<History_HistoryItem*>(L, 1, HistoryItemMT);
	lua_pushinteger(L, historyItem->_itemID);

	return 1;
}

static int Lua_HistoryItemGetLevelStage(lua_State* L)
{
	History_HistoryItem* historyItem = lua::GetUserdata<History_HistoryItem*>(L, 1, HistoryItemMT);
	lua_pushinteger(L, historyItem->_levelStage);

	return 1;
}

static int Lua_HistoryItemGetStageType(lua_State* L)
{
	History_HistoryItem* historyItem = lua::GetUserdata<History_HistoryItem*>(L, 1, HistoryItemMT);
	lua_pushinteger(L, historyItem->_stageType);

	return 1;
}

static int Lua_HistoryItemGetRoomType(lua_State* L)
{
	History_HistoryItem* historyItem = lua::GetUserdata<History_HistoryItem*>(L, 1, HistoryItemMT);
	lua_pushinteger(L, historyItem->_roomType);

	return 1;
}

static int Lua_HistoryItemGetItemPoolType(lua_State* L)
{
	History_HistoryItem* historyItem = lua::GetUserdata<History_HistoryItem*>(L, 1, HistoryItemMT);
	lua_pushinteger(L, historyItem->_itemPoolType);

	return 1;
}

static void RegisterHistoryItem(lua_State* L) {

	luaL_newmetatable(L, HistoryItemMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {

		{ "GetItemID", Lua_HistoryItemGetItemID },
		{ "GetLevelStage", Lua_HistoryItemGetLevelStage },
		{ "GetStageType", Lua_HistoryItemGetStageType },
		{ "GetRoomType", Lua_HistoryItemGetRoomType },
		{ "GetItemPoolType", Lua_HistoryItemGetItemPoolType },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterHistory(state);
	RegisterHistoryItem(state);
}