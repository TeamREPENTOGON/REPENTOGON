#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../Patches/XMLData.h"

static void lua_push_string(lua_State* L, const std::string& string)
{
	lua_pushlstring(L, string.c_str(), string.length());
}

class LuaEntityConfigBoss
{
public:
    struct Userdata
    {
	public:
        static constexpr char* MT = "EntityConfigBoss";
	private:
		uint32_t id = 0;

	public:
		EntityConfig_Boss* GetBossConfig() const;
	private:
        private: Userdata(uint32_t id);
		private: Userdata(const EntityConfig_Boss& boss);
		friend LuaEntityConfigBoss;
    };

    static Userdata* NewUserdata(lua_State* L, uint32_t id);
    static Userdata* NewUserdata(lua_State* L, const EntityConfig_Boss& boss);
    static Userdata* GetUserdata(lua_State* L, int idx);
};

class LuaEntityConfigBossAlt
{
public:
    struct Userdata
    {
	public:
        static constexpr char* MT = "EntityConfigBossAlt";
	private:
		uint32_t bossId = 0;
		uint32_t altIdx = 0;

	public:
		EntityConfig_Boss_Alt* GetBossAlt() const;
	private:
		private: Userdata(const EntityConfig_Boss& boss, size_t idx);
		friend LuaEntityConfigBossAlt;
    };

    static Userdata* NewUserdata(lua_State* L, const EntityConfig_Boss& boss, size_t idx);
    static Userdata* GetUserdata(lua_State* L, int idx);
};

#pragma region EntityConfigBoss

LuaEntityConfigBoss::Userdata::Userdata(uint32_t id)
	: id(id)
{}

LuaEntityConfigBoss::Userdata::Userdata(const EntityConfig_Boss& boss)
	: id(boss.bossID)
{}

EntityConfig_Boss* LuaEntityConfigBoss::Userdata::GetBossConfig() const
{
	auto& bosses = *g_Manager->GetEntityConfig()->GetBosses();
	return &bosses[this->id];
}

LuaEntityConfigBoss::Userdata* LuaEntityConfigBoss::NewUserdata(lua_State* L, uint32_t id)
{
	void* newUserdata = lua_newuserdata(L, sizeof(Userdata));
	Userdata* userdata = new (newUserdata) Userdata(id);
	luaL_setmetatable(L, Userdata::MT);
	return userdata;
}

LuaEntityConfigBoss::Userdata* LuaEntityConfigBoss::NewUserdata(lua_State* L, const EntityConfig_Boss& boss)
{
	void* newUserdata = lua_newuserdata(L, sizeof(Userdata));
	Userdata* userdata = new (newUserdata) Userdata(boss);
	luaL_setmetatable(L, Userdata::MT);
	return userdata;
}

LuaEntityConfigBoss::Userdata* LuaEntityConfigBoss::GetUserdata(lua_State* L, int idx)
{
	return lua::GetRawUserdata<Userdata*>(L, idx, Userdata::MT);
}

static EntityConfig_Boss* get_boss_config_ud(lua_State* L, int idx)
{
	const LuaEntityConfigBoss::Userdata* ud = LuaEntityConfigBoss::GetUserdata(L, idx);
	return ud->GetBossConfig();
}

LUA_FUNCTION(Lua_Boss_GetBossId)
{
	lua_pushinteger(L, get_boss_config_ud(L, 1)->bossID);
	return 1;
}

LUA_FUNCTION(Lua_Boss_GetName)
{
	lua_push_string(L, get_boss_config_ud(L, 1)->name);
	return 1;
}

LUA_FUNCTION(Lua_Boss_GetNamePath)
{
	lua_push_string(L, get_boss_config_ud(L, 1)->namePath);
	return 1;
}

LUA_FUNCTION(Lua_Boss_GetPortraitPath)
{
	lua_push_string(L, get_boss_config_ud(L, 1)->portraitPath);
	return 1;
}

LUA_FUNCTION(Lua_Boss_GetPivot)
{
	Vector& pivot = get_boss_config_ud(L, 1)->pivot;
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), pivot);
	return 1;
}

LUA_FUNCTION(Lua_Boss_GetAchievement)
{
	lua_pushinteger(L, get_boss_config_ud(L, 1)->achievement);
	return 1;
}

