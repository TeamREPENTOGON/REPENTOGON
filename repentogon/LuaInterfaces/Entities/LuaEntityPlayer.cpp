#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"

#include "../../Patches/ASMPatches/ASMPlayer.h"
#include "../../Patches/CustomCache.h"
#include "../../Patches/ItemPoolManager.h"
#include "../../Patches/ExtraLives.h"
#include "../../Patches/EntityPlus.h"
#include "../../Patches/PlayerFeatures.h"
#include "../../Patches/ItemSpoofSystem.h"
#include "../../MiscFunctions.h"

#include <algorithm>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <vector>

MOD_EXPORT void L_EntityPlayer_AddMaxHearts(Entity_Player* player, int amount, bool ignoreKeeper) {
	player->AddMaxHearts(amount, ignoreKeeper);
}

MOD_EXPORT bool L_EntityPlayer_HasFullHearts(Entity_Player* player) {
	return player->HasFullHearts();
}

MOD_EXPORT void L_EntityPlayer_AddHearts(Entity_Player* player, int hearts, bool unk, bool unk2) {
	player->AddHearts(hearts, unk, unk2);
}

MOD_EXPORT void L_EntityPlayer_AddEternalHearts(Entity_Player* player, int amount) {
	player->AddEternalHearts(amount);
}

MOD_EXPORT void L_EntityPlayer_AddSoulHearts(Entity_Player* player, int amount, bool unk) {
	player->AddSoulHearts(amount, unk);
}

MOD_EXPORT void L_EntityPlayer_AddBlackHearts(Entity_Player* player, int amount) {
	player->AddBlackHearts(amount);
}

MOD_EXPORT void L_EntityPlayer_RemoveBlackHeart(Entity_Player* player, int heart) {
	player->RemoveBlackHeart(heart);
}

MOD_EXPORT bool L_EntityPlayer_IsBlackHeart(Entity_Player* player, int heart) {
	return player->IsBlackHeart(heart);
}

MOD_EXPORT void L_EntityPlayer_AddJarHearts(Entity_Player* player, int amount) {
	player->AddJarHearts(amount);
}

MOD_EXPORT void L_EntityPlayer_AddJarFlies(Entity_Player* player, int amount) {
	player->AddJarFlies(amount);
}

MOD_EXPORT void L_EntityPlayer_AddCoins(Entity_Player* player, int amount) {
	player->AddCoins(amount);
}

MOD_EXPORT void L_EntityPlayer_AddBombs(Entity_Player* player, int amount) {
	player->AddBombs(amount);
}

MOD_EXPORT void L_EntityPlayer_AddKeys(Entity_Player* player, int amount) {
	player->AddKeys(amount);
}

MOD_EXPORT void L_EntityPlayer_AddGoldenKey(Entity_Player* player) {
	player->AddGoldenKey();
}

MOD_EXPORT void L_EntityPlayer_RemoveGoldenKey(Entity_Player* player) {
	player->RemoveGoldenKey();
}

MOD_EXPORT void L_EntityPlayer_AddGoldenBomb(Entity_Player* player) {
	player->AddGoldenBomb();
}

MOD_EXPORT void L_EntityPlayer_RemoveGoldenBomb(Entity_Player* player) {
	player->RemoveGoldenBomb();
}

MOD_EXPORT void L_EntityPlayer_AddGoldenHearts(Entity_Player* player, int amount, bool unk) {
	player->AddGoldenHearts(amount, unk);
}

MOD_EXPORT void L_EntityPlayer_AddPrettyFly(Entity_Player* player) {
	player->AddPrettyFly();
}

MOD_EXPORT bool L_EntityPlayer_TryUseKey(Entity_Player* player) {
	return player->TryUseKey();
}

MOD_EXPORT void L_EntityPlayer_AddBoneHearts(Entity_Player* player, int amount) {
	player->AddBoneHearts(amount);
}

MOD_EXPORT void L_EntityPlayer_AddBrokenHearts(Entity_Player* player, int amount) {
	player->AddBrokenHearts(amount);
}

MOD_EXPORT void L_EntityPlayer_AddRottenHearts(Entity_Player* player, int amount, bool unk) {
	player->AddRottenHearts(amount, unk);
}

MOD_EXPORT void L_EntityPlayer_AddGigaBombs(Entity_Player* player, int amount) {
	player->AddGigaBombs(amount);
}

MOD_EXPORT void L_EntityPlayer_AddSoulCharge(Entity_Player* player, int amount) {
	player->AddSoulCharge(amount);
}

MOD_EXPORT void L_EntityPlayer_AddBloodCharge(Entity_Player* player, int amount) {
	player->AddBloodCharge(amount);
}

MOD_EXPORT int L_EntityPlayer_GetEffectiveSoulCharge(Entity_Player* player) {
	return player->GetEffectiveSoulCharge();
}

MOD_EXPORT int L_EntityPlayer_GetEffectiveBloodCharge(Entity_Player* player) {
	return player->GetEffectiveBloodCharge();
}

MOD_EXPORT int L_EntityPlayer_GetEffectiveMaxHearts(Entity_Player* player) {
	return player->GetEffectiveMaxHearts();
}

MOD_EXPORT int L_EntityPlayer_GetHeartLimit(Entity_Player* player, bool keeper) {
	return player->GetHealthLimit(keeper);
}

MOD_EXPORT void L_EntityPlayer_SetFullHearts(Entity_Player* player) {
	player->SetFullHearts();
}

MOD_EXPORT bool L_EntityPlayer_IsBoneHeart(Entity_Player* player, int heart) {
	return player->IsBoneHeart(heart);
}

MOD_EXPORT bool L_EntityPlayer_CanPickRedHearts(Entity_Player* player) {
	return player->CanPickRedHearts();
}

MOD_EXPORT bool L_EntityPlayer_CanPickSoulHearts(Entity_Player* player) {
	return player->CanPickSoulHearts();
}

MOD_EXPORT bool L_EntityPlayer_CanPickBlackHearts(Entity_Player* player) {
	return player->CanPickBlackHearts();
}

MOD_EXPORT bool L_EntityPlayer_CanPickGoldenHearts(Entity_Player* player) {
	return player->CanPickGoldenHearts();
}

MOD_EXPORT bool L_EntityPlayer_CanPickBoneHearts(Entity_Player* player) {
	return player->CanPickBoneHearts();
}

MOD_EXPORT bool L_EntityPlayer_CanPickRottenHearts(Entity_Player* player) {
	return player->CanPickRottenHearts();
}

MOD_EXPORT void L_EntityPlayer_ChangePlayerType(Entity_Player* player, int playerType, bool unk) {
	player->ChangePlayerType(playerType, unk);
}

MOD_EXPORT int L_EntityPlayer_GetExtraLives(Entity_Player* player) {
	return player->GetExtraLives();
}

MOD_EXPORT bool L_EntityPlayer_WillPlayerRevive(Entity_Player* player) {
	return player->WillPlayerRevive();
}

MOD_EXPORT void L_EntityPlayer_Revive(Entity_Player* player) {
	player->Revive();
}

MOD_EXPORT void L_EntityPlayer_DonateLuck(Entity_Player* player, int amount) {
	player->DonateLuck(amount);
}

MOD_EXPORT void L_EntityPlayer_AddCard(Entity_Player* player, int card) {
	player->AddCard(card);
}

MOD_EXPORT void L_EntityPlayer_AddPill(Entity_Player* player, int pill) {
	player->AddPill(pill);
}

MOD_EXPORT int L_EntityPlayer_GetCard(Entity_Player* player, int slot) {
	return player->GetCard(slot);
}

MOD_EXPORT int L_EntityPlayer_GetPill(Entity_Player* player, int slot) {
	return player->GetPill(slot);
}

MOD_EXPORT void L_EntityPlayer_SetCard(Entity_Player* player, int slot, int card) {
	player->SetCard(slot, card);
}

MOD_EXPORT void L_EntityPlayer_SetPill(Entity_Player* player, int slot, int color) {
	player->SetPill(slot, color);
}

MOD_EXPORT bool L_EntityPlayer_FlushQueueItem(Entity_Player* player) {
	return player->FlushQueueItem();
}

MOD_EXPORT int L_EntityPlayer_GetCollectibleCount(Entity_Player* player) {
	return player->GetCollectibleCount();
}

MOD_EXPORT void L_EntityPlayer_AddTrinket(Entity_Player* player, int trinket, bool firstTime) {
	player->AddTrinket(trinket, firstTime);
}

MOD_EXPORT bool L_EntityPlayer_TryRemoveTrinket(Entity_Player* player, int trinket) {
	return player->TryRemoveTrinket(trinket);
}

MOD_EXPORT int L_EntityPlayer_GetMaxTrinkets(Entity_Player* player) {
	return player->GetMaxTrinkets();
}

MOD_EXPORT void L_EntityPlayer_RemoveCollectible(Entity_Player* player, int collectible, bool ignoreModifiers, int slot, bool removeFromPlayerForm) {
	player->RemoveCollectible(collectible, ignoreModifiers, slot, removeFromPlayerForm);
}

MOD_EXPORT void L_EntityPlayer_ClearTemporaryEffects(Entity_Player* player) {
	player->ClearTemporaryEffects();
}

