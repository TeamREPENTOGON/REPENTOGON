#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../Patches/XMLData.h"
#include "../Patches/CardsExtras.h"

XMLItem* GetItemXML(const ItemConfig_Item* config) {
	if (config->type == 0) {
		return XMLStuff.NullItemData;
	}
	else if (config->type == 2) {
		return XMLStuff.TrinketData;
	}
	return XMLStuff.ItemData;
}

LUA_FUNCTION(Lua_ItemConfigItem_GetCustomTags) {
	ItemConfig_Item* config = lua::GetUserdata<ItemConfig_Item*>(L, 1, lua::Metatables::ITEM, "Item");

	lua_newtable(L);

	XMLItem* xml = GetItemXML(config);
	if (xml->customtags.find(config->id) != xml->customtags.end()) {
		int i = 0;
		for (const std::string& tag : xml->customtags[config->id]) {
			lua_pushinteger(L, ++i);
			lua_pushstring(L, tag.c_str());
			lua_settable(L, -3);
		}
	}

	return 1;
}

LUA_FUNCTION(Lua_ItemConfigItem_HasCustomTag)
{
	ItemConfig_Item* config = lua::GetUserdata<ItemConfig_Item*>(L, 1, lua::Metatables::ITEM, "Item");
	const std::string tag = luaL_checkstring(L, 2);
	lua_pushboolean(L, GetItemXML(config)->HasCustomTag(config->id, tag));
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigItem_GetCustomCacheTags) {
	ItemConfig_Item* config = lua::GetUserdata<ItemConfig_Item*>(L, 1, lua::Metatables::ITEM, "Item");

	lua_newtable(L);

	XMLItem* xml = GetItemXML(config);
	if (xml->customcache.find(config->id) != xml->customcache.end()) {
		int i = 0;
		for (const std::string& tag : xml->customcache[config->id]) {
			lua_pushinteger(L, ++i);
			lua_pushstring(L, tag.c_str());
			lua_settable(L, -3);
		}
	}

	return 1;
}

LUA_FUNCTION(Lua_ItemConfigItem_HasCustomCacheTag)
{
	ItemConfig_Item* config = lua::GetUserdata<ItemConfig_Item*>(L, 1, lua::Metatables::ITEM, "Item");
	const std::string tag = luaL_checkstring(L, 2);
	lua_pushboolean(L, GetItemXML(config)->HasCustomCache(config->id, tag));
	return 1;
}

void RegisterItemFunctions(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetCustomTags", Lua_ItemConfigItem_GetCustomTags },
		{ "HasCustomTag", Lua_ItemConfigItem_HasCustomTag },
		{ "GetCustomCacheTags", Lua_ItemConfigItem_GetCustomCacheTags },
		{ "HasCustomCacheTag", Lua_ItemConfigItem_HasCustomCacheTag },
		{ NULL, NULL }
	};

	lua::RegisterFunctions(L, lua::Metatables::ITEM, functions);
	lua::RegisterFunctions(L, lua::Metatables::CONST_ITEM, functions);
}

