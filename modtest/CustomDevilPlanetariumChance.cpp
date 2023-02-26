#include <lua.hpp>

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"

HOOK_METHOD(Room, GetDevilRoomChance, () -> float) {
	// Soon.
    return super();
}

bool modsChangingPlanetariumChance(lua_State *L) {
    int callbacks[6] = { 1110, 1111, 1112, 1113, 1114, 1115 };
    lua::LuaStackProtector protector(L);

    for (int callback : callbacks) {

        lua_getglobal(L, "Isaac");
        lua_getfield(L, -1, "GetCallbacks");
        lua_remove(L, lua_absindex(L, -2));
        lua::LuaResults callbackResult = lua::LuaCaller(L).push(callback)
            .call(1);

        if (!callbackResult) {
            if (lua_rawlen(L, -1) > 0) {
                return true;
            }
        }
    }
    return false;
}

HOOK_METHOD(Game, GetPlanetariumChance, () -> float) {
    float originalChance = super();

    // For dailies, take zero chances (hah) and just return the original value
    if (g_Game->GetDailyChallenge()._id) {
        printf("DAILY PLAYER SPOTTED :(\n");
        return originalChance;
    }
    lua_State* L = g_LuaEngine->_state;

    // My reimplementation *should* be accurate, but there's no reason to run it if mods aren't actively attempting to change values.
    if (!modsChangingPlanetariumChance(L))
        return originalChance;
 
    lua::LuaStackProtector protector(L);
    int stageType = this->_stageType;
    int stage = this->_stage;
    PlayerManager* manager = g_Game->GetPlayerManager();
    bool hasTelescopeLens = manager->FirstTrinketOwner(TRINKET_TELESCOPE_LENS, 0, true);
    bool shouldBypassPlanetariumRestriction = false;
    bool shouldBypassStageRestriction = false;
    float chance = 0.01;

    //MC_PRE_PLANETARIUM_APPLY_STAGE_PENALTY
    lua_getglobal(L, "Isaac");
    lua_getfield(L, -1, "RunCallback");
    lua_remove(L, lua_absindex(L, -2));

    lua::LuaResults preApplyStageResult = lua::LuaCaller(L).push(1110)
        .call(1);

    if (!preApplyStageResult) {
        shouldBypassStageRestriction = lua_toboolean(L, -1);
    }
    //MC_PRE_PLANETARIUM_APPLY_STAGE_PENALTY

    if (stageType == STAGETYPE_REPENTANCE || stageType == STAGETYPE_REPENTANCE_B)
        stage++;

    if (!shouldBypassStageRestriction && (stage > STAGE3_2 && !hasTelescopeLens || stage > STAGE4_2))
        return 0.f;

    //printf("after APPLY STAGE PENALTY the chance is %f\n", chance);

    //MC_PRE_PLANETARIUM_APPLY_PLANETARIUM_PENALTY
    lua_getglobal(L, "Isaac");
    lua_getfield(L, -1, "RunCallback");
    lua_remove(L, lua_absindex(L, -2));

    lua::LuaResults preApplyPlanetariumResult = lua::LuaCaller(L).push(1111)
        .call(1);

    if (!preApplyPlanetariumResult) {
        shouldBypassPlanetariumRestriction = lua_toboolean(L, -1);
    }
    //MC_PRE_PLANETARIUM_APPLY_PLANETARIUM_PENALTY

    if (!g_Game->GetPlanetariumsVisited() || shouldBypassPlanetariumRestriction) {
        bool shouldBypassTreasureRestriction = false;
        int treasureRoomsVisited = g_Game->GetTreasureRoomsVisited();
        //MC_PRE_PLANETARIUM_APPLY_TREASURE_PENALTY
        lua_getglobal(L, "Isaac");
        lua_getfield(L, -1, "RunCallback");
        lua_remove(L, lua_absindex(L, -2));

        lua::LuaResults preApplyTreasureResult = lua::LuaCaller(L).push(1112)
            .push(treasureRoomsVisited)
            .call(1);

        if (!preApplyTreasureResult) {
            if (lua_isinteger(L, -1))
                treasureRoomsVisited = lua_tointeger(L, -1);
            else shouldBypassTreasureRestriction = lua_toboolean(L, -1);
        }
        //MC_PRE_PLANETARIUM_APPLY_TREASURE_PENALTY

        if (treasureRoomsVisited < stage - 1 || shouldBypassTreasureRestriction) {
                chance += ((stage - treasureRoomsVisited - 1) * 0.2);

            // no idea if these values to FirstCollectibleOwner are right, but... trust in the decomp, i guess?
            // second is 0 for now, gotta figure out why FirstCollectibleOwner even... needs RNG, that doesn't make sense to me
            if (g_Game->GetPlayerManager()->FirstCollectibleOwner(COLLECTIBLE_CRYSTAL_BALL, 0, true)) {
                chance = chance + 1.f;
            }
        }

        //MC_PRE_PLANETARIUM_APPLY_ITEMS
        lua_getglobal(L, "Isaac");
        lua_getfield(L, -1, "RunAdditiveCallback");
        lua_remove(L, lua_absindex(L, -2));

        lua::LuaResults preApplyItemsResult = lua::LuaCaller(L).push(1113)
            .push(chance)
            .call(1);

        if (!preApplyItemsResult) {
            if(lua_isnumber(L, -1))
                chance = lua_tonumber(L, -1);
        }
        //MC_PRE_PLANETARIUM_APPLY_ITEMS

        if (g_Game->GetPlayerManager()->FirstCollectibleOwner(COLLECTIBLE_MAGIC_8_BALL, 0, true))
            chance += 0.15;

        if (g_Game->GetPlayerManager()->FirstCollectibleOwner(COLLECTIBLE_CRYSTAL_BALL, 0, true))
            chance += 0.15;

        if (g_Game->GetPlayerManager()->FirstCollectibleOwner(COLLECTIBLE_SAUSAGE, 0, true))
            chance += 0.069; //teehee

        if (hasTelescopeLens)
            chance += 0.15;
    }
    //MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS
    lua_getglobal(L, "Isaac");
    lua_getfield(L, -1, "RunAdditiveCallback");
    lua_remove(L, lua_absindex(L, -2));

    lua::LuaResults preApplyLensResult = lua::LuaCaller(L).push(1114)
        .push(chance)
        .call(1);

    if (!preApplyLensResult) {
        if(lua_isnumber(L, -1))
            chance = lua_tonumber(L, -1);
    }
    //MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS

    if (hasTelescopeLens)
        chance += 0.09;

    //MC_POST_PLANETARIUM_CALCULATE
    lua_getglobal(L, "Isaac");
    lua_getfield(L, -1, "RunAdditiveCallback");
    lua_remove(L, lua_absindex(L, -2));

    lua::LuaResults postCalculateResult = lua::LuaCaller(L).push(1115)
        .push(chance)
        .call(1);

    if (!postCalculateResult) 
        if(lua_isnumber(L, -1))
            chance = lua_tonumber(L, -1);
    //MC_POST_PLANETARIUM_CALCULATE

    if (chance > 1.f)
        chance = 1.f;

    return chance;
}