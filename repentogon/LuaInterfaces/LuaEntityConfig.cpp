#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EntityGetEntityConfigEntity)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");

	EntityConfig_Entity* entityConfigEntity = g_Manager->GetEntityConfig()->GetEntity(*entity->GetType(), *entity->GetVariant(), *entity->GetSubType());

	if (entityConfigEntity == nullptr) {
		// how
		lua_pushnil(L);
	}
	else {
		EntityConfig_Entity** toLua = (EntityConfig_Entity**)lua_newuserdata(L, sizeof(EntityConfig_Entity*));
		*toLua = entityConfigEntity;
		luaL_setmetatable(L, lua::metatables::EntityConfigEntityMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityConfigGetEntity)
{
	const int type = (int)luaL_checkinteger(L, 1);
	const int variant = (int)luaL_optinteger(L, 2, -1);
	const int subtype = (int)luaL_optinteger(L, 3, -1);

	EntityConfig_Entity* entity = g_Manager->GetEntityConfig()->GetEntity(type, variant, subtype);

	if (entity == nullptr) {
		lua_pushnil(L);
	}
	else {
		EntityConfig_Entity** toLua = (EntityConfig_Entity**)lua_newuserdata(L, sizeof(EntityConfig_Entity*));
		*toLua = entity;
		luaL_setmetatable(L, lua::metatables::EntityConfigEntityMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetEntityConfigPlayer)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");

	EntityConfig_Player* playerConfig = g_Manager->GetEntityConfig()->GetPlayer(player->GetPlayerType());

	if (playerConfig == nullptr) {
		lua_pushnil(L);
	}
	else {
		EntityConfig_Player** toLua = (EntityConfig_Player**)lua_newuserdata(L, sizeof(EntityConfig_Player*));
		*toLua = playerConfig;
		luaL_setmetatable(L, lua::metatables::EntityConfigPlayerMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityConfigGetPlayer)
{
	const int playerType = (int)luaL_checkinteger(L, 1);

	EntityConfig_Player* playerConfig = g_Manager->GetEntityConfig()->GetPlayer(playerType);

	if (playerConfig == nullptr) {
		lua_pushnil(L);
	}
	else {
		EntityConfig_Player** toLua = (EntityConfig_Player**)lua_newuserdata(L, sizeof(EntityConfig_Player*));
		*toLua = playerConfig;
		luaL_setmetatable(L, lua::metatables::EntityConfigPlayerMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityConfigGetMaxPlayerType)
{
	lua_pushinteger(L, g_Manager->GetEntityConfig()->GetPlayers()->size()-1);
	return 1;
}

/*
* EntityConfigEntity Functions
*/

LUA_FUNCTION(Lua_EntityConfigEntityGetType)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->id);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetVariant)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->variant);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetSubType)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->subtype);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetName)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionDamage)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->collisionDamage);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBossId)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->bossID);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionRadius)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->collisionRadius);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionRadiusMultiplier)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), entity->collisionRadiusMulti);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetMass)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->mass);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetGridCollisionPoints)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->gridCollisionPoints);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetFriction)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->friction);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBaseHP)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->baseHP);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetStageHP)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->stageHP);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetAnm2Path)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->anm2Path.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetModName)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	if (entity->modEntry == nullptr) {
		lua_pushnil(L);
	}
	else {
		lua_pushstring(L, entity->modEntry->GetName().c_str());
	}
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetEntityTags)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->tags);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityHasEntityTags)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	const int tags = (int)luaL_checkinteger(L, 2);
	if (tags <= 0) {
		lua_pushboolean(L, false);
	}
	else {
		lua_pushboolean(L, (tags & entity->tags) == tags);
	}
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetShadowSize)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->shadowSize);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityIsBoss)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushboolean(L, entity->isBoss);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityCanBeChampion)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushboolean(L, entity->canBeChampion);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityCanShutDoors)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushboolean(L, entity->shutDoors);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetGibsAmount)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->gibsAmount);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetGibFlags)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->gibFlags);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityHasGibFlags)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	const int flags = (int)luaL_checkinteger(L, 2);
	if (flags <= 0) {
		lua_pushboolean(L, false);
	}
	else {
		lua_pushboolean(L, (flags & entity->gibFlags) == flags);
	}
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetPortraitID)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->portrait);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityCanBeRerolledInto)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushboolean(L, entity->reroll);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityHasFloorAlts)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushboolean(L, entity->hasFloorAlts);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionInterval)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->collisionInterval);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetShieldStrength)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->shieldStrength);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryOffset)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), entity->bestiaryOffset);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryScale)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->bestiaryScale);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryAnm2Path)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->bestiaryAnm2Path.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryAnim)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->bestiaryAnim.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryOverlay)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->bestiaryOverlay.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryFloorAlt)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->bestiaryFloorAlt.c_str());
	return 1;
}