LUA_FUNCTION(Lua_ItemConfigPill_EffectClass_propget) {
	ItemConfig_Pill* config = lua::GetUserdata<ItemConfig_Pill*>(L, 1, lua::Metatables::CONST_PILL_EFFECT, "PillEffect");
	lua_pushinteger(L, config->effectClass);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCard_ModdedCardFront_propget) {
	ItemConfig_Card* config = lua::GetUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	lua::luabridge::UserdataPtr::push(L, config->moddedCardFront, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCard_GetInitialWeight) {
	ItemConfig_Card* config = lua::GetUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = Cards_EX::GetCardConfigEX(config);
	lua_pushnumber(L, config_EX->initialWeight);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCard_GetWeight) {
	ItemConfig_Card* config = lua::GetUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = Cards_EX::GetCardConfigEX(config);
	lua_pushnumber(L, config_EX->weight);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCard_SetWeight) {
	ItemConfig_Card* config = lua::GetUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	float weight = (float)luaL_checknumber(L, 2);

	weight = max(weight, 0.0f);
	ItemConfig_Card_EX* config_EX = Cards_EX::GetCardConfigEX(config);
	config_EX->weight = weight;
	config_EX->invalidateVanillaMethod = weight != 1.0f;

	return 0;
}

LUA_FUNCTION(Lua_ItemConfigCard_GetAvailabilityCondition) {
	ItemConfig_Card* config = lua::GetUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = Cards_EX::GetCardConfigEX(config);

	lua_rawgeti(L, LUA_REGISTRYINDEX, config_EX->availabilityFuncRef);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCard_SetAvailabilityCondition) {
	ItemConfig_Card* config = lua::GetUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = Cards_EX::GetCardConfigEX(config);

	config_EX->SetAvailabilityCondition(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_ItemConfigCard_ClearAvailabilityCondition) {
	ItemConfig_Card* config = lua::GetUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = Cards_EX::GetCardConfigEX(config);

	config_EX->ClearAvailabilityCondition(L);
	return 0;
}

LUA_FUNCTION(Lua_ItemConfigCard_GetHidden) {
	ItemConfig_Card* config = lua::GetUserdata<ItemConfig_Card*>(L, 1, lua::Metatables::CONST_CARD, "Card");
	ItemConfig_Card_EX* config_EX = Cards_EX::GetCardConfigEX(config);

	lua_pushboolean(L, config_EX->hidden);
	return 1;
}

void RegisterCardFunctions(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetAvailabilityCondition", Lua_ItemConfigCard_GetAvailabilityCondition },
		{ "SetAvailabilityCondition", Lua_ItemConfigCard_SetAvailabilityCondition },
		{ "ClearAvailabilityCondition", Lua_ItemConfigCard_ClearAvailabilityCondition },
		{ NULL, NULL }
	};

	lua::RegisterFunctions(L, lua::Metatables::CARD, functions);
	lua::RegisterFunctions(L, lua::Metatables::CONST_CARD, functions);
}

LUA_FUNCTION(Lua_ItemConfigPill_EffectSubClass_propget) {
	ItemConfig_Pill* config = lua::GetUserdata<ItemConfig_Pill*>(L, 1, lua::Metatables::CONST_PILL_EFFECT, "PillEffect");
	lua_pushinteger(L, config->effectSubClass);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfig_CanRerollCollectible) {
	int id = (int)luaL_checkinteger(L, 1);
	lua_pushboolean(L, CanRerollCollectible(id));
	return 1;
}

LUA_FUNCTION(Lua_ItemConfig_GetTaggedItems) {
	ItemConfig* config = lua::GetUserdata<ItemConfig*>(L, 1, lua::Metatables::CONFIG, "ItemConfig");
	unsigned int tags = (unsigned int)luaL_checkinteger(L, 2);

	std::vector<ItemConfig_Item*>& itemPtrs = config->GetTaggedItems(tags);

	lua_newtable(L);
	for (size_t i = 0; i < itemPtrs.size(); ++i) {
		lua_pushinteger(L, i + 1);
		lua::luabridge::UserdataPtr::push(L, itemPtrs[i], lua::GetMetatableKey(lua::Metatables::ITEM));
		lua_rawset(L, -3);
	}

	return 1;
}

/*LUA_FUNCTION(Lua_ItemConfig_IsValidTrinket) {
	ItemConfig* config = lua::GetUserdata<ItemConfig*>(L, 1, lua::Metatables::CONFIG, "Config");
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

LUA_FUNCTION(Lua_ItemConfigCostume_GetSkinColor) {
	ItemConfig_Costume* costume = lua::GetUserdata<ItemConfig_Costume*>(L, 1, lua::Metatables::CONST_COSTUME, "Costume");
	lua_pushinteger(L, costume->skinColor);
	return 1;
}

LUA_FUNCTION(Lua_ItemConfigCostume_SetSkinColor) {
	ItemConfig_Costume* costume = lua::GetUserdata<ItemConfig_Costume*>(L, 1, lua::Metatables::CONST_COSTUME, "Costume");
	costume->skinColor = (int)luaL_checkinteger(L, 2);
	return 1;
}


HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	RegisterItemFunctions(_state);

	luaL_Reg functions[] = {
		{ "GetTaggedItems", Lua_ItemConfig_GetTaggedItems },
		{ "CanRerollCollectible", Lua_ItemConfig_CanRerollCollectible },
		//{ "IsValidTrinket", Lua_ItemConfig_IsValidTrinket },
		{ NULL, NULL }
	};

	lua::RegisterVariableGetter(_state, lua::Metatables::CARD, "ModdedCardFront",Lua_ItemConfigCard_ModdedCardFront_propget);
	lua::RegisterVariableGetter(_state, lua::Metatables::CARD, "Hidden", Lua_ItemConfigCard_GetHidden);
	lua::RegisterVariableGetter(_state, lua::Metatables::CARD, "InitialWeight", Lua_ItemConfigCard_GetInitialWeight);
	lua::RegisterVariable(_state, lua::Metatables::CARD, "Weight", Lua_ItemConfigCard_GetWeight, Lua_ItemConfigCard_SetWeight);
	lua::RegisterVariableGetter(_state, lua::Metatables::CONST_CARD, "Hidden", Lua_ItemConfigCard_GetHidden);
	lua::RegisterVariableGetter(_state, lua::Metatables::CONST_CARD, "InitialWeight", Lua_ItemConfigCard_GetInitialWeight);
	RegisterCardFunctions(_state);

	FixItemConfigPillEffects(_state);
	lua::RegisterFunctions(_state, lua::Metatables::CONFIG, functions);
	lua::RegisterFunctions(_state, lua::Metatables::CONST_CONFIG, functions);
	//lua::RegisterGlobalClassFunction(_state, "Config", "IsValidTrinket", Lua_ItemConfig_IsValidTrinket);

	lua::RegisterVariable(_state, lua::Metatables::COSTUME, "SkinColor", Lua_ItemConfigCostume_GetSkinColor, Lua_ItemConfigCostume_SetSkinColor);
	lua::RegisterVariableGetter(_state, lua::Metatables::CONST_COSTUME, "SkinColor", Lua_ItemConfigCostume_GetSkinColor);
}
