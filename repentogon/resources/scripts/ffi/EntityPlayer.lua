ffi.cdef[[
    // stubs
    typedef struct {void *_;} ActiveItemDesc;
    typedef struct {void *_;} TemporaryEffects;
    typedef struct {void *_;} EntityConfig_Player;
    typedef struct {void *_;} History;
    typedef struct {void *_;} PosVel;
    typedef struct {void *_;} Weapon;
    typedef struct {void *_;} Weapon_MultiShotParams;
    typedef struct {void *_;} TearParams;
    typedef struct {void *_;} EntityDesc;
    typedef struct {void *_;} QueueItemData;

    int L_EntityPlayer_AddActiveCharge(Entity_Player*, int, int, bool, bool, bool);
    void L_EntityPlayer_AddBoneOrbital(Entity_Player*, Vector*);
    void L_EntityPlayer_AddBlackHearts(Entity_Player*, int);
    void L_EntityPlayer_AddBloodCharge(Entity_Player*, int);
    Entity* L_EntityPlayer_AddBlueFlies(Entity_Player*, int, Vector*, Entity*);
    Entity* L_EntityPlayer_AddBlueSpider(Entity_Player*, Vector*);
    void L_EntityPlayer_AddBombs(Entity_Player*, int);
    void L_EntityPlayer_AddBoneHearts(Entity_Player*, int);
    void L_EntityPlayer_AddBrokenHearts(Entity_Player*, int);
    void L_EntityPlayer_AddCacheFlags(Entity_Player*, int, bool);
    void L_EntityPlayer_AddCard(Entity_Player*, int);
    void L_EntityPlayer_AddCoins(Entity_Player*, int);
    void L_EntityPlayer_AddCollectible(Entity_Player*, int, int, bool, int, int);
    void L_EntityPlayer_AddControlsCooldown(Entity_Player*, int);
    void L_EntityPlayer_AddCostume(Entity_Player*, ItemConfig_Item*, bool);
    void L_EntityPlayer_AddCurseMistEffect(Entity_Player*);
    void L_EntityPlayer_AddDeadEyeCharge(Entity_Player*);
    void L_EntityPlayer_AddDollarBillEffect(Entity_Player*);
    void L_EntityPlayer_AddEternalHearts(Entity_Player*, int);
    void L_EntityPlayer_AddFriendlyDip(Entity_Player*, int, Vector*);
    void L_EntityPlayer_AddGigaBombs(Entity_Player*, int);
    void L_EntityPlayer_AddGoldenBomb(Entity_Player*);
    void L_EntityPlayer_AddGoldenHearts(Entity_Player*, int);
    void L_EntityPlayer_AddGoldenKey(Entity_Player*);
    void L_EntityPlayer_AddHearts(Entity_Player*, int);
    void L_EntityPlayer_AddInnateCollectible(Entity_Player*, int, uint32_t);
    Entity_Familiar* L_EntityPlayer_AddItemWisp(Entity_Player*, int, Vector*, bool);
    void L_EntityPlayer_AddJarFlies(Entity_Player*, int);
    void L_EntityPlayer_AddJarHearts(Entity_Player*, int);
    void L_EntityPlayer_AddKeys(Entity_Player*, int);
    void L_EntityPlayer_AddLeprosy(Entity_Player*);
    void L_EntityPlayer_AddLocust(Entity_Player*, int, Vector*);
    void L_EntityPlayer_AddMaxHearts(Entity_Player*, int, bool);
    Entity_Familiar* L_EntityPlayer_AddMinisaac(Entity_Player*, Vector*, bool);
    void L_EntityPlayer_AddNullCostume(Entity_Player*, int);
    void L_EntityPlayer_AddPill(Entity_Player*, uint32_t);
    void L_EntityPlayer_AddPlayerFormCostume(Entity_Player*, int);
    void L_EntityPlayer_AddPoopMana(Entity_Player*, int);
    void L_EntityPlayer_AddPrettyFly(Entity_Player*);
    void L_EntityPlayer_AddRottenHearts(Entity_Player*, int);
    void L_EntityPlayer_AddSmeltedTrinket(Entity_Player*, int, bool);
    void L_EntityPlayer_AddSoulCharge(Entity_Player*, int);
    void L_EntityPlayer_AddSoulHearts(Entity_Player*, int);
    Entity_Familiar* L_EntityPlayer_AddSwarmFlyOrbital(Entity_Player*, Vector*);
    void L_EntityPlayer_AddTrinket(Entity_Player*, int, bool);
    void L_EntityPlayer_AddUrnSouls(Entity_Player*, int);
    Entity_Familiar* L_EntityPlayer_AddWisp(Entity_Player*, int, Vector*, bool, bool);
    void L_EntityPlayer_AnimateCard(Entity_Player*, int, const char*);
    void L_EntityPlayer_AnimateCollectible(Entity_Player*, int, const char*, const char*);
    void L_EntityPlayer_AnimateHappy(Entity_Player*);
    void L_EntityPlayer_AnimateLightTravel(Entity_Player*);
    void L_EntityPlayer_AnimatePickup(Entity_Player*, ANM2*, bool, const char*);
    void L_EntityPlayer_AnimatePill(Entity_Player*, int, const char*);
    void L_EntityPlayer_AnimatePitfallIn(Entity_Player*, bool);
    void L_EntityPlayer_AnimatePitfallOut(Entity_Player*);
    void L_EntityPlayer_AnimateSad(Entity_Player*);
    void L_EntityPlayer_AnimateTeleport(Entity_Player*, bool);
    void L_EntityPlayer_AnimateTrapdoor(Entity_Player*);
    void L_EntityPlayer_AnimateTrinket(Entity_Player*, int, const char*, const char*);
    bool L_EntityPlayer_AreControlsEnabled(Entity_Player*);
    bool L_EntityPlayer_AreOpposingShootDirectionsPressed(Entity_Player*);
    bool L_EntityPlayer_CanAddCollectible(Entity_Player*, int);
    bool L_EntityPlayer_CanAddCollectibleToInventory(Entity_Player*, int);
    bool L_EntityPlayer_CanCrushRocks(Entity_Player*);
    bool L_EntityPlayer_CanOverrideActiveItem(Entity_Player*, uint32_t);
    bool L_EntityPlayer_CanPickBlackHearts(Entity_Player*);
    bool L_EntityPlayer_CanPickBoneHearts(Entity_Player*);
    bool L_EntityPlayer_CanPickGoldenHearts(Entity_Player*);
    bool L_EntityPlayer_CanPickRedHearts(Entity_Player*);
    bool L_EntityPlayer_CanPickRottenHearts(Entity_Player*);
    bool L_EntityPlayer_CanPickSoulHearts(Entity_Player*);
    bool L_EntityPlayer_CanPickupItem(Entity_Player*);
    bool L_EntityPlayer_CanShoot(Entity_Player*);
    bool L_EntityPlayer_CanUsePill(Entity_Player*, int);
    bool L_EntityPlayer_CanTurnHead(Entity_Player*);
    void L_EntityPlayer_ChangePlayerType(Entity_Player*, int, bool);
    void L_EntityPlayer_CheckFamiliar(Entity_Player*, int, int, RNG*, ItemConfig_Item*, int);
    void L_EntityPlayer_ClearCollectibleAnim(Entity_Player*, int);
    void L_EntityPlayer_ClearItemAnimCollectible(Entity_Player*, int);
    void L_EntityPlayer_ClearItemAnimNullItems(Entity_Player*);
    void L_EntityPlayer_ClearCostumes(Entity_Player*);
    void L_EntityPlayer_ClearDeadEyeCharge(Entity_Player*, bool);
    void L_EntityPlayer_ClearTemporaryEffects(Entity_Player*);
    void L_EntityPlayer_DischargeActiveItem(Entity_Player*, int);
    void L_EntityPlayer_DonateLuck(Entity_Player*, int);
    void L_EntityPlayer_DoZitEffect(Entity_Player*, Vector*);
    Entity_Pickup* L_EntityPlayer_DropCollectible(Entity_Player*, int, Entity_Pickup*, bool);
    Entity_Pickup* L_EntityPlayer_DropCollectibleByHistoryIndex(Entity_Player*, int, Entity_Pickup*);
    void L_EntityPlayer_DropPocketItem(Entity_Player*, int, Vector*);
    Entity_Pickup* L_EntityPlayer_DropTrinket(Entity_Player*, Vector*, bool);
    void L_EntityPlayer_EnableWeaponType(Entity_Player*, int, bool);
    void L_EntityPlayer_EvaluateItems(Entity_Player*);
    Entity_Bomb* L_EntityPlayer_FireBomb(Entity_Player*, Vector*, Vector*, Entity*);
    Entity_Laser* L_EntityPlayer_FireBrimstone(Entity_Player*, Vector*, Entity*, float);
    Entity_Effect* L_EntityPlayer_FireBrimstoneBall(Entity_Player*, Vector*, Vector*, Vector*);
    Entity_Laser* L_EntityPlayer_FireDelayedBrimstone(Entity_Player*, float, Entity*);
    Entity_Knife* L_EntityPlayer_FireKnife(Entity_Player*, Entity*, float, bool, int, int);
    // i'll need some help with this one
    Entity_Tear* L_EntityPlayer_FireTear(Entity_Player*, Vector*, Vector*, bool, bool, bool, Entity*, float);
    Entity_Laser* L_EntityPlayer_FireTechLaser(Entity_Player*, Vector*, int, Vector*, bool, bool, Entity*, float);
    bool L_EntityPlayer_FlushQueueItem(Entity_Player*);
    bool L_EntityPlayer_FullCharge(Entity_Player*, int, bool);
    int L_EntityPlayer_GetActiveCharge(Entity_Player*, int);
    int L_EntityPlayer_GetActiveItem(Entity_Player*, int);
    ActiveItemDesc* L_EntityPlayer_GetActiveItemDesc(Entity_Player*, int);
    int L_EntityPlayer_GetActiveItemSlot(Entity_Player*, int);
    int L_EntityPlayer_GetActiveMaxCharge(Entity_Player*, int);
    int L_EntityPlayer_GetActiveMinUsableCharge(Entity_Player*, int);
    int L_EntityPlayer_GetActiveSubCharge(Entity_Player*, int);
    Entity* L_EntityPlayer_GetActiveWeaponEntity(Entity_Player*);
    int L_EntityPlayer_GetActiveWeaponNumFired(Entity_Player*);
    Vector* L_EntityPlayer_GetAimDirection(Entity_Player*);
    int L_EntityPlayer_GetBabySkin(Entity_Player*);
    int* L_EntityPlayer_GetBagOfCraftingOutput(Entity_Player*);
    int L_EntityPlayer_GetBagOfCraftingSlot(Entity_Player*, int);
    int L_EntityPlayer_GetBatteryCharge(Entity_Player*, int);
    int L_EntityPlayer_GetBlackHearts(Entity_Player*);
    int L_EntityPlayer_GetBodyColor(Entity_Player*);
    // !!!TODO!!!
    int L_EntityPlayer_GetBombFlags(Entity_Player*);
    int L_EntityPlayer_GetBombVariant(Entity_Player*, int, bool);
    int L_EntityPlayer_GetBoneHearts(Entity_Player*);
    int L_EntityPlayer_GetBrokenHearts(Entity_Player*);
    int L_EntityPlayer_GetCambionConceptionState(Entity_Player*);
    int L_EntityPlayer_GetCambionPregnancyLevel(Entity_Player*);
    int L_EntityPlayer_GetCard(Entity_Player*, int);
    RNG* L_EntityPlayer_GetCardRNG(Entity_Player*, int);
    int L_EntityPlayer_GetCollectibleCount(Entity_Player*);
    int L_EntityPlayer_GetCollectibleNum(Entity_Player*, int, bool);
    RNG* L_EntityPlayer_GetCollectibleRNG(Entity_Player*, int);
    // !!!TODO!!!
    int* L_EntityPlayer_GetCollectiblesList(Entity_Player*);
    // !!!TODO!!!
    void* L_EntityPlayer_GetCostumeLayerMap(Entity_Player*);
    // !!!TODO!!!
    void* L_EntityPlayer_GetCostumeSpriteDescs(Entity_Player*);
    Vector* L_EntityPlayer_GetCostumeNullPos(Entity_Player*, const char*, bool, Vector*);
    float L_EntityPlayer_GetD8DamageModifier(Entity_Player*);
    float L_EntityPlayer_GetD8FireDelayModifier(Entity_Player*);
    float L_EntityPlayer_GetD8RangeModifier(Entity_Player*);
    float L_EntityPlayer_GetD8SpeedModifier(Entity_Player*);
    int L_EntityPlayer_GetDamageCooldown(Entity_Player*);
    int L_EntityPlayer_GetDamageModifier(Entity_Player*);
    int L_EntityPlayer_GetDeadEyeCharge(Entity_Player*);
    const char* L_EntityPlayer_GetDeathAnimName(Entity_Player*);
    float L_EntityPlayer_GetEdenDamage(Entity_Player*);
    float L_EntityPlayer_GetEdenFireDelay(Entity_Player*);
    float L_EntityPlayer_GetEdenLuck(Entity_Player*);
    float L_EntityPlayer_GetEdenRange(Entity_Player*);
    float L_EntityPlayer_GetEdenShotSpeed(Entity_Player*);
    float L_EntityPlayer_GetEdenSpeed(Entity_Player*);
    int L_EntityPlayer_GetEffectiveBloodCharge(Entity_Player*);
    int L_EntityPlayer_GetEffectiveMaxHearts(Entity_Player*);
    int L_EntityPlayer_GetEffectiveSoulCharge(Entity_Player*);
    TemporaryEffects* L_EntityPlayer_GetEffects(Entity_Player*);
    Vector* L_EntityPlayer_GetEnterPosition(Entity_Player*);
    EntityConfig_Player* L_EntityPlayer_GetEntityConfigPlayer(Entity_Player*);
    int L_EntityPlayer_GetEpiphoraCharge(Entity_Player*);
    int L_EntityPlayer_GetEternalHearts(Entity_Player*);
    int L_EntityPlayer_GetEveSumptoriumCharge(Entity_Player*);
    int L_EntityPlayer_GetExtraLives(Entity_Player*);
    int L_EntityPlayer_GetFireDelayModifier(Entity_Player*);
    int L_EntityPlayer_GetFireDirection(Entity_Player*);
    Entity_Player* L_EntityPlayer_GetFlippedForm(Entity_Player*);
    Entity* L_EntityPlayer_GetFocusEntity(Entity_Player*);
    Color* L_EntityPlayer_GetFootprintColor(Entity_Player*, bool);
    Vector* L_EntityPlayer_GetFlyingOffset(Entity_Player*);
    int L_EntityPlayer_GetGlitchBabySubType(Entity_Player*);
    // TODO
    void L_EntityPlayer_GetGlyphOfBalanceDrop(Entity_Player*, int*, int*);
    // ?????? https://repentogon.com/EntityPlayer.html#getgnawedleaftimer
    int L_EntityPlayer_GetGnawedLeafTimer(Entity_Player*);
    int L_EntityPlayer_GetGoldenHearts(Entity_Player*);
    float L_EntityPlayer_GetGreedDonationBreakChance(Entity_Player*);
    int L_EntityPlayer_GetGreedsGulletHearts(Entity_Player*);
    int L_EntityPlayer_GetHeadColor(Entity_Player*);
    int L_EntityPlayer_GetHeadDirection(Entity_Player*);
    int L_EntityPlayer_GetHealthType(Entity_Player*);
    int L_EntityPlayer_GetHeartLimit(Entity_Player*, bool);
    int L_EntityPlayer_GetHearts(Entity_Player*);
    Entity* L_EntityPlayer_GetHeldEntity(Entity_Player*);
    ANM2* L_EntityPlayer_GetHeldSprite(Entity_Player*);
    History* L_EntityPlayer_GetHistory(Entity_Player*);
    int L_EntityPlayer_GetImmaculateConceptionState(Entity_Player*);
    int L_EntityPlayer_GetItemState(Entity_Player*);
    int L_EntityPlayer_GetJarFlies(Entity_Player*);
    int L_EntityPlayer_GetJarHearts(Entity_Player*);
    int L_EntityPlayer_GetKeepersSackBonus(Entity_Player*);
    ColorMod* L_EntityPlayer_GetLaserColor(Entity_Player*);
    Vector* L_EntityPlayer_GetLaserOffset(Entity_Player*, int, Vector*);
    int L_EntityPlayer_GetLuckModifier(Entity_Player*);
    int L_EntityPlayer_GetLastActionTriggers(Entity_Player*);
    int L_EntityPlayer_GetLastDamageFlags(Entity_Player*);
    const EntityRef* L_EntityPlayer_GetLastDamageSource(Entity_Player*);
    const Vector* L_EntityPlayer_GetLastDirection(Entity_Player*);
    int L_EntityPlayer_GetMaggySwingCooldown(Entity_Player*);
    Entity_Player* L_EntityPlayer_GetMainTwin(Entity_Player*);
    Entity_Effect* L_EntityPlayer_GetMarkedTarget(Entity_Player*);
    int L_EntityPlayer_GetMaxBladderCharge(Entity_Player*);
    int L_EntityPlayer_GetMaxPeeBurstCooldown(Entity_Player*);
    int L_EntityPlayer_GetMaxHearts(Entity_Player*);
    int L_EntityPlayer_GetMaxPocketItems(Entity_Player*);
    int L_EntityPlayer_GetMaxPoopMana(Entity_Player*);
    int L_EntityPlayer_GetMaxTrinkets(Entity_Player*);
    int L_EntityPlayer_GetMegaBlastDuration(Entity_Player*);
    int L_EntityPlayer_GetMetronomeCollectibleID(Entity_Player*);
    int L_EntityPlayer_GetModelingClayEffect(Entity_Player*);
    int L_EntityPlayer_GetMovementDirection(Entity_Player*);
    const Vector* L_EntityPlayer_GetMovementInput(Entity_Player*, float);
    Vector* L_EntityPlayer_GetMovementJoystick(Entity_Player*);
    Vector* L_EntityPlayer_GetMovementVector(Entity_Player*);
    Weapon_MultiShotParams* L_EntityPlayer_GetMultiShotParams(Entity_Player*, int);
    PosVel* L_EntityPlayer_GetMultiShotPositionVelocity(Entity_Player*, int, int, Vector*, float, Weapon_MultiShotParams*);
    const char* L_EntityPlayer_GetName(Entity_Player*);
    int L_EntityPlayer_GetNumBlueFlies(Entity_Player*);
    int L_EntityPlayer_GetNumBlueSpiders(Entity_Player*);
    int L_EntityPlayer_GetNumBombs(Entity_Player*);
    int L_EntityPlayer_GetNumCoins(Entity_Player*);
    int L_EntityPlayer_GetNumGigaBombs(Entity_Player*);
    int L_EntityPlayer_GetNumKeys(Entity_Player*);
    Entity_Player* L_EntityPlayer_GetOtherTwin(Entity_Player*);
    int L_EntityPlayer_GetPeeBurstCooldown(Entity_Player*);
    int L_EntityPlayer_GetPill(Entity_Player*, int);
    RNG* L_EntityPlayer_GetPillRNG(Entity_Player*, int);
    int L_EntityPlayer_GetPlayerFormCounter(Entity_Player*, int);
    int L_EntityPlayer_GetPlayerType(Entity_Player*);
    int L_EntityPlayer_GetPonyCharge(Entity_Player*);
    int L_EntityPlayer_GetPurityState(Entity_Player*);
    int L_EntityPlayer_GetPoopMana(Entity_Player*);
    int L_EntityPlayer_GetPoopSpell(Entity_Player*, int);
    Vector* L_EntityPlayer_GetRecentMovementVector(Entity_Player*);
    int L_EntityPlayer_GetRottenHearts(Entity_Player*);
    Vector* L_EntityPlayer_GetShootingInput(Entity_Player*);
    int L_EntityPlayer_GetShotSpeedModifier(Entity_Player*);
    // !!!TODO!!!
    void* L_EntityPlayer_GetSmeltedTrinkets(Entity_Player*);
    int L_EntityPlayer_GetSpecialGridCollision(Entity_Player*, Vector*);
    int L_EntityPlayer_GetSpeedModifier(Entity_Player*);
    int L_EntityPlayer_GetTotalActiveCharge(Entity_Player*, int);
    void* L_EntityPlayer_GetVoidedCollectiblesList(Entity_Player*);
    Weapon* L_EntityPlayer_GetWeapon(Entity_Player*, int);
    int L_EntityPlayer_GetWildCardItem(Entity_Player*);
    uint8_t L_EntityPlayer_GetWildCardItemType(Entity_Player*);
    // !!!TODO!!!
    void* L_EntityPlayer_GetWispCollectiblesList(Entity_Player*);
    float L_EntityPlayer_GetSmoothBodyRotation(Entity_Player*);
    int L_EntityPlayer_GetSoulCharge(Entity_Player*);
    int L_EntityPlayer_GetSoulHearts(Entity_Player*);
    Entity_Player* L_EntityPlayer_GetSubPlayer(Entity_Player*);
    TearParams* L_EntityPlayer_GetTearHitParams(Entity_Player*, int, float, int, Entity*);
    Vector* L_EntityPlayer_GetTearMovementInheritance(Entity_Player*, Vector*);
    float L_EntityPlayer_GetTearPoisonDamage(Entity_Player*);
    int L_EntityPlayer_GetTearRangeModifier(Entity_Player*);
    int L_EntityPlayer_GetTotalDamageTaken(Entity_Player*);
    Entity* L_EntityPlayer_GetTractorBeam(Entity_Player*);
    // TODO: return ItemConfig::Item here
    ItemConfig_Item* L_EntityPlayer_GetTrinket(Entity_Player*, int);
    int L_EntityPlayer_GetTrinketMultiplier(Entity_Player*, int);
    RNG* L_EntityPlayer_GetTrinketRNG(Entity_Player*, int);
    Vector* L_EntityPlayer_GetVelocityBeforeUpdate(Entity_Player*);
    int L_EntityPlayer_GetWeaponModifiers(Entity_Player*);
    int L_EntityPlayer_GetZodiacEffect(Entity_Player*);
    bool L_EntityPlayer_HasCollectible(Entity_Player*, int, bool);
    bool L_EntityPlayer_HasCurseMistEffect(Entity_Player*);
    bool L_EntityPlayer_HasFullHearts(Entity_Player*);
    bool L_EntityPlayer_HasFullHeartsAndSoulHearts(Entity_Player*);
    bool L_EntityPlayer_HasGoldenBomb(Entity_Player*);
    bool L_EntityPlayer_HasGoldenKey(Entity_Player*);
    bool L_EntityPlayer_HasInstantDeathCurse(Entity_Player*);
    bool L_EntityPlayer_HasInvincibility(Entity_Player*, int);
    bool L_EntityPlayer_HasPoisonImmunity(Entity_Player*);
    bool L_EntityPlayer_HasPlayerForm(Entity_Player*, int);
    bool L_EntityPlayer_HasTimedItem(Entity_Player*);
    bool L_EntityPlayer_HasTrinket(Entity_Player*, int, bool);
    bool L_EntityPlayer_HasWeaponType(Entity_Player*, int);
    void L_EntityPlayer_IncrementPlayerFormCounter(Entity_Player*, int, int);
    void L_EntityPlayer_InitBabySkin(Entity_Player*);
    void L_EntityPlayer_InitPostLevelInitStats(Entity_Player*);
    void L_EntityPlayer_InitTwin(Entity_Player*, int);
    bool L_EntityPlayer_IsBlackHeart(Entity_Player*, int);
    bool L_EntityPlayer_IsBoneHeart(Entity_Player*, int);
    bool L_EntityPlayer_IsCollectibleAnimFinished(Entity_Player*, int, const char*);
    bool L_EntityPlayer_IsCollectibleCostumeVisibleLayerID(Entity_Player*, int, int);
    bool L_EntityPlayer_IsCollectibleCostumeVisibleLayerName(Entity_Player*, int, const char*);
    bool L_EntityPlayer_IsCoopGhost(Entity_Player*);
    bool L_EntityPlayer_IsEntityValidTarget(Entity_Player*, Entity*);
    bool L_EntityPlayer_IsExtraAnimationFinished(Entity_Player*);
    bool L_EntityPlayer_IsFootstepFrame(Entity_Player*, int);
    bool L_EntityPlayer_IsFullSpriteRendering(Entity_Player*);
    bool L_EntityPlayer_IsHeadless(Entity_Player*);
    bool L_EntityPlayer_IsHeldItemVisible(Entity_Player*);
    bool L_EntityPlayer_IsHoldingItem(Entity_Player*);
    bool L_EntityPlayer_IsHologram(Entity_Player*);
    bool L_EntityPlayer_IsInvisible(Entity_Player*);
    bool L_EntityPlayer_IsItemCostumeVisible(Entity_Player*, ItemConfig_Item*, int);
    bool L_EntityPlayer_IsItemCostumeVisibleLayerName(Entity_Player*, ItemConfig_Item*, const char*);
    bool L_EntityPlayer_IsItemQueueEmpty(Entity_Player*);
    bool L_EntityPlayer_IsLocalPlayer(Entity_Player*);
    bool L_EntityPlayer_IsNullItemCostumeVisible(Entity_Player*, int, int);
    bool L_EntityPlayer_IsNullItemCostumeVisibleLayerName(Entity_Player*, int, const char*);
    bool L_EntityPlayer_IsP2Appearing(Entity_Player*);
    bool L_EntityPlayer_IsPacifist(Entity_Player*);
    bool L_EntityPlayer_IsPosInSpotLight(Entity_Player*, Vector*);
    bool L_EntityPlayer_IsSubPlayer(Entity_Player*);
    bool L_EntityPlayer_IsUrethraBlocked(Entity_Player*);
    void L_EntityPlayer_MorphToCoopGhost(Entity_Player*);
    bool L_EntityPlayer_NeedsCharge(Entity_Player*, int);
    void L_EntityPlayer_PlayCollectibleAnim(Entity_Player*, int, bool, const char*, int);
    void L_EntityPlayer_PlayExtraAnimation(Entity_Player*, const char*);
    void L_EntityPlayer_QueueExtraAnimation(Entity_Player*, const char*);
    // need some help on this one too
    void L_EntityPlayer_QueueItem(Entity_Player*, ItemConfig_Item*, int, bool, bool, int);
    void L_EntityPlayer_RemoveBlackHeart(Entity_Player*, int);
    void L_EntityPlayer_RemoveBlueFly(Entity_Player*);
    void L_EntityPlayer_RemoveBlueSpider(Entity_Player*);
    void L_EntityPlayer_RemoveCollectible(Entity_Player*, int, bool, int, bool);
    void L_EntityPlayer_RemoveCollectibleByHistoryIndex(Entity_Player*, int);
    void L_EntityPlayer_RemoveCostume(Entity_Player*, ItemConfig_Item*);
    void L_EntityPlayer_RemoveCurseMistEffect(Entity_Player*);
    void L_EntityPlayer_RemoveGoldenBomb(Entity_Player*);
    void L_EntityPlayer_RemoveGoldenKey(Entity_Player*);
    void L_EntityPlayer_RemovePocketItem(Entity_Player*, int);
    void L_EntityPlayer_RemovePoopSpell(Entity_Player*, int);
    void L_EntityPlayer_RemoveSkinCostume(Entity_Player*);
    void L_EntityPlayer_RenderBody(Entity_Player*, Vector*);
    void L_EntityPlayer_RenderDebugInfo(Entity_Player*, Vector*);
    void L_EntityPlayer_RenderGlow(Entity_Player*, Vector*);
    void L_EntityPlayer_RenderHead(Entity_Player*, Vector*);
    void L_EntityPlayer_RenderHeadBack(Entity_Player*, Vector*);
    bool L_EntityPlayer_RenderShadowLayer(Entity_Player*, Vector*);
    void L_EntityPlayer_RenderTop(Entity_Player*, Vector*);
    void L_EntityPlayer_ReplaceCostumeSprite(Entity_Player*, ItemConfig_Item*, const char*, int);
    void L_EntityPlayer_RerollAllCollectibles(Entity_Player*, RNG*, bool);
    void L_EntityPlayer_ResetDamageCooldown(Entity_Player*);
    void L_EntityPlayer_ResetItemState(Entity_Player*);
    void L_EntityPlayer_ResetPlayer(Entity_Player*);
    void L_EntityPlayer_RespawnFamiliars(Entity_Player*);
    void L_EntityPlayer_Revive(Entity_Player*);
    bool L_EntityPlayer_ReviveCoopGhost(Entity_Player*);
    void L_EntityPlayer_SalvageCollectibleID(Entity_Player*, int, Vector*, RNG*, int);
    void L_EntityPlayer_SetActiveCharge(Entity_Player*, int, int);
    void L_EntityPlayer_SetActiveVarData(Entity_Player*, int, int);
    void L_EntityPlayer_SetBagOfCraftingContent(Entity_Player*, int*);
    void L_EntityPlayer_SetBagOfCraftingOutput(Entity_Player*, int);
    void L_EntityPlayer_SetBagOfCraftingSlot(Entity_Player*, int, int);
    void L_EntityPlayer_SetBladderCharge(Entity_Player*, int);
    void L_EntityPlayer_SetBloodCharge(Entity_Player*, int);
    void L_EntityPlayer_SetCanShoot(Entity_Player*, bool);
    void L_EntityPlayer_SetCard(Entity_Player*, int, int);
    void L_EntityPlayer_SetControllerIndex(Entity_Player*, int);
    void L_EntityPlayer_SetDamageModifier(Entity_Player*, int);
    void L_EntityPlayer_SetEdenDamage(Entity_Player*, float);
    void L_EntityPlayer_SetEdenFireDelay(Entity_Player*, float);
    void L_EntityPlayer_SetEdenLuck(Entity_Player*, float);
    void L_EntityPlayer_SetEdenRange(Entity_Player*, float);
    void L_EntityPlayer_SetEdenShotSpeed(Entity_Player*, float);
    void L_EntityPlayer_SetEdenSpeed(Entity_Player*, float);
    void L_EntityPlayer_SetEveSumptoriumCharge(Entity_Player*, int);
    void L_EntityPlayer_SetFireDelayModifier(Entity_Player*, int);
    void L_EntityPlayer_SetFootprintColor(Entity_Player*, Color*, bool);
    void L_EntityPlayer_SetFullHearts(Entity_Player*);
    void L_EntityPlayer_SetGnawedLeafTimer(Entity_Player*, int);
    void L_EntityPlayer_SetImmaculateConceptionState(Entity_Player*, int);
    void L_EntityPlayer_SetItemState(Entity_Player*, int);
    void L_EntityPlayer_SetKeepersSackBonus(Entity_Player*, int);
    void L_EntityPlayer_SetLaserColor(Entity_Player*, ColorMod*);
    void L_EntityPlayer_SetLuckModifier(Entity_Player*, int);
    void L_EntityPlayer_SetMaggySwingCooldown(Entity_Player*, int);
    void L_EntityPlayer_SetMaxBladderCharge(Entity_Player*, int);
    void L_EntityPlayer_SetMegaBlastDuration(Entity_Player*, int);
    void L_EntityPlayer_SetMinDamageCooldown(Entity_Player*, int);
    void L_EntityPlayer_SetNextUrethraBlockFrame(Entity_Player*, int);
    void L_EntityPlayer_SetPill(Entity_Player*, int, int);
    void L_EntityPlayer_SetPocketActiveItem(Entity_Player*, int, int, bool);
    void L_EntityPlayer_SetPonyCharge(Entity_Player*, int);
    void L_EntityPlayer_SetPoopSpell(Entity_Player*, int, int);
    void L_EntityPlayer_SetPurityState(Entity_Player*, int);
    void L_EntityPlayer_SetRedStewBonusDuration(Entity_Player*, int);
    void L_EntityPlayer_SetShootingCooldown(Entity_Player*, int);
    void L_EntityPlayer_SetShotSpeedModifier(Entity_Player*, int);
    void L_EntityPlayer_SetSoulCharge(Entity_Player*, int);
    void L_EntityPlayer_SetSpeedModifier(Entity_Player*, int);
    void L_EntityPlayer_SetTearPoisonDamage(Entity_Player*, float);
    void L_EntityPlayer_SetTearRangeModifier(Entity_Player*, int);
    void L_EntityPlayer_SetUrethraBlock(Entity_Player*, bool);
    void L_EntityPlayer_SetWeapon(Entity_Player*, Weapon*, int);
    void L_EntityPlayer_ShootBlueCandle(Entity_Player*, Vector*);
    void L_EntityPlayer_ShuffleCostumes(Entity_Player*, int);
    Entity_Effect* L_EntityPlayer_SpawnAquariusCreep(Entity_Player*, TearParams*);
    void L_EntityPlayer_SpawnClot(Entity_Player*, Vector*, bool);
    Entity_Laser* L_EntityPlayer_SpawnMawOfVoid(Entity_Player*, int);
    int L_EntityPlayer_SpawnSaturnusTears(Entity_Player*);
    void L_EntityPlayer_StopExtraAnimation(Entity_Player*);
    void L_EntityPlayer_SwapActiveItems(Entity_Player*);
    void L_EntityPlayer_SwapForgottenForm(Entity_Player*, bool, bool);
    void L_EntityPlayer_SyncConsumableCounts(Entity_Player*, Entity_Player*, int);
    void L_EntityPlayer_Teleport(Entity_Player*, Vector*, bool, bool);
    Entity_Familiar* L_EntityPlayer_ThrowFriendlyDip(Entity_Player*, int, Vector*, Vector*);
    Entity* L_EntityPlayer_ThrowHeldEntity(Entity_Player*, Vector*);
    void L_EntityPlayer_TriggerBookOfVirtues(Entity_Player*, int, int);
    void L_EntityPlayer_TriggerRoomClear(Entity_Player*);
    void L_EntityPlayer_TryAddToBagOfCrafting(Entity_Player*, Entity_Pickup*);
    void L_EntityPlayer_TryDecreaseGlowingHourglassUses(Entity_Player*, int, bool);
    bool L_EntityPlayer_TryFakeDeath(Entity_Player*);
    bool L_EntityPlayer_TryForgottenThrow(Entity_Player*, Vector*);
    bool L_EntityPlayer_TryHoldEntity(Entity_Player*, Entity*);
    bool L_EntityPlayer_TryHoldTrinket(Entity_Player*, int);
    bool L_EntityPlayer_TryPreventDeath(Entity_Player*);
    void L_EntityPlayer_TryRemoveCollectibleCostume(Entity_Player*, int, bool);
    void L_EntityPlayer_TryRemoveNullCostume(Entity_Player*, int);
    bool L_EntityPlayer_TryRemoveTrinket(Entity_Player*, int);
    void L_EntityPlayer_TryRemoveTrinketCostume(Entity_Player*, int);
    void L_EntityPlayer_TryRemoveSmeltedTrinket(Entity_Player*, int);
    bool L_EntityPlayer_TryUseKey(Entity_Player*);
    void L_EntityPlayer_UpdateCanShoot(Entity_Player*);
    void L_EntityPlayer_UpdateIsaacPregnancy(Entity_Player*, bool);
    void L_EntityPlayer_UseActiveItem(Entity_Player*, int, int, int, int);
    void L_EntityPlayer_UseActiveItemAbp(Entity_Player*, int, bool, bool, bool, bool, int, int);
    void L_EntityPlayer_UseCard(Entity_Player*, int, int);
    void L_EntityPlayer_UsePill(Entity_Player*, int, int, int);
    void L_EntityPlayer_UsePoopSpell(Entity_Player*, int);
    bool L_EntityPlayer_VoidHasCollectible(Entity_Player*, int);
    bool L_EntityPlayer_WillPlayerRevive(Entity_Player*);

    int L_EntityPlayer_GetBabySkin(Entity_Player*);
    void L_EntityPlayer_SetBabySkin(Entity_Player*, int);
    bool L_EntityPlayer_GetCanFly(Entity_Player*);
    void L_EntityPlayer_SetCanFly(Entity_Player*, bool);
    int L_EntityPlayer_GetControllerIndex(Entity_Player*);
    int L_EntityPlayer_GetControlsCooldown(Entity_Player*);
    void L_EntityPlayer_SetControlsCooldown(Entity_Player*, int);
    bool L_EntityPlayer_GetControlsEnabled(Entity_Player*);
    void L_EntityPlayer_SetControlsEnabled(Entity_Player*, bool);
    float L_EntityPlayer_GetDamage(Entity_Player*);
    void L_EntityPlayer_SetDamage(Entity_Player*, float);
    float L_EntityPlayer_GetFireDelay(Entity_Player*);
    void L_EntityPlayer_SetFireDelay(Entity_Player*, float);
    EntityDesc* L_EntityPlayer_GetFriendBallEnemy(Entity_Player*);
    void L_EntityPlayer_SetFriendBallEnemy(Entity_Player*, EntityDesc*);
    int L_EntityPlayer_GetHeadFrameDelay(Entity_Player*);
    void L_EntityPlayer_SetHeadFrameDelay(Entity_Player*, int);
    float L_EntityPlayer_GetIBSCharge(Entity_Player*);
    void L_EntityPlayer_SetIBSCharge(Entity_Player*, float);
    int L_EntityPlayer_GetItemHoldCooldown(Entity_Player*);
    void L_EntityPlayer_SetItemHoldCooldown(Entity_Player*, int);
    float L_EntityPlayer_GetLuck(Entity_Player*);
    void L_EntityPlayer_SetLuck(Entity_Player*, float);
    float L_EntityPlayer_GetMaxFireDelay(Entity_Player*);
    void L_EntityPlayer_SetMaxFireDelay(Entity_Player*, float);
    float L_EntityPlayer_GetMoveSpeed(Entity_Player*);
    void L_EntityPlayer_SetMoveSpeed(Entity_Player*, float);
    QueueItemData* L_EntityPlayer_GetQueuedItem(Entity_Player*);
    void L_EntityPlayer_SetQueuedItem(Entity_Player*, QueueItemData*);
    float L_EntityPlayer_GetSamsonBerserkCharge(Entity_Player*);
    void L_EntityPlayer_SetSamsonBerserkCharge(Entity_Player*, float);
    float L_EntityPlayer_GetShotSpeed(Entity_Player*);
    void L_EntityPlayer_SetShotSpeed(Entity_Player*, float);
    ColorMod* L_EntityPlayer_GetTearColor(Entity_Player*);
    void L_EntityPlayer_SetTearColor(Entity_Player*, ColorMod*);
    float L_EntityPlayer_GetTearFallingAcceleration(Entity_Player*);
    void L_EntityPlayer_SetTearFallingAcceleration(Entity_Player*, float);
    float L_EntityPlayer_GetTearFallingSpeed(Entity_Player*);
    void L_EntityPlayer_SetTearFallingSpeed(Entity_Player*, float);
    BitSet128* L_EntityPlayer_GetTearFlags(Entity_Player*);
    void L_EntityPlayer_SetTearFlags(Entity_Player*, BitSet128*);
    float L_EntityPlayer_GetTearHeight(Entity_Player*);
    void L_EntityPlayer_SetTearHeight(Entity_Player*, float);
    float L_EntityPlayer_GetTearRange(Entity_Player*);
    void L_EntityPlayer_SetTearRange(Entity_Player*, float);
    Vector* L_EntityPlayer_GetTearsOffset(Entity_Player*);
    void L_EntityPlayer_SetTearsOffset(Entity_Player*, Vector*);
]]

