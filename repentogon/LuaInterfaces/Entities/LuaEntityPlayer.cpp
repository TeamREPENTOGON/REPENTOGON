#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "../LuaWeapon.h"
#include "../../Patches/ASMPatches/ASMPlayer.h"

/*

         .___.
       / /\ /\ \
      ||●  `\ ●||
       \   ⌓   /
        /  ᴥ  \
       ||  _  ||
        |_| |_|

	 "code"
 (copycat of kilburn's ascii art of Isaac)
*/

extern "C" {
	int L_EntityPlayer_AddActiveCharge(Entity_Player* self, int charge, int slot, bool flashHUD, bool overcharge, bool force) {
		return self->AddActiveCharge(charge, slot, flashHUD, overcharge, force);
	}

	void L_EntityPlayer_AddBoneOrbital(Entity_Player* self, Vector* pos) {
		self->AddBoneOrbital(pos);
	}

	void L_EntityPlayer_AddBlackHearts(Entity_Player* self, int blackHearts) {
		self->AddBlackHearts(blackHearts);
	}

	void L_EntityPlayer_AddBloodCharge(Entity_Player* self, int amount) {
		self->AddBloodCharge(amount);
	}

	Entity* L_EntityPlayer_AddBlueFlies(Entity_Player* self, int amount, Vector* pos, Entity* target) {
		return self->AddBlueFlies(amount, pos, target);
	}

	Entity* L_EntityPlayer_AddBlueSpider(Entity_Player* self, Vector* pos) {
		return self->AddBlueSpider(pos);
	}

	void L_EntityPlayer_AddBombs(Entity_Player* self, int amount) {
		self->AddBombs(amount);
	}

	void L_EntityPlayer_AddBoneHearts(Entity_Player* self, int hearts) {
		self->AddBoneHearts(hearts);
	}

	void L_EntityPlayer_AddBrokenHearts(Entity_Player* self, int brokenHearts) {
		self->AddBrokenHearts(brokenHearts);
	}

	void L_EntityPlayer_AddCacheFlags(Entity_Player* self, int cacheFlag, bool evaluateItems) {
		self->AddCacheFlags(cacheFlag);
	}

	void L_EntityPlayer_AddCard(Entity_Player* self, int id) {
		self->AddPocketItem(id, 1);
	}

	void L_EntityPlayer_AddCoins(Entity_Player* self, int amount) {
		self->AddCoins(amount);
	}

	void L_EntityPlayer_AddCollectible(Entity_Player* self, int type, int charge, bool firstTimePickingUp, int slot, int varData) {
		self->AddCollectible(type, charge, firstTimePickingUp, slot, varData);
	}

	void L_EntityPlayer_AddControlsCooldown(Entity_Player* self, int cooldown) {
		self->_controlsCooldown += cooldown;
	}

	void L_EntityPlayer_AddCostume(Entity_Player* self, ItemConfig_Item* item, bool itemStateOnly) {
		self->AddCostume(item, itemStateOnly);
	}

	void L_EntityPlayer_AddCurseMistEffect(Entity_Player* self) {
		self->AddCurseMistEffect();
	}

	void L_EntityPlayer_AddDeadEyeCharge(Entity_Player* self) {
		self->AddDeadEyeCharge();
	}

	void L_EntityPlayer_AddDollarBillEffect(Entity_Player* self) {
		self->AddDollarBillEffect();
	}

	void L_EntityPlayer_AddEternalHearts(Entity_Player* self, int eternalHearts) {
		self->AddEternalHearts(eternalHearts);
	}

	void L_EntityPlayer_AddFriendlyDip(Entity_Player* self, int subtype, Vector* pos) {
		Entity_Familiar::SpawnDip(subtype, *pos, self);
	}

	void L_EntityPlayer_AddGigaBombs(Entity_Player* self, int gigaBombs) {
		self->AddGigaBombs(gigaBombs);
	}

	void L_EntityPlayer_AddGoldenBomb(Entity_Player* self) {
		self->AddGoldenBomb();
	}

	void L_EntityPlayer_AddGoldenHearts(Entity_Player* self, int hearts) {
		self->AddGoldenHearts(hearts);
	}

	void L_EntityPlayer_AddGoldenKey(Entity_Player* self) {
		self->AddGoldenKey();
	}

	void L_EntityPlayer_AddHearts(Entity_Player* self, int hearts) {
		self->AddHearts(hearts, false);
	}

	void L_EntityPlayer_AddInnateCollectible(Entity_Player* self, int itemId, uint32_t amount) {
		/*std::set<uint32_t>& itemWispList = *plr->GetItemWispsList();
		itemWispList.insert(luaL_checkinteger(L,2));
		*/

		ItemConfig itemConfig = g_Manager->_itemConfig;
		ItemConfig_Item* item = itemConfig.GetCollectibles()[0][itemId];

		//ItemConfig_Item* item_ptr = item;

		std::map<int, int>& wispMap = *self->GetItemWispsList();
		wispMap[itemId] += amount;

		if (amount > 0 && item->addCostumeOnPickup) {
			self->AddCostume(item, false);
		}

		self->AddCacheFlags(item->cacheFlags);
		self->EvaluateItems();
	}

	Entity_Familiar* L_EntityPlayer_AddItemWisp(Entity_Player* self, CollectibleType collectible, Vector* pos, bool adjustOrbitLayer) {
		return self->AddItemWisp(collectible, pos, adjustOrbitLayer);
	}

	void L_EntityPlayer_AddJarFlies(Entity_Player* self, int flies) {
		self->AddJarFlies(flies);
	}

	void L_EntityPlayer_AddJarHearts(Entity_Player* self, int hearts) {
		self->AddJarHearts(hearts);
	}

	void L_EntityPlayer_AddKeys(Entity_Player* self, int amount) {
		self->AddKeys(amount);
	}

	void L_EntityPlayer_AddLeprosy(Entity_Player* self) {
		self->AddLeprosy();
	}

	void L_EntityPlayer_AddLocust(Entity_Player* self, int itemType, Vector* pos) {
		Isaac::SpawnLocust(self, itemType, pos);
	}

	void L_EntityPlayer_AddMaxHearts(Entity_Player* self, int maxHearts, bool ignoreKeeper) {
		self->AddMaxHearts(maxHearts, ignoreKeeper);
	}

	Entity_Familiar* L_EntityPlayer_AddMinisaac(Entity_Player* self, Vector* pos, bool playAnim) {
		return self->AddMinisaac(pos, playAnim);
	}

	void L_EntityPlayer_AddNullCostume(Entity_Player* self, int nullId) {
		self->AddNullCostume(nullId);
	}

	void L_EntityPlayer_AddPill(Entity_Player* self, uint32_t pill) {
		self->AddPocketItem(pill, 0);
	}

	void L_EntityPlayer_AddPlayerFormCostume(Entity_Player* self, int form) {
		self->AddPlayerFormCostume(form);
	}

	void L_EntityPlayer_AddPoopMana(Entity_Player* self, int num) {
		self->AddPoopMana(num);
	}

	void L_EntityPlayer_AddPrettyFly(Entity_Player* self) {
		self->AddPrettyFly();
	}

	void L_EntityPlayer_AddRottenHearts(Entity_Player* self, int rottenHearts) {
		self->AddRottenHearts(rottenHearts, false);
	}

	void L_EntityPlayer_AddSmeltedTrinket(Entity_Player* self, int trinketType, bool firstTimePickingUp) {
		self->AddSmeltedTrinket(trinketType, firstTimePickingUp);
	}

	void L_EntityPlayer_AddSoulCharge(Entity_Player* self, int amount) {
		self->AddSoulCharge(amount);
	}

	void L_EntityPlayer_AddSoulHearts(Entity_Player* self, int soulHearts) {
		self->AddSoulHearts(soulHearts);
	}

	Entity_Familiar* L_EntityPlayer_AddSwarmFlyOrbital(Entity_Player* self, Vector* pos) {
		return self->AddSwarmFlyOrbital(pos);
	}

	void L_EntityPlayer_AddTrinket(Entity_Player* self, TrinketType type, bool firstTimePickingUp) {
		self->AddTrinket(type, firstTimePickingUp);
	}

	void L_EntityPlayer_AddUrnSouls(Entity_Player* self, int amount) {
		self->AddUrnSouls(amount);
	}

	Entity_Familiar* L_EntityPlayer_AddWisp(Entity_Player* self, int collectible, Vector* pos, bool adjustOrbitLayer, bool dontUpdate) {
		return self->AddWisp(collectible, pos, adjustOrbitLayer, dontUpdate);
	}
	void L_EntityPlayer_AnimateAppear(Entity_Player* self) {
		self->PlayExtraAnimation("Appear");
	}

	void L_EntityPlayer_AnimateCard(Entity_Player* self, int card, const char* animName) {
		self->AnimateCard(card, animName);
	}

	void L_EntityPlayer_AnimateCollectible(Entity_Player* self, int collectible, const char* animName, const char* spriteAnimName) {
		self->AnimateCollectible(collectible, animName, spriteAnimName);
	}

	void L_EntityPlayer_AnimateHappy(Entity_Player* self) {
		self->AnimateHappy();
	}

	void L_EntityPlayer_AnimateLightTravel(Entity_Player* self) {
		self->PlayExtraAnimation("LightTravel");
	}

	void L_EntityPlayer_AnimatePickup(Entity_Player* self, ANM2* anm2, bool hideShadow, const char* animName) {
		self->AnimatePickup(anm2, hideShadow, animName);
	}

	void L_EntityPlayer_AnimatePill(Entity_Player* self, int pill, const char* animName) {
		self->AnimatePill(pill, animName);
	}

	void L_EntityPlayer_AnimatePitfallIn(Entity_Player* self, bool force) {
		self->AnimatePitfallIn(force);
	}

	void L_EntityPlayer_AnimatePitfallOut(Entity_Player* self) {
		self->AnimatePitfallOut();
	}

	void L_EntityPlayer_AnimateSad(Entity_Player* self) {
		self->AnimateSad();
	}

	void L_EntityPlayer_AnimateTeleport(Entity_Player* self, bool up) {
		self->AnimateTeleport(up);
	}

	void L_EntityPlayer_AnimateTrapdoor(Entity_Player* self) {
		self->PlayExtraAnimation("Trapdoor");
	}

	void L_EntityPlayer_AnimateTrinket(Entity_Player* self, TrinketType trinket, const char* animName, const char* spriteAnimName) {
		self->AnimateTrinket(trinket, animName, spriteAnimName);
	}

	bool L_EntityPlayer_AreControlsEnabled(Entity_Player* self) {
		return self->AreControlsEnabled();
	}

	bool L_EntityPlayer_AreOpposingShootDirectionsPressed(Entity_Player* self) {
		return self->_opposingShootDirectionsPressed;
	}

	bool L_EntityPlayer_CanAddCollectible(Entity_Player* self, int type) {
		return self->CanAddCollectible(type);
	}

	bool L_EntityPlayer_CanAddCollectibleToInventory(Entity_Player* self, int type) {
		return self->CanAddCollectibleToInventory(type);
	}

	bool L_EntityPlayer_CanCrushRocks(Entity_Player* self) {
		return self->CanCrushRocks();
	}

	bool L_EntityPlayer_CanOverrideActiveItem(Entity_Player* self, uint32_t slot) {
		return self->CanOverrideActiveItem(slot);
	}

	bool L_EntityPlayer_CanPickBlackHearts(Entity_Player* self) {
		return self->CanPickBlackHearts();
	}

	bool L_EntityPlayer_CanPickBoneHearts(Entity_Player* self) {
		return self->CanPickBoneHearts();
	}

	bool L_EntityPlayer_CanPickGoldenHearts(Entity_Player* self) {
		return self->CanPickGoldenHearts();
	}

	bool L_EntityPlayer_CanPickRedHearts(Entity_Player* self) {
		return self->CanPickRedHearts();
	}

	bool L_EntityPlayer_CanPickRottenHearts(Entity_Player* self) {
		return self->CanPickRottenHearts();
	}

	bool L_EntityPlayer_CanPickSoulHearts(Entity_Player* self) {
		return self->CanPickSoulHearts();
	}

	bool L_EntityPlayer_CanPickupItem(Entity_Player* self) {
		return self->CanPickupItem();
	}

	bool L_EntityPlayer_CanShoot(Entity_Player* self) {
		return *self->GetCanShoot();
	}

	bool L_EntityPlayer_CanUsePill(Entity_Player* self, int pillEffect) {
		return self->CanUsePill(pillEffect);
	}

	bool L_EntityPlayer_CanTurnHead(Entity_Player* self) {
		return self->CanTurnHead();
	}

	void L_EntityPlayer_ChangePlayerType(Entity_Player* self, int playerType, bool force) {
		self->ChangePlayerType(playerType, force);
	}

	void L_EntityPlayer_CheckFamiliar(Entity_Player* self, int familiarVariant, int targetCount, RNG* rng, ItemConfig_Item* sourceItemConfigItem, int familiarSubType) {
		self->CheckFamiliar(familiarVariant, targetCount, rng, sourceItemConfigItem, familiarSubType);
	}
	// !!!TODO!!!
	void* L_EntityPlayer_CheckFamiliarEx(Entity_Player* self, int familiarVariant, int targetCount, RNG* rng, ItemConfig_Item* sourceItemConfigItem, int familiarSubType) {
		std::vector<Entity_Familiar*>& familiars = InitFamiliarStorage();
		self->CheckFamiliar(familiarVariant, targetCount, rng, sourceItemConfigItem, familiarSubType);
		return &familiars;
	}

	void L_EntityPlayer_ClearCollectibleAnim(Entity_Player* self, CollectibleType item) {
		self->ClearCollectibleAnim(item);
	}

	void L_EntityPlayer_ClearItemAnimCollectible(Entity_Player* self, CollectibleType item) {
		self->ClearItemAnimCollectible(item);
	}

	void L_EntityPlayer_ClearItemAnimNullItems(Entity_Player* self) {
		self->ClearItemAnimNullItems();
	}

	void L_EntityPlayer_ClearCostumes(Entity_Player* self) {
		self->ClearCostumes();
	}

	void L_EntityPlayer_ClearDeadEyeCharge(Entity_Player* self, bool force) {
		if (force) {
			self->_deadEyeCharges = 0;
			self->_deadEyeMisses = 0;
			self->_cacheFlags |= 1;
			self->EvaluateItems();
		}
		else {
			self->ClearDeadEyeCharge();
		}
	}

	void L_EntityPlayer_ClearTemporaryEffects(Entity_Player* self) {
		self->ClearTemporaryEffects();
	}

	void L_EntityPlayer_DischargeActiveItem(Entity_Player* self, int activeSlot) {
		self->DischargeActiveItem(activeSlot, 0);
	}

	void L_EntityPlayer_DonateLuck(Entity_Player* self, int luck) {
		self->DonateLuck(luck);
	}

	void L_EntityPlayer_DoZitEffect(Entity_Player* self, Vector* dir) {
		self->do_zit_effect(dir);
	}

	Entity_Pickup* L_EntityPlayer_DropCollectible(Entity_Player* self, int collectible, Entity_Pickup* existingPedestal, bool removeFromPlayerForm) {
		return self->DropCollectible(collectible, existingPedestal, removeFromPlayerForm);
	}

	Entity_Pickup* L_EntityPlayer_DropCollectibleByHistoryIndex(Entity_Player* self, int idx, Entity_Pickup* existingPedestal) {
		return self->DropCollectibleByHistoryIndex(idx, existingPedestal);
	}

	void L_EntityPlayer_DropPocketItem(Entity_Player* self, int pocketNum, Vector* pos) {
		self->DropPocketItem(pocketNum, pos);
	}

	Entity_Pickup* L_EntityPlayer_DropTrinket(Entity_Player* self, Vector* dropPos, bool replaceTick) {
		return self->DropTrinket(dropPos, replaceTick);
	}

	void L_EntityPlayer_EnableWeaponType(Entity_Player* self, WeaponType weaponType, bool set) {
		self->EnableWeaponType(weaponType, set);
	}

	void L_EntityPlayer_EvaluateItems(Entity_Player* self) {
		self->EvaluateItems();
	}

	Entity_Bomb* L_EntityPlayer_FireBomb(Entity_Player* self, Vector* pos, Vector* vel, Entity* source) {
		return self->FireBomb(pos, vel, source);
	}

	Entity_Laser* L_EntityPlayer_FireBrimstone(Entity_Player* self, Vector* dir, Entity* source, float damageMultiplier) {
		return self->FireBrimstone(dir, source, damageMultiplier);
	}

	Entity_Effect* L_EntityPlayer_FireBrimstoneBall(Entity_Player* self, Vector* pos, Vector* vel, Vector* offset) {
		return self->FireBrimstoneBall(*pos, *vel, *offset, 0, 0, nullptr);
	}

	Entity_Laser* L_EntityPlayer_FireDelayedBrimstone(Entity_Player* self, float angle, Entity* source) {
		return self->FireDelayedBrimstone(angle, source);
	}

	Entity_Knife* L_EntityPlayer_FireKnife(Entity_Player* self, Entity* parent, float rotationOffset, bool cantOverwrite, int subtype, int variant) {
		return self->FireKnife(parent, variant, rotationOffset, cantOverwrite, subtype);
	}

	// i'll need some help with this one
	Entity_Tear* L_EntityPlayer_FireTear(Entity_Player* self, Vector* pos, Vector* vel, bool canBeEye, bool noTractorBeam, bool canTriggerStreakEnd, Entity* source, float damageMultiplier) {
		// return self->FireTear(self, pos, vel, can);
		return nullptr;
	}

	Entity_Laser* L_EntityPlayer_FireTechLaser(Entity_Player* self, Vector* pos, int offsetID, Vector* dir, bool leftEye, bool oneHit, Entity* source, float damageMultiplier) {
		return self->FireTechLaser(*pos, offsetID, *dir, leftEye, oneHit, source, damageMultiplier);
	}
	Entity_Laser* L_EntityPlayer_FireTechXLaser(Entity_Player* self, Vector* pos, Vector* dir, float radius, Entity* source, float damageMultiplier) {
		return self->FireTechXLaser(*pos, *dir, radius, source, damageMultiplier);
	}

	void L_EntityPlayer_ClearQueueItem(Entity_Player* self) {
		self->ClearQueueItem();
	}

	bool L_EntityPlayer_FlushQueueItem(Entity_Player* self) {
		return self->FlushQueueItem();
	}

	bool L_EntityPlayer_FullCharge(Entity_Player* self, int activeSlot, bool force) {
		return self->FullCharge(activeSlot, force);
	}

	int L_EntityPlayer_GetActiveCharge(Entity_Player* self, int activeSlot) {
		return self->GetActiveCharge(activeSlot);
	}

	CollectibleType L_EntityPlayer_GetActiveItem(Entity_Player* self, int activeSlot) {
		return static_cast<CollectibleType>(self->GetActiveItemDesc(activeSlot)->_item);
	}

	ActiveItemDesc* L_EntityPlayer_GetActiveItemDesc(Entity_Player* self, int activeSlot) {
		return self->GetActiveItemDesc(activeSlot);
	}

	int L_EntityPlayer_GetActiveItemSlot(Entity_Player* self, int id) {
		return self->GetActiveItemSlot(id);
	}

	int L_EntityPlayer_GetActiveMaxCharge(Entity_Player* self, int activeSlot) {
		return self->GetActiveMaxCharge(activeSlot);
	}

	int L_EntityPlayer_GetActiveMinUsableCharge(Entity_Player* self, int activeSlot) {
		return self->GetActiveMinUsableCharge(activeSlot);
	}

	int L_EntityPlayer_GetActiveSubCharge(Entity_Player* self, int activeSlot) {
		return self->GetActiveItemDesc(activeSlot)->_subCharge;
	}

	Entity* L_EntityPlayer_GetActiveWeaponEntity(Entity_Player* self) {
		return self->GetActiveWeaponEntity();
	}

	int L_EntityPlayer_GetActiveWeaponNumFired(Entity_Player* self) {
		Weapon* wep = *self->GetWeapon(0);
		if (wep == nullptr) {
			wep = *self->GetWeapon(1);
			if (wep == nullptr) return 0;
		}

		return wep->GetNumFired();
	}

	Vector* L_EntityPlayer_GetAimDirection(Entity_Player* self) {
		return &self->_aimDirection;
	}

	int L_EntityPlayer_GetBabySkin(Entity_Player* self) {
		return self->_babySkin;
	}
	BagOfCraftingPickup* L_EntityPlayer_GetBagOfCraftingContent(Entity_Player* self) {
		return self->GetBagOfCraftingContent();
	}

	int* L_EntityPlayer_GetBagOfCraftingOutput(Entity_Player* self) {
		return self->GetBagOfCraftingOutput();
	}

	BagOfCraftingPickup L_EntityPlayer_GetBagOfCraftingSlot(Entity_Player* self, int slot) {
		return self->GetBagOfCraftingContent()[slot];
	}

	int L_EntityPlayer_GetBatteryCharge(Entity_Player* self, int activeSlot) {
		return self->GetActiveItemDesc(activeSlot)->_batteryCharge;
	}

	int L_EntityPlayer_GetBlackHearts(Entity_Player* self) {
		return self->_blackHearts;
	}
	int L_EntityPlayer_GetBladderCharge(Entity_Player* self) {
		return *self->GetBladderCharge();
	}
	int L_EntityPlayer_GetBloodCharge(Entity_Player* self) {
		return self->_bloodCharges;
	}

	int L_EntityPlayer_GetBodyColor(Entity_Player* self) {
		return self->_bodyColor;
	}
	Vector* L_EntityPlayer_GetBodyMoveDirection(Entity_Player* self) {
		return new auto(self->GetBodyMoveDirection());
	}

	// !!!TODO!!!
	int L_EntityPlayer_GetBombFlags(Entity_Player* self);

	int L_EntityPlayer_GetBombVariant(Entity_Player* self, int tearFlags, bool ForceSmallBomb) {
		return 0;
	}

	int L_EntityPlayer_GetBoneHearts(Entity_Player* self) {
		return *self->GetBoneHearts();
	}

	int L_EntityPlayer_GetBrokenHearts(Entity_Player* self) {
		return *self->GetBrokenHearts();
	}

	int L_EntityPlayer_GetCambionConceptionState(Entity_Player* self) {
		return *self->GetCambionConceptionState();
	}

	int L_EntityPlayer_GetCambionPregnancyLevel(Entity_Player* self) {
		return self->GetCambionPregnancyLevel();
	}

	int L_EntityPlayer_GetCard(Entity_Player* self, int slotId) {
		return self->GetCard(slotId);
	}

	RNG* L_EntityPlayer_GetCardRNG(Entity_Player* self, int id) {
		return self->GetCardRNG(id);
	}

	int L_EntityPlayer_GetCollectibleCount(Entity_Player* self) {
		return self->GetCollectibleCount();
	}

	int L_EntityPlayer_GetCollectibleNum(Entity_Player* self, CollectibleType type, bool onlyCountTrueItems) {
		return self->GetCollectibleNum(type, onlyCountTrueItems);
	}

	RNG* L_EntityPlayer_GetCollectibleRNG(Entity_Player* self, CollectibleType id) {
		return self->GetCollectibleRNG(id);
	}

	// !!!TODO!!!
	int* L_EntityPlayer_GetCollectiblesList(Entity_Player* self) {
		return &self->GetCollectiblesList().at(0);
	}

	// !!!TODO!!!
	void* L_EntityPlayer_GetCostumeLayerMap(Entity_Player* self) {
		PlayerCostumeMap* costumeLayerMap = self->_playerCostumeMap;
		/*for (int idx = 0; idx < 15; idx++) {
			lua_pushinteger(L, idx + 1);

			lua_newtable(L);

			lua_pushstring(L, "costumeIndex");
			lua_pushinteger(L, costumeLayerMap[idx]._index);
			lua_settable(L, -3);

			lua_pushstring(L, "layerID");
			lua_pushinteger(L, costumeLayerMap[idx]._layerID);
			lua_settable(L, -3);

			lua_pushstring(L, "priority");
			lua_pushinteger(L, costumeLayerMap[idx]._priority);
			lua_settable(L, -3);

			lua_pushstring(L, "isBodyLayer");
			lua_pushboolean(L, costumeLayerMap[idx]._isBodyLayer);
			lua_settable(L, -3);

			lua_settable(L, -3);
		}*/

		return costumeLayerMap;
	}

	// !!!TODO!!!
	void* L_EntityPlayer_GetCostumeSpriteDescs(Entity_Player* self) {
		return nullptr;
	}

	Vector* L_EntityPlayer_GetCostumeNullPos(Entity_Player* self, const char* nullFrameName, bool headScale, Vector* dir) {
		return new auto(self->GetCostumeNullPos(std::string(nullFrameName), headScale, *dir));
	}

	float L_EntityPlayer_GetD8DamageModifier(Entity_Player* self) {
		return *self->GetD8DamageModifier();
	}

	float L_EntityPlayer_GetD8FireDelayModifier(Entity_Player* self) {
		return *self->GetD8FireDelayModifier();
	}

	float L_EntityPlayer_GetD8RangeModifier(Entity_Player* self) {
		return *self->GetD8RangeModifier();
	}

	float L_EntityPlayer_GetD8SpeedModifier(Entity_Player* self) {
		return *self->GetD8DamageModifier();
	}

	int L_EntityPlayer_GetDamageCooldown(Entity_Player* self) {
		return self->_damageCooldown;
	}

	int L_EntityPlayer_GetDamageModifier(Entity_Player* self) {
		return *self->GetDamageModifier();
	}

	int L_EntityPlayer_GetDeadEyeCharge(Entity_Player* self) {
		return self->GetDeadEyeCharge();
	}

	const char* L_EntityPlayer_GetDeathAnimName(Entity_Player* self) {
		return self->GetDeathAnimName();
	}

	float L_EntityPlayer_GetEdenDamage(Entity_Player* self) {
		return *self->GetEdenDamage();
	}

	float L_EntityPlayer_GetEdenFireDelay(Entity_Player* self) {
		return *self->GetEdenFireDelay();
	}

	float L_EntityPlayer_GetEdenLuck(Entity_Player* self) {
		return *self->GetEdenLuck();
	}

	float L_EntityPlayer_GetEdenRange(Entity_Player* self) {
		return *self->GetEdenRange();
	}

	float L_EntityPlayer_GetEdenShotSpeed(Entity_Player* self) {
		return *self->GetEdenShotSpeed();
	}

	float L_EntityPlayer_GetEdenSpeed(Entity_Player* self) {
		return *self->GetEdenSpeed();
	}

	int L_EntityPlayer_GetEffectiveBloodCharge(Entity_Player* self) {
		if (self->_type == 0x24) return self->_bloodCharges;
		return 0;
	}

	int L_EntityPlayer_GetEffectiveMaxHearts(Entity_Player* self) {
		return self->GetEffectiveMaxHearts();
	}

	int L_EntityPlayer_GetEffectiveSoulCharge(Entity_Player* self) {
		if (self->_type == 0x12) return self->_soulCharges;
		return 0;
	}

	TemporaryEffects* L_EntityPlayer_GetEffects(Entity_Player* self) {
		return &self->_temporaryEffects;
	}

	Vector* L_EntityPlayer_GetEnterPosition(Entity_Player* self) {
		return new auto(self->GetEnterPosition());
	}

	EntityConfig_Player* L_EntityPlayer_GetEntityConfigPlayer(Entity_Player* self) {
		return g_Manager->GetEntityConfig()->GetPlayer(self->GetPlayerType());
	}

	int L_EntityPlayer_GetEpiphoraCharge(Entity_Player* self) {
		return *self->GetEpiphoraCharge();
	}

	int L_EntityPlayer_GetEternalHearts(Entity_Player* self) {
		return *self->GetEternalHearts();
	}
	int L_EntityPlayer_GetEveSumptoriumCharge(Entity_Player* self) {
		return self->_eveSumptoriumCharge;
	}

	int L_EntityPlayer_GetExtraLives(Entity_Player* self) {
		return self->GetExtraLives();
	}

	int L_EntityPlayer_GetFireDelayModifier(Entity_Player* self) {
		return *self->GetFireDelayModifier();
	}

	int L_EntityPlayer_GetFireDirection(Entity_Player* self) {
		return self->_fireDirection;
	}

	Entity_Player* L_EntityPlayer_GetFlippedForm(Entity_Player* self) {
		return self->_backupPlayer;
	}

	Entity* L_EntityPlayer_GetFocusEntity(Entity_Player* self) {
		return self->GetFocusEntity();
	}

	KColor* L_EntityPlayer_GetFootprintColor(Entity_Player* self, bool leftFootPrint) {
		if (leftFootPrint) return &self->_footprintColor2;
		return &self->_footprintColor1;
	}

	Vector* L_EntityPlayer_GetFlyingOffset(Entity_Player* self) {
		return new auto(self->GetFlyingOffset());
	}

	int L_EntityPlayer_GetGlitchBabySubType(Entity_Player* self) {
		return self->GetGlitchBabySubType();
	}

	// TODO
	void* L_EntityPlayer_GetGlyphOfBalanceDrop(Entity_Player* self) {
		int* variant, *subtype;
		self->GetGlyphOfBalanceDrop(variant, subtype);
		return nullptr;
	}

	// ?????? https://repentogon.com/EntityPlayer.html#getgnawedleaftimer
	int L_EntityPlayer_GetGnawedLeafTimer(Entity_Player* self) {
		return self->_gnawedLeafCooldown;
	}

	int L_EntityPlayer_GetGoldenHearts(Entity_Player* self) {
		return *self->GetGoldenHearts();
	}

	float L_EntityPlayer_GetGreedDonationBreakChance(Entity_Player* self) {
		return self->GetGreedDonationBreakChance();
	}

	int L_EntityPlayer_GetGreedsGulletHearts(Entity_Player* self) {
		return self->GetGreedsGulletHearts();
	}

	int L_EntityPlayer_GetHeadColor(Entity_Player* self) {
		return self->_headColor;
	}

	int L_EntityPlayer_GetHeadDirection(Entity_Player* self) {
		return self->_headDirection;
	}

	int L_EntityPlayer_GetHealthType(Entity_Player* self) {
		return self->GetHealthType();
	}

	int L_EntityPlayer_GetHeartLimit(Entity_Player* self, bool keeper) {
		return self->GetHealthLimit(keeper);
	}

	int L_EntityPlayer_GetHearts(Entity_Player* self) {
		return *self->GetRedHearts();
	}

	Entity* L_EntityPlayer_GetHeldEntity(Entity_Player* self) {
		return *self->GetHeldEntity();
	}

	ANM2* L_EntityPlayer_GetHeldSprite(Entity_Player* self) {
		return self->GetHeldSprite();
	}

	History* L_EntityPlayer_GetHistory(Entity_Player* self) {
		return self->GetHistory();
	}

	int L_EntityPlayer_GetImmaculateConceptionState(Entity_Player* self) {
		return *self->GetImmaculateConceptionState();
	}

	CollectibleType L_EntityPlayer_GetItemState(Entity_Player* self) {
		return (CollectibleType)self->_itemState;
	}

	int L_EntityPlayer_GetJarFlies(Entity_Player* self) {
		return self->_jarFlies;
	}

	int L_EntityPlayer_GetJarHearts(Entity_Player* self) {
		return self->_jarHearts;
	}

	int L_EntityPlayer_GetKeepersSackBonus(Entity_Player* self) {
		return self->_keepersSackBonus;
	}

	ColorMod* L_EntityPlayer_GetLaserColor(Entity_Player* self) {
		return &self->_laserColor;
	}

	Vector* L_EntityPlayer_GetLaserOffset(Entity_Player* self, int id, Vector* dir) {
		return new auto(self->GetLaserOffset(id, *dir));
	}

	int L_EntityPlayer_GetLuckModifier(Entity_Player* self) {
		return self->_luckModifier;
	}

	int L_EntityPlayer_GetLastActionTriggers(Entity_Player* self) {
		return self->_lastActionTriggers;
	}

	int L_EntityPlayer_GetLastDamageFlags(Entity_Player* self) {
		return self->_lastDamageFlags;
	}

	const EntityRef* L_EntityPlayer_GetLastDamageSource(Entity_Player* self) {
		return &self->_lastDamageSource;
	}

	const Vector* L_EntityPlayer_GetLastDirection(Entity_Player* self) {
		return &self->_lastDirection;
	}
	int L_EntityPlayer_GetMaggySwingCooldown(Entity_Player* self) {
		return self->_maggySwingCooldown;
	}

	Entity_Player* L_EntityPlayer_GetMainTwin(Entity_Player* self) {
		return self->GetMainTwin();
	}

	Entity_Effect* L_EntityPlayer_GetMarkedTarget(Entity_Player* self) {
		return self->GetMarkedTarget();
	}

	int L_EntityPlayer_GetMaxBladderCharge(Entity_Player* self) {
		return *self->GetMaxBladderCharge();
	}

	int L_EntityPlayer_GetMaxPeeBurstCooldown(Entity_Player* self) {
		return *self->GetMaxPeeBurstCooldown();
	}

	int L_EntityPlayer_GetMaxHearts(Entity_Player* self) {
		return *self->GetMaxHearts();
	}

	int L_EntityPlayer_GetMaxPocketItems(Entity_Player* self) {
		return self->GetMaxPocketItems();
	}

	int L_EntityPlayer_GetMaxPoopMana(Entity_Player* self) {
		return self->GetMaxPoopMana();
	}

	int L_EntityPlayer_GetMaxTrinkets(Entity_Player* self) {
		return self->GetMaxTrinkets();
	}

	int L_EntityPlayer_GetMegaBlastDuration(Entity_Player* self) {
		return *self->GetMegaBlastDuration();
	}

	int L_EntityPlayer_GetMetronomeCollectibleID(Entity_Player* self) {
		return *self->GetMetronomeCollectibleID();
	}

	CollectibleType L_EntityPlayer_GetModelingClayEffect(Entity_Player* self) {
		return static_cast<CollectibleType>(self->_modelingClayEffect);
	}

	Direction L_EntityPlayer_GetMovementDirection(Entity_Player* self) {
		return static_cast<Direction>(self->_movementDirection);
	}

	const Vector* L_EntityPlayer_GetMovementInput(Entity_Player* self, float moveSpeed) {
		return new auto(self->GetMovementInput(moveSpeed));
	}

	Vector* L_EntityPlayer_GetMovementJoystick(Entity_Player* self) {
		return new auto(self->GetMovementInput(0.0f));
	}

	Vector* L_EntityPlayer_GetMovementVector(Entity_Player* self) {
		return &self->_movementVector;
	}

	Weapon_MultiShotParams* L_EntityPlayer_GetMultiShotParams(Entity_Player* self, int weaponType) {
		return new auto(self->GetMultiShotParams(weaponType));
	}

	PosVel* L_EntityPlayer_GetMultiShotPositionVelocity(Entity_Player* self, int loopIndex, WeaponType weapon, Vector* shotDirection, float shotSpeed, Weapon_MultiShotParams* params) {
		return new auto(self->GetMultiShotPositionVelocity(loopIndex, weapon, * shotDirection, shotSpeed, *params));
	}

	const char* L_EntityPlayer_GetName(Entity_Player* self) {
		return self->GetName();
	}
	int L_EntityPlayer_GetNextUrethraBlockFrame(Entity_Player* self) {
		return *self->GetNextUrethraBlockFrame();
	}
	Entity* L_EntityPlayer_GetNPCTarget(Entity_Player* self) {
		return self->GetNPCTarget();
	}

	int L_EntityPlayer_GetNumBlueFlies(Entity_Player* self) {
		return self->_blueFlies;
	}

	int L_EntityPlayer_GetNumBlueSpiders(Entity_Player* self) {
		return self->_blueSpiders;
	}

	int L_EntityPlayer_GetNumBombs(Entity_Player* self) {
		return self->_numBombs;
	}

	int L_EntityPlayer_GetNumCoins(Entity_Player* self) {
		return self->_numCoins;
	}

	int L_EntityPlayer_GetNumGigaBombs(Entity_Player* self) {
		return self->_numGigaBombs;
	}

	int L_EntityPlayer_GetNumKeys(Entity_Player* self) {
		return self->_numKeys;
	}

	Entity_Player* L_EntityPlayer_GetOtherTwin(Entity_Player* self) {
		return self->_twinPlayer;
	}

	int L_EntityPlayer_GetPeeBurstCooldown(Entity_Player* self) {
		return *self->GetPeeBurstCooldown();
	}

	int L_EntityPlayer_GetPill(Entity_Player* self, int slotId) {
		return self->GetPill(slotId);
	}

	RNG* L_EntityPlayer_GetPillRNG(Entity_Player* self, int id) {
		return self->GetPillRNG(id);
	}

	int L_EntityPlayer_GetPlayerFormCounter(Entity_Player* self, int form) {
		if (form < 0 || form > 14) return 0;
		return self->_playerForms[form];
	}

	int L_EntityPlayer_GetPlayerType(Entity_Player* self) {
		return self->_type;
	}
	PocketItem L_EntityPlayer_GetPocketItem(Entity_Player* self, int slotId) {
		return *self->GetPocketItem(slotId);
	}

	int L_EntityPlayer_GetPonyCharge(Entity_Player* self) {
		return *self->GetPonyCharge();
	}

	int L_EntityPlayer_GetPurityState(Entity_Player* self) {
		return *self->GetPurityState();
	}

	int L_EntityPlayer_GetPoopMana(Entity_Player* self) {
		return self->_poopMana;
	}

	int L_EntityPlayer_GetPoopSpell(Entity_Player* self, int position) {
		return self->_poopSpellQueue[position];
	}

	Vector* L_EntityPlayer_GetRecentMovementVector(Entity_Player* self) {
		return new auto(self->_recentMovementVector);
	}
	int L_EntityPlayer_GetRedStewBonusDuration(Entity_Player* self) {
		return *self->GetRedStewBonusDuration();
	}

	int L_EntityPlayer_GetRottenHearts(Entity_Player* self) {
		return *self->GetRottenHearts();
	}

	Vector* L_EntityPlayer_GetShootingInput(Entity_Player* self) {
		return new auto(self->GetShootingInput(0.0f));
	}

	int L_EntityPlayer_GetShotSpeedModifier(Entity_Player* self) {
		return self->_shotSpeedModifier;
	}

	// !!!TODO!!!
	void* L_EntityPlayer_GetSmeltedTrinkets(Entity_Player* self) {
		return &self->_smeltedTrinkets;
	}

	int L_EntityPlayer_GetSpecialGridCollision(Entity_Player* self, Vector* pos) {
		return self->GetSpecialGridCollision(pos);
	}

	int L_EntityPlayer_GetSpeedModifier(Entity_Player* self) {
		return self->_speedModifier;
	}

	int L_EntityPlayer_GetTotalActiveCharge(Entity_Player* self, int activeSlot) {
		return self->GetTotalActiveCharge(activeSlot);
	}

	void* L_EntityPlayer_GetVoidedCollectiblesList(Entity_Player* self) {
		return &self->GetVoidedCollectiblesList().at(0);
	}

	Weapon* L_EntityPlayer_GetWeapon(Entity_Player* self, int index) {
		if (index < 0 || index > 4) return nullptr;
		return *self->GetWeapon(index);
	}
	
	int L_EntityPlayer_GetWeaponModifiers(Entity_Player* self) {
		return self->GetWeaponModifiers();
	}

	int L_EntityPlayer_GetWildCardItem(Entity_Player* self) {
		return *self->GetWildCardItem();
	}

	uint8_t L_EntityPlayer_GetWildCardItemType(Entity_Player* self) {
		return *self->GetWildCardItemType();
	}

	// !!!TODO!!!
	void* L_EntityPlayer_GetWispCollectiblesList(Entity_Player* self) {
		return self->GetItemWispsList();
	}

	float L_EntityPlayer_GetSmoothBodyRotation(Entity_Player* self) {
		return self->_smoothBodyRotation;
	}

	int L_EntityPlayer_GetSoulCharge(Entity_Player* self) {
		return self->_soulCharges;
	}

	int L_EntityPlayer_GetSoulHearts(Entity_Player* self) {
		return *self->GetSoulHearts();
	}

	Entity_Player* L_EntityPlayer_GetSubPlayer(Entity_Player* self) {
		return self->_subPlayer;
	}

	TearParams* L_EntityPlayer_GetTearHitParams(Entity_Player* self, int weaponType, float damageScale, int tearDisplacement, Entity* source) {
		return new auto(self->GetTearHitParams(weaponType, damageScale, tearDisplacement, source));
	}

	Vector* L_EntityPlayer_GetTearMovementInheritance(Entity_Player* self, Vector* shotDirection) {
		return new auto(self->GetTearMovementInheritance(shotDirection, false));
	}

	float L_EntityPlayer_GetTearPoisonDamage(Entity_Player* self) {
		return *self->GetTearPoisonDamage();
	}

	int L_EntityPlayer_GetTearRangeModifier(Entity_Player* self) {
		return self->_tearRangeModifier;
	}

	int L_EntityPlayer_GetTotalDamageTaken(Entity_Player* self) {
		return self->_totalDamageTaken;
	}

	Entity* L_EntityPlayer_GetTractorBeam(Entity_Player* self) {
		return self->_tractorBeam;
	}

	// TODO: return ItemConfig::Item here
	int L_EntityPlayer_GetTrinket(Entity_Player* self, int trinketIndex) {
		return self->GetTrinket(trinketIndex)->id;
	}

	int L_EntityPlayer_GetTrinketMultiplier(Entity_Player* self, TrinketType trinketID) {
		return self->GetTrinketMultiplier(trinketID);
	}

	RNG* L_EntityPlayer_GetTrinketRNG(Entity_Player* self, TrinketType trinketID) {
		return self->GetTrinketRNG(trinketID);
	}

	Vector* L_EntityPlayer_GetVelocityBeforeUpdate(Entity_Player* self) {
		return &self->_velocityBeforeUpdate;
	}

	int L_EntityPlayer_GetZodiacEffect(Entity_Player* self) {
		return self->GetZodiacEffect();
	}

	bool L_EntityPlayer_HasCollectible(Entity_Player* self, CollectibleType type, bool ignoreModifiers) {
		return self->HasCollectible(type, ignoreModifiers);
	}

	bool L_EntityPlayer_HasCurseMistEffect(Entity_Player* self) {
		return self->_hasCurseMistEffect;
	}

	bool L_EntityPlayer_HasFullHearts(Entity_Player* self) {
		return self->HasFullHearts();
	}

	bool L_EntityPlayer_HasFullHeartsAndSoulHearts(Entity_Player* self) {
		return self->HasFullHeartsAndSoulHearts();
	}

	bool L_EntityPlayer_HasGoldenBomb(Entity_Player* self) {
		return self->_hasGoldenBomb;
	}

	bool L_EntityPlayer_HasGoldenKey(Entity_Player* self) {
		return self->_hasGoldenKey;
	}

	bool L_EntityPlayer_HasInstantDeathCurse(Entity_Player* self) {
		return self->HasInstantDeathCurse();
	}

	bool L_EntityPlayer_HasInvincibility(Entity_Player* self, int dmgFlags) {
		return self->HasInvincibility(dmgFlags);
	}

	bool L_EntityPlayer_HasPoisonImmunity(Entity_Player* self) {
		return self->HasPoisonImmunity();
	}

	bool L_EntityPlayer_HasPlayerForm(Entity_Player* self, int form) {
		return self->HasPlayerForm(form);
	}

	bool L_EntityPlayer_HasTimedItem(Entity_Player* self) {
		return self->HasTimedItem();
	}

	bool L_EntityPlayer_HasTrinket(Entity_Player* self, TrinketType type, bool ignoreModifiers) {
		return self->HasTrinket(type, ignoreModifiers);
	}

	bool L_EntityPlayer_HasWeaponType(Entity_Player* self, WeaponType weaponType) {
		return self->_weaponTypeBool[weaponType];
	}

	void L_EntityPlayer_IncrementPlayerFormCounter(Entity_Player* self, int ePlayerForm, int count) {
		self->IncrementPlayerFormCounter(ePlayerForm, count);
	}

	void L_EntityPlayer_InitBabySkin(Entity_Player* self) {
		return self->InitBabySkin();
	}

	void L_EntityPlayer_InitPostLevelInitStats(Entity_Player* self) {
		self->InitPostLevelInitStats();
	}

	void L_EntityPlayer_InitTwin(Entity_Player* self, int playerType) {
		self->InitTwin(playerType);
	}

	bool L_EntityPlayer_IsBlackHeart(Entity_Player* self, int heart) {
		return self->IsBlackHeart(heart);
	}

	bool L_EntityPlayer_IsBoneHeart(Entity_Player* self, int heart) {
		return self->IsBoneHeart(heart);
	}

	bool L_EntityPlayer_IsCollectibleAnimFinished(Entity_Player* self, CollectibleType type, const char* anim) {
		return self->IsCollectibleAnimFinished(type, std::string(anim));
	}

	bool L_EntityPlayer_IsCollectibleCostumeVisibleLayerID(Entity_Player* self, CollectibleType type, int playerSpriteLayerID) {
		return self->IsCollectibleCostumeVisible(type, playerSpriteLayerID);
	}

	bool L_EntityPlayer_IsCollectibleCostumeVisibleLayerName(Entity_Player* self, CollectibleType type, const char* layer) {
		LayerState* layerState = self->_sprite.GetLayer(layer);
		if (layerState != nullptr) {
			return self->IsCollectibleCostumeVisible(type, layerState->GetLayerID());
		}
		return false;
	}

	bool L_EntityPlayer_IsCoopGhost(Entity_Player* self) {
		return self->_isCoopGhost;
	}

	bool L_EntityPlayer_IsEntityValidTarget(Entity_Player* self, Entity* ent) {
		return self->IsEntityValidTarget(ent);
	}

	bool L_EntityPlayer_IsExtraAnimationFinished(Entity_Player* self) {
		return self->IsExtraAnimationFinished();
	}

	bool L_EntityPlayer_IsFootstepFrame(Entity_Player* self, int foot) {
		return self->IsFootstepFrame(foot);
	}

	bool L_EntityPlayer_IsFullSpriteRendering(Entity_Player* self) {
		return self->IsFullSpriteRendering();
	}

	bool L_EntityPlayer_IsHeadless(Entity_Player* self) {
		return self->IsHeadless();
	}

	bool L_EntityPlayer_IsHeldItemVisible(Entity_Player* self) {
		return self->IsHeldItemVisible();
	}

	bool L_EntityPlayer_IsHoldingItem(Entity_Player* self) {
		return self->IsHoldingItem();
	}

	bool L_EntityPlayer_IsHologram(Entity_Player* self) {
		return self->IsHologram();
	}

	bool L_EntityPlayer_IsInvisible(Entity_Player* self) {
		return self->IsInvisible();
	}

	bool L_EntityPlayer_IsItemCostumeVisible(Entity_Player* self, ItemConfig_Item* item, int playerSpriteLayerID) {
		return self->IsItemCostumeVisible(item, playerSpriteLayerID);
	}

	bool L_EntityPlayer_IsItemCostumeVisibleLayerName(Entity_Player* self, ItemConfig_Item* item, const char* playerSpriteLayerName) {
		LayerState* layerState = self->_sprite.GetLayer(playerSpriteLayerName);
		if (layerState != nullptr) {
			return self->IsItemCostumeVisible(item, layerState->GetLayerID());
		}
		return false;
	}

	bool L_EntityPlayer_IsItemQueueEmpty(Entity_Player* self) {
		return self->IsItemQueueEmpty();
	}

	bool L_EntityPlayer_IsLocalPlayer(Entity_Player* self) {
		return self->IsLocalPlayer();
	}

	bool L_EntityPlayer_IsNullItemCostumeVisible(Entity_Player* self, int nullItem, int layerID) {
		return self->IsNullItemCostumeVisible(nullItem, layerID);
	}

	bool L_EntityPlayer_IsNullItemCostumeVisibleLayerName(Entity_Player* self, int nullItem, const char* playerSpriteLayerName) {
		LayerState* layerState = self->_sprite.GetLayer(playerSpriteLayerName);
		if (layerState != nullptr) {
			return self->IsNullItemCostumeVisible(nullItem, layerState->GetLayerID());
		}
		return false;
	}

	bool L_EntityPlayer_IsP2Appearing(Entity_Player* self) {
		return self->IsP2Appearing();
	}

	bool L_EntityPlayer_IsPacifist(Entity_Player* self) {
		return g_Game->_level._room->_pacifist;
	}

	bool L_EntityPlayer_IsPosInSpotLight(Entity_Player* self, Vector* position) {
		return self->IsPosInSpotLight(position);
	}

	bool L_EntityPlayer_IsSubPlayer(Entity_Player* self) {
		return self->IsSubPlayer();
	}

	bool L_EntityPlayer_IsUrethraBlocked(Entity_Player* self) {
		return *self->IsUrethraBlocked();
	}

	void L_EntityPlayer_MorphToCoopGhost(Entity_Player* self) {
		self->MorphToCoopGhost();
	}

	bool L_EntityPlayer_NeedsCharge(Entity_Player* self, int activeSlot) {
		return self->NeedsCharge(activeSlot);
	}

	void L_EntityPlayer_PlayCollectibleAnim(Entity_Player* self, CollectibleType item, bool checkBodyLayers, const char* anim, int frame) {
		self->PlayCollectibleAnim(item, checkBodyLayers, std::string(anim), frame, false);
	}
	
	void L_EntityPlayer_PlayDelayedSFX(Entity_Player* self, int soundID, int soundDelay, int frameDelay, float volume) {
		self->PlayDelayedSFX(soundID, soundDelay, frameDelay, volume);
	}

	void L_EntityPlayer_PlayExtraAnimation(Entity_Player* self, const char* animation) {
		self->PlayExtraAnimation(animation);
	}

	void L_EntityPlayer_QueueExtraAnimation(Entity_Player* self, const char* animation) {
		self->QueueExtraAnimation(animation);
	}

	// need some help on this one too
	void L_EntityPlayer_QueueItem(Entity_Player* self, ItemConfig_Item* item, int charge, bool touched, bool golden, int varData) {
		self->QueueItem(item, charge, touched, varData);
	}

	void L_EntityPlayer_RemoveBlackHeart(Entity_Player* self, int blackHeart) {
		self->RemoveBlackHeart(blackHeart);
	}

	void L_EntityPlayer_RemoveBlueFly(Entity_Player* self) {
		self->RemoveBlueFly();
	}

	void L_EntityPlayer_RemoveBlueSpider(Entity_Player* self) {
		self->RemoveBlueSpider();
	}

	void L_EntityPlayer_RemoveCollectible(Entity_Player* self, CollectibleType type, bool ignoreModifiers, int activeSlot, bool removeFromPlayerForm) {
		self->RemoveCollectible(type, ignoreModifiers, activeSlot, removeFromPlayerForm);
	}

	void L_EntityPlayer_RemoveCollectibleByHistoryIndex(Entity_Player* self, int idx) {
		self->RemoveCollectibleByHistoryIndex(idx, true);
	}

	void L_EntityPlayer_RemoveCostume(Entity_Player* self, ItemConfig_Item* item) {
		self->RemoveCostume(item);
	}

	void L_EntityPlayer_RemoveCurseMistEffect(Entity_Player* self) {
		self->RemoveCurseMistEffect();
	}

	void L_EntityPlayer_RemoveGoldenBomb(Entity_Player* self) {
		self->RemoveGoldenBomb();
	}

	void L_EntityPlayer_RemoveGoldenKey(Entity_Player* self) {
		self->RemoveGoldenKey();
	}

	void L_EntityPlayer_RemovePocketItem(Entity_Player* self, int activeSlot) {
		self->RemovePocketItem(activeSlot);
	}

	void L_EntityPlayer_RemovePoopSpell(Entity_Player* self, int pos) {
		if (pos < 0 || pos > 5) {
			return;
		}

		for (int i = pos; i < 5; i++) {
			self->_poopSpellQueue[i] = self->_poopSpellQueue[i + 1];
		}
		self->_poopSpellQueue[5] = 0;
		self->CheckPoopSpellQueue();
	}

	void L_EntityPlayer_RemoveSkinCostume(Entity_Player* self) {
		self->RemoveSkinCostume();
	}

	void L_EntityPlayer_RenderBody(Entity_Player* self, Vector* position) {
		self->RenderBody(position);
	}

	void L_EntityPlayer_RenderDebugInfo(Entity_Player* self, Vector* position) {
		self->RenderDebugInfo(position);
	}

	void L_EntityPlayer_RenderGlow(Entity_Player* self, Vector* position) {
		self->RenderGlow(position);
	}

	void L_EntityPlayer_RenderHead(Entity_Player* self, Vector* position) {
		self->RenderHead(position);
	}

	void L_EntityPlayer_RenderHeadBack(Entity_Player* self, Vector* position) {
		self->RenderHeadBack(position);
	}

	bool L_EntityPlayer_RenderShadowLayer(Entity_Player* self, Vector* position) {
		return self->RenderShadowLayer(position);
	}

	void L_EntityPlayer_RenderTop(Entity_Player* self, Vector* position) {
		self->RenderTop(position);
	}

	void L_EntityPlayer_ReplaceCostumeSprite(Entity_Player* self, ItemConfig_Item* item, const char* spritePath, int spriteId) {
		self->ReplaceCostumeSprite(item, std::string(spritePath), spriteId);
	}

	void L_EntityPlayer_RerollAllCollectibles(Entity_Player* self, RNG* rng, bool includeActiveItems) {
		self->RerollAllCollectibles(rng, includeActiveItems);
	}

	void L_EntityPlayer_ResetDamageCooldown(Entity_Player* self) {
		self->_damageCooldown = 0;
	}

	void L_EntityPlayer_ResetItemState(Entity_Player* self) {
		self->ResetItemState();
	}

	void L_EntityPlayer_ResetPlayer(Entity_Player* self) {
		self->ResetPlayer();
	}

	void L_EntityPlayer_RespawnFamiliars(Entity_Player* self) {
		self->RespawnFamiliars();
	}

	void L_EntityPlayer_Revive(Entity_Player* self) {
		self->Revive();
	}

	bool L_EntityPlayer_ReviveCoopGhost(Entity_Player* self) {
		if (self->_isCoopGhost) {
			self->RevivePlayerGhost();
			return true;
		}

		return false;
	}

	void L_EntityPlayer_SalvageCollectiblePickup(Entity_Player* self, Entity_Pickup* pickup, RNG* rng, eItemPoolType pool, bool removePickup) {
		if (rng == nullptr) {
			rng = &pickup->_dropRNG;
		}

		if (removePickup) pickup->Remove();
		self->SalvageCollectible(pickup->GetPosition(), pickup->_subtype, rng->Next(), pool);
	}

	void L_EntityPlayer_SalvageCollectibleID(Entity_Player* self, CollectibleType collectible, Vector* pos, RNG* rng, eItemPoolType pool) {
		if (pos == nullptr) {
			pos = self->GetPosition();
		}
		if (rng == nullptr) {
			rng = &self->_dropRNG;
		}
		self->SalvageCollectible(pos, collectible, rng->Next(), pool);
	}

	void L_EntityPlayer_SetActiveCharge(Entity_Player* self, int charge, int activeSlot) {
		self->SetActiveCharge(charge, activeSlot);
	}

	void L_EntityPlayer_SetActiveVarData(Entity_Player* self, int vardata, int slot) {
		self->SetActiveVarData(vardata, slot);
	}

	void L_EntityPlayer_SetBagOfCraftingContent(Entity_Player* self, BagOfCraftingPickup* pickups) {
		memcpy(&self->_bagOfCraftingContent, pickups, sizeof(int) * 8);
	}

	void L_EntityPlayer_SetBagOfCraftingOutput(Entity_Player* self, CollectibleType output) {
		*self->GetBagOfCraftingOutput() = output;
	}

	void L_EntityPlayer_SetBagOfCraftingSlot(Entity_Player* self, int slot, BagOfCraftingPickup pickup) {
		self->_bagOfCraftingContent[slot] = pickup;
	}

	void L_EntityPlayer_SetBladderCharge(Entity_Player* self, int charge) {
		*self->GetBladderCharge() = charge;
	}

	void L_EntityPlayer_SetBloodCharge(Entity_Player* self, int amount) {
		self->_bloodCharges = amount;
	}
	
	void L_EntityPlayer_SetCambionConceptionState(Entity_Player* self, int state) {
		*self->GetCambionConceptionState() = state;
	}

	void L_EntityPlayer_SetCanShoot(Entity_Player* self, bool canShoot) {
		*self->GetCanShoot() = canShoot;
	}

	void L_EntityPlayer_SetCard(Entity_Player* self, int slotId, int card) {
		self->SetCard(slotId, card);
	}

	void L_EntityPlayer_SetControllerIndex(Entity_Player* self, int idx) {
		self->SetControllerIndex(idx);
	}

	void L_EntityPlayer_SetDamageModifier(Entity_Player* self, int modifier) {
		self->_damageModifier = modifier;
	}

	void L_EntityPlayer_SetEdenDamage(Entity_Player* self, float value) {
		*self->GetEdenDamage() = value;
	}

	void L_EntityPlayer_SetEdenFireDelay(Entity_Player* self, float value) {
		*self->GetEdenFireDelay() = value;
	}

	void L_EntityPlayer_SetEdenLuck(Entity_Player* self, float value) {
		*self->GetEdenLuck() = value;
	}

	void L_EntityPlayer_SetEdenRange(Entity_Player* self, float value) {
		*self->GetEdenRange() = value;
	}

	void L_EntityPlayer_SetEdenShotSpeed(Entity_Player* self, float value) {
		*self->GetEdenShotSpeed() = value;
	}

	void L_EntityPlayer_SetEdenSpeed(Entity_Player* self, float value) {
		*self->GetEdenSpeed() = value;
	}

	void L_EntityPlayer_SetEveSumptoriumCharge(Entity_Player* self, int charge) {
		self->_eveSumptoriumCharge = charge;
	}

	void L_EntityPlayer_SetFireDelayModifier(Entity_Player* self, int modifier) {
		self->_fireDelayModifier = modifier;
	}

	void L_EntityPlayer_SetFootprintColor(Entity_Player* self, KColor* color, bool rightFoot) {
		self->SetFootprintColor(color, rightFoot);
	}

	void L_EntityPlayer_SetFullHearts(Entity_Player* self) {
		self->SetFullHearts();
	}

	void L_EntityPlayer_SetGnawedLeafTimer(Entity_Player* self, int timer) {
		self->_gnawedLeafCooldown = timer;
	}

	void L_EntityPlayer_SetImmaculateConceptionState(Entity_Player* self, int state) {
		*self->GetImmaculateConceptionState() = state;
	}

	void L_EntityPlayer_SetItemState(Entity_Player* self, CollectibleType item) {
		self->SetItemState(item);
	}

	void L_EntityPlayer_SetKeepersSackBonus(Entity_Player* self, int bonus) {
		self->_keepersSackBonus = bonus;
	}

	void L_EntityPlayer_SetLaserColor(Entity_Player* self, ColorMod* colorMod) {
		self->_laserColor = *colorMod;
	}

	void L_EntityPlayer_SetLuckModifier(Entity_Player* self, int modifier) {
		self->_luckModifier = modifier;
	}

	void L_EntityPlayer_SetMaggySwingCooldown(Entity_Player* self, int cooldown) {
		self->_maggySwingCooldown = cooldown;
	}

	void L_EntityPlayer_SetMaxBladderCharge(Entity_Player* self, int charge) {
		*self->GetMaxBladderCharge() = charge;
	}

	void L_EntityPlayer_SetMegaBlastDuration(Entity_Player* self, int duration) {
		*self->GetMegaBlastDuration() = duration;
	}

	void L_EntityPlayer_SetMinDamageCooldown(Entity_Player* self, int damageCooldown) {
		self->SetMinDamageCooldown(damageCooldown);
	}

	void L_EntityPlayer_SetNextUrethraBlockFrame(Entity_Player* self, int frame) {
		*self->GetNextUrethraBlockFrame() = frame;
	}

	void L_EntityPlayer_SetPill(Entity_Player* self, int slotId, int pill) {
		self->SetPill(slotId, pill);
	}

	void L_EntityPlayer_SetPocketActiveItem(Entity_Player* self, CollectibleType type, int slot, bool keepInPools) {
		self->SetPocketActiveItem(type, slot, keepInPools);
	}

	void L_EntityPlayer_SetPonyCharge(Entity_Player* self, int time) {
		*self->GetPonyCharge() = time;
	}

	void L_EntityPlayer_SetPoopSpell(Entity_Player* self, int slot, int spell) {
		if (slot < 0 || slot > 5) {
			return;
		}
		if (spell < 1 || spell > 11) {
			// At least until we decide to add custom PoopSpellType support :^)
			return;
		}
		self->_poopSpellQueue[slot] = spell;
	}

	void L_EntityPlayer_SetPurityState(Entity_Player* self, int state) {
		*self->GetPurityState() = state;
	}

	void L_EntityPlayer_SetRedStewBonusDuration(Entity_Player* self, int duration) {
		*self->GetRedStewBonusDuration() = duration;
	}

	void L_EntityPlayer_SetShootingCooldown(Entity_Player* self, int cooldown) {
		self->SetShootingCooldown(cooldown);
	}

	void L_EntityPlayer_SetShotSpeedModifier(Entity_Player* self, int modifier) {
		self->_shotSpeedModifier = modifier;
	}

	void L_EntityPlayer_SetSoulCharge(Entity_Player* self, int amount) {
		self->_soulCharges = amount;
	}

	void L_EntityPlayer_SetSpeedModifier(Entity_Player* self, int modifier) {
		self->_speedModifier = modifier;
	}

	void L_EntityPlayer_SetTearPoisonDamage(Entity_Player* self, float poisonDamage) {
		*self->GetTearPoisonDamage() = poisonDamage;
	}

	void L_EntityPlayer_SetTearRangeModifier(Entity_Player* self, int modifier) {
		self->_tearRangeModifier = modifier;
	}

	void L_EntityPlayer_SetUrethraBlock(Entity_Player* self, bool blocked) {
		*self->IsUrethraBlocked() = blocked;
	}

	void L_EntityPlayer_SetWeapon(Entity_Player* self, Weapon* weapon, int slot) {
		if (slot < 0 || slot > 4) return;
		*self->GetWeapon(slot) = weapon;
	}

	void L_EntityPlayer_ShootBlueCandle(Entity_Player* self, Vector* dir) {
		self->ShootBlueCandle(dir);
	}
	void L_EntityPlayer_ShootRedCandle(Entity_Player* self, Vector* dir) {
		self->ShootRedCandle(dir);
	}

	void L_EntityPlayer_ShuffleCostumes(Entity_Player* self, int seed) {
		if (seed == 0) seed = Isaac::genrand_int32();
		self->ShuffleCostumes(seed);
	}

	Entity_Effect* L_EntityPlayer_SpawnAquariusCreep(Entity_Player* self, TearParams* tearParams) {
		TearParams params;

		if (tearParams == nullptr) {
			params = self->GetTearHitParams((int)WeaponType::WEAPON_TEARS, (*self->GetTearPoisonDamage() * 0.666f) / self->_damage, (-(int)(Isaac::Random(2) != 0) & 2) - 1, 0);
		}
		else {
			params = *tearParams;
		}

		Entity_Effect* effect = (Entity_Effect*)g_Game->Spawn(1000, 54, *self->GetPosition(), Vector(0.0, 0.0), self, 0, Random(), 0);
		float random = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		effect->_sprite._scale *= ((random * 0.5f) + 0.2f);
		effect->_collisionDamage = params._tearDamage;
		effect->SetColor(&params._tearColor, 0, -1, true, false);

		effect->_varData = params._flags;
		effect->Update();

		return effect;
	}

	void L_EntityPlayer_SpawnClot(Entity_Player* self, Vector* pos, bool allowPlayerDeath) {
		self->SpawnClot(pos, allowPlayerDeath);
	}

	Entity_Laser* L_EntityPlayer_SpawnMawOfVoid(Entity_Player* self, int timeout) {
		return self->SpawnMawOfVoid(timeout);
	}

	int L_EntityPlayer_SpawnSaturnusTears(Entity_Player* self) {
		return self->SpawnSaturnusTears();
	}

	void L_EntityPlayer_StopExtraAnimation(Entity_Player* self) {
		self->StopExtraAnimation();
	}

	void L_EntityPlayer_SwapActiveItems(Entity_Player* self) {
		self->SwapActiveItems();
	}

	void L_EntityPlayer_SwapForgottenForm(Entity_Player* self, bool force, bool noEffects) {
		self->SwapForgottenForm(force, noEffects);
	}

	void L_EntityPlayer_SyncConsumableCounts(Entity_Player* self, Entity_Player* player2, int flags) {
		self->SyncConsumableCounts(player2, flags);
	}

	void L_EntityPlayer_Teleport(Entity_Player* self, Vector* pos, bool doEffects, bool teleportTwinPlayers) {
		self->Teleport(pos, doEffects, teleportTwinPlayers);
	}
	Entity* L_EntityPlayer_ThrowBlueSpider(Entity_Player* self, Vector* pos, Vector* target) {
		return self->ThrowBlueSpider(pos, target);
	}

	Entity_Familiar* L_EntityPlayer_ThrowFriendlyDip(Entity_Player* self, int subtype, Vector* pos, Vector* target) {
		return self->ThrowFriendlyDip(subtype, pos, self, target);
	}

	Entity* L_EntityPlayer_ThrowHeldEntity(Entity_Player* self, Vector* vel) {
		return self->ThrowHeldEntity(vel);
	}

	void L_EntityPlayer_TriggerBookOfVirtues(Entity_Player* self, CollectibleType type, int charge) {
		self->TriggerBookOfVirtues(type, charge);
	}

	void L_EntityPlayer_TriggerRoomClear(Entity_Player* self) {
		self->TriggerRoomClear();
	}

	void L_EntityPlayer_TryAddToBagOfCrafting(Entity_Player* self, Entity_Pickup* pickup) {
		self->TryAddToBagOfCrafting(pickup);
	}

	void L_EntityPlayer_TryDecreaseGlowingHourglassUses(Entity_Player* self, int uses, bool forceHourglass) {
		self->TryDecreaseGlowingHourglassUses(uses, forceHourglass);
	}

	bool L_EntityPlayer_TryFakeDeath(Entity_Player* self) {
		return self->TryFakeDeath();
	}

	bool L_EntityPlayer_TryForgottenThrow(Entity_Player* self, Vector* dir) {
		return self->TryForgottenThrow(dir);
	}

	bool L_EntityPlayer_TryHoldEntity(Entity_Player* self, Entity* entity) {
		return self->TryHoldEntity(entity);
	}

	bool L_EntityPlayer_TryHoldTrinket(Entity_Player* self, TrinketType type) {
		return self->TryHoldTrinket(type);
	}

	bool L_EntityPlayer_TryPreventDeath(Entity_Player* self) {
		return self->TryPreventDeath();
	}

	void L_EntityPlayer_TryRemoveCollectibleCostume(Entity_Player* self, CollectibleType collectible, bool keepPersistent) {
		self->TryRemoveCollectibleCostume(collectible, keepPersistent);
	}

	void L_EntityPlayer_TryRemoveNullCostume(Entity_Player* self, int nullId) {
		self->TryRemoveNullCostume(nullId);
	}

	bool L_EntityPlayer_TryRemoveTrinket(Entity_Player* self, int type) {
		return self->TryRemoveTrinket(type);
	}

	void L_EntityPlayer_TryRemoveTrinketCostume(Entity_Player* self, int trinket) {
		self->TryRemoveTrinketCostume(trinket);
	}

	void L_EntityPlayer_TryRemoveSmeltedTrinket(Entity_Player* self, int id) {
		self->TryRemoveSmeltedTrinket(id);
	}

	bool L_EntityPlayer_TryUseKey(Entity_Player* self) {
		return self->TryUseKey();
	}

	void L_EntityPlayer_UpdateCanShoot(Entity_Player* self) {
		self->UpdateCanShoot();
	}

	void L_EntityPlayer_UpdateIsaacPregnancy(Entity_Player* self, bool updateCambion) {
		self->UpdateIsaacPregnancy(updateCambion);
	}

	void L_EntityPlayer_UseActiveItem(Entity_Player* self, CollectibleType item, int flags, int slot, int customVarData) {
		self->UseActiveItem(item, flags, slot, customVarData);
	}

	void L_EntityPlayer_UseActiveItemAbp(Entity_Player* self, CollectibleType item, bool showAnim, bool keepActiveItem, bool allowNonMainPlayer, bool toAddCostume, int slot, int customVarData) {
		int flags = !showAnim | !keepActiveItem << 4 | allowNonMainPlayer << 3 | !toAddCostume << 1;
		self->UseActiveItem(item, flags, slot, customVarData);
	}

	void L_EntityPlayer_UseCard(Entity_Player* self, int id, int useFlags) {
		self->UseCard(id, useFlags);
	}

	void L_EntityPlayer_UsePill(Entity_Player* self, int id, int pillColor, int useFlags) {
		self->UsePill(id, pillColor, useFlags);
	}

	void L_EntityPlayer_UsePoopSpell(Entity_Player* self, int type) {
		self->UsePoopSpell(type);
	}

	bool L_EntityPlayer_VoidHasCollectible(Entity_Player* self, CollectibleType type) {
		return self->VoidHasCollectible(type);
	}

	bool L_EntityPlayer_WillPlayerRevive(Entity_Player* self) {
		return self->WillRevive();
	}
}