LUA_FUNCTION(Lua_Boss_GetAlt)
{
	EntityConfig_Boss* boss = get_boss_config_ud(L, 1);
	size_t idx = (size_t)lua_tointeger(L, 2);

	bool inRange = idx < boss->alts.size();
	if (!inRange)
	{
		luaL_argerror(L, 2, "index out of range");
	}

	LuaEntityConfigBossAlt::NewUserdata(L, *boss, idx);
	return 1;
}

LUA_FUNCTION(Lua_Boss_GetNumAlts)
{
	lua_pushinteger(L, get_boss_config_ud(L, 1)->alts.size());
	return 1;
}

static void Boss_RegisterClass(lua_State* L)
{
	luaL_Reg functions[] = {
		{ "GetBossID", Lua_Boss_GetBossId },
		{ "GetName", Lua_Boss_GetName },
		{ "GetNamePath", Lua_Boss_GetNamePath },
		{ "GetPortraitPath", Lua_Boss_GetPortraitPath },
		{ "GetPivot", Lua_Boss_GetPivot },
		{ "GetAchievement", Lua_Boss_GetAchievement },
		{ "GetAlt", Lua_Boss_GetAlt },
		{ "GetNumAlts", Lua_Boss_GetNumAlts },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, LuaEntityConfigBoss::Userdata::MT, LuaEntityConfigBoss::Userdata::MT, functions);
}

#pragma endregion

#pragma region EntityConfigBossAlt

LuaEntityConfigBossAlt::Userdata::Userdata(const EntityConfig_Boss& boss, size_t idx)
	: bossId(boss.bossID), altIdx(idx)
{}

EntityConfig_Boss_Alt* LuaEntityConfigBossAlt::Userdata::GetBossAlt() const
{
	auto& bosses = *g_Manager->GetEntityConfig()->GetBosses();
	return &bosses[this->bossId].alts[this->altIdx];
}

LuaEntityConfigBossAlt::Userdata* LuaEntityConfigBossAlt::NewUserdata(lua_State* L, const EntityConfig_Boss& boss, size_t idx)
{
	void* newUserdata = lua_newuserdata(L, sizeof(Userdata));
	Userdata* userdata = new (newUserdata) Userdata(boss, idx);
	luaL_setmetatable(L, Userdata::MT);
	return userdata;
}

LuaEntityConfigBossAlt::Userdata* LuaEntityConfigBossAlt::GetUserdata(lua_State* L, int idx)
{
	return lua::GetRawUserdata<Userdata*>(L, idx, Userdata::MT);
}

static EntityConfig_Boss_Alt* get_boss_alt_ud(lua_State* L, int idx)
{
	const LuaEntityConfigBossAlt::Userdata* ud = LuaEntityConfigBossAlt::GetUserdata(L, idx);
	return ud->GetBossAlt();
}

LUA_FUNCTION(Lua_BossAlt_GetStageId)
{
	lua_pushinteger(L, get_boss_alt_ud(L, 1)->stageId);
	return 1;
}

LUA_FUNCTION(Lua_BossAlt_GetPortraitPath)
{
	lua_push_string(L, get_boss_alt_ud(L, 1)->portraitPath);
	return 1;
}

static void BossAlt_RegisterClass(lua_State* L)
{
	luaL_Reg functions[] = {
		{ "GetStageID", Lua_BossAlt_GetStageId },
		{ "GetPortraitPath", Lua_BossAlt_GetPortraitPath },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, LuaEntityConfigBossAlt::Userdata::MT, LuaEntityConfigBossAlt::Userdata::MT, functions);
}

#pragma endregion