MOD_EXPORT bool L_EntityPlayer_HasPlayerForm(Entity_Player* player, int form) {
	return player->HasPlayerForm(form);
}

MOD_EXPORT bool L_EntityPlayer_CanAddCollectible(Entity_Player* player, int collectible) {
	return player->CanAddCollectible(collectible);
}

MOD_EXPORT bool L_EntityPlayer_TryHoldTrinket(Entity_Player* player, int trinket) {
	return player->TryHoldTrinket(trinket);
}

MOD_EXPORT void L_EntityPlayer_EvaluateItems(Entity_Player* player) {
	player->EvaluateItems();
}

MOD_EXPORT void L_EntityPlayer_RespawnFamiliars(Entity_Player* player) {
	player->RespawnFamiliars();
}

MOD_EXPORT bool L_EntityPlayer_HasWeaponType(Entity_Player* player, int weaponType) {
	return player->HasWeaponType(weaponType);
}

MOD_EXPORT void L_EntityPlayer_TryRemoveCollectibleCostume(Entity_Player* player, int collectible, bool unk) {
	player->TryRemoveCollectibleCostume(collectible, unk);
}

MOD_EXPORT void L_EntityPlayer_TryRemoveTrinketCostume(Entity_Player* player, int trinket) {
	player->TryRemoveTrinketCostume(trinket);
}

MOD_EXPORT void L_EntityPlayer_TryRemoveNullCostume(Entity_Player* player, int nullItem) {
	player->TryRemoveNullCostume(nullItem);
}

MOD_EXPORT void L_EntityPlayer_RemoveSkinCostume(Entity_Player* player) {
	player->RemoveSkinCostume();
}

MOD_EXPORT void L_EntityPlayer_ClearCostumes(Entity_Player* player) {
	player->ClearCostumes();
}

MOD_EXPORT void L_EntityPlayer_AddPlayerFormCostume(Entity_Player* player, int form) {
	player->AddPlayerFormCostume(form);
}

MOD_EXPORT void L_EntityPlayer_UseCard(Entity_Player* player, int card, unsigned int useFlags) {
	player->UseCard(card, useFlags);
}

MOD_EXPORT void L_EntityPlayer_UsePill(Entity_Player* player, int effect, int color, unsigned int useFlags) {
	player->UsePill(effect, color, useFlags);
}

MOD_EXPORT void L_EntityPlayer_TriggerBookOfVirtues(Entity_Player* player, int collectible, int charge) {
	player->TriggerBookOfVirtues(collectible, charge);
}

MOD_EXPORT void L_EntityPlayer_SwapActiveItems(Entity_Player* player) {
	player->SwapActiveItems();
}

MOD_EXPORT void L_EntityPlayer_ResetItemState(Entity_Player* player) {
	player->ResetItemState();
}

MOD_EXPORT bool L_EntityPlayer_HasTimedItem(Entity_Player* player) {
	return player->HasTimedItem();
}

MOD_EXPORT void L_EntityPlayer_AddDollarBillEffect(Entity_Player* player) {
	player->AddDollarBillEffect();
}

MOD_EXPORT void L_EntityPlayer_AddCurseMistEffect(Entity_Player* player) {
	player->AddCurseMistEffect();
}

MOD_EXPORT void L_EntityPlayer_RemoveCurseMistEffect(Entity_Player* player) {
	player->RemoveCurseMistEffect();
}

MOD_EXPORT void L_EntityPlayer_InitBabySkin(Entity_Player* player) {
	player->InitBabySkin();
}

MOD_EXPORT void L_EntityPlayer_UpdateCanShoot(Entity_Player* player) {
	player->UpdateCanShoot();
}

MOD_EXPORT void L_EntityPlayer_AddDeadEyeCharge(Entity_Player* player) {
	player->AddDeadEyeCharge();
}

MOD_EXPORT int L_EntityPlayer_GetZodiacEffect(Entity_Player* player) {
	return player->GetZodiacEffect();
}

MOD_EXPORT float L_EntityPlayer_GetGreedDonationBreakChance(Entity_Player* player) {
	return player->GetGreedDonationBreakChance();
}

MOD_EXPORT bool L_EntityPlayer_IsFullSpriteRendering(Entity_Player* player) {
	return player->IsFullSpriteRendering();
}

MOD_EXPORT void L_EntityPlayer_UsePoopSpell(Entity_Player* player, int spell) {
	player->UsePoopSpell(spell);
}

MOD_EXPORT int L_EntityPlayer_GetMaxPoopMana(Entity_Player* player) {
	return player->GetMaxPoopMana();
}

MOD_EXPORT void L_EntityPlayer_AddPoopMana(Entity_Player* player, int amount) {
	player->AddPoopMana(amount);
}

MOD_EXPORT int L_EntityPlayer_GetPoopSpell(Entity_Player* player, int slot) {
	return player->GetPoopSpell(slot);
}

MOD_EXPORT Entity* L_EntityPlayer_GetNPCTarget(Entity_Player* player) {
	return player->GetNPCTarget();
}

MOD_EXPORT Entity* L_EntityPlayer_GetActiveWeaponEntity(Entity_Player* player) {
	return player->GetActiveWeaponEntity();
}

MOD_EXPORT Entity_Player* L_EntityPlayer_GetMainTwin(Entity_Player* player) {
	return player->GetMainTwin();
}

MOD_EXPORT bool L_EntityPlayer_CanPickupItem(Entity_Player* player) {
	return player->CanPickupItem();
}

MOD_EXPORT bool L_EntityPlayer_IsHoldingItem(Entity_Player* player) {
	return player->IsHoldingItem();
}

MOD_EXPORT bool L_EntityPlayer_IsHeldItemVisible(Entity_Player* player) {
	return player->IsHeldItemVisible();
}

MOD_EXPORT bool L_EntityPlayer_TryHoldEntity(Entity_Player* player, Entity* entity) {
	return player->TryHoldEntity(entity);
}

MOD_EXPORT void L_EntityPlayer_SetShootingCooldown(Entity_Player* player, int cooldown) {
	player->SetShootingCooldown(cooldown);
}

MOD_EXPORT void L_EntityPlayer_SetMinDamageCooldown(Entity_Player* player, int cooldown) {
	player->SetMinDamageCooldown(cooldown);
}

MOD_EXPORT bool L_EntityPlayer_AreControlsEnabled(Entity_Player* player) {
	return player->AreControlsEnabled();
}

MOD_EXPORT void L_EntityPlayer_AnimateCollectible(Entity_Player* player, int collectible, const char* animName, const char* spriteAnimName) {
	player->AnimateCollectible(collectible, animName, spriteAnimName);
}

MOD_EXPORT void L_EntityPlayer_AnimateTrinket(Entity_Player* player, int trinket, const char* animName, const char* spriteAnimName) {
	player->AnimateTrinket(trinket, animName, spriteAnimName);
}

MOD_EXPORT void L_EntityPlayer_AnimateCard(Entity_Player* player, int card, const char* animName) {
	player->AnimateCard(card, animName);
}

MOD_EXPORT void L_EntityPlayer_AnimatePill(Entity_Player* player, int pill, const char* animName) {
	player->AnimatePill(pill, animName);
}

MOD_EXPORT void L_EntityPlayer_AnimateTrapdoor(Entity_Player* player) {
	player->AnimateTrapdoor();
}

MOD_EXPORT void L_EntityPlayer_AnimateLightTravel(Entity_Player* player) {
	player->AnimateLightTravel();
}

MOD_EXPORT void L_EntityPlayer_AnimateAppear(Entity_Player* player) {
	player->AnimateAppear();
}

MOD_EXPORT void L_EntityPlayer_AnimateTeleport(Entity_Player* player, bool unk) {
	player->AnimateTeleport(unk);
}

MOD_EXPORT void L_EntityPlayer_AnimateHappy(Entity_Player* player) {
	player->AnimateHappy();
}

MOD_EXPORT void L_EntityPlayer_AnimateSad(Entity_Player* player) {
	player->AnimateSad();
}

MOD_EXPORT void L_EntityPlayer_AnimatePitfallIn(Entity_Player* player, bool unk) {
	player->AnimatePitfallIn(unk);
}

MOD_EXPORT void L_EntityPlayer_AnimatePitfallOut(Entity_Player* player) {
	player->AnimatePitfallOut();
}

MOD_EXPORT void L_EntityPlayer_PlayExtraAnimation(Entity_Player* player, const char* animName) {
	player->PlayExtraAnimation(animName);
}

MOD_EXPORT void L_EntityPlayer_QueueExtraAnimation(Entity_Player* player, const char* animName) {
	player->QueueExtraAnimation(animName);
}

MOD_EXPORT Entity* L_EntityPlayer_AddBlueFlies(Entity_Player* player, int amount, Vector* position, Entity* target) {
	return player->AddBlueFlies(amount, position, target);
}

MOD_EXPORT Entity* L_EntityPlayer_AddBlueSpider(Entity_Player* player, Vector* position) {
	return player->AddBlueSpider(position);
}

MOD_EXPORT Entity_Familiar* L_EntityPlayer_AddFriendlyDip(Entity_Player* player, int subtype, Vector* position) {
	return player->AddFriendlyDip(subtype, position);
}

