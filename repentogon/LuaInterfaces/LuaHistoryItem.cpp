#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_HistoryItemGetTime)
{
	History_HistoryItem* historyItem = lua::GetRawUserdata<History_HistoryItem*>(L, 1, lua::metatables::HistoryItemMT);
	lua_pushinteger(L, historyItem->_time);

	return 1;
}

LUA_FUNCTION(Lua_HistoryItemGetItemID)
{
	History_HistoryItem* historyItem = lua::GetRawUserdata<History_HistoryItem*>(L, 1, lua::metatables::HistoryItemMT);
	lua_pushinteger(L, historyItem->_itemID);

	return 1;
}

LUA_FUNCTION(Lua_HistoryItemGetLevelStage)
{
	History_HistoryItem* historyItem = lua::GetRawUserdata<History_HistoryItem*>(L, 1, lua::metatables::HistoryItemMT);
	lua_pushinteger(L, historyItem->_levelStage);

	return 1;
}

LUA_FUNCTION(Lua_HistoryItemGetStageType)
{
	History_HistoryItem* historyItem = lua::GetRawUserdata<History_HistoryItem*>(L, 1, lua::metatables::HistoryItemMT);
	lua_pushinteger(L, historyItem->_stageType);

	return 1;
}

LUA_FUNCTION(Lua_HistoryItemGetRoomType)
{
	History_HistoryItem* historyItem = lua::GetRawUserdata<History_HistoryItem*>(L, 1, lua::metatables::HistoryItemMT);
	lua_pushinteger(L, historyItem->_roomType);

	return 1;
}

LUA_FUNCTION(Lua_HistoryItemGetItemPoolType)
{
	History_HistoryItem* historyItem = lua::GetRawUserdata<History_HistoryItem*>(L, 1, lua::metatables::HistoryItemMT);
	lua_pushinteger(L, historyItem->_itemPoolType);

	return 1;
}

LUA_FUNCTION(Lua_HistoryItemIsTrinket)
{
	History_HistoryItem* historyItem = lua::GetRawUserdata<History_HistoryItem*>(L, 1, lua::metatables::HistoryItemMT);
	lua_pushboolean(L, historyItem->_isTrinket);

	return 1;
}

static void RegisterHistoryItem(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetTime", Lua_HistoryItemGetTime },
		{ "GetItemID", Lua_HistoryItemGetItemID },
		{ "GetLevelStage", Lua_HistoryItemGetLevelStage },
		{ "GetStageType", Lua_HistoryItemGetStageType },
		{ "GetRoomType", Lua_HistoryItemGetRoomType },
		{ "GetItemPoolType", Lua_HistoryItemGetItemPoolType },
		{ "IsTrinket", Lua_HistoryItemIsTrinket },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::HistoryItemMT, lua::metatables::HistoryItemMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterHistoryItem(_state);
}