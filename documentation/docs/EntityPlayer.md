# Class "EntityPlayer"

This class contains both new functions and modified reimplementations of existing ones.

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Modified Functions

### Add·Cache·Flags () {: aria-label='Modified Functions' }
#### void AddCacheFlags ( [CacheFlag](https://wofsauge.github.io/IsaacDocs/rep/enums/CacheFlag.html) CacheFlag, boolean EvaluateItems = false ) {: .copyable aria-label='Modified Functions' }
Now accepts an optional `bool` to determine if [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html):EvaluateItems() should be automatically called after adding cache flags. In most cases, you'll want to do this.

___
### Clear·Dead·Eye·Charge () {: aria-label='Modified Functions' }
#### void ClearDeadEyeCharge ( boolean Force = false ) {: .copyable aria-label='Modified Functions' }
Now accepts a `Force` argument to forcefully reset the charge instead of only rolling for a change to reset.

___
### Get·Multi·Shot·Params () {: aria-label='Modified Functions' }
#### [MultiShotParams](MultiShotParams.md) GetMultiShotParams ( [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) WeaponType ) {: .copyable aria-label='Modified Functions' }
Now returns a proper `MultiShotParams` object.

___
### Get·Multi·Shot·Position·Velocity () {: aria-label='Modified Functions' }
#### [PosVel](https://wofsauge.github.io/IsaacDocs/rep/PosVel.html) GetMultiShotPositionVelocity ( int LoopIndex, [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) WeaponType, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) ShotDirection, float ShotSpeed, [MultiShotParams](MultiShotParams.md) Params ) {: .copyable aria-label='Modified Functions' }
This magically vanished from the API some time after 1.7.8.