local repentogon = ffidll
local lffi = ffi

local EntityPlayerFuncs = {}

function EntityPlayerFuncs:AddActiveCharge(charge, slot, flashHUD, overcharge, force)
    ffichecks.checknumber(2, charge)
    ffichecks.checknumber(3, slot)
    flashHUD = ffichecks.optboolean(flashHUD, true)
    overcharge = ffichecks.optboolean(overcharge, false)
    force = ffichecks.optboolean(force, false)

    return repentogon.L_EntityPlayer_AddActiveCharge(self, charge, slot, flashHUD, overcharge, force)
end

function EntityPlayerFuncs:AddBoneOrbital(pos)
    ffichecks.checkcdata(2, pos, "Vector")
    repentogon.L_EntityPlayer_AddBoneOrbital(self, pos)
end

function EntityPlayerFuncs:AddBlackHearts(blackHearts)
    ffichecks.checknumber(2, blackHearts)
    repentogon.L_EntityPlayer_AddBlackHearts(self, blackHearts)
end

function EntityPlayerFuncs:AddBloodCharge(amount)
    ffichecks.checknumber(2, amount)
    repentogon.L_EntityPlayer_AddBloodCharge(self, amount)
end

function EntityPlayerFuncs:AddBlueFlies(amount, pos, target)
    ffichecks.checknumber(2, amount)
    ffichecks.checkcdata(3, pos, "Vector")
    ffichecks.checkcdata(4, target, "Entity")
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_AddBlueFlies(self, amount))
end