MOD_EXPORT Entity_Familiar* L_EntityPlayer_AddItemWisp(Entity_Player* player, int collectible, Vector* position, bool adjustOrbitLayer) {
	return player->AddItemWisp((CollectibleType)collectible, *position, adjustOrbitLayer);
}

MOD_EXPORT Entity_Familiar* L_EntityPlayer_AddMinisaac(Entity_Player* player, Vector* position, bool playAnim) {
	return player->AddMinisaac(position, playAnim);
}

MOD_EXPORT Entity_Familiar* L_EntityPlayer_AddSwarmFlyOrbital(Entity_Player* player, Vector* position) {
	return player->AddSwarmFlyOrbital(position);
}

MOD_EXPORT Entity_Familiar* L_EntityPlayer_AddWisp(Entity_Player* player, int collectible, Vector* position, bool adjustOrbitLayer, bool dontUpdate) {
	return player->AddWisp((CollectibleType)collectible, position, adjustOrbitLayer, dontUpdate);
}

MOD_EXPORT void L_EntityPlayer_DoZitEffect(Entity_Player* player, Vector* direction) {
	player->DoZitEffect(direction);
}

MOD_EXPORT void L_EntityPlayer_DropPocketItem(Entity_Player* player, int slot, Vector* position) {
	player->DropPocketItem(slot, position);
}

MOD_EXPORT void L_EntityPlayer_DropTrinket(Entity_Player* player, Vector* position, bool replaceTick) {
	player->DropTrinket(position, replaceTick);
}

MOD_EXPORT Entity_Bomb* L_EntityPlayer_FireBomb(Entity_Player* player, Vector* position, Vector* velocity, Entity* source) {
	return player->FireBomb(position, velocity, source);
}

MOD_EXPORT Entity_Knife* L_EntityPlayer_FireKnife(Entity_Player* player, Entity* parent, float rotationOffset, bool cantOverwrite, int subType, int variant) {
	return player->FireKnife(parent, variant, rotationOffset, cantOverwrite, subType);
}

MOD_EXPORT Entity_Laser* L_EntityPlayer_FireDelayedBrimstone(Entity_Player* player, float angle, Entity* source) {
	return player->FireDelayedBrimstone(angle, source);
}

MOD_EXPORT Entity_Laser* L_EntityPlayer_SpawnMawOfVoid(Entity_Player* player, int timeout) {
	return player->SpawnMawOfVoid(timeout);
}

MOD_EXPORT Entity_Laser* L_EntityPlayer_FireBrimstone(Entity_Player* player, Vector* direction, Entity* source, float damageMultiplier) {
	return player->FireBrimstone(direction, source, damageMultiplier);
}

MOD_EXPORT Entity_Laser* L_EntityPlayer_FireTechLaser(Entity_Player* player, Vector* position, int offsetID, Vector* direction, bool leftEye, bool oneHit, Entity* source, float damageMultiplier) {
	return player->FireTechLaser(*position, offsetID, *direction, leftEye, oneHit, source, damageMultiplier);
}

MOD_EXPORT Entity_Laser* L_EntityPlayer_FireTechXLaser(Entity_Player* player, Vector* position, Vector* direction, float radius, Entity* source, float damageMultiplier) {
	return player->FireTechXLaser(*position, *direction, radius, source, damageMultiplier);
}

MOD_EXPORT Entity* L_EntityPlayer_ThrowBlueSpider(Entity_Player* player, Vector* position, Vector* target) {
	return player->ThrowBlueSpider(position, target);
}

MOD_EXPORT Entity* L_EntityPlayer_ThrowHeldEntity(Entity_Player* player, Vector* velocity) {
	return player->ThrowHeldEntity(velocity);
}

MOD_EXPORT void L_EntityPlayer_GetFlyingOffset(Entity_Player* player, Vector* out) {
	player->GetFlyingOffset(out);
}

MOD_EXPORT void L_EntityPlayer_GetLaserOffset(Entity_Player* player, int laserOffsetID, Vector* direction, Vector* out) {
	player->GetLaserOffset(out, laserOffsetID, direction);
}

MOD_EXPORT void L_EntityPlayer_GetMovementJoystick(Entity_Player* player, Vector* out) {
	player->GetMovementJoystick(out);
}

MOD_EXPORT void L_EntityPlayer_GetShootingJoystick(Entity_Player* player, Vector* out) {
	player->GetShootingJoystick(out);
}

MOD_EXPORT void L_EntityPlayer_GetTearMovementInheritance(Entity_Player* player, Vector* shotDirection, Vector* out) {
	player->GetTearMovementInheritance(out, shotDirection, false);
}

MOD_EXPORT void L_EntityPlayer_GetBodyMoveDirection(Entity_Player* player, Vector* out) {
	player->GetBodyMoveDirection(out);
}

MOD_EXPORT void L_EntityPlayer_GetEnterPosition(Entity_Player* player, Vector* out) {
	player->GetEnterPosition(out);
}

MOD_EXPORT bool L_EntityPlayer_IsPosInSpotLight(Entity_Player* player, Vector* position) {
	return player->IsPosInSpotLight(position);
}

MOD_EXPORT void L_EntityPlayer_RenderBody(Entity_Player* player, Vector* position) {
	player->RenderBody(position);
}

MOD_EXPORT void L_EntityPlayer_RenderGlow(Entity_Player* player, Vector* position) {
	player->RenderGlow(position);
}

MOD_EXPORT void L_EntityPlayer_RenderHead(Entity_Player* player, Vector* position) {
	player->RenderHead(position);
}

MOD_EXPORT void L_EntityPlayer_RenderTop(Entity_Player* player, Vector* position) {
	player->RenderTop(position);
}

MOD_EXPORT void L_EntityPlayer_Teleport(Entity_Player* player, Vector* position, bool doEffects, bool teleportTwinPlayers) {
	player->Teleport(position, doEffects, teleportTwinPlayers);
}

MOD_EXPORT void L_EntityPlayer_SpawnClot(Entity_Player* player, Vector* position, bool canKillPlayer) {
	player->SpawnClot(position, canKillPlayer);
}

MOD_EXPORT bool L_EntityPlayer_TryForgottenThrow(Entity_Player* player, Vector* direction) {
	return player->TryForgottenThrow(direction);
}

MOD_EXPORT void L_EntityPlayer_AddCollectible(Entity_Player* player, int collectible, int charge, bool firstTime, int slot, int varData, int pool) {
	player->AddCollectible(collectible, charge, firstTime, slot, varData, pool);
}

MOD_EXPORT void L_EntityPlayer_AddCostume(Entity_Player* player, ItemConfig_Item* item, bool itemStateOnly) {
	player->AddCostume(item, itemStateOnly);
}

MOD_EXPORT void L_EntityPlayer_CheckFamiliar(Entity_Player* player, unsigned int variant, unsigned int targetCount, RNG* rng, ItemConfig_Item* item, int subType) {
	player->CheckFamiliar(variant, targetCount, rng, item, subType);
}

MOD_EXPORT void L_EntityPlayer_RemoveCostume(Entity_Player* player, ItemConfig_Item* item) {
	player->RemoveCostume(item);
}

MOD_EXPORT Entity_Player* L_EntityPlayer_InitTwin(Entity_Player* player, int playerType) {
	return player->InitTwin(playerType);
}

MOD_EXPORT void L_EntityPlayer_InitPostLevelInitStats(Entity_Player* player) {
	player->InitPostLevelInitStats();
}

MOD_EXPORT void L_EntityPlayer_SetItemState(Entity_Player* player, int collectible) {
	player->SetItemState((CollectibleType)collectible);
}

MOD_EXPORT int L_EntityPlayer_GetHealthType(Entity_Player* player) {
	return player->GetHealthType();
}

MOD_EXPORT int L_EntityPlayer_GetTotalActiveCharge(Entity_Player* player, int slot) {
	return player->GetTotalActiveCharge(slot);
}

MOD_EXPORT int L_EntityPlayer_GetActiveMaxCharge(Entity_Player* player, int slot) {
	return player->GetActiveMaxCharge(slot);
}

MOD_EXPORT int L_EntityPlayer_GetActiveMinUsableCharge(Entity_Player* player, int slot) {
	return player->GetActiveMinUsableCharge(slot);
}

MOD_EXPORT void L_EntityPlayer_SetActiveVarData(Entity_Player* player, int varData, int slot) {
	player->SetActiveVarData(varData, slot);
}

MOD_EXPORT int L_EntityPlayer_GetActiveItem(Entity_Player* player, int slot) {
	return player->GetActiveItem(slot);
}

MOD_EXPORT int L_EntityPlayer_GetActiveCharge(Entity_Player* player, int slot) {
	return player->GetActiveCharge(slot);
}

MOD_EXPORT int L_EntityPlayer_GetActiveSubCharge(Entity_Player* player, int slot) {
	return player->GetActiveSubCharge(slot);
}

MOD_EXPORT int L_EntityPlayer_GetBatteryCharge(Entity_Player* player, int slot) {
	return player->GetBatteryCharge(slot);
}

MOD_EXPORT bool L_EntityPlayer_NeedsCharge(Entity_Player* player, int slot) {
	return player->NeedsCharge(slot);
}

MOD_EXPORT void L_EntityPlayer_SetActiveCharge(Entity_Player* player, int charge, int slot) {
	player->SetActiveCharge(charge, slot);
}

