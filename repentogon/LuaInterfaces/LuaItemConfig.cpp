#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../LuaClasses.h"
#include "../Patches/XMLData.h"
#include "../Patches/CardsExtras.h"
#include "../Patches/ItemConfigEx.h"

XMLItem* GetItemXML(const ItemConfig_Item* config) {
	if (config->type == 0) {
		return XMLStuff.NullItemData;
	}
	else if (config->type == 2) {
		return XMLStuff.TrinketData;
	}
	return XMLStuff.ItemData;
}

LUA_FUNCTION(Lua_ItemConfigPill_EffectClass_propget) {
	ItemConfig_Pill* config = lua::GetLuabridgeUserdata<ItemConfig_Pill*>(L, 1, lua::Metatables::CONST_PILL_EFFECT, "PillEffect");
	lua_pushinteger(L, config->effectClass);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCard_ModdedCardFront_propget) {
	ItemConfig_Card* config = lua::GetLuabridgeUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	LuaSprite::PushPtr(L, config->moddedCardFront);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCard_GetInitialWeight) {
	ItemConfig_Card* config = lua::GetLuabridgeUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = CardsEX::GetCardConfigEX(config);
	lua_pushnumber(L, config_EX->initialWeight);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCard_GetWeight) {
	ItemConfig_Card* config = lua::GetLuabridgeUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = CardsEX::GetCardConfigEX(config);
	lua_pushnumber(L, config_EX->weight);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCard_SetWeight) {
	ItemConfig_Card* config = lua::GetLuabridgeUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	float weight = (float)luaL_checknumber(L, 2);

	weight = max(weight, 0.0f);
	ItemConfig_Card_EX* config_EX = CardsEX::GetCardConfigEX(config);
	config_EX->weight = weight;
	config_EX->invalidateVanillaMethod = weight != 1.0f;

	return 0;
}

LUA_FUNCTION(Lua_ItemConfigCard_GetAvailabilityCondition) {
	ItemConfig_Card* config = lua::GetLuabridgeUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = CardsEX::GetCardConfigEX(config);

	lua_rawgeti(L, LUA_REGISTRYINDEX, config_EX->availabilityFuncRef);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCard_SetAvailabilityCondition) {
	ItemConfig_Card* config = lua::GetLuabridgeUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = CardsEX::GetCardConfigEX(config);

	config_EX->SetAvailabilityCondition(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_ItemConfigCard_ClearAvailabilityCondition) {
	ItemConfig_Card* config = lua::GetLuabridgeUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = CardsEX::GetCardConfigEX(config);

	config_EX->ClearAvailabilityCondition(L);
	return 0;
}

LUA_FUNCTION(Lua_ItemConfigCard_GetHidden) {
	ItemConfig_Card* config = lua::GetLuabridgeUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = CardsEX::GetCardConfigEX(config);

	lua_pushboolean(L, config_EX->hidden);
	return 1;
}

template <bool Const>
static inline void RegisterCardFields(lua_State* L) {
	constexpr auto mt = Const ? lua::Metatables::CONST_CARD : lua::Metatables::CARD;

	lua::RegisterVariableGetter(L, mt, "ModdedCardFront", Lua_ItemConfigCard_ModdedCardFront_propget);
	lua::RegisterVariableGetter(L, mt, "Hidden", Lua_ItemConfigCard_GetHidden);
	lua::RegisterVariableGetter(L, mt, "InitialWeight", Lua_ItemConfigCard_GetInitialWeight);
	lua::RegisterVariableGetter(L, mt, "Weight", Lua_ItemConfigCard_GetWeight);

	if constexpr (!Const)
	{
		lua::RegisterVariableSetter(L, mt, "Weight", Lua_ItemConfigCard_SetWeight);
	}

	lua::RegisterFunction(L, mt, "GetAvailabilityCondition", Lua_ItemConfigCard_GetAvailabilityCondition);

	if constexpr (!Const)
	{
		lua::RegisterFunction(L, mt, "SetAvailabilityCondition", Lua_ItemConfigCard_SetAvailabilityCondition);
		lua::RegisterFunction(L, mt, "ClearAvailabilityCondition", Lua_ItemConfigCard_ClearAvailabilityCondition);
	}
}

LUA_FUNCTION(Lua_ItemConfigPill_EffectSubClass_propget) {
	ItemConfig_Pill* config = lua::GetLuabridgeUserdata<ItemConfig_Pill*>(L, 1, lua::Metatables::CONST_PILL_EFFECT, "PillEffect");
	lua_pushinteger(L, config->effectSubClass);
	return 1;
}

// Legacy compat for a player function in ItemConfig
LUA_FUNCTION(Lua_ItemConfig_CanRerollCollectible) {
	int id = (int)luaL_checkinteger(L, 1);

	if (!g_Game->_playerManager._playerList.empty() && g_Game->GetPlayer(0) && g_Game->GetPlayer(0)->_exists) {
		lua_pushboolean(L, g_Game->GetPlayer(0)->CanRerollCollectible(id, false));
	} else {
		lua_pushboolean(L, !g_Manager->GetItemConfig()->IsQuestItem(id));
	}

	return 1;
}

LUA_FUNCTION(Lua_ItemConfig_GetTaggedItems) {
	ItemConfig* config = lua::GetLuabridgeUserdata<ItemConfig*>(L, 1, lua::Metatables::CONFIG, "ItemConfig");
	unsigned int tags = (unsigned int)luaL_checkinteger(L, 2);

	std::vector<ItemConfig_Item*>& itemPtrs = config->GetTaggedItems(tags);

	lua_newtable(L);
	for (size_t i = 0; i < itemPtrs.size(); ++i) {
		lua_pushinteger(L, i + 1);
		lua::ffi::pushCdataPtr(L, itemPtrs[i], lua::ffi::CData[lua::ffi::CDataID::ITEM_PTR]);
		lua_rawset(L, -3);
	}

	return 1;
}

/*LUA_FUNCTION(Lua_ItemConfig_IsValidTrinket) {
	ItemConfig* config = lua::GetRawUserdata<ItemConfig*>(L, 1, lua::Metatables::CONFIG, "Config");
	const unsigned int trinketType = (const unsigned int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, ItemConfig::IsValidTrinket(trinketType));

	return 1;
}*/

static void FixItemConfigPillEffects(lua_State* L) {
	lua::RegisterVariableGetter(L, lua::Metatables::PILL_EFFECT, "EffectClass", Lua_ItemConfigPill_EffectClass_propget);
	lua::RegisterVariableGetter(L, lua::Metatables::PILL_EFFECT, "EffectSubClass", Lua_ItemConfigPill_EffectSubClass_propget);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_PILL_EFFECT, "EffectClass", Lua_ItemConfigPill_EffectClass_propget);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_PILL_EFFECT, "EffectSubClass", Lua_ItemConfigPill_EffectSubClass_propget);
}