function EntityPlayerFuncs:AddBlueSpider(pos)
    ffichecks.checkcdata(2, pos, "Vector")
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_AddBlueSpider(pos))
end

function EntityPlayerFuncs:AddBombs(amount)
    ffichecks.checknumber(2, amount)
    repentogon.L_EntityPlayer_AddBombs(self, amount)
end

function EntityPlayerFuncs:AddBoneHearts(hearts)
    ffichecks.checknumber(2, hearts)
    repentogon.L_EntityPlayer_AddBoneHearts(self, hearts)
end

function EntityPlayerFuncs:AddBrokenHearts(brokenHearts)
    ffichecks.checknumber(2, brokenHearts)
    repentogon.L_EntityPlayer_AddBrokenHearts(self, brokenHearts)
end

function EntityPlayerFuncs:AddCacheFlags(cacheFlag, evaluateItems)
    ffichecks.checknumber(2, cacheFlag)
    evaluateItems = ffichecks.optboolean(evaluateItems, false)
    repentogon.L_EntityPlayer_AddCacheFlags(self, cacheFlag, evaluateItems)
end

function EntityPlayerFuncs:AddCard(id)
    ffichecks.checknumber(2, id)
    repentogon.L_EntityPlayer_AddCard(self, id)
end

function EntityPlayerFuncs:AddCoins(amount)
    ffichecks.checknumber(2, amount)
    repentogon.L_EntityPlayer_AddCoins(self, amount)
end

function EntityPlayerFuncs:AddCollectible(itemType, charge, firstTimePickingUp, slot, varData)
    ffichecks.checknumber(2, itemType)
    charge = ffichecks.optnumber(charge, 0)
    firstTimePickingUp = ffichecks.optboolean(firstTimePickingUp, true)
    slot = ffichecks.optnumber(slot, ActiveSlot.SLOT_PRIMARY)
    varData = ffichecks.optnumber(varData, 0)

    repentogon.L_EntityPlayer_AddCollectible(self, itemType, charge, firstTimePickingUp, slot, varData)
end

function EntityPlayerFuncs:AddControlsCooldown(cooldown)
    ffichecks.checknumber(2, cooldown)
    repentogon.L_EntityPlayer_AddControlsCooldown(self, cooldown)
end

function EntityPlayerFuncs:AddCostume(item, itemStateOnly)
    ffichecks.checkcdata(2, item, "ItemConfig_Item")
    itemStateOnly = ffichecks.optboolean(itemStateOnly, false)

    repentogon.L_EntityPlayer_AddCostume(self, item, itemStateOnly)
end

EntityPlayerFuncs.AddCurseMistEffect = repentogon.L_EntityPlayer_AddCurseMistEffect
EntityPlayerFuncs.AddDeadEyeCharge = repentogon.L_EntityPlayer_AddDeadEyeCharge
EntityPlayerFuncs.AddDollarBillEffect = repentogon.L_EntityPlayer_AddDollarBillEffect

function EntityPlayerFuncs:AddEternalHearts(eternalHearts)
    ffichecks.checknumber(2, eternalHearts)
    repentogon.L_EntityPlayer_AddEternalHearts(self, eternalHearts)
end

function EntityPlayerFuncs:AddFriendlyDip(subtype, pos)
    ffichecks.checknumber(2, subtype)
    ffichecks.checkcdata(3, pos, "Vector")
    repentogon.L_EntityPlayer_AddFriendlyDip(self, subtype, pos)
end

function EntityPlayerFuncs:AddGigaBombs(gigaBombs)
    ffichecks.checknumber(2, gigaBombs)
    repentogon.L_EntityPlayer_AddGigaBombs(self, gigaBombs)
end

EntityPlayerFuncs.AddGoldenBomb = repentogon.L_EntityPlayer_AddGoldenBomb

function EntityPlayerFuncs:AddGoldenHearts(hearts)
    ffichecks.checknumber(2, hearts)
    repentogon.L_EntityPlayer_AddGoldenHearts(self, hearts)
end

EntityPlayerFuncs.AddGoldenKey = repentogon.L_EntityPlayer_AddGoldenKey

function EntityPlayerFuncs:AddHearts(hearts)
    ffichecks.checknumber(2, hearts)
    repentogon.L_EntityPlayer_AddHearts(self, hearts)
end

function EntityPlayerFuncs:AddInnateCollectible(itemId, amount)
    ffichecks.checknumber(2, itemId)
    amount = ffichecks.optnumber(amount, 1)
    
    repentogon.L_EntityPlayer_AddInnateCollectible(self, itemId, amount)
end

function EntityPlayerFuncs:AddItemWisp(collectible, pos, adjustOrbitLayer)
    ffichecks.checknumber(2, collectible)
    ffichecks.checkcdata(3, pos, "Vector")
    adjustOrbitLayer = ffichecks.optboolean(adjustOrbitLayer, false)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_AddItemWisp(self, collectible, pos, adjustOrbitLayer))
end

function EntityPlayerFuncs:AddJarFlies(flies)
    ffichecks.checknumber(2, flies)
    repentogon.L_EntityPlayer_AddJarFlies(self, flies)
end

function EntityPlayerFuncs:AddJarHearts(hearts)
    ffichecks.checknumber(2, hearts)
    repentogon.L_EntityPlayer_AddJarHearts(self, hearts)
end

function EntityPlayerFuncs:AddKeys(amount)
    ffichecks.checknumber(2, amount)
    repentogon.L_EntityPlayer_AddKeys(self, amount)
end

EntityPlayerFuncs.AddLeprosy = repentogon.L_EntityPlayer_AddLeprosy

function EntityPlayerFuncs:AddLocust(itemType, pos)
    ffichecks.checknumber(2, itemType)
    ffichecks.checkcdata(3, pos, "Vector")

    repentogon.L_EntityPlayer_AddLocust(self, itemType, pos)
end

function EntityPlayerFuncs:AddMaxHearts(maxHearts, ignoreKeeper)
    ffichecks.checknumber(2, maxHearts)
    ignoreKeeper = ffichecks.optboolean(ignoreKeeper, false)

    repentogon.L_EntityPlayer_AddMaxHearts(self, maxHearts, ignoreKeeper)