MOD_EXPORT int L_EntityPlayer_AddActiveCharge(Entity_Player* player, int charge, int slot, bool flashHUD, bool overcharge, bool force) {
	return player->AddActiveCharge(charge, slot, flashHUD, overcharge, force);
}

MOD_EXPORT bool L_EntityPlayer_FullCharge(Entity_Player* player, int slot, bool force) {
	return player->FullCharge(slot, force);
}

MOD_EXPORT void L_EntityPlayer_DischargeActiveItem(Entity_Player* player, int slot) {
	player->DischargeActiveItem(slot, false);
}

MOD_EXPORT bool L_EntityPlayer_CanOverrideActiveItem(Entity_Player* player, int slot) {
	return player->CanOverrideActiveItem(slot);
}

MOD_EXPORT int L_EntityPlayer_GetActiveItemSlot(Entity_Player* player, int collectible) {
	return player->GetActiveItemSlot(collectible);
}

MOD_EXPORT void L_EntityPlayer_IncrementPlayerFormCounter(Entity_Player* player, int form, int amount) {
	player->IncrementPlayerFormCounter(form, amount);
}

MOD_EXPORT bool L_EntityPlayer_TryPreventDeath(Entity_Player* player) {
	return player->TryPreventDeath();
}

MOD_EXPORT void L_EntityPlayer_RemoveCollectibleByHistoryIndex(Entity_Player* player, int index) {
	player->RemoveCollectibleByHistoryIndex(index, true);
}

MOD_EXPORT bool L_EntityPlayer_TryFakeDeath(Entity_Player* player) {
	return player->TryFakeDeath();
}

MOD_EXPORT int L_EntityPlayer_GetWeaponModifiers(Entity_Player* player) {
	return player->GetWeaponModifiers();
}

MOD_EXPORT void L_EntityPlayer_EnableWeaponType(Entity_Player* player, int weaponType, bool set) {
	player->EnableWeaponType((WeaponType)weaponType, set);
}

MOD_EXPORT void L_EntityPlayer_TriggerRoomClear(Entity_Player* player) {
	player->TriggerRoomClear();
}

MOD_EXPORT void L_EntityPlayer_UpdateIsaacPregnancy(Entity_Player* player, bool cambion) {
	player->UpdateIsaacPregnancy(cambion);
}

MOD_EXPORT int L_EntityPlayer_GetCambionPregnancyLevel(Entity_Player* player) {
	return player->GetCambionPregnancyLevel();
}

MOD_EXPORT bool L_EntityPlayer_SwapForgottenForm(Entity_Player* player, bool ignoreHealth, bool noEffects) {
	return player->SwapForgottenForm(ignoreHealth, noEffects);
}

MOD_EXPORT void L_EntityPlayer_PlayDelayedSFX(Entity_Player* player, unsigned int soundEffectID, int soundDelay, int frameDelay, float volume) {
	player->PlayDelayedSFX(soundEffectID, soundDelay, frameDelay, volume);
}

MOD_EXPORT bool L_EntityPlayer_CanUsePill(Entity_Player* player, int pillEffect) {
	return player->CanUsePill(pillEffect);
}

MOD_EXPORT unsigned int L_EntityPlayer_GetMaxPocketItems(Entity_Player* player) {
	return player->GetMaxPocketItems();
}

MOD_EXPORT bool L_EntityPlayer_CanAddCollectibleToInventory(Entity_Player* player, int collectible) {
	return player->CanAddCollectibleToInventory(collectible);
}

MOD_EXPORT void L_EntityPlayer_AddLeprosy(Entity_Player* player) {
	player->AddLeprosy();
}

MOD_EXPORT void L_EntityPlayer_AddUrnSouls(Entity_Player* player, unsigned int amount) {
	player->AddUrnSouls(amount);
}

MOD_EXPORT const char* L_EntityPlayer_GetDeathAnimName(Entity_Player* player) {
	return player->GetDeathAnimName();
}

MOD_EXPORT unsigned int L_EntityPlayer_GetGlitchBabySubType(Entity_Player* player) {
	return player->GetGlitchBabySubType();
}

MOD_EXPORT int L_EntityPlayer_GetGreedsGulletHearts(Entity_Player* player) {
	return player->GetGreedsGulletHearts();
}

MOD_EXPORT bool L_EntityPlayer_CanCrushRocks(Entity_Player* player) {
	return player->CanCrushRocks();
}

MOD_EXPORT bool L_EntityPlayer_HasInstantDeathCurse(Entity_Player* player) {
	return player->HasInstantDeathCurse();
}

MOD_EXPORT bool L_EntityPlayer_HasPoisonImmunity(Entity_Player* player) {
	return player->HasPoisonImmunity();
}

MOD_EXPORT bool L_EntityPlayer_IsEntityValidTarget(Entity_Player* player, Entity* target) {
	return player->IsEntityValidTarget(target);
}

MOD_EXPORT bool L_EntityPlayer_IsHeadless(Entity_Player* player) {
	return player->IsHeadless();
}

MOD_EXPORT bool L_EntityPlayer_IsHologram(Entity_Player* player) {
	return player->IsHologram();
}

MOD_EXPORT bool L_EntityPlayer_IsInvisible(Entity_Player* player) {
	return player->IsInvisible();
}

MOD_EXPORT void L_EntityPlayer_MorphToCoopGhost(Entity_Player* player) {
	player->MorphToCoopGhost();
}

MOD_EXPORT void L_EntityPlayer_ResetPlayer(Entity_Player* player) {
	player->ResetPlayer();
}

MOD_EXPORT void L_EntityPlayer_SetControllerIndex(Entity_Player* player, int index, bool includePlayerOwned) {
	player->SetControllerIndex(index, includePlayerOwned);
}

MOD_EXPORT void L_EntityPlayer_SyncConsumableCounts(Entity_Player* player, Entity_Player* other, int flags) {
	player->SyncConsumableCounts(other, flags);
}

MOD_EXPORT bool L_EntityPlayer_TryAddToBagOfCrafting(Entity_Player* player, Entity_Pickup* pickup) {
	return player->TryAddToBagOfCrafting(pickup);
}

MOD_EXPORT void L_EntityPlayer_TryDecreaseGlowingHourglassUses(Entity_Player* player, int unk1, bool unk2) {
	player->TryDecreaseGlowingHourglassUses(unk1, unk2);
}

MOD_EXPORT void L_EntityPlayer_TryRemoveSmeltedTrinket(Entity_Player* player, unsigned int trinket) {
	player->TryRemoveSmeltedTrinket(trinket);
}

MOD_EXPORT bool L_EntityPlayer_VoidHasCollectible(Entity_Player* player, int collectible) {
	return player->VoidHasCollectible(collectible);
}

MOD_EXPORT bool L_EntityPlayer_PlayItemNullAnimation(Entity_Player* player, const char* animName) {
	return player->PlayItemNullAnimation(animName);
}

MOD_EXPORT void L_EntityPlayer_ClearQueueItem(Entity_Player* player) {
	player->ClearQueueItem();
}

MOD_EXPORT void L_EntityPlayer_RemovePocketItem(Entity_Player* player, int slot) {
	player->RemovePocketItem(slot);
}

MOD_EXPORT bool L_EntityPlayer_IsFootstepFrame(Entity_Player* player, int foot) {
	return player->IsFootstepFrame(foot);
}

MOD_EXPORT PocketItem* L_EntityPlayer_GetPocketItem(Entity_Player* player, int slot) {
	return player->GetPocketItem(slot);
}

MOD_EXPORT History* L_EntityPlayer_GetHistory(Entity_Player* player) {
	return player->GetHistory();
}

MOD_EXPORT RNG* L_EntityPlayer_GetCardRNG(Entity_Player* player, int id) {
	return player->GetCardRNG(id);
}

MOD_EXPORT RNG* L_EntityPlayer_GetCollectibleRNG(Entity_Player* player, int id) {
	return player->GetCollectibleRNG(id);
}

MOD_EXPORT RNG* L_EntityPlayer_GetPillRNG(Entity_Player* player, int id) {
	return player->GetPillRNG(id);
}

MOD_EXPORT RNG* L_EntityPlayer_GetTrinketRNG(Entity_Player* player, int id) {
	return player->GetTrinketRNG(id);
}

MOD_EXPORT void L_EntityPlayer_GetBombFlags(Entity_Player* player, bool isFetus, BitSet128* out) {
	player->GetBombFlags(out, isFetus);
}

MOD_EXPORT ActiveItemDesc* L_EntityPlayer_GetActiveItemDesc(Entity_Player* player, int slot) {
	return player->GetActiveItemDesc(slot);
}

MOD_EXPORT Entity* L_EntityPlayer_GetFocusEntity(Entity_Player* player) {
	return player->GetFocusEntity();
}

MOD_EXPORT unsigned int L_EntityPlayer_SpawnSaturnusTears(Entity_Player* player) {
	return player->SpawnSaturnusTears();
}

MOD_EXPORT void L_EntityPlayer_SetPocketActiveItem(Entity_Player* player, int collectible, int slot, bool keepInPools) {
	player->SetPocketActiveItem(collectible, slot, keepInPools);
}

