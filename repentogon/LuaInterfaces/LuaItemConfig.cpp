#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../Patches/XMLData.h"

XMLDataHolder* GetItemXML(const ItemConfig_Item* config) {
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

	XMLDataHolder* xml = GetItemXML(config);
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

void RegisterItemFunctions(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetCustomTags", Lua_ItemConfigItem_GetCustomTags },
		{ "HasCustomTag", Lua_ItemConfigItem_HasCustomTag },
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

LUA_FUNCTION(Lua_ItemConfig_IsValidTrinket) {
	ItemConfig* config = lua::GetUserdata<ItemConfig*>(L, 1, lua::Metatables::CONFIG, "Config");
	const unsigned int trinketType = (const unsigned int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, ItemConfig::IsValidTrinket(trinketType));

	return 1;
}

static void FixItemConfigPillEffects(lua_State* L) {
	lua::RegisterVariableGetter(L, lua::Metatables::PILL_EFFECT, "EffectClass", Lua_ItemConfigPill_EffectClass_propget);
	lua::RegisterVariableGetter(L, lua::Metatables::PILL_EFFECT, "EffectSubClass", Lua_ItemConfigPill_EffectSubClass_propget);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_PILL_EFFECT, "EffectClass", Lua_ItemConfigPill_EffectClass_propget);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_PILL_EFFECT, "EffectSubClass", Lua_ItemConfigPill_EffectSubClass_propget);
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
	FixItemConfigPillEffects(_state);
	lua::RegisterFunctions(_state, lua::Metatables::CONFIG, functions);
	lua::RegisterFunctions(_state, lua::Metatables::CONST_CONFIG, functions);
	//lua::RegisterGlobalClassFunction(_state, "Config", "IsValidTrinket", Lua_ItemConfig_IsValidTrinket);
}
