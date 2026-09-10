#include "IsaacRepentance.h"

extern "C" {
    __declspec(dllexport) void L_TemporaryEffects_AddCollectibleEffect(TemporaryEffects* effects, int collectibleType, bool addCostume, int count) {
        effects->AddCollectibleEffect(collectibleType, addCostume, count);
    }

    __declspec(dllexport) void L_TemporaryEffects_AddNullEffect(TemporaryEffects* effects, int nullId, bool addCostume, int count) {
        effects->AddNullEffect(nullId, addCostume, count);
    }

    __declspec(dllexport) void L_TemporaryEffects_AddTrinketEffect(TemporaryEffects* effects, int trinketType, bool addCostume, int count) {
        effects->AddTrinketEffect(trinketType, addCostume, count);
    }

    __declspec(dllexport) void L_TemporaryEffects_ClearEffects(TemporaryEffects* effects) {
        effects->ClearEffects();
    }

    __declspec(dllexport) const TemporaryEffect* L_TemporaryEffects_GetCollectibleEffect(TemporaryEffects* effects, int collectibleType) {
        return effects->GetCollectibleEffect(collectibleType);
    }

    __declspec(dllexport) int L_TemporaryEffects_GetCollectibleEffectNum(TemporaryEffects* effects, int collectibleType) {
        return effects->GetCollectibleEffectNum(collectibleType);
    }

    __declspec(dllexport) const TemporaryEffect* L_TemporaryEffects_GetNullEffect(TemporaryEffects* effects, int nullId) {
        return effects->GetNullEffect(nullId);
    }

    __declspec(dllexport) int L_TemporaryEffects_GetNullEffectNum(TemporaryEffects* effects, int nullId) {
        return effects->GetNullEffectNum(nullId);
    }

    __declspec(dllexport) const TemporaryEffect* L_TemporaryEffects_GetTrinketEffect(TemporaryEffects* effects, int trinketType) {
        return effects->GetTrinketEffect(trinketType);
    }

    __declspec(dllexport) int L_TemporaryEffects_GetTrinketEffectNum(TemporaryEffects* effects, int trinketType) {
        return effects->GetTrinketEffectNum(trinketType);
    }

    __declspec(dllexport) bool L_TemporaryEffects_HasCollectibleEffect(TemporaryEffects* effects, int collectibleType) {
        return effects->HasCollectibleEffect(collectibleType);
    }

    __declspec(dllexport) bool L_TemporaryEffects_HasNullEffect(TemporaryEffects* effects, int nullId) {
        return effects->HasNullEffect(nullId);
    }

    __declspec(dllexport) bool L_TemporaryEffects_HasTrinketEffect(TemporaryEffects* effects, int trinketType) {
        return effects->HasTrinketEffect(trinketType);
    }

    __declspec(dllexport) void L_TemporaryEffects_RemoveCollectibleEffect(TemporaryEffects* effects, int collectibleType, int count) {
        effects->RemoveCollectibleEffect(collectibleType, count);
    }

    __declspec(dllexport) void L_TemporaryEffects_RemoveNullEffect(TemporaryEffects* effects, int nullId, int count) {
        effects->RemoveNullEffect(nullId, count);
    }

    __declspec(dllexport) void L_TemporaryEffects_RemoveTrinketEffect(TemporaryEffects* effects, int trinketType, int count) {
        effects->RemoveTrinketEffect(trinketType, count);
    }
}