MOD_EXPORT void L_EntityPlayer_QueueItemEx(Entity_Player* player, ItemConfig_Item* item, int charge, int flags, int varData) {
	player->QueueItem(item, charge, flags, varData, 0);
}

MOD_EXPORT Entity_Tear* L_EntityPlayer_FireTearEx(Entity_Player* player, Vector* position, Vector* velocity, int flags, Entity* source, float damageMultiplier) {
	return player->FireTear(position, *velocity, flags, source, damageMultiplier);
}

MOD_EXPORT void L_EntityPlayer_ClearDeadEyeChargeNative(Entity_Player* player) {
	player->ClearDeadEyeCharge();
}

MOD_EXPORT bool L_EntityPlayer_IsItemCostumeVisibleEx(Entity_Player* player, ItemConfig_Item* item, int layerId) {
	return player->IsItemCostumeVisible(item, layerId);
}

MOD_EXPORT bool L_EntityPlayer_IsCollectibleCostumeVisibleEx(Entity_Player* player, int collectible, int layerId) {
	return player->IsCollectibleCostumeVisible((CollectibleType)collectible, layerId);
}

MOD_EXPORT bool L_EntityPlayer_IsNullItemCostumeVisibleEx(Entity_Player* player, int nullItem, int layerId) {
	return player->IsNullItemCostumeVisible(nullItem, layerId);
}

MOD_EXPORT float L_EntityPlayer_GetFireDelayNative(Entity_Player* player) {
	return player->GetFireDelay();
}

MOD_EXPORT void L_EntityPlayer_SetFireDelayNative(Entity_Player* player, float delay) {
	player->SetFireDelay(delay);
}

// ---- structure access

MOD_EXPORT EntityConfig_Player* L_EntityPlayer_GetEntityConfigPlayer(Entity_Player* player) {
	return g_Manager->GetEntityConfig()->GetPlayer(player->GetPlayerType());
}

MOD_EXPORT Weapon* L_EntityPlayer_GetWeapon(Entity_Player* player, int index) {
	return *player->GetWeapon(index);
}

MOD_EXPORT void L_EntityPlayer_SetWeapon(Entity_Player* player, Weapon* weapon, int index) {
	*player->GetWeapon(index) = weapon;
}

MOD_EXPORT bool L_EntityPlayer_GetActiveWeaponNumFired(Entity_Player* player, int* numFired) {
	Weapon* weapon = *player->GetWeapon(0);
	if (!weapon) {
		weapon = *player->GetWeapon(1);
		if (!weapon) {
			return false;
		}
	}
	*numFired = weapon->GetNumFired();
	return true;
}

MOD_EXPORT TemporaryEffects* L_EntityPlayer_GetEffects(Entity_Player* player) {
	return &player->_temporaryeffects;
}

MOD_EXPORT ANM2* L_EntityPlayer_GetBodySprite(Entity_Player* player) {
	return &player->_bodySprite;
}

MOD_EXPORT ANM2* L_EntityPlayer_GetBloodGushSprite(Entity_Player* player) {
	return &player->_bloodGushSprite;
}

MOD_EXPORT ANM2* L_EntityPlayer_GetHeldSprite(Entity_Player* player) {
	return player->GetHeldSprite();
}

MOD_EXPORT EntityRef* L_EntityPlayer_GetLastDamageSource(Entity_Player* player) {
	return &player->_lastDamageSource;
}

MOD_EXPORT std::vector<EntitySaveState>* L_EntityPlayer_GetMovingBoxContents(Entity_Player* player) {
	return player->GetMovingBoxContents();
}

MOD_EXPORT int L_EntityPlayer_GetPlayerFormCounter(Entity_Player* player, int form) {
	return player->_playerForms[form];
}

MOD_EXPORT void L_EntityPlayer_GetCostumeLayer(Entity_Player* player, int index, int* costumeIndex, int* layerID, int* priority, bool* isBodyLayer) {
	const PlayerCostumeMap& map = player->_playerCostumeMap[index];
	*costumeIndex = map._index;
	*layerID = map._layerID;
	*priority = map._priority;
	*isBodyLayer = map._isBodyLayer;
}

MOD_EXPORT int L_EntityPlayer_GetHeartStatUp(Entity_Player* player, bool soulLocket, int index) {
	return soulLocket ? player->_soulLocketStatUps[index] : player->_candyHeartStatUps[index];
}

MOD_EXPORT int L_EntityPlayer_GetInventoryHistoryIndex(Entity_Player* player, int slot) {
	return player->_inventoryHistoryIdx[slot];
}

MOD_EXPORT int L_EntityPlayer_GetInventoryCollectible(Entity_Player* player, int slot) {
	return player->GetHistory()->_historyItems[player->_inventoryHistoryIdx[slot]]._itemID;
}

MOD_EXPORT int L_EntityPlayer_GetMaxInventorySize(Entity_Player* player) {
	if (player->_playerType != ePlayerType::PLAYER_ISAAC_B) {
		return 0;
	}
	return player->GetMaxInventorySize();
}

MOD_EXPORT PlayerHUD* L_EntityPlayer_GetPlayerHUD(Entity_Player* player) {
	PlayerHUD* playerhud = player->_playerHUD;  // Strawman etc have this

	if (!playerhud) {
		for (int i = 0; i < 8; i++) {
			PlayerHUD* phudi = g_Game->GetHUD()->GetPlayerHUD(i);
			if (phudi && phudi->GetPlayer() == player) {
				playerhud = phudi;
				break;
			}
		}
	}

	return playerhud;
}

// ---- items that need the spoof system

MOD_EXPORT bool L_EntityPlayer_HasCollectible(Entity_Player* player, int collectible, bool ignoreModifiers, bool ignoreSpoof) {
	ItemSpoofSystem::StartLuaRequest(ignoreSpoof);
	return player->HasCollectible(collectible, ignoreModifiers);
}

MOD_EXPORT int L_EntityPlayer_GetCollectibleNum(Entity_Player* player, int collectible, bool onlyCountTrueItems, bool ignoreSpoof) {
	ItemSpoofSystem::StartLuaRequest(ignoreSpoof);
	return player->GetCollectibleNum(collectible, onlyCountTrueItems);
}

MOD_EXPORT bool L_EntityPlayer_HasTrinket(Entity_Player* player, unsigned int trinket, bool ignoreModifiers, bool ignoreSpoof) {
	ItemSpoofSystem::StartLuaRequest(ignoreSpoof);
	if (!ignoreModifiers) {
		// HasTrinket will defer to GetTrinketMultiplier instead, which messes up the ItemSpoofSystem context.
		// Directly call it ourselves instead.
		return player->GetTrinketMultiplier(trinket) > 0;
	}
	return player->HasTrinket(trinket, ignoreModifiers);
}

MOD_EXPORT int L_EntityPlayer_GetTrinketMultiplier(Entity_Player* player, unsigned int trinket, bool ignoreSpoof) {
	ItemSpoofSystem::StartLuaRequest(ignoreSpoof);
	return player->GetTrinketMultiplier(trinket);
}

MOD_EXPORT bool L_EntityPlayer_HasGoldenTrinket(Entity_Player* player, unsigned int trinket, bool ignoreSpoof) {
	ItemSpoofSystem::StartLuaRequest(ignoreSpoof);
	return player->HasGoldenTrinket(trinket);
}

MOD_EXPORT bool L_EntityPlayer_BlockCollectible(Entity_Player* player, int collectible) {
	if (!ItemConfig::IsValidCollectible(collectible)) {
		return false;
	}
	if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player)) {
		playerPlus->itemSpoofs.GetCollectibleSpoof().Block(*player, collectible);
	}
	return true;
}

MOD_EXPORT bool L_EntityPlayer_UnblockCollectible(Entity_Player* player, int collectible) {
	if (!ItemConfig::IsValidCollectible(collectible)) {
		return false;
	}
	if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player)) {
		playerPlus->itemSpoofs.GetCollectibleSpoof().Unblock(*player, collectible);
	}
	return true;
}

MOD_EXPORT bool L_EntityPlayer_IsCollectibleBlocked(Entity_Player* player, int collectible) {
	if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player)) {
		return playerPlus->itemSpoofs.GetCollectibleSpoof().IsBlocked(collectible);
	}
	return false;
}

MOD_EXPORT bool L_EntityPlayer_BlockTrinket(Entity_Player* player, int trinket) {
	if (!ItemConfig::IsValidTrinket(trinket)) {
		return false;
	}
	if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player)) {
		playerPlus->itemSpoofs.GetTrinketSpoof().Block(*player, trinket);
	}
	return true;
}

MOD_EXPORT bool L_EntityPlayer_UnblockTrinket(Entity_Player* player, int trinket) {
	if (!ItemConfig::IsValidTrinket(trinket)) {
		return false;
	}
	if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player)) {
		playerPlus->itemSpoofs.GetTrinketSpoof().Unblock(*player, trinket);
	}
	return true;
}

MOD_EXPORT bool L_EntityPlayer_IsTrinketBlocked(Entity_Player* player, int trinket) {
	if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player)) {
		return playerPlus->itemSpoofs.GetTrinketSpoof().IsBlocked(trinket);
	}
	return false;
}