std::map<int, int> fakeItems;

LUA_FUNCTION(Lua_GetMultiShotPositionVelocity) // This *should* be in the API, but magically vanished some point after 1.7.8.
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY, "EntityPlayer");
	int loopIndex = (int)luaL_checkinteger(L, 2);
	int weaponType = (int)luaL_checkinteger(L, 3);
	Vector* shotDirection = lua::GetUserdata<Vector*>(L, 4, lua::Metatables::VECTOR, "Vector");
	float shotSpeed = (float)luaL_checknumber(L, 5);

	Weapon_MultiShotParams* multiShotParams = lua::GetUserdata<Weapon_MultiShotParams*>(L, 6, lua::metatables::MultiShotParamsMT);
	if (multiShotParams->numTears < loopIndex) {
		return luaL_argerror(L, 2, "LoopIndex cannot be higher than MultiShotParams.NumTears");
	};

	lua::luabridge::UserdataValue<PosVel>::push(L, lua::GetMetatableKey(lua::Metatables::POS_VEL), player->GetMultiShotPositionVelocity(loopIndex, (WeaponType)weaponType, *shotDirection, shotSpeed, *multiShotParams));

	return 1;
}

LUA_FUNCTION(Lua_InitTwin)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int playerType = (int)luaL_checkinteger(L, 2);

	Entity_Player* twinPlayer = player->InitTwin(playerType);
	if (!twinPlayer) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, twinPlayer, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	
	return 1;
}

