#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"
#include "../../Utils/Entity/PickupUtils.h"
#include "../../Patches/EntityPlus.h"

MOD_EXPORT void L_EntityPickup_Morph(Entity_Pickup* pickup, int type, int variant, int subType, bool keepPrice, bool keepSeed, bool ignoreModifiers) {
	pickup->Morph(type, variant, subType, keepPrice, keepSeed, ignoreModifiers);
}

MOD_EXPORT void L_EntityPickup_SetPrice(Entity_Pickup* pickup, int price) {
	pickup->SetPrice(price);
}

MOD_EXPORT int L_EntityPickup_GetCoinValue(Entity_Pickup* pickup) {
	return pickup->GetCoinValue();
}

MOD_EXPORT bool L_EntityPickup_TryOpenChest(Entity_Pickup* pickup, Entity_Player* player) {
	return pickup->TryOpenChest(player);
}

MOD_EXPORT void L_EntityPickup_PlayDropSound(Entity_Pickup* pickup) {
	pickup->PlayDropSound();
}

MOD_EXPORT void L_EntityPickup_PlayPickupSound(Entity_Pickup* pickup) {
	pickup->PlayPickupSound();
}

MOD_EXPORT void L_EntityPickup_AppearFast(Entity_Pickup* pickup) {
	pickup->AppearFast();
}

MOD_EXPORT bool L_EntityPickup_CanReroll(Entity_Pickup* pickup) {
	return pickup->CanReroll();
}

MOD_EXPORT bool L_EntityPickup_CanJeraDuplicate(Entity_Pickup* pickup) {
	return pickup->CanJeraDuplicate();
}

MOD_EXPORT void L_EntityPickup_SetAlternatePedestal(Entity_Pickup* pickup, int pedestalType) {
	pickup->SetAlternatePedestal(pedestalType);
}

MOD_EXPORT int L_EntityPickup_GetAlternatePedestal(Entity_Pickup* pickup) {
	return pickup->GetAlternatePedestal();
}

MOD_EXPORT bool L_EntityPickup_TryRemoveCollectible(Entity_Pickup* pickup) {
	return pickup->TryRemoveCollectible();
}

MOD_EXPORT void L_EntityPickup_SetForceBlind(Entity_Pickup* pickup, bool blind) {
	pickup->SetForceBlind(blind);
}

MOD_EXPORT bool L_EntityPickup_IsBlind(Entity_Pickup* pickup, bool checkForcedBlindOnly) {
	if (pickup->_variant != 100) {
		return false;
	}

	if (checkForcedBlindOnly) {
		return pickup->IsBlind();
	}
	return pickup->IsBlind() || !pickup->_sprite._layerState[1]._spriteSheetPath.compare("gfx/Items/Collectibles/questionmark.png");
}

MOD_EXPORT int L_EntityPickup_SetNewOptionsPickupIndex(Entity_Pickup* pickup) {
	return pickup->SetNewOptionsPickupIndex();
}

MOD_EXPORT bool L_EntityPickup_TryInitOptionCycle(Entity_Pickup* pickup, int numCycle) {
	return pickup->TryInitOptionCycle(numCycle);
}

MOD_EXPORT void L_EntityPickup_MakeShopItem(Entity_Pickup* pickup, int shopItemId) {
	pickup->MakeShopItem(shopItemId);
}

MOD_EXPORT bool L_EntityPickup_TryFlip(Entity_Pickup* pickup) {
	return pickup->TryFlip(nullptr, 0);
}

MOD_EXPORT ANM2* L_EntityPickup_GetPriceSprite(Entity_Pickup* pickup) {
	return &pickup->_priceANM2;
}

MOD_EXPORT void L_EntityPickup_UpdatePickupGhosts(Entity_Pickup* pickup) {
	pickup->UpdatePickupGhosts();
}

MOD_EXPORT void L_EntityPickup_TriggerTheresOptionsPickup(Entity_Pickup* pickup) {
	pickup->TriggerTheresOptionsPickup();
}

MOD_EXPORT int L_EntityPickup_AddCycleCollectible(Entity_Pickup* pickup, int id) {
	if (g_Manager->_itemConfig.GetCollectible(id) == nullptr) {
		return -1;
	}

	if (pickup->_cycleCollectibleCount < 8) {
		pickup->_cycleCollectibleList[pickup->_cycleCollectibleCount] = id;
		pickup->_cycleCollectibleCount += 1;
		return 1;
	}
	return 0;
}

MOD_EXPORT bool L_EntityPickup_HasFlipData(Entity_Pickup* pickup) {
	return pickup->_variant == 100 && pickup->_flipSaveState.saveState != nullptr;
}

MOD_EXPORT int L_EntityPickup_GetFlipCollectible(Entity_Pickup* pickup) {
	return pickup->_flipSaveState.saveState->subtype;
}

MOD_EXPORT void L_EntityPickup_InitFlipState(Entity_Pickup* pickup, int collectibleId, bool setupCollectibleGraphics) {
	PickupUtils::InitFlipState(*pickup, (CollectibleType)collectibleId, setupCollectibleGraphics);
}

MOD_EXPORT void L_EntityPickup_ReloadGraphics(Entity_Pickup* pickup, bool ignoreBlind) {
	pickup->ReloadGraphics(ignoreBlind);
}

MOD_EXPORT void L_EntityPickup_GetLootList(Entity_Pickup* pickup, bool shouldAdvance, LootList* out) {
	LootList list = pickup->GetLootList(shouldAdvance, nullptr);
	out->~LootList();
	new (out) LootList(std::move(list));
}

MOD_EXPORT int L_EntityPickup_GetCanRerollOverride(Entity_Pickup* pickup) {
	if (EntityPickupPlus* entityPlus = GetEntityPickupPlus(pickup); entityPlus && entityPlus->canRerollOverride.has_value()) {
		return *entityPlus->canRerollOverride ? 1 : 0;
	}
	return -1;
}

MOD_EXPORT void L_EntityPickup_SetCanRerollOverride(Entity_Pickup* pickup, bool canReroll) {
	if (EntityPickupPlus* entityPlus = GetEntityPickupPlus(pickup)) {
		entityPlus->canRerollOverride = canReroll;
	}
}

MOD_EXPORT void L_EntityPickup_ClearCanRerollOverride(Entity_Pickup* pickup) {
	if (EntityPickupPlus* entityPlus = GetEntityPickupPlus(pickup)) {
		entityPlus->canRerollOverride = std::nullopt;
	}
}

MOD_EXPORT bool L_EntityPickup_ShouldIgnoreModifiers() {
	return Entity_Pickup::ShouldIgnoreModifiers();
}

MOD_EXPORT void L_EntityPickup_GetRandomPickupVelocity(Vector* position, RNG* rng, int velocityType, Vector* result) {
	Vector velocity;
	*result = *Entity_Pickup::GetRandomPickupVelocity(velocity, position, rng, velocityType);
}

MOD_EXPORT int L_EntityPickup_SetupCollectibleGraphics(ANM2* sprite, int layerId, int collectibleType, bool blind, unsigned int seed, bool loadGraphics) {
	if (layerId < 0 || sprite->_layerCount <= layerId) {
		return 1;
	}

	if (!g_Manager->GetItemConfig()->GetCollectible(collectibleType)) {
		return 2;
	}

	if (seed == 0) seed = 1;

	Entity_Pickup::SetupCollectibleGraphics(sprite, layerId, (CollectibleType)collectibleType, seed, blind);

	if (loadGraphics) {
		sprite->LoadGraphics(false);
	}
	return 0;
}