static ItemSpoofSystem::ItemSpoof* GetSpoof(Entity_Player* player, bool trinket) {
	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);
	if (!playerPlus) {
		return nullptr;
	}
	if (trinket) {
		return &playerPlus->itemSpoofs.GetTrinketSpoof();
	}
	return &playerPlus->itemSpoofs.GetCollectibleSpoof();
}

MOD_EXPORT bool L_EntityPlayer_IsValidInnateItem(bool trinket, int id) {
	return trinket ? ItemConfig::IsValidTrinket(id) : ItemConfig::IsValidCollectible(id);
}

MOD_EXPORT void L_EntityPlayer_AddInnateItem(Entity_Player* player, bool trinket, int id, int amount, const char* groupKey, int duration, bool addCostume) {
	if (ItemSpoofSystem::ItemSpoof* spoof = GetSpoof(player, trinket)) {
		spoof->AddInnate(*player, id, amount, groupKey, duration, addCostume);
	}
}

MOD_EXPORT void L_EntityPlayer_RemoveInnateItemLegacy(Entity_Player* player, int id, int amount) {
	if (ItemSpoofSystem::ItemSpoof* spoof = GetSpoof(player, false)) {
		spoof->RemoveInnate(*player, id, amount, "", true);
	}
}

MOD_EXPORT int L_EntityPlayer_RemoveInnateItem(Entity_Player* player, bool trinket, int id, int amount, const char* groupKey) {
	if (ItemSpoofSystem::ItemSpoof* spoof = GetSpoof(player, trinket)) {
		return spoof->RemoveInnate(*player, id, amount, groupKey, false);
	}
	return 0;
}

MOD_EXPORT int L_EntityPlayer_GetInnateItemCount(Entity_Player* player, bool trinket, int id, const char* groupKey) {
	if (ItemSpoofSystem::ItemSpoof* spoof = GetSpoof(player, trinket)) {
		return spoof->GetInnateCount(id, groupKey);
	}
	return 0;
}

MOD_EXPORT int L_EntityPlayer_SetInnateItemCount(Entity_Player* player, bool trinket, int id, int count, const char* groupKey, bool addCostume) {
	if (ItemSpoofSystem::ItemSpoof* spoof = GetSpoof(player, trinket)) {
		return spoof->SetInnateCount(*player, id, count, groupKey, addCostume);
	}
	return 0;
}

// Lists are copied here and read back one entry at a time
static std::vector<std::pair<int, int>> s_pairs;
static std::vector<std::array<int, 3>> s_triples;

MOD_EXPORT int L_EntityPlayer_SnapshotInnateGroup(Entity_Player* player, bool trinket, const char* groupKey) {
	s_pairs.clear();
	if (ItemSpoofSystem::ItemSpoof* spoof = GetSpoof(player, trinket)) {
		for (const auto& [id, count] : spoof->GetInnateGroupCounts(groupKey)) {
			s_pairs.emplace_back(id, count);
		}
	}
	return (int)s_pairs.size();
}

MOD_EXPORT void L_EntityPlayer_SetInnateGroup(Entity_Player* player, bool trinket, const char* groupKey, const int* ids, const int* counts, int length, bool addCostumes) {
	std::unordered_map<int, int> map;
	for (int i = 0; i < length; i++) {
		map[ids[i]] = counts[i];
	}
	if (ItemSpoofSystem::ItemSpoof* spoof = GetSpoof(player, trinket)) {
		spoof->SetInnateGroup(*player, map, groupKey, addCostumes);
	}
}

MOD_EXPORT void L_EntityPlayer_ClearInnateItemGroup(Entity_Player* player, const char* groupKey) {
	if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player)) {
		playerPlus->itemSpoofs.GetCollectibleSpoof().ClearItemGroup(*player, groupKey);
		playerPlus->itemSpoofs.GetTrinketSpoof().ClearItemGroup(*player, groupKey);
	}
}

MOD_EXPORT int L_EntityPlayer_SnapshotSpoofedCollectibles(Entity_Player* player) {
	s_triples.clear();
	if (ItemSpoofSystem::ItemSpoof* spoof = GetSpoof(player, false)) {
		std::unordered_set<int> spoofedIDs = spoof->GetBlockedIDs();
		for (const auto& [id, count] : spoof->GetInnateTotalCounts()) {
			spoofedIDs.insert(id);
		}
		for (const int id : spoofedIDs) {
			s_triples.push_back({ id, spoof->GetInnateCount(id), spoof->IsBlocked(id) ? 1 : 0 });
		}
	}
	return (int)s_triples.size();
}

MOD_EXPORT void L_EntityPlayer_GetSpoofedCollectible(int index, int* id, int* appendedCount, bool* isBlocked) {
	*id = s_triples[index][0];
	*appendedCount = s_triples[index][1];
	*isBlocked = s_triples[index][2] != 0;
}

MOD_EXPORT void L_EntityPlayer_GetSnapshotPair(int index, int* first, int* second) {
	*first = s_pairs[index].first;
	*second = s_pairs[index].second;
}

MOD_EXPORT int L_EntityPlayer_SnapshotWispCollectibles(Entity_Player* player) {
	s_pairs.clear();
	for (const auto& item : player->_itemWispsList) {
		s_pairs.emplace_back(item.first, item.second);
	}
	return (int)s_pairs.size();
}

// ---- spawning

MOD_EXPORT Entity_Familiar* L_EntityPlayer_AddBoneOrbital(Entity_Player* player, Vector* position) {
	Entity* orbital = player->AddBoneOrbital(position);
	return orbital ? orbital->ToFamiliar() : nullptr;
}

MOD_EXPORT Entity_Effect* L_EntityPlayer_FireBrimstoneBall(Entity_Player* player, Vector* position, Vector* velocity, Vector* offset) {
	return player->FireBrimstoneBall(*position, *velocity, offset ? *offset : Vector(), 0, 0, nullptr);
}

MOD_EXPORT Entity_Effect* L_EntityPlayer_ShootRedCandle(Entity_Player* player, Vector* direction) {
	Entity* flame = player->ShootRedCandle(direction);
	return flame ? flame->ToEffect() : nullptr;
}

MOD_EXPORT Entity_Effect* L_EntityPlayer_ShootBlueCandle(Entity_Player* player, Vector* direction) {
	Entity* flame = player->ShootBlueCandle(direction);
	return flame ? flame->ToEffect() : nullptr;
}

MOD_EXPORT Entity_Familiar* L_EntityPlayer_ThrowFriendlyDip(Entity_Player* player, int subtype, Vector* position, Vector* target) {
	Vector targetPosition = target ? *target : Vector(0, 0);
	return Entity_Player::ThrowFriendlyDip(subtype, position, player, &targetPosition);
}

MOD_EXPORT Entity_Pickup* L_EntityPlayer_DropCollectible(Entity_Player* player, int collectible, Entity_Pickup* pickup, bool removeFromForm) {
	Entity_Pickup* ret = player->DropCollectible(collectible, pickup, removeFromForm);
	return ret ? ret : pickup;
}

MOD_EXPORT void L_EntityPlayer_DropCollectibleByHistoryIndex(Entity_Player* player, int index, Entity_Pickup* pickup) {
	player->DropCollectibleByHistoryIndex(index, pickup, false);
}

MOD_EXPORT Entity_Effect* L_EntityPlayer_SpawnAquariusCreep(Entity_Player* player, TearParams* tearParams) {
	TearParams params;

	if (tearParams) {
		params = *tearParams;
	}
	else {
		player->GetTearHitParams(&params, (int)WeaponType::WEAPON_TEARS, (*player->GetTearPoisonDamage() * 0.666f) / player->_damage, (-(int)(Isaac::Random(2) != 0) & 2) - 1, 0);
	}

	Entity_Effect* effect = (Entity_Effect*)g_Game->Spawn(1000, 54, *player->GetPosition(), Vector(0.0, 0.0), player, 0, Random(), 0);

	if (effect) {
		float random = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		effect->_sprite._scale *= ((random * 0.5f) + 0.2f);
		effect->_collisionDamage = params._tearDamage;
		effect->SetColor(&params._tearColor, 0, -1, true, false);

		effect->_varData = params._flags;
		effect->Update();
	}

	return effect;
}

MOD_EXPORT void L_EntityPlayer_AddLocust(Entity_Player* player, int collectibleType, Vector* position) {
	Isaac::SpawnLocust(player, collectibleType, position);
}

// ---- parameters and results that are not plain values

MOD_EXPORT void L_EntityPlayer_GetTearHitParams(Entity_Player* player, int weaponType, float damageScale, int tearDisplacement, Entity* source, TearParams* out) {
	TearParams params;
	player->GetTearHitParams(&params, weaponType, damageScale, tearDisplacement, source);
	*out = params;
}

MOD_EXPORT void L_EntityPlayer_AnimatePickup(Entity_Player* player, ANM2* anm2, bool hideShadow, const char* animName) {
	std::string name = animName;
	player->AnimatePickup(anm2, hideShadow, &name);
}

MOD_EXPORT void L_EntityPlayer_ReplaceCostumeSprite(Entity_Player* player, ItemConfig_Item* item, const char* spritePath, int spriteId) {
	std::string path = spritePath;
	player->ReplaceCostumeSprite(item, &path, spriteId);
}