end

function EntityPlayerFuncs:AddMinisaac(pos, playAnim)
    ffichecks.checkcdata(3, pos, "Vector")
    playAnim = ffichecks.optboolean(playAnim, true)

    repentogon.L_EntityPlayer_AddMinisaac(self, pos, playAnim)
end

function EntityPlayerFuncs:AddNullCostume(nullId)
    ffichecks.checknumber(2, nullId)
    repentogon.L_EntityPlayer_AddNullCostume(self, nullId)
end

function EntityPlayerFuncs:AddPill(pill)
    ffichecks.checknumber(2, pill)
    repentogon.L_EntityPlayer_AddPill(self, pill)
end

function EntityPlayerFuncs:AddPlayerFormCostume(form)
    ffichecks.checknumber(2, form)
    repentogon.L_EntityPlayer_AddPlayerFormCostume(self, form)
end

function EntityPlayerFuncs:AddPoopMana(num)
    ffichecks.checknumber(2, num)
    repentogon.L_EntityPlayer_AddPoopMana(self, num)
end

EntityPlayerFuncs.AddPrettyFly = repentogon.L_EntityPlayer_AddPrettyFly

function EntityPlayerFuncs:AddRottenHearts(rottenHearts)
    ffichecks.checknumber(2, rottenHearts)
    repentogon.L_EntityPlayer_AddRottenHearts(self, rottenHearts)
end

function EntityPlayerFuncs:AddSmeltedTrinket(trinketType, firstTimePickingUp)
    ffichecks.checknumber(2, trinketType)
    firstTimePickingUp = ffichecks.optboolean(firstTimePickingUp, true)
    repentogon.L_EntityPlayer_AddSmeltedTrinket(self, trinketType, firstTimePickingUp)
end

function EntityPlayerFuncs:AddSoulCharge(amount)
    ffichecks.checknumber(2, amount)
    repentogon.L_EntityPlayer_AddSoulCharge(self, amount)
end

function EntityPlayerFuncs:AddSoulHearts(soulHearts)
    ffichecks.checknumber(2, soulHearts)
    repentogon.L_EntityPlayer_AddSoulHearts(self, soulHearts)
end

function EntityPlayerFuncs:AddSwarmFlyOrbital(pos)
    ffichecks.checkcdata(2, pos, "Vector")
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_AddSwarmFlyOrbital(self, pos))
end

function EntityPlayerFuncs:AddTrinket(trinketType, firstTimePickingUp)
    ffichecks.checknumber(2, trinketType)
    firstTimePickingUp = ffichecks.optboolean(firstTimePickingUp, true)
    repentogon.L_EntityPlayer_AddTrinket(self, trinketType, firstTimePickingUp)
end

function EntityPlayerFuncs:AddUrnSouls(amount)
    ffichecks.checknumber(2, amount)
    repentogon.L_EntityPlayer_AddUrnSouls(self, amount)
end

function EntityPlayerFuncs:AddWisp(collectible, pos, adjustOrbitLayer, dontUpdate)
    ffichecks.checknumber(2, collectible)
    ffichecks.checkcdata(3, pos, "Vector")
    adjustOrbitLayer = ffichecks.optboolean(adjustOrbitLayer, false)
    dontUpdate = ffichecks.optboolean(dontUpdate, false)
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_AddWisp(self, collectible, pos, adjustOrbitLayer, dontUpdate))
end

function EntityPlayerFuncs:AnimateCard(card, animName)
    ffichecks.checknumber(2, card)
    animName = ffichecks.optstring(animName, "Pickup")

    repentogon.L_EntityPlayer_AnimateCard(self, card, animName)
end

function EntityPlayerFuncs:AnimateCollectible(collectible, animName, spriteAnimName)
    ffichecks.checknumber(2, collectible)
    animName = ffichecks.optstring(animName, "Pickup")
    spriteAnimName = ffichecks.optstring(spriteAnimName, "PlayerPickupSparkle")

    repentogon.L_EntityPlayer_AnimateCollectible(self, collectible, animName, spriteAnimName)
end

EntityPlayerFuncs.AnimateHappy = repentogon.L_EntityPlayer_AnimateHappy
EntityPlayerFuncs.AnimateLightTravel = repentogon.L_EntityPlayer_AnimateLightTravel

function EntityPlayerFuncs:AnimatePickup(anm2, hideShadow, animName)
    ffichecks.checkcdata(2, anm2, "Sprite")
    hideShadow = ffichecks.optboolean(hideShadow, false)
    animName = ffichecks.optstring(animName, "Pickup")

    repentogon.L_EntityPlayer_AnimatePickup(self, anm2, hideShadow, animName)
end

function EntityPlayerFuncs:AnimatePill(pill, animName)
    ffichecks.checknumber(2, pill)
    animName = ffichecks.optstring(animName, "Pickup")

    repentogon.L_EntityPlayer_AnimatePill(self, pill, animName)
end

function EntityPlayerFuncs:AnimatePitfallIn(force)
    force = ffichecks.optboolean(force, false)

    repentogon.L_EntityPlayer_AnimatePitfallIn(self, force)
end

EntityPlayerFuncs.AnimatePitfallOut = repentogon.L_EntityPlayer_AnimatePitfallOut
EntityPlayerFuncs.AnimateSad = repentogon.L_EntityPlayer_AnimateSad

function EntityPlayerFuncs:AnimateTeleport(up)
    up = ffichecks.optboolean(up, false)

    repentogon.L_EntityPlayer_AnimateTeleport(self, up)
end

EntityPlayerFuncs.AnimateTrapdoor = repentogon.L_EntityPlayer_AnimateTrapdoor

function EntityPlayerFuncs:AnimateTrinket(trinket, animName, spriteAnimName)
    ffichecks.checknumber(2, trinket)
    animName = ffichecks.optstring(animName, "Pickup")
    spriteAnimName = ffichecks.optstring(spriteAnimName, "PlayerPickupSparkle")

    repentogon.L_EntityPlayer_AnimateTrinket(self, trinket, animName, spriteAnimName)
end

EntityPlayerFuncs.AreControlsEnabled = repentogon.L_EntityPlayer_AreControlsEnabled
EntityPlayerFuncs.AreOpposingShootDirectionsPressed = repentogon.L_EntityPlayer_AreOpposingShootDirectionsPressed

function EntityPlayerFuncs:CanAddCollectible(type)
    ffichecks.checknumber(2, type)

    return repentogon.L_EntityPlayer_CanAddCollectible(self, type)
end

function EntityPlayerFuncs:CanAddCollectibleToInventory(type)
    ffichecks.checknumber(2, type)

    return repentogon.L_EntityPlayer_CanAddCollectibleToInventory(self, type)
end

EntityPlayerFuncs.CanCrushRocks = repentogon.L_EntityPlayer_CanCrushRocks

function EntityPlayerFuncs:CanOverrideActiveItem(slot)
    ffichecks.checknumber(2, slot)

    return repentogon.L_EntityPlayer_CanOverrideActiveItem(self, slot)
end

EntityPlayerFuncs.CanPickBlackHearts = repentogon.L_EntityPlayer_CanPickBlackHearts
EntityPlayerFuncs.CanPickBoneHearts = repentogon.L_EntityPlayer_CanPickBoneHearts
EntityPlayerFuncs.CanPickGoldenHearts = repentogon.L_EntityPlayer_CanPickGoldenHearts
EntityPlayerFuncs.CanPickRedHearts = repentogon.L_EntityPlayer_CanPickRedHearts
EntityPlayerFuncs.CanPickRottenHearts = repentogon.L_EntityPlayer_CanPickRottenHearts
EntityPlayerFuncs.CanPickSoulHearts = repentogon.L_EntityPlayer_CanPickSoulHearts
EntityPlayerFuncs.CanPickupItem = repentogon.L_EntityPlayer_CanPickupItem
EntityPlayerFuncs.CanShoot = repentogon.L_EntityPlayer_CanShoot

function EntityPlayerFuncs:CanUsePill(pillEffect)
    ffichecks.checknumber(2, pillEffect)

    return repentogon.L_EntityPlayer_CanUsePill(self, pillEffect)
end

EntityPlayerFuncs.CanTurnHead = repentogon.L_EntityPlayer_CanTurnHead

function EntityPlayerFuncs:ChangePlayerType(playerType, force)
    ffichecks.checknumber(2, playerType)
    force = ffichecks.optboolean(force, false)

    repentogon.L_EntityPlayer_ChangePlayerType(self, playerType, force)
end

function EntityPlayerFuncs:CheckFamiliar(familiarVariant, targetCount, rng, sourceItemConfigItem, familiarSubType)
    ffichecks.checknumber(2, familiarVariant)
    ffichecks.checknumber(3, targetCount)
    ffichecks.checkcdata(4, rng, "RNG")
    sourceItemConfigItem = ffichecks.optcdata(sourceItemConfigItem, "ItemConfig_Item", nil)
    familiarSubType = ffichecks.optnumber(familiarSubType, -1)

    repentogon.L_EntityPlayer_CheckFamiliar(self, familiarVariant, targetCount, rng, sourceItemConfigItem, familiarSubType)
end

function EntityPlayerFuncs:ClearCollectibleAnim(item)
    ffichecks.checknumber(2, item)

    repentogon.L_EntityPlayer_ClearCollectibleAnim(self, item)
end

function EntityPlayerFuncs:ClearItemAnimCollectible(item)
    ffichecks.checknumber(2, item)

    repentogon.L_EntityPlayer_ClearItemAnimCollectible(self, item)
end

EntityPlayerFuncs.ClearItemAnimNullItems = repentogon.L_EntityPlayer_ClearItemAnimNullItems
EntityPlayerFuncs.ClearCostumes = repentogon.L_EntityPlayer_ClearCostumes

function EntityPlayerFuncs:ClearDeadEyeCharge(force)
    force = ffichecks.optboolean(force, false)
    
    repentogon.L_EntityPlayer_ClearDeadEyeCharge(self, force)
end

EntityPlayerFuncs.ClearTemporaryEffects = repentogon.L_EntityPlayer_ClearTemporaryEffects

function EntityPlayerFuncs:DischargeActiveItem(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)
    repentogon.L_EntityPlayer_DischargeActiveItem(self, activeSlot)
end

function EntityPlayerFuncs:DonateLuck(luck)
    ffichecks.checknumber(2, luck)
    repentogon.L_EntityPlayer_DonateLuck(self, luck)
end

function EntityPlayerFuncs:DoZitEffect(dir)
    ffichecks.checkcdata(2, dir, "Vector")
    repentogon.L_EntityPlayer_DoZitEffect(self, dir)
end

function EntityPlayerFuncs:DropCollectible(collectible, existingPedestal, removeFromPlayerForm)
    ffichecks.checknumber(2, collectible)
    existingPedestal = ffichecks.optcdata(existingPedestal, "Entity_Pickup", nil)
    removeFromPlayerForm = ffichecks.optboolean(removeFromPlayerForm, false)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_DropCollectible(self, collectible, existingPedestal, removeFromPlayerForm))
end

function EntityPlayerFuncs:DropCollectibleByHistoryIndex(idx, existingPedestal)
    ffichecks.checknumber(2, idx)
    existingPedestal = ffichecks.optcdata(existingPedestal, "Entity_Pickup", nil)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_DropCollectibleByHistoryIndex(self, idx, existingPedestal))
end

function EntityPlayerFuncs:DropPocketItem(pocketNum, pos)
    ffichecks.checknumber(2, pocketNum)
    ffichecks.checkcdata(3, pos, "Vector")

    repentogon.L_EntityPlayer_DropPocketItem(self, pocketNum, pos)
end

function EntityPlayerFuncs:DropTrinket(dropPos, replaceTick)
    ffichecks.checkcdata(2, dropPos, "Vector")
    ffichecks.checkboolean(3, replaceTick)

    repentogon.L_EntityPlayer_DropTrinket(self, dropPos, replaceTick)
end

function EntityPlayerFuncs:EnableWeaponType(weaponType, set)
    ffichecks.checknumber(2, weaponType)
    ffichecks.checkboolean(3, set)

    repentogon.L_EntityPlayer_EnableWeaponType(self, weaponType, set)
end

EntityPlayerFuncs.EvaluateItems = repentogon.L_EntityPlayer_EvaluateItems

function EntityPlayerFuncs:FireBomb(pos, vel, source)
    ffichecks.checkcdata(2, pos, "Vector")
    ffichecks.checkcdata(3, vel, "Vector")
    source = ffichecks.optcdata(source, "Entity", nil)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_FireBomb(self, pos, vel, source))
end

function EntityPlayerFuncs:FireBrimstone(dir, source, damageMultiplier)
    ffichecks.checkcdata(2, dir, "Vector")
    source = ffichecks.optcdata(source, "Entity", nil)
    damageMultiplier = ffichecks.optnumber(damageMultiplier, 1)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_FireBrimstone(self, dir, source, damageMultiplier))
end

function EntityPlayerFuncs:FireBrimstoneBall(pos, vel, offset)
    ffichecks.checkcdata(2, pos, "Vector")
    ffichecks.checkcdata(3, vel, "Vector")
    ffichecks.optcdata(offset, "Vector", Vector.Zero)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_FireBrimstoneBall(self, pos, vel, offset))
end

function EntityPlayerFuncs:FireDelayedBrimstone(angle, source)
    ffichecks.checknumber(2, angle)
    source = ffichecks.optcdata(source, "Entity", nil)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_FireDelayedBrimstone(self, angle, source))
end

function EntityPlayerFuncs:FireKnife(parent, rotationOffset, cantOverwrite, subtype, variant)
    parent = ffichecks.optcdata(parent, "Entity", nil)
    rotationOffset = ffichecks.optnumber(rotationOffset, 0)
    cantOverwrite = ffichecks.optboolean(cantOverwrite, false)
    subtype = ffichecks.optnumber(subtype, 0)
    variant = ffichecks.optnumber(variant, 0)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_FireKnife(self, parent, rotationOffset, cantOverwrite, subtype, variant))
end

function EntityPlayerFuncs:FireTear(pos, vel, canBeEye, noTractorBeam, canTriggerStreakEnd, source, damageMultiplier)
    ffichecks.checkcdata(2, pos, "Vector")
    ffichecks.checkcdata(3, vel, "Vector")
    canBeEye = ffichecks.optboolean(canBeEye, true)
    noTractorBeam = ffichecks.optboolean(noTractorBeam, false)
    canTriggerStreakEnd = ffichecks.optboolean(canTriggerStreakEnd, true)
    source = ffichecks.optcdata(source, "Entity", nil)
    damageMultiplier = ffichecks.optnumber(damageMultiplier, 1)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_FireTear(self, pos, vel, canBeEye, noTractorBeam, canTriggerStreakEnd, source, damageMultiplier))
end

function EntityPlayerFuncs:FireTechLaser(pos, offsetID, dir, leftEye, oneHit, source, damageMultiplier)
    ffichecks.checkcdata(2, pos, "Vector")
    ffichecks.checknumber(3, offsetID)
    ffichecks.checkcdata(4, dir, "Vector")
    ffichecks.checkcdata(5, leftEye)
    oneHit = ffichecks.optboolean(oneHit, false)
    source = ffichecks.optcdata(source, "Entity", nil)
    damageMultiplier = ffichecks.optnumber(damageMultiplier, 1)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_FireTechLaser(self, pos, offsetID, dir, leftEye, oneHit, source, damageMultiplier))
end

EntityPlayerFuncs.FlushQueueItem = repentogon.L_EntityPlayer_FlushQueueItem