/*
* EntityConfigPlayer Functions
*/

LUA_FUNCTION(Lua_EntityConfigPlayerGetPlayerType)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_id);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetName)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetSkinPath)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_skinPath.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetNameImagePath)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_nameImagePath.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetPortraitPath)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_portraitPath.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetExtraPortraitPath)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_extraPortraitPath.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetCostumeSuffix)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_costumeSuffixName.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetCostumeID)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_costumeID);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetSkinColor)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_skinColor);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetRedHearts)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_heart);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetSoulHearts)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_armor);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetBlackHearts)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_blackHeart);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetBrokenHearts)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_brokenHeart);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetCoins)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_coins);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetBombs)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_bombs);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetKeys)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_keys);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetCard)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_card);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetPill)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_pill);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetTrinket)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_trinket);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetCollectibles)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);

	lua_newtable(L);
	for (unsigned int i = 0; i < player->_collectibles.size(); ++i) {
		lua_pushinteger(L, i + 1);
		lua_pushinteger(L, player->_collectibles[i]);
		lua_settable(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetPocketActive)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_pocketActiveID);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerCanShoot)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushboolean(L, player->_canShoot);
	return 1;
}

const std::set<int> HiddenVanillaCharacters = { 11, 12, 17, 20, 38, 39, 40 };

LUA_FUNCTION(Lua_EntityConfigPlayerIsHidden)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);

	if (player->_hidden || HiddenVanillaCharacters.find(player->_id) != HiddenVanillaCharacters.end()) {
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, false);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetAchievement)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_achievement);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetBirthrightDescription)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_birthrightDescription.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetModdedMenuBackgroundSprite)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	if (player->_moddedMenuBackgroundANM2 == nullptr) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player->_moddedMenuBackgroundANM2, lua::GetMetatableKey(lua::Metatables::SPRITE));
	}
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetModdedPortraitSprite)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	if (player->_moddedMenuPortraitANM2 == nullptr) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player->_moddedMenuPortraitANM2, lua::GetMetatableKey(lua::Metatables::SPRITE));
	}
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetModdedGameOverSprite)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	if (player->_moddedGameOverANM2 == nullptr) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player->_moddedGameOverANM2, lua::GetMetatableKey(lua::Metatables::SPRITE));
	}
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetModdedCoopMenuSprite)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	if (player->_moddedCoopMenuANM2 == nullptr) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player->_moddedCoopMenuANM2, lua::GetMetatableKey(lua::Metatables::SPRITE));
	}
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetModdedControlsSprite)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	if (player->_moddedControlsANM2 == nullptr) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player->_moddedControlsANM2, lua::GetMetatableKey(lua::Metatables::SPRITE));
	}
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerIsTainted)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushboolean(L, (player->_id >= 21 && player->_id <= 40) || !player->_bSkinParentName.empty());
	return 1;
}

std::unordered_map<int, int> TaintedMap = {
	{0, 21},  // Isaac
	{1, 22},  // Maggy
	{2, 23},  // Cain
	{3, 24},  // Judas
	{4, 25},  // BlueBaby
	{5, 26},  // Eve
	{6, 27},  // Samson
	{7, 28},  // Azazel
	{8, 29},  // Lazarus
	{9, 30},  // Eden
	{10, 31}, // Lost
	{11, 38}, // Lazarus 2
	{12, 24}, // Dark Judas
	{13, 32}, // Lilith
	{14, 33}, // Keeper
	{15, 34}, // Apollyon
	{16, 35}, // Forgotten
	{17, 40}, // Soul
	{18, 36}, // Bethany
	{19, 37}, // Jacob
	{20, 37}, // Esau
	{21, 0},  // IsaacB
	{22, 1},  // MaggyB
	{23, 2},  // CainB
	{24, 3},  // JudasB
	{25, 4},  // BlueBabyB
	{26, 5},  // EveB
	{27, 6},  // SamsonB
	{28, 7},  // AzazelB
	{29, 8},  // LazarusB
	{30, 9},  // EdenB
	{31, 10}, // LostB
	{32, 13}, // LilithB
	{33, 14}, // KeeperB
	{34, 15}, // ApollyonB
	{35, 16}, // ForgottenB
	{36, 18}, // BethanyB
	{37, 19}, // JacobB
	{38, 11}, // Lazarus2B
	{39, 19}, // Jacob2B
	{40, 17}, // SoulB
};

