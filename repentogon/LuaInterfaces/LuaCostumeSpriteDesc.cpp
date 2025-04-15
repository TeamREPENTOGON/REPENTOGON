#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PlayerGetCostumeSpriteDescs)
{
	Entity_Player* player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	std::vector<CostumeSpriteDesc>& spriteDescs = player->_costumeSpriteDescs;

	lua_newtable(L);
	int idx = 1;
	for (auto& spriteDesc : spriteDescs) {
		CostumeSpriteDesc** toLua = (CostumeSpriteDesc**)lua_newuserdata(L, sizeof(CostumeSpriteDesc*));
		*toLua = &spriteDesc;
		luaL_setmetatable(L, lua::metatables::CostumeSpriteDescMT);
		lua_rawseti(L, -2, idx);
		idx++;
	}
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_GetSprite) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua::luabridge::UserdataPtr::push(L, &costumeSprDesc->_sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_GetItemConfig) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	ItemConfig_Item* config = costumeSprDesc->_item;
	lua::luabridge::UserdataPtr::push(L, config, lua::GetMetatableKey(lua::Metatables::ITEM));
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_GetPriority) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua_pushinteger(L, costumeSprDesc->_priority);
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_IsItemAnimPlaying) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua_pushboolean(L, costumeSprDesc->_itemAnimPlay);
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_IsFlying) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua_pushboolean(L, costumeSprDesc->_isFlying);
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_HasOverlay) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua_pushboolean(L, costumeSprDesc->_hasOverlay);
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_HasSkinAlt) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua_pushboolean(L, costumeSprDesc->_hasSkinAlt);
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_GetHeadColor) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua_pushinteger(L, costumeSprDesc->_headColor);
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_GetBodyColor) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua_pushinteger(L, costumeSprDesc->_bodyColor);
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_CanOverwriteColor) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua_pushboolean(L, costumeSprDesc->_overwriteColor);
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_IsItemStateOnly) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua_pushboolean(L, costumeSprDesc->_itemStateOnly);
	return 1;
}

LUA_FUNCTION(CostumeSprDesc_GetPlayerType) {
	CostumeSpriteDesc* costumeSprDesc = *lua::GetRawUserdata<CostumeSpriteDesc**>(L, 1, lua::metatables::CostumeSpriteDescMT);
	lua_pushinteger(L, costumeSprDesc->_playerType);
	return 1;
}

static void RegisterCostumeSpriteDesc(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY_PLAYER, "GetCostumeSpriteDescs", Lua_PlayerGetCostumeSpriteDescs);

	luaL_Reg functions[] = {
		{ "GetSprite", CostumeSprDesc_GetSprite },
		{ "GetItemConfig", CostumeSprDesc_GetItemConfig },
		{ "GetPriority", CostumeSprDesc_GetPriority },
		{ "IsItemAnimPlaying", CostumeSprDesc_IsItemAnimPlaying },
		{ "IsFlying", CostumeSprDesc_IsFlying },
		{ "HasOverlay", CostumeSprDesc_HasOverlay },
		{ "HasSkinAlt", CostumeSprDesc_HasSkinAlt },
		{ "GetHeadColor", CostumeSprDesc_GetHeadColor },
		{ "GetBodyColor", CostumeSprDesc_GetBodyColor },
		{ "CanOverwriteColor", CostumeSprDesc_CanOverwriteColor },
		{ "IsItemStateOnly", CostumeSprDesc_IsItemStateOnly },
		{ "GetPlayerType", CostumeSprDesc_GetPlayerType },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::CostumeSpriteDescMT, lua::metatables::CostumeSpriteDescMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterCostumeSpriteDesc(_state);
}