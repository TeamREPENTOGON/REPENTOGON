#include <lua.hpp>

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "LuaCore.h"

extern std::bitset<500> CallbackState;
extern int additiveCallbackKey;

bool modsChangingDevilChance(lua_State* L) {
    int callbacks[4] = { 1130, 1131, 1132, 1133 };

    for (int callback : callbacks) {
        if (CallbackState.test(callback - 1000)) {
            return true;
        }
    }

    return false;
}

HOOK_METHOD(Room, GetDevilRoomChance, () -> float) {
    float originalChance = super();

    // Same as with planetariums, return the original value for dailies
    // The original deal code has some special handling for dailies that aren't necessary in cases where the reimplementation is needed
    if (g_Game->GetDailyChallenge()._id)
        return originalChance;

    lua_State* L = g_LuaEngine->_state;

    // My reimplementation *should* be accurate, but there's no reason to run it if mods aren't actively attempting to change values.
    if (!modsChangingDevilChance(L)) 
        return originalChance;

    PlayerManager* manager = g_Game->GetPlayerManager();
    RNG *rng = &manager->_rng;
    int flags = *g_Game->GetLevelStateFlags();
    Room* room = *g_Game->GetCurrentRoom();
    RoomDescriptor* desc = g_Game->GetRoomByIdx(g_Game->GetCurrentRoomIdx(), -1);
    EntityList* list = room->GetEntityList();
    bool hasActOfContrition = manager->FirstCollectibleOwner(COLLECTIBLE_ACT_OF_CONTRITION, &rng, true);
    lua::LuaStackProtector protector(L);

    float chance = 0;

    if (!g_Game->_devilRoomDisabled) { 
        chance += 0.01f;

        //MC_PRE_DEVIL_APPLY_ITEMS
        if (CallbackState.test(1130 - 1000)) {

            lua_rawgeti(L, LUA_REGISTRYINDEX, additiveCallbackKey);

            lua::LuaResults preApplyItemsResult = lua::LuaCaller(L).push(1130)
                .push(chance)
                .call(1);

            if (!preApplyItemsResult) {
                chance = (float)lua_tonumber(L, -1);
            }
        }
        //MC_PRE_DEVIL_APPLY_ITEMS

        if (manager->FirstCollectibleOwner(COLLECTIBLE_PENTAGRAM, &rng, true))
            chance += 0.1f;
        if (manager->GetNumCollectibles(COLLECTIBLE_PENTAGRAM) > 1)
            chance += 0.05f;

        if (manager->FirstCollectibleOwner(COLLECTIBLE_BLACK_CANDLE, &rng, true))
            chance += 0.15f;
        if (manager->FirstCollectibleOwner(COLLECTIBLE_BOOK_OF_BELIAL, &rng, true) || manager->FirstCollectibleOwner(COLLECTIBLE_BOOK_OF_BELIAL_PASSIVE, &rng, true))
            chance += 0.125f;
        if (manager->FirstCollectibleOwner(COLLECTIBLE_BOOK_OF_REVELATIONS, &rng, true))
            chance += 0.175f;

        if ((flags & 1) != 0) //Beggar killed, haven't identified the right enum yet
            chance += 0.35f;

        if (desc && desc->ListIndex == g_Game->GetLastBossRoomListIdx() && room->GetRedHeartDamage()) {
            if (hasActOfContrition)
                chance += 0.15f;
        }
        else
            chance += 0.35f;

        if ((flags & 4) != 0) { // Took red heart damage (excluding sac rooms and such)
            if (hasActOfContrition)
                chance += 0.4f;
        }
        else
            chance += 0.99f;

        if ((flags & 0x40) != 0) // Shopkeeper crushkilled
            chance += 0.1f;

        if (manager->FirstTrinketOwner(TRINKET_NUMBER_MAGNET, &rng, true))
            chance += 0.1f;

        if (manager->FirstCollectibleOwner(COLLECTIBLE_SAUSAGE, &rng, true))
            chance += 0.069f; // nice

        chance += (0.1f * list->CountWisps(COLLECTIBLE_SATANIC_BIBLE));

        unsigned int lastDevilRoomStage = g_Game->GetLastDevilRoomStage();
        bool shouldApplyStagePenalty = true;

        //MC_PRE_DEVIL_APPLY_STAGE_PENALTY
        if (CallbackState.test(1131 - 1000)) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

            lua::LuaResults preApplyStagePenaltyResult = lua::LuaCaller(L).push(1131)
                .push(chance)
                .call(1);

            if (!preApplyStagePenaltyResult) {
                if (lua_isboolean(L, -1))
                    shouldApplyStagePenalty = lua_toboolean(L, -1);
            }
        }
        //MC_PRE_DEVIL_APPLY_STAGE_PENALTY

        if (lastDevilRoomStage && shouldApplyStagePenalty) {
            unsigned int stageType = g_Game->_stageType;
            unsigned int stage = g_Game->_stage;

            if (stageType == STAGETYPE_REPENTANCE || stageType == STAGETYPE_REPENTANCE_B)
                stage++;

            unsigned int stagesSinceDealTaken = stage - lastDevilRoomStage;

            if (stagesSinceDealTaken && (--stagesSinceDealTaken, stagesSinceDealTaken)) {
                if (stagesSinceDealTaken == 1)
                    chance *= 0.5;
            }
            else
                chance *= 0.25;
        }
    }

    //MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS
    if (CallbackState.test(1132 - 1000)) {

        lua_rawgeti(L, LUA_REGISTRYINDEX, additiveCallbackKey);

        lua::LuaResults preApplySpecialItemsResult = lua::LuaCaller(L).push(1132)
            .push(chance)
            .call(1);

        if (!preApplySpecialItemsResult) {
            chance = (float)lua_tonumber(L, -1);
        }
    }
    //MC_PRE_DEVIL_APPLY_SPECIAL_ITEMS

    if (manager->FirstCollectibleOwner(COLLECTIBLE_GOAT_HEAD, &rng, true))
        chance = 66.6f; // the game truncates the value anyways... but hey, vanilla game does it, i have to be accurate
     
    if (!this->GetTemporaryEffects()->_disabled) {
        for (TemporaryEffect effect : this->GetTemporaryEffects()->_effects) {
            int type = effect._item->type;
            if ((type == 1 || type == 3 || type == 4) && effect._item->id == COLLECTIBLE_GOAT_HEAD) // Passive, active and familiar types. TODO Need to add enums
                chance = 66.6f;
        }
    }

    if (manager->FirstCollectibleOwner(COLLECTIBLE_EUCHARIST, &rng, true))
        chance = 1;

    //MC_POST_DEVIL_CALCULATE
    if (CallbackState.test(1133 - 1000)) {

        lua_rawgeti(L, LUA_REGISTRYINDEX, additiveCallbackKey);

        lua::LuaResults postDevilCalculateResult = lua::LuaCaller(L).push(1133)
            .push(chance)
            .call(1);

        if (!postDevilCalculateResult) {
            chance = (float)lua_tonumber(L, -1);
        }
    }
    //MC_POST_DEVIL_CALCULATE

    if (g_Game->IsGreedMode()) {
        if (g_Game->GetGreedModeWave() < (int)(g_Game->GetDifficulty() == 3) + 11) // 11 waves pass for devil chance in Greed, 12 in Greedier
            chance = 0;
        else
            chance = 1;
    }

    return chance;
}