LUA_FUNCTION(Lua_EntityConfigPlayerGetTaintedCounterpart)
{
	EntityConfig_Player* player = *lua::GetUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);

	int counterpartID = -1;

	if (TaintedMap.find(player->_id) != TaintedMap.end()) {
		counterpartID = TaintedMap[player->_id];
	}
	else if (!player->_bSkinParentName.empty()) {
		// Modded Tainted
		for (int i = 41; i < g_Manager->GetEntityConfig()->GetPlayers()->size(); i++) {
			EntityConfig_Player* otherPlayer = g_Manager->GetEntityConfig()->GetPlayer(i);
			if (otherPlayer->_id != player->_id && otherPlayer->_name == player->_bSkinParentName && otherPlayer->_bSkinParentName.empty()) {
				counterpartID = otherPlayer->_id;
				break;
			}
		}
		// Cache it
		TaintedMap[player->_id] = counterpartID;
	}
	else if (player->_moddedTaintedPlayerID > 40 && player->_moddedTaintedPlayerID != player->_id) {
		// Modded Non-Tainted
		counterpartID = player->_moddedTaintedPlayerID;
	}

	if (counterpartID < 0 || counterpartID >= g_Manager->GetEntityConfig()->GetPlayers()->size()) {
		lua_pushnil(L);
	}
	else {
		EntityConfig_Player** toLua = (EntityConfig_Player**)lua_newuserdata(L, sizeof(EntityConfig_Player*));
		*toLua = g_Manager->GetEntityConfig()->GetPlayer(counterpartID);
		luaL_setmetatable(L, lua::metatables::EntityConfigPlayerMT);
	}

	return 1;
}

/*
* Registration Stuff
*/

static void RegisterEntityConfig(lua_State* L) {
	lua_newtable(L);
	lua::TableAssoc(L, "GetEntity", Lua_EntityConfigGetEntity);
	lua::TableAssoc(L, "GetPlayer", Lua_EntityConfigGetPlayer);
	lua::TableAssoc(L, "GetMaxPlayerType", Lua_EntityConfigGetMaxPlayerType);
	lua_setglobal(L, lua::metatables::EntityConfigMT);
}

static void RegisterEntityConfigEntity(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetType", Lua_EntityConfigEntityGetType },
		{ "GetVariant", Lua_EntityConfigEntityGetVariant },
		{ "GetSubType", Lua_EntityConfigEntityGetSubType },
		{ "GetName", Lua_EntityConfigEntityGetName },
		{ "GetCollisionDamage", Lua_EntityConfigEntityGetCollisionDamage },
		{ "GetBossID", Lua_EntityConfigEntityGetBossId },
		{ "GetCollisionRadius", Lua_EntityConfigEntityGetCollisionRadius },
		{ "GetCollisionRadiusMultiplier", Lua_EntityConfigEntityGetCollisionRadiusMultiplier },
		{ "GetMass", Lua_EntityConfigEntityGetMass },
		{ "GetGridCollisionPoints", Lua_EntityConfigEntityGetGridCollisionPoints },
		{ "GetFriction", Lua_EntityConfigEntityGetFriction },
		{ "GetBaseHP", Lua_EntityConfigEntityGetBaseHP },
		{ "GetStageHP", Lua_EntityConfigEntityGetStageHP },
		{ "GetAnm2Path", Lua_EntityConfigEntityGetAnm2Path },
		{ "GetModName", Lua_EntityConfigEntityGetModName },
		{ "GetEntityTags", Lua_EntityConfigEntityGetEntityTags },
		{ "HasEntityTags", Lua_EntityConfigEntityHasEntityTags },
		{ "GetShadowSize", Lua_EntityConfigEntityGetShadowSize },
		{ "IsBoss", Lua_EntityConfigEntityIsBoss },
		{ "CanBeChampion", Lua_EntityConfigEntityCanBeChampion },
		{ "CanShutDoors", Lua_EntityConfigEntityCanShutDoors },
		{ "GetGibsAmount", Lua_EntityConfigEntityGetGibsAmount },
		{ "GetGibFlags", Lua_EntityConfigEntityGetGibFlags },
		{ "HasGibFlags", Lua_EntityConfigEntityHasGibFlags },
		{ "GetPortraitID", Lua_EntityConfigEntityGetPortraitID },
		{ "CanBeRerolledInto", Lua_EntityConfigEntityCanBeRerolledInto },
		{ "GetCollisionInterval", Lua_EntityConfigEntityGetCollisionInterval },
		{ "GetShieldStrength", Lua_EntityConfigEntityGetShieldStrength },
		{ "GetBestiaryAnm2Path", Lua_EntityConfigEntityGetBestiaryAnm2Path },
		{ "GetBestiaryAnimation", Lua_EntityConfigEntityGetBestiaryAnim },
		{ "GetBestiaryOverlay", Lua_EntityConfigEntityGetBestiaryOverlay },
		{ "GetBestiaryOffset", Lua_EntityConfigEntityGetBestiaryOffset },
		{ "GetBestiaryScale", Lua_EntityConfigEntityGetBestiaryScale },
		{ "GetBestiaryFloorAlt", Lua_EntityConfigEntityGetBestiaryFloorAlt },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::EntityConfigEntityMT, lua::metatables::EntityConfigEntityMT, functions);
}