function EntityPlayerFuncs:FullCharge(activeSlot, force)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)
    force = ffichecks.optboolean(force, false)

    repentogon.L_EntityPlayer_FullCharge(self, activeSlot, force)
end

function EntityPlayerFuncs:GetActiveCharge(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)

    return repentogon.L_EntityPlayer_GetActiveCharge(self, activeSlot)
end

function EntityPlayerFuncs:GetActiveItem(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)

    return repentogon.L_EntityPlayer_GetActiveItem(self, activeSlot)
end

-- !!!TODO!!!
function EntityPlayerFuncs:GetActiveItemDesc(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)

    -- return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_))
end

function EntityPlayerFuncs:GetActiveItemSlot(id)
    ffichecks.checknumber(2, id)

    return repentogon.L_EntityPlayer_GetActiveItemSlot(self, id)
end

function EntityPlayerFuncs:GetActiveMaxCharge(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)

    return repentogon.L_EntityPlayer_GetActiveMaxCharge(self, activeSlot)
end

function EntityPlayerFuncs:GetActiveMinUsableCharge(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)

    return repentogon.L_EntityPlayer_GetActiveMinUsableCharge(self, activeSlot)
end

function EntityPlayerFuncs:GetActiveSubCharge(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)

    return repentogon.L_EntityPlayer_GetActiveSubCharge(self, activeSlot)
end

function EntityPlayerFuncs:GetActiveWeaponEntity()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetActiveWeaponEntity(self))
end

EntityPlayerFuncs.GetActiveWeaponNumFired = repentogon.L_EntityPlayer_GetActiveWeaponNumFired

function EntityPlayerFuncs:GetAimDirection()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetAimDirection(self))
end

EntityPlayerFuncs.GetBabySkin = repentogon.L_EntityPlayer_GetBabySkin
EntityPlayerFuncs.GetBagOfCraftingOutput = repentogon.L_EntityPlayer_GetBagOfCraftingOutput

function EntityPlayerFuncs:GetBagOfCraftingSlot(slot)
    ffichecks.checknumber(2, slot)
    if slot < 0 or slot > 7 then
		error(string.format("EntityPlayer:GetBagOfCraftingSlot: invalid slot id %d", slot))
	end

    return repentogon.L_EntityPlayer_GetBagOfCraftingSlot(self, slot)
end

function EntityPlayerFuncs:GetBatteryCharge(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)
    
    return repentogon.L_EntityPlayer_GetBatteryCharge(self, activeSlot)
end

EntityPlayerFuncs.GetBabySkin = repentogon.L_EntityPlayer_GetBabySkin
EntityPlayerFuncs.GetBagOfCraftingOutput = repentogon.L_EntityPlayer_GetBagOfCraftingOutput

-- !!!TODO!!!
function EntityPlayerFuncs:GetBombFlags()
    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetBombFlags(self)))
end

-- https://ptb.discord.com/channels/1069368602156863610/1069368602597273630/1209332490511319113
function EntityPlayerFuncs:GetBombVariant(tearFlags, ForceSmallBomb)
    return 0 
end

EntityPlayerFuncs.GetBoneHearts = repentogon.L_EntityPlayer_GetBoneHearts
EntityPlayerFuncs.GetBrokenHearts = repentogon.L_EntityPlayer_GetBrokenHearts
EntityPlayerFuncs.GetCambionConceptionState = repentogon.L_EntityPlayer_GetCambionConceptionState
EntityPlayerFuncs.GetCambionPregnancyLevel = repentogon.L_EntityPlayer_GetCambionPregnancyLevel

function EntityPlayerFuncs:GetCard(slotId)
    ffichecks.checknumber(2, slotId)

    return repentogon.L_EntityPlayer_GetCard(self, slotId)
end

function EntityPlayerFuncs:GetCardRNG(id)
    ffichecks.checknumber(2, id)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetCardRNG(self, id))
end

EntityPlayerFuncs.GetCollectibleCount = repentogon.L_EntityPlayer_GetCollectibleCount

function EntityPlayerFuncs:GetCollectibleNum(type, onlyCountTrueItems)
    ffichecks.checknumber(2, type)
    onlyCountTrueItems = ffichecks.optboolean(onlyCountTrueItems, false)

    return repentogon.L_EntityPlayer_GetCollectibleNum(self, type, onlyCountTrueItems)
end

function EntityPlayerFuncs:GetCollectibleRNG(id)
    ffichecks.checknumber(2, id)

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetCollectibleRNG(self, id))
end

-- !!!TODO!!!
function EntityPlayerFuncs:GetCollectiblesList()
    -- ffichecks.
end

-- !!!TODO!!!
function EntityPlayerFuncs:GetCostumeLayerMap()

end

-- !!!TODO!!!
function EntityPlayerFuncs:GetCostumeSpriteDescs()

end

function EntityPlayerFuncs:GetCostumeNullPos(nullFrameName, headScale, dir)
    ffichecks.checkstring(2, nullFrameName)
    ffichecks.checkboolean(3, headScale)
    ffichecks.checkcdata(4, dir, "Vector")

    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetCostumeNullPos()))
end

EntityPlayerFuncs.GetD8DamageModifier = repentogon.L_EntityPlayer_GetD8DamageModifier
EntityPlayerFuncs.GetD8FireDelayModifier = repentogon.L_EntityPlayer_GetD8FireDelayModifier
EntityPlayerFuncs.GetD8RangeModifier = repentogon.L_EntityPlayer_GetD8RangeModifier
EntityPlayerFuncs.GetD8SpeedModifier = repentogon.L_EntityPlayer_GetD8SpeedModifier
EntityPlayerFuncs.GetDamageCooldown = repentogon.L_EntityPlayer_GetDamageCooldown
EntityPlayerFuncs.GetDamageModifier = repentogon.L_EntityPlayer_GetDamageModifier
EntityPlayerFuncs.GetDeadEyeCharge = repentogon.L_EntityPlayer_GetDeadEyeCharge

function EntityPlayerFuncs:GetDeathAnimName()
    return lffi.string(repentogon.L_EntityPlayer_GetDeathAnimName(self))
end

EntityPlayerFuncs.GetEdenDamage = repentogon.L_EntityPlayer_GetEdenDamage
EntityPlayerFuncs.GetEdenFireDelay = repentogon.L_EntityPlayer_GetEdenFireDelay
EntityPlayerFuncs.GetEdenLuck = repentogon.L_EntityPlayer_GetEdenLuck
EntityPlayerFuncs.GetEdenRange = repentogon.L_EntityPlayer_GetEdenRange
EntityPlayerFuncs.GetEdenShotSpeed = repentogon.L_EntityPlayer_GetEdenShotSpeed
EntityPlayerFuncs.GetEdenSpeed = repentogon.L_EntityPlayer_GetEdenSpeed
EntityPlayerFuncs.GetEffectiveBloodCharge = repentogon.L_EntityPlayer_GetEffectiveBloodCharge
EntityPlayerFuncs.GetEffectiveMaxHearts = repentogon.L_EntityPlayer_GetEffectiveMaxHearts
EntityPlayerFuncs.GetEffectiveSoulCharge = repentogon.L_EntityPlayer_GetEffectiveSoulCharge
EntityPlayerFuncs.GetEffects = repentogon.L_EntityPlayer_GetEffects
EntityPlayerFuncs.GetEffectiveSoulCharge = repentogon.L_EntityPlayer_GetEffectiveSoulCharge

function EntityPlayerFuncs:GetEnterPosition()
    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetEnterPosition(self)))
end

function EntityPlayerFuncs:GetEntityConfigPlayer()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetEntityConfigPlayer(self))
end

EntityPlayerFuncs.GetEpiphoraCharge = repentogon.L_EntityPlayer_GetEpiphoraCharge
EntityPlayerFuncs.GetEternalHearts = repentogon.L_EntityPlayer_GetEternalHearts
EntityPlayerFuncs.GetEveSumptoriumCharge = repentogon.L_EntityPlayer_GetEveSumptoriumCharge
EntityPlayerFuncs.GetExtraLives = repentogon.L_EntityPlayer_GetExtraLives
EntityPlayerFuncs.GetFireDelayModifier = repentogon.L_EntityPlayer_GetFireDelayModifier
EntityPlayerFuncs.GetFireDirection = repentogon.L_EntityPlayer_GetFireDirection

function EntityPlayerFuncs:GetFlippedForm()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetFlippedForm(self))
end

function EntityPlayerFuncs:GetFocusEntity()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetFocusEntity(self))
end

function EntityPlayerFuncs:GetFootprintColor(leftFootPrint)
    leftFootPrint = ffichecks.optboolean(leftFootPrint, false)
    return repentogon.L_EntityPlayer_GetFootprintColor(self, leftFootPrint)
end

function EntityPlayerFuncs:GetFlyingOffset()
    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetFlyingOffset(self)))
end

EntityPlayerFuncs.GetGlitchBabySubType = repentogon.L_EntityPlayer_GetGlitchBabySubType

-- TODO
function EntityPlayerFuncs:GetGlyphOfBalanceDrop(variant, subtype)
    variant = ffichecks.optnumber(variant, -1)
    subtype = ffichecks.optnumber(subtype, -1)

    repentogon.L_EntityPlayer_GetGlyphOfBalanceDrop(self, variant, subtype)

    return {
        Variant = variant,
        SubType = subtype
    }
end

-- ?????? https://repentogon.com/EntityPlayer.html#getgnawedleaftimer
EntityPlayerFuncs.GetGnawedLeafTimer = repentogon.L_EntityPlayer_GetGnawedLeafTimer

EntityPlayerFuncs.GetGoldenHearts = repentogon.L_EntityPlayer_GetGoldenHearts
EntityPlayerFuncs.GetGreedDonationBreakChance = repentogon.L_EntityPlayer_GetGreedDonationBreakChance
EntityPlayerFuncs.GetGreedsGulletHearts = repentogon.L_EntityPlayer_GetGreedsGulletHearts
EntityPlayerFuncs.GetHeadColor = repentogon.L_EntityPlayer_GetHeadColor
EntityPlayerFuncs.GetHeadDirection = repentogon.L_EntityPlayer_GetHeadDirection
EntityPlayerFuncs.GetHealthType = repentogon.L_EntityPlayer_GetHealthType
EntityPlayerFuncs.GetHeartLimit = repentogon.L_EntityPlayer_GetHeartLimit
EntityPlayerFuncs.GetHearts = repentogon.L_EntityPlayer_GetHearts

function EntityPlayerFuncs:GetHeldEntity()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetHeldEntity(self))
end

function EntityPlayerFuncs:GetHeldSprite()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetHeldSprite(self))
end

function EntityPlayerFuncs:GetHistory()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetHistory(self))
end

EntityPlayerFuncs.GetImmaculateConceptionState = repentogon.L_EntityPlayer_GetImmaculateConceptionState
EntityPlayerFuncs.GetItemState = repentogon.L_EntityPlayer_GetItemState
EntityPlayerFuncs.GetJarFlies = repentogon.L_EntityPlayer_GetJarFlies
EntityPlayerFuncs.GetJarHearts = repentogon.L_EntityPlayer_GetJarHearts
EntityPlayerFuncs.GetKeepersSackBonus = repentogon.L_EntityPlayer_GetKeepersSackBonus

function EntityPlayerFuncs:GetLaserColor()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetLaserColor(self))
end

function EntityPlayerFuncs:GetLaserOffset(id, dir)
    ffichecks.checknumber(2, id)
    ffichecks.checkcdata(3, dir, "Vector")

    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetLaserOffset(self, id, dir)))
end

EntityPlayerFuncs.GetLuckModifier = repentogon.L_EntityPlayer_GetLuckModifier
EntityPlayerFuncs.GetLastActionTriggers = repentogon.L_EntityPlayer_GetLastActionTriggers
EntityPlayerFuncs.GetLastDamageFlags = repentogon.L_EntityPlayer_GetLastDamageFlags
EntityPlayerFuncs.GetLastDamageSource = repentogon.L_EntityPlayer_GetLastDamageSource

function EntityPlayerFuncs:GetLastDamageSource()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetLastDamageSource(self))
end

function EntityPlayerFuncs:GetLastDirection()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetLastDirection(self))
end

EntityPlayerFuncs.GetMaggySwingCooldown = repentogon.L_EntityPlayer_GetMaggySwingCooldown

function EntityPlayerFuncs:GetMainTwin()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetMainTwin(self))
end

function EntityPlayerFuncs:GetMarkedTarget()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetMarkedTarget(self))
end

EntityPlayerFuncs.GetMaxBladderCharge = repentogon.L_EntityPlayer_GetMaxBladderCharge
EntityPlayerFuncs.GetMaxHearts = repentogon.L_EntityPlayer_GetMaxHearts
EntityPlayerFuncs.GetMaxPeeBurstCooldown = repentogon.L_EntityPlayer_GetMaxPeeBurstCooldown
EntityPlayerFuncs.GetMaxPocketItems = repentogon.L_EntityPlayer_GetMaxPocketItems
EntityPlayerFuncs.GetMaxPoopMana = repentogon.L_EntityPlayer_GetMaxPoopMana
EntityPlayerFuncs.GetMaxTrinkets = repentogon.L_EntityPlayer_GetMaxTrinkets
EntityPlayerFuncs.GetMegaBlastDuration = repentogon.L_EntityPlayer_GetMegaBlastDuration
EntityPlayerFuncs.GetMetronomeCollectibleID = repentogon.L_EntityPlayer_GetMetronomeCollectibleID
EntityPlayerFuncs.GetModelingClayEffect = repentogon.L_EntityPlayer_GetModelingClayEffect
EntityPlayerFuncs.GetMovementDirection = repentogon.L_EntityPlayer_GetMovementDirection

function EntityPlayerFuncs:GetMovementInput(moveSpeed)
    ffichecks.optnumber(moveSpeed, 0)
    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetMovementInput(self, moveSpeed)))
end

function EntityPlayerFuncs:GetMovementJoystick()
    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetMovementInput(self)))
end

function EntityPlayerFuncs:GetMovementVector()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetMovementVector())
end

function EntityPlayerFuncs:GetMultiShotParams(weaponType)
    ffichecks.checknumber(2, weaponType)
    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetMultiShotParams(self, weaponType)))
end

function EntityPlayerFuncs:GetMultiShotPositionVelocity(loopIndex, weapon, shotDirection, shotSpeed, params)
    ffichecks.checknumber(2, loopIndex)
    ffichecks.checknumber(3, weapon)
    ffichecks.checkcdata(4, shotDirection, "Vector")
    ffichecks.checknumber(5, shotSpeed)
    ffichecks.checkcdata(6, params, "Weapon_MultiShotParams")

    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetMultiShotPositionVelocity(self, loopIndex, weapon, shotDirection, shotSpeed, params)))
end

function EntityPlayerFuncs:GetName()
    return lffi.string(repentogon.L_EntityPlayer_GetName(self))
end

EntityPlayerFuncs.GetNumBlueFlies = repentogon.L_EntityPlayer_GetNumBlueFlies
EntityPlayerFuncs.GetNumBlueSpiders = repentogon.L_EntityPlayer_GetNumBlueSpiders
EntityPlayerFuncs.GetNumBombs = repentogon.L_EntityPlayer_GetNumBombs
EntityPlayerFuncs.GetNumCoins = repentogon.L_EntityPlayer_GetNumCoins
EntityPlayerFuncs.GetNumGigaBombs = repentogon.L_EntityPlayer_GetNumGigaBombs
EntityPlayerFuncs.GetNumKeys = repentogon.L_EntityPlayer_GetNumKeys

function EntityPlayerFuncs:GetOtherTwin()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetOtherTwin(self))
end

EntityPlayerFuncs.GetPeeBurstCooldown = repentogon.L_EntityPlayer_GetPeeBurstCooldown

