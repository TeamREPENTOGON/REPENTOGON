#pragma once

#include "IsaacRepentance.h"
#include "../SaveStateManagement/EntitySaveStateManagement.h"
namespace ESSM = EntitySaveStateManagement;

namespace EntitySaveStateUtils
{
    struct Bomb
    {
        static inline uint32_t* GetScale(EntitySaveState& saveState) { return (uint32_t*)&saveState.intStorage1; }
        static inline BitSet128* GetTearFlags(EntitySaveState& saveState) { return (BitSet128*)&saveState.intStorage2; }
        static inline bool* GetIsFetus(EntitySaveState& saveState) { return (bool*)&saveState.boolStorage2; }
        static inline float* GetExplosionDamage(EntitySaveState& saveState) { return (float*)&saveState.floatStorage1; }
        static inline float* GetRadiusMulti(EntitySaveState& saveState) { return (float*)&saveState.floatStorage2; }
        static inline int8_t* GetSpawnerPlayerIndex(EntitySaveState& saveState) { return (int8_t*)&saveState.byteStorage; } // -1 if not spawned by player
    };

    struct Pickup
    {
        static inline int* GetCharge(EntitySaveState& saveState) { return (int*)&saveState.intStorage2; }
        static inline int* GetPrice(EntitySaveState& saveState) { return (int*)&saveState.intStorage3; }
        static inline bool* GetAutoUpdatePrice(EntitySaveState& saveState) { return (bool*)&saveState.byteStorage; }
        static inline int* GetShopItemId(EntitySaveState& saveState) { return (int*)&saveState.intStorage4; }
        static inline bool* GetTouched(EntitySaveState& saveState) { return (bool*)&saveState.boolStorage1; }
        static inline int* GetOptionsPickupIndex(EntitySaveState& saveState) { return (int*)&ESSM::EntitySaveState_GetI7(saveState); }
        static inline int* GetTimeout(EntitySaveState& saveState) { return (int*)&saveState.intStorage5; }
        static inline bool* GetIsBlind(EntitySaveState& saveState) { return (bool*)&saveState.boolStorage2; }
        static inline int* GetAlternatePedestal(EntitySaveState& saveState) { return (int*)&saveState.intStorage1; }
        static inline int* GetActiveVarData(EntitySaveState& saveState) { return (int*)&saveState.intStorage8; }
        static inline int* GetSourcePoolType(EntitySaveState& saveState) { return (int*)&saveState.floatStorage1; }
        static inline float* GetSpriteScale(EntitySaveState& saveState) { return (float*)&saveState.floatStorage2; }
        static inline std::array<int, 8>* GetCycleCollectibles(EntitySaveState& saveState) { return *(std::array<int, 8>**)((char*)&saveState + 0x64); } // 0 is used as a terminator if < 8
        static inline EntitySaveState* GetFlipSaveState(const EntitySaveState& saveState) { return saveState.entitySaveState.saveState; }
    };

    struct Slot
    {
        static inline int* GetDonationValue(EntitySaveState& saveState) { return (int*)&saveState.intStorage1; }
        static inline int* GetTriggerTimer(EntitySaveState& saveState) { return (int*)&saveState.intStorage2; } // Only used by SHOP_RESTOCK_MACHINE 
        static inline int* GetPrizeCollectible(EntitySaveState& saveState) { return (int*)&ESSM::EntitySaveState_GetI7(saveState); };
    };

    struct NPC // FIREPLACE and MOVABLE_TNT
    {
        static inline int* GetHealth(EntitySaveState& saveState) { return (int*)&saveState.intStorage1; }
    };

    struct Effect
    {
        static inline int* GetState(EntitySaveState& saveState) { return (int*)&saveState.intStorage1; } // Used by DIRT_PATCH, SPAWNER and LIL_GHOST
        static inline int* GetVarData0(EntitySaveState& saveState) { return (int*)&saveState.intStorage4; } // Used by SPAWNER
        static inline int* GetVarData1(EntitySaveState& saveState) { return (int*)&saveState.intStorage2; } // Used by SPAWNER
    };

    struct Minecart
    {
        static inline float* GetAngleDegrees(EntitySaveState& saveState) { return (float*)&saveState.floatStorage1; }
    };
}