Compared to the vanilla function, this implementation has been further augmented to throw an error if LoopIndex is higher than [MultiShotParams:GetNumTears()](MultiShotParams.md#getnumtears).
### Get·Pocket·Item () {: aria-label='Modified Functions' }
#### [PocketItem](PocketItem.md) GetPocketItem ( int SlotId ) {: .copyable aria-label='Modified Functions' }
Now returns a proper `PocketItem` object.

___

## Modified Variables
___
### Baby·Skin {: aria-label='Modified Variables' }
[ ](#){: .tooltip .badge }
#### [BabySubType](https://wofsauge.github.io/IsaacDocs/rep/enums/BabySubType.html) BabySkin  {: .copyable aria-label='Modified Variables' }
Same as default, but now returns a proper integer value instead of userdata.

___

## Functions

### Add·Active·Charge () {: aria-label='Functions' }
#### int AddActiveCharge ( int Charge, [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot, boolean Unknown, boolean Overcharge, boolean Force ) {: .copyable aria-label='Functions' }

Returns the true amount of charge added, which may have been capped by the targeted item's MaxCharge.

___
### Add·Bone·Orbital () {: aria-label='Functions' }
#### void AddBoneOrbital ( ) {: .copyable aria-label='Functions' }

___
### Add·Innate·Collectible () {: aria-label='Functions' }
#### void AddInnateCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) CollectibleType, int amount = 1 ) {: .copyable aria-label='Functions' }

???+ bug "Bug"
	Currently this function directly modifies the contents of WispCollectiblesList, so if this list was updated on wisp init/delete, or the player exits the run, your added innate items will not be saved.
	
___
### Add·Leprocy () {: aria-label='Functions' }
#### void AddLeprocy ( ) {: .copyable aria-label='Functions' }

???+ info "Info"
    This is currently still capped at a max of three familiars, and would require further modification to change this.

___
### Add·Locust () {: aria-label='Functions' }
#### int AddLocust ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) CollectibleType, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position ) {: .copyable aria-label='Functions' }

Returns the true amount of charge added, which may have been capped by the targeted item's MaxCharge.

___
### Add·Smelted·Trinket () {: aria-label='Functions' }
#### boolean AddSmeltedTrinket ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) TrinketID, boolean FirstTimePickingUp = true ) {: .copyable aria-label='Functions' }
Adds a smelted trinket directly to the player's inventory.

Returns ``true`` if the trinket was successfully added, otherwise ``false``.

___
### Add·Urn·Souls () {: aria-label='Functions' }
#### void AddUrnSouls ( int count = 0 ) {: .copyable aria-label='Functions' }

___
### Attach·Minecart () {: aria-label='Functions' }
#### void AttachMinecart ( [EntityNPC](EntityNPC.md) minecart ) {: .copyable aria-label='Functions' }     

___
### Can·Add·Collectible·To·Inventory () {: aria-label='Functions' }
#### boolean CanAddCollectibleToInventory ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) id ) {: .copyable aria-label='Functions' } 

___
### Can·Crush·Rocks () {: aria-label='Functions' }
#### boolean CanCrushRocks ( ) {: .copyable aria-label='Functions' }

___
### Can·Override·Active·Item () {: aria-label='Functions' }
#### boolean CanOverrideActiveItem ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) id ) {: .copyable aria-label='Functions' }        

___
### Can·Use·Pill () {: aria-label='Functions' }
#### boolean CanUsePill ( [PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.html) ID ) {: .copyable aria-label='Functions' }
By some conditions, usually health-related, determines whether a player can use a given pill effect.

___
### Check·Familiar·Ex () {: aria-label='Functions' }
#### table CheckFamiliarEx ( int FamiliarVariant, int TargetCount, [RNG](https://wofsauge.github.io/IsaacDocs/rep/RNG.html) rng, [ItemConfigItem](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html) SourceItemConfigItem = nil, int FamiliarSubType = -1 ) {: .copyable aria-label='Functions' }

A version of [CheckFamiliar](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html#checkfamiliar) that returns all familiars spawned by the function as a table.

___
### Clear·Collectible·Anim () {: aria-label='Functions' }
#### void ClearCollectibleAnim ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible) {: .copyable aria-label='Functions' }

___
### Clear·Item·Anim·Collectible () {: aria-label='Functions' }
#### void ClearItemAnimCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) id ) {: .copyable aria-label='Functions' }

___
### Clear·Item·Anim·Null·Items () {: aria-label='Functions' }
#### void ClearItemAnimNullItems ( ) {: .copyable aria-label='Functions' }

___
### Drop·Collectible () {: aria-label='Functions' }
#### void DropCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, EntityPickup ExistingPedestal = nil, bool RemoveFromPlayerForm = false ) {: .copyable aria-label='Functions' }

If `ExistingPedestal` is set, the collectible it contains will be swapped out for the dropped collectible instead of a new pedestal spawning.

___
### Drop·Collectible·By·History·Index () {: aria-label='Functions' }
#### [EntityPickup](EntityPickup.md) DropCollectibleByHistoryIndex ( int idx, [EntityPickup](EntityPickup.md) pickup = nil ) {: .copyable aria-label='Functions' }

___
### Enable·Weapon·Type() {: aria-label='Functions' }
#### void EnableWeaponType ( [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) WeaponType, boolean Set ) {: .copyable aria-label='Functions' }

___
### Get·Active·Item·Desc () {: aria-label='Functions' }
#### [ActiveItemDesc](https://wofsauge.github.io/IsaacDocs/rep/PlayerTypes_ActiveItemDesc.html) GetActiveItemDesc ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot = ActiveSlot.SLOT_PRIMARY ) {: .copyable aria-label='Functions' }

___
### Get·Active·Item·Slot () {: aria-label='Functions' }
#### [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) GetActiveItemSlot ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }

___
### Get·Active·Max·Charge () {: aria-label='Functions' }
#### int GetActiveMaxCharge ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### Get·Active·Min·Usable·Charge () {: aria-label='Functions' }
#### int GetActiveMinUsableCharge ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### Get·Active·Weapon·Num·Fired () {: aria-label='Functions' }
#### int GetActiveWeaponNumFired ( ) {: .copyable aria-label='Functions' }

___
### Get·Bag·Of·Crafting·Content () {: aria-label='Functions' }
#### table GetBagOfCraftingContent ( ) {: .copyable aria-label='Functions' }

___
### Get·Bag·Of·Crafting·Output () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) GetBagOfCraftingOutput ( ) {: .copyable aria-label='Functions' }

___
### Get·Bag·Of·Crafting·Slot () {: aria-label='Functions' }
#### [BagOfCraftingPickup](enums/BagOfCraftingPickup.md) GetBagOfCraftingSlot ( int SlotID ) {: .copyable aria-label='Functions' }
Gets the current content of the bag in the given Slot ID.
___
### Get·Bladder·Charge () {: aria-label='Functions' }
#### int GetBladderCharge ( ) {: .copyable aria-label='Functions' }

___
### Get·Body·Move·Direction () {: aria-label='Functions' }
#### [Vector](Vector.md) GetBodyMoveDirection ( ) {: .copyable aria-label='Functions' }    

___
### Get·Cambion·Conception·State () {: aria-label='Functions' }
#### int GetCambionConceptionState ( ) {: .copyable aria-label='Functions' }

___
### Get·Cambion·Pregnancy·Level () {: aria-label='Functions' }
#### int GetCambionPregnancyLevel ( ) {: .copyable aria-label='Functions' }

___
### Get·Collectibles·List () {: aria-label='Functions' }
#### table GetCollectiblesList ( ) {: .copyable aria-label='Functions' }

___
### Get·Costume·Layer·Map () {: aria-label='Functions' }
#### table GetCostumeLayerMap ( ) {: .copyable aria-label='Functions' }
Returns table of player sprite layers data for costumes with the following fields:

|Field|Type|Comment|
|:--|:--|:--|
| costumeIndex | int | Index of active/visible costume for layer. |
| layerID | int | |
| priority | int | |
| isBodyLayer | bool | |

___
### Get·Costume·Sprite·Descs () {: aria-label='Functions' }
#### table GetCostumeSpriteDescs ( ) {: .copyable aria-label='Functions' }
Returns a table of [CostumeSpriteDesc](CostumeSpriteDesc.md).
___
### Get·D8·Damage·Modifier () {: aria-label='Functions' }
#### int GetD8DamageModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·D8·Fire·Delay·Modifier () {: aria-label='Functions' }
#### int GetD8FireDelayModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·D8·Range·Modifier () {: aria-label='Functions' }
#### int GetD8RangeModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·D8·Speed·Modifier () {: aria-label='Functions' }
#### int GetD8SpeedModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·Damage·Modifier () {: aria-label='Functions' }
#### int GetDamageModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·Dead·Eye·Charge () {: aria-label='Functions' }
#### int GetDeadEyeCharge ( ) {: .copyable aria-label='Functions' }

___
### Get·Death·Anim·Name () {: aria-label='Functions' }
#### string GetDeathAnimName ( ) {: .copyable aria-label='Functions' }

___
### Get·Eden·Damage () {: aria-label='Functions' }
#### float GetEdenDamage ( ) {: .copyable aria-label='Functions' }

___
### Get·Eden·Fire·Delay () {: aria-label='Functions' }
#### float GetEdenFireDelay ( ) {: .copyable aria-label='Functions' }

___
### Get·Eden·Luck () {: aria-label='Functions' }
#### float GetEdenLuck ( ) {: .copyable aria-label='Functions' }

___
### Get·Eden·Range () {: aria-label='Functions' }
#### float GetEdenRange ( ) {: .copyable aria-label='Functions' }

___
### Get·Eden·Shot·Speed () {: aria-label='Functions' }
#### float GetEdenShotSpeed ( ) {: .copyable aria-label='Functions' }

___
### Get·Eden·Speed () {: aria-label='Functions' }
#### float GetEdenSpeed ( ) {: .copyable aria-label='Functions' }

___
### Get·Enter·Position () {: aria-label='Functions' }
#### [Vector](Vector.md) GetEnterPosition ( ) {: .copyable aria-label='Functions' }        

___
### Get·Entity·Config·Player () {: aria-label='Functions' }
#### [EntityConfigPlayer](EntityConfigPlayer.md) GetEntityConfigPlayer ( ) {: .copyable aria-label='Functions' }

___
### Get·Epiphora·Charge () {: aria-label='Functions' }
#### int GetEpiphoraCharge ( ) {: .copyable aria-label='Functions' }

___
### Get·Eve·Sumptorium·Charge () {: aria-label='Functions' }
#### int GetEveSumptoriumCharge ( ) {: .copyable aria-label='Functions' }

___
### Get·Fire·Delay·Modifier () {: aria-label='Functions' }
#### int GetFireDelayModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·Flipped·Form () {: aria-label='Functions' }
#### [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) GetFlippedForm ( ) {: .copyable aria-label='Functions' }
Returns flipped form of the current character (only used for Tainted Lazarus)

Otherwise, returns nil
___
### Get·Focus·Entity () {: aria-label='Functions' }
#### [Entity](Entity.md) GetFocusEntity ( ) {: .copyable aria-label='Functions' }

___
### Get·Footprint·Color () {: aria-label='Functions' }
#### [Color](Color.md) GetFootprintColor ( boolean leftFootprint ) {: .copyable aria-label='Functions' }

___
### Get·Glitch·Baby·Sub·Type () {: aria-label='Functions' }
#### int GetGlitchBabySubType ( ) {: .copyable aria-label='Functions' }

___
### Get·Glyph·Of·Balance·Drop () {: aria-label='Functions' }
#### table GetGlyphOfBalanceDrop ( int variant = -1, int subtype = -1 ) {: .copyable aria-label='Functions' }
returns a table containing the variant and subtype of the possible glyph of balance drop.
___
### Get·Greeds·Gullet·Hearts () {: aria-label='Functions' }
#### int GetGreedsGulletHearts ( ) {: .copyable aria-label='Functions' }

___
### Get·Health·Type () {: aria-label='Functions' }
#### [HealthType](enums/HealthType.md) GetHealthType ( ) {: .copyable aria-label='Functions' }

___
### Get·Held·Entity () {: aria-label='Functions' }
#### [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) GetHeldEntity ( ) {: .copyable aria-label='Functions' }
Returns the Entity that the player is holding over their head, such as with throwable red bombs or Suplex! Returns nil if no entity is currently being held.

___
### Get·Held·Sprite () {: aria-label='Functions' }
#### [Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) GetHeldSprite ( ) {: .copyable aria-label='Functions' }
Gets the Sprite object used for when the player is doing an animation that involves holding a sprite over their head, such as active item usage.

___
### Get·History () {: aria-label='Functions' }
#### [History](History.md) GetHistory ( ) {: .copyable aria-label='Functions' }

___
### Get·Immaculate·Conception·State () {: aria-label='Functions' }
#### int GetImmaculateConceptionState ( ) {: .copyable aria-label='Functions' }

___
### Get·Keepers·Sack·Bonus () {: aria-label='Functions' }
#### int GetKeepersSackBonus ( ) {: .copyable aria-label='Functions' }

___
### Get·Laser·Color () {: aria-label='Functions' }
#### [Color](Color.md) GetLaserColor ( ) {: .copyable aria-label='Functions' }

___
### Get·Luck·Modifier () {: aria-label='Functions' }
#### int GetLuckModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·Maggy·Swing·Cooldown () {: aria-label='Functions' }
#### int GetMaggySwingCooldown ( ) {: .copyable aria-label='Functions' }
Used for Tainted Magdalene's hugging attack.

___
### Get·Marked·Target () {: aria-label='Functions' }
#### [EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/EntityEffect.html) GetMarkedTarget ( ) { : .copyable aria-label='Functions' }

Returns the entity effect representing the target of the [Marked](https://bindingofisaacrebirth.fandom.com/wiki/Marked) item. 
If the target is not displayed on the ground, this function returns nil.

___
### Get·Max·Bladder·Charge () {: aria-label='Functions' }
#### int GetMaxBladderCharge ( ) {: .copyable aria-label='Functions' }

___
### Get·Max·Pee·Burst·Cooldown () {: aria-label='Functions' }
#### int GetMaxPeeBurstCooldown ( ) {: .copyable aria-label='Functions' }

___
### Get·Max·Pocket·Items () {: aria-label='Functions' }
#### int GetMaxPocketItems ( ) {: .copyable aria-label='Functions' }

___
### Get·Mega·Blast·Duration () {: aria-label='Functions' }
#### int GetMegaBlastDuration ( ) {: .copyable aria-label='Functions' }

___
### Get·Metronome·Collectible·ID () {: aria-label='Functions' }
#### int GetMetronomeCollectibleID ( ) {: .copyable aria-label='Functions' }

___
### Get·Next·Urethra·Block·Frame () {: aria-label='Functions' }
#### int GetNextUrethraBlockFrame ( ) {: .copyable aria-label='Functions' }

___
### Get·Num·Tears () {: aria-label='Functions' }
#### int GetNumTears ( ) {: .copyable aria-label='Functions' }

___
### Get·Pee·Burst·Cooldown () {: aria-label='Functions' }
#### int GetPeeBurstCooldown ( ) {: .copyable aria-label='Functions' }

___
### Get·Player·Form·Counter () {: aria-label='Functions' }
#### int GetPlayerFormCounter ( [PlayerForm](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerForm.html) playerFormID ) {: .copyable aria-label='Functions' } 

___
### Get·Pony·Charge () {: aria-label='Functions' }
#### int GetPonyCharge ( ) {: .copyable aria-label='Functions' }

___
### Get·Purity·State () {: aria-label='Functions' }
#### [PurityState](enums/PurityState.md) GetPurityState ( ) {: .copyable aria-label='Functions' }
Returns the state in which the "Purity" item effect currently is. 

___
### Get·Red·Stew·Bonus·Duration () {: aria-label='Functions' }
#### int GetRedStewBonusDuration ( ) {: .copyable aria-label='Functions' }

___
### Get·Shot·Speed·Modifier () {: aria-label='Functions' }
#### int GetShotSpeedModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·Smelted·Trinkets () {: aria-label='Functions' }
#### table GetSmeltedTrinkets ( ) {: .copyable aria-label='Functions' }
Returns a table of smelted trinkets and their corresponding amounts. The returned table contains the following fields:

|Field|Type|Comment|
|:--|:--|:--|
| trinketNum | int | |
| goldenTrinketNum | int | |

___
### Get·Special·Grid·Collision () {: aria-label='Functions' }
#### int GetSpecialGridCollision ( [Vector](Vector.md) position ) {: .copyable aria-label='Functions' }      

___
### Get·Speed·Modifier () {: aria-label='Functions' }
#### int GetSpeedModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·Total·Active·Charge () {: aria-label='Functions' }
#### int GetTotalActiveCharge ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### Get·Voided·Collectibles·List () {: aria-label='Functions' }
#### table GetVoidedCollectiblesList ( ) {: .copyable aria-label='Functions' }

___
### Get·Weapon () {: aria-label='Functions' }
#### [Weapon](Weapon.md) GetWeapon ( int slot ) {: .copyable aria-label='Functions' }
Slot needs to be between 0 and 4.
___
### Get·Weapon·Modifiers () {: aria-label='Functions' }
#### int GetWeaponModifiers ( ) {: .copyable aria-label='Functions' }

___
### Get·Wild·Card·Item () {: aria-label='Functions' }
#### int GetWildCardItem ( ) {: .copyable aria-label='Functions' }

___
### Get·Wild·Card·Item·Type () {: aria-label='Functions' }
#### [PocketItemType](enums/PocketItemType.md) GetWildCardItemType ( ) {: .copyable aria-label='Functions' }

___
### Get·Wisp·Collectibles·List () {: aria-label='Functions' }
#### table GetWispCollectiblesList ( ) {: .copyable aria-label='Functions' }

___
### Has·Instant·Death·Curse () {: aria-label='Functions' }
#### boolean HasInstantDeathCurse ( ) {: .copyable aria-label='Functions' }

___
### Has·Poison·Immunity () {: aria-label='Functions' }
#### boolean HasPoisonImmunity ( ) {: .copyable aria-label='Functions' }

___
### Increment·Player·Form·Counter () {: aria-label='Functions' }
#### void IncrementPlayerFormCounter ( [PlayerForm](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Form, int Num ) {: .copyable aria-label='Functions' }

___
### Init·Post·Level·Init·Stats () {: aria-label='Functions' }
#### void InitPostLevelInitStats ( ) {: .copyable aria-label='Functions' }
Call this after spawning characters with "special" tears (Forgotten, Lilith, Azazel etc) with InitTwin, or they won't have their proper tear type.

___
### Init·Twin () {: aria-label='Functions' }
#### [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) InitTwin ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }
I believe this is used by J&E, Strawman etc.

???+ bug "Bug"
    The twin player will desync from its main twin on save and continue. This softlocks the game in singleplayer, as the game prompts for a controller.
	
	We've received confirmation from \_Kilburn that this is hardcoded to be handled on vanilla characters. We will need to add a workaround for this.

___
### Is·Collectible·Anim·Finished () {: aria-label='Functions' }
#### boolean IsCollectibleAnimFinished ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) collectibleType, string animName ) {: .copyable aria-label='Functions' }

___
### Is·Collectible·Costume·Visible () {: aria-label='Functions' }
#### boolean IsCollectibleCostumeVisible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) collectibleType, int playerSpriteLayer ) {: .copyable aria-label='Functions' }

___
### Is·Entity·Valid·Target () {: aria-label='Functions' }
#### boolean IsEntityValidTarget ( [Entity](Entity.md) entity ) {: .copyable aria-label='Functions' }

___
### Is·Footstep·Frame () {: aria-label='Functions' }
#### boolean IsFootstepFrame ( int foot = -1 ) {: .copyable aria-label='Functions' }       

___
### Is·Headless () {: aria-label='Functions' }
#### boolean IsHeadless ( ) {: .copyable aria-label='Functions' }

___
### Is·Hologram () {: aria-label='Functions' }
#### boolean IsHologram ( ) {: .copyable aria-label='Functions' }

___
### Is·Invisible () {: aria-label='Functions' }
#### boolean IsInvisible ( ) {: .copyable aria-label='Functions' }

___
### Is·Item·Costume·Visible () {: aria-label='Functions' }
#### boolean IsItemCostumeVisible ( [ItemConfig_Item](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html) item, int playerSpriteLayer ) {: .copyable aria-label='Functions' }

___
### Is·Local·Player () {: aria-label='Functions' }
#### boolean IsLocalPlayer ( ) {: .copyable aria-label='Functions' }

___
### Is·Pacifist () {: aria-label='Functions' }
#### boolean IsPacifist ( ) {: .copyable aria-label='Functions' }

___
### Is·Urethra·Blocked () {: aria-label='Functions' }
#### boolean IsUrethraBlocked ( ) {: .copyable aria-label='Functions' }

___
### Morph·To·Coop·Ghost () {: aria-label='Functions' }
#### void MorphToCoopGhost ( ) {: .copyable aria-label='Functions' }

___
### Play·Collectible·Anim () {: aria-label='Functions' }
#### void PlayCollectibleAnim ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) collectibleType, bool CheckBodyLayers, string animName, int frameNum = -1 ) {: .copyable aria-label='Functions' }

___
### Play·Delayed·SFX () {: aria-label='Functions' }
#### void PlayDelayedSFX ( [SoundEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) ID, int SoundDelay = 0, int FrameDelay = 2, float Volume = 1 ) {: .copyable aria-label='Functions' }

___
### Remove·Collectible·By·History·Index () {: aria-label='Functions' }
#### void RemoveCollectibleByHistoryIndex ( int Index ) {: .copyable aria-label='Functions' }

___
### Remove·Pocket·Item () {: aria-label='Functions' }
#### void RemovePocketItem ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) slot ) {: .copyable aria-label='Functions' }

___
### Remove·Poop·Spell () {: aria-label='Functions' }
#### void RemovePoopSpell ( int Position = 0 ) {: .copyable aria-label='Functions' }
Removes the poop spell from the specified queue position and shifts all spells after it forward to fill the space. A new spell is randomly picked to fill the last position.

___
### Reroll·All·Collectibles () {: aria-label='Functions' }
#### void RerollAllCollectibles ( [RNG](RNG.md) rng, bool includeActiveItems ) {: .copyable aria-label='Functions' }

___
### Reset·Player () {: aria-label='Functions' }
#### void ResetPlayer ( ) {: .copyable aria-label='Functions' }

???+ info "Info"
    This is used by the Genesis active item.

___
### Revive·Coop·Ghost () {: aria-label='Functions' }
#### boolean ReviveCoopGhost ( ) {: .copyable aria-label='Functions' }

___
### Salvage·Collectible () {: aria-label='Functions' }
#### void SalvageCollectible ( [EntityPickup](EntityPickup.md) pickup, [RNG](RNG.md) rng = PickupDropRNG, [ItemPool](ItemPool.md) pool = -1) {: .copyable aria-label='Functions' }
#### void SalvageCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) CollectibleID, [Vector](Vector.md) position = playerPosition, [RNG](RNG.md) rng = PlayerDropRNG, [ItemPool](ItemPool.md) pool = -1 ) {: .copyable aria-label='Functions' }

___
### Set·Active·Var·Data () {: aria-label='Functions' }
#### void SetActiveVarData ( int VarData, [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### Set·Bag·Of·Crafting·Content () {: aria-label='Functions' }
#### void SetBagOfCraftingContent ( table ContentTable ) {: .copyable aria-label='Functions' }
Sets the content of the bag to the content of the table. Table must use valid [BagOfCraftingPickup](enums/BagOfCraftingPickup.md) ids. Table can be shorter than 8, in which case the remaining indexes are set to empty.
___
### Set·Bag·Of·Crafting·Output () {: aria-label='Functions' }
#### void SetBagOfCraftingOutput ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) CollectibleID ) {: .copyable aria-label='Functions' }

___
### Set·Bag·Of·Crafting·Slot () {: aria-label='Functions' }
#### void SetBagOfCraftingSlot ( int SlotID, int PickupID ) {: .copyable aria-label='Functions' }

___
### Set·Bladder·Charge () {: aria-label='Functions' }
#### void SetBladderCharge ( int Charge ) {: .copyable aria-label='Functions' }

___
### Set·Cambion·Conception·State () {: aria-label='Functions' }
#### void SetCambionConceptionState ( int State ) {: .copyable aria-label='Functions' }

___
### Set·Can·Shoot () {: aria-label='Functions' }
#### boolean SetCanShoot ( boolean CanShoot ) {: .copyable aria-label='Functions' }
Instantaneously disables (or enables) the player's ability to shoot. The base game primarily uses this for special challenges.
___
### Set·Controller·Index () {: aria-label='Functions' }
#### void SetControllerIndex ( int idx ) {: .copyable aria-label='Functions' }        

___
### Set·Damage·Modifier () {: aria-label='Functions' }
#### void SetDamageModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### Set·Eden·Damage () {: aria-label='Functions' }
#### void SetEdenDamage ( float Value ) {: .copyable aria-label='Functions' }

___
### Set·Eden·Fire·Delay () {: aria-label='Functions' }
#### void SetEdenFireDelay ( float Value ) {: .copyable aria-label='Functions' }

___
### Set·Eden·Luck () {: aria-label='Functions' }
#### void SetEdenLuck ( float Value ) {: .copyable aria-label='Functions' }

___
### Set·Eden·Range () {: aria-label='Functions' }
#### void SetEdenRange ( float Value ) {: .copyable aria-label='Functions' }

___
### Set·Eden·Shot·Speed () {: aria-label='Functions' }
#### void SetEdenShotSpeed ( float Value ) {: .copyable aria-label='Functions' }

___
### Set·Eden·Speed () {: aria-label='Functions' }
#### void SetEdenSpeed ( float Value ) {: .copyable aria-label='Functions' }

___
### Set·Eve·Sumptorium·Charge () {: aria-label='Functions' }
#### void SetEveSumptoriumCharge ( int ChargeNum ) {: .copyable aria-label='Functions' }

___
### Set·Fire·Delay·Modifier () {: aria-label='Functions' }
#### void SetFireDelayModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### Set·Footprint·Color () {: aria-label='Functions' }
#### void SetFootprintColor ( [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor.html) color, bool unk = false ) {: .copyable aria-label='Functions' }

___
### Set·Immaculate·Conception·State () {: aria-label='Functions' }
#### void SetImmaculateConceptionState ( int State ) {: .copyable aria-label='Functions' }

___
### Set·Item·State () {: aria-label='Functions' }
#### void SetItemState ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) CollectibleType ) {: .copyable aria-label='Functions' }
This is used for thrown/2-input items (e.g. Bob's Rotten Head).

___
### Set·Keepers·Sack·Bonus () {: aria-label='Functions' }
#### void SetKeepersSackBonus ( int bonus ) {: .copyable aria-label='Functions' }

___
### Set·Laser·Color () {: aria-label='Functions' }
#### void SetLaserColor ( [Color](Color.md) color ) {: .copyable aria-label='Functions' }

___
### Set·Luck·Modifier () {: aria-label='Functions' }
#### void SetLuckModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### Set·Maggy·Swing·Cooldown () {: aria-label='Functions' }
#### void SetMaggySwingCooldown ( int Cooldown ) {: .copyable aria-label='Functions' }

___
### Set·Max·Bladder·Charge () {: aria-label='Functions' }
#### void SetMaxBladderCharge ( int Charge ) {: .copyable aria-label='Functions' }

___
### Set·Mega·Blast·Duration () {: aria-label='Functions' }
#### void SetMegaBlastDuration ( int Duration ) {: .copyable aria-label='Functions' }

___
### Set·Next·Urethra·Block·Frame () {: aria-label='Functions' }
#### void SetNextUrethraBlockFrame ( int Frame ) {: .copyable aria-label='Functions' }

___
### Set·Pony·Charge () {: aria-label='Functions' }
#### void SetPonyCharge ( int Time ) {: .copyable aria-label='Functions' }

___
### Set·Poop·Spell () {: aria-label='Functions' }
#### void SetPoopSpell ( int Position, [PoopSpellType](https://wofsauge.github.io/IsaacDocs/rep/enums/PoopSpellType.html) PoopSpellType ) {: .copyable aria-label='Functions' }

___
### Set·Purity·State () {: aria-label='Functions' }
#### void SetPurityState ( [state](enums/PurityState.md) ) {: .copyable aria-label='Functions' }
Set the state in which the "Purity" item effect currently is.

___
### Set·Red·Stew·Bonus·Duration () {: aria-label='Functions' }
#### void SetRedStewBonusDuration ( int Duration ) {: .copyable aria-label='Functions' }

___
### Set·Shot·Speed·Modifier () {: aria-label='Functions' }
#### void SetShotSpeedModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### Set·Speed·Modifier () {: aria-label='Functions' }
#### void SetSpeedModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### Set·Tear·Poison·Damage () {: aria-label='Functions' }
#### void SetTearPoisonDamage ( float Damage ) {: .copyable aria-label='Functions' }

___
### Set·Tear·Range·Modifier () {: aria-label='Functions' }
#### void SetTearRangeModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### Set·Urethra·Block () {: aria-label='Functions' }
#### void SetUrethraBlock ( boolean Block ) {: .copyable aria-label='Functions' }

___
### Set·Weapon () {: aria-label='Functions' }
#### void SetWeapon ( [Weapon](Weapon.md) Weapon, int WeaponSlot ) {: .copyable aria-label='Functions' }

___
### Shoot·Blue·Candle () {: aria-label='Functions' }
#### void ShootBlueCandle ( [Vector](Vector.md) direction ) {: .copyable aria-label='Functions' }

___
### Shuffle·Costumes () {: aria-label='Functions' }
#### void ShuffleCostumes ( int Seed ) {: .copyable aria-label='Functions' }
Randomizes the current costumes.

___
### Spawn·Aquarius·Creep () {: aria-label='Functions' }
#### [EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/EntityEffect.html) SpawnAquariusCreep ( [TearParams](https://wofsauge.github.io/IsaacDocs/rep/TearParams.html) TearParams = nil) {: .copyable aria-label='Functions' }
Spawns a creep effect that acts like the ones created by Aquarius, including inhereting the player's `TearParams`. Supports passing a custom `TearParams` instead.


???+ info "Info"
    For reference, this is how the game calculates the `TearParams` for this normally:
	
	``player->GetTearHitParams(&params, WeaponType.WEAPON_TEARS, (*player->GetTearPoisonDamage() * 0.666f) / player->_damage, -(int)(-Isaac::Random(2) != 0) & 2 - 1, nil)``

___
### Spawn·Clot () {: aria-label='Functions' }
#### void SpawnClot ( [Vector](Vector.md) pos, bool AllowPlayerDeath = false ) {: .copyable aria-label='Functions' } 

Acts like a use of Sumptorium, removing health and spawning a clot with the type of health removed. If `AllowPlayerDeath` is set, a clot will spawn even if the health drained will kill the player.

___
### Spawn·Saturnus·Tears () {: aria-label='Functions' }
#### int SpawnSaturnusTears ( ) {: .copyable aria-label='Functions' }

___
### Swap·Forgotten·Form () {: aria-label='Functions' }
#### void SwapForgottenForm ( boolean Force = false, boolean NoEffects = false) {: .copyable aria-label='Functions' }
If the player has a [SubPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html#getsubplayer), swaps between them. `Force` will swap even if the subplayer doesn't have any health, or while a room/stage transition is active. `NoEffects` will disable the dust effect & fade from white when switching from The Soul to The Forgotten.


???+ warning "Warning"
    This function currently only has an effect for The Forgotten and The Soul.

___
### Sync·Consumable·Counts () {: aria-label='Functions' }
#### void SyncConsumableCounts ( [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) Player, int CollectibleFlags ) {: .copyable aria-label='Functions' }      

___
### Teleport () {: aria-label='Functions' }
#### void Teleport ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position, boolean DoEffects = true, boolean TeleportTwinPlayers = false ) {: .copyable aria-label='Functions' }
Teleports the player to a position within the room. 

`DoEffects` controls whether the teleport animation and sound plays. `TeleportTwinPlayers` controls whether twin players (e.g. Esau, Tainted Lazarus w/ Birthright) are teleported alongside this one.

___
### Trigger·Room·Clear () {: aria-label='Functions' }
#### void TriggerRoomClear ( ) {: .copyable aria-label='Functions' }

___
### Try·Add·To·Bag·Of·Crafting () {: aria-label='Functions' }
#### void TryAddToBagOfCrafting ( [EntityPickup](EntityPickup.md) pickup ) {: .copyable aria-label='Functions' }

___
### Try·Decrease·Glowing·Hourglass·Uses () {: aria-label='Functions' }
#### void TryDecreaseGlowingHourglassUses ( int unk1, bool unk2 ) {: .copyable aria-label='Functions' }

___
### Try·Fake·Death () {: aria-label='Functions' }
#### boolean TryFakeDeath ( ) {: .copyable aria-label='Functions' }

___
### Try·Forgotten·Throw () {: aria-label='Functions' }
#### boolean TryForgottenThrow ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) direction ) {: .copyable aria-label='Functions' }        

___
### Try·Prevent·Death () {: aria-label='Functions' }
#### boolean TryPreventDeath ( ) {: .copyable aria-label='Functions' }
Adds a heart container to a character if there are none left, depending on its [HealthType](enums/HealthType.md).

Returns `true` on success, if not then `false`.

___
### Try·Remove·Smelted·Trinket () {: aria-label='Functions' }
#### void TryRemoveSmeltedTrinket ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) id ) {: .copyable aria-label='Functions' }    

___
### Update·Isaac·Pregnancy () {: aria-label='Functions' }
#### void UpdateIsaacPregnancy ( boolean Cambion ) {: .copyable aria-label='Functions' }
Set ``true`` if you want to update Cambion Conception costume, otherwise Immaculate Conception.

___
### Void·Has·Collectible () {: aria-label='Functions' }
#### boolean VoidHasCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) id ) {: .copyable aria-label='Functions' }

___
