#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

// Struct that pairs HistoryHUD_Item with its index information for use as userdata values.
struct HistoryHUD_Item_Ex {
	int playerSlot;
	int index;
	HistoryHUD_Item hudItem;
};

static Vector CalculateHistoryHUDItemRenderOffset(const int playerSlot, const int index) {
	const HistoryHUD& historyHUD = g_Game->GetHUD()->_historyHUD;

	const int numColumns = historyHUD.GetNumColumns();
	Vector offset;
	offset.x = (float)(index % numColumns);
	offset.y = (float)std::floor(index / numColumns);
	offset *= historyHUD.GetIconSize();
	if (historyHUD.HasTwin()) {
		offset.x += -2 + 33 * playerSlot;
	}

	return offset;
}

LUA_FUNCTION(Lua_HUD_GetHistoryHUD) {
	HistoryHUD** ud = (HistoryHUD**)lua_newuserdata(L, sizeof(HistoryHUD*));
	*ud = &g_Game->GetHUD()->_historyHUD;
	luaL_setmetatable(L, lua::metatables::HistoryHUDMT);
	return 1;
}

LUA_FUNCTION(Lua_HistoryHUD_GetPosition) {
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), g_Game->GetHUD()->_historyHUD.GetPosition());
	return 1;
}

LUA_FUNCTION(Lua_HistoryHUD_GetPlayer) {
	int playerIdx = (int)luaL_checkinteger(L, 2);

	if (playerIdx < 0 || playerIdx > 1) {
		return luaL_error(L, "Invalid HistoryHUDPlayer index %d, must be 0 or 1", playerIdx);
	}

	const HistoryHUD_Player& historyPlayer = g_Game->GetHUD()->_historyHUD._playerHistoryHuds[playerIdx];

	if (historyPlayer._player) {
		lua::luabridge::UserdataPtr::push(L, historyPlayer._player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	} else {
		lua_pushnil(L);
	}

	return 1;
}

LUALIB_API int GetHistoryHUDItems(lua_State* L, const int playerIdx, const bool includeCollectibles, const bool includeTrinkets, const bool includeNotVisible, const bool offsetsOnly, std::optional<std::set<int>> idFilter) {
	if (playerIdx < 0 || playerIdx > 1) {
		return luaL_error(L, "Invalid HistoryHUDPlayer index %d, must be 0 or 1", playerIdx);
	}
	
	const HistoryHUD& historyHUD = g_Game->GetHUD()->_historyHUD;
	const HistoryHUD_Player& hudPlayer = historyHUD._playerHistoryHuds[playerIdx];
	const int numVisibleItems = historyHUD.GetNumVisibleItems();

	lua_newtable(L);

	if (hudPlayer._player != nullptr) {
		int tableIndex = 1;
		for (int i = 0; i < hudPlayer._historyHudItems.size(); i++) {
			if (!includeNotVisible && i >= numVisibleItems) {
				break;
			}

			const HistoryHUD_Item& item = hudPlayer._historyHudItems[i];
			if (item._historyItem._isTrinket && !includeTrinkets) continue;
			if (!item._historyItem._isTrinket && !includeCollectibles) continue;

			int id = item._historyItem._itemID;
			if (item._historyItem._isTrinket) {
				id &= TRINKET_ID_MASK;
			}

			if (!idFilter || idFilter->empty() || idFilter->find(id) != idFilter->end()) {
				if (offsetsOnly) {
					Vector offset = CalculateHistoryHUDItemRenderOffset(playerIdx, i);
					lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), offset);
				} else {
					HistoryHUD_Item_Ex* ud = (HistoryHUD_Item_Ex*)lua_newuserdata(L, sizeof(HistoryHUD_Item_Ex));
					ud->playerSlot = playerIdx;
					ud->index = i;
					ud->hudItem = item;
					luaL_setmetatable(L, lua::metatables::HistoryHUDItemMT);

				}
				lua_rawseti(L, -2, tableIndex);
				tableIndex++;
			}
		}
	}

	return 1;
}

LUA_FUNCTION(Lua_HistoryHUD_GetItems) {
	const int playerIdx = (int)luaL_checkinteger(L, 2);
	const bool includeNotVisible = lua::luaL_optboolean(L, 3, false);
	return GetHistoryHUDItems(L, playerIdx, true, true, includeNotVisible, false, std::nullopt);
}