LUA_FUNCTION(Lua_EntityGetEntityConfigEntity)
{
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");

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
	Entity_Player* player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");

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

LUA_FUNCTION(Lua_EntityConfig_GetBoss)
{
	size_t id = (size_t)luaL_checkinteger(L, 1);
	auto& bosses = *g_Manager->GetEntityConfig()->GetBosses();

	bool inRange = id < bosses.size();
	if (!inRange)
	{
		luaL_argerror(L, 1, "id out of range");
	}

	LuaEntityConfigBoss::NewUserdata(L, bosses[id]);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfig_GetMaxBossId)
{
	lua_pushinteger(L, g_Manager->GetEntityConfig()->GetBosses()->size() - 1);
	return 1;
}

/*
* EntityConfigEntity Functions
*/

LUA_FUNCTION(Lua_EntityConfigEntityGetType)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->id);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetVariant)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->variant);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetSubType)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->subtype);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetName)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionDamage)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->collisionDamage);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBossId)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->bossID);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionRadius)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->collisionRadius);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionRadiusMultiplier)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), entity->collisionRadiusMulti);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetMass)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->mass);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetGridCollisionPoints)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->gridCollisionPoints);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetFriction)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->friction);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBaseHP)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->baseHP);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetStageHP)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->stageHP);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetAnm2Path)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->anm2Path.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetModName)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
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
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->tags);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityHasEntityTags)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
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
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->shadowSize);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityIsBoss)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushboolean(L, entity->isBoss);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityCanBeChampion)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushboolean(L, entity->canBeChampion);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityCanShutDoors)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushboolean(L, entity->shutDoors);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetGibsAmount)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->gibsAmount);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetGibFlags)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->gibFlags);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityHasGibFlags)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
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
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->portrait);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityCanBeRerolledInto)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushboolean(L, entity->reroll);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityHasFloorAlts)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushboolean(L, entity->hasFloorAlts);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionInterval)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->collisionInterval);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetShieldStrength)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->shieldStrength);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryOffset)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), entity->bestiaryOffset);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryScale)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->bestiaryScale);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryAnm2Path)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->bestiaryAnm2Path.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryAnim)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->bestiaryAnim.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryOverlay)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->bestiaryOverlay.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBestiaryFloorAlt)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->bestiaryFloorAlt.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCustomTags)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	const std::set<std::string>& customtags = XMLStuff.EntityData->GetCustomTags(*entity);

	lua_newtable(L);
	int i = 0;
	for (const std::string& tag : customtags) {
		lua_pushinteger(L, ++i);
		lua_pushstring(L, tag.c_str());
		lua_settable(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityHasCustomTag)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	const std::string tag = luaL_checkstring(L, 2);
	lua_pushboolean(L, XMLStuff.EntityData->HasCustomTag(*entity, tag));
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetDevolvedEntity)
{
	EntityConfig_Entity* entity = *lua::GetRawUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);

	EntityConfig_Entity* devolvedEntity = nullptr;
	if (!entity->devolve.empty()) {
		// The game only uses the first one.
		const Devolve& devolve = entity->devolve.front();
		devolvedEntity = g_Manager->GetEntityConfig()->GetEntity(devolve.type, devolve.variant, devolve.subtype);
	}

	if (devolvedEntity) {
		EntityConfig_Entity** toLua = (EntityConfig_Entity**)lua_newuserdata(L, sizeof(EntityConfig_Entity*));
		*toLua = devolvedEntity;
		luaL_setmetatable(L, lua::metatables::EntityConfigEntityMT);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

/*
* EntityConfigPlayer Functions
*/

LUA_FUNCTION(Lua_EntityConfigPlayerGetPlayerType)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_id);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetName)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetSkinPath)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_skinPath.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetNameImagePath)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_nameImagePath.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetPortraitPath)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_portraitPath.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetExtraPortraitPath)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_extraPortraitPath.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetCostumeSuffix)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_costumeSuffixName.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetCostumeID)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_costumeID);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetSkinColor)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_skinColor);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetRedHearts)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_heart);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetSoulHearts)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_armor);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetBlackHearts)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_blackHeart);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetBrokenHearts)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_brokenHeart);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetCoins)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_coins);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetBombs)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_bombs);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetKeys)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_keys);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetCard)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_card);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetPill)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_pill);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetTrinket)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_trinket);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetCollectibles)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);

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
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_pocketActiveID);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerCanShoot)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushboolean(L, player->_canShoot);
	return 1;
}

const std::set<int> HiddenVanillaCharacters = { 11, 12, 17, 20, 38, 39, 40 };