function EntityPlayerFuncs:GetPill(slotId)
    ffichecks.checknumber(2, slotId)
    return repentogon.L_EntityPlayer_GetPill(self, slotId)
end

function EntityPlayerFuncs:GetPillRNG(id)
    ffichecks.checknumber(2, id)
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetPillRNG(self, id))
end

function EntityPlayerFuncs:GetPlayerFormCounter(form)
    ffichecks.checknumber(2, form)

    if form < 0 or form > 14 then
        error(string.format("Invalid PlayerForm %d", form))
    end

    return repentogon.L_EntityPlayer_GetPlayerFormCounter(self, form)
end

EntityPlayerFuncs.GetPlayerType = repentogon.L_EntityPlayer_GetPlayerType
EntityPlayerFuncs.GetPonyCharge = repentogon.L_EntityPlayer_GetPonyCharge
EntityPlayerFuncs.GetPurityState = repentogon.L_EntityPlayer_GetPurityState

function EntityPlayerFuncs:GetPoopSpell(pos)
    if pos < 0 or pos > 5 then
		error(string.format("Invalid Poop Spell queue position %d", pos))
	end

    return repentogon.L_EntityPlayer_GetPoopSpell(self, pos)
end

function EntityPlayerFuncs:GetRecentMovementVector()
    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetRecentMovementVector(self)))
end

EntityPlayerFuncs.GetRottenHearts = repentogon.L_EntityPlayer_GetRottenHearts

function EntityPlayerFuncs:GetShootingInput()
    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetShootingInput(self)))
end

function EntityPlayerFuncs:GetShootingJoystick()
    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetShootingInput(self)))
end

EntityPlayerFuncs.GetShotSpeedModifier = repentogon.L_EntityPlayer_GetShotSpeedModifier

-- !!!TODO!!!
function EntityPlayerFuncs:GetSmeltedTrinkets()

end

function EntityPlayerFuncs:GetSpecialGridCollision(pos)
    ffichecks.checkcdata(2, pos, "Vector")
    return repentogon.L_EntityPlayer_GetSpecialGridCollision(pos)
end

EntityPlayerFuncs.GetSpeedModifier = repentogon.L_EntityPlayer_GetSpeedModifier

function EntityPlayerFuncs:GetTotalActiveCharge(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)
    return repentogon.L_EntityPlayer_GetTotalActiveCharge(self, activeSlot)
end

-- !!!TODO!!!
function EntityPlayerFuncs:GetVoidedCollectiblesList()

end

function EntityPlayerFuncs:GetWeapon(index)
    if index < 0 or index > 4 then
        error(string.format("Invalid Weapon index %d", index))
    end

    return repentogon.L_EntityPlayer_GetWeapon(self, index)
end

EntityPlayerFuncs.GetWeaponModifiers = repentogon.L_EntityPlayer_GetWeaponModifiers
EntityPlayerFuncs.GetWildCardItem = repentogon.L_EntityPlayer_GetWildCardItem
EntityPlayerFuncs.GetWildCardItemType = repentogon.L_EntityPlayer_GetWildCardItemType

-- !!!TODO!!!
function EntityPlayerFuncs:GetWispCollectiblesList()

end

EntityPlayerFuncs.GetSmoothBodyRotation = repentogon.L_EntityPlayer_GetSmoothBodyRotation
EntityPlayerFuncs.GetSoulCharge = repentogon.L_EntityPlayer_GetSoulCharge
EntityPlayerFuncs.GetSoulHearts = repentogon.L_EntityPlayer_GetSoulHearts

function EntityPlayerFuncs:GetSubPlayer()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetSubPlayer(self))
end

function EntityPlayerFuncs:GetTearHitParams(weaponType, damageScale, tearDisplacement, source)
    ffichecks.checknumber(2, weaponType)
    damageScale = ffichecks.optnumber(damageScale, 1)
    tearDisplacement = ffichecks.optnumber(tearDisplacement, 1)
    source = ffichecks.optcdata(source, "Entity", nil)

    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetTearHitParams(self, weaponType, damageScale, tearDisplacement, source)))
end

function EntityPlayerFuncs:GetTearMovementInheritance(shotDirection)
    ffichecks.checkcdata(2, shotDirection, "Vector")

    return GC(ffichecks.fixreturn(repentogon.L_EntityPlayer_GetTearMovementInheritance(self, shotDirection)))
end

EntityPlayerFuncs.GetTearPoisonDamage = repentogon.L_EntityPlayer_GetTearPoisonDamage
EntityPlayerFuncs.GetTearRangeModifier = repentogon.L_EntityPlayer_GetTearRangeModifier
EntityPlayerFuncs.GetTotalDamageTaken = repentogon.L_EntityPlayer_GetTotalDamageTaken
EntityPlayerFuncs.GetTearRangeModifier = repentogon.L_EntityPlayer_GetTearRangeModifier

function EntityPlayerFuncs:GetTractorBeam()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetTractorBeam(self))
end

-- TODO: return ItemConfig::Item here
function EntityPlayerFuncs:GetTrinket(trinketIndex)
    ffichecks.checknumber(2, trinketIndex)
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetTrinket(self, trinketIndex))
end

function EntityPlayerFuncs:GetTrinketMultiplier(trinketID)
    ffichecks.checknumber(2, trinketID)
    return repentogon.L_EntityPlayer_GetTrinketMultiplier(self, trinketID)
end

function EntityPlayerFuncs:GetTrinketRNG(trinketID)
    ffichecks.checknumber(2, trinketID)
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetTrinketRNG(self, trinketID))
end

function EntityPlayerFuncs:GetVelocityBeforeUpdate()
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_GetVelocityBeforeUpdate(self))
end

EntityPlayerFuncs.GetZodiacEffect = repentogon.L_EntityPlayer_GetZodiacEffect

function EntityPlayerFuncs:HasCollectible(type, ignoreModifiers)
    ffichecks.checknumber(2, type)
    ignoreModifiers = ffichecks.optboolean(ignoreModifiers, false)

    return repentogon.L_EntityPlayer_HasCollectible(self, type, ignoreModifiers)
end

EntityPlayerFuncs.HasCurseMistEffect = repentogon.L_EntityPlayer_HasCurseMistEffect
EntityPlayerFuncs.HasFullHearts = repentogon.L_EntityPlayer_HasFullHearts
EntityPlayerFuncs.HasFullHeartsAndSoulHearts = repentogon.L_EntityPlayer_HasFullHeartsAndSoulHearts
EntityPlayerFuncs.HasGoldenBomb = repentogon.L_EntityPlayer_HasGoldenBomb
EntityPlayerFuncs.HasGoldenKey = repentogon.L_EntityPlayer_HasGoldenKey
EntityPlayerFuncs.HasInstantDeathCurse = repentogon.L_EntityPlayer_HasInstantDeathCurse

function EntityPlayerFuncs:HasInvincibility(dmgFlags)
    dmgFlags = ffichecks.optnumber(dmgFlags, 0)
    return repentogon.L_EntityPlayer_HasInvincibility(self, dmgFlags)
end

EntityPlayerFuncs.HasPoisonImmunity = repentogon.L_EntityPlayer_HasPoisonImmunity

function EntityPlayerFuncs:HasPlayerForm(form)
    ffichecks.checknumber(2, form)
    return repentogon.L_EntityPlayer_HasPlayerForm(self, form)
end

EntityPlayerFuncs.HasTimedItem = repentogon.L_EntityPlayer_HasTimedItem

function EntityPlayerFuncs:HasTrinket(type, ignoreModifiers)
    ffichecks.checknumber(2, type)
    ignoreModifiers = ffichecks.optboolean(ignoreModifiers, false)

    return repentogon.L_EntityPlayer_HasTrinket(self, type, ignoreModifiers)
end

function EntityPlayerFuncs:HasWeaponType(weaponType)
    ffichecks.checknumber(2, weaponType)

    return repentogon.L_EntityPlayer_HasWeaponType(self, weaponType)
end

function EntityPlayerFuncs:IncrementPlayerFormCounter(ePlayerForm, count)
    ffichecks.checknumber(2, ePlayerForm)
    ffichecks.checknumber(3, count)

    repentogon.L_EntityPlayer_IncrementPlayerFormCounter(self, ePlayerForm, count)
end

EntityPlayerFuncs.InitBabySkin = repentogon.L_EntityPlayer_InitBabySkin
EntityPlayerFuncs.InitPostLevelInitStats = repentogon.L_EntityPlayer_InitPostLevelInitStats

function EntityPlayerFuncs:InitTwin(playerType)
    ffichecks.checknumber(2, playerType)
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_InitTwin(self, playerType))
end

function EntityPlayerFuncs:IsBlackHeart(heart)
    ffichecks.checknumber(2, heart)
    return repentogon.L_EntityPlayer_IsBlackHeart(self, heart)
end

function EntityPlayerFuncs:IsBoneHeart(heart)
    ffichecks.checknumber(2, heart)
    return repentogon.L_EntityPlayer_IsBoneHeart(self, heart)
end

function EntityPlayerFuncs:IsCollectibleAnimFinished(itemType, anim)
    ffichecks.checknumber(2, itemType)
    ffichecks.checkstring(3, anim)

    return repentogon.L_EntityPlayer_IsCollectibleAnimFinished(self, itemType, anim)
end

function EntityPlayerFuncs:IsCollectibleCostumeVisible(itemType, layerIDOrName)
    ffichecks.checknumber(2, itemType)
    if ffichecks.isnumber(layerIDOrName) then
        return repentogon.L_EntityPlayer_IsCollectibleCostumeVisibleLayerID(self, itemType, layerIDOrName)
    end
    
    ffichecks.checkstring(3, layerIDOrName)
    return repentogon.L_EntityPlayer_IsCollectibleCostumeVisibleLayerName(self, itemType, layerIDOrName)
end

EntityPlayerFuncs.IsCoopGhost = repentogon.L_EntityPlayer_IsCoopGhost

function EntityPlayerFuncs:IsEntityValidTarget(ent)
    ffichecks.checkcdata(2, ent, "Entity")

    return repentogon.L_EntityPlayer_IsEntityValidTarget(self, ent)
end

EntityPlayerFuncs.IsExtraAnimationFinished = repentogon.L_EntityPlayer_IsExtraAnimationFinished

function EntityPlayerFuncs:IsFootstepFrame(foot)
    foot = ffichecks.optnumber(foot, -1)

    return repentogon.L_EntityPlayer_IsEntityValidTarget(self, foot)
end

EntityPlayerFuncs.IsFullSpriteRendering = repentogon.L_EntityPlayer_IsFullSpriteRendering
EntityPlayerFuncs.IsHeadless = repentogon.L_EntityPlayer_IsHeadless
EntityPlayerFuncs.IsHeldItemVisible = repentogon.L_EntityPlayer_IsHeldItemVisible
EntityPlayerFuncs.IsHoldingItem = repentogon.L_EntityPlayer_IsHoldingItem
EntityPlayerFuncs.IsHologram = repentogon.L_EntityPlayer_IsHologram
EntityPlayerFuncs.IsInvisible = repentogon.L_EntityPlayer_IsInvisible

function EntityPlayerFuncs:IsItemCostumeVisible(item, layerIDOrName)
    ffichecks.checkcdata(2, item, "ItemConfig_Item")
    if ffichecks.isnumber(layerIDOrName) then
        return repentogon.L_EntityPlayer_IsItemCostumeVisible(self, item, layerIDOrName)
    end

    ffichecks.checkstring(3, layerIDOrName)
    return repentogon.L_EntityPlayer_IsItemCostumeVisibleLayerName(self, item, layerIDOrName)
end

EntityPlayerFuncs.IsItemQueueEmpty = repentogon.L_EntityPlayer_IsItemQueueEmpty
EntityPlayerFuncs.IsLocalPlayer = repentogon.L_EntityPlayer_IsLocalPlayer

function EntityPlayerFuncs:IsNullItemCostumeVisible(nullItem, layerIDOrName)
    ffichecks.checknumber(2, nullItem)
    if ffichecks.isnumber(layerIDOrName) then
        return repentogon.L_EntityPlayer_IsNullItemCostumeVisible(self, nullItem, layerIDOrName)
    end

    ffichecks.checkstring(3, layerIDOrName)
    return repentogon.L_EntityPlayer_IsNullItemCostumeVisibleLayerName(self, nullItem, layerIDOrName)
end

EntityPlayerFuncs.IsP2Appearing = repentogon.L_EntityPlayer_IsP2Appearing
EntityPlayerFuncs.IsPacifist = repentogon.L_EntityPlayer_IsPacifist

function EntityPlayerFuncs:IsPosInSpotLight(position)
    ffichecks.checkcdata(2, position, "Vector")

    return repentogon.L_EntityPlayer_IsPosInSpotLight(self, position)
end

EntityPlayerFuncs.IsSubPlayer = repentogon.L_EntityPlayer_IsSubPlayer
EntityPlayerFuncs.IsUrethraBlocked = repentogon.L_EntityPlayer_IsUrethraBlocked
EntityPlayerFuncs.MorphToCoopGhost = repentogon.L_EntityPlayer_MorphToCoopGhost

function EntityPlayerFuncs:NeedsCharge(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)

    return repentogon.L_EntityPlayer_NeedsCharge(self, activeSlot)
end

function EntityPlayerFuncs:PlayCollectibleAnim(item, checkBodyLayers, anim, frame)
    ffichecks.checknumber(2, item)
    ffichecks.checkboolean(3, checkBodyLayers)
    ffichecks.checkstring(4, anim)
    frame = ffichecks.optboolean(frame, -1)

    repentogon.L_EntityPlayer_PlayCollectibleAnim(self, item, checkBodyLayers, anim, frame)
end

function EntityPlayerFuncs:PlayDelayedSFX(soundID, soundDelay, frameDelay, volume)
    ffichecks.checknumber(2, soundID)
    soundDelay = ffichecks.optnumber(soundDelay, 0)
    frameDelay = ffichecks.optnumber(frameDelay, 2)
    volume = ffichecks.optnumber(frameDelay, 1)

    repentogon.L_EntityPlayer_PlayDelayedSFX(self, soundID, soundDelay, frameDelay, volume)
end

function EntityPlayerFuncs:PlayExtraAnimation(animation)
    ffichecks.checkstring(2, animation)

    repentogon.L_EntityPlayer_PlayExtraAnimation(self, animation)
end

function EntityPlayerFuncs:QueueExtraAnimation(animation)
    ffichecks.checkstring(2, animation)

    repentogon.L_EntityPlayer_QueueExtraAnimation(self, animation)
end

-- need some help on this one too
function EntityPlayerFuncs:QueueItem(item, charge, touched, golden, varData)

end

function EntityPlayerFuncs:RemoveBlackHeart(blackHeart)
    ffichecks.checknumber(2, blackHeart)

    repentogon.L_EntityPlayer_RemoveBlackHeart(self, blackHeart)
end

EntityPlayerFuncs.RemoveBlueFly = repentogon.L_EntityPlayer_RemoveBlueFly
EntityPlayerFuncs.RemoveBlueSpider = repentogon.L_EntityPlayer_RemoveBlueSpider

function EntityPlayerFuncs:RemoveCollectible(item, ignoreModifiers, activeSlot, removeFromPlayerForm)
    ffichecks.checknumber(2, item)
    ignoreModifiers = ffichecks.optboolean(ignoreModifiers, false)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)
    removeFromPlayerForm = ffichecks.optboolean(removeFromPlayerForm, true)

    repentogon.L_EntityPlayer_RemoveCollectible(self, item, ignoreModifiers, activeSlot, removeFromPlayerForm)
end

function EntityPlayerFuncs:RemoveCollectibleByHistoryIndex(idx)
    ffichecks.checknumber(2, idx)

    repentogon.L_EntityPlayer_RemoveCollectibleByHistoryIndex(self, idx)