LUA_FUNCTION(Lua_InitPostLevelInitStats)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->InitPostLevelInitStats();

	return 0;
}

LUA_FUNCTION(Lua_PlayerSetItemState)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType item = (CollectibleType)luaL_checkinteger(L, 2);

	player->SetItemState(item);

	return 0;
}

LUA_FUNCTION(Lua_PlayerAddCacheFlags)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int flags = (int)luaL_checkinteger(L, 2);
	bool evaluateCache = lua::luaL_optboolean(L, 3, false);

	player->AddCacheFlags(flags);
	if (evaluateCache) {
		player->EvaluateItems();
	}

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetHealthType)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetHealthType());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetTotalActiveCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = (int)luaL_checkinteger(L, 2);

	lua_pushinteger(L, player->GetTotalActiveCharge(slot));
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetActiveMaxCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = (int)luaL_checkinteger(L, 2);

	lua_pushinteger(L, player->GetActiveMaxCharge(slot));
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetActiveMinUsableCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = (int)luaL_checkinteger(L, 2);

	lua_pushinteger(L, player->GetActiveMinUsableCharge(slot));
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetActiveVarData) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int vardata = (int)luaL_checkinteger(L, 2);
	int slot = (int)luaL_checkinteger(L, 3);

	player->SetActiveVarData(vardata, slot);
	return 0;
}