LUA_FUNCTION(Lua_EntityConfigPlayerIsHidden)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);

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
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushinteger(L, player->_achievement);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetBirthrightDescription)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
	lua_pushstring(L, player->_birthrightDescription.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigPlayerGetModdedMenuBackgroundSprite)
{
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
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
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
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
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
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
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
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
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
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
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);
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
	EntityConfig_Player* player = *lua::GetRawUserdata<EntityConfig_Player**>(L, 1, lua::metatables::EntityConfigPlayerMT);

	int counterpartID = -1;

	if (TaintedMap.find(player->_id) != TaintedMap.end()) {
		counterpartID = TaintedMap[player->_id];
	}
	else if (!player->_bSkinParentName.empty()) {
		// Modded Tainted
		for (unsigned int i = 41; i < g_Manager->GetEntityConfig()->GetPlayers()->size(); i++) {
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

	if (counterpartID < 0 || (unsigned int)counterpartID >= g_Manager->GetEntityConfig()->GetPlayers()->size()) {
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
* EntityConfigBaby Functions
*/

LUA_FUNCTION(Lua_EntityConfigGetBaby)
{
	const int id = (int)luaL_checkinteger(L, 1);

	EntityConfig_Baby* babyConfig = g_Manager->GetEntityConfig()->GetBaby(id);

	if (babyConfig == nullptr) {
		lua_pushnil(L);
	}
	else {
		EntityConfig_Baby** toLua = (EntityConfig_Baby**)lua_newuserdata(L, sizeof(EntityConfig_Baby*));
		*toLua = babyConfig;
		luaL_setmetatable(L, lua::metatables::EntityConfigBabyMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityConfigGetMaxBabyID)
{
	lua_pushinteger(L, g_Manager->GetEntityConfig()->GetBabies()->size() - 1);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigBabyGetId)
{
	EntityConfig_Baby* baby = *lua::GetRawUserdata<EntityConfig_Baby**>(L, 1, lua::metatables::EntityConfigBabyMT);
	lua_pushinteger(L, baby->id);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigBabyGetName)
{
	EntityConfig_Baby* baby = *lua::GetRawUserdata<EntityConfig_Baby**>(L, 1, lua::metatables::EntityConfigBabyMT);
	lua_pushstring(L, baby->name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigBabyGetSpritesheetPath)
{
	EntityConfig_Baby* baby = *lua::GetRawUserdata<EntityConfig_Baby**>(L, 1, lua::metatables::EntityConfigBabyMT);
	lua_pushstring(L, baby->gfx.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigBabyGetAchievementID)
{
	EntityConfig_Baby* baby = *lua::GetRawUserdata<EntityConfig_Baby**>(L, 1, lua::metatables::EntityConfigBabyMT);
	lua_pushinteger(L, baby->achievementID);
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
	lua::TableAssoc(L, "GetBaby", Lua_EntityConfigGetBaby);
	lua::TableAssoc(L, "GetMaxBabyID", Lua_EntityConfigGetMaxBabyID);
	lua::TableAssoc(L, "GetBoss", Lua_EntityConfig_GetBoss);
	lua::TableAssoc(L, "GetMaxBossID", Lua_EntityConfig_GetMaxBossId);
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
		{ "HasFloorAlts", Lua_EntityConfigEntityHasFloorAlts },
		{ "GetCustomTags", Lua_EntityConfigEntityGetCustomTags },
		{ "HasCustomTag", Lua_EntityConfigEntityHasCustomTag },
		{ "GetDevolvedEntity", Lua_EntityConfigEntityGetDevolvedEntity},
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
		{ "GetPortraitPath", Lua_EntityConfigPlayerGetPortraitPath },
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

static void RegisterEntityConfigBaby(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetID", Lua_EntityConfigBabyGetId },
		{ "GetName", Lua_EntityConfigBabyGetName },
		{ "GetSpritesheetPath", Lua_EntityConfigBabyGetSpritesheetPath },
		{ "GetAchievementID", Lua_EntityConfigBabyGetAchievementID },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::EntityConfigBabyMT, lua::metatables::EntityConfigBabyMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	lua::RegisterFunction(_state, lua::Metatables::ENTITY, "GetEntityConfigEntity", Lua_EntityGetEntityConfigEntity);
	lua::RegisterFunction(_state, lua::Metatables::ENTITY_PLAYER, "GetEntityConfigPlayer", Lua_PlayerGetEntityConfigPlayer);

	RegisterEntityConfig(_state);
	RegisterEntityConfigEntity(_state);
	RegisterEntityConfigPlayer(_state);
	RegisterEntityConfigBaby(_state);
	Boss_RegisterClass(_state);
	BossAlt_RegisterClass(_state);
}