end

function EntityPlayerFuncs:RemoveCostume(item)
    ffichecks.checkcdata(2, item, "ItemConfig_Item")

    repentogon.L_EntityPlayer_RemoveCostume(self, item)
end

EntityPlayerFuncs.RemoveCurseMistEffect = repentogon.L_EntityPlayer_RemoveCurseMistEffect
EntityPlayerFuncs.RemoveGoldenBomb = repentogon.L_EntityPlayer_RemoveGoldenBomb
EntityPlayerFuncs.RemoveGoldenKey = repentogon.L_EntityPlayer_RemoveGoldenKey

function EntityPlayerFuncs:RemovePocketItem(activeSlot)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)
end

function EntityPlayerFuncs:RemovePoopSpell(pos)
    if pos < 0 or pos > 5 then
        error(string.format("Invalid Poop Spell queue position %d", pos))
    end

    repentogon.L_EntityPlayer_RemovePoopSpell(self, pos)
end

EntityPlayerFuncs.RemoveSkinCostume = repentogon.L_EntityPlayer_RemoveSkinCostume

function EntityPlayerFuncs:RenderBody(pos)
    ffichecks.checkcdata(2, pos, "Vector")

    repentogon.L_EntityPlayer_RenderBody(self, pos)
end

function EntityPlayerFuncs:RenderDebugInfo(position)
    ffichecks.checkcdata(2, position, "Vector")

    repentogon.L_EntityPlayer_RenderBody(self, position)
end

function EntityPlayerFuncs:RenderGlow(position)
    ffichecks.checkcdata(2, position, "Vector")

    repentogon.L_EntityPlayer_RenderBody(self, position)
end

function EntityPlayerFuncs:RenderHead(position)
    ffichecks.checkcdata(2, position, "Vector")

    repentogon.L_EntityPlayer_RenderBody(self, position)
end

function EntityPlayerFuncs:RenderHeadBack(position)
    ffichecks.checkcdata(2, position, "Vector")

    repentogon.L_EntityPlayer_RenderBody(self, position)
end

function EntityPlayerFuncs:RenderShadowLayer(position)
    ffichecks.checkcdata(2, position, "Vector")

    repentogon.L_EntityPlayer_RenderBody(self, position)
end

function EntityPlayerFuncs:RenderTop(position)
    ffichecks.checkcdata(2, position, "Vector")

    repentogon.L_EntityPlayer_RenderBody(self, position)
end

function EntityPlayerFuncs:ReplaceCostumeSprite(item, spritePath, spriteId)
    ffichecks.checkcdata(2, item, "ItemConfig_Item")
    ffichecks.checkstring(3, spritePath)
    ffichecks.checknumber(4, spriteId)

    repentogon.L_EntityPlayer_ReplaceCostumeSprite(self, item, spritePath, spriteId)
end

function EntityPlayerFuncs:RerollAllCollectibles(rng, includeActiveItems)
    rng = ffichecks.optcdata(rng, "RNG", self.DropRNG)
    includeActiveItems = ffichecks.optboolean(includeActiveItems, false)

    repentogon.L_EntityPlayer_RerollAllCollectibles(self, rng, includeActiveItems)
end

EntityPlayerFuncs.ResetDamageCooldown = repentogon.L_EntityPlayer_ResetDamageCooldown
EntityPlayerFuncs.ResetItemState = repentogon.L_EntityPlayer_ResetItemState
EntityPlayerFuncs.RespawnFamiliars = repentogon.L_EntityPlayer_RespawnFamiliars
EntityPlayerFuncs.Revive = repentogon.L_EntityPlayer_Revive
EntityPlayerFuncs.ReviveCoopGhost = repentogon.L_EntityPlayer_ReviveCoopGhost

function EntityPlayerFuncs:SalvageCollectible(pickup, rng, pool, removePickup)
    if ffichecks.iscdata(pickup, "Entity_Pickup") then
        rng = ffichecks.optcdata(rng, "RNG", self.DropRNG)
        pool = ffichecks.optnumber(pool, ItemPoolType.POOL_NULL)
        removePickup = ffichecks.optboolean(removePickup, true)

        repentogon.L_EntityPlayer_SalvageCollectiblePickup(self, pickup, rng, pool, removePickup)
        return
    end

    ffichecks.checknumber(2, pickup)
    rng = ffichecks.optcdata(rng, "Vector", self.Position)
    pool = ffichecks.optcdata(pool, "RNG", self.DropRNG)
    removePickup = ffichecks.optnumber(removePickup, ItemPoolType.POOL_NULL)

    repentogon.L_EntityPlayer_SalvageCollectibleID(self, pickup, rng, pool, removePickup)
end

function EntityPlayerFuncs:SetActiveCharge(charge, activeSlot)
    ffichecks.checknumber(2, charge)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)

    repentogon.L_EntityPlayer_SetActiveCharge(self, charge, activeSlot)
end

function EntityPlayerFuncs:SetActiveVarData(vardata, slot)
    ffichecks.checknumber(2, vardata)
    activeSlot = ffichecks.optnumber(activeSlot, ActiveSlot.SLOT_PRIMARY)

    repentogon.L_EntityPlayer_SetActiveVarData(self, vardata, slot)
end

function EntityPlayerFuncs:SetBagOfCraftingContent(pickups)
    if type(pickups) ~= "table" then
        error("EntityPlayer:SetBagOfCraftingContent: Expected a table as second argument")
    end

    if #pickups > 8 then
        error("EntityPlayer:SetBagOfCraftingContent: Table cannot be larger than 8 pickups")
    end

    local list = lffi.new("int[8]")

    for i, pickup in ipairs(pickups) do
        if pickup < 0 or pickup > 29 then
            error(string.format("EntityPlayer:SetBagOfCraftingContent: Invalid pickup %d at index %d", pickup))
        end
        list[i-1] = pickup
    end

    repentogon.L_EntityPlayer_SetBagOfCraftingContent(self, list)
end

function EntityPlayerFuncs:SetBagOfCraftingOutput(output)
    ffichecks.checknumber(2, output)
    repentogon.L_EntityPlayer_SetBagOfCraftingOutput(self, output)
end

function EntityPlayerFuncs:SetBagOfCraftingSlot(slot, pickup)
    ffichecks.checknumber(2, slot)
    if slot < 0 or slot > 7 then
		error(string.format("EntityPlayer:GetBagOfCraftingSlot: invalid slot id %d", slot))
	end

    ffichecks.checknumber(3, pickup)
    if pickup < 0 or pickup > 29 then
        error(string.format("EntityPlayer:SetBagOfCraftingContent: Invalid pickup %d at index %d", pickup))
    end

    repentogon.L_EntityPlayer_SetBagOfCraftingSlot(self, slot, pickup)
end

function EntityPlayerFuncs:SetBladderCharge(charge)
    ffichecks.checknumber(2, charge)

    repentogon.L_EntityPlayer_SetBladderCharge(self, charge)
end

function EntityPlayerFuncs:SetBloodCharge(amount)
    ffichecks.checknumber(2, amount)

    repentogon.L_EntityPlayer_SetBloodCharge(self, amount)
end

function EntityPlayerFuncs:SetCanShoot(canShoot)
    ffichecks.checkboolean(2, canShoot)

    repentogon.L_EntityPlayer_SetCanShoot(self, canShoot)
end

function EntityPlayerFuncs:SetCard(slotId, card)
    ffichecks.checknumber(2, slotId)
    ffichecks.checknumber(3, card)

    repentogon.L_EntityPlayer_SetCard(self, slotId, card)
end

function EntityPlayerFuncs:SetControllerIndex(idx)
    ffichecks.checknumber(2, idx)

    repentogon.L_EntityPlayer_SetControllerIndex(self, idx)
end

function EntityPlayerFuncs:SetDamageModifier(modifier)
    ffichecks.checknumber(2, modifier)

    repentogon.L_EntityPlayer_SetDamageModifier(self, modifier)
end

function EntityPlayerFuncs:SetEdenDamage(value)
    ffichecks.checknumber(2, value)

    repentogon.L_EntityPlayer_SetEdenDamage(self, value)
end

function EntityPlayerFuncs:SetEdenFireDelay(value)
    ffichecks.checknumber(2, value)

    repentogon.L_EntityPlayer_SetEdenFireDelay(self, value)
end

function EntityPlayerFuncs:SetEdenLuck(value)
    ffichecks.checknumber(2, value)

    repentogon.L_EntityPlayer_SetEdenLuck(self, value)
end

function EntityPlayerFuncs:SetEdenRange(value)
    ffichecks.checknumber(2, value)

    repentogon.L_EntityPlayer_SetEdenRange(self, value)
end

function EntityPlayerFuncs:SetEdenShotSpeed(value)
    ffichecks.checknumber(2, value)

    repentogon.L_EntityPlayer_SetEdenShotSpeed(self, value)
end

function EntityPlayerFuncs:SetEdenSpeed(value)
    ffichecks.checknumber(2, value)

    repentogon.L_EntityPlayer_SetEdenSpeed(self, value)
end

function EntityPlayerFuncs:SetEveSumptoriumCharge(charge)
    ffichecks.checknumber(2, charge)

    repentogon.L_EntityPlayer_SetEveSumptoriumCharge(self, charge)
end

function EntityPlayerFuncs:SetFireDelayModifier(modifier)
    ffichecks.checknumber(2, modifier)

    repentogon.L_EntityPlayer_SetFireDelayModifier(self, modifier)
end

function EntityPlayerFuncs:SetFootprintColor(color, rightFoot)
    ffichecks.checkcdata(2, color, "Color")
    rightFoot = ffichecks.optboolean(rightFoot, false)

    repentogon.L_EntityPlayer_SetFootprintColor(self, color, rightFoot)
end

EntityPlayerFuncs.SetFullHearts = repentogon.L_EntityPlayer_SetFullHearts

function EntityPlayerFuncs:SetGnawedLeafTimer(timer)
    ffichecks.checknumber(2, timer)

    repentogon.L_EntityPlayer_SetGnawedLeafTimer(self, timer)
end

function EntityPlayerFuncs:SetImmaculateConceptionState(state)
    ffichecks.checknumber(2, state)

    repentogon.L_EntityPlayer_SetImmaculateConceptionState(self, state)
end

function EntityPlayerFuncs:SetItemState(item)
    ffichecks.checknumber(2, item)

    repentogon.L_EntityPlayer_SetItemState(self, item)
end

function EntityPlayerFuncs:SetKeepersSackBonus(bonus)
    ffichecks.checknumber(2, bonus)

    repentogon.L_EntityPlayer_SetKeepersSackBonus(self, bonus)
end

function EntityPlayerFuncs:SetLaserColor(colorMod)
    ffichecks.checkcdata(2, colorMod, "ColorMod")

    repentogon.L_EntityPlayer_SetLaserColor(self, colorMod)
end

function EntityPlayerFuncs:SetLuckModifier(modifier)
    ffichecks.checknumber(2, modifier)

    repentogon.L_EntityPlayer_SetLuckModifier(self, modifier)
end

function EntityPlayerFuncs:SetMaggySwingCooldown(cooldown)
    ffichecks.checknumber(2, cooldown)

    repentogon.L_EntityPlayer_SetMaggySwingCooldown(self, cooldown)
end

function EntityPlayerFuncs:SetMaxBladderCharge(charge)
    ffichecks.checknumber(2, charge)

    repentogon.L_EntityPlayer_SetMaxBladderCharge(self, charge)
end

function EntityPlayerFuncs:SetMegaBlastDuration(duration)
    ffichecks.checknumber(2, duration)

    repentogon.L_EntityPlayer_SetMegaBlastDuration(self, duration)
end

function EntityPlayerFuncs:SetMinDamageCooldown(damageCooldown)
    ffichecks.checknumber(2, damageCooldown)

    repentogon.L_EntityPlayer_SetMinDamageCooldown(self, damageCooldown)
end

function EntityPlayerFuncs:SetNextUrethraBlockFrame(frame)
    ffichecks.checknumber(2, frame)

    repentogon.L_EntityPlayer_SetNextUrethraBlockFrame(self, frame)
end

function EntityPlayerFuncs:SetPill(slotId, pill)
    ffichecks.checknumber(2, slotId)
    ffichecks.checknumber(3, pill)

    repentogon.L_EntityPlayer_SetPill(self, slotId, pill)
end

function EntityPlayerFuncs:SetPocketActiveItem(type, slot, keepInPools)
    ffichecks.checknumber(2, type)
    ffichecks.checknumber(3, slot)
    keepInPools = ffichecks.optboolean(keepInPools, false)

    repentogon.L_EntityPlayer_SetPocketActiveItem(self, type, slot, keepInPools)
end

function EntityPlayerFuncs:SetPonyCharge(time)
    ffichecks.checknumber(2, time)

    repentogon.L_EntityPlayer_SetPonyCharge(self, time)
end

function EntityPlayerFuncs:SetPoopSpell(slot, spell)
    ffichecks.checknumber(2, slot)
    ffichecks.checknumber(3, spell)

    if slot < 0 or slot > 5 then
		error(string.format("Invalid Poop Spell queue position %d", slot))
	end

	if spell < 1 or spell > 11 then
        -- At least until we decide to add custom PoopSpellType support :^)
        error(string.format("Invalid PoopSpellType %d", spell))
    end

    repentogon.L_EntityPlayer_SetPoopSpell(self, slot, spell)
end

function EntityPlayerFuncs:SetPurityState(state)
    ffichecks.checknumber(2, state)

    repentogon.L_EntityPlayer_SetPurityState(self, state)
end

function EntityPlayerFuncs:SetRedStewBonusDuration(duration)
    ffichecks.checknumber(2, duration)

    repentogon.L_EntityPlayer_SetRedStewBonusDuration(self, duration)
end

function EntityPlayerFuncs:SetShootingCooldown(cooldown)
    ffichecks.checknumber(2, cooldown)
    
    repentogon.L_EntityPlayer_SetShootingCooldown(self, cooldown)
end

function EntityPlayerFuncs:SetShotSpeedModifier(modifier)
    ffichecks.checknumber(2, modifier)
    
    repentogon.L_EntityPlayer_SetShotSpeedModifier(self, modifier)
end

function EntityPlayerFuncs:SetSoulCharge(amount)
    ffichecks.checknumber(2, amount)
    
    repentogon.L_EntityPlayer_SetSoulCharge(self, amount)
end

function EntityPlayerFuncs:SetSpeedModifier(modifier)
    ffichecks.checknumber(2, modifier)
    
    repentogon.L_EntityPlayer_SetSpeedModifier(self, modifier)
end

function EntityPlayerFuncs:SetTearPoisonDamage(poisonDamage)
    ffichecks.checknumber(2, poisonDamage)
    
    repentogon.L_EntityPlayer_SetTearPoisonDamage(self, poisonDamage)
end

function EntityPlayerFuncs:SetTearRangeModifier(modifier)
    ffichecks.checknumber(2, modifier)
    
    repentogon.L_EntityPlayer_SetTearRangeModifier(self, modifier)
end

function EntityPlayerFuncs:SetUrethraBlock(blocked)
    ffichecks.checknumber(2, blocked)
    
    repentogon.L_EntityPlayer_SetUrethraBlock(self, blocked)
end

function EntityPlayerFuncs:SetWeapon(weapon, slot)
    ffichecks.checkcdata(2, weapon, "Weapon")
    ffichecks.checknumber(3, slot)

    repentogon.L_EntityPlayer_SetWeapon(self, weapon, slot)
end

function EntityPlayerFuncs:ShootBlueCandle(dir)
    ffichecks.checkcdata(2, dir, "Vector")

    repentogon.L_EntityPlayer_ShootBlueCandle(self, dir)
end