LUALIB_API int GetFilteredItems(lua_State* L, const bool trinkets, const bool offsetsOnly) {
	const int playerIdx = (int)luaL_checkinteger(L, 2);
	
	std::set<int> filter;

	if (lua_type(L, 3) == LUA_TTABLE) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isinteger(L, -1)) {
				int id = (int)lua_tointeger(L, -1);
				if (trinkets) {
					id &= TRINKET_ID_MASK;
				}
				filter.insert(id);
			}
			lua_pop(L, 1);
		}
	} else if (!lua_isnoneornil(L, 3)) {
		int id = (int)luaL_checkinteger(L, 3);
		if (trinkets) {
			id &= TRINKET_ID_MASK;
		}
		filter.insert(id);
	}

	const bool includeNotVisible = lua::luaL_optboolean(L, 4, false);

	return GetHistoryHUDItems(L, playerIdx, !trinkets, trinkets, includeNotVisible, offsetsOnly, filter);
}
LUA_FUNCTION(Lua_HistoryHUD_GetCollectibles) {
	return GetFilteredItems(L, /*trinkets=*/false, /*offsetsOnly=*/false);
}
LUA_FUNCTION(Lua_HistoryHUD_GetCollectibleOffsets) {
	return GetFilteredItems(L, /*trinkets=*/false, /*offsetsOnly=*/true);
}
LUA_FUNCTION(Lua_HistoryHUD_GetTrinkets) {
	return GetFilteredItems(L, /*trinkets=*/true, /*offsetsOnly=*/false);
}
LUA_FUNCTION(Lua_HistoryHUD_GetTrinketOffsets) {
	return GetFilteredItems(L, /*trinkets=*/true, /*offsetsOnly=*/true);
}

LUA_FUNCTION(Lua_HistoryHUDItem_GetItemID) {
	HistoryHUD_Item_Ex* historyItem = lua::GetRawUserdata<HistoryHUD_Item_Ex*>(L, 1, lua::metatables::HistoryHUDItemMT);
	lua_pushinteger(L, historyItem->hudItem._historyItem._itemID);
	return 1;
}

LUA_FUNCTION(Lua_HistoryHUDItem_IsTrinket) {
	HistoryHUD_Item_Ex* historyItem = lua::GetRawUserdata<HistoryHUD_Item_Ex*>(L, 1, lua::metatables::HistoryHUDItemMT);
	lua_pushboolean(L, historyItem->hudItem._historyItem._isTrinket);
	return 1;
}

LUA_FUNCTION(Lua_HistoryHUDItem_GetTime) {
	HistoryHUD_Item_Ex* historyItem = lua::GetRawUserdata<HistoryHUD_Item_Ex*>(L, 1, lua::metatables::HistoryHUDItemMT);
	lua_pushinteger(L, historyItem->hudItem._historyItem._time);
	return 1;
}

LUA_FUNCTION(Lua_HistoryHUDItem_GetHistoryItem) {
	HistoryHUD_Item_Ex* historyItem = lua::GetRawUserdata<HistoryHUD_Item_Ex*>(L, 1, lua::metatables::HistoryHUDItemMT);
	History_HistoryItem* ud = (History_HistoryItem*)lua_newuserdata(L, sizeof(History_HistoryItem));
	*ud = historyItem->hudItem._historyItem;
	luaL_setmetatable(L, lua::metatables::HistoryItemMT);
	return 1;
}

LUA_FUNCTION(Lua_HistoryHUDItem_GetRenderOffset) {
	HistoryHUD_Item_Ex* historyItem = lua::GetRawUserdata<HistoryHUD_Item_Ex*>(L, 1, lua::metatables::HistoryHUDItemMT);

	Vector offset = CalculateHistoryHUDItemRenderOffset(historyItem->playerSlot, historyItem->index);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), offset);

	return 1;
}

LUA_FUNCTION(Lua_HistoryHUDItem_IsVisible) {
	HistoryHUD_Item_Ex* historyItem = lua::GetRawUserdata<HistoryHUD_Item_Ex*>(L, 1, lua::metatables::HistoryHUDItemMT);
	lua_pushboolean(L, historyItem->index < g_Game->GetHUD()->_historyHUD.GetNumVisibleItems());
	return 1;
}

static void RegisterHistoryHUD(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::HUD, "GetHistoryHUD", Lua_HUD_GetHistoryHUD);

	luaL_Reg functions[] = {
		{ "GetPlayer", Lua_HistoryHUD_GetPlayer },
		{ "GetPosition", Lua_HistoryHUD_GetPosition },
		{ "GetItems", Lua_HistoryHUD_GetItems },
		{ "GetCollectibles", Lua_HistoryHUD_GetCollectibles },
		{ "GetCollectibleOffsets", Lua_HistoryHUD_GetCollectibleOffsets },
		{ "GetTrinkets", Lua_HistoryHUD_GetTrinkets },
		{ "GetTrinketOffsets", Lua_HistoryHUD_GetTrinketOffsets },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::HistoryHUDMT, lua::metatables::HistoryHUDMT, functions);
}

static void RegisterHistoryHUDItem(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetItemID", Lua_HistoryHUDItem_GetItemID },
		{ "IsTrinket", Lua_HistoryHUDItem_IsTrinket },
		{ "GetTime", Lua_HistoryHUDItem_GetTime },
		{ "GetHistoryItem", Lua_HistoryHUDItem_GetHistoryItem },
		{ "GetRenderOffset", Lua_HistoryHUDItem_GetRenderOffset },
		{ "IsVisible", Lua_HistoryHUDItem_IsVisible },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::HistoryHUDItemMT, lua::metatables::HistoryHUDItemMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterHistoryHUD(_state);
	RegisterHistoryHUDItem(_state);
}