MOD_EXPORT void L_EntityPlayer_GetCostumeNullPos(Entity_Player* player, const char* nullFrameName, bool headScale, Vector* direction, Vector* out) {
	std::string name = nullFrameName;
	player->GetCostumeNullPos(out, &name, headScale, direction);
}

MOD_EXPORT void L_EntityPlayer_PlayCollectibleAnim(Entity_Player* player, int collectible, bool checkBodyAnim, const char* animName, int frameNum) {
	std::string name = animName;
	player->PlayCollectibleAnim((CollectibleType)collectible, checkBodyAnim, name, frameNum, false);
}

MOD_EXPORT bool L_EntityPlayer_IsCollectibleAnimFinished(Entity_Player* player, int collectible, const char* animName) {
	std::string name = animName;
	return player->IsCollectibleAnimFinished((CollectibleType)collectible, name);
}

MOD_EXPORT void L_EntityPlayer_ClearCollectibleAnim(Entity_Player* player, int collectible) {
	player->ClearCollectibleAnim((CollectibleType)collectible);
}

MOD_EXPORT void L_EntityPlayer_GetMultiShotParams(Entity_Player* player, int weaponType, Weapon_MultiShotParams* out) {
	player->GetMultiShotParams(out, (WeaponType)weaponType);
}

MOD_EXPORT void L_EntityPlayer_GetMultiShotPositionVelocity(Entity_Player* player, int loopIndex, int weaponType, Vector* shotDirection, float shotSpeed, Weapon_MultiShotParams* params, PosVel* out) {
	*out = player->GetMultiShotPositionVelocity(loopIndex, (WeaponType)weaponType, *shotDirection, shotSpeed, *params);
}

MOD_EXPORT void L_EntityPlayer_GetGlyphOfBalanceDrop(Entity_Player* player, int* variant, int* subtype) {
	player->GetGlyphOfBalanceDrop(variant, subtype);
}

MOD_EXPORT unsigned int L_EntityPlayer_GetSpecialGridCollision(Entity_Player* player, Vector* position) {
	Vector pos = position ? *position : *player->GetPosition();
	return player->GetSpecialGridCollision(&pos);
}

MOD_EXPORT short L_EntityPlayer_UseActiveItem(Entity_Player* player, int collectible, unsigned int useFlags, int activeSlot, int varData) {
	short resultFlags = 0;
	player->UseActiveItem(&resultFlags, collectible, useFlags, activeSlot, varData);
	return resultFlags;
}

MOD_EXPORT bool L_EntityPlayer_HasInvincibility(Entity_Player* player, uint64_t flags, EntityRef* source) {
	return player->HasInvincibility(flags, source);
}

MOD_EXPORT void L_EntityPlayer_GetFootprintColor(Entity_Player* player, bool second, KColor* out) {
	// The footprint colors are ColorMods, but the old output structure is kept for the REP+ migration
	ColorMod* footprintColor = second ? &player->_footprintColor2 : &player->_footprintColor1;
	*out = KColor(footprintColor->_offset[0], footprintColor->_offset[1], footprintColor->_offset[2], footprintColor->_tint[3]);
}

MOD_EXPORT void L_EntityPlayer_SetFootprintColor(Entity_Player* player, KColor* color, bool unk) {
	player->SetFootprintColor(*color, unk);
}

// ---- state with side effects

MOD_EXPORT void L_EntityPlayer_SetMegaBlastDuration(Entity_Player* player, int duration) {
	*player->GetMegaBlastDuration() = duration;

	Entity_Laser* laser = player->_megaBlastLaser;
	if (laser) {
		laser->_timeout = std::max(1, duration);
	}
}

MOD_EXPORT void L_EntityPlayer_ShuffleCostumes(Entity_Player* player, bool hasSeed, unsigned int seed) {
	player->ShuffleCostumes(hasSeed ? seed : std::max(Isaac::genrand_int32(), 1U));
}

MOD_EXPORT void L_EntityPlayer_RerollAllCollectibles(Entity_Player* player, RNG* rng, bool includeActives) {
	player->RerollAllCollectibles(rng ? rng : &player->_dropRNG, includeActives);
}

MOD_EXPORT bool L_EntityPlayer_ReviveCoopGhost(Entity_Player* player) {
	if (player->_isCoopGhost) {
		player->RevivePlayerGhost();
		return true;
	}
	return false;
}

MOD_EXPORT bool L_EntityPlayer_IsPacifist(Entity_Player* player) {
	return g_Game->_room->_pacifist;
}

MOD_EXPORT bool L_EntityPlayer_IsLocalPlayer(Entity_Player* player) {
	return g_Manager->GetNetplayManager()->IsIdxLocalPlayer(player->_controllerIndex);
}

MOD_EXPORT int L_EntityPlayer_GetErrorTrinketEffect(Entity_Player* player) {
	return g_Game->GetCurrentRoomDesc()->GetErrorTrinketEffect();
}

MOD_EXPORT void L_EntityPlayer_SetBlackHeart(Entity_Player* player, int blackHeart) {
	if ((blackHeart <= player->_soulHearts) && (blackHeart > -1)) {
		player->_blackHearts |= 1 << (blackHeart >> 1 & 0x1f);
		player->update_golden_hearts(false);
		player->update_bone_hearts();
	}
}

MOD_EXPORT int L_EntityPlayer_AddNullCostume(Entity_Player* player, int id) {
	int size = g_Manager->_itemConfig.GetNullItems()->size() - 1;
	if (id < 0 || id > size) {
		return size;
	}
	player->AddNullCostume(id);
	return -1;
}

MOD_EXPORT void L_EntityPlayer_SetHeadDirection(Entity_Player* player, int direction, int time, bool force) {
	static const char* headAnims[4] = {
		"HeadLeft",
		"HeadUp",
		"HeadRight",
		"HeadDown",
	};

	if (force || player->_headDirectionTime < 0) {
		if (player->_headDirection != direction) {
			player->_headDirection = direction;
			player->_headAnim = headAnims[direction];
		}
		player->_headDirectionTime = time;
	}
}

MOD_EXPORT void L_EntityPlayer_SetPoopSpell(Entity_Player* player, int position, int spell) {
	player->_poopSpellQueue[position] = spell;
}

MOD_EXPORT void L_EntityPlayer_RemovePoopSpell(Entity_Player* player, int position) {
	for (int i = position; i < 5; i++) {
		player->_poopSpellQueue[i] = player->_poopSpellQueue[i + 1];
	}
	player->_poopSpellQueue[5] = 0;
	player->CheckPoopSpellQueue();
}

MOD_EXPORT void L_EntityPlayer_ClearDeadEyeChargeNow(Entity_Player* player) {
	player->_deadEyeCharges = 0;
	player->_deadEyeMisses = 0;
	player->_cacheFlags |= 1;
	player->EvaluateItems();
}

MOD_EXPORT void L_EntityPlayer_CreateAfterimage(Entity_Player* player, int duration, Vector* position) {
	player->_afterImageFrames.push_back({ duration, *position });
}

static void AddTemporaryEffect(Entity_Player* player, int id, bool costume, int cooldown, bool additive, TemporaryEffect* (TemporaryEffects::*get)(int), void (TemporaryEffects::*add)(int, bool, int)) {
	constexpr int NO_COOLDOWN = -6942069;
	TemporaryEffects* effs = &player->_temporaryeffects;
	if (additive && (cooldown != NO_COOLDOWN)) {
		TemporaryEffect* effect = (effs->*get)(id);
		if (effect && (effect->_count > 0)) {
			cooldown += effect->_cooldown;
		}
		if (cooldown < 1) { cooldown = 1; }
	}
	(effs->*add)(id, costume, 1);
	if ((!additive) || (cooldown != NO_COOLDOWN)) {
		((effs->*get)(id))->_cooldown = cooldown;
	}
}

MOD_EXPORT void L_EntityPlayer_AddCollectibleEffect(Entity_Player* player, int id, bool costume, int cooldown, bool additive) {
	AddTemporaryEffect(player, id, costume, cooldown, additive, &TemporaryEffects::GetCollectibleEffect, &TemporaryEffects::AddCollectibleEffect);
}

MOD_EXPORT void L_EntityPlayer_AddNullItemEffect(Entity_Player* player, int id, bool costume, int cooldown, bool additive) {
	AddTemporaryEffect(player, id, costume, cooldown, additive, &TemporaryEffects::GetNullEffect, &TemporaryEffects::AddNullEffect);
}

MOD_EXPORT void L_EntityPlayer_AddTrinketEffect(Entity_Player* player, int id, bool costume, int cooldown, bool additive) {
	AddTemporaryEffect(player, id, costume, cooldown, additive, &TemporaryEffects::GetTrinketEffect, &TemporaryEffects::AddTrinketEffect);
}

MOD_EXPORT bool L_EntityPlayer_AddSmeltedTrinket(Entity_Player* player, int trinket, bool firstTime) {
	return AddSmeltedTrinketToPlayer(player, trinket, firstTime);
}

MOD_EXPORT bool L_EntityPlayer_IsValidTrinket(int trinket) {
	return g_Manager->GetItemConfig()->IsValidTrinket(trinket);
}

MOD_EXPORT int L_EntityPlayer_GetLayerCount(Entity_Player* player) {
	return player->_sprite.GetLayerCount();
}