static void RegisterEntityConfigPlayer(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetPlayerType", Lua_EntityConfigPlayerGetPlayerType },
		{ "GetName", Lua_EntityConfigPlayerGetName },
		{ "GetSkinPath", Lua_EntityConfigPlayerGetSkinPath },
		{ "GetNameImagePath", Lua_EntityConfigPlayerGetNameImagePath },
		{ "GetPortaitPath", Lua_EntityConfigPlayerGetPortraitPath },
		{ "GetExtraPortraitPath", Lua_EntityConfigPlayerGetExtraPortraitPath },
		{ "GetCostumeSuffix", Lua_EntityConfigPlayerGetCostumeSuffix },
		{ "GetCostumeID", Lua_EntityConfigPlayerGetCostumeID },
		{ "GetSkinColor", Lua_EntityConfigPlayerGetSkinColor },
		{ "GetRedHearts", Lua_EntityConfigPlayerGetRedHearts },
		{ "GetSoulHearts", Lua_EntityConfigPlayerGetSoulHearts },
		{ "GetBlackHearts", Lua_EntityConfigPlayerGetBlackHearts },
		{ "GetBrokenHearts", Lua_EntityConfigPlayerGetBrokenHearts },
		{ "GetCoins", Lua_EntityConfigPlayerGetCoins },
		{ "GetBombs", Lua_EntityConfigPlayerGetBombs },
		{ "GetKeys", Lua_EntityConfigPlayerGetKeys },
		{ "GetCard", Lua_EntityConfigPlayerGetCard },
		{ "GetPill", Lua_EntityConfigPlayerGetPill },
		{ "GetTrinket", Lua_EntityConfigPlayerGetTrinket },
		{ "GetCollectibles", Lua_EntityConfigPlayerGetCollectibles },
		{ "GetPocketActive", Lua_EntityConfigPlayerGetPocketActive },
		{ "CanShoot", Lua_EntityConfigPlayerCanShoot },
		{ "GetAchievementID", Lua_EntityConfigPlayerGetAchievement },
		{ "GetBirthrightDescription", Lua_EntityConfigPlayerGetBirthrightDescription },
		{ "GetModdedMenuBackgroundSprite", Lua_EntityConfigPlayerGetModdedMenuBackgroundSprite },
		{ "GetModdedMenuPortraitSprite", Lua_EntityConfigPlayerGetModdedPortraitSprite },
		{ "GetModdedGameOverSprite", Lua_EntityConfigPlayerGetModdedGameOverSprite },
		{ "GetModdedCoopMenuSprite", Lua_EntityConfigPlayerGetModdedCoopMenuSprite },
		{ "GetModdedControlsSprite", Lua_EntityConfigPlayerGetModdedControlsSprite },
		{ "IsTainted", Lua_EntityConfigPlayerIsTainted },
		{ "GetTaintedCounterpart", Lua_EntityConfigPlayerGetTaintedCounterpart },
		{ "IsHidden", Lua_EntityConfigPlayerIsHidden },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::EntityConfigPlayerMT, lua::metatables::EntityConfigPlayerMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	lua::RegisterFunction(_state, lua::Metatables::ENTITY, "GetEntityConfigEntity", Lua_EntityGetEntityConfigEntity);
	lua::RegisterFunction(_state, lua::Metatables::ENTITY_PLAYER, "GetEntityConfigPlayer", Lua_PlayerGetEntityConfigPlayer);

	RegisterEntityConfig(_state);
	RegisterEntityConfigEntity(_state);
	RegisterEntityConfigPlayer(_state);
}
