#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PlayerGetHistory)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	History** toLua = (History**)lua_newuserdata(L, sizeof(History*));
	*toLua = player->GetHistory();
	luaL_setmetatable(L, lua::metatables::HistoryMT);
	return 1;
}

LUA_FUNCTION(Lua_HistoryRemoveHistoryItemByIndex)
{
	History* history = *lua::GetUserdata<History**>(L, 1, lua::metatables::HistoryMT);
	int index = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, history->RemoveHistoryItemByIndex(index));
	return 1;
}

LUA_FUNCTION(Lua_HistoryGetCollectiblesHistory)
{
	History* history = *lua::GetUserdata<History**>(L, 1, lua::metatables::HistoryMT);
	std::vector<History_HistoryItem> historyItems = history->_historyItems;

	lua_newtable(L);
	int idx = 1;
	for (History_HistoryItem& item : historyItems) {
		History_HistoryItem* ud = (History_HistoryItem*)lua_newuserdata(L, sizeof(History_HistoryItem));
		*ud = item;
		luaL_setmetatable(L, lua::metatables::HistoryItemMT);
		lua_rawseti(L, -2, idx);
		idx++;
	}
	return 1;
}

static void RegisterHistory(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY_PLAYER, "GetHistory", Lua_PlayerGetHistory);

	luaL_Reg functions[] = {
		{ "RemoveHistoryItemByIndex", Lua_HistoryRemoveHistoryItemByIndex },
		{ "GetCollectiblesHistory", Lua_HistoryGetCollectiblesHistory },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::HistoryMT, lua::metatables::HistoryMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterHistory(_state);
}