MOD_EXPORT int L_EntityPlayer_GetLayerId(Entity_Player* player, const char* layerName) {
	LayerState* layerState = player->_sprite.GetLayer(layerName);
	return layerState ? layerState->GetLayerID() : -1;
}

MOD_EXPORT bool L_EntityPlayer_HasChanceRevive(Entity_Player* player) {
	return PlayerHasChanceRevive(player);
}

// ---- custom cache, camo

MOD_EXPORT void L_EntityPlayer_AddCustomCacheTag(Entity_Player* player, const char* tag) {
	if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player)) {
		playerPlus->customCacheTags.insert(stringlower(tag));
	}
}

MOD_EXPORT double L_EntityPlayer_GetCustomCacheValue(Entity_Player* player, const char* tag) {
	return GetCustomCacheValue(player, tag);
}

MOD_EXPORT bool L_EntityPlayer_IsForceCamo(Entity_Player* player) {
	EntityPlayerPlus* entityPlayerPlus = GetEntityPlayerPlus(player);
	return entityPlayerPlus && entityPlayerPlus->camoOverride;
}

MOD_EXPORT void L_EntityPlayer_SetForceCamo(Entity_Player* player, bool value) {
	if (EntityPlayerPlus* entityPlayerPlus = GetEntityPlayerPlus(player)) {
		entityPlayerPlus->camoOverride = value;
	}
}

MOD_EXPORT bool L_EntityPlayer_HasCamoEffect(Entity_Player* player) {
	EntityPlayerPlus* entityPlayerPlus = GetEntityPlayerPlus(player);
	TemporaryEffects* effects = &player->_temporaryeffects;

	return
		(entityPlayerPlus && entityPlayerPlus->camoOverride)
		|| g_Game->HasSeedEffect(SEED_CAMO_ISAAC)
		|| g_Game->HasSeedEffect(SEED_CAMO_EVERYTHING)
		|| effects->HasCollectibleEffect(CollectibleType::COLLECTIBLE_CAMO_UNDIES)
		|| effects->HasTrinketEffect(TrinketType::TRINKET_FADED_POLAROID);
}

MOD_EXPORT int L_EntityPlayer_GetMaxCoins() {
	return GetMaxCoins();
}

MOD_EXPORT int L_EntityPlayer_GetMaxKeys() {
	return GetMaxKeys();
}

MOD_EXPORT int L_EntityPlayer_GetMaxBombs() {
	return GetMaxBombs();
}

// ---- candy heart and soul locket

static const int candyHeartSoulLocketStats[6] = { CACHE_DAMAGE, CACHE_FIREDELAY, CACHE_RANGE, CACHE_SHOTSPEED, CACHE_SPEED, CACHE_LUCK };

MOD_EXPORT void L_EntityPlayer_AddCandyHeartSoulLocketBonus(Entity_Player* plr, bool isSoulLocket, int cacheFlags, int amount) {
	cacheFlags = cacheFlags & CACHE_ALL;
	if (cacheFlags <= 0) {
		cacheFlags = candyHeartSoulLocketStats[plr->GetCollectibleRNG(isSoulLocket ? COLLECTIBLE_SOUL_LOCKET : COLLECTIBLE_CANDY_HEART)->RandomInt(6)];
	}

	bool evaluateItems = false;

	for (int i = 0; i < 6; i++) {
		if (cacheFlags & candyHeartSoulLocketStats[i]) {
			if (isSoulLocket) {
				plr->_soulLocketStatUps[i] = (uint16_t)std::clamp(plr->_soulLocketStatUps[i] + amount, 0, 0xFFFF);
			} else {
				plr->_candyHeartStatUps[i] = (uint16_t)std::clamp(plr->_candyHeartStatUps[i] + amount, 0, 0xFFFF);
			}
			evaluateItems = true;
		}
	}

	if (evaluateItems) {
		plr->AddCacheFlags(cacheFlags);
		plr->EvaluateItems();
	}
}

// ---- bag of crafting

static void RecalculateBagOfCraftingOutput(Entity_Player* player) {
	g_Game->GetHUD()->InvalidateCraftingItem(player);

	BagOfCraftingPickup* content = player->GetBagOfCraftingContent();
	BagOfCraftingOutput* output = player->GetBagOfCraftingOutput();

	// Go through the slots, and if any empty spots are found before the end, shift everything after it down.
	// SetBagOfCraftingContent and SetBagOfCraftingSlot can allow people to write empty slots into the middle
	// of the array, which can cause issues, so this corrects it if it happens.
	for (int i = 0; i < 7; i++) {
		if (content[i] == BagOfCraftingPickup::BOC_NONE) {
			bool didShift = false;
			for (int j = i + 1; j < 8; j++) {
				if (content[j] > BagOfCraftingPickup::BOC_NONE) {
					content[i] = content[j];
					content[j] = BagOfCraftingPickup::BOC_NONE;
					didShift = true;
					break;
				}
			}
			if (!didShift) break;
		}
	}

	if (content[7] == BagOfCraftingPickup::BOC_NONE) {
		output->collectibleType = COLLECTIBLE_NULL;
		output->itemPoolType = POOL_NULL;
	}
	else {
		player->CalculateBagOfCraftingOutput(output, content, false);
	}
}

MOD_EXPORT int L_EntityPlayer_GetBagOfCraftingSlot(Entity_Player* player, int slot) {
	return player->GetBagOfCraftingContent()[slot];
}

MOD_EXPORT void L_EntityPlayer_SetBagOfCraftingContent(Entity_Player* player, const int* pickups) {
	BagOfCraftingPickup list[8]{};
	for (int i = 0; i < 8; i++) {
		list[i] = (BagOfCraftingPickup)pickups[i];
	}
	memcpy(&player->_bagOfCraftingContent, list, sizeof(list));

	RecalculateBagOfCraftingOutput(player);
}

MOD_EXPORT void L_EntityPlayer_SetBagOfCraftingSlot(Entity_Player* player, int slot, int pickup) {
	player->GetBagOfCraftingContent()[slot] = (BagOfCraftingPickup)pickup;

	RecalculateBagOfCraftingOutput(player);
}

MOD_EXPORT int L_EntityPlayer_GetBagOfCraftingOutput(Entity_Player* player) {
	return player->GetBagOfCraftingOutput()->collectibleType;
}

MOD_EXPORT int L_EntityPlayer_GetBagOfCraftingOutputItemPool(Entity_Player* player) {
	return player->GetBagOfCraftingOutput()->itemPoolType;
}

MOD_EXPORT void L_EntityPlayer_SetBagOfCraftingOutput(Entity_Player* player, int collectible, int itemPoolType) {
	if (itemPoolType < 0 || (uint32_t)itemPoolType >= ItemPoolManager::GetNumItemPools()) {
		itemPoolType = g_Game->_itemPool.GetFirstItemPoolForCollectible(collectible);
	}
	player->GetBagOfCraftingOutput()->collectibleType = collectible;
	player->GetBagOfCraftingOutput()->itemPoolType = itemPoolType;
	g_Game->GetHUD()->InvalidateCraftingItem(player);
}

MOD_EXPORT void L_EntityPlayer_CalculateBagOfCraftingOutput(const int* pickups, int* collectible, int* itemPool) {
	std::array<BagOfCraftingPickup, 8> content;
	for (int i = 0; i < 8; i++) {
		content[i] = (BagOfCraftingPickup)pickups[i];
	}

	BagOfCraftingOutput output;
	Entity_Player::CalculateBagOfCraftingOutput(&output, content.data(), false);

	*collectible = output.collectibleType;
	*itemPool = output.itemPoolType;
}

// ---- CheckFamiliar with results

static std::vector<Entity_Familiar*> s_checkedFamiliars;

MOD_EXPORT int L_EntityPlayer_CheckFamiliarEx(Entity_Player* player, int variant, int targetCount, RNG* rng, ItemConfig_Item* item, int subtype) {
	std::vector<Entity_Familiar*>& familiars = InitFamiliarStorage();
	player->CheckFamiliar(variant, targetCount, rng, item, subtype);

	s_checkedFamiliars = familiars;
	familiarsStorage.familiars.clear();
	familiarsStorage.inUse = false;
	return (int)s_checkedFamiliars.size();
}

MOD_EXPORT Entity_Familiar* L_EntityPlayer_GetCheckedFamiliar(int index) {
	return s_checkedFamiliars[index];
}

// ---- salvage

MOD_EXPORT void L_EntityPlayer_SalvageCollectibleEntity(Entity_Player* player, Entity_Pickup* pickup, RNG* rng, int pool) {
	rng = rng ? rng : &pickup->_dropRNG;
	player->SalvageCollectible(pickup->GetPosition(), pickup->_subtype, rng->Next(), pool);

	pickup->TryRemoveCollectible();
	g_Game->Spawn(ENTITY_EFFECT, 15, *pickup->GetPosition() + Vector(0, 10), Vector(0, 0), nullptr, 0, Random(), 0);
	pickup->_timeout = 2;
}

MOD_EXPORT void L_EntityPlayer_SalvageCollectibleType(Entity_Player* player, int subtype, Vector* position, RNG* rng, int pool) {
	position = position ? position : player->GetPosition();
	rng = rng ? rng : &player->_dropRNG;

	player->SalvageCollectible(position, subtype, rng->Next(), pool);
}