LUA_FUNCTION(Lua_PlayerAddActiveCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	unsigned int charge = (unsigned int)luaL_checkinteger(L, 2);
	int slot = (int)luaL_checkinteger(L, 3);
	bool flashHUD = lua::luaL_optboolean(L, 4, true);
	bool overcharge = lua::luaL_optboolean(L, 5, false);
	bool force = lua::luaL_optboolean(L, 6, false);

	int ret = player->AddActiveCharge(charge, slot, flashHUD, overcharge, force);
	lua_pushinteger(L, ret);

	return 1;
}

LUA_FUNCTION(Lua_PlayerDropCollectible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int collectible = (int)luaL_checkinteger(L, 2);
	Entity_Pickup* pickup = nullptr;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		pickup = lua::GetUserdata<Entity_Pickup*>(L, 3, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	}
	bool removeFromForm = lua::luaL_optboolean(L, 4, false);

	player->DropCollectible(collectible, pickup, removeFromForm);
	if (!pickup) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, pickup, lua::Metatables::ENTITY_PICKUP);
	}
	
	return 1;
}

LUA_FUNCTION(Lua_PlayerDropCollectibleByHistoryIndex) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int idx = (int)luaL_checkinteger(L, 2);
	Entity_Pickup* pickup = nullptr;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		pickup = lua::GetUserdata<Entity_Pickup*>(L, 3, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	}

	player->DropCollectibleByHistoryIndex(idx, pickup);
	if (!pickup) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, pickup, lua::Metatables::ENTITY_PICKUP);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerIncrementPlayerFormCounter) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int ePlayerForm = (int)luaL_checkinteger(L, 2);
	int num = (int)luaL_checkinteger(L, 3);

	player->IncrementPlayerFormCounter(ePlayerForm, num);
	return 0;
}

