local Entity = require("ffi.Entity.Entity")

ffi.cdef [[
    struct SmeltedTrinketDesc {
        short TrinketAmount;
        short GoldenTrinketAmount;
    } : 0x4;
]]

ffi.cdef("struct EntityPlayer { " .. Entity.Fields .. [[
    private bool ControlsEnabledValue : 0x410;
    int ControlsCooldown : 0x414;
    private struct CostumeSpriteDesc* CostumeSpriteDescsFirstValue : 0x1220;
    private struct CostumeSpriteDesc* CostumeSpriteDescsLastValue : 0x1224;
    int HeadFrameDelay : 0x1338;
    private int HeadDirectionTimeValue : 0x133c;
    private int MaxHeartsValue : 0x1340;
    private int RedHeartsValue : 0x1344;
    private int EternalHeartsValue : 0x1348;
    private int SoulHeartsValue : 0x134c;
    private int BlackHeartsValue : 0x1350;
    private int JarHeartsValue : 0x1354;
    private int JarFliesValue : 0x1358;
    private int NumKeysValue : 0x135c;
    private bool HasGoldenKeyValue : 0x1360;
    private bool HasGoldenBombValue : 0x1361;
    private int NumBombsValue : 0x1364;
    private int NumCoinsValue : 0x1368;
    private int NumBlueFliesValue : 0x136c;
    private int NumBlueSpidersValue : 0x1370;
    private int GnawedLeafTimerValue : 0x1374;
    private uint32_t LastActionTriggersValue : 0x1378;
    private short BloodLustCounterValue : 0x137c;
    private int MetronomeCollectibleIDValue : 0x1388;
    private bool ExtraAnimationAValue : 0x1398;
    private bool ExtraAnimationBValue : 0x139a;
    private int DamageCooldownValue : 0x13b8;
    private int PlayerTypeValue : 0x13bc;
    private struct StdString NameValue : 0x13c0;
    private struct EntityEffect* MarkedTargetValue : 0x1408;
    private struct Entity* HeldEntityValue : 0x140c;
    private struct Entity* TractorBeamValue : 0x142c;
    private struct EntityLaser* MegaBlastLaserValue : 0x1454;
    private int RUAWizardTimerValue : 0x1458;
    float MaxFireDelay : 0x145c;
    float ShotSpeed : 0x1460;
    float Damage : 0x146c;
    float TearHeight : 0x1470;
    float TearFallingSpeed : 0x1474;
    float TearFallingAcceleration : 0x1478;
    float TearRange : 0x147c;
    private struct BitSet128 TearFlagsValue : 0x1480;
    private struct Color TearColorValue : 0x1490;
    private struct Color LaserColorValue : 0x14bc;
    private struct Vector PlayerSpriteScaleValue : 0x14e8;
    private int SpeedModifierValue : 0x1518;
    private int FireDelayModifierValue : 0x151c;
    private int DamageModifierValue : 0x1520;
    private int TearRangeModifierValue : 0x1524;
    private int ShotSpeedModifierValue : 0x1528;
    private int LuckModifierValue : 0x152c;
    private int DonateLuckValue : 0x1530;
    private int PotatoPeelerCounterValue : 0x1534;
    private float EdenSpeedValue : 0x1540;
    private float EdenFireDelayValue : 0x1544;
    private float EdenDamageValue : 0x1548;
    private float EdenTearRangeValue : 0x154c;
    private float EdenShotSpeedValue : 0x1550;
    private float EdenLuckValue : 0x1554;
    float MoveSpeed : 0x155c;
    float Luck : 0x1560;
    private bool CanFlyValue : 0x1564;
    private uint32_t CacheFlagsValue : 0x1568;
    private int8_t TearDisplacementValue : 0x156c;
    private float TearPoisonDamageValue : 0x1570;
    private int ItemStateValue : 0x15f4;
    private int ItemStateCooldownValue : 0x15f8;
    private uint32_t VampireCharmKillsValue : 0x15fc;
    private int BombPlaceDelayValue : 0x1604;
    private int ForgottenSwapFormCooldownValue : 0x1608;
    private int ControllerIndexValue : 0x160c;
    private int PlayerIndexValue : 0x1610;
    private int MoveDirectionValue : 0x1618;
    private int HeadDirectionValue : 0x161c;
    private int FireDirectionValue : 0x1620;
    private struct Vector AimDirectionValue : 0x1624;
    private bool OpposingShootDirectionsValue : 0x162c;
    private struct Vector LastDirectionValue : 0x1630;
    private struct Vector MovementInputValue : 0x1638;
    private struct Vector TearsOffsetValue : 0x1640;
    private struct Vector RecentMovementVectorValue : 0x1648;
    private struct Vector VelocityBeforeUpdateValue : 0x1654;
    private uint64_t LastDamageFlagsValue : 0x1698;
    private int TotalDamageTakenValue : 0x16a4;
    private uint32_t Trinket0Value : 0x16b0;
    private uint32_t Trinket1Value : 0x16b4;
    private int* CollectibleCountsFirstValue : 0x16b8;
    private int* CollectibleCountsLastValue : 0x16bc;
    private struct SmeltedTrinketDesc* SmeltedTrinketsFirstValue : 0x1738;
    private struct SmeltedTrinketDesc* SmeltedTrinketsLastValue : 0x173c;
    private int* VoidedCollectiblesFirstValue : 0x1744;
    private int* VoidedCollectiblesLastValue : 0x1748;
    private uint32_t ActionHoldDropValue : 0x17b0;
    private struct QueueItemData QueuedItemValue : 0x17b4;
    private bool PostLevelInitFinishedValue : 0x1810;
    private bool CanShootValue : 0x1811;
    int ItemHoldCooldown : 0x1870;
    private int PonyChargeValue : 0x1874;
    private int PlanCKillCountdownValue : 0x1878;
    private int HeadColorValue : 0x1880;
    private int BodyColorValue : 0x1884;
    private int EpiphoraChargeValue : 0x1890;
    private uint32_t DeadEyeChargesValue : 0x189c;
    private uint32_t DeadEyeMissesValue : 0x18a0;
    private uint32_t MawOfTheVoidChargeTimerValue : 0x18a8;
    private uint32_t ImExcitedSpeedupCountdownValue : 0x18d4;
    private int MegaBlastDurationValue : 0x18e0;
    private int PeeBurstCooldownValue : 0x18e4;
    private int MaxPeeBurstCooldownValue : 0x18e8;
    private int BladderChargeValue : 0x18ec;
    private int MaxBladderChargeValue : 0x18f0;
    private bool IsUrethraBlockedValue : 0x18f4;
    private int NextUrethraBlockFrameValue : 0x18f8;
    private struct EntityDesc FriendBallEnemyValue : 0x18fc;
    private int PurityStateValue : 0x1918;
    private int ImmaculateConceptionStateValue : 0x191c;
    private int CambionConceptionStateValue : 0x1920;
    private uint32_t ConceptionFamiliarFlagsValue : 0x1924;
    private float D8DamageModifierValue : 0x1928;
    private float D8SpeedModifierValue : 0x192c;
    private float D8RangeModifierValue : 0x1930;
    private float D8FireDelayModifierValue : 0x1934;
    private int GoldenHeartsValue : 0x1938;
    private float SmoothBodyRotationValue : 0x1ad0;
    int BabySkin : 0x1ad8;
    private int BoneHeartsValue : 0x1d74;
    private int HallowedGroundCountdownValue : 0x1d7c;
    private struct EntityPlayer* SubPlayerValue : 0x1d84;
    private int BrokenHeartsValue : 0x1d8c;
    private int RottenHeartsValue : 0x1d90;
    private int SoulChargeValue : 0x1d94;
    private int BloodChargeValue : 0x1d98;
    private uint32_t UrnSoulsValue : 0x1dbc;
    private int NumGigaBombsValue : 0x1dc0;
    private struct EntityPlayer* TwinPlayerValue : 0x1e4c;
    private struct EntityPlayer* BackupPlayerValue : 0x1e50;
    private int RedStewBonusDurationValue : 0x1e58;
    private float RockBottomMoveSpeedValue : 0x1e94;
    private float RockBottomMaxFireDelayValue : 0x1e98;
    private float RockBottomDamageValue : 0x1e9c;
    private float RockBottomTearRangeValue : 0x1ea0;
    private float RockBottomShotSpeedValue : 0x1ea4;
    private float RockBottomLuckValue : 0x1ea8;
    private uint32_t RevelationChargeTimerValue : 0x1ec0;
    private uint32_t MontezumaChargeTimerValue : 0x1ec4;
    private uint32_t MaggyHealthDrainCooldownValue : 0x1ed0;
    private int MaggySwingCooldownValue : 0x1ed4;
    private int SuplexStateValue : 0x1f08;
    private int SuplexAimCountdownValue : 0x1f0c;
    private struct Vector SuplexTargetPosValue : 0x1f10;
    private struct Vector SuplexLandPosValue : 0x1f18;
    private int PoopManaValue : 0x1f30;
    private int EveSumptoriumChargeValue : 0x1f58;
    private int ModelingClayEffectValue : 0x1f94;
    private int KeepersSackBonusValue : 0x1f98;
    private bool CurseMistEffectValue : 0x2010;
    private uint8_t WildCardItemTypeValue : 0x2064;
    private int WildCardItemValue : 0x2068;
    int SamsonBerserkCharge : 0x2070;
    float IBSCharge : 0x2074;
    private bool IsCoopGhostValue : 0x207d;
    private struct PlayerHUD* PlayerHUDValue : 0x20a8;
]] .. "} : 0x2ea0;\ntypedef struct EntityPlayer* EntityPlayerPtr;")