LUA_FUNCTION(Lua_ItemConfig_GetCollectible) {
	ItemConfig* config = lua::GetLuabridgeUserdata<ItemConfig*>(L, 1, lua::Metatables::CONFIG, "ItemConfig");
	int id = (int)luaL_checkinteger(L, 2);

	lua::ffi::pushCdataPtr(L, config->GetCollectible(id), lua::ffi::CData[lua::ffi::CDataID::ITEM_PTR]);

	return 1;
}

LUA_FUNCTION(Lua_ItemConfig_GetNullItem) {
	ItemConfig* config = lua::GetLuabridgeUserdata<ItemConfig*>(L, 1, lua::Metatables::CONFIG, "ItemConfig");
	int id = (int)luaL_checkinteger(L, 2);

	lua::ffi::pushCdataPtr(L, config->GetNullItem(id), lua::ffi::CData[lua::ffi::CDataID::ITEM_PTR]);

	return 1;
}

LUA_FUNCTION(Lua_ItemConfig_GetTrinket) {
	ItemConfig* config = lua::GetLuabridgeUserdata<ItemConfig*>(L, 1, lua::Metatables::CONFIG, "ItemConfig");
	int id = (int)luaL_checkinteger(L, 2);

	lua::ffi::pushCdataPtr(L, config->GetTrinket(id), lua::ffi::CData[lua::ffi::CDataID::ITEM_PTR]);

	return 1;
}

LUA_FUNCTION(Lua_ItemConfig_ShouldAddCostumeOnPickup) {
	ItemConfig_Item* config = lua::GetCData<ItemConfig_Item*>(L, 1, lua::ffi::CData[lua::ffi::CDataID::ITEM], "ItemConfigItem");

	lua_pushboolean(L, config->type != 0 && config->addCostumeOnPickup);
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetTaggedItems", Lua_ItemConfig_GetTaggedItems },
		{ "CanRerollCollectible", Lua_ItemConfig_CanRerollCollectible },
		{ "GetCollectible", Lua_ItemConfig_GetCollectible },
		{ "GetNullItem", Lua_ItemConfig_GetNullItem },
		{ "GetTrinket", Lua_ItemConfig_GetTrinket },
		//{ "IsValidTrinket", Lua_ItemConfig_IsValidTrinket },
		{ NULL, NULL }
	};

	RegisterCardFields<false>(_state);
	RegisterCardFields<true>(_state);

	FixItemConfigPillEffects(_state);

	lua::RegisterFunctions(_state, lua::Metatables::CONFIG, functions);
	lua::RegisterFunctions(_state, lua::Metatables::CONST_CONFIG, functions);

	lua_getglobal(_state, "ItemConfig");
	lua_getfield(_state, -1, "Config");
	lua_pushstring(_state, "ShouldAddCostumeOnPickup");
	lua_pushcfunction(_state, Lua_ItemConfig_ShouldAddCostumeOnPickup);
	lua_rawset(_state, -3);
	lua_pop(_state, 2);

	//lua::RegisterGlobalClassFunction(_state, "Config", "IsValidTrinket", Lua_ItemConfig_IsValidTrinket);
}