bool modsChangingPlanetariumChance(lua_State *L) {
    int callbacks[6] = { 1110, 1111, 1112, 1113, 1114, 1115 };

    for (int callback : callbacks) {
        if (CallbackState.test(callback - 1000)) {
            return true;
        }
    }
    return false;
}

HOOK_METHOD(Game, GetPlanetariumChance, () -> float) {
    float originalChance = super();

    // For dailies, take zero chances (hah) and just return the original value
    if (g_Game->GetDailyChallenge()._id) {
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
    bool shouldApplyStageRestriction = true;
    float chance = 0.01f;

    //MC_PRE_PLANETARIUM_APPLY_STAGE_PENALTY
    if (CallbackState.test(1110 - 1000)) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
        lua_remove(L, lua_absindex(L, -2));

        lua::LuaResults preApplyStageResult = lua::LuaCaller(L).push(1110)
            .call(1);

        if (!preApplyStageResult) {
            if (lua_isboolean(L, -1))
                shouldApplyStageRestriction = lua_toboolean(L, -1);
        }
    }
    //MC_PRE_PLANETARIUM_APPLY_STAGE_PENALTY

    if (stageType == STAGETYPE_REPENTANCE || stageType == STAGETYPE_REPENTANCE_B)
        stage++;

    if (shouldApplyStageRestriction && (stage > STAGE3_2 && !hasTelescopeLens || stage > STAGE4_2))
        return 0.f;

    //MC_PRE_PLANETARIUM_APPLY_PLANETARIUM_PENALTY
    if (CallbackState.test(1111 - 1000)) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
        lua_remove(L, lua_absindex(L, -2));

        lua::LuaResults preApplyPlanetariumResult = lua::LuaCaller(L).push(1111)
            .call(1);

        if (!preApplyPlanetariumResult) {
            if (lua_isboolean(L, -1))
                shouldBypassPlanetariumRestriction = !lua_toboolean(L, -1);
        }
    }
    //MC_PRE_PLANETARIUM_APPLY_PLANETARIUM_PENALTY

    if (!g_Game->GetPlanetariumsVisited() || shouldBypassPlanetariumRestriction) {
        bool shouldBypassTreasureRestriction = false;
        int treasureRoomsVisited = g_Game->GetTreasureRoomsVisited();
        //MC_PRE_PLANETARIUM_APPLY_TREASURE_PENALTY
        if (CallbackState.test(1112 - 1000)) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
            lua_remove(L, lua_absindex(L, -2));

            lua::LuaResults preApplyTreasureResult = lua::LuaCaller(L).push(1112)
                .push(treasureRoomsVisited)
                .call(1);

            if (!preApplyTreasureResult) {
                if (lua_isinteger(L, -1)) {
                    treasureRoomsVisited = (int)lua_tointeger(L, -1);
                }
                else if (lua_isboolean(L, -1)) {
                    shouldBypassTreasureRestriction = !lua_toboolean(L, -1);
                    if (shouldBypassTreasureRestriction)
                        treasureRoomsVisited = 0;
                }
            }
        }
        //MC_PRE_PLANETARIUM_APPLY_TREASURE_PENALTY
        if (treasureRoomsVisited < stage - 1 || shouldBypassTreasureRestriction) {
            chance += ((stage - treasureRoomsVisited - 1) * 0.2f);

            // no idea if these values to FirstCollectibleOwner are right, but... trust in the decomp, i guess?
            // second is 0 for now, gotta figure out why FirstCollectibleOwner even... needs RNG, that doesn't make sense to me
            if (g_Game->GetPlayerManager()->FirstCollectibleOwner(COLLECTIBLE_CRYSTAL_BALL, 0, true)) {
                chance = chance + 1.f;
            }
        }

        //MC_PRE_PLANETARIUM_APPLY_ITEMS
        if (CallbackState.test(1113 - 1000)) {

            lua_rawgeti(L, LUA_REGISTRYINDEX, additiveCallbackKey);

            lua::LuaResults preApplyItemsResult = lua::LuaCaller(L).push(1113)
                .push(chance)
                .call(1);

            if (!preApplyItemsResult) {
                if (lua_isnumber(L, -1))
                    chance = (float)lua_tonumber(L, -1);
            }
        }
        //MC_PRE_PLANETARIUM_APPLY_ITEMS

        if (g_Game->GetPlayerManager()->FirstCollectibleOwner(COLLECTIBLE_MAGIC_8_BALL, 0, true))
            chance += 0.15f;

        if (g_Game->GetPlayerManager()->FirstCollectibleOwner(COLLECTIBLE_CRYSTAL_BALL, 0, true))
            chance += 0.15f;

        if (g_Game->GetPlayerManager()->FirstCollectibleOwner(COLLECTIBLE_SAUSAGE, 0, true))
            chance += 0.069f; //teehee

        if (hasTelescopeLens)
            chance += 0.15f;
    }
    //MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS
    if (CallbackState.test(1114 - 1000)) {

        lua_rawgeti(L, LUA_REGISTRYINDEX, additiveCallbackKey);

        lua::LuaResults preApplyLensResult = lua::LuaCaller(L).push(1114)
            .push(chance)
            .call(1);

        if (!preApplyLensResult) {
            if (lua_isnumber(L, -1))
                chance = (float)lua_tonumber(L, -1);
        }
    }
    //MC_PRE_PLANETARIUM_APPLY_TELESCOPE_LENS

    if (hasTelescopeLens)
        chance += 0.09f;

    //MC_POST_PLANETARIUM_CALCULATE
    if (CallbackState.test(1115 - 1000)) {

        lua_rawgeti(L, LUA_REGISTRYINDEX, additiveCallbackKey);

        lua::LuaResults postCalculateResult = lua::LuaCaller(L).push(1115)
            .push(chance)
            .call(1);

        if (!postCalculateResult)
            if (lua_isnumber(L, -1))
                chance = (float)lua_tonumber(L, -1);
    }
    //MC_POST_PLANETARIUM_CALCULATE

    if (chance > 1.f)
        chance = 1.f;

    return chance;
}