ffi.cdef [[
    void L_EntityPlayer_AddMaxHearts(struct EntityPlayer*, int, bool);
    bool L_EntityPlayer_HasFullHearts(struct EntityPlayer*);
    void L_EntityPlayer_AddHearts(struct EntityPlayer*, int, bool, bool);
    void L_EntityPlayer_AddEternalHearts(struct EntityPlayer*, int);
    void L_EntityPlayer_AddSoulHearts(struct EntityPlayer*, int, bool);
    void L_EntityPlayer_AddBlackHearts(struct EntityPlayer*, int);
    void L_EntityPlayer_RemoveBlackHeart(struct EntityPlayer*, int);
    bool L_EntityPlayer_IsBlackHeart(struct EntityPlayer*, int);
    void L_EntityPlayer_AddJarHearts(struct EntityPlayer*, int);
    void L_EntityPlayer_AddJarFlies(struct EntityPlayer*, int);
    void L_EntityPlayer_AddCoins(struct EntityPlayer*, int);
    void L_EntityPlayer_AddBombs(struct EntityPlayer*, int);
    void L_EntityPlayer_AddKeys(struct EntityPlayer*, int);
    void L_EntityPlayer_AddGoldenKey(struct EntityPlayer*);
    void L_EntityPlayer_RemoveGoldenKey(struct EntityPlayer*);
    void L_EntityPlayer_AddGoldenBomb(struct EntityPlayer*);
    void L_EntityPlayer_RemoveGoldenBomb(struct EntityPlayer*);
    void L_EntityPlayer_AddGoldenHearts(struct EntityPlayer*, int, bool);
    void L_EntityPlayer_AddPrettyFly(struct EntityPlayer*);
    bool L_EntityPlayer_TryUseKey(struct EntityPlayer*);
    void L_EntityPlayer_AddBoneHearts(struct EntityPlayer*, int);
    void L_EntityPlayer_AddBrokenHearts(struct EntityPlayer*, int);
    void L_EntityPlayer_AddRottenHearts(struct EntityPlayer*, int, bool);
    void L_EntityPlayer_AddGigaBombs(struct EntityPlayer*, int);
    void L_EntityPlayer_AddSoulCharge(struct EntityPlayer*, int);
    void L_EntityPlayer_AddBloodCharge(struct EntityPlayer*, int);
    int L_EntityPlayer_GetEffectiveSoulCharge(struct EntityPlayer*);
    int L_EntityPlayer_GetEffectiveBloodCharge(struct EntityPlayer*);
    int L_EntityPlayer_GetEffectiveMaxHearts(struct EntityPlayer*);
    int L_EntityPlayer_GetHeartLimit(struct EntityPlayer*, bool);
    void L_EntityPlayer_SetFullHearts(struct EntityPlayer*);
    bool L_EntityPlayer_IsBoneHeart(struct EntityPlayer*, int);
    bool L_EntityPlayer_CanPickRedHearts(struct EntityPlayer*);
    bool L_EntityPlayer_CanPickSoulHearts(struct EntityPlayer*);
    bool L_EntityPlayer_CanPickBlackHearts(struct EntityPlayer*);
    bool L_EntityPlayer_CanPickGoldenHearts(struct EntityPlayer*);
    bool L_EntityPlayer_CanPickBoneHearts(struct EntityPlayer*);
    bool L_EntityPlayer_CanPickRottenHearts(struct EntityPlayer*);
    void L_EntityPlayer_ChangePlayerType(struct EntityPlayer*, int, bool);
    int L_EntityPlayer_GetExtraLives(struct EntityPlayer*);
    bool L_EntityPlayer_WillPlayerRevive(struct EntityPlayer*);
    void L_EntityPlayer_Revive(struct EntityPlayer*);
    void L_EntityPlayer_DonateLuck(struct EntityPlayer*, int);
    void L_EntityPlayer_AddCard(struct EntityPlayer*, int);
    void L_EntityPlayer_AddPill(struct EntityPlayer*, int);
    int L_EntityPlayer_GetCard(struct EntityPlayer*, int);
    int L_EntityPlayer_GetPill(struct EntityPlayer*, int);
    void L_EntityPlayer_SetCard(struct EntityPlayer*, int, int);
    void L_EntityPlayer_SetPill(struct EntityPlayer*, int, int);
    bool L_EntityPlayer_FlushQueueItem(struct EntityPlayer*);
    int L_EntityPlayer_GetCollectibleCount(struct EntityPlayer*);
    void L_EntityPlayer_AddTrinket(struct EntityPlayer*, int, bool);
    bool L_EntityPlayer_TryRemoveTrinket(struct EntityPlayer*, int);
    int L_EntityPlayer_GetMaxTrinkets(struct EntityPlayer*);
    void L_EntityPlayer_RemoveCollectible(struct EntityPlayer*, int, bool, int, bool);
    void L_EntityPlayer_ClearTemporaryEffects(struct EntityPlayer*);
    bool L_EntityPlayer_HasPlayerForm(struct EntityPlayer*, int);
    bool L_EntityPlayer_CanAddCollectible(struct EntityPlayer*, int);
    bool L_EntityPlayer_TryHoldTrinket(struct EntityPlayer*, int);
    void L_EntityPlayer_EvaluateItems(struct EntityPlayer*);
    void L_EntityPlayer_RespawnFamiliars(struct EntityPlayer*);
    bool L_EntityPlayer_HasWeaponType(struct EntityPlayer*, int);
    void L_EntityPlayer_TryRemoveCollectibleCostume(struct EntityPlayer*, int, bool);
    void L_EntityPlayer_TryRemoveTrinketCostume(struct EntityPlayer*, int);
    void L_EntityPlayer_TryRemoveNullCostume(struct EntityPlayer*, int);
    void L_EntityPlayer_RemoveSkinCostume(struct EntityPlayer*);
    void L_EntityPlayer_ClearCostumes(struct EntityPlayer*);
    void L_EntityPlayer_AddPlayerFormCostume(struct EntityPlayer*, int);
    void L_EntityPlayer_UseCard(struct EntityPlayer*, int, unsigned int);
    void L_EntityPlayer_UsePill(struct EntityPlayer*, int, int, unsigned int);
    void L_EntityPlayer_TriggerBookOfVirtues(struct EntityPlayer*, int, int);
    void L_EntityPlayer_SwapActiveItems(struct EntityPlayer*);
    void L_EntityPlayer_ResetItemState(struct EntityPlayer*);
    bool L_EntityPlayer_HasTimedItem(struct EntityPlayer*);
    void L_EntityPlayer_AddDollarBillEffect(struct EntityPlayer*);
    void L_EntityPlayer_AddCurseMistEffect(struct EntityPlayer*);
    void L_EntityPlayer_RemoveCurseMistEffect(struct EntityPlayer*);
    void L_EntityPlayer_InitBabySkin(struct EntityPlayer*);
    void L_EntityPlayer_UpdateCanShoot(struct EntityPlayer*);
    void L_EntityPlayer_AddDeadEyeCharge(struct EntityPlayer*);
    int L_EntityPlayer_GetZodiacEffect(struct EntityPlayer*);
    float L_EntityPlayer_GetGreedDonationBreakChance(struct EntityPlayer*);
    bool L_EntityPlayer_IsFullSpriteRendering(struct EntityPlayer*);
    void L_EntityPlayer_UsePoopSpell(struct EntityPlayer*, int);
    int L_EntityPlayer_GetMaxPoopMana(struct EntityPlayer*);
    void L_EntityPlayer_AddPoopMana(struct EntityPlayer*, int);
    int L_EntityPlayer_GetPoopSpell(struct EntityPlayer*, int);
    struct Entity* L_EntityPlayer_GetNPCTarget(struct EntityPlayer*);
    struct Entity* L_EntityPlayer_GetActiveWeaponEntity(struct EntityPlayer*);
    struct EntityPlayer* L_EntityPlayer_GetMainTwin(struct EntityPlayer*);
    bool L_EntityPlayer_CanPickupItem(struct EntityPlayer*);
    bool L_EntityPlayer_IsHoldingItem(struct EntityPlayer*);
    bool L_EntityPlayer_IsHeldItemVisible(struct EntityPlayer*);
    bool L_EntityPlayer_TryHoldEntity(struct EntityPlayer*, void*);
    void L_EntityPlayer_SetShootingCooldown(struct EntityPlayer*, int);
    void L_EntityPlayer_SetMinDamageCooldown(struct EntityPlayer*, int);
    bool L_EntityPlayer_AreControlsEnabled(struct EntityPlayer*);
    void L_EntityPlayer_AnimateCollectible(struct EntityPlayer*, int, const char*, const char*);
    void L_EntityPlayer_AnimateTrinket(struct EntityPlayer*, int, const char*, const char*);
    void L_EntityPlayer_AnimateCard(struct EntityPlayer*, int, const char*);
    void L_EntityPlayer_AnimatePill(struct EntityPlayer*, int, const char*);
    void L_EntityPlayer_AnimateTrapdoor(struct EntityPlayer*);
    void L_EntityPlayer_AnimateLightTravel(struct EntityPlayer*);
    void L_EntityPlayer_AnimateAppear(struct EntityPlayer*);
    void L_EntityPlayer_AnimateTeleport(struct EntityPlayer*, bool);
    void L_EntityPlayer_AnimateHappy(struct EntityPlayer*);
    void L_EntityPlayer_AnimateSad(struct EntityPlayer*);
    void L_EntityPlayer_AnimatePitfallIn(struct EntityPlayer*, bool);
    void L_EntityPlayer_AnimatePitfallOut(struct EntityPlayer*);
    void L_EntityPlayer_PlayExtraAnimation(struct EntityPlayer*, const char*);
    void L_EntityPlayer_QueueExtraAnimation(struct EntityPlayer*, const char*);
    struct Entity* L_EntityPlayer_AddBlueFlies(struct EntityPlayer*, int, struct Vector*, void*);
    struct Entity* L_EntityPlayer_AddBlueSpider(struct EntityPlayer*, struct Vector*);
    struct EntityFamiliar* L_EntityPlayer_AddFriendlyDip(struct EntityPlayer*, int, struct Vector*);
    struct EntityFamiliar* L_EntityPlayer_AddItemWisp(struct EntityPlayer*, int, struct Vector*, bool);
    struct EntityFamiliar* L_EntityPlayer_AddMinisaac(struct EntityPlayer*, struct Vector*, bool);
    struct EntityFamiliar* L_EntityPlayer_AddSwarmFlyOrbital(struct EntityPlayer*, struct Vector*);
    struct EntityFamiliar* L_EntityPlayer_AddWisp(struct EntityPlayer*, int, struct Vector*, bool, bool);
    void L_EntityPlayer_DoZitEffect(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_DropPocketItem(struct EntityPlayer*, int, struct Vector*);
    void L_EntityPlayer_DropTrinket(struct EntityPlayer*, struct Vector*, bool);
    struct EntityBomb* L_EntityPlayer_FireBomb(struct EntityPlayer*, struct Vector*, struct Vector*, void*);
    struct EntityKnife* L_EntityPlayer_FireKnife(struct EntityPlayer*, void*, float, bool, int, int);
    struct EntityLaser* L_EntityPlayer_FireDelayedBrimstone(struct EntityPlayer*, float, void*);
    struct EntityLaser* L_EntityPlayer_SpawnMawOfVoid(struct EntityPlayer*, int);
    struct EntityLaser* L_EntityPlayer_FireBrimstone(struct EntityPlayer*, struct Vector*, void*, float);
    struct EntityLaser* L_EntityPlayer_FireTechLaser(struct EntityPlayer*, struct Vector*, int, struct Vector*, bool, bool, void*, float);
    struct EntityLaser* L_EntityPlayer_FireTechXLaser(struct EntityPlayer*, struct Vector*, struct Vector*, float, void*, float);
    struct Entity* L_EntityPlayer_ThrowBlueSpider(struct EntityPlayer*, struct Vector*, struct Vector*);
    struct Entity* L_EntityPlayer_ThrowHeldEntity(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_GetFlyingOffset(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_GetLaserOffset(struct EntityPlayer*, int, struct Vector*, struct Vector*);
    void L_EntityPlayer_GetMovementJoystick(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_GetShootingJoystick(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_GetTearMovementInheritance(struct EntityPlayer*, struct Vector*, struct Vector*);
    void L_EntityPlayer_GetBodyMoveDirection(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_GetEnterPosition(struct EntityPlayer*, struct Vector*);
    bool L_EntityPlayer_IsPosInSpotLight(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_RenderBody(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_RenderGlow(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_RenderHead(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_RenderTop(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_Teleport(struct EntityPlayer*, struct Vector*, bool, bool);
    void L_EntityPlayer_SpawnClot(struct EntityPlayer*, struct Vector*, bool);
    bool L_EntityPlayer_TryForgottenThrow(struct EntityPlayer*, struct Vector*);
    void L_EntityPlayer_AddCollectible(struct EntityPlayer*, int, int, bool, int, int, int);
    void L_EntityPlayer_AddCostume(struct EntityPlayer*, struct ItemConfigItem*, bool);
    void L_EntityPlayer_CheckFamiliar(struct EntityPlayer*, unsigned int, unsigned int, struct RNG*, struct ItemConfigItem*, int);
    void L_EntityPlayer_RemoveCostume(struct EntityPlayer*, struct ItemConfigItem*);
    struct EntityPlayer* L_EntityPlayer_InitTwin(struct EntityPlayer*, int);
    void L_EntityPlayer_InitPostLevelInitStats(struct EntityPlayer*);
    void L_EntityPlayer_SetItemState(struct EntityPlayer*, int);
    int L_EntityPlayer_GetHealthType(struct EntityPlayer*);
    int L_EntityPlayer_GetTotalActiveCharge(struct EntityPlayer*, int);
    int L_EntityPlayer_GetActiveMaxCharge(struct EntityPlayer*, int);
    int L_EntityPlayer_GetActiveMinUsableCharge(struct EntityPlayer*, int);
    void L_EntityPlayer_SetActiveVarData(struct EntityPlayer*, int, int);
    int L_EntityPlayer_GetActiveItem(struct EntityPlayer*, int);
    int L_EntityPlayer_GetActiveCharge(struct EntityPlayer*, int);
    int L_EntityPlayer_GetActiveSubCharge(struct EntityPlayer*, int);
    int L_EntityPlayer_GetBatteryCharge(struct EntityPlayer*, int);
    bool L_EntityPlayer_NeedsCharge(struct EntityPlayer*, int);
    void L_EntityPlayer_SetActiveCharge(struct EntityPlayer*, int, int);
    int L_EntityPlayer_AddActiveCharge(struct EntityPlayer*, int, int, bool, bool, bool);
    bool L_EntityPlayer_FullCharge(struct EntityPlayer*, int, bool);
    void L_EntityPlayer_DischargeActiveItem(struct EntityPlayer*, int);
    bool L_EntityPlayer_CanOverrideActiveItem(struct EntityPlayer*, int);
    int L_EntityPlayer_GetActiveItemSlot(struct EntityPlayer*, int);
    void L_EntityPlayer_IncrementPlayerFormCounter(struct EntityPlayer*, int, int);
    bool L_EntityPlayer_TryPreventDeath(struct EntityPlayer*);
    void L_EntityPlayer_RemoveCollectibleByHistoryIndex(struct EntityPlayer*, int);
    bool L_EntityPlayer_TryFakeDeath(struct EntityPlayer*);
    int L_EntityPlayer_GetWeaponModifiers(struct EntityPlayer*);
    void L_EntityPlayer_EnableWeaponType(struct EntityPlayer*, int, bool);
    void L_EntityPlayer_TriggerRoomClear(struct EntityPlayer*);
    void L_EntityPlayer_UpdateIsaacPregnancy(struct EntityPlayer*, bool);
    int L_EntityPlayer_GetCambionPregnancyLevel(struct EntityPlayer*);
    bool L_EntityPlayer_SwapForgottenForm(struct EntityPlayer*, bool, bool);
    void L_EntityPlayer_PlayDelayedSFX(struct EntityPlayer*, unsigned int, int, int, float);
    bool L_EntityPlayer_CanUsePill(struct EntityPlayer*, int);
    unsigned int L_EntityPlayer_GetMaxPocketItems(struct EntityPlayer*);
    bool L_EntityPlayer_CanAddCollectibleToInventory(struct EntityPlayer*, int);
    void L_EntityPlayer_AddLeprosy(struct EntityPlayer*);
    void L_EntityPlayer_AddUrnSouls(struct EntityPlayer*, unsigned int);
    const char* L_EntityPlayer_GetDeathAnimName(struct EntityPlayer*);
    unsigned int L_EntityPlayer_GetGlitchBabySubType(struct EntityPlayer*);
    int L_EntityPlayer_GetGreedsGulletHearts(struct EntityPlayer*);
    bool L_EntityPlayer_CanCrushRocks(struct EntityPlayer*);
    bool L_EntityPlayer_HasInstantDeathCurse(struct EntityPlayer*);
    bool L_EntityPlayer_HasPoisonImmunity(struct EntityPlayer*);
    bool L_EntityPlayer_IsEntityValidTarget(struct EntityPlayer*, void*);
    bool L_EntityPlayer_IsHeadless(struct EntityPlayer*);
    bool L_EntityPlayer_IsHologram(struct EntityPlayer*);
    bool L_EntityPlayer_IsInvisible(struct EntityPlayer*);
    void L_EntityPlayer_MorphToCoopGhost(struct EntityPlayer*);
    void L_EntityPlayer_ResetPlayer(struct EntityPlayer*);
    void L_EntityPlayer_SetControllerIndex(struct EntityPlayer*, int, bool);
    void L_EntityPlayer_SyncConsumableCounts(struct EntityPlayer*, void*, int);
    bool L_EntityPlayer_TryAddToBagOfCrafting(struct EntityPlayer*, void*);
    void L_EntityPlayer_TryDecreaseGlowingHourglassUses(struct EntityPlayer*, int, bool);
    void L_EntityPlayer_TryRemoveSmeltedTrinket(struct EntityPlayer*, unsigned int);
    bool L_EntityPlayer_VoidHasCollectible(struct EntityPlayer*, int);
    bool L_EntityPlayer_PlayItemNullAnimation(struct EntityPlayer*, const char*);
    void L_EntityPlayer_ClearQueueItem(struct EntityPlayer*);
    void L_EntityPlayer_RemovePocketItem(struct EntityPlayer*, int);
    bool L_EntityPlayer_IsFootstepFrame(struct EntityPlayer*, int);
    struct PocketItem* L_EntityPlayer_GetPocketItem(struct EntityPlayer*, int);
    struct History* L_EntityPlayer_GetHistory(struct EntityPlayer*);
    struct RNG* L_EntityPlayer_GetCardRNG(struct EntityPlayer*, int);
    struct RNG* L_EntityPlayer_GetCollectibleRNG(struct EntityPlayer*, int);
    struct RNG* L_EntityPlayer_GetPillRNG(struct EntityPlayer*, int);
    struct RNG* L_EntityPlayer_GetTrinketRNG(struct EntityPlayer*, int);
    void L_EntityPlayer_GetBombFlags(struct EntityPlayer*, bool, struct BitSet128*);
    struct ActiveItemDesc* L_EntityPlayer_GetActiveItemDesc(struct EntityPlayer*, int);
    struct Entity* L_EntityPlayer_GetFocusEntity(struct EntityPlayer*);
    unsigned int L_EntityPlayer_SpawnSaturnusTears(struct EntityPlayer*);
    void L_EntityPlayer_SetPocketActiveItem(struct EntityPlayer*, int, int, bool);
    void L_EntityPlayer_QueueItemEx(struct EntityPlayer*, struct ItemConfigItem*, int, int, int);
    struct EntityTear* L_EntityPlayer_FireTearEx(struct EntityPlayer*, struct Vector*, struct Vector*, int, void*, float);
    void L_EntityPlayer_ClearDeadEyeChargeNative(struct EntityPlayer*);
    bool L_EntityPlayer_IsItemCostumeVisibleEx(struct EntityPlayer*, struct ItemConfigItem*, int);
    bool L_EntityPlayer_IsCollectibleCostumeVisibleEx(struct EntityPlayer*, int, int);
    bool L_EntityPlayer_IsNullItemCostumeVisibleEx(struct EntityPlayer*, int, int);
    float L_EntityPlayer_GetFireDelayNative(struct EntityPlayer*);
    void L_EntityPlayer_SetFireDelayNative(struct EntityPlayer*, float);

    struct EntityConfigPlayer* L_EntityPlayer_GetEntityConfigPlayer(struct EntityPlayer*);
    struct Weapon* L_EntityPlayer_GetWeapon(struct EntityPlayer*, int);
    void L_EntityPlayer_SetWeapon(struct EntityPlayer*, struct Weapon*, int);
    bool L_EntityPlayer_GetActiveWeaponNumFired(struct EntityPlayer*, int*);
    struct TemporaryEffects* L_EntityPlayer_GetEffects(struct EntityPlayer*);
    struct Sprite* L_EntityPlayer_GetBodySprite(struct EntityPlayer*);
    struct Sprite* L_EntityPlayer_GetBloodGushSprite(struct EntityPlayer*);
    struct Sprite* L_EntityPlayer_GetHeldSprite(struct EntityPlayer*);
    struct EntityRef* L_EntityPlayer_GetLastDamageSource(struct EntityPlayer*);
    struct EntitiesSaveStateVector* L_EntityPlayer_GetMovingBoxContents(struct EntityPlayer*);
    int L_EntityPlayer_GetPlayerFormCounter(struct EntityPlayer*, int);
    void L_EntityPlayer_GetCostumeLayer(struct EntityPlayer*, int, int*, int*, int*, bool*);
    int L_EntityPlayer_GetHeartStatUp(struct EntityPlayer*, bool, int);
    int L_EntityPlayer_GetInventoryHistoryIndex(struct EntityPlayer*, int);
    int L_EntityPlayer_GetInventoryCollectible(struct EntityPlayer*, int);
    int L_EntityPlayer_GetMaxInventorySize(struct EntityPlayer*);
    struct PlayerHUD* L_EntityPlayer_GetPlayerHUD(struct EntityPlayer*);

    bool L_EntityPlayer_HasCollectible(struct EntityPlayer*, int, bool, bool);
    int L_EntityPlayer_GetCollectibleNum(struct EntityPlayer*, int, bool, bool);
    bool L_EntityPlayer_HasTrinket(struct EntityPlayer*, unsigned int, bool, bool);
    int L_EntityPlayer_GetTrinketMultiplier(struct EntityPlayer*, unsigned int, bool);
    bool L_EntityPlayer_HasGoldenTrinket(struct EntityPlayer*, unsigned int, bool);
    bool L_EntityPlayer_BlockCollectible(struct EntityPlayer*, int);
    bool L_EntityPlayer_UnblockCollectible(struct EntityPlayer*, int);
    bool L_EntityPlayer_IsCollectibleBlocked(struct EntityPlayer*, int);
    bool L_EntityPlayer_BlockTrinket(struct EntityPlayer*, int);
    bool L_EntityPlayer_UnblockTrinket(struct EntityPlayer*, int);
    bool L_EntityPlayer_IsTrinketBlocked(struct EntityPlayer*, int);
    bool L_EntityPlayer_IsValidInnateItem(bool, int);
    void L_EntityPlayer_AddInnateItem(struct EntityPlayer*, bool, int, int, const char*, int, bool);
    void L_EntityPlayer_RemoveInnateItemLegacy(struct EntityPlayer*, int, int);
    int L_EntityPlayer_RemoveInnateItem(struct EntityPlayer*, bool, int, int, const char*);
    int L_EntityPlayer_GetInnateItemCount(struct EntityPlayer*, bool, int, const char*);
    int L_EntityPlayer_SetInnateItemCount(struct EntityPlayer*, bool, int, int, const char*, bool);
    int L_EntityPlayer_SnapshotInnateGroup(struct EntityPlayer*, bool, const char*);
    void L_EntityPlayer_SetInnateGroup(struct EntityPlayer*, bool, const char*, const int*, const int*, int, bool);
    void L_EntityPlayer_ClearInnateItemGroup(struct EntityPlayer*, const char*);
    int L_EntityPlayer_SnapshotSpoofedCollectibles(struct EntityPlayer*);
    void L_EntityPlayer_GetSpoofedCollectible(int, int*, int*, bool*);
    void L_EntityPlayer_GetSnapshotPair(int, int*, int*);
    int L_EntityPlayer_SnapshotWispCollectibles(struct EntityPlayer*);

    struct EntityFamiliar* L_EntityPlayer_AddBoneOrbital(struct EntityPlayer*, struct Vector*);
    struct EntityEffect* L_EntityPlayer_FireBrimstoneBall(struct EntityPlayer*, struct Vector*, struct Vector*, struct Vector*);
    struct EntityEffect* L_EntityPlayer_ShootRedCandle(struct EntityPlayer*, struct Vector*);
    struct EntityEffect* L_EntityPlayer_ShootBlueCandle(struct EntityPlayer*, struct Vector*);
    struct EntityFamiliar* L_EntityPlayer_ThrowFriendlyDip(struct EntityPlayer*, int, struct Vector*, struct Vector*);
    struct EntityPickup* L_EntityPlayer_DropCollectible(struct EntityPlayer*, int, void*, bool);
    void L_EntityPlayer_DropCollectibleByHistoryIndex(struct EntityPlayer*, int, void*);
    struct EntityEffect* L_EntityPlayer_SpawnAquariusCreep(struct EntityPlayer*, struct TearParams*);
    void L_EntityPlayer_AddLocust(struct EntityPlayer*, int, struct Vector*);

    void L_EntityPlayer_AnimatePickup(struct EntityPlayer*, struct Sprite*, bool, const char*);
    void L_EntityPlayer_ReplaceCostumeSprite(struct EntityPlayer*, struct ItemConfigItem*, const char*, int);
    void L_EntityPlayer_GetCostumeNullPos(struct EntityPlayer*, const char*, bool, struct Vector*, struct Vector*);
    void L_EntityPlayer_PlayCollectibleAnim(struct EntityPlayer*, int, bool, const char*, int);
    bool L_EntityPlayer_IsCollectibleAnimFinished(struct EntityPlayer*, int, const char*);
    void L_EntityPlayer_ClearCollectibleAnim(struct EntityPlayer*, int);
    void L_EntityPlayer_GetTearHitParams(struct EntityPlayer*, int, float, int, void*, struct TearParams*);
    void L_EntityPlayer_GetMultiShotParams(struct EntityPlayer*, int, struct MultiShotParams*);
    void L_EntityPlayer_GetMultiShotPositionVelocity(struct EntityPlayer*, int, int, struct Vector*, float, struct MultiShotParams*, struct PosVel*);
    void L_EntityPlayer_GetGlyphOfBalanceDrop(struct EntityPlayer*, int*, int*);
    unsigned int L_EntityPlayer_GetSpecialGridCollision(struct EntityPlayer*, struct Vector*);
    short L_EntityPlayer_UseActiveItem(struct EntityPlayer*, int, unsigned int, int, int);
    bool L_EntityPlayer_HasInvincibility(struct EntityPlayer*, uint64_t, struct EntityRef*);
    void L_EntityPlayer_GetFootprintColor(struct EntityPlayer*, bool, struct KColor*);
    void L_EntityPlayer_SetFootprintColor(struct EntityPlayer*, struct KColor*, bool);

    void L_EntityPlayer_SetMegaBlastDuration(struct EntityPlayer*, int);
    void L_EntityPlayer_ShuffleCostumes(struct EntityPlayer*, bool, unsigned int);
    void L_EntityPlayer_RerollAllCollectibles(struct EntityPlayer*, struct RNG*, bool);
    bool L_EntityPlayer_ReviveCoopGhost(struct EntityPlayer*);
    bool L_EntityPlayer_IsPacifist(struct EntityPlayer*);
    bool L_EntityPlayer_IsLocalPlayer(struct EntityPlayer*);
    int L_EntityPlayer_GetErrorTrinketEffect(struct EntityPlayer*);
    void L_EntityPlayer_SetBlackHeart(struct EntityPlayer*, int);
    int L_EntityPlayer_AddNullCostume(struct EntityPlayer*, int);
    void L_EntityPlayer_SetHeadDirection(struct EntityPlayer*, int, int, bool);
    void L_EntityPlayer_SetPoopSpell(struct EntityPlayer*, int, int);
    void L_EntityPlayer_RemovePoopSpell(struct EntityPlayer*, int);
    void L_EntityPlayer_ClearDeadEyeChargeNow(struct EntityPlayer*);
    void L_EntityPlayer_CreateAfterimage(struct EntityPlayer*, int, struct Vector*);
    void L_EntityPlayer_AddCollectibleEffect(struct EntityPlayer*, int, bool, int, bool);
    void L_EntityPlayer_AddNullItemEffect(struct EntityPlayer*, int, bool, int, bool);
    void L_EntityPlayer_AddTrinketEffect(struct EntityPlayer*, int, bool, int, bool);
    bool L_EntityPlayer_AddSmeltedTrinket(struct EntityPlayer*, int, bool);
    bool L_EntityPlayer_IsValidTrinket(int);
    int L_EntityPlayer_GetLayerCount(struct EntityPlayer*);
    int L_EntityPlayer_GetLayerId(struct EntityPlayer*, const char*);
    bool L_EntityPlayer_HasChanceRevive(struct EntityPlayer*);

    void L_EntityPlayer_AddCustomCacheTag(struct EntityPlayer*, const char*);
    double L_EntityPlayer_GetCustomCacheValue(struct EntityPlayer*, const char*);
    bool L_EntityPlayer_IsForceCamo(struct EntityPlayer*);
    void L_EntityPlayer_SetForceCamo(struct EntityPlayer*, bool);
    bool L_EntityPlayer_HasCamoEffect(struct EntityPlayer*);
    int L_EntityPlayer_GetMaxCoins();
    int L_EntityPlayer_GetMaxKeys();
    int L_EntityPlayer_GetMaxBombs();
    void L_EntityPlayer_AddCandyHeartSoulLocketBonus(struct EntityPlayer*, bool, int, int);

    int L_EntityPlayer_GetBagOfCraftingSlot(struct EntityPlayer*, int);
    void L_EntityPlayer_SetBagOfCraftingContent(struct EntityPlayer*, const int*);
    void L_EntityPlayer_SetBagOfCraftingSlot(struct EntityPlayer*, int, int);
    int L_EntityPlayer_GetBagOfCraftingOutput(struct EntityPlayer*);
    int L_EntityPlayer_GetBagOfCraftingOutputItemPool(struct EntityPlayer*);
    void L_EntityPlayer_SetBagOfCraftingOutput(struct EntityPlayer*, int, int);
    void L_EntityPlayer_CalculateBagOfCraftingOutput(const int*, int*, int*);

    int L_EntityPlayer_CheckFamiliarEx(struct EntityPlayer*, int, int, struct RNG*, struct ItemConfigItem*, int);
    struct EntityFamiliar* L_EntityPlayer_GetCheckedFamiliar(int);
    void L_EntityPlayer_SalvageCollectibleEntity(struct EntityPlayer*, void*, struct RNG*, int);
    void L_EntityPlayer_SalvageCollectibleType(struct EntityPlayer*, int, struct Vector*, struct RNG*, int);
]]

local ffi = ffi
local repentogon = ffidll

ffi.reentrant(repentogon.L_EntityPlayer_CheckFamiliar)
ffi.reentrant(repentogon.L_EntityPlayer_CheckFamiliarEx)

local helpers = Entity.Helpers
local Getter = helpers.Getter
local VectorGetter = helpers.VectorGetter
local VectorSetter = helpers.VectorSetter
local BooleanSetter = helpers.BooleanSetter
local CopyStruct = helpers.CopyStruct
local Flags64 = helpers.Flags64
local CheckFlags64 = helpers.CheckFlags64
local EntityToPointer = ffichecks.entitytopointer
local traceback = debug.traceback

local TYPE_PLAYER = 1
local TRINKET_ID_MASK = 0x7fff
local NO_COOLDOWN = -6942069
local USE_NOANIM = 0x1
local USE_NOCOSTUME = 0x2
local USE_ALLOWNONMAIN = 0x8
local USE_REMOVEACTIVE = 0x10
local USE_CUSTOMVARDATA = 0x400
local COSTUME_SPRITE_DESC_SIZE = ffi.sizeof("struct CostumeSpriteDesc")
local STAT_NAMES = { "Damage", "FireDelay", "TearRange", "ShotSpeed", "MoveSpeed", "Luck" }

local printedNegativeInnateWarning = false

local function IntegerSetter(field)
    return function(self, value)
        ffichecks.checkinteger(1, value)
        ffi.setprivate(self, field, value)
    end
end

local function NumberSetter(field)
    return function(self, value)
        ffichecks.checknumber(1, value)
        ffi.setprivate(self, field, value)
    end
end

local function OptInteger(index, value, default, level)
    if value == nil then
        return default
    end
    ffichecks.checkinteger(index, value, level or 3)
    return value
end

local function OptNumber(index, value, default, level)
    if value == nil then
        return default
    end
    ffichecks.checknumber(index, value, level or 3)
    return value
end

local function CheckActiveSlot(index, slot, allowNegative)
    if slot == nil then
        return 0
    end
    ffichecks.checkinteger(index, slot, 3)
    if (not allowNegative and slot < 0) or slot > 3 then
        ffichecks.argerror(index, "Invalid ActiveSlot " .. slot, 3)
    end
    return slot
end

local function ValidatePool(index, pool)
    pool = OptInteger(index, pool, -1, 4)
    if pool > 30 or pool < 0 then
        return -1
    end
    return pool
end

local function LayerId(self, index, layer)
    if type(layer) == "string" then
        local id = repentogon.L_EntityPlayer_GetLayerId(self, layer)
        if id < 0 then
            ffichecks.argerror(index, "Invalid layer name " .. layer, 3)
        end
        return id
    end
    ffichecks.checkinteger(index, layer, 3)
    if layer < 0 or layer + 1 > repentogon.L_EntityPlayer_GetLayerCount(self) then
        ffichecks.argerror(index, "Invalid layer ID " .. layer, 3)
    end
    return layer
end

local function CheckInnateId(trinket, id)
    if not repentogon.L_EntityPlayer_IsValidInnateItem(trinket, id) then
        ffichecks.argerror(1, string.format(trinket and "Invalid TrinketType %d" or "Invalid CollectibleType %d", id), 3)
    end
end

local function SetInnateGroup(self, trinket, groupKey, items, addCostumes)
    groupKey = ffichecks.checkstring(1, groupKey)
    if not ffichecks.istable(items) then
        ffichecks.argerror(2, "Expected a table")
    end
    addCostumes = ffichecks.optboolean(addCostumes, true)

    local ids, counts = {}, {}
    for id, count in pairs(items) do
        if math.type(id) == "integer" and math.type(count) == "integer" then
            ids[#ids + 1] = id
            counts[#counts + 1] = count
        end
    end
    local length = #ids
    local idBuffer, countBuffer = ffi.new("int[?]", length), ffi.new("int[?]", length)
    for i = 1, length do
        idBuffer[i - 1] = ids[i]
        countBuffer[i - 1] = counts[i]
    end
    repentogon.L_EntityPlayer_SetInnateGroup(self, trinket, groupKey, idBuffer, countBuffer, length, addCostumes)
end

local function SnapshotPairs(count)
    local result = {}
    local key, value = ffi.new("int[1]"), ffi.new("int[1]")
    for i = 0, count - 1 do
        repentogon.L_EntityPlayer_GetSnapshotPair(i, key, value)
        result[key[0]] = value[0]
    end
    return result
end

local function HeartStatUps(self, soulLocket)
    local result = {}
    for i = 1, 6 do
        result[STAT_NAMES[i]] = repentogon.L_EntityPlayer_GetHeartStatUp(self, soulLocket, i - 1)
    end
    return result
end

local function SmeltedTrinketDesc(self, trinket)
    local entry = ffi.getprivate(self, "SmeltedTrinketsFirstValue")[trinket]
    return { trinketAmount = entry.TrinketAmount, goldenTrinketAmount = entry.GoldenTrinketAmount }
end

local function ColorGetter(field)
    return function(self)
        return CopyStruct("struct Color", ffi.getprivate(self, field))
    end
end

local function StructSetter(field, name)
    return function(self, value)
        ffichecks.checkcdata(1, value, name)
        ffi.setprivate(self, field, value)
    end
end

local getters = {
    ControlsEnabled = Getter("ControlsEnabledValue"),
    ControllerIndex = Getter("ControllerIndexValue"),
    CanFly = Getter("CanFlyValue"),
    FireDelay = function(self)
        return repentogon.L_EntityPlayer_GetFireDelayNative(self)
    end,
    TearsOffset = VectorGetter("TearsOffsetValue"),
    SpriteScale = VectorGetter("PlayerSpriteScaleValue"),
    TearFlags = function(self)
        return CopyStruct("struct BitSet128", ffi.getprivate(self, "TearFlagsValue"))
    end,
    TearColor = ColorGetter("TearColorValue"),
    LaserColor = ColorGetter("LaserColorValue"),
    QueuedItem = function(self)
        return CopyStruct("struct QueueItemData", ffi.getprivate(self, "QueuedItemValue"))
    end,
    FriendBallEnemy = function(self)
        return CopyStruct("struct EntityDesc", ffi.getprivate(self, "FriendBallEnemyValue"))
    end,
}

local setters = {
    ControlsEnabled = function(self, value)
        ffi.setprivate(self, "ControlsEnabledValue", not not value)
    end,
    CanFly = function(self, value)
        ffi.setprivate(self, "CanFlyValue", not not value)
    end,
    FireDelay = function(self, value)
        ffichecks.checknumber(1, value)
        repentogon.L_EntityPlayer_SetFireDelayNative(self, value)
    end,
    TearsOffset = VectorSetter("TearsOffsetValue"),
    SpriteScale = VectorSetter("PlayerSpriteScaleValue"),
    TearFlags = StructSetter("TearFlagsValue", "BitSet128"),
    TearColor = StructSetter("TearColorValue", "Color"),
    LaserColor = StructSetter("LaserColorValue", "Color"),
    QueuedItem = StructSetter("QueuedItemValue", "QueueItemData"),
    FriendBallEnemy = StructSetter("FriendBallEnemyValue", "EntityDesc"),
}

local methods = {
    AddMaxHearts = function(self, amount, ignoreKeeper)
        ffichecks.checkinteger(1, amount)
        ignoreKeeper = not not ignoreKeeper
        repentogon.L_EntityPlayer_AddMaxHearts(self, amount, ignoreKeeper)
    end,
    HasFullHearts = function(self)
        return repentogon.L_EntityPlayer_HasFullHearts(self)
    end,
    AddHearts = function(self, hearts, unk, unk2)
        ffichecks.checkinteger(1, hearts)
        unk = not not unk
        unk2 = not not unk2
        repentogon.L_EntityPlayer_AddHearts(self, hearts, unk, unk2)
    end,
    AddEternalHearts = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddEternalHearts(self, amount)
    end,
    AddSoulHearts = function(self, amount, unk)
        ffichecks.checkinteger(1, amount)
        unk = not not unk
        repentogon.L_EntityPlayer_AddSoulHearts(self, amount, unk)
    end,
    AddBlackHearts = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddBlackHearts(self, amount)
    end,
    RemoveBlackHeart = function(self, heart)
        ffichecks.checkinteger(1, heart)
        repentogon.L_EntityPlayer_RemoveBlackHeart(self, heart)
    end,
    IsBlackHeart = function(self, heart)
        ffichecks.checkinteger(1, heart)
        return repentogon.L_EntityPlayer_IsBlackHeart(self, heart)
    end,
    AddJarHearts = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddJarHearts(self, amount)
    end,
    AddJarFlies = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddJarFlies(self, amount)
    end,
    AddCoins = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddCoins(self, amount)
    end,
    AddBombs = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddBombs(self, amount)
    end,
    AddKeys = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddKeys(self, amount)
    end,
    AddGoldenKey = function(self)
        repentogon.L_EntityPlayer_AddGoldenKey(self)
    end,
    RemoveGoldenKey = function(self)
        repentogon.L_EntityPlayer_RemoveGoldenKey(self)
    end,
    AddGoldenBomb = function(self)
        repentogon.L_EntityPlayer_AddGoldenBomb(self)
    end,
    RemoveGoldenBomb = function(self)
        repentogon.L_EntityPlayer_RemoveGoldenBomb(self)
    end,
    AddGoldenHearts = function(self, amount, unk)
        ffichecks.checkinteger(1, amount)
        unk = not not unk
        repentogon.L_EntityPlayer_AddGoldenHearts(self, amount, unk)
    end,
    AddPrettyFly = function(self)
        repentogon.L_EntityPlayer_AddPrettyFly(self)
    end,
    TryUseKey = function(self)
        return repentogon.L_EntityPlayer_TryUseKey(self)
    end,
    AddBoneHearts = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddBoneHearts(self, amount)
    end,
    AddBrokenHearts = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddBrokenHearts(self, amount)
    end,
    AddRottenHearts = function(self, amount, unk)
        ffichecks.checkinteger(1, amount)
        unk = not not unk
        repentogon.L_EntityPlayer_AddRottenHearts(self, amount, unk)
    end,
    AddGigaBombs = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddGigaBombs(self, amount)
    end,
    AddSoulCharge = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddSoulCharge(self, amount)
    end,
    AddBloodCharge = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddBloodCharge(self, amount)
    end,
    GetEffectiveSoulCharge = function(self)
        return repentogon.L_EntityPlayer_GetEffectiveSoulCharge(self)
    end,
    GetEffectiveBloodCharge = function(self)
        return repentogon.L_EntityPlayer_GetEffectiveBloodCharge(self)
    end,
    GetEffectiveMaxHearts = function(self)
        return repentogon.L_EntityPlayer_GetEffectiveMaxHearts(self)
    end,
    GetHeartLimit = function(self, keeper)
        keeper = not not keeper
        return repentogon.L_EntityPlayer_GetHeartLimit(self, keeper)
    end,
    SetFullHearts = function(self)
        repentogon.L_EntityPlayer_SetFullHearts(self)
    end,
    IsBoneHeart = function(self, heart)
        ffichecks.checkinteger(1, heart)
        return repentogon.L_EntityPlayer_IsBoneHeart(self, heart)
    end,
    CanPickRedHearts = function(self)
        return repentogon.L_EntityPlayer_CanPickRedHearts(self)
    end,
    CanPickSoulHearts = function(self)
        return repentogon.L_EntityPlayer_CanPickSoulHearts(self)
    end,
    CanPickBlackHearts = function(self)
        return repentogon.L_EntityPlayer_CanPickBlackHearts(self)
    end,
    CanPickGoldenHearts = function(self)
        return repentogon.L_EntityPlayer_CanPickGoldenHearts(self)
    end,
    CanPickBoneHearts = function(self)
        return repentogon.L_EntityPlayer_CanPickBoneHearts(self)
    end,
    CanPickRottenHearts = function(self)
        return repentogon.L_EntityPlayer_CanPickRottenHearts(self)
    end,
    ChangePlayerType = function(self, playerType, unk)
        ffichecks.checkinteger(1, playerType)
        unk = not not unk
        repentogon.L_EntityPlayer_ChangePlayerType(self, playerType, unk)
    end,
    GetExtraLives = function(self)
        return repentogon.L_EntityPlayer_GetExtraLives(self)
    end,
    WillPlayerRevive = function(self)
        return repentogon.L_EntityPlayer_WillPlayerRevive(self)
    end,
    Revive = function(self)
        repentogon.L_EntityPlayer_Revive(self)
    end,
    DonateLuck = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_DonateLuck(self, amount)
    end,
    AddCard = function(self, card)
        ffichecks.checkinteger(1, card)
        repentogon.L_EntityPlayer_AddCard(self, card)
    end,
    AddPill = function(self, pill)
        ffichecks.checkinteger(1, pill)
        repentogon.L_EntityPlayer_AddPill(self, pill)
    end,
    GetCard = function(self, slot)
        ffichecks.checkinteger(1, slot)
        return repentogon.L_EntityPlayer_GetCard(self, slot)
    end,
    GetPill = function(self, slot)
        ffichecks.checkinteger(1, slot)
        return repentogon.L_EntityPlayer_GetPill(self, slot)
    end,
    SetCard = function(self, slot, card)
        ffichecks.checkinteger(1, slot)
        ffichecks.checkinteger(2, card)
        repentogon.L_EntityPlayer_SetCard(self, slot, card)
    end,
    SetPill = function(self, slot, color)
        ffichecks.checkinteger(1, slot)
        ffichecks.checkinteger(2, color)
        repentogon.L_EntityPlayer_SetPill(self, slot, color)
    end,
    FlushQueueItem = function(self)
        return repentogon.L_EntityPlayer_FlushQueueItem(self)
    end,
    GetCollectibleCount = function(self)
        return repentogon.L_EntityPlayer_GetCollectibleCount(self)
    end,
    AddTrinket = function(self, trinket, firstTime)
        ffichecks.checkinteger(1, trinket)
        firstTime = ffichecks.optboolean(firstTime, true)
        repentogon.L_EntityPlayer_AddTrinket(self, trinket, firstTime)
    end,
    TryRemoveTrinket = function(self, trinket)
        ffichecks.checkinteger(1, trinket)
        return repentogon.L_EntityPlayer_TryRemoveTrinket(self, trinket)
    end,
    GetMaxTrinkets = function(self)
        return repentogon.L_EntityPlayer_GetMaxTrinkets(self)
    end,
    RemoveCollectible = function(self, collectible, ignoreModifiers, slot, removeFromPlayerForm)
        ffichecks.checkinteger(1, collectible)
        ignoreModifiers = not not ignoreModifiers
        if slot == nil then slot = 0 else ffichecks.checkinteger(3, slot) end
        removeFromPlayerForm = ffichecks.optboolean(removeFromPlayerForm, true)
        repentogon.L_EntityPlayer_RemoveCollectible(self, collectible, ignoreModifiers, slot, removeFromPlayerForm)
    end,
    ClearTemporaryEffects = function(self)
        repentogon.L_EntityPlayer_ClearTemporaryEffects(self)
    end,
    HasPlayerForm = function(self, form)
        ffichecks.checkinteger(1, form)
        return repentogon.L_EntityPlayer_HasPlayerForm(self, form)
    end,
    CanAddCollectible = function(self, collectible)
        if collectible == nil then collectible = 0 else ffichecks.checkinteger(1, collectible) end
        return repentogon.L_EntityPlayer_CanAddCollectible(self, collectible)
    end,
    TryHoldTrinket = function(self, trinket)
        ffichecks.checkinteger(1, trinket)
        return repentogon.L_EntityPlayer_TryHoldTrinket(self, trinket)
    end,
    EvaluateItems = function(self)
        repentogon.L_EntityPlayer_EvaluateItems(self)
    end,
    RespawnFamiliars = function(self)
        repentogon.L_EntityPlayer_RespawnFamiliars(self)
    end,
    HasWeaponType = function(self, weaponType)
        ffichecks.checkinteger(1, weaponType)
        return repentogon.L_EntityPlayer_HasWeaponType(self, weaponType)
    end,
    TryRemoveCollectibleCostume = function(self, collectible, unk)
        ffichecks.checkinteger(1, collectible)
        unk = not not unk
        repentogon.L_EntityPlayer_TryRemoveCollectibleCostume(self, collectible, unk)
    end,
    TryRemoveTrinketCostume = function(self, trinket)
        ffichecks.checkinteger(1, trinket)
        repentogon.L_EntityPlayer_TryRemoveTrinketCostume(self, trinket)
    end,
    TryRemoveNullCostume = function(self, nullItem)
        ffichecks.checkinteger(1, nullItem)
        repentogon.L_EntityPlayer_TryRemoveNullCostume(self, nullItem)
    end,
    RemoveSkinCostume = function(self)
        repentogon.L_EntityPlayer_RemoveSkinCostume(self)
    end,
    ClearCostumes = function(self)
        repentogon.L_EntityPlayer_ClearCostumes(self)
    end,
    AddPlayerFormCostume = function(self, form)
        ffichecks.checkinteger(1, form)
        repentogon.L_EntityPlayer_AddPlayerFormCostume(self, form)
    end,
    UseCard = function(self, card, useFlags)
        ffichecks.checkinteger(1, card)
        if useFlags == nil then useFlags = 0 else ffichecks.checkinteger(2, useFlags) end
        repentogon.L_EntityPlayer_UseCard(self, card, useFlags)
    end,
    UsePill = function(self, effect, color, useFlags)
        ffichecks.checkinteger(1, effect)
        ffichecks.checkinteger(2, color)
        if useFlags == nil then useFlags = 0 else ffichecks.checkinteger(3, useFlags) end
        repentogon.L_EntityPlayer_UsePill(self, effect, color, useFlags)
    end,
    TriggerBookOfVirtues = function(self, collectible, charge)
        if collectible == nil then collectible = 0 else ffichecks.checkinteger(1, collectible) end
        if charge == nil then charge = 0 else ffichecks.checkinteger(2, charge) end
        repentogon.L_EntityPlayer_TriggerBookOfVirtues(self, collectible, charge)
    end,
    SwapActiveItems = function(self)
        repentogon.L_EntityPlayer_SwapActiveItems(self)
    end,
    ResetItemState = function(self)
        repentogon.L_EntityPlayer_ResetItemState(self)
    end,
    HasTimedItem = function(self)
        return repentogon.L_EntityPlayer_HasTimedItem(self)
    end,
    AddDollarBillEffect = function(self)
        repentogon.L_EntityPlayer_AddDollarBillEffect(self)
    end,
    AddCurseMistEffect = function(self)
        repentogon.L_EntityPlayer_AddCurseMistEffect(self)
    end,
    RemoveCurseMistEffect = function(self)
        repentogon.L_EntityPlayer_RemoveCurseMistEffect(self)
    end,
    InitBabySkin = function(self)
        repentogon.L_EntityPlayer_InitBabySkin(self)
    end,
    UpdateCanShoot = function(self)
        repentogon.L_EntityPlayer_UpdateCanShoot(self)
    end,
    AddDeadEyeCharge = function(self)
        repentogon.L_EntityPlayer_AddDeadEyeCharge(self)
    end,
    GetZodiacEffect = function(self)
        return repentogon.L_EntityPlayer_GetZodiacEffect(self)
    end,
    GetGreedDonationBreakChance = function(self)
        return repentogon.L_EntityPlayer_GetGreedDonationBreakChance(self)
    end,
    IsFullSpriteRendering = function(self)
        return repentogon.L_EntityPlayer_IsFullSpriteRendering(self)
    end,
    UsePoopSpell = function(self, spell)
        ffichecks.checkinteger(1, spell)
        repentogon.L_EntityPlayer_UsePoopSpell(self, spell)
    end,
    GetMaxPoopMana = function(self)
        return repentogon.L_EntityPlayer_GetMaxPoopMana(self)
    end,
    AddPoopMana = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddPoopMana(self, amount)
    end,
    GetPoopSpell = function(self, slot)
        ffichecks.checkinteger(1, slot)
        return repentogon.L_EntityPlayer_GetPoopSpell(self, slot)
    end,
    GetNPCTarget = function(self)
        return repentogon.L_EntityPlayer_GetNPCTarget(self)
    end,
    GetActiveWeaponEntity = function(self)
        return repentogon.L_EntityPlayer_GetActiveWeaponEntity(self)
    end,
    GetMainTwin = function(self)
        return repentogon.L_EntityPlayer_GetMainTwin(self)
    end,
    CanPickupItem = function(self)
        return repentogon.L_EntityPlayer_CanPickupItem(self)
    end,
    IsHoldingItem = function(self)
        return repentogon.L_EntityPlayer_IsHoldingItem(self)
    end,
    IsHeldItemVisible = function(self)
        return repentogon.L_EntityPlayer_IsHeldItemVisible(self)
    end,
    TryHoldEntity = function(self, entity)
        if entity == nil then ffichecks.argerror(1, "Entity expected, got nil") end
        entity = EntityToPointer(entity)
        return repentogon.L_EntityPlayer_TryHoldEntity(self, entity)
    end,
    SetShootingCooldown = function(self, cooldown)
        ffichecks.checkinteger(1, cooldown)
        repentogon.L_EntityPlayer_SetShootingCooldown(self, cooldown)
    end,
    SetMinDamageCooldown = function(self, cooldown)
        ffichecks.checkinteger(1, cooldown)
        repentogon.L_EntityPlayer_SetMinDamageCooldown(self, cooldown)
    end,
    AreControlsEnabled = function(self)
        return repentogon.L_EntityPlayer_AreControlsEnabled(self)
    end,
    AnimateCollectible = function(self, collectible, animName, spriteAnimName)
        ffichecks.checkinteger(1, collectible)
        if animName == nil then animName = "Pickup" else animName = ffichecks.checkstring(2, animName) end
        if spriteAnimName == nil then spriteAnimName = "PlayerPickupSparkle" else spriteAnimName = ffichecks.checkstring(3, spriteAnimName) end
        repentogon.L_EntityPlayer_AnimateCollectible(self, collectible, animName, spriteAnimName)
    end,
    AnimateTrinket = function(self, trinket, animName, spriteAnimName)
        ffichecks.checkinteger(1, trinket)
        if animName == nil then animName = "Pickup" else animName = ffichecks.checkstring(2, animName) end
        if spriteAnimName == nil then spriteAnimName = "PlayerPickupSparkle" else spriteAnimName = ffichecks.checkstring(3, spriteAnimName) end
        repentogon.L_EntityPlayer_AnimateTrinket(self, trinket, animName, spriteAnimName)
    end,
    AnimateCard = function(self, card, animName)
        ffichecks.checkinteger(1, card)
        if animName == nil then animName = "Pickup" else animName = ffichecks.checkstring(2, animName) end
        repentogon.L_EntityPlayer_AnimateCard(self, card, animName)
    end,
    AnimatePill = function(self, pill, animName)
        ffichecks.checkinteger(1, pill)
        if animName == nil then animName = "Pickup" else animName = ffichecks.checkstring(2, animName) end
        repentogon.L_EntityPlayer_AnimatePill(self, pill, animName)
    end,
    AnimateTrapdoor = function(self)
        repentogon.L_EntityPlayer_AnimateTrapdoor(self)
    end,
    AnimateLightTravel = function(self)
        repentogon.L_EntityPlayer_AnimateLightTravel(self)
    end,
    AnimateAppear = function(self)
        repentogon.L_EntityPlayer_AnimateAppear(self)
    end,
    AnimateTeleport = function(self, unk)
        unk = not not unk
        repentogon.L_EntityPlayer_AnimateTeleport(self, unk)
    end,
    AnimateHappy = function(self)
        repentogon.L_EntityPlayer_AnimateHappy(self)
    end,
    AnimateSad = function(self)
        repentogon.L_EntityPlayer_AnimateSad(self)
    end,
    AnimatePitfallIn = function(self, unk)
        unk = not not unk
        repentogon.L_EntityPlayer_AnimatePitfallIn(self, unk)
    end,
    AnimatePitfallOut = function(self)
        repentogon.L_EntityPlayer_AnimatePitfallOut(self)
    end,
    PlayExtraAnimation = function(self, animName)
        animName = ffichecks.checkstring(1, animName)
        repentogon.L_EntityPlayer_PlayExtraAnimation(self, animName)
    end,
    QueueExtraAnimation = function(self, animName)
        animName = ffichecks.checkstring(1, animName)
        repentogon.L_EntityPlayer_QueueExtraAnimation(self, animName)
    end,
    AddBlueFlies = function(self, amount, position, target)
        ffichecks.checkinteger(1, amount)
        ffichecks.checkcdata(2, position, "Vector")
        target = EntityToPointer(target)
        return repentogon.L_EntityPlayer_AddBlueFlies(self, amount, position, target)
    end,
    AddBlueSpider = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        return repentogon.L_EntityPlayer_AddBlueSpider(self, position)
    end,
    AddFriendlyDip = function(self, subtype, position)
        ffichecks.checkinteger(1, subtype)
        ffichecks.checkcdata(2, position, "Vector")
        return repentogon.L_EntityPlayer_AddFriendlyDip(self, subtype, position)
    end,
    AddItemWisp = function(self, collectible, position, adjustOrbitLayer)
        ffichecks.checkinteger(1, collectible)
        ffichecks.checkcdata(2, position, "Vector")
        adjustOrbitLayer = ffichecks.optboolean(adjustOrbitLayer, false)
        return repentogon.L_EntityPlayer_AddItemWisp(self, collectible, position, adjustOrbitLayer)
    end,
    AddMinisaac = function(self, position, playAnim)
        ffichecks.checkcdata(1, position, "Vector")
        playAnim = ffichecks.optboolean(playAnim, true)
        return repentogon.L_EntityPlayer_AddMinisaac(self, position, playAnim)
    end,
    AddSwarmFlyOrbital = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        return repentogon.L_EntityPlayer_AddSwarmFlyOrbital(self, position)
    end,
    AddWisp = function(self, collectible, position, adjustOrbitLayer, dontUpdate)
        ffichecks.checkinteger(1, collectible)
        ffichecks.checkcdata(2, position, "Vector")
        adjustOrbitLayer = ffichecks.optboolean(adjustOrbitLayer, false)
        dontUpdate = ffichecks.optboolean(dontUpdate, false)
        return repentogon.L_EntityPlayer_AddWisp(self, collectible, position, adjustOrbitLayer, dontUpdate)
    end,
    DoZitEffect = function(self, direction)
        ffichecks.checkcdata(1, direction, "Vector")
        repentogon.L_EntityPlayer_DoZitEffect(self, direction)
    end,
    DropPocketItem = function(self, slot, position)
        ffichecks.checkinteger(1, slot)
        ffichecks.checkcdata(2, position, "Vector")
        repentogon.L_EntityPlayer_DropPocketItem(self, slot, position)
    end,
    DropTrinket = function(self, position, replaceTick)
        ffichecks.checkcdata(1, position, "Vector")
        replaceTick = ffichecks.optboolean(replaceTick, false)
        repentogon.L_EntityPlayer_DropTrinket(self, position, replaceTick)
    end,
    FireBomb = function(self, position, velocity, source)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, velocity, "Vector")
        source = EntityToPointer(source)
        return repentogon.L_EntityPlayer_FireBomb(self, position, velocity, source)
    end,
    FireKnife = function(self, parent, rotationOffset, cantOverwrite, subType, variant)
        if parent == nil then ffichecks.argerror(1, "Entity expected, got nil") end
        parent = EntityToPointer(parent)
        if rotationOffset == nil then rotationOffset = 0 else ffichecks.checknumber(2, rotationOffset) end
        cantOverwrite = not not cantOverwrite
        if subType == nil then subType = 0 else ffichecks.checkinteger(4, subType) end
        if variant == nil then variant = 0 else ffichecks.checkinteger(5, variant) end
        return repentogon.L_EntityPlayer_FireKnife(self, parent, rotationOffset, cantOverwrite, subType, variant)
    end,
    FireDelayedBrimstone = function(self, angle, source)
        ffichecks.checknumber(1, angle)
        if source == nil then ffichecks.argerror(2, "Entity expected, got nil") end
        source = EntityToPointer(source)
        return repentogon.L_EntityPlayer_FireDelayedBrimstone(self, angle, source)
    end,
    SpawnMawOfVoid = function(self, timeout)
        ffichecks.checkinteger(1, timeout)
        return repentogon.L_EntityPlayer_SpawnMawOfVoid(self, timeout)
    end,
    FireBrimstone = function(self, direction, source, damageMultiplier)
        ffichecks.checkcdata(1, direction, "Vector")
        source = EntityToPointer(source)
        if damageMultiplier == nil then damageMultiplier = 1 else ffichecks.checknumber(3, damageMultiplier) end
        return repentogon.L_EntityPlayer_FireBrimstone(self, direction, source, damageMultiplier)
    end,
    FireTechLaser = function(self, position, offsetID, direction, leftEye, oneHit, source, damageMultiplier)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkinteger(2, offsetID)
        ffichecks.checkcdata(3, direction, "Vector")
        leftEye = ffichecks.optboolean(leftEye, true)
        oneHit = ffichecks.optboolean(oneHit, false)
        source = EntityToPointer(source)
        if damageMultiplier == nil then damageMultiplier = 1 else ffichecks.checknumber(7, damageMultiplier) end
        return repentogon.L_EntityPlayer_FireTechLaser(self, position, offsetID, direction, leftEye, oneHit, source, damageMultiplier)
    end,
    FireTechXLaser = function(self, position, direction, radius, source, damageMultiplier)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, direction, "Vector")
        ffichecks.checknumber(3, radius)
        source = EntityToPointer(source)
        if damageMultiplier == nil then damageMultiplier = 1 else ffichecks.checknumber(5, damageMultiplier) end
        return repentogon.L_EntityPlayer_FireTechXLaser(self, position, direction, radius, source, damageMultiplier)
    end,
    ThrowBlueSpider = function(self, position, target)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, target, "Vector")
        return repentogon.L_EntityPlayer_ThrowBlueSpider(self, position, target)
    end,
    ThrowHeldEntity = function(self, velocity)
        ffichecks.checkcdata(1, velocity, "Vector")
        return repentogon.L_EntityPlayer_ThrowHeldEntity(self, velocity)
    end,
    GetFlyingOffset = function(self)
        local result = Vector(0, 0)
        repentogon.L_EntityPlayer_GetFlyingOffset(self, result)
        return result
    end,
    GetLaserOffset = function(self, laserOffsetID, direction)
        ffichecks.checkinteger(1, laserOffsetID)
        ffichecks.checkcdata(2, direction, "Vector")
        local result = Vector(0, 0)
        repentogon.L_EntityPlayer_GetLaserOffset(self, laserOffsetID, direction, result)
        return result
    end,
    GetMovementJoystick = function(self)
        local result = Vector(0, 0)
        repentogon.L_EntityPlayer_GetMovementJoystick(self, result)
        return result
    end,
    GetShootingJoystick = function(self)
        local result = Vector(0, 0)
        repentogon.L_EntityPlayer_GetShootingJoystick(self, result)
        return result
    end,
    GetTearMovementInheritance = function(self, shotDirection)
        ffichecks.checkcdata(1, shotDirection, "Vector")
        local result = Vector(0, 0)
        repentogon.L_EntityPlayer_GetTearMovementInheritance(self, shotDirection, result)
        return result
    end,
    GetBodyMoveDirection = function(self)
        local result = Vector(0, 0)
        repentogon.L_EntityPlayer_GetBodyMoveDirection(self, result)
        return result
    end,
    GetEnterPosition = function(self)
        local result = Vector(0, 0)
        repentogon.L_EntityPlayer_GetEnterPosition(self, result)
        return result
    end,
    IsPosInSpotLight = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        return repentogon.L_EntityPlayer_IsPosInSpotLight(self, position)
    end,
    RenderBody = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        repentogon.L_EntityPlayer_RenderBody(self, position)
    end,
    RenderGlow = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        repentogon.L_EntityPlayer_RenderGlow(self, position)
    end,
    RenderHead = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        repentogon.L_EntityPlayer_RenderHead(self, position)
    end,
    RenderTop = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        repentogon.L_EntityPlayer_RenderTop(self, position)
    end,
    Teleport = function(self, position, doEffects, teleportTwinPlayers)
        ffichecks.checkcdata(1, position, "Vector")
        doEffects = ffichecks.optboolean(doEffects, true)
        teleportTwinPlayers = ffichecks.optboolean(teleportTwinPlayers, false)
        repentogon.L_EntityPlayer_Teleport(self, position, doEffects, teleportTwinPlayers)
    end,
    SpawnClot = function(self, position, canKillPlayer)
        ffichecks.checkcdata(1, position, "Vector")
        canKillPlayer = ffichecks.optboolean(canKillPlayer, false)
        repentogon.L_EntityPlayer_SpawnClot(self, position, canKillPlayer)
    end,
    TryForgottenThrow = function(self, direction)
        ffichecks.checkcdata(1, direction, "Vector")
        return repentogon.L_EntityPlayer_TryForgottenThrow(self, direction)
    end,
    AddCollectible = function(self, collectible, charge, firstTime, slot, varData, pool)
        ffichecks.checkinteger(1, collectible)
        if charge == nil then charge = 0 else ffichecks.checkinteger(2, charge) end
        firstTime = ffichecks.optboolean(firstTime, true)
        slot = CheckActiveSlot(4, slot, false)
        if varData == nil then varData = 0 else ffichecks.checkinteger(5, varData) end
        if pool == nil then pool = 0 else ffichecks.checkinteger(6, pool) end
        repentogon.L_EntityPlayer_AddCollectible(self, collectible, charge, firstTime, slot, varData, pool)
    end,
    AddCostume = function(self, item, itemStateOnly)
        ffichecks.checkcdata(1, item, "ItemConfigItem")
        itemStateOnly = ffichecks.checkboolean(2, itemStateOnly)
        repentogon.L_EntityPlayer_AddCostume(self, item, itemStateOnly)
    end,
    CheckFamiliar = function(self, variant, targetCount, rng, item, subType)
        ffichecks.checkinteger(1, variant)
        ffichecks.checkinteger(2, targetCount)
        ffichecks.checkcdata(3, rng, "RNG")
        ffichecks.checkcdata(4, item, "ItemConfigItem", true)
        if subType == nil then subType = -1 else ffichecks.checkinteger(5, subType) end
        repentogon.L_EntityPlayer_CheckFamiliar(self, variant, targetCount, rng, item, subType)
    end,
    RemoveCostume = function(self, item)
        ffichecks.checkcdata(1, item, "ItemConfigItem")
        repentogon.L_EntityPlayer_RemoveCostume(self, item)
    end,
    InitTwin = function(self, playerType)
        ffichecks.checkinteger(1, playerType)
        return repentogon.L_EntityPlayer_InitTwin(self, playerType)
    end,
    InitPostLevelInitStats = function(self)
        repentogon.L_EntityPlayer_InitPostLevelInitStats(self)
    end,
    SetItemState = function(self, collectible)
        ffichecks.checkinteger(1, collectible)
        repentogon.L_EntityPlayer_SetItemState(self, collectible)
    end,
    GetHealthType = function(self)
        return repentogon.L_EntityPlayer_GetHealthType(self)
    end,
    GetTotalActiveCharge = function(self, slot)
        slot = CheckActiveSlot(1, slot, false)
        return repentogon.L_EntityPlayer_GetTotalActiveCharge(self, slot)
    end,
    GetActiveMaxCharge = function(self, slot)
        slot = CheckActiveSlot(1, slot, false)
        return repentogon.L_EntityPlayer_GetActiveMaxCharge(self, slot)
    end,
    GetActiveMinUsableCharge = function(self, slot)
        slot = CheckActiveSlot(1, slot, false)
        return repentogon.L_EntityPlayer_GetActiveMinUsableCharge(self, slot)
    end,
    SetActiveVarData = function(self, varData, slot)
        ffichecks.checkinteger(1, varData)
        slot = CheckActiveSlot(2, slot, false)
        repentogon.L_EntityPlayer_SetActiveVarData(self, varData, slot)
    end,
    GetActiveItem = function(self, slot)
        slot = CheckActiveSlot(1, slot, false)
        return repentogon.L_EntityPlayer_GetActiveItem(self, slot)
    end,
    GetActiveCharge = function(self, slot)
        slot = CheckActiveSlot(1, slot, false)
        return repentogon.L_EntityPlayer_GetActiveCharge(self, slot)
    end,
    GetActiveSubCharge = function(self, slot)
        slot = CheckActiveSlot(1, slot, false)
        return repentogon.L_EntityPlayer_GetActiveSubCharge(self, slot)
    end,
    GetBatteryCharge = function(self, slot)
        slot = CheckActiveSlot(1, slot, false)
        return repentogon.L_EntityPlayer_GetBatteryCharge(self, slot)
    end,
    NeedsCharge = function(self, slot)
        slot = CheckActiveSlot(1, slot, true)
        return repentogon.L_EntityPlayer_NeedsCharge(self, slot)
    end,
    SetActiveCharge = function(self, charge, slot)
        ffichecks.checkinteger(1, charge)
        slot = CheckActiveSlot(2, slot, true)
        repentogon.L_EntityPlayer_SetActiveCharge(self, charge, slot)
    end,
    AddActiveCharge = function(self, charge, slot, flashHUD, overcharge, force)
        ffichecks.checkinteger(1, charge)
        slot = CheckActiveSlot(2, slot, true)
        flashHUD = ffichecks.optboolean(flashHUD, true)
        overcharge = ffichecks.optboolean(overcharge, false)
        force = ffichecks.optboolean(force, false)
        return repentogon.L_EntityPlayer_AddActiveCharge(self, charge, slot, flashHUD, overcharge, force)
    end,
    FullCharge = function(self, slot, force)
        slot = CheckActiveSlot(1, slot, true)
        force = ffichecks.optboolean(force, true)
        return repentogon.L_EntityPlayer_FullCharge(self, slot, force)
    end,
    DischargeActiveItem = function(self, slot)
        slot = CheckActiveSlot(1, slot, false)
        repentogon.L_EntityPlayer_DischargeActiveItem(self, slot)
    end,
    CanOverrideActiveItem = function(self, slot)
        slot = CheckActiveSlot(1, slot, false)
        return repentogon.L_EntityPlayer_CanOverrideActiveItem(self, slot)
    end,
    GetActiveItemSlot = function(self, collectible)
        ffichecks.checkinteger(1, collectible)
        return repentogon.L_EntityPlayer_GetActiveItemSlot(self, collectible)
    end,
    IncrementPlayerFormCounter = function(self, form, amount)
        ffichecks.checkinteger(1, form)
        ffichecks.checkinteger(2, amount)
        repentogon.L_EntityPlayer_IncrementPlayerFormCounter(self, form, amount)
    end,
    TryPreventDeath = function(self)
        return repentogon.L_EntityPlayer_TryPreventDeath(self)
    end,
    RemoveCollectibleByHistoryIndex = function(self, index)
        ffichecks.checkinteger(1, index)
        repentogon.L_EntityPlayer_RemoveCollectibleByHistoryIndex(self, index)
    end,
    TryFakeDeath = function(self)
        return repentogon.L_EntityPlayer_TryFakeDeath(self)
    end,
    GetWeaponModifiers = function(self)
        return repentogon.L_EntityPlayer_GetWeaponModifiers(self)
    end,
    EnableWeaponType = function(self, weaponType, set)
        ffichecks.checkinteger(1, weaponType)
        set = ffichecks.checkboolean(2, set)
        repentogon.L_EntityPlayer_EnableWeaponType(self, weaponType, set)
    end,
    TriggerRoomClear = function(self)
        repentogon.L_EntityPlayer_TriggerRoomClear(self)
    end,
    UpdateIsaacPregnancy = function(self, cambion)
        cambion = ffichecks.checkboolean(1, cambion)
        repentogon.L_EntityPlayer_UpdateIsaacPregnancy(self, cambion)
    end,
    GetCambionPregnancyLevel = function(self)
        return repentogon.L_EntityPlayer_GetCambionPregnancyLevel(self)
    end,
    SwapForgottenForm = function(self, ignoreHealth, noEffects)
        ignoreHealth = ffichecks.optboolean(ignoreHealth, false)
        noEffects = ffichecks.optboolean(noEffects, false)
        return repentogon.L_EntityPlayer_SwapForgottenForm(self, ignoreHealth, noEffects)
    end,
    PlayDelayedSFX = function(self, soundEffectID, soundDelay, frameDelay, volume)
        ffichecks.checkinteger(1, soundEffectID)
        if soundDelay == nil then soundDelay = 0 else ffichecks.checkinteger(2, soundDelay) end
        if frameDelay == nil then frameDelay = 2 else ffichecks.checkinteger(3, frameDelay) end
        if volume == nil then volume = 1 else ffichecks.checknumber(4, volume) end
        repentogon.L_EntityPlayer_PlayDelayedSFX(self, soundEffectID, soundDelay, frameDelay, volume)
    end,
    CanUsePill = function(self, pillEffect)
        ffichecks.checkinteger(1, pillEffect)
        return repentogon.L_EntityPlayer_CanUsePill(self, pillEffect)
    end,
    GetMaxPocketItems = function(self)
        return repentogon.L_EntityPlayer_GetMaxPocketItems(self)
    end,
    CanAddCollectibleToInventory = function(self, collectible)
        ffichecks.checkinteger(1, collectible)
        return repentogon.L_EntityPlayer_CanAddCollectibleToInventory(self, collectible)
    end,
    AddLeprosy = function(self)
        repentogon.L_EntityPlayer_AddLeprosy(self)
    end,
    AddUrnSouls = function(self, amount)
        ffichecks.checkinteger(1, amount)
        repentogon.L_EntityPlayer_AddUrnSouls(self, amount)
    end,
    GetDeathAnimName = function(self)
        local text = repentogon.L_EntityPlayer_GetDeathAnimName(self)
        if text == nil then return nil end
        return ffi.string(text)
    end,
    GetGlitchBabySubType = function(self)
        return repentogon.L_EntityPlayer_GetGlitchBabySubType(self)
    end,
    GetGreedsGulletHearts = function(self)
        return repentogon.L_EntityPlayer_GetGreedsGulletHearts(self)
    end,
    CanCrushRocks = function(self)
        return repentogon.L_EntityPlayer_CanCrushRocks(self)
    end,
    HasInstantDeathCurse = function(self)
        return repentogon.L_EntityPlayer_HasInstantDeathCurse(self)
    end,
    HasPoisonImmunity = function(self)
        return repentogon.L_EntityPlayer_HasPoisonImmunity(self)
    end,
    IsEntityValidTarget = function(self, target)
        if target == nil then ffichecks.argerror(1, "Entity expected, got nil") end
        target = EntityToPointer(target)
        return repentogon.L_EntityPlayer_IsEntityValidTarget(self, target)
    end,
    IsHeadless = function(self)
        return repentogon.L_EntityPlayer_IsHeadless(self)
    end,
    IsHologram = function(self)
        return repentogon.L_EntityPlayer_IsHologram(self)
    end,
    IsInvisible = function(self)
        return repentogon.L_EntityPlayer_IsInvisible(self)
    end,
    MorphToCoopGhost = function(self)
        repentogon.L_EntityPlayer_MorphToCoopGhost(self)
    end,
    ResetPlayer = function(self)
        repentogon.L_EntityPlayer_ResetPlayer(self)
    end,
    SetControllerIndex = function(self, index, includePlayerOwned)
        ffichecks.checkinteger(1, index)
        includePlayerOwned = ffichecks.checkboolean(2, includePlayerOwned)
        repentogon.L_EntityPlayer_SetControllerIndex(self, index, includePlayerOwned)
    end,
    SyncConsumableCounts = function(self, other, flags)
        other = ffichecks.playertopointer(other)
        ffichecks.checkinteger(2, flags)
        repentogon.L_EntityPlayer_SyncConsumableCounts(self, other, flags)
    end,
    TryAddToBagOfCrafting = function(self, pickup)
        if pickup == nil then ffichecks.argerror(1, "Entity expected, got nil") end
        pickup = EntityToPointer(pickup)
        return repentogon.L_EntityPlayer_TryAddToBagOfCrafting(self, pickup)
    end,
    TryDecreaseGlowingHourglassUses = function(self, unk1, unk2)
        ffichecks.checkinteger(1, unk1)
        unk2 = ffichecks.checkboolean(2, unk2)
        repentogon.L_EntityPlayer_TryDecreaseGlowingHourglassUses(self, unk1, unk2)
    end,
    TryRemoveSmeltedTrinket = function(self, trinket)
        ffichecks.checkinteger(1, trinket)
        repentogon.L_EntityPlayer_TryRemoveSmeltedTrinket(self, trinket)
    end,
    VoidHasCollectible = function(self, collectible)
        ffichecks.checkinteger(1, collectible)
        return repentogon.L_EntityPlayer_VoidHasCollectible(self, collectible)
    end,
    PlayItemNullAnimation = function(self, animName)
        animName = ffichecks.checkstring(1, animName)
        return repentogon.L_EntityPlayer_PlayItemNullAnimation(self, animName)
    end,
    ClearQueueItem = function(self)
        repentogon.L_EntityPlayer_ClearQueueItem(self)
    end,
    RemovePocketItem = function(self, slot)
        ffichecks.checkinteger(1, slot)
        if slot < 0 or slot > 3 then ffichecks.argerror(1, "Invalid slot ID " .. slot) end
        repentogon.L_EntityPlayer_RemovePocketItem(self, slot)
    end,
    IsFootstepFrame = function(self, foot)
        if foot == nil then foot = -1 else ffichecks.checkinteger(1, foot) end
        if foot < -1 or foot > 1 then ffichecks.argerror(1, "Invalid foot ID " .. foot .. ", valid range is -1 to 1") end
        return repentogon.L_EntityPlayer_IsFootstepFrame(self, foot)
    end,
    GetPocketItem = function(self, slot)
        ffichecks.checkinteger(1, slot)
        return repentogon.L_EntityPlayer_GetPocketItem(self, slot)
    end,
    GetHistory = function(self)
        return repentogon.L_EntityPlayer_GetHistory(self)
    end,
    GetCardRNG = function(self, id)
        ffichecks.checkinteger(1, id)
        return repentogon.L_EntityPlayer_GetCardRNG(self, id)
    end,
    GetCollectibleRNG = function(self, id)
        ffichecks.checkinteger(1, id)
        return repentogon.L_EntityPlayer_GetCollectibleRNG(self, id)
    end,
    GetPillRNG = function(self, id)
        ffichecks.checkinteger(1, id)
        return repentogon.L_EntityPlayer_GetPillRNG(self, id)
    end,
    GetTrinketRNG = function(self, id)
        ffichecks.checkinteger(1, id)
        return repentogon.L_EntityPlayer_GetTrinketRNG(self, id)
    end,
    GetBombFlags = function(self, isFetus)
        isFetus = ffichecks.optboolean(isFetus, false)
        local result = BitSet128(0, 0)
        repentogon.L_EntityPlayer_GetBombFlags(self, isFetus, result)
        return result
    end,
    GetActiveItemDesc = function(self, slot)
        slot = CheckActiveSlot(1, slot, false)
        return repentogon.L_EntityPlayer_GetActiveItemDesc(self, slot)
    end,
    GetFocusEntity = function(self)
        return repentogon.L_EntityPlayer_GetFocusEntity(self)
    end,
    SpawnSaturnusTears = function(self)
        return repentogon.L_EntityPlayer_SpawnSaturnusTears(self)
    end,
    SetPocketActiveItem = function(self, collectible, slot, keepInPools)
        ffichecks.checkinteger(1, collectible)
        if slot == nil then slot = 2 else ffichecks.checkinteger(2, slot) end
        keepInPools = ffichecks.optboolean(keepInPools, false)
        if slot ~= 2 and slot ~= 3 then ffichecks.argerror(2, "Invalid ActiveSlot - SetPocketActiveItem can only be used for ActiveSlot.SLOT_POCKET or ActiveSlot.SLOT_POCKET2") end
        repentogon.L_EntityPlayer_SetPocketActiveItem(self, collectible, slot, keepInPools)
    end,
    GetMovementDirection = Getter("MoveDirectionValue"),
    GetFireDirection = Getter("FireDirectionValue"),
    GetHeadDirection = Getter("HeadDirectionValue"),
    AreOpposingShootDirectionsPressed = Getter("OpposingShootDirectionsValue"),
    GetSmoothBodyRotation = Getter("SmoothBodyRotationValue"),
    GetTearPoisonDamage = Getter("TearPoisonDamageValue"),
    SetTearPoisonDamage = NumberSetter("TearPoisonDamageValue"),
    GetHearts = Getter("RedHeartsValue"),
    GetMaxHearts = Getter("MaxHeartsValue"),
    GetSoulHearts = Getter("SoulHeartsValue"),
    GetBlackHearts = Getter("BlackHeartsValue"),
    GetEternalHearts = Getter("EternalHeartsValue"),
    GetJarHearts = Getter("JarHeartsValue"),
    GetJarFlies = Getter("JarFliesValue"),
    GetNumBombs = Getter("NumBombsValue"),
    GetNumKeys = Getter("NumKeysValue"),
    HasGoldenKey = Getter("HasGoldenKeyValue"),
    HasGoldenBomb = Getter("HasGoldenBombValue"),
    GetGoldenHearts = Getter("GoldenHeartsValue"),
    GetNumCoins = Getter("NumCoinsValue"),
    GetPlayerType = Getter("PlayerTypeValue"),
    GetNumBlueFlies = Getter("NumBlueFliesValue"),
    GetNumBlueSpiders = Getter("NumBlueSpidersValue"),
    GetItemState = Getter("ItemStateValue"),
    GetTearRangeModifier = Getter("TearRangeModifierValue"),
    SetTearRangeModifier = IntegerSetter("TearRangeModifierValue"),
    GetTractorBeam = Getter("TractorBeamValue"),
    GetDamageCooldown = Getter("DamageCooldownValue"),
    GetLastActionTriggers = Getter("LastActionTriggersValue"),
    GetBabySkin = function(self)
        return self.BabySkin
    end,
    GetBoneHearts = Getter("BoneHeartsValue"),
    GetSubPlayer = Getter("SubPlayerValue"),
    GetOtherTwin = Getter("TwinPlayerValue"),
    GetBrokenHearts = Getter("BrokenHeartsValue"),
    GetRottenHearts = Getter("RottenHeartsValue"),
    GetSoulCharge = Getter("SoulChargeValue"),
    SetSoulCharge = IntegerSetter("SoulChargeValue"),
    GetBloodCharge = Getter("BloodChargeValue"),
    SetBloodCharge = IntegerSetter("BloodChargeValue"),
    GetNumGigaBombs = Getter("NumGigaBombsValue"),
    GetModelingClayEffect = Getter("ModelingClayEffectValue"),
    HasCurseMistEffect = Getter("CurseMistEffectValue"),
    IsCoopGhost = Getter("IsCoopGhostValue"),
    GetPoopMana = Getter("PoopManaValue"),
    GetBodyColor = Getter("BodyColorValue"),
    GetHeadColor = Getter("HeadColorValue"),
    GetTotalDamageTaken = Getter("TotalDamageTakenValue"),
    CanShoot = Getter("CanShootValue"),
    SetCanShoot = BooleanSetter("CanShootValue"),
    GetSpeedModifier = Getter("SpeedModifierValue"),
    SetSpeedModifier = IntegerSetter("SpeedModifierValue"),
    GetFireDelayModifier = Getter("FireDelayModifierValue"),
    SetFireDelayModifier = IntegerSetter("FireDelayModifierValue"),
    GetDamageModifier = Getter("DamageModifierValue"),
    SetDamageModifier = IntegerSetter("DamageModifierValue"),
    GetShotSpeedModifier = Getter("ShotSpeedModifierValue"),
    SetShotSpeedModifier = IntegerSetter("ShotSpeedModifierValue"),
    GetLuckModifier = Getter("LuckModifierValue"),
    SetLuckModifier = IntegerSetter("LuckModifierValue"),
    GetRedStewBonusDuration = Getter("RedStewBonusDurationValue"),
    SetRedStewBonusDuration = IntegerSetter("RedStewBonusDurationValue"),
    GetD8DamageModifier = Getter("D8DamageModifierValue"),
    SetD8DamageModifier = NumberSetter("D8DamageModifierValue"),
    GetD8SpeedModifier = Getter("D8SpeedModifierValue"),
    SetD8SpeedModifier = NumberSetter("D8SpeedModifierValue"),
    GetD8RangeModifier = Getter("D8RangeModifierValue"),
    SetD8RangeModifier = NumberSetter("D8RangeModifierValue"),
    GetD8FireDelayModifier = Getter("D8FireDelayModifierValue"),
    SetD8FireDelayModifier = NumberSetter("D8FireDelayModifierValue"),
    GetEpiphoraCharge = Getter("EpiphoraChargeValue"),
    GetPeeBurstCooldown = Getter("PeeBurstCooldownValue"),
    GetMaxPeeBurstCooldown = Getter("MaxPeeBurstCooldownValue"),
    GetMetronomeCollectibleID = Getter("MetronomeCollectibleIDValue"),
    GetMarkedTarget = Getter("MarkedTargetValue"),
    GetWildCardItem = Getter("WildCardItemValue"),
    GetWildCardItemType = Getter("WildCardItemTypeValue"),
    GetPonyCharge = Getter("PonyChargeValue"),
    SetPonyCharge = IntegerSetter("PonyChargeValue"),
    GetEdenSpeed = Getter("EdenSpeedValue"),
    SetEdenSpeed = NumberSetter("EdenSpeedValue"),
    GetEdenFireDelay = Getter("EdenFireDelayValue"),
    SetEdenFireDelay = NumberSetter("EdenFireDelayValue"),
    GetEdenDamage = Getter("EdenDamageValue"),
    SetEdenDamage = NumberSetter("EdenDamageValue"),
    GetEdenRange = Getter("EdenTearRangeValue"),
    SetEdenRange = NumberSetter("EdenTearRangeValue"),
    GetEdenShotSpeed = Getter("EdenShotSpeedValue"),
    SetEdenShotSpeed = NumberSetter("EdenShotSpeedValue"),
    GetEdenLuck = Getter("EdenLuckValue"),
    SetEdenLuck = NumberSetter("EdenLuckValue"),
    GetPurityState = Getter("PurityStateValue"),
    SetPurityState = IntegerSetter("PurityStateValue"),
    GetCambionConceptionState = Getter("CambionConceptionStateValue"),
    SetCambionConceptionState = IntegerSetter("CambionConceptionStateValue"),
    GetConceptionFamiliarFlags = Getter("ConceptionFamiliarFlagsValue"),
    SetConceptionFamiliarFlags = IntegerSetter("ConceptionFamiliarFlagsValue"),
    GetBladderCharge = Getter("BladderChargeValue"),
    SetBladderCharge = IntegerSetter("BladderChargeValue"),
    GetMaxBladderCharge = Getter("MaxBladderChargeValue"),
    SetMaxBladderCharge = IntegerSetter("MaxBladderChargeValue"),
    IsUrethraBlocked = Getter("IsUrethraBlockedValue"),
    SetUrethraBlock = BooleanSetter("IsUrethraBlockedValue"),
    GetNextUrethraBlockFrame = Getter("NextUrethraBlockFrameValue"),
    SetNextUrethraBlockFrame = IntegerSetter("NextUrethraBlockFrameValue"),
    GetPlayerIndex = Getter("PlayerIndexValue"),
    GetRevelationCharge = Getter("RevelationChargeTimerValue"),
    SetRevelationCharge = IntegerSetter("RevelationChargeTimerValue"),
    GetMaggySwingCooldown = Getter("MaggySwingCooldownValue"),
    SetMaggySwingCooldown = IntegerSetter("MaggySwingCooldownValue"),
    GetEveSumptoriumCharge = Getter("EveSumptoriumChargeValue"),
    SetEveSumptoriumCharge = IntegerSetter("EveSumptoriumChargeValue"),
    GetUrnSouls = Getter("UrnSoulsValue"),
    GetKeepersSackBonus = Getter("KeepersSackBonusValue"),
    SetKeepersSackBonus = IntegerSetter("KeepersSackBonusValue"),
    GetGnawedLeafTimer = Getter("GnawedLeafTimerValue"),
    SetGnawedLeafTimer = IntegerSetter("GnawedLeafTimerValue"),
    GetBloodLustCounter = Getter("BloodLustCounterValue"),
    SetBloodLustCounter = IntegerSetter("BloodLustCounterValue"),
    GetBombPlaceDelay = Getter("BombPlaceDelayValue"),
    SetBombPlaceDelay = IntegerSetter("BombPlaceDelayValue"),
    GetHeadDirectionLockTime = Getter("HeadDirectionTimeValue"),
    SetHeadDirectionLockTime = IntegerSetter("HeadDirectionTimeValue"),
    GetHallowedGroundCountdown = Getter("HallowedGroundCountdownValue"),
    SetHallowedGroundCountdown = IntegerSetter("HallowedGroundCountdownValue"),
    GetActionHoldDrop = Getter("ActionHoldDropValue"),
    SetActionHoldDrop = IntegerSetter("ActionHoldDropValue"),
    GetForgottenSwapFormCooldown = Getter("ForgottenSwapFormCooldownValue"),
    SetForgottenSwapFormCooldown = IntegerSetter("ForgottenSwapFormCooldownValue"),
    GetRockBottomMoveSpeed = Getter("RockBottomMoveSpeedValue"),
    SetRockBottomMoveSpeed = NumberSetter("RockBottomMoveSpeedValue"),
    GetRockBottomMaxFireDelay = Getter("RockBottomMaxFireDelayValue"),
    SetRockBottomMaxFireDelay = NumberSetter("RockBottomMaxFireDelayValue"),
    GetRockBottomDamage = Getter("RockBottomDamageValue"),
    SetRockBottomDamage = NumberSetter("RockBottomDamageValue"),
    GetRockBottomTearRange = Getter("RockBottomTearRangeValue"),
    SetRockBottomTearRange = NumberSetter("RockBottomTearRangeValue"),
    GetRockBottomShotSpeed = Getter("RockBottomShotSpeedValue"),
    SetRockBottomShotSpeed = NumberSetter("RockBottomShotSpeedValue"),
    GetRockBottomLuck = Getter("RockBottomLuckValue"),
    SetRockBottomLuck = NumberSetter("RockBottomLuckValue"),
    GetSuplexState = Getter("SuplexStateValue"),
    SetSuplexState = IntegerSetter("SuplexStateValue"),
    GetSuplexAimCountdown = Getter("SuplexAimCountdownValue"),
    SetSuplexAimCountdown = IntegerSetter("SuplexAimCountdownValue"),
    GetSuplexTargetPosition = VectorGetter("SuplexTargetPosValue"),
    SetSuplexTargetPosition = VectorSetter("SuplexTargetPosValue"),
    GetSuplexLandPosition = VectorGetter("SuplexLandPosValue"),
    SetSuplexLandPosition = VectorSetter("SuplexLandPosValue"),
    GetCharmOfTheVampireKills = Getter("VampireCharmKillsValue"),
    SetCharmOfTheVampireKills = IntegerSetter("VampireCharmKillsValue"),
    GetMaggyHealthDrainCooldown = Getter("MaggyHealthDrainCooldownValue"),
    SetMaggyHealthDrainCooldown = IntegerSetter("MaggyHealthDrainCooldownValue"),
    IsPostLevelInitFinished = Getter("PostLevelInitFinishedValue"),
    GetPlanCKillCountdown = Getter("PlanCKillCountdownValue"),
    SetPlanCKillCountdown = IntegerSetter("PlanCKillCountdownValue"),
    GetPotatoPeelerUses = Getter("PotatoPeelerCounterValue"),
    SetPotatoPeelerUses = IntegerSetter("PotatoPeelerCounterValue"),
    GetMawOfTheVoidCharge = Getter("MawOfTheVoidChargeTimerValue"),
    SetMawOfTheVoidCharge = IntegerSetter("MawOfTheVoidChargeTimerValue"),
    GetMontezumaRevengeCharge = Getter("MontezumaChargeTimerValue"),
    SetMontezumaRevengeCharge = IntegerSetter("MontezumaChargeTimerValue"),
    GetItemStateCooldown = Getter("ItemStateCooldownValue"),
    SetItemStateCooldown = IntegerSetter("ItemStateCooldownValue"),
    GetImExcitedSpeedupCountdown = Getter("ImExcitedSpeedupCountdownValue"),
    SetImExcitedSpeedupCountdown = IntegerSetter("ImExcitedSpeedupCountdownValue"),
    GetDonateLuck = Getter("DonateLuckValue"),
    SetDonateLuck = IntegerSetter("DonateLuckValue"),
    GetRUAWizardTimer = Getter("RUAWizardTimerValue"),
    SetRUAWizardTimer = IntegerSetter("RUAWizardTimerValue"),
    GetDeadEyeCharge = Getter("DeadEyeChargesValue"),
    GetAimDirection = VectorGetter("AimDirectionValue"),
    GetLastDirection = VectorGetter("LastDirectionValue"),
    GetMovementInput = VectorGetter("MovementInputValue"),
    GetMovementVector = VectorGetter("MovementInputValue"),
    GetRecentMovementVector = VectorGetter("RecentMovementVectorValue"),
    GetVelocityBeforeUpdate = VectorGetter("VelocityBeforeUpdateValue"),
    GetHeldEntity = Getter("HeldEntityValue"),
    GetFlippedForm = Getter("BackupPlayerValue"),
    GetImmaculateConceptionState = Getter("ImmaculateConceptionStateValue"),
    GetMegaBlastDuration = Getter("MegaBlastDurationValue"),
    GetEntityConfigPlayer = function(self)
        return repentogon.L_EntityPlayer_GetEntityConfigPlayer(self)
    end,
    GetWeapon = function(self, index)
        ffichecks.checkinteger(1, index)
        if index < 0 or index > 4 then
            ffichecks.argerror(1, "Index must be between 0 and 4")
        end
        return repentogon.L_EntityPlayer_GetWeapon(self, index)
    end,
    SetWeapon = function(self, weapon, index)
        ffichecks.checkcdata(1, weapon, "Weapon")
        ffichecks.checkinteger(2, index)
        if index < 0 or index > 4 then
            ffichecks.argerror(2, "Index must be between 0 and 4")
        end
        repentogon.L_EntityPlayer_SetWeapon(self, weapon, index)
    end,
    GetActiveWeaponNumFired = function(self)
        local numFired = ffi.new("int[1]")
        if repentogon.L_EntityPlayer_GetActiveWeaponNumFired(self, numFired) then
            return numFired[0]
        end
        return nil
    end,
    GetEffects = function(self)
        return repentogon.L_EntityPlayer_GetEffects(self)
    end,
    GetBodySprite = function(self)
        return repentogon.L_EntityPlayer_GetBodySprite(self)
    end,
    GetBloodGushSprite = function(self)
        return repentogon.L_EntityPlayer_GetBloodGushSprite(self)
    end,
    GetHeldSprite = function(self)
        return repentogon.L_EntityPlayer_GetHeldSprite(self)
    end,
    GetLastDamageSource = function(self)
        return CopyStruct("struct EntityRef", repentogon.L_EntityPlayer_GetLastDamageSource(self))
    end,
    GetMovingBoxContents = function(self)
        return repentogon.L_EntityPlayer_GetMovingBoxContents(self)
    end,
    GetPlayerHUD = function(self)
        return repentogon.L_EntityPlayer_GetPlayerHUD(self)
    end,
    GetCostumeSpriteDescs = function(self)
        return ffichecks.vectortotable(ffi.getprivate(self, "CostumeSpriteDescsFirstValue"), ffi.getprivate(self, "CostumeSpriteDescsLastValue"), COSTUME_SPRITE_DESC_SIZE)
    end,
    GetCollectiblesList = function(self)
        local result = {}
        local first = ffi.getprivate(self, "CollectibleCountsFirstValue")
        for i = 1, ffichecks.vectorsize(first, ffi.getprivate(self, "CollectibleCountsLastValue"), 4) - 1 do
            result[i] = first[i]
        end
        return result
    end,
    GetVoidedCollectiblesList = function(self)
        return ffichecks.vectortotable(ffi.getprivate(self, "VoidedCollectiblesFirstValue"), ffi.getprivate(self, "VoidedCollectiblesLastValue"), 4)
    end,
    GetWispCollectiblesList = function(self)
        return SnapshotPairs(repentogon.L_EntityPlayer_SnapshotWispCollectibles(self))
    end,
    GetPlayerFormCounter = function(self, form)
        ffichecks.checkinteger(1, form)
        if form < 0 or form > 14 then
            ffichecks.argerror(1, string.format("Invalid PlayerForm %d", form))
        end
        return repentogon.L_EntityPlayer_GetPlayerFormCounter(self, form)
    end,
    GetCostumeLayerMap = function(self)
        local result = {}
        local costumeIndex, layerID, priority, isBodyLayer = ffi.new("int[1]"), ffi.new("int[1]"), ffi.new("int[1]"), ffi.new("bool[1]")
        for i = 0, 14 do
            repentogon.L_EntityPlayer_GetCostumeLayer(self, i, costumeIndex, layerID, priority, isBodyLayer)
            result[i + 1] = { costumeIndex = costumeIndex[0], layerID = layerID[0], priority = priority[0], isBodyLayer = isBodyLayer[0] }
        end
        return result
    end,
    GetCandyHeartBonus = function(self)
        return HeartStatUps(self, false)
    end,
    GetSoulLocketBonus = function(self)
        return HeartStatUps(self, true)
    end,
    AddCandyHeartBonus = function(self, cacheFlags, amount)
        cacheFlags = OptInteger(1, cacheFlags, 0)
        amount = OptInteger(2, amount, 1)
        if amount ~= 0 then
            repentogon.L_EntityPlayer_AddCandyHeartSoulLocketBonus(self, false, cacheFlags, amount)
        end
    end,
    AddSoulLocketBonus = function(self, cacheFlags, amount)
        cacheFlags = OptInteger(1, cacheFlags, 0)
        amount = OptInteger(2, amount, 1)
        if amount ~= 0 then
            repentogon.L_EntityPlayer_AddCandyHeartSoulLocketBonus(self, true, cacheFlags, amount)
        end
    end,
    GetSmeltedTrinkets = function(self, ids)
        local result = {}
        if type(ids) == "table" then
            for i = 1, #ids do
                local id = ids[i]
                if id == nil then
                    break
                end
                ffichecks.checkinteger(1, id)
                id = id & TRINKET_ID_MASK
                if repentogon.L_EntityPlayer_IsValidTrinket(id) then
                    result[id] = SmeltedTrinketDesc(self, id)
                end
            end
        else
            local first = ffi.getprivate(self, "SmeltedTrinketsFirstValue")
            for i = 1, ffichecks.vectorsize(first, ffi.getprivate(self, "SmeltedTrinketsLastValue"), 4) - 1 do
                result[i] = { trinketAmount = first[i].TrinketAmount, goldenTrinketAmount = first[i].GoldenTrinketAmount }
            end
        end
        return result
    end,
    GetSmeltedTrinketDesc = function(self, trinket)
        ffichecks.checkinteger(1, trinket)
        trinket = trinket & TRINKET_ID_MASK
        if not repentogon.L_EntityPlayer_IsValidTrinket(trinket) then
            return nil
        end
        return SmeltedTrinketDesc(self, trinket)
    end,
    AddSmeltedTrinket = function(self, trinket, firstTime)
        ffichecks.checkinteger(1, trinket)
        return repentogon.L_EntityPlayer_AddSmeltedTrinket(self, trinket, ffichecks.optboolean(firstTime, true))
    end,

    AddBoneOrbital = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        return repentogon.L_EntityPlayer_AddBoneOrbital(self, position)
    end,
    FireBrimstoneBall = function(self, position, velocity, offset)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, velocity, "Vector")
        ffichecks.checkcdata(3, offset, "Vector", true)
        return repentogon.L_EntityPlayer_FireBrimstoneBall(self, position, velocity, offset)
    end,
    FireTear = function(self, position, velocity, canBeEye, noTractorBeam, canTriggerStreakEnd, source, damageMultiplier)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, velocity, "Vector")
        canBeEye = ffichecks.optboolean(canBeEye, true)
        noTractorBeam = ffichecks.optboolean(noTractorBeam, false)
        canTriggerStreakEnd = ffichecks.optboolean(canTriggerStreakEnd, true)
        damageMultiplier = OptNumber(7, damageMultiplier, 1)
        local flags = (canBeEye and 0x4 or 0) | (noTractorBeam and 0x2 or 0) | (canTriggerStreakEnd and 0x1 or 0)
        return repentogon.L_EntityPlayer_FireTearEx(self, position, velocity, flags, EntityToPointer(source), damageMultiplier)
    end,
    ShootRedCandle = function(self, direction)
        ffichecks.checkcdata(1, direction, "Vector")
        return repentogon.L_EntityPlayer_ShootRedCandle(self, direction)
    end,
    ShootBlueCandle = function(self, direction)
        ffichecks.checkcdata(1, direction, "Vector")
        return repentogon.L_EntityPlayer_ShootBlueCandle(self, direction)
    end,
    ThrowFriendlyDip = function(self, subtype, position, target)
        ffichecks.checkinteger(1, subtype)
        ffichecks.checkcdata(2, position, "Vector")
        if not ffichecks.iscdata(target, "Vector") then
            target = nil
        end
        return repentogon.L_EntityPlayer_ThrowFriendlyDip(self, subtype, position, target)
    end,
    DropCollectible = function(self, collectible, pickup, removeFromPlayerForm)
        ffichecks.checkinteger(1, collectible)
        return repentogon.L_EntityPlayer_DropCollectible(self, collectible, EntityToPointer(pickup), ffichecks.optboolean(removeFromPlayerForm, false))
    end,
    DropCollectibleByHistoryIndex = function(self, index, pickup)
        ffichecks.checkinteger(1, index)
        repentogon.L_EntityPlayer_DropCollectibleByHistoryIndex(self, index, EntityToPointer(pickup))
        return pickup
    end,
    SpawnAquariusCreep = function(self, params)
        ffichecks.checkcdata(1, params, "TearParams", true)
        return repentogon.L_EntityPlayer_SpawnAquariusCreep(self, params)
    end,
    AddLocust = function(self, collectible, position)
        ffichecks.checkinteger(1, collectible)
        ffichecks.checkcdata(2, position, "Vector")
        repentogon.L_EntityPlayer_AddLocust(self, collectible, position)
    end,
    CheckFamiliarEx = function(self, variant, targetCount, rng, item, subtype)
        ffichecks.checkinteger(1, variant)
        ffichecks.checkinteger(2, targetCount)
        ffichecks.checkcdata(3, rng, "RNG")
        ffichecks.checkcdata(4, item, "ItemConfigItem", true)
        subtype = OptInteger(5, subtype, -1)
        local result = {}
        for i = 1, repentogon.L_EntityPlayer_CheckFamiliarEx(self, variant, targetCount, rng, item, subtype) do
            result[i] = repentogon.L_EntityPlayer_GetCheckedFamiliar(i - 1)
        end
        return result
    end,

    QueueItem = function(self, item, charge, touched, golden, varData)
        ffichecks.checkcdata(1, item, "ItemConfigItem")
        charge = OptInteger(2, charge, 0)
        touched = ffichecks.optboolean(touched, false)
        golden = ffichecks.optboolean(golden, false)
        varData = OptInteger(5, varData, 0)
        local flags = 0
        if touched then
            flags = flags + 1
        end
        if golden then
            flags = flags + 2
        end
        repentogon.L_EntityPlayer_QueueItemEx(self, item, charge, flags, varData)
    end,
    AnimatePickup = function(self, sprite, hideShadow, animName)
        ffichecks.checkcdata(1, sprite, "Sprite")
        hideShadow = ffichecks.optboolean(hideShadow, false)
        if type(animName) ~= "string" then
            animName = "Pickup"
        end
        repentogon.L_EntityPlayer_AnimatePickup(self, sprite, hideShadow, animName)
    end,
    ReplaceCostumeSprite = function(self, item, spritePath, spriteId)
        ffichecks.checkcdata(1, item, "ItemConfigItem")
        spritePath = ffichecks.checkstring(2, spritePath)
        ffichecks.checknumber(3, spriteId)
        repentogon.L_EntityPlayer_ReplaceCostumeSprite(self, item, spritePath, spriteId)
    end,
    GetCostumeNullPos = function(self, nullFrameName, headScale, direction)
        nullFrameName = ffichecks.checkstring(1, nullFrameName)
        headScale = ffichecks.checkboolean(2, headScale)
        ffichecks.checkcdata(3, direction, "Vector")
        local result = Vector(0, 0)
        repentogon.L_EntityPlayer_GetCostumeNullPos(self, nullFrameName, headScale, direction, result)
        return result
    end,
    PlayCollectibleAnim = function(self, collectible, checkBodyAnim, animName, frameNum)
        ffichecks.checkinteger(1, collectible)
        checkBodyAnim = ffichecks.checkboolean(2, checkBodyAnim)
        animName = ffichecks.checkstring(3, animName)
        frameNum = OptInteger(4, frameNum, -1)
        repentogon.L_EntityPlayer_PlayCollectibleAnim(self, collectible, checkBodyAnim, animName, frameNum)
    end,
    IsCollectibleAnimFinished = function(self, collectible, animName)
        ffichecks.checkinteger(1, collectible)
        animName = ffichecks.checkstring(2, animName)
        return repentogon.L_EntityPlayer_IsCollectibleAnimFinished(self, collectible, animName)
    end,
    ClearCollectibleAnim = function(self, collectible)
        ffichecks.checkinteger(1, collectible)
        repentogon.L_EntityPlayer_ClearCollectibleAnim(self, collectible)
    end,
    GetMultiShotParams = function(self, weaponType)
        local result = ffi.new("struct MultiShotParams")
        repentogon.L_EntityPlayer_GetMultiShotParams(self, OptInteger(1, weaponType, 1), result)
        return result
    end,
    GetMultiShotPositionVelocity = function(self, loopIndex, weaponType, shotDirection, shotSpeed, multiShotParams)
        ffichecks.checkinteger(1, loopIndex)
        ffichecks.checkinteger(2, weaponType)
        ffichecks.checkcdata(3, shotDirection, "Vector")
        ffichecks.checknumber(4, shotSpeed)
        ffichecks.checkcdata(5, multiShotParams, "MultiShotParams")
        if multiShotParams:GetNumTears() < loopIndex then
            ffichecks.argerror(1, "LoopIndex cannot be higher than MultiShotParams.NumTears")
        end
        local result = ffi.new("struct PosVel")
        repentogon.L_EntityPlayer_GetMultiShotPositionVelocity(self, loopIndex, weaponType, shotDirection, shotSpeed, multiShotParams, result)
        return result
    end,
    GetTearHitParams = function(self, weaponType, damageScale, tearDisplacement, source)
        ffichecks.checkinteger(1, weaponType)
        damageScale = OptNumber(2, damageScale, 1)
        tearDisplacement = OptInteger(3, tearDisplacement, 1)
        local result = ffi.new("struct TearParams")
        repentogon.L_EntityPlayer_GetTearHitParams(self, weaponType, damageScale, tearDisplacement, EntityToPointer(source), result)
        return result
    end,
    GetGlyphOfBalanceDrop = function(self, variant, subtype)
        local variantBuffer = ffi.new("int[1]", OptInteger(1, variant, -1))
        local subtypeBuffer = ffi.new("int[1]", OptInteger(2, subtype, -1))
        repentogon.L_EntityPlayer_GetGlyphOfBalanceDrop(self, variantBuffer, subtypeBuffer)
        return { variantBuffer[0], subtypeBuffer[0] }
    end,
    GetSpecialGridCollision = function(self, position)
        ffichecks.checkcdata(1, position, "Vector", true)
        return repentogon.L_EntityPlayer_GetSpecialGridCollision(self, position)
    end,
    GetFootprintColor = function(self, rightFoot)
        rightFoot = ffichecks.checkboolean(1, rightFoot)
        local result = KColor(0, 0, 0, 0)
        repentogon.L_EntityPlayer_GetFootprintColor(self, rightFoot, result)
        return result
    end,
    SetFootprintColor = function(self, color, unk)
        ffichecks.checkcdata(1, color, "KColor")
        repentogon.L_EntityPlayer_SetFootprintColor(self, color, ffichecks.optboolean(unk, false))
    end,
    GetLaserColor = function(self)
        return CopyStruct("struct Color", ffi.getprivate(self, "LaserColorValue"))
    end,
    SetLaserColor = function(self, color)
        ffichecks.checkcdata(1, color, "Color")
        ffi.setprivate(self, "LaserColorValue", color)
    end,

    HasCollectible = function(self, collectible, ignoreModifiers, ignoreSpoof)
        ffichecks.checkinteger(1, collectible)
        return repentogon.L_EntityPlayer_HasCollectible(self, collectible, ffichecks.optboolean(ignoreModifiers, false), ffichecks.optboolean(ignoreSpoof, false))
    end,
    GetCollectibleNum = function(self, collectible, onlyCountTrueItems, ignoreSpoof)
        ffichecks.checkinteger(1, collectible)
        return repentogon.L_EntityPlayer_GetCollectibleNum(self, collectible, ffichecks.optboolean(onlyCountTrueItems, false), ffichecks.optboolean(ignoreSpoof, false))
    end,
    HasTrinket = function(self, trinket, ignoreModifiers, ignoreSpoof)
        ffichecks.checkinteger(1, trinket)
        return repentogon.L_EntityPlayer_HasTrinket(self, trinket, ffichecks.optboolean(ignoreModifiers, false), ffichecks.optboolean(ignoreSpoof, false))
    end,
    GetTrinketMultiplier = function(self, trinket, ignoreSpoof)
        ffichecks.checkinteger(1, trinket)
        return repentogon.L_EntityPlayer_GetTrinketMultiplier(self, trinket, ffichecks.optboolean(ignoreSpoof, false))
    end,
    HasGoldenTrinket = function(self, trinket, ignoreSpoof)
        ffichecks.checkinteger(1, trinket)
        return repentogon.L_EntityPlayer_HasGoldenTrinket(self, trinket, ffichecks.optboolean(ignoreSpoof, false))
    end,
    AddCacheFlags = function(self, flags, evaluateItems)
        ffichecks.checkinteger(1, flags)
        evaluateItems = ffichecks.optboolean(evaluateItems, false)
        ffi.setprivate(self, "CacheFlagsValue", ffi.getprivate(self, "CacheFlagsValue") | flags)
        if evaluateItems then
            repentogon.L_EntityPlayer_EvaluateItems(self)
        end
    end,
    UseActiveItem = function(self, collectible, ...)
        ffichecks.checkinteger(1, collectible)
        local useFlags, activeSlot, varData = 0, -1, 0
        local first = ...
        if type(first) == "number" then
            local _, slot, data = ...
            useFlags = first
            ffichecks.checkinteger(2, useFlags)
            if slot then
                ffichecks.checkinteger(3, slot)
                activeSlot = slot
            end
            if data then
                ffichecks.checkinteger(4, data)
                varData = data
            end
        else
            local showAnim, keepActive, allowNonMain, addCostume, slot, customVarData = ...
            if showAnim == false then
                useFlags = useFlags | USE_NOANIM
            end
            if keepActive == false then
                useFlags = useFlags | USE_REMOVEACTIVE
            end
            if allowNonMain then
                useFlags = useFlags | USE_ALLOWNONMAIN
            end
            if addCostume == false then
                useFlags = useFlags | USE_NOCOSTUME
            end
            if slot then
                ffichecks.checkinteger(6, slot)
                activeSlot = slot
            end
            if customVarData then
                useFlags = useFlags | USE_CUSTOMVARDATA
                ffichecks.checkinteger(7, customVarData)
                varData = customVarData
            end
        end
        return repentogon.L_EntityPlayer_UseActiveItem(self, collectible, useFlags, activeSlot, varData)
    end,
    HasInvincibility = function(self, damageFlags, source)
        if damageFlags == nil then
            damageFlags = 0
        else
            damageFlags = CheckFlags64(1, damageFlags)
        end
        ffichecks.checkcdata(2, source, "EntityRef", true)
        return repentogon.L_EntityPlayer_HasInvincibility(self, damageFlags, source)
    end,
    ClearDeadEyeCharge = function(self, reset)
        if reset then
            repentogon.L_EntityPlayer_ClearDeadEyeChargeNow(self)
        else
            repentogon.L_EntityPlayer_ClearDeadEyeChargeNative(self)
        end
    end,
    ShuffleCostumes = function(self, seed)
        if seed ~= nil then
            ffichecks.checkinteger(1, seed)
        end
        repentogon.L_EntityPlayer_ShuffleCostumes(self, seed ~= nil, seed or 0)
    end,
    RerollAllCollectibles = function(self, rng, includeActives)
        ffichecks.checkcdata(1, rng, "RNG", true)
        repentogon.L_EntityPlayer_RerollAllCollectibles(self, rng, ffichecks.optboolean(includeActives, false))
    end,
    SalvageCollectible = function(self, source, ...)
        if type(source) == "number" then
            local position, rng, pool = ...
            ffichecks.checkinteger(1, source)
            ffichecks.checkcdata(2, position, "Vector", true)
            ffichecks.checkcdata(3, rng, "RNG", true)
            repentogon.L_EntityPlayer_SalvageCollectibleType(self, source, position, rng, ValidatePool(4, pool))
        else
            local rng, pool = ...
            ffichecks.checkcdata(1, source, "EntityPickup")
            ffichecks.checkcdata(2, rng, "RNG", true)
            repentogon.L_EntityPlayer_SalvageCollectibleEntity(self, source, rng, ValidatePool(3, pool))
        end
    end,
    AddNullCostume = function(self, nullItem)
        ffichecks.checkinteger(1, nullItem)
        local maxId = repentogon.L_EntityPlayer_AddNullCostume(self, nullItem)
        if maxId >= 0 then
            ffichecks.argerror(1, string.format("Invalid null item id %d, valid range is 0 to %d", nullItem, maxId))
        end
    end,
    SetBlackHeart = function(self, heart)
        ffichecks.checkinteger(1, heart)
        repentogon.L_EntityPlayer_SetBlackHeart(self, heart)
    end,
    HasChanceRevive = function(self)
        return repentogon.L_EntityPlayer_HasChanceRevive(self)
    end,
    ReviveCoopGhost = function(self)
        return repentogon.L_EntityPlayer_ReviveCoopGhost(self)
    end,
    IsPacifist = function(self)
        return repentogon.L_EntityPlayer_IsPacifist(self)
    end,
    IsLocalPlayer = function(self)
        return repentogon.L_EntityPlayer_IsLocalPlayer(self)
    end,
    GetErrorTrinketEffect = function(self)
        return repentogon.L_EntityPlayer_GetErrorTrinketEffect(self)
    end,
    IsItemCostumeVisible = function(self, item, layer)
        ffichecks.checkcdata(1, item, "ItemConfigItem")
        return repentogon.L_EntityPlayer_IsItemCostumeVisibleEx(self, item, LayerId(self, 2, layer))
    end,
    IsCollectibleCostumeVisible = function(self, collectible, layer)
        ffichecks.checkinteger(1, collectible)
        return repentogon.L_EntityPlayer_IsCollectibleCostumeVisibleEx(self, collectible, LayerId(self, 2, layer))
    end,
    IsNullItemCostumeVisible = function(self, nullItem, layer)
        ffichecks.checkinteger(1, nullItem)
        return repentogon.L_EntityPlayer_IsNullItemCostumeVisibleEx(self, nullItem, LayerId(self, 2, layer))
    end,
    SetHeadDirection = function(self, direction, time, force)
        ffichecks.checkinteger(1, direction)
        ffichecks.checkinteger(2, time)
        force = ffichecks.optboolean(force, false)
        if direction < 0 or direction > 3 then
            ffichecks.argerror(1, "Invalid Direction")
        end
        repentogon.L_EntityPlayer_SetHeadDirection(self, direction, time, force)
    end,
    SetPoopSpell = function(self, position, spell)
        ffichecks.checkinteger(1, position)
        ffichecks.checkinteger(2, spell)
        if position < 0 or position > 5 then
            ffichecks.argerror(1, "Invalid Poop Spell queue position")
        end
        if spell < 1 or spell > 11 then
            ffichecks.argerror(2, "Invalid PoopSpellType")
        end
        repentogon.L_EntityPlayer_SetPoopSpell(self, position, spell)
    end,
    RemovePoopSpell = function(self, position)
        position = OptInteger(1, position, 0)
        if position < 0 or position > 5 then
            ffichecks.argerror(1, "Invalid Poop Spell queue position")
        end
        repentogon.L_EntityPlayer_RemovePoopSpell(self, position)
    end,
    SetMegaBlastDuration = function(self, duration)
        ffichecks.checkinteger(1, duration)
        repentogon.L_EntityPlayer_SetMegaBlastDuration(self, duration)
    end,
    SetImmaculateConceptionState = function(self, state)
        ffichecks.checkinteger(1, state)
        ffi.setprivate(self, "ImmaculateConceptionStateValue", math.max(0, math.min(14, state)))
    end,
    SetTearDisplacement = function(self, displacement)
        ffichecks.checkinteger(1, displacement)
        if displacement < -1 or displacement > 1 then
            ffichecks.argerror(1, "TearDisplacement may only be set to -1 or 1")
        end
        ffi.setprivate(self, "TearDisplacementValue", displacement)
    end,
    GetTearDisplacement = Getter("TearDisplacementValue"),
    CreateAfterimage = function(self, duration, position)
        ffichecks.checkinteger(1, duration)
        ffichecks.checkcdata(2, position, "Vector")
        repentogon.L_EntityPlayer_CreateAfterimage(self, duration, position)
    end,
    AddCollectibleEffect = function(self, id, costume, cooldown, additive)
        ffichecks.checkinteger(1, id)
        costume = ffichecks.checkboolean(2, costume)
        repentogon.L_EntityPlayer_AddCollectibleEffect(self, id, costume, OptInteger(3, cooldown, NO_COOLDOWN), ffichecks.optboolean(additive, true))
    end,
    AddNullItemEffect = function(self, id, costume, cooldown, additive)
        ffichecks.checkinteger(1, id)
        costume = ffichecks.checkboolean(2, costume)
        repentogon.L_EntityPlayer_AddNullItemEffect(self, id, costume, OptInteger(3, cooldown, NO_COOLDOWN), ffichecks.optboolean(additive, true))
    end,
    AddTrinketEffect = function(self, id, costume, cooldown, additive)
        ffichecks.checkinteger(1, id)
        costume = ffichecks.checkboolean(2, costume)
        repentogon.L_EntityPlayer_AddTrinketEffect(self, id, costume, OptInteger(3, cooldown, NO_COOLDOWN), ffichecks.optboolean(additive, true))
    end,
    BlockCollectible = function(self, collectible)
        ffichecks.checkinteger(1, collectible)
        if not repentogon.L_EntityPlayer_BlockCollectible(self, collectible) then
            ffichecks.argerror(1, string.format("Invalid CollectibleType %d", collectible))
        end
    end,
    UnblockCollectible = function(self, collectible)
        ffichecks.checkinteger(1, collectible)
        if not repentogon.L_EntityPlayer_UnblockCollectible(self, collectible) then
            ffichecks.argerror(1, string.format("Invalid CollectibleType %d", collectible))
        end
    end,
    IsCollectibleBlocked = function(self, collectible)
        ffichecks.checkinteger(1, collectible)
        return repentogon.L_EntityPlayer_IsCollectibleBlocked(self, collectible)
    end,
    BlockTrinket = function(self, trinket)
        ffichecks.checkinteger(1, trinket)
        if not repentogon.L_EntityPlayer_BlockTrinket(self, trinket) then
            ffichecks.argerror(1, string.format("Invalid TrinketType %d", trinket))
        end
    end,
    UnblockTrinket = function(self, trinket)
        ffichecks.checkinteger(1, trinket)
        if not repentogon.L_EntityPlayer_UnblockTrinket(self, trinket) then
            ffichecks.argerror(1, string.format("Invalid TrinketType %d", trinket))
        end
    end,
    IsTrinketBlocked = function(self, trinket)
        ffichecks.checkinteger(1, trinket)
        return repentogon.L_EntityPlayer_IsTrinketBlocked(self, trinket)
    end,
    AddInnateCollectible = function(self, collectible, amount, groupKey, duration, addCostume)
        ffichecks.checkinteger(1, collectible)
        CheckInnateId(false, collectible)
        amount = OptInteger(2, amount, 1)
        local group = ""
        if groupKey ~= nil and type(groupKey) ~= "boolean" then
            group = ffichecks.checkstring(3, groupKey)
        end
        local newFeatures = type(groupKey) == "string" or type(groupKey) == "number" or type(duration) == "number"
        duration = OptInteger(4, duration, -1)
        addCostume = ffichecks.optboolean(addCostume, true)

        if amount < 0 then
            local deprecationWarning = "Removing innate collectibles with AddInnateCollectible via a negative `Amount` is deprecated. Please use RemoveInnateCollectible instead."
            if newFeatures then
                ffichecks.argerror(2, deprecationWarning)
            elseif not printedNegativeInnateWarning then
                printedNegativeInnateWarning = true
                Console.PrintWarning(string.format("[WARN] %s\n%s\n", deprecationWarning, traceback("", 2)))
            end
            repentogon.L_EntityPlayer_RemoveInnateItemLegacy(self, collectible, -amount)
            return
        end

        repentogon.L_EntityPlayer_AddInnateItem(self, false, collectible, amount, group, duration, addCostume)
    end,
    AddInnateTrinket = function(self, trinket, amount, groupKey, duration, addCostume)
        ffichecks.checkinteger(1, trinket)
        CheckInnateId(true, trinket)
        amount = OptInteger(2, amount, 1)
        local group = ffichecks.optstring(groupKey, "")
        duration = OptInteger(4, duration, -1)
        addCostume = ffichecks.optboolean(addCostume, true)
        repentogon.L_EntityPlayer_AddInnateItem(self, true, trinket, amount, group, duration, addCostume)
    end,
    RemoveInnateCollectible = function(self, collectible, amount, groupKey)
        ffichecks.checkinteger(1, collectible)
        CheckInnateId(false, collectible)
        return repentogon.L_EntityPlayer_RemoveInnateItem(self, false, collectible, OptInteger(2, amount, 1), ffichecks.optstring(groupKey, ""))
    end,
    RemoveInnateTrinket = function(self, trinket, amount, groupKey)
        ffichecks.checkinteger(1, trinket)
        CheckInnateId(true, trinket)
        return repentogon.L_EntityPlayer_RemoveInnateItem(self, true, trinket, OptInteger(2, amount, 1), ffichecks.optstring(groupKey, ""))
    end,
    GetInnateCollectibleCount = function(self, collectible, groupKey)
        ffichecks.checkinteger(1, collectible)
        CheckInnateId(false, collectible)
        return repentogon.L_EntityPlayer_GetInnateItemCount(self, false, collectible, ffichecks.optstring(groupKey, ""))
    end,
    GetInnateTrinketCount = function(self, trinket, groupKey)
        ffichecks.checkinteger(1, trinket)
        CheckInnateId(true, trinket)
        return repentogon.L_EntityPlayer_GetInnateItemCount(self, true, trinket, ffichecks.optstring(groupKey, ""))
    end,
    SetInnateCollectibleCount = function(self, collectible, count, groupKey, addCostume)
        ffichecks.checkinteger(1, collectible)
        CheckInnateId(false, collectible)
        return repentogon.L_EntityPlayer_SetInnateItemCount(self, false, collectible, OptInteger(2, count, 1), ffichecks.optstring(groupKey, ""), ffichecks.optboolean(addCostume, true))
    end,
    SetInnateTrinketCount = function(self, trinket, count, groupKey, addCostume)
        ffichecks.checkinteger(1, trinket)
        CheckInnateId(true, trinket)
        return repentogon.L_EntityPlayer_SetInnateItemCount(self, true, trinket, OptInteger(2, count, 1), ffichecks.optstring(groupKey, ""), ffichecks.optboolean(addCostume, true))
    end,
    GetInnateCollectibleGroup = function(self, groupKey)
        return SnapshotPairs(repentogon.L_EntityPlayer_SnapshotInnateGroup(self, false, ffichecks.optstring(groupKey, "")))
    end,
    GetInnateTrinketGroup = function(self, groupKey)
        return SnapshotPairs(repentogon.L_EntityPlayer_SnapshotInnateGroup(self, true, ffichecks.optstring(groupKey, "")))
    end,
    SetInnateCollectibleGroup = function(self, groupKey, items, addCostumes)
        SetInnateGroup(self, false, groupKey, items, addCostumes)
    end,
    SetInnateTrinketGroup = function(self, groupKey, items, addCostumes)
        SetInnateGroup(self, true, groupKey, items, addCostumes)
    end,
    ClearInnateItemGroup = function(self, groupKey)
        groupKey = ffichecks.checkstring(1, groupKey)
        repentogon.L_EntityPlayer_ClearInnateItemGroup(self, groupKey)
    end,
    GetSpoofedCollectiblesList = function(self)
        local result = {}
        local id, appendedCount, isBlocked = ffi.new("int[1]"), ffi.new("int[1]"), ffi.new("bool[1]")
        for i = 0, repentogon.L_EntityPlayer_SnapshotSpoofedCollectibles(self) - 1 do
            repentogon.L_EntityPlayer_GetSpoofedCollectible(i, id, appendedCount, isBlocked)
            result[id[0]] = { CollectibleID = id[0], AppendedCount = appendedCount[0], IsBlocked = isBlocked[0] }
        end
        return result
    end,

    GetBagOfCraftingContent = function(self)
        local result = {}
        for i = 0, 7 do
            result[i + 1] = repentogon.L_EntityPlayer_GetBagOfCraftingSlot(self, i)
        end
        return result
    end,
    SetBagOfCraftingContent = function(self, content)
        if not ffichecks.istable(content) then
            ffichecks.argerror(1, "Expected a table")
        end
        local length = #content
        if length > 8 then
            ffichecks.argerror(1, "Table cannot be larger than 8 pickups")
        end
        local pickups = ffi.new("int[8]")
        for i = 1, length do
            local pickup = content[i]
            ffichecks.checkinteger(1, pickup)
            if pickup < 0 or pickup > 29 then
                ffichecks.argerror(1, string.format("Invalid pickup %d at index %d", pickup, i))
            end
            pickups[i - 1] = pickup
        end
        repentogon.L_EntityPlayer_SetBagOfCraftingContent(self, pickups)
    end,
    GetBagOfCraftingSlot = function(self, slot)
        ffichecks.checkinteger(1, slot)
        if slot < 0 or slot > 7 then
            ffichecks.argerror(1, string.format("invalid slot id %d", slot))
        end
        return repentogon.L_EntityPlayer_GetBagOfCraftingSlot(self, slot)
    end,
    SetBagOfCraftingSlot = function(self, slot, pickup)
        ffichecks.checkinteger(1, slot)
        if slot < 0 or slot > 7 then
            ffichecks.argerror(1, string.format("invalid slot id %d", slot))
        end
        pickup = OptInteger(2, pickup, 0)
        if pickup < 0 or pickup >= 30 then
            ffichecks.argerror(2, string.format("invalid pickup id %d", pickup))
        end
        repentogon.L_EntityPlayer_SetBagOfCraftingSlot(self, slot, pickup)
    end,
    GetBagOfCraftingOutput = function(self)
        return repentogon.L_EntityPlayer_GetBagOfCraftingOutput(self)
    end,
    GetBagOfCraftingOutputItemPool = function(self)
        return repentogon.L_EntityPlayer_GetBagOfCraftingOutputItemPool(self)
    end,
    SetBagOfCraftingOutput = function(self, collectible, itemPool)
        ffichecks.checkinteger(1, collectible)
        repentogon.L_EntityPlayer_SetBagOfCraftingOutput(self, collectible, OptInteger(2, itemPool, -1))
    end,

    AddCustomCacheTag = function(self, tags, evaluateItems)
        if type(tags) == "table" then
            for i = 1, #tags do
                if tags[i] == nil then
                    break
                end
                repentogon.L_EntityPlayer_AddCustomCacheTag(self, ffichecks.checkstring(1, tags[i]))
            end
        else
            repentogon.L_EntityPlayer_AddCustomCacheTag(self, ffichecks.checkstring(1, tags))
        end
        if ffichecks.optboolean(evaluateItems, false) then
            repentogon.L_EntityPlayer_EvaluateItems(self)
        end
    end,
    GetCustomCacheValue = function(self, tag)
        tag = ffichecks.checkstring(1, tag)
        return repentogon.L_EntityPlayer_GetCustomCacheValue(self, tag)
    end,
    GetTearsCap = function(self)
        return repentogon.L_EntityPlayer_GetCustomCacheValue(self, "tearscap")
    end,
    GetStatMultiplier = function(self)
        return repentogon.L_EntityPlayer_GetCustomCacheValue(self, "statmultiplier")
    end,
    GetMaxCoins = function(self)
        return repentogon.L_EntityPlayer_GetMaxCoins()
    end,
    GetMaxKeys = function(self)
        return repentogon.L_EntityPlayer_GetMaxKeys()
    end,
    GetMaxBombs = function(self)
        return repentogon.L_EntityPlayer_GetMaxBombs()
    end,
    HasForcedCamoEffect = function(self)
        return repentogon.L_EntityPlayer_IsForceCamo(self)
    end,
    SetForceCamoEffect = function(self, forced)
        forced = ffichecks.checkboolean(1, forced)
        repentogon.L_EntityPlayer_SetForceCamo(self, forced)
    end,
    HasCamoEffect = function(self)
        return repentogon.L_EntityPlayer_HasCamoEffect(self)
    end,
    GetMaxInventorySize = function(self)
        return repentogon.L_EntityPlayer_GetMaxInventorySize(self)
    end,
    GetInventoryHistoryIndex = function(self, slot)
        ffichecks.checkinteger(1, slot)
        if slot < 0 or slot >= repentogon.L_EntityPlayer_GetMaxInventorySize(self) then
            ffichecks.argerror(1, string.format("Invalid slot index %d", slot))
        end
        local index = repentogon.L_EntityPlayer_GetInventoryHistoryIndex(self, slot)
        if index >= 0 then
            return index
        end
        return nil
    end,
    GetInventoryCollectible = function(self, slot)
        ffichecks.checkinteger(1, slot)
        if slot < 0 or slot >= repentogon.L_EntityPlayer_GetMaxInventorySize(self) then
            ffichecks.argerror(1, string.format("Invalid slot index %d", slot))
        end
        if repentogon.L_EntityPlayer_GetInventoryHistoryIndex(self, slot) >= 0 then
            return repentogon.L_EntityPlayer_GetInventoryCollectible(self, slot)
        end
        return nil
    end,
    GetBombVariant = function(self, flags, forceSmall)
        ffichecks.checkcdata(1, flags, "BitSet128")
        ffichecks.checkboolean(2, forceSmall)
        return 0
    end,

    GetName = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "NameValue"))
    end,
    HasFullHeartsAndSoulHearts = function(self)
        return ffi.getprivate(self, "SoulHeartsValue") + ffi.getprivate(self, "RedHeartsValue") >= ffi.getprivate(self, "MaxHeartsValue")
    end,
    RemoveBlueSpider = function(self)
        ffi.setprivate(self, "NumBlueSpidersValue", ffi.getprivate(self, "NumBlueSpidersValue") - 1)
    end,
    RemoveBlueFly = function(self)
        ffi.setprivate(self, "NumBlueFliesValue", ffi.getprivate(self, "NumBlueFliesValue") - 1)
    end,
    IsItemQueueEmpty = function(self)
        return ffi.getprivate(self, "QueuedItemValue").Item == nil
    end,
    GetTrinket = function(self, slot)
        ffichecks.checkinteger(1, slot)
        if slot == 0 then
            return ffi.getprivate(self, "Trinket0Value")
        elseif slot == 1 then
            return ffi.getprivate(self, "Trinket1Value")
        end
        return 0
    end,
    IsExtraAnimationFinished = function(self)
        return not (ffi.getprivate(self, "ExtraAnimationAValue") or ffi.getprivate(self, "ExtraAnimationBValue"))
    end,
    StopExtraAnimation = function(self)
        ffi.setprivate(self, "ExtraAnimationAValue", false)
        ffi.setprivate(self, "ExtraAnimationBValue", false)
    end,
    AddControlsCooldown = function(self, cooldown)
        ffichecks.checkinteger(1, cooldown)
        self.ControlsCooldown = self.ControlsCooldown + cooldown
    end,
    ResetDamageCooldown = function(self)
        ffi.setprivate(self, "DamageCooldownValue", 0)
    end,
    GetLastDamageFlags = function(self)
        return Flags64(ffi.getprivate(self, "LastDamageFlagsValue"))
    end,
    IsP2Appearing = function(self)
        return ffi.getprivate(self, "VariantValue") == 1 and ffi.getprivate(self, "DamageCooldownValue") > 0
    end,
    CanTurnHead = function(self)
        return ffi.getprivate(self, "HeadDirectionTimeValue") < 0
    end,
    IsSubPlayer = function(self)
        return ffi.getprivate(self, "PlayerIndexValue") < 0
    end,
    GetBlinkLockTime = function(self)
        return self.HeadFrameDelay
    end,
    SetBlinkLockTime = function(self, time)
        ffichecks.checkinteger(1, time)
        self.HeadFrameDelay = time
    end,
}

methods.GetShootingInput = methods.GetShootingJoystick
methods.GetMaxPoketItems = methods.GetMaxPocketItems
methods.DropPoketItem = methods.DropPocketItem
methods.ClearItemAnimCollectible = methods.ClearCollectibleAnim

local PlayerMT = Entity.Inherit("EntityPlayer", methods, getters, setters)
ffi.metatype("struct EntityPlayer", PlayerMT)
Entity.SetClassType(TYPE_PLAYER, ffi.typeof("struct EntityPlayer*"))

EntityPlayer = setmetatable({
    CalculateBagOfCraftingOutput = function(pickups)
        ffichecks.checktable(1, pickups)
        if #pickups ~= 8 then
            ffichecks.argerror(1, string.format("Expected 8 pickups, got %d", #pickups))
        end
        local buffer = ffi.new("int[8]")
        for i = 1, 8 do
            ffichecks.checkinteger(1, pickups[i])
            if pickups[i] < 0 or pickups[i] > 29 then
                ffichecks.argerror(1, string.format("Invalid pickup %d at index %d", pickups[i], i))
            end
            buffer[i - 1] = pickups[i]
        end
        local collectible, itemPool = ffi.new("int[1]"), ffi.new("int[1]")
        repentogon.L_EntityPlayer_CalculateBagOfCraftingOutput(buffer, collectible, itemPool)
        return collectible[0], itemPool[0]
    end,
}, { __class = PlayerMT })