LUA_FUNCTION(Lua_PlayerTryPreventDeath) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->TryPreventDeath());
	return 1;
}

LUA_FUNCTION(lua_PlayerRemoveCollectibleByHistoryIndex) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int index = (int)luaL_checkinteger(L, 2);
	player->RemoveCollectibleByHistoryIndex(index, true);
	return 0;
}

LUA_FUNCTION(Lua_PlayerSetCanShoot)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	bool canShoot = lua::luaL_checkboolean(L, 2);
	*player->GetCanShoot() = canShoot;
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetDeadEyeCharge)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetDeadEyeCharge());
	return 1;
}

LUA_FUNCTION(Lua_PlayerTeleport)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* position = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	bool doEffects = lua::luaL_optboolean(L, 3, true);
	bool teleportTwinPlayers = lua::luaL_optboolean(L, 4, false);

	player->Teleport(position, doEffects, teleportTwinPlayers);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetMegaBlastDuration)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetMegaBlastDuration());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetMegaBlastDuration)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetMegaBlastDuration() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetActiveItemDesc)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int index = (int)luaL_optinteger(L, 2, 0);
	if (index > 3) {
		return luaL_argerror(L, 2, "ActiveSlot cannot be higher than 3");
	}
	ActiveItemDesc* desc = player->GetActiveItemDesc(index);
	if (!desc) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, desc, lua::GetMetatableKey(lua::Metatables::ACTIVE_ITEM_DESC));
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetActiveItemSlot)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetActiveItemSlot((int)luaL_checkinteger(L, 2)));
	return 1;
}