function EntityPlayerFuncs:ShootRedCandle(dir)
    ffichecks.checkcdata(2, dir, "Vector")

    repentogon.L_EntityPlayer_ShootRedCandle(self, dir)
end

function EntityPlayerFuncs:ShuffleCostumes(seed)
    ffichecks.checknumber(2, seed)

    repentogon.L_EntityPlayer_ShuffleCostumes(self, seed)
end

function EntityPlayerFuncs:SpawnAquariusCreep(tearParams)
    return ffichecks.fixreturn(repentogon.L_EntityPlayer_SpawnAquariusCreep(self, tearParams))
end

function EntityPlayerFuncs:SpawnClot(pos, allowPlayerDeath)
    ffichecks.checkcdata(2, pos, "Vector")
    allowPlayerDeath = ffichecks.optboolean(allowPlayerDeath, false)

    repentogon.L_EntityPlayer_SpawnClot(self, pos, allowPlayerDeath)
end

function EntityPlayerFuncs:SpawnMawOfVoid(timeout)
    ffichecks.checknumber(2, timeout)

    repentogon.L_EntityPlayer_SpawnMawOfVoid(self, timeout)
end

EntityPlayerFuncs.SpawnSaturnusTears = repentogon.L_EntityPlayer_SpawnSaturnusTears
EntityPlayerFuncs.StopExtraAnimation = repentogon.L_EntityPlayer_StopExtraAnimation
EntityPlayerFuncs.SwapActiveItems = repentogon.L_EntityPlayer_SwapActiveItems

function EntityPlayerFuncs:SwapForgottenForm(force, noEffects)
    force = ffichecks.optboolean(force, false)
    noEffects = ffichecks.optboolean(noEffects, false)

    repentogon.L_EntityPlayer_SwapForgottenForm(self, force, noEffects)
end

function EntityPlayerFuncs:SyncConsumableCounts(player2, flags)
    ffichecks.checkcdata(2, player2, "Entity_Player")
    ffichecks.checknumber(3, flags)

    repentogon.L_EntityPlayer_SyncConsumableCounts(self, player2, flags)
end

function EntityPlayerFuncs:Teleport(pos, doEffects, teleportTwinPlayers)
    ffichecks.checkcdata(2, pos, "Vector")
    doEffects = ffichecks.optboolean(doEffects, false)
    teleportTwinPlayers = ffichecks.optboolean(teleportTwinPlayers, false)

    repentogon.L_EntityPlayer_Teleport(self, pos, doEffects, teleportTwinPlayers)
end

function EntityPlayerFuncs:ThrowBlueSpider(pos, target)
    ffichecks.checkcdata(2, pos, "Vector")
    ffichecks.checkcdata(3, target, "Vector")

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_ThrowBlueSpider(self, pos, target))
end

function EntityPlayerFuncs:ThrowFriendlyDip(subtype, pos, target)
    ffichecks.checknumber(2, subtype)
    ffichecks.checkcdata(3, pos, "Vector")
    ffichecks.checkcdata(4, target, "Vector")

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_ThrowFriendlyDip(self, subtype, pos, target))
end

function EntityPlayerFuncs:ThrowHeldEntity(vel)
    ffichecks.checkcdata(2, vel, "Vector")

    return ffichecks.fixreturn(repentogon.L_EntityPlayer_ThrowHeldEntity(self, vel))
end

function EntityPlayerFuncs:TriggerBookOfVirtues(itemType, charge)
    itemType = ffichecks.optnumber(itemType, CollectibleType.COLLECTIBLE_NULL)
    charge = ffichecks.optnumber(charge, 0)

    repentogon.L_EntityPlayer_TriggerBookOfVirtues(self, itemType, charge)
end

EntityPlayerFuncs.TriggerRoomClear = repentogon.L_EntityPlayer_TriggerRoomClear

function EntityPlayerFuncs:TryAddToBagOfCrafting(pickup)
    ffichecks.checkcdata(2, pickup, "Entity_Pickup")

    return repentogon.L_EntityPlayer_TryAddToBagOfCrafting(self, pickup)
end

function EntityPlayerFuncs:TryDecreaseGlowingHourglassUses(uses, forceHourglass)
    ffichecks.checknumber(2, uses)
    forceHourglass = ffichecks.optboolean(forceHourglass)
end

EntityPlayerFuncs.TryFakeDeath = repentogon.L_EntityPlayer_TryFakeDeath

function EntityPlayerFuncs:TryForgottenThrow(dir)
    ffichecks.checkcdata(2, dir, "Vector")

    return repentogon.L_EntityPlayer_TryForgottenThrow(self, dir)
end

function EntityPlayerFuncs:TryHoldEntity(entity)
    ffichecks.checkcdata(2, entity, "Entity")

    return repentogon.L_EntityPlayer_TryHoldEntity(self, entity)
end

function EntityPlayerFuncs:TryHoldTrinket(trinketType)
    ffichecks.checkinteger(2, trinketType)

    return repentogon.L_EntityPlayer_TryHoldTrinket(self, trinketType)
end

EntityPlayerFuncs.TryPreventDeath = repentogon.L_EntityPlayer_TryPreventDeath

function EntityPlayerFuncs:TryRemoveCollectibleCostume(collectible, keepPersistent)
    ffichecks.checkinteger(2, collectible)
    ffichecks.checkboolean(3, keepPersistent)

    return repentogon.L_EntityPlayer_TryRemoveCollectibleCostume(self, collectible, keepPersistent)
end

function EntityPlayerFuncs:TryRemoveNullCostume(nullId)
    ffichecks.checkinteger(2, nullId)

    return repentogon.L_EntityPlayer_TryRemoveNullCostume(self, nullId)
end

function EntityPlayerFuncs:TryRemoveTrinket(trinketType)
    ffichecks.checkinteger(2, trinketType)

    return repentogon.L_EntityPlayer_TryRemoveTrinket(self, trinketType)
end

function EntityPlayerFuncs:TryRemoveTrinketCostume(trinketType)
    ffichecks.checkinteger(2, trinketType)

    return repentogon.L_EntityPlayer_TryRemoveTrinketCostume(self, trinketType)
end

function EntityPlayerFuncs:TryRemoveSmeltedTrinket(id)
    ffichecks.checkinteger(2, id)

    return repentogon.L_EntityPlayer_TryRemoveSmeltedTrinket(self, id)
end

EntityPlayerFuncs.TryUseKey = repentogon.L_EntityPlayer_TryUseKey
EntityPlayerFuncs.UpdateCanShoot = repentogon.L_EntityPlayer_UpdateCanShoot

function EntityPlayerFuncs:UpdateIsaacPregnancy(updateCambion)
    ffichecks.checkboolean(2, updateCambion)

    return repentogon.L_EntityPlayer_UpdateIsaacPregnancy(self, updateCambion)
end

-- i hate my life
function EntityPlayerFuncs:UseActiveItem(item, flagsOrShowAnim, slotOrKeepActiveItem, customVarDataOrAllowNonMainPlayer, toAddCostume, slot, customVarData)
    ffichecks.checknumber(2, item)

    if ffichecks.isnumber(flagsOrShowAnim) then
        slotOrKeepActiveItem = ffichecks.optnumber(slotOrKeepActiveItem, -1)
        customVarDataOrAllowNonMainPlayer = ffichecks.optnumber(customVarDataOrAllowNonMainPlayer, 0)

        repentogon.L_EntityPlayer_UseActiveItem(self, item, flagsOrShowAnim, slotOrKeepActiveItem, customVarDataOrAllowNonMainPlayer)
        return
    end

    if ffichecks.isboolean(flagsOrShowAnim) then
        slotOrKeepActiveItem = ffichecks.optboolean(slotOrKeepActiveItem, false)
        customVarDataOrAllowNonMainPlayer = ffichecks.optboolean(customVarDataOrAllowNonMainPlayer, true)
        toAddCostume = ffichecks.optboolean(toAddCostume, false)
        slot = ffichecks.optnumber(slot, -1)
        customVarData = ffichecks.optnumber(customVarData, 0)

        repentogon.L_EntityPlayer_UseActiveItemAbp(self, flagsOrShowAnim, slotOrKeepActiveItem, customVarDataOrAllowNonMainPlayer, toAddCostume, slot, customVarData)
        return
    end

    repentogon.L_EntityPlayer_UseActiveItem(self, item, 0, -1, 0)
end

function EntityPlayerFuncs:UseCard(id, useFlags)
    ffichecks.checknumber(2, id)
    useFlags = ffichecks.optnumber(useFlags, 0)

    repentogon.L_EntityPlayer_UseCard(self, id, useFlags)
end

function EntityPlayerFuncs:UsePill(id, pillColor, useFlags)
    ffichecks.checknumber(2, id)
    ffichecks.checknumber(3, pillColor)
    useFlags = ffichecks.optnumber(useFlags, 0)

    repentogon.L_EntityPlayer_UsePill(self, id, pillColor, useFlags)
end

function EntityPlayerFuncs:UsePoopSpell(spellType)
    ffichecks.checknumber(2, spellType)

    repentogon.L_EntityPlayer_UsePoopSpell(self, spellType)
end

function EntityPlayerFuncs:VoidHasCollectible(itemType)
    ffichecks.checknumber(2, itemType)

    return repentogon.L_EntityPlayer_VoidHasCollectible(self, itemType)
end

EntityPlayerFuncs.WillPlayerRevive = repentogon.L_EntityPlayer_WillPlayerRevive

local function GetCDataFunc(func)
    return function(self)
        return ffichecks.fixreturn(func(self))
    end
end

local function SetTypeFunc(func, typ)
    return function(self, value)
        ffichecks.checktype(2, value, typ)
        func(self, value)
    end
end

local function SetCDataFunc(func, ctype)
    return function(self, value)
        ffichecks.checkcdata(2, value, ctype)
        func(self, value)
    end
end

local getkeys = {
    BabySkin = repentogon.L_EntityPlayer_GetBabySkin,
	CanFly = repentogon.L_EntityPlayer_GetCanFly,
	ControllerIndex = repentogon.L_EntityPlayer_GetControllerIndex,
	ControlsCooldown = repentogon.L_EntityPlayer_GetControlsCooldown,
	ControlsEnabled = repentogon.L_EntityPlayer_GetControlsEnabled,
	Damage = repentogon.L_EntityPlayer_GetDamage,
	FireDelay = repentogon.L_EntityPlayer_GetFireDelay,
	FriendBallEnemy = GetCDataFunc(repentogon.L_EntityPlayer_GetFriendBallEnemy),
	HeadFrameDelay = repentogon.L_EntityPlayer_GetHeadFrameDelay,
	IBSCharge = repentogon.L_EntityPlayer_GetIBSCharge,
	ItemHoldCooldown = repentogon.L_EntityPlayer_GetItemHoldCooldown,
	LaserColor = GetCDataFunc(EntityPlayerFuncs.GetLaserColor),
	Luck = repentogon.L_EntityPlayer_GetLuck,
	MaxFireDelay = repentogon.L_EntityPlayer_GetMaxFireDelay,
	MoveSpeed = repentogon.L_EntityPlayer_GetMoveSpeed,
	QueuedItem = GetCDataFunc(repentogon.L_EntityPlayer_GetQueuedItem),
	SamsonBerserkCharge = repentogon.L_EntityPlayer_GetSamsonBerserkCharge,
	ShotSpeed = repentogon.L_EntityPlayer_GetShotSpeed,
	TearColor = GetCDataFunc(repentogon.L_EntityPlayer_GetTearColor),
	TearFallingAcceleration = repentogon.L_EntityPlayer_GetTearFallingAcceleration,
	TearFallingSpeed = repentogon.L_EntityPlayer_GetTearFallingSpeed,
	TearFlags = GetCDataFunc(repentogon.L_EntityPlayer_GetTearFlags),
	TearHeight = repentogon.L_EntityPlayer_GetTearHeight,
	TearRange = repentogon.L_EntityPlayer_GetTearRange,
	TearsOffset = GetCDataFunc(repentogon.L_EntityPlayer_GetTearsOffset)
}

local setkeys = {
    BabySkin = SetTypeFunc(repentogon.L_EntityPlayer_SetBabySkin, "number"),
	CanFly = SetTypeFunc(repentogon.L_EntityPlayer_SetCanFly, "boolean"),
	ControllerIndex = SetTypeFunc(repentogon.L_EntityPlayer_SetControllerIndex, "number"),
	ControlsCooldown = SetTypeFunc(repentogon.L_EntityPlayer_SetControlsCooldown, "number"),
	ControlsEnabled = SetTypeFunc(repentogon.L_EntityPlayer_SetControlsEnabled, "boolean"),
	Damage = SetTypeFunc(repentogon.L_EntityPlayer_SetDamage, "number"),
	FireDelay = SetTypeFunc(repentogon.L_EntityPlayer_SetFireDelay, "number"),
	FriendBallEnemy = SetCDataFunc(repentogon.L_EntityPlayer_SetFriendBallEnemy, "EntityDesc"),
	HeadFrameDelay = SetTypeFunc(repentogon.L_EntityPlayer_SetHeadFrameDelay, "number"),
	IBSCharge = SetTypeFunc(repentogon.L_EntityPlayer_SetIBSCharge, "number"),
	ItemHoldCooldown = SetTypeFunc(repentogon.L_EntityPlayer_SetItemHoldCooldown, "number"),
	LaserColor = SetCDataFunc(repentogon.L_EntityPlayer_SetLaserColor, "ColorMod"),
	Luck = SetTypeFunc(repentogon.L_EntityPlayer_SetLuck, "number"),
	MaxFireDelay = SetTypeFunc(repentogon.L_EntityPlayer_SetMaxFireDelay, "number"),
	MoveSpeed = SetTypeFunc(repentogon.L_EntityPlayer_SetMoveSpeed, "number"),
	QueuedItem = SetCDataFunc(repentogon.L_EntityPlayer_SetQueuedItem, "QueueItemData"),
	SamsonBerserkCharge = SetTypeFunc(repentogon.L_EntityPlayer_SetSamsonBerserkCharge, "number"),
	ShotSpeed = SetTypeFunc(repentogon.L_EntityPlayer_SetShotSpeed, "number"),
	TearColor = SetCDataFunc(repentogon.L_EntityPlayer_SetTearColor, "ColorMod"),
	TearFallingAcceleration = SetTypeFunc(repentogon.L_EntityPlayer_SetTearFallingAcceleration, "number"),
	TearFallingSpeed = SetTypeFunc(repentogon.L_EntityPlayer_SetTearFallingSpeed, "number"),
	TearFlags = SetCDataFunc(repentogon.L_EntityPlayer_SetTearFlags, "BitSet128"),
	TearHeight = SetTypeFunc(repentogon.L_EntityPlayer_SetTearHeight, "number"),
	TearRange = SetTypeFunc(repentogon.L_EntityPlayer_SetTearRange, "number"),
	TearsOffset = SetCDataFunc(repentogon.L_EntityPlayer_SetTearsOffset, "Vector")
}

local EntityPlayerMT = lffi.metatype("Entity_Player", {
    __tostring = function(self) return string.format("EntityPlayer(%s)", self:GetName()) end,
    __index = function(self, key) 
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        elseif Entity_getkeys[key] ~= nil then
            return Entity_getkeys[key](lffi.cast("Entity*", self))
        elseif EntityPlayerFuncs[key] ~= nil then
			return EntityPlayerFuncs[key]
		elseif EntityFuncs[key] ~= nil then
			return EntityFuncs[key]
		else
			error(string.format("attempted to access nil value '%s'", key))
		end
    end,
    __newindex = function(self, key, value)
        if setkeys[key] ~= nil then
            return setkeys[key](self, value)
        else
            error(string.format("no writable variable '%s'", key))
        end
    end
})