LUA_FUNCTION(Lua_PlayerTryFakeDeath)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->TryFakeDeath());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetBoCContent) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_newtable(L);
	BagOfCraftingPickup* content = player->GetBagOfCraftingContent();
	for (int i = 0; i < 8; ++i) {
		lua_pushinteger(L, i + 1);
		lua_pushinteger(L, *content++);
		lua_rawset(L, -3);
	}
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBoCContent) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	if (!lua_istable(L, 2))
	{
		return luaL_error(L, "EntityPlayer:SetBagOfCraftingContent: Expected a table as second argument");
	}

	BagOfCraftingPickup list[8]{};
	size_t length = (size_t)lua_objlen(L, 2);
	if (length > 0)
	{
		if (length > 8) {
			return luaL_error(L, "EntityPlayer:SetBagOfCraftingContent: Table cannot be larger than 8 pickups");
		}

		size_t index;
		for (index = 0; index < length; index++)
		{
			lua_rawgeti(L, 2, index + 1);
			int pickup = (int)luaL_checkinteger(L, -1);
			lua_pop(L, 1);
			if (pickup < 0 || pickup > 29) {
				return luaL_error(L, "EntityPlayer:SetBagOfCraftingContent: Invalid pickup %d at index %d", pickup, index + 1);
			}
			list[index] = (BagOfCraftingPickup)pickup;
		}
	}
	memcpy(&player->_bagOfCraftingContent, list, sizeof(list));

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetBoCSlot) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = (int)luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		return luaL_error(L, "EntityPlayer:GetBagOfCraftingSlot: invalid slot id %d\n", slot);
	}

	lua_pushinteger(L, player->GetBagOfCraftingContent()[slot]);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBoCSlot) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");

	int slot = (int)luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		return luaL_error(L, "EntityPlayer::GetBagOfCraftingSlot: invalid slot id %d\n", slot);
	}

	int8_t pickup = (int8_t)luaL_optinteger(L, 3, BagOfCraftingPickup::BOC_NONE);
	if (pickup < 0 || pickup >= BagOfCraftingPickup::BOC_MAX) {
		return luaL_error(L, "EntityPlayer::SetBagOfCraftingSlot: invalid pickup id %d\n", pickup);
	}

	player->GetBagOfCraftingContent()[slot] = (BagOfCraftingPickup)pickup;
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetBagOfCraftingOutput)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetBagOfCraftingOutput());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBagOfCraftingOutput)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetBagOfCraftingOutput() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetSpeedModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_speedModifier);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetSpeedModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_speedModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetFireDelayModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_fireDelayModifier);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetFireDelayModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_fireDelayModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetDamageModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_damageModifier);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetDamageModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_damageModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerSetTearRangeModifier) // ._.
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_tearRangeModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetShotSpeedModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_shotSpeedModifier);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetShotSpeedModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_shotSpeedModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetLuckModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_luckModifier);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetLuckModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_luckModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetRedStewBonusDuration)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetRedStewBonusDuration());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetRedStewBonusDuration)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetRedStewBonusDuration() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetWeaponModifiers)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetWeaponModifiers());
	return 1;
}

LUA_FUNCTION(Lua_PlayerEnableWeaponType)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	WeaponType weaponType = (WeaponType)luaL_checkinteger(L, 2);
	bool set = lua::luaL_checkboolean(L, 3);
	player->EnableWeaponType(weaponType, set);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetD8DamageModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8DamageModifier());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetD8SpeedModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8SpeedModifier());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetD8RangeModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8RangeModifier());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetD8FireDelayModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8FireDelayModifier());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetEpiphoraCharge)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetEpiphoraCharge());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetPeeBurstCooldown)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetPeeBurstCooldown());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetMaxPeeBurstCooldown)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetMaxPeeBurstCooldown());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetMetronomeCollectibleID)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetMetronomeCollectibleID());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetMarkedTarget) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity_Effect* target = player->GetMarkedTarget();
	if (!target) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, target, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
	}
	return 1;
}

LUA_FUNCTION(Lua_PlayerIsLocalPlayer) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->IsLocalPlayer());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetWildCardItem) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetWildCardItem());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetWildCardItemType) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetWildCardItemType());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetWeapon) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	WeaponData* weaponData = lua::GetUserdata<WeaponData*>(L, 2, lua::metatables::WeaponMT);
	int index = (int)luaL_checkinteger(L, 3);
	if (index < 0 || index > 4) {
		return luaL_argerror(L, 2, "Index must be between 0 and 4");
	}
	*player->GetWeapon(index) = weaponData->weapon;
	weaponData->owner = player;
	weaponData->slot = index;

	return 0;
}

LUA_FUNCTION(Lua_PlayerAddLocust) {
	Entity_Player* ent = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int collectibleType = (int)luaL_checkinteger(L, 2);
	Vector* pos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	Isaac::SpawnLocust(ent, collectibleType, pos);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetPonyCharge) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetPonyCharge());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetPonyCharge) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetPonyCharge() = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenSpeed) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenSpeed());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenSpeed) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenSpeed() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenFireDelay) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenFireDelay());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenFireDelay) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenFireDelay() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenDamage) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenDamage());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenDamage) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenDamage() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenRange) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenRange());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenRange) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenRange() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenShotSpeed) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenShotSpeed());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenShotSpeed) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenShotSpeed() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenLuck) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenLuck());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenLuck) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenLuck() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerTriggerRoomClear) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	plr->TriggerRoomClear();

	return 0;

}

LUA_FUNCTION(Lua_PlayerShuffleCostumes) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	unsigned int seed = (unsigned int)luaL_optinteger(L, 2, min(Isaac::genrand_int32(), 1));
	plr->ShuffleCostumes(seed);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetCollectiblesList)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	std::vector<int>& collectibleInv = plr->GetCollectiblesList();

	lua_newtable(L);

	for (size_t i = 1; i < collectibleInv.size(); i++) {
		lua_pushinteger(L, i);
		lua_pushinteger(L, collectibleInv[i]);
		lua_rawset(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetPurityState) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetPurityState());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetPurityState) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetPurityState() = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerSetTearPoisonDamage) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetTearPoisonDamage() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetVoidedCollectiblesList)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	std::vector<int>& collecitbleInv = plr->GetVoidedCollectiblesList();

	lua_newtable(L);
	int idx = 1;
	for (int collectible : collecitbleInv) {
		lua_pushinteger(L, idx);
		lua_pushinteger(L, collectible);
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerAddInnateCollectible)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	/*std::set<uint32_t>& itemWispList = *plr->GetItemWispsList();
	itemWispList.insert(luaL_checkinteger(L,2));
	*/

	const int collectibleID = (int)luaL_checkinteger(L, 2);
	const int amount = (int)luaL_optinteger(L, 3, 1);

	ItemConfig itemConfig = g_Manager->_itemConfig;
	ItemConfig_Item* item = itemConfig.GetCollectibles()[0][collectibleID];

	//ItemConfig_Item* item_ptr = item;
	if (item == nullptr) {
		return luaL_error(L, "Invalid item");
	}

	std::map<int, int>& wispMap = *plr->GetItemWispsList();
	wispMap[collectibleID] += amount;

	if (amount > 0 && item->addCostumeOnPickup) {
		plr->AddCostume(item, false);
	}

	plr->AddCacheFlags(item->cacheFlags);
	plr->EvaluateItems();

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetWispCollecitblesList)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	/*std::set<uint32_t> collectibles = *plr->GetItemWispsList();

	lua_newtable(L);
	int idx = 1;
	for (int collectible : collectibles) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, collectible);
		lua_settable(L, -3);
		idx++;
	}
	*/

	std::map<int, int> wispMap = plr->_itemWispsList;

	lua_newtable(L);

	for (auto& item : wispMap) {
		lua_pushinteger(L, item.first); // push the collectible
		lua_pushinteger(L, item.second); // push the amount
		lua_settable(L, -3); // set the table entry
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetImmaculateConceptionState)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetImmaculateConceptionState());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetImmaculateConceptionState)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetImmaculateConceptionState() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetCambionConceptionState)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetCambionConceptionState());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetCambionConceptionState)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetCambionConceptionState() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerUpdateIsaacPregnancy)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	bool cambion = lua::luaL_checkboolean(L, 2);
	plr->UpdateIsaacPregnancy(cambion);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetCambionPregnancyLevel)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, plr->GetCambionPregnancyLevel());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetBladderCharge)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetBladderCharge());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBladderCharge)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetBladderCharge() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetMaxBladderCharge)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetMaxBladderCharge());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetMaxBladderCharge)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetMaxBladderCharge() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerIsUrethraBlocked)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, *plr->IsUrethraBlocked());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetUrethraBlock)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->IsUrethraBlocked() = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetNextUrethraBlockFrame)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetNextUrethraBlockFrame());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetNextUrethraBlockFrame)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetNextUrethraBlockFrame() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetHeldSprite)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua::luabridge::UserdataPtr::push(L, plr->GetHeldSprite(), lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetHeldEntity)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity* heldEntity = *plr->GetHeldEntity();
	if (!heldEntity) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, heldEntity, lua::GetMetatableKey(lua::Metatables::ENTITY));
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetActiveWeaponNumFired)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Weapon* wep = nullptr;

	wep = *plr->GetWeapon(0);
	if ((wep == nullptr) && ((wep = *plr->GetWeapon(1)) == nullptr)) {
		lua_pushnil(L);
	}
	else
	{
		lua_pushinteger(L, wep->GetNumFired());
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetPoopSpell)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	const int pos = (int)luaL_checkinteger(L, 2);
	const int spell = (int)luaL_checkinteger(L, 3);

	if (pos < 0 || pos > 5) {
		return luaL_argerror(L, 2, "Invalid Poop Spell queue position");
	}
	if (spell < 1 || spell > 11) {
		// At least until we decide to add custom PoopSpellType support :^)
		return luaL_argerror(L, 3, "Invalid PoopSpellType");
	}

	plr->_poopSpellQueue[pos] = spell;

	return 0;
}

LUA_FUNCTION(Lua_PlayerRemovePoopSpell)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	const int pos = (int)luaL_optinteger(L, 2, 0);

	if (pos < 0 || pos > 5) {
		return luaL_argerror(L, 2, "Invalid Poop Spell queue position");
	}

	for (int i = pos; i < 5; i++) {
		plr->_poopSpellQueue[i] = plr->_poopSpellQueue[i + 1];
	}
	plr->_poopSpellQueue[5] = 0;
	plr->CheckPoopSpellQueue();

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetBackupPlayer) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity_Player* backupPlayer = plr->_backupPlayer;

	if (!backupPlayer) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, backupPlayer, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}

	return 1;
}

LUA_FUNCTION(Lua_ClearDeadEyeCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	if (lua_toboolean(L, 2)) {
		player->_deadEyeCharges = 0;
		player->_deadEyeMisses = 0;
		player->_cacheFlags |= 1;
		player->EvaluateItems();
	}
	else {
		player->ClearDeadEyeCharge();
	}

	return 0;
}

LUA_FUNCTION(Lua_SwapForgottenForm) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	bool IgnoreHealth = lua::luaL_optboolean(L, 2, false);
	bool NoEffects = lua::luaL_optboolean(L, 3, false);
	player->SwapForgottenForm(IgnoreHealth, NoEffects);
	return 0;
}

//LUA_FUNCTION(Lua_SpawnAquariusCreep) {
//	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
//	TearParams params;
//
//	if (lua_gettop(L) >= 2) {
//		params = *lua::GetUserdata<TearParams*>(L, 2, lua::Metatables::TEAR_PARAMS, "TearParams");
//	}
//	else {
//		player->GetTearHitParams(params, (int)WeaponType::WEAPON_TEARS, (*player->GetTearPoisonDamage() * 0.666f) / player->_damage, (-(int)(Isaac::Random(2) != 0) & 2) - 1, 0);
//	}
//
//	Entity_Effect* effect = (Entity_Effect*)g_Game->Spawn(1000, 54, *player->GetPosition(), Vector(0.0, 0.0), player, 0, Random(), 0);
//
//	if (!effect) {
//		lua_pushnil(L);
//	}
//	else
//	{
//		float random = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
//		effect->_sprite._scale *= ((random * 0.5f) + 0.2f);
//		effect->_collisionDamage = params._tearDamage;
//		effect->SetColor(&params._tearColor, 0, -1, true, false);
//
//		effect->_varData = params._flags;
//		effect->Update();
//
//		lua::luabridge::UserdataPtr::push(L, effect, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
//	}
//
//	return 1;
//}

LUA_FUNCTION(Lua_PlayerGetBabySkin) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_babySkin);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBabySkin) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_babySkin = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetMaggySwingCooldown) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_maggySwingCooldown);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetMaggySwingCooldown) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_maggySwingCooldown = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerPlayDelayedSFX) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	unsigned int soundEffectID = (unsigned int)luaL_checkinteger(L, 2);
	int soundDelay = (int)luaL_optinteger(L, 3, 0);
	int frameDelay = (int)luaL_optinteger(L, 4, 2);
	float volume = (float)luaL_optnumber(L, 5, 1.0f);
	player->PlayDelayedSFX(soundEffectID, soundDelay, frameDelay, volume);

	return 0;
}

LUA_FUNCTION(Lua_PlayerCanUsePill) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	const int pillEffect = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, player->CanUsePill(pillEffect));

	return 1;
}

LUA_FUNCTION(Lua_PlayerAddSmeltedTrinket) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	const int trinketID = (int)luaL_checkinteger(L, 2);
	const bool firstTime = lua::luaL_optboolean(L, 3, true);

	lua_pushboolean(L, player->AddSmeltedTrinket(trinketID, firstTime));
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetSmeltedTrinkets) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	std::vector<SmeltedTrinketDesc>& smeltedTrinkets = player->_smeltedTrinkets;

	lua_newtable(L);
	for (size_t i = 1; i < smeltedTrinkets.size(); i++) {
		lua_pushinteger(L, i); 

		lua_newtable(L);

		lua_pushstring(L, "trinketAmount");
		lua_pushinteger(L, smeltedTrinkets[i]._trinketNum);
		lua_settable(L, -3);

		lua_pushstring(L, "goldenTrinketAmount");
		lua_pushinteger(L, smeltedTrinkets[i]._goldenTrinketNum);
		lua_settable(L, -3);

		lua_settable(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetCostumeLayerMap)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	PlayerCostumeMap* costumeLayerMap = plr->_playerCostumeMap;

	lua_newtable(L);
	for (int idx = 0; idx < 15; idx++) {
		lua_pushinteger(L, idx + 1);

		lua_newtable(L);

		lua_pushstring(L, "costumeIndex");
		lua_pushinteger(L, costumeLayerMap[idx]._index);
		lua_settable(L, -3);

		lua_pushstring(L, "layerID");
		lua_pushinteger(L, costumeLayerMap[idx]._layerID);
		lua_settable(L, -3);

		lua_pushstring(L, "priority");
		lua_pushinteger(L, costumeLayerMap[idx]._priority);
		lua_settable(L, -3);

		lua_pushstring(L, "isBodyLayer");
		lua_pushboolean(L, costumeLayerMap[idx]._isBodyLayer);
		lua_settable(L, -3);

		lua_settable(L, -3);
	}
	return 1;
}


LUA_FUNCTION(Lua_PlayerIsItemCostumeVisible) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	ItemConfig_Item* item = lua::GetUserdata<ItemConfig_Item*>(L, 2, lua::Metatables::ITEM, "Item");
	int layerID = 0;
	if (lua_type(L, 3) == LUA_TSTRING) {
		const char* layerName = luaL_checkstring(L, 3);
		LayerState* layerState = plr->_sprite.GetLayer(layerName);
		if (layerState != nullptr) {
			layerID = layerState->GetLayerID();
		}
		else
		{
			return luaL_error(L, "Invalid layer name %s", layerName);
		}
	}
	else {
		layerID = (int)luaL_checkinteger(L, 3);
		if (layerID < 0 || (const unsigned int)layerID + 1 > plr->_sprite.GetLayerCount()) {
			return luaL_error(L, "Invalid layer ID %d", layerID);
		}
	}

	lua_pushboolean(L, plr->IsItemCostumeVisible(item, layerID));
	return 1;
}

LUA_FUNCTION(Lua_PlayerIsCollectibleCostumeVisible) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType collectibleType = (CollectibleType)luaL_checkinteger(L, 2);
	int layerID = 0;
	if (lua_type(L, 3) == LUA_TSTRING) {
		const char* layerName = luaL_checkstring(L, 3);
		LayerState* layerState = plr->_sprite.GetLayer(layerName);
		if (layerState != nullptr) {
			layerID = layerState->GetLayerID();
		}
		else
		{
			return luaL_error(L, "Invalid layer name %s", layerName);
		}
	}
	else {
		layerID = (int)luaL_checkinteger(L, 3);
		if (layerID < 0 || (const unsigned int)layerID + 1 > plr->_sprite.GetLayerCount()) {
			return luaL_error(L, "Invalid layer ID %d", layerID);
		}
	}

	lua_pushboolean(L, plr->IsCollectibleCostumeVisible(collectibleType, layerID));
	return 1;
}

LUA_FUNCTION(Lua_PlayerIsNullItemCostumeVisible) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int nullItem = (int)luaL_checkinteger(L, 2);
	int layerID = 0;
	if (lua_type(L, 3) == LUA_TSTRING) {
		const char* layerName = luaL_checkstring(L, 3);
		LayerState* layerState = plr->_sprite.GetLayer(layerName);
		if (layerState != nullptr) {
			layerID = layerState->GetLayerID();
		}
		else
		{
			return luaL_error(L, "Invalid layer name %s", layerName);
		}
	}
	else {
		layerID = (int)luaL_checkinteger(L, 3);
		if (layerID < 0 || (const unsigned int)layerID + 1 > plr->_sprite.GetLayerCount()) {
			return luaL_error(L, "Invalid layer ID %d", layerID);
		}
	}

	lua_pushboolean(L, plr->IsNullItemCostumeVisible(nullItem, layerID));
	return 1;
}


LUA_FUNCTION(Player_PlayCollectibleAnim) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType collectibleType = (CollectibleType)luaL_checkinteger(L, 2);
	bool unk = lua::luaL_checkboolean(L, 3);
	std::string animName = luaL_checkstring(L, 4);
	int frameNum = (int)luaL_optinteger(L, 5, -1);

	plr->PlayCollectibleAnim(collectibleType, unk, animName, frameNum, false);
	return 0;
}

LUA_FUNCTION(Player_IsCollectibleAnimFinished) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType collectibleType = (CollectibleType)luaL_checkinteger(L, 2);
	std::string animName = luaL_checkstring(L, 3);

	lua_pushboolean(L, plr->IsCollectibleAnimFinished(collectibleType, animName));
	return 1;
}

LUA_FUNCTION(Player_ClearCollectibleAnim) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType collectibleType = (CollectibleType)luaL_checkinteger(L, 2);

	plr->ClearCollectibleAnim(collectibleType);
	return 0;
}

static void FamiliarStorageToLua(lua_State* L, std::vector<Entity_Familiar*>& familiars) {
	lua_newtable(L);
	for (size_t i = 0; i < familiars.size(); ++i) {
		lua_pushinteger(L, i + 1);
		lua::luabridge::UserdataPtr::push(L, familiars[i], lua::GetMetatableKey(lua::Metatables::ENTITY_FAMILIAR));
		lua_rawset(L, -3);
	}

	familiarsStorage.familiars.clear();
	familiarsStorage.inUse = false;
}

LUA_FUNCTION(Lua_EntityPlayer_CheckFamiliarEx) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int variant = (int)luaL_checkinteger(L, 2);
	int targetCount = (int)luaL_checkinteger(L, 3);
	RNG* rng = lua::GetUserdata<RNG*>(L, 4, lua::Metatables::RNG, lua::metatables::RngMT);
	ItemConfig_Item* configPtr = nullptr;
	if (lua_type(L, 5) == LUA_TUSERDATA) {
		configPtr = lua::GetUserdata<ItemConfig_Item*>(L, 5, lua::Metatables::ITEM, "ItemConfigItem");
	}
	
	int subtype = (int)luaL_optinteger(L, 6, -1);

	std::vector<Entity_Familiar*>& familiars = InitFamiliarStorage();
	plr->CheckFamiliar(variant, targetCount, rng, configPtr, subtype);
	FamiliarStorageToLua(L, familiars);

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetEveSumptoriumCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_eveSumptoriumCharge);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEveSumptoriumCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_eveSumptoriumCharge = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetPlayerFormCounter) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int playerFormType = (int)luaL_checkinteger(L, 2);

	if (playerFormType >= 0 && playerFormType <= 14) {
		lua_pushinteger(L, player->_playerForms[playerFormType]);
	}
	else {
		return luaL_error(L, "Invalid PlayerForm %d", playerFormType);
	}
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetMaxPocketItems) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetMaxPocketItems());

	return 1;
}

LUA_FUNCTION(Lua_PlayerAddBoneOrbital) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* position = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	player->AddBoneOrbital(position);

	return 0;
}

/*
// this seems to be super hardcoded to not work outside of cantripped
LUA_FUNCTION(Lua_PlayerAddItemCard) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	if (g_Manager->_itemConfig.GetCollectible(id) == nullptr) {
		std::string error("Invalid collectible ID ");
		error.append(std::to_string(id));
		return luaL_argerror(L, 2, error.c_str());
	}
	lua_pushinteger(L, player->AddItemCard(id));

	return 1;
}
*/

LUA_FUNCTION(Lua_PlayerAddLeprosy) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->AddLeprosy();

	return 0;
}

LUA_FUNCTION(Lua_PlayerAddUrnSouls) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int count = (int)luaL_checkinteger(L, 2);
	player->AddUrnSouls(count);

	return 0;
}

LUA_FUNCTION(Lua_PlayerCanAddCollectibleToInventory) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, player->CanAddCollectibleToInventory(id));

	return 1;
}

LUA_FUNCTION(Lua_PlayerCanOverrideActiveItem) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	if (id < 0 || id > 3) {
		std::string error("Invalid slot ID ");
		error.append(std::to_string(id));
		return luaL_argerror(L, 2, error.c_str());
	}
	lua_pushboolean(L, player->CanOverrideActiveItem(id));

	return 1;
}

LUA_FUNCTION(Lua_PlayerClearItemAnimCollectible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	player->ClearItemAnimCollectible(id);

	return 0;
}

LUA_FUNCTION(Lua_PlayerClearItemAnimNullItems) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->ClearItemAnimNullItems();

	return 0;
}

/*
// Spawns club, immediately kills it. Needs investigation
LUA_FUNCTION(Lua_PlayerFireBoneClub) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity* parent = nullptr;
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		parent = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	}
	int variant = (int)luaL_checkinteger(L, 3);
	bool unk = lua::luaL_checkboolean(L, 4);

	player->FireBoneClub(parent, variant, unk);

	return 0;
}
*/


// might need asm patch to retrieve laser. this is wacky and can spawn both an effect and a laser
LUA_FUNCTION(Lua_PlayerFireBrimstoneBall) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector* vel = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	Vector offset;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		offset = *lua::GetUserdata<Vector*>(L, 4, lua::Metatables::VECTOR, "Vector");
	}

	Entity_Effect* effect = player->FireBrimstoneBall(*pos, *vel, offset, 0, 0, nullptr);
	if (!effect) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, effect, lua::Metatables::ENTITY_EFFECT);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetBodyMoveDirection) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");

	Vector dir = player->GetBodyMoveDirection();
	lua::luabridge::UserdataPtr::push(L, &dir, lua::Metatables::VECTOR);

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetDeathAnimName) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushstring(L, player->GetDeathAnimName());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetGlitchBabySubType) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetGlitchBabySubType());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetGreedsGulletHearts) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetGreedsGulletHearts());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetSpecialGridCollision) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector pos = *player->GetPosition();
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		pos = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	}

	lua_pushinteger(L, player->GetSpecialGridCollision(&pos));
	return 1;
}

LUA_FUNCTION(Lua_PlayerCanCrushRocks) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->CanCrushRocks());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetEnterPosition) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector dir = player->GetEnterPosition();
	lua::luabridge::UserdataPtr::push(L, &dir, lua::Metatables::VECTOR);
	return 1;
}

/*
// needs return register override support
LUA_FUNCTION(Lua_PlayerGetExplosionRadiusMultiplier) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	BitSet128* flags = lua::GetUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, "BitSet128");
	lua_pushnumber(L, player->GetExplosionRadiusMultiplier(*flags));
	return 1;
}
*/

LUA_FUNCTION(Lua_PlayerGetFocusEntity) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity* entity = player->GetFocusEntity();
	if (!entity)
	{
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, entity, lua::Metatables::ENTITY);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetGlyphOfBalanceDrop) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int variant = (int)luaL_optinteger(L, 2, -1);
	int subtype = (int)luaL_optinteger(L, 3, -1);
	player->GetGlyphOfBalanceDrop(&variant, &subtype);

	lua_newtable(L);
	lua_pushinteger(L, 1);
	lua_pushinteger(L, variant);
	lua_rawset(L, -3);
	lua_pushinteger(L, 2);
	lua_pushinteger(L, subtype);
	lua_rawset(L, -3);

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetLaserColor) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua::luabridge::UserdataPtr::push(L, &player->_laserColor, lua::Metatables::COLOR);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetLaserColor) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_laserColor = *lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");

	return 0;
}

/*
// needs return register override support
LUA_FUNCTION(Lua_PlayerGetSoundPitch) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, player->GetSoundPitch());

	return 1;
}


LUA_FUNCTION(Lua_PlayerGetSalvationScale) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, player->GetSalvationScale());

	return 1;
}
*/

LUA_FUNCTION(Lua_PlayerHasInstantDeathCurse) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->HasInstantDeathCurse());

	return 1;
}

LUA_FUNCTION(Lua_PlayerHasPoisonImmunity) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->HasPoisonImmunity());

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsEntityValidTarget) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity* target = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");

	lua_pushboolean(L, player->IsEntityValidTarget(target));

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsFootstepFrame) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int foot = (int)luaL_optinteger(L, 2, -1);
	if (foot < -1 || foot > 1) {
		std::string error("Invalid foot ID ");
		error.append(std::to_string(foot));
		error.append(", valid range is -1 to 1");
		return luaL_argerror(L, 2, error.c_str());
	}
	lua_pushboolean(L, player->IsFootstepFrame(foot));

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsHeadless) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->IsHeadless());

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsHologram) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->IsHologram());

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsInvisible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->IsInvisible());

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsPacifist) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, g_Game->_level._room->_pacifist);

	return 1;
}

LUA_FUNCTION(Lua_PlayerMorphToCoopGhost) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->MorphToCoopGhost();

	return 0;
}

LUA_FUNCTION(Lua_PlayerRemovePocketItem) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	if (id < 0 || id > 3) {
		std::string error("Invalid slot ID ");
		error.append(std::to_string(id));
		return luaL_argerror(L, 2, error.c_str());
	}
	player->RemovePocketItem(id);

	return 0;
}

LUA_FUNCTION(Lua_PlayerRerollAllCollectibles) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	RNG* rng;
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		rng = lua::GetUserdata<RNG*>(L, 2, lua::Metatables::RNG, "RNG");
	}
	else
	{
		rng = &player->_dropRNG;
	}

	bool includeActives = lua::luaL_optboolean(L, 3, false);

	player->RerollAllCollectibles(rng, includeActives);

	return 0;
}

LUA_FUNCTION(Lua_PlayerResetPlayer) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->ResetPlayer();

	return 0;
}

LUA_FUNCTION(Lua_PlayerReviveCoopGhost) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	bool res = false;
	
	//printf("IsPlayerGhost: %s\n", player->_isCoopGhost ? "TRUE" : "FALSE");
	if (player->_isCoopGhost) {
		//printf("attempting revive");
		player->RevivePlayerGhost();
		res = true;
	}
	
	lua_pushboolean(L, res);
	return 1;
}

int ValidatePool(lua_State* L, unsigned int pos)
{
	int ret = (int)luaL_optinteger(L, pos, -1);
	if (ret > 30 || ret < 0) {
		ret = -1;
	}
	return ret;
}

LUA_FUNCTION(Lua_PlayerSalvageCollectible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* pos = nullptr;
	RNG* rng = nullptr;
	unsigned int subtype, seed;
	int pool = -1;

	// pickup override
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 2, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
		subtype = pickup->_subtype;
		pos = pickup->GetPosition();
		if (lua_type(L, 3) == LUA_TUSERDATA) {
			rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");
		}
		else
		{
			rng = &pickup->_dropRNG;
		}
		pool = ValidatePool(L, 4);

		pickup->Remove();
	}
	// CollectibleType override
	else {
		subtype = (int)luaL_checkinteger(L, 2);
		if (lua_type(L, 3) == LUA_TUSERDATA) {
			pos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
		}
		else
		{
			pos = player->GetPosition();
		}
		if (lua_type(L, 4) == LUA_TUSERDATA) {
			rng = lua::GetUserdata<RNG*>(L, 4, lua::Metatables::RNG, "RNG");
		}
		else
		{
			rng = &player->_dropRNG;
		}
		pool = ValidatePool(L, 5);
	}
	seed = rng->Next();

	player->SalvageCollectible(pos, subtype, seed, pool);
	return 0;
}

LUA_FUNCTION(Lua_PlayerSetControllerIndex) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int idx = (int)luaL_checkinteger(L, 2);
	player->SetControllerIndex(idx);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetFootprintColor) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	KColor* color = nullptr;
	if (lua::luaL_checkboolean(L, 2)) {
		color = &player->_footprintColor2;
	}
	else
	{
		color = &player->_footprintColor1;
	}
	lua::luabridge::UserdataPtr::push(L, color, lua::Metatables::KCOLOR);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetFootprintColor) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	KColor* color = lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
	bool unk = lua::luaL_optboolean(L, 3, false);
	player->SetFootprintColor(color, unk);

	return 0;
}

// todo: asm patch to return effect
LUA_FUNCTION(Lua_PlayerShootBlueCandle) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* dir = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	player->ShootBlueCandle(dir);
	return 0;
}

// not sure if this returns the clot or not
LUA_FUNCTION(Lua_PlayerSpawnClot) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	bool canKillPlayer = lua::luaL_optboolean(L, 3, false);
	player->SpawnClot(pos, canKillPlayer);
	return 0;
}

//  todo: asm patch to return tears
LUA_FUNCTION(Lua_PlayerSpawnSaturnusTears) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->SpawnSaturnusTears());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSyncConsumableCounts) {
	Entity_Player* player1 = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity_Player* player2 = lua::GetUserdata<Entity_Player*>(L, 2, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int bitflags = (int)luaL_checkinteger(L, 3);
	player1->SyncConsumableCounts(player2, bitflags);
	return 0;
}

LUA_FUNCTION(Lua_PlayerTryAddToBagOfCrafting) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 2, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	player->TryAddToBagOfCrafting(pickup);
	return 0;
}

LUA_FUNCTION(Lua_PlayerTryDecreaseGlowingHourglassUses) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int unk1 = (int)luaL_checkinteger(L, 2);
	bool unk2 = lua::luaL_checkboolean(L, 3);
	player->TryDecreaseGlowingHourglassUses(unk1, unk2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerTryForgottenThrow) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* dir = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	lua_pushboolean(L, player->TryForgottenThrow(dir));
	return 1;
}

LUA_FUNCTION(Lua_PlayerTryRemoveSmeltedTrinket) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	unsigned int id = (int)luaL_checkinteger(L, 2);
	player->TryRemoveSmeltedTrinket(id);
	return 0;
}

/*
// this seems to rely on a struct of door outline effects that doesn't exist when red key or cracked key aren't in possession
LUA_FUNCTION(Lua_PlayerUseRedKey) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->UseRedKey();
	return 0;
}
*/

LUA_FUNCTION(Lua_PlayerVoidHasCollectible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, player->VoidHasCollectible(id));
	return 1;
}

/*
// doesn't seem to work
LUA_FUNCTION(Lua_PlayerAttachMinecart) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity_NPC* minecart = lua::GetUserdata<Entity_NPC*>(L, 2, lua::Metatables::ENTITY_NPC, "EntityNPC");
	player->AttachMinecart(minecart);
	return 0;
}
*/

LUA_FUNCTION(Lua_PlayerGetKeepersSackBonus) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_keepersSackBonus);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetKeepersSackBonus) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_keepersSackBonus = (int)luaL_checkinteger(L, 2);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetMultiShotPositionVelocity", Lua_GetMultiShotPositionVelocity },
		{ "InitTwin", Lua_InitTwin },
		{ "InitPostLevelInitStats", Lua_InitPostLevelInitStats },
		{ "SetItemState", Lua_PlayerSetItemState },
		{ "AddCacheFlags", Lua_PlayerAddCacheFlags },
		{ "GetHealthType", Lua_PlayerGetHealthType },
		{ "GetTotalActiveCharge", Lua_PlayerGetTotalActiveCharge },
		{ "GetActiveMaxCharge", Lua_PlayerGetActiveMaxCharge },
		{ "GetActiveMinUsableCharge", Lua_PlayerGetActiveMinUsableCharge },
		{ "SetActiveVarData", Lua_PlayerSetActiveVarData },
		{ "AddActiveCharge", Lua_PlayerAddActiveCharge },
		{ "DropCollectible", Lua_PlayerDropCollectible },
		{ "DropCollectibleByHistoryIndex", Lua_PlayerDropCollectibleByHistoryIndex },
		{ "IncrementPlayerFormCounter", Lua_PlayerIncrementPlayerFormCounter },
		{ "TryPreventDeath", Lua_PlayerTryPreventDeath },
		{ "SetCanShoot", Lua_PlayerSetCanShoot },
		{ "GetDeadEyeCharge", Lua_PlayerGetDeadEyeCharge },
		{ "RemoveCollectibleByHistoryIndex", lua_PlayerRemoveCollectibleByHistoryIndex },
		{ "Teleport", Lua_PlayerTeleport },
		{ "GetMegaBlastDuration", Lua_PlayerGetMegaBlastDuration },
		{ "SetMegaBlastDuration", Lua_PlayerSetMegaBlastDuration },
		{ "GetActiveItemDesc", Lua_PlayerGetActiveItemDesc },
		{ "GetActiveItemSlot", Lua_PlayerGetActiveItemSlot },
		{ "TryFakeDeath", Lua_PlayerTryFakeDeath },
		{ "GetBagOfCraftingContent", Lua_PlayerGetBoCContent },
		{ "SetBagOfCraftingContent", Lua_PlayerSetBoCContent },
		{ "SetBagOfCraftingSlot", Lua_PlayerSetBoCSlot },
		{ "GetBagOfCraftingSlot", Lua_PlayerGetBoCSlot },
		{ "GetBagOfCraftingOutput", Lua_PlayerGetBagOfCraftingOutput },
		{ "SetBagOfCraftingOutput", Lua_PlayerSetBagOfCraftingOutput },
		{ "GetSpeedModifier", Lua_PlayerGetSpeedModifier },
		{ "SetSpeedModifier", Lua_PlayerSetSpeedModifier },
		{ "GetFireDelayModifier", Lua_PlayerGetFireDelayModifier },
		{ "SetFireDelayModifier", Lua_PlayerSetFireDelayModifier },
		{ "GetDamageModifier", Lua_PlayerGetDamageModifier },
		{ "SetDamageModifier", Lua_PlayerSetDamageModifier },
		{ "SetTearRangeModifier", Lua_PlayerSetTearRangeModifier }, // .-.
		{ "GetShotSpeedModifier", Lua_PlayerGetShotSpeedModifier },
		{ "SetShotSpeedModifier", Lua_PlayerSetShotSpeedModifier },
		{ "GetLuckModifier", Lua_PlayerGetLuckModifier },
		{ "SetLuckModifier", Lua_PlayerSetLuckModifier },
		{ "GetRedStewBonusDuration", Lua_PlayerGetRedStewBonusDuration },
		{ "SetRedStewBonusDuration", Lua_PlayerSetRedStewBonusDuration },
		{ "GetWeaponModifiers", Lua_PlayerGetWeaponModifiers },
		{ "EnableWeaponType", Lua_PlayerEnableWeaponType },
		{ "GetD8DamageModifier", Lua_PlayerGetD8DamageModifier },
		{ "GetD8SpeedModifier", Lua_PlayerGetD8SpeedModifier },
		{ "GetD8RangeModifier", Lua_PlayerGetD8RangeModifier },
		{ "GetD8FireDelayModifier", Lua_PlayerGetD8FireDelayModifier },
		{ "GetEpiphoraCharge", Lua_PlayerGetEpiphoraCharge },
		{ "GetPeeBurstCooldown", Lua_PlayerGetPeeBurstCooldown },
		{ "GetMaxPeeBurstCooldown", Lua_PlayerGetMaxPeeBurstCooldown },
		{ "GetMetronomeCollectibleID", Lua_PlayerGetMetronomeCollectibleID },
		{ "GetMarkedTarget", Lua_PlayerGetMarkedTarget },
		{ "IsLocalPlayer", Lua_PlayerIsLocalPlayer },
		{ "GetWildCardItem", Lua_PlayerGetWildCardItem },
		{ "GetWildCardItemType", Lua_PlayerGetWildCardItemType },
		{ "SetWeapon", Lua_PlayerSetWeapon },
		{ "AddLocust", Lua_PlayerAddLocust },
		{ "GetPonyCharge", Lua_PlayerGetPonyCharge },
		{ "SetPonyCharge", Lua_PlayerSetPonyCharge },
		{ "GetEdenSpeed", Lua_PlayerGetEdenSpeed },
		{ "SetEdenSpeed", Lua_PlayerSetEdenSpeed },
		{ "GetEdenFireDelay", Lua_PlayerGetEdenFireDelay },
		{ "SetEdenFireDelay", Lua_PlayerSetEdenFireDelay },
		{ "GetEdenDamage", Lua_PlayerGetEdenDamage },
		{ "SetEdenDamage", Lua_PlayerSetEdenDamage },
		{ "GetEdenRange", Lua_PlayerGetEdenRange },
		{ "SetEdenRange", Lua_PlayerSetEdenRange },
		{ "GetEdenShotSpeed", Lua_PlayerGetEdenShotSpeed },
		{ "SetEdenShotSpeed", Lua_PlayerSetEdenShotSpeed },
		{ "GetEdenLuck", Lua_PlayerGetEdenLuck },
		{ "SetEdenLuck", Lua_PlayerSetEdenLuck },
		{ "TriggerRoomClear", Lua_PlayerTriggerRoomClear },
		{ "GetCollectiblesList", Lua_PlayerGetCollectiblesList },
		{ "ShuffleCostumes", Lua_PlayerShuffleCostumes },
		{ "GetPurityState", Lua_PlayerGetPurityState },
		{ "SetPurityState", Lua_PlayerSetPurityState },
		{ "SetTearPoisonDamage", Lua_PlayerSetTearPoisonDamage },
		{ "GetVoidedCollectiblesList", Lua_PlayerGetVoidedCollectiblesList },
		{ "AddInnateCollectible", Lua_PlayerAddInnateCollectible },
		{ "GetWispCollectiblesList", Lua_PlayerGetWispCollecitblesList },
		{ "GetImmaculateConceptionState", Lua_PlayerGetImmaculateConceptionState },
		{ "SetImmaculateConceptionState", Lua_PlayerSetImmaculateConceptionState },
		{ "GetCambionConceptionState", Lua_PlayerGetCambionConceptionState },
		{ "SetCambionConceptionState", Lua_PlayerSetCambionConceptionState },
		{ "UpdateIsaacPregnancy", Lua_PlayerUpdateIsaacPregnancy },
		{ "GetCambionPregnancyLevel", Lua_PlayerGetCambionPregnancyLevel },
		{ "GetBladderCharge", Lua_PlayerGetBladderCharge },
		{ "SetBladderCharge", Lua_PlayerSetBladderCharge },
		{ "GetMaxBladderCharge", Lua_PlayerGetMaxBladderCharge },
		{ "SetMaxBladderCharge", Lua_PlayerSetMaxBladderCharge },
		{ "ClearDeadEyeCharge", Lua_ClearDeadEyeCharge },
		{ "IsUrethraBlocked", Lua_PlayerIsUrethraBlocked },
		{ "SetUrethraBlock", Lua_PlayerSetUrethraBlock },
		{ "GetNextUrethraBlockFrame", Lua_PlayerGetNextUrethraBlockFrame },
		{ "SetNextUrethraBlockFrame", Lua_PlayerSetNextUrethraBlockFrame },
		{ "GetHeldSprite", Lua_PlayerGetHeldSprite },
		{ "GetHeldEntity", Lua_PlayerGetHeldEntity },
		{ "GetActiveWeaponNumFired", Lua_PlayerGetActiveWeaponNumFired },
		{ "SetPoopSpell", Lua_PlayerSetPoopSpell },
		{ "RemovePoopSpell", Lua_PlayerRemovePoopSpell },
		{ "GetFlippedForm", Lua_PlayerGetBackupPlayer },
		{ "SwapForgottenForm", Lua_SwapForgottenForm },
		// { "SpawnAquariusCreep", Lua_SpawnAquariusCreep },
		{ "GetMaggySwingCooldown", Lua_PlayerGetMaggySwingCooldown },
		{ "SetMaggySwingCooldown", Lua_PlayerSetMaggySwingCooldown },
		{ "PlayDelayedSFX", Lua_PlayerPlayDelayedSFX },
		{ "CanUsePill", Lua_PlayerCanUsePill },
		{ "AddSmeltedTrinket", Lua_PlayerAddSmeltedTrinket },
		{ "GetSmeltedTrinkets", Lua_PlayerGetSmeltedTrinkets },
		{ "GetCostumeLayerMap", Lua_PlayerGetCostumeLayerMap },
		{ "IsItemCostumeVisible", Lua_PlayerIsItemCostumeVisible },
		{ "IsCollectibleCostumeVisible", Lua_PlayerIsCollectibleCostumeVisible },
		{ "IsNullItemCostumeVisible", Lua_PlayerIsNullItemCostumeVisible },
		{ "PlayCollectibleAnim", Player_PlayCollectibleAnim },
		{ "IsCollectibleAnimFinished", Player_IsCollectibleAnimFinished },
		{ "ClearCollectibleAnim", Player_ClearCollectibleAnim },
		{ "CheckFamiliarEx", Lua_EntityPlayer_CheckFamiliarEx },
		{ "GetEveSumptoriumCharge", Lua_PlayerGetEveSumptoriumCharge },
		{ "SetEveSumptoriumCharge", Lua_PlayerSetEveSumptoriumCharge },
		{ "GetPlayerFormCounter", Lua_PlayerGetPlayerFormCounter },
		{ "GetMaxPocketItems", Lua_PlayerGetMaxPocketItems },
		{ "AddBoneOrbital", Lua_PlayerAddBoneOrbital },
		//{ "AddItemCard", Lua_PlayerAddItemCard },
		{ "AddLeprosy", Lua_PlayerAddLeprosy },
		{ "AddUrnSouls", Lua_PlayerAddUrnSouls },
		{ "CanAddCollectibleToInventory", Lua_PlayerCanAddCollectibleToInventory },
		{ "CanCrushRocks", Lua_PlayerCanCrushRocks },
		{ "CanOverrideActiveItem", Lua_PlayerCanOverrideActiveItem },
		{ "ClearItemAnimCollectible", Lua_PlayerClearItemAnimCollectible },
		{ "ClearItemAnimNullItems", Lua_PlayerClearItemAnimNullItems },
		//{ "FireBoneClub", Lua_PlayerFireBoneClub },
		{ "FireBrimstoneBall", Lua_PlayerFireBrimstoneBall },
		{ "GetBodyMoveDirection", Lua_PlayerGetBodyMoveDirection },
		{ "GetDeathAnimName", Lua_PlayerGetDeathAnimName },
		{ "GetEnterPosition", Lua_PlayerGetEnterPosition },
		//{ "GetExplosionRadiusMultiplier", Lua_PlayerGetExplosionRadiusMultiplier },
		{ "GetFocusEntity", Lua_PlayerGetFocusEntity },
		{ "GetFootprintColor", Lua_PlayerGetFootprintColor },
		{ "SetFootprintColor", Lua_PlayerSetFootprintColor },
		{ "GetGlitchBabySubType", Lua_PlayerGetGlitchBabySubType },
		{ "GetGlyphOfBalanceDrop", Lua_PlayerGetGlyphOfBalanceDrop },
		{ "GetGreedsGulletHearts", Lua_PlayerGetGreedsGulletHearts },
		{ "GetLaserColor", Lua_PlayerGetLaserColor },
		{ "SetLaserColor", Lua_PlayerSetLaserColor },
		//{ "GetSalvationScale", Lua_PlayerGetSalvationScale },
		//{ "GetSoundPitch", Lua_PlayerGetSoundPitch },
		{ "GetSpecialGridCollision", Lua_PlayerGetSpecialGridCollision },
		{ "HasInstantDeathCurse", Lua_PlayerHasInstantDeathCurse },
		{ "HasPoisonImmunity", Lua_PlayerHasPoisonImmunity },
		{ "IsEntityValidTarget", Lua_PlayerIsEntityValidTarget },
		{ "IsFootstepFrame", Lua_PlayerIsFootstepFrame },
		{ "IsHeadless", Lua_PlayerIsHeadless },
		{ "IsHologram", Lua_PlayerIsHologram },
		{ "IsInvisible", Lua_PlayerIsInvisible },
		{ "IsPacifist", Lua_PlayerIsPacifist },
		{ "MorphToCoopGhost", Lua_PlayerMorphToCoopGhost },
		{ "RemovePocketItem", Lua_PlayerRemovePocketItem },
		{ "RerollAllCollectibles", Lua_PlayerRerollAllCollectibles },
		{ "ResetPlayer", Lua_PlayerResetPlayer },
		{ "ReviveCoopGhost", Lua_PlayerReviveCoopGhost },
		{ "SalvageCollectible", Lua_PlayerSalvageCollectible },
		{ "SetControllerIndex", Lua_PlayerSetControllerIndex },
		{ "ShootBlueCandle", Lua_PlayerShootBlueCandle },
		{ "SpawnClot", Lua_PlayerSpawnClot },
		{ "SpawnSaturnusTears", Lua_PlayerSpawnSaturnusTears },
		{ "SyncConsumableCounts", Lua_PlayerSyncConsumableCounts },
		{ "TryAddToBagOfCrafting", Lua_PlayerTryAddToBagOfCrafting },
		{ "TryDecreaseGlowingHourglassUses", Lua_PlayerTryDecreaseGlowingHourglassUses },
		{ "TryForgottenThrow", Lua_PlayerTryForgottenThrow },
		{ "TryRemoveSmeltedTrinket", Lua_PlayerTryRemoveSmeltedTrinket },
		//{ "UseRedKey", Lua_PlayerUseRedKey },
		{ "VoidHasCollectible", Lua_PlayerVoidHasCollectible },
		//{ "AttachMinecart", Lua_PlayerAttachMinecart },
		{ "GetKeepersSackBonus", Lua_PlayerGetKeepersSackBonus },
		{ "SetKeepersSackBonus", Lua_PlayerSetKeepersSackBonus },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_PLAYER, functions);

	// fix BabySkin Variable
	lua::RegisterVariable(_state, lua::Metatables::ENTITY_PLAYER, "BabySkin", Lua_PlayerGetBabySkin, Lua_PlayerSetBabySkin);
}