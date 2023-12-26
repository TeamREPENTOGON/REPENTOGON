---
tags:
  - Class
---
# Class "EntityPlayer"

This class contains both new functions and modified reimplementations of existing ones.

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Modified Functions

### AddCacheFlags () {: aria-label='Modified Functions' }
#### void AddCacheFlags ( [CacheFlag](https://wofsauge.github.io/IsaacDocs/rep/enums/CacheFlag.html) CacheFlag, boolean EvaluateItems = false ) {: .copyable aria-label='Modified Functions' }
Now accepts an optional `bool` to determine if [EntityPlayer](EntityPlayer.md):EvaluateItems() should be automatically called after adding cache flags. In most cases, you'll want to do this.

___
### ClearDeadEyeCharge () {: aria-label='Modified Functions' }
#### void ClearDeadEyeCharge ( boolean Force = false ) {: .copyable aria-label='Modified Functions' }
Now accepts a `Force` argument to forcefully reset the charge instead of only rolling for a change to reset.

___
### GetMultiShotParams () {: aria-label='Modified Functions' }
#### [MultiShotParams](MultiShotParams.md) GetMultiShotParams ( [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) WeaponType ) {: .copyable aria-label='Modified Functions' }
Now returns a proper `MultiShotParams` object.

___
### GetMultiShotPositionVelocity () {: aria-label='Modified Functions' }
#### [PosVel](https://wofsauge.github.io/IsaacDocs/rep/PlayerTypes_PosVel.html) GetMultiShotPositionVelocity ( int LoopIndex, [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) WeaponType, [Vector](Vector.md) ShotDirection, float ShotSpeed, [MultiShotParams](MultiShotParams.md) Params ) {: .copyable aria-label='Modified Functions' }
This magically vanished from the API some time after 1.7.8.

Compared to the vanilla function, this implementation has been further augmented to throw an error if LoopIndex is higher than [MultiShotParams:GetNumTears()](MultiShotParams.md#getnumtears).

___
### GetPocketItem () {: aria-label='Modified Functions' }
#### [PocketItem](PocketItem.md) GetPocketItem ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) SlotId ) {: .copyable aria-label='Modified Functions' }
Now returns a proper `PocketItem` object.

___

## Modified Variables
___
### BabySkin {: aria-label='Modified Variables' }
#### [BabySubType](https://wofsauge.github.io/IsaacDocs/rep/enums/BabySubType.html) BabySkin  {: .copyable aria-label='Modified Variables' }
Same as default, but now returns a proper integer value instead of userdata.

___

## Functions

### AddActiveCharge () {: aria-label='Functions' }
#### int AddActiveCharge ( int Charge, [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot, boolean FlashHUD, boolean Overcharge, boolean Force ) {: .copyable aria-label='Functions' }

Returns the true amount of charge added, which may have been capped by the targeted item's MaxCharge.

___
### AddBoneOrbital () {: aria-label='Functions' }
#### void AddBoneOrbital ( ) {: .copyable aria-label='Functions' }

___
### AddInnateCollectible () {: aria-label='Functions' }
#### void AddInnateCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, int Amount = 1 ) {: .copyable aria-label='Functions' }

???+ bug "Bug"
	Currently this function directly modifies the contents of WispCollectiblesList, so if this list was updated on wisp init/delete, or the player exits the run, your added innate items will not be saved.
	
___
### AddLeprocy () {: aria-label='Functions' }
#### void AddLeprocy ( ) {: .copyable aria-label='Functions' }

???+ info "Info"
    This is currently still capped at a max of three familiars, and would require further modification to change this.

___
### AddLocust () {: aria-label='Functions' }
#### void AddLocust ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, [Vector](Vector.md) Position ) {: .copyable aria-label='Functions' }

___
### AddSmeltedTrinket () {: aria-label='Functions' }
#### boolean AddSmeltedTrinket ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Trinket, boolean FirstTimePickingUp = true ) {: .copyable aria-label='Functions' }
Adds a smelted trinket directly to the player's inventory.

Returns ``true`` if the trinket was successfully added, otherwise ``false``.

___
### AddUrnSouls () {: aria-label='Functions' }
#### void AddUrnSouls ( int Count = 0 ) {: .copyable aria-label='Functions' }

___
### AttachMinecart () {: aria-label='Functions' }
#### void AttachMinecart ( [EntityNPC](EntityNPC.md) Minecart ) {: .copyable aria-label='Functions' }     

___
### CanAddCollectibleToInventory () {: aria-label='Functions' }
#### boolean CanAddCollectibleToInventory ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }

___
### CanCrushRocks () {: aria-label='Functions' }
#### boolean CanCrushRocks ( ) {: .copyable aria-label='Functions' }

___
### CanOverrideActiveItem () {: aria-label='Functions' }
#### boolean CanOverrideActiveItem ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }

___
### CanUsePill () {: aria-label='Functions' }
#### boolean CanUsePill ( [PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.html) ID ) {: .copyable aria-label='Functions' }
By some conditions, usually health-related, determines whether a player can use a given pill effect.

___
### CheckFamiliarEx () {: aria-label='Functions' }
#### [EntityFamiliar](https://wofsauge.github.io/IsaacDocs/rep/EntityFamiliar.html)[] CheckFamiliarEx ( int [FamiliarVariant](https://wofsauge.github.io/IsaacDocs/rep/EntityFamiliar.html) Familiar, int TargetCount, [RNG](https://wofsauge.github.io/IsaacDocs/rep/RNG.html) rng, [ItemConfigItem](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html) SourceItemConfigItem = nil, int FamiliarSubType = -1 ) {: .copyable aria-label='Functions' }

A version of [CheckFamiliar](EntityPlayer.md#checkfamiliar) that returns all familiars spawned by the function as a table.

___
### ClearCollectibleAnim () {: aria-label='Functions' }
#### void ClearCollectibleAnim ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible) {: .copyable aria-label='Functions' }

___
### ClearItemAnimCollectible () {: aria-label='Functions' }
#### void ClearItemAnimCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) ID ) {: .copyable aria-label='Functions' }

___
### ClearItemAnimNullItems () {: aria-label='Functions' }
#### void ClearItemAnimNullItems ( ) {: .copyable aria-label='Functions' }

___
### DropCollectible () {: aria-label='Functions' }
#### void DropCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, [EntityPickup](EntityPickup.md) ExistingPedestal = nil, boolean RemoveFromPlayerForm = false ) {: .copyable aria-label='Functions' }

If `ExistingPedestal` is set, the collectible it contains will be swapped out for the dropped collectible instead of a new pedestal spawning.

___
### DropCollectibleByHistoryIndex () {: aria-label='Functions' }
#### [EntityPickup](EntityPickup.md) DropCollectibleByHistoryIndex ( int Idx, [EntityPickup](EntityPickup.md) ExistingPedestal = nil ) {: .copyable aria-label='Functions' }

If `ExistingPedestal` is set, the collectible it contains will be swapped out for the dropped collectible instead of a new pedestal spawning.

___
### EnableWeaponType () {: aria-label='Functions' }
#### void EnableWeaponType ( [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) Weapon, boolean Set ) {: .copyable aria-label='Functions' }

___
### FireBrimstoneBall () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) FireBrimstoneBall ( [Vector](Vector.md) Position, [Vector](Vector.md) Velocity, [Vector](Vector.md) Offset = Vector.Zero ) {: .copyable aria-label='Functions' }

???+ info "Info"
    If the player has Tech X, this function will fire an [EntityLaser](EntityLaser.md) as well. The laser will have the Brimstone ball effect as a parent, it's unclear if the effect also links back to the laser.

___
### GetActiveItemDesc () {: aria-label='Functions' }
#### [ActiveItemDesc](https://wofsauge.github.io/IsaacDocs/rep/PlayerTypes_ActiveItemDesc.html) GetActiveItemDesc ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot = ActiveSlot.SLOT_PRIMARY ) {: .copyable aria-label='Functions' }

___
### GetActiveItemSlot () {: aria-label='Functions' }
#### [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) GetActiveItemSlot ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }

___
### GetActiveMaxCharge () {: aria-label='Functions' }
#### int GetActiveMaxCharge ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### GetActiveMinUsableCharge () {: aria-label='Functions' }
#### int GetActiveMinUsableCharge ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### GetActiveWeaponNumFired () {: aria-label='Functions' }
#### int GetActiveWeaponNumFired ( ) {: .copyable aria-label='Functions' }

___
### GetBagOfCraftingContent () {: aria-label='Functions' }
#### [BagOfCraftingPickup](enums/BagOfCraftingPickup.md)[] GetBagOfCraftingContent ( ) {: .copyable aria-label='Functions' }

___
### GetBagOfCraftingOutput () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) GetBagOfCraftingOutput ( ) {: .copyable aria-label='Functions' }

___
### GetBagOfCraftingSlot () {: aria-label='Functions' }
#### [BagOfCraftingPickup](enums/BagOfCraftingPickup.md) GetBagOfCraftingSlot ( int SlotID ) {: .copyable aria-label='Functions' }
Gets the current content of the bag in the given Slot ID.
___
### GetBladderCharge () {: aria-label='Functions' }
#### int GetBladderCharge ( ) {: .copyable aria-label='Functions' }

___
### GetBodyMoveDirection () {: aria-label='Functions' }
#### [Vector](Vector.md) GetBodyMoveDirection ( ) {: .copyable aria-label='Functions' }    

___
### GetCambionConceptionState () {: aria-label='Functions' }
#### int GetCambionConceptionState ( ) {: .copyable aria-label='Functions' }

___
### GetCambionPregnancyLevel () {: aria-label='Functions' }
#### int GetCambionPregnancyLevel ( ) {: .copyable aria-label='Functions' }

___
### GetCollectiblesList () {: aria-label='Functions' }
#### table GetCollectiblesList ( ) {: .copyable aria-label='Functions' }
Returns a table with the amount of each collectible the player has without counting innate items.


???- example "Example Code"
    This code prints how many sad onions the player has.

    ```lua
    local collectiblesList = player:GetCollectiblesList()

    print(collectiblesList[CollectibleType.COLLECTIBLE_SAD_ONION])
    ```

___
### GetCostumeLayerMap () {: aria-label='Functions' }
#### table GetCostumeLayerMap ( ) {: .copyable aria-label='Functions' }
Returns table of player sprite layers data for costumes with the following fields:

|Field|Type|Comment|
|:--|:--|:--|
| costumeIndex | int | Index of active/visible costume for layer. |
| layerID | int | |
| priority | int | |
| isBodyLayer | boolean | |

___
### GetCostumeSpriteDescs () {: aria-label='Functions' }
#### [CostumeSpriteDesc](CostumeSpriteDesc.md)[] GetCostumeSpriteDescs ( ) {: .copyable aria-label='Functions' }
Returns a table of [CostumeSpriteDesc](CostumeSpriteDesc.md).

___
### GetD8DamageModifier () {: aria-label='Functions' }
#### int GetD8DamageModifier ( ) {: .copyable aria-label='Functions' }

___
### GetD8FireDelayModifier () {: aria-label='Functions' }
#### int GetD8FireDelayModifier ( ) {: .copyable aria-label='Functions' }

___
### GetD8RangeModifier () {: aria-label='Functions' }
#### int GetD8RangeModifier ( ) {: .copyable aria-label='Functions' }

___
### GetD8SpeedModifier () {: aria-label='Functions' }
#### int GetD8SpeedModifier ( ) {: .copyable aria-label='Functions' }

___
### GetDamageModifier () {: aria-label='Functions' }
#### int GetDamageModifier ( ) {: .copyable aria-label='Functions' }
For Experimental Treatement, returns `-1`, `0` or `1` depending on the damage rolled.

___
### GetDeadEyeCharge () {: aria-label='Functions' }
#### int GetDeadEyeCharge ( ) {: .copyable aria-label='Functions' }

___
### GetDeathAnimName () {: aria-label='Functions' }
#### string GetDeathAnimName ( ) {: .copyable aria-label='Functions' }

___
### GetEdenDamage () {: aria-label='Functions' }
#### float GetEdenDamage ( ) {: .copyable aria-label='Functions' }

___
### GetEdenFireDelay () {: aria-label='Functions' }
#### float GetEdenFireDelay ( ) {: .copyable aria-label='Functions' }

___
### GetEdenLuck () {: aria-label='Functions' }
#### float GetEdenLuck ( ) {: .copyable aria-label='Functions' }

___
### GetEdenRange () {: aria-label='Functions' }
#### float GetEdenRange ( ) {: .copyable aria-label='Functions' }

___
### GetEdenShotSpeed () {: aria-label='Functions' }
#### float GetEdenShotSpeed ( ) {: .copyable aria-label='Functions' }

___
### GetEdenSpeed () {: aria-label='Functions' }
#### float GetEdenSpeed ( ) {: .copyable aria-label='Functions' }

___
### GetEnterPosition () {: aria-label='Functions' }
#### [Vector](Vector.md) GetEnterPosition ( ) {: .copyable aria-label='Functions' }        

___
### GetEntityConfigPlayer () {: aria-label='Functions' }
#### [EntityConfigPlayer](EntityConfigPlayer.md) GetEntityConfigPlayer ( ) {: .copyable aria-label='Functions' }

___
### GetEpiphoraCharge () {: aria-label='Functions' }
#### int GetEpiphoraCharge ( ) {: .copyable aria-label='Functions' }

___
### GetEveSumptoriumCharge () {: aria-label='Functions' }
#### int GetEveSumptoriumCharge ( ) {: .copyable aria-label='Functions' }

___
### GetFireDelayModifier () {: aria-label='Functions' }
#### int GetFireDelayModifier ( ) {: .copyable aria-label='Functions' }
For Experimental Treatement, returns `-1`, `0` or `1` depending on the fire delay rolled.

___
### GetFlippedForm () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) GetFlippedForm ( ) {: .copyable aria-label='Functions' }
Returns flipped form of the current character. (only used for Tainted Lazarus)

Otherwise, returns `nil`.

___
### GetFocusEntity () {: aria-label='Functions' }
#### [Entity](Entity.md) GetFocusEntity ( ) {: .copyable aria-label='Functions' }

Returns the entity used by Active Camera to determine where the camera should focus. This can be either the [Marked](https://bindingofisaacrebirth.fandom.com/wiki/Marked) target [EntityEffect](EntityEffect.md) or a weapon's entity. 
If none of these exist, this returns `nil`.

___
### GetFootprintColor () {: aria-label='Functions' }
#### [Color](Color.md) GetFootprintColor ( boolean LeftFootprint ) {: .copyable aria-label='Functions' }

___
### GetGlitchBabySubType () {: aria-label='Functions' }
#### int GetGlitchBabySubType ( ) {: .copyable aria-label='Functions' }

___
### GetGlyphOfBalanceDrop () {: aria-label='Functions' }
#### table GetGlyphOfBalanceDrop ( int Variant = -1, int SubType = -1 ) {: .copyable aria-label='Functions' }
returns a table containing the variant and subtype of the possible [Glyph of Balance]((https://bindingofisaacrebirth.fandom.com/wiki/Glyph_of_Balance) drop.
___
### GetGreedsGulletHearts () {: aria-label='Functions' }
#### int GetGreedsGulletHearts ( ) {: .copyable aria-label='Functions' }

___
### GetHealthType () {: aria-label='Functions' }
#### [HealthType](enums/HealthType.md) GetHealthType ( ) {: .copyable aria-label='Functions' }

___
### GetHeldEntity () {: aria-label='Functions' }
#### [Entity](Entity.md) GetHeldEntity ( ) {: .copyable aria-label='Functions' }
Returns the Entity that the player is holding over their head, such as with throwable red bombs or [Suplex!]((https://bindingofisaacrebirth.fandom.com/wiki/Suplex!)
Returns `nil` if no entity is currently being held.

___
### GetHeldSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetHeldSprite ( ) {: .copyable aria-label='Functions' }
Gets the Sprite object used for when the player is doing an animation that involves holding a sprite over their head, such as active item usage.

___
### GetHistory () {: aria-label='Functions' }
#### [History](History.md) GetHistory ( ) {: .copyable aria-label='Functions' }

___
### GetImmaculateConceptionState () {: aria-label='Functions' }
#### int GetImmaculateConceptionState ( ) {: .copyable aria-label='Functions' }

___
### GetKeepersSackBonus () {: aria-label='Functions' }
#### int GetKeepersSackBonus ( ) {: .copyable aria-label='Functions' }

Gets the number of coins spent while possessing [Keeper's Sack]((https://bindingofisaacrebirth.fandom.com/wiki/Keeper's_Sack).

___
### GetLaserColor () {: aria-label='Functions' }
#### [Color](Color.md) GetLaserColor ( ) {: .copyable aria-label='Functions' }

___
### GetLuckModifier () {: aria-label='Functions' }
#### int GetLuckModifier ( ) {: .copyable aria-label='Functions' }
For Experimental Treatement, returns `-1`, `0` or `1` depending on the luck rolled.

___
### GetMaggySwingCooldown () {: aria-label='Functions' }
#### int GetMaggySwingCooldown ( ) {: .copyable aria-label='Functions' }
Used for Tainted Magdalene's hugging attack.

___
### GetMarkedTarget () {: aria-label='Functions' }
#### [EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/EntityEffect.html) GetMarkedTarget ( ) { : .copyable aria-label='Functions' }

Returns the entity effect representing the target of the [Marked](https://bindingofisaacrebirth.fandom.com/wiki/Marked) item. 
If the target is not displayed on the ground, this function returns nil.

___
### GetMaxBladderCharge () {: aria-label='Functions' }
#### int GetMaxBladderCharge ( ) {: .copyable aria-label='Functions' }

___
### GetMaxPeeBurstCooldown () {: aria-label='Functions' }
#### int GetMaxPeeBurstCooldown ( ) {: .copyable aria-label='Functions' }

___
### GetMaxPocketItems () {: aria-label='Functions' }
#### int GetMaxPocketItems ( ) {: .copyable aria-label='Functions' }

___
### GetMegaBlastDuration () {: aria-label='Functions' }
#### int GetMegaBlastDuration ( ) {: .copyable aria-label='Functions' }

___
### GetMetronomeCollectibleID () {: aria-label='Functions' }
#### int GetMetronomeCollectibleID ( ) {: .copyable aria-label='Functions' }

___
### GetNextUrethraBlockFrame () {: aria-label='Functions' }
#### int GetNextUrethraBlockFrame ( ) {: .copyable aria-label='Functions' }
Returns the frame at which the player stops shooting and starts charging the [Kidney Stone](https://bindingofisaacrebirth.fandom.com/wiki/Kidney_Stone) item.

___
### GetPeeBurstCooldown () {: aria-label='Functions' }
#### int GetPeeBurstCooldown ( ) {: .copyable aria-label='Functions' }
Returns the attack duration of the [Kidney Stone](https://bindingofisaacrebirth.fandom.com/wiki/Kidney_Stone) item.

___
### GetPlayerFormCounter () {: aria-label='Functions' }
#### int GetPlayerFormCounter ( [PlayerForm](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerForm.html) PlayerFormID ) {: .copyable aria-label='Functions' } 

___
### GetPonyCharge () {: aria-label='Functions' }
#### int GetPonyCharge ( ) {: .copyable aria-label='Functions' }

___
### GetPurityState () {: aria-label='Functions' }
#### [PurityState](enums/PurityState.md) GetPurityState ( ) {: .copyable aria-label='Functions' }
Returns the state in which the [Purity](https://bindingofisaacrebirth.fandom.com/wiki/Purity) item effect currently is. 

___
### GetRedStewBonusDuration () {: aria-label='Functions' }
#### int GetRedStewBonusDuration ( ) {: .copyable aria-label='Functions' }

___
### GetShotSpeedModifier () {: aria-label='Functions' }
#### int GetShotSpeedModifier ( ) {: .copyable aria-label='Functions' }
For Experimental Treatement, returns `-1`, `0` or `1` depending on the shot speed rolled.

___
### GetSmeltedTrinkets () {: aria-label='Functions' }
#### table GetSmeltedTrinkets ( ) {: .copyable aria-label='Functions' }
Returns a table of smelted trinkets and their corresponding amounts. The returned table contains the following fields:

|Field|Type|Comment|
|:--|:--|:--|
| trinketNum | int | |
| goldenTrinketNum | int | |

___
### GetSpecialGridCollision () {: aria-label='Functions' }
#### int GetSpecialGridCollision ( [Vector](Vector.md) Position ) {: .copyable aria-label='Functions' }      

___
### GetSpeedModifier () {: aria-label='Functions' }
#### int GetSpeedModifier ( ) {: .copyable aria-label='Functions' }
For Experimental Treatement, returns `-1`, `0` or `1` depending on the speed rolled.

___
### GetTotalActiveCharge () {: aria-label='Functions' }
#### int GetTotalActiveCharge ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### GetVoidedCollectiblesList () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)[] Collectibles GetVoidedCollectiblesList ( ) {: .copyable aria-label='Functions' }

___
### GetWeapon () {: aria-label='Functions' }
#### [Weapon](Weapon.md) GetWeapon ( int Slot ) {: .copyable aria-label='Functions' }
Slot needs to be between `0` and `4`.
___
### GetWeaponModifiers () {: aria-label='Functions' }
#### int GetWeaponModifiers ( ) {: .copyable aria-label='Functions' }

___
### GetWildCardItem () {: aria-label='Functions' }
#### int GetWildCardItem ( ) {: .copyable aria-label='Functions' }

___
### GetWildCardItemType () {: aria-label='Functions' }
#### [PocketItemType](enums/PocketItemType.md) GetWildCardItemType ( ) {: .copyable aria-label='Functions' }

___
### GetWispCollectiblesList () {: aria-label='Functions' }
#### table GetWispCollectiblesList ( ) {: .copyable aria-label='Functions' }

___
### HasInstantDeathCurse () {: aria-label='Functions' }
#### boolean HasInstantDeathCurse ( ) {: .copyable aria-label='Functions' }
Returns true when the player is in the Lost form triggered by either the white fire in Downpour or Soul of The Lost. (or when in Tainted Jacob's ghost form when being touched by Dark Esau)

___
### HasPoisonImmunity () {: aria-label='Functions' }
#### boolean HasPoisonImmunity ( ) {: .copyable aria-label='Functions' }

___
### IncrementPlayerFormCounter () {: aria-label='Functions' }
#### void IncrementPlayerFormCounter ( [PlayerForm](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerForm.html) Form, int Count ) {: .copyable aria-label='Functions' }
???+ info "Info"
	`Count` can be negative to decrement the [PlayerForm](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerForm.html).

___
### InitPostLevelInitStats () {: aria-label='Functions' }
#### void InitPostLevelInitStats ( ) {: .copyable aria-label='Functions' }
Call this after spawning characters with "special" tears (Forgotten, Lilith, Azazel etc) with InitTwin, or they won't have their proper tear type.

___
### InitTwin () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) InitTwin ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }
I believe this is used by J&E, Strawman etc.

???+ bug "Bug"
    The twin player will desync from its main twin on save and continue. This softlocks the game in singleplayer, as the game prompts for a controller.
	
	We've received confirmation from \_Kilburn that this is hardcoded to be handled on vanilla characters. We will need to add a workaround for this.

___
### IsCollectibleAnimFinished () {: aria-label='Functions' }
#### boolean IsCollectibleAnimFinished ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, string Animation ) {: .copyable aria-label='Functions' }

___
### IsCollectibleCostumeVisible () {: aria-label='Functions' }
#### boolean IsCollectibleCostumeVisible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, int PlayerSpriteLayerID ) {: .copyable aria-label='Functions' }
#### boolean IsCollectibleCostumeVisible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, string PlayerSpriteLayerName ) {: .copyable aria-label='Functions' }

___
### IsEntityValidTarget () {: aria-label='Functions' }
#### boolean IsEntityValidTarget ( [Entity](Entity.md) Entity ) {: .copyable aria-label='Functions' }

___
### IsFootstepFrame () {: aria-label='Functions' }
#### boolean IsFootstepFrame ( int Foot = -1 ) {: .copyable aria-label='Functions' }       

___
### IsHeadless () {: aria-label='Functions' }
#### boolean IsHeadless ( ) {: .copyable aria-label='Functions' }

___
### IsHologram () {: aria-label='Functions' }
#### boolean IsHologram ( ) {: .copyable aria-label='Functions' }

___
### IsInvisible () {: aria-label='Functions' }
#### boolean IsInvisible ( ) {: .copyable aria-label='Functions' }

___
### IsItemCostumeVisible () {: aria-label='Functions' }
#### boolean IsItemCostumeVisible ( [ItemConfig_Item](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html) Item, int PlayerSpriteLayerID ) {: .copyable aria-label='Functions' }
#### boolean IsItemCostumeVisible ( [ItemConfig_Item](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html) Item, int PlayerSpriteLayerName ) {: .copyable aria-label='Functions' }

___
### IsLocalPlayer () {: aria-label='Functions' }
#### boolean IsLocalPlayer ( ) {: .copyable aria-label='Functions' }

___
### IsPacifist () {: aria-label='Functions' }
#### boolean IsPacifist ( ) {: .copyable aria-label='Functions' }

___
### IsUrethraBlocked () {: aria-label='Functions' }
#### boolean IsUrethraBlocked ( ) {: .copyable aria-label='Functions' }
Returns true when the player can no longer shoot due to charging the [Kidney Stone](https://bindingofisaacrebirth.fandom.com/wiki/Kidney_Stone) item.

___
### MorphToCoopGhost () {: aria-label='Functions' }
#### void MorphToCoopGhost ( ) {: .copyable aria-label='Functions' }

___
### PlayCollectibleAnim () {: aria-label='Functions' }
#### void PlayCollectibleAnim ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, boolean CheckBodyLayers, string AnimationName, int Frame = -1 ) {: .copyable aria-label='Functions' }

___
### PlayDelayedSFX () {: aria-label='Functions' }
#### void PlayDelayedSFX ( [SoundEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) ID, int SoundDelay = 0, int FrameDelay = 2, float Volume = 1.0 ) {: .copyable aria-label='Functions' }

___
### RemoveCollectibleByHistoryIndex () {: aria-label='Functions' }
#### void RemoveCollectibleByHistoryIndex ( int Index ) {: .copyable aria-label='Functions' }

___
### RemovePocketItem () {: aria-label='Functions' }
#### void RemovePocketItem ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### RemovePoopSpell () {: aria-label='Functions' }
#### void RemovePoopSpell ( int Position = 0 ) {: .copyable aria-label='Functions' }
Removes the poop spell from the specified queue position and shifts all spells after it forward to fill the space. A new spell is randomly picked to fill the last position.

___
### RerollAllCollectibles () {: aria-label='Functions' }
#### void RerollAllCollectibles ( [RNG](RNG.md) rng, boolean includeActiveItems ) {: .copyable aria-label='Functions' }

___
### ResetPlayer () {: aria-label='Functions' }
#### void ResetPlayer ( ) {: .copyable aria-label='Functions' }

???+ info "Info"
    This is used by the Genesis active item.

___
### ReviveCoopGhost () {: aria-label='Functions' }
#### boolean ReviveCoopGhost ( ) {: .copyable aria-label='Functions' }

___
### SalvageCollectible () {: aria-label='Functions' }
#### void SalvageCollectible ( [EntityPickup](EntityPickup.md) Pickup, [RNG](RNG.md) rng = PickupDropRNG, [ItemPoolType](https://wofsauge.github.io/IsaacDocs/rep/enums/ItemPoolType.html) Pool = ItemPoolType.POOL_NULL) {: .copyable aria-label='Functions' }
#### void SalvageCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, [Vector](Vector.md) position = playerPosition, [RNG](RNG.md) rng = PlayerDropRNG, [ItemPoolType](https://wofsauge.github.io/IsaacDocs/rep/enums/ItemPoolType.html) Pool = ItemPoolType.POOL_NULL ) {: .copyable aria-label='Functions' }

???+ info "Info"
    The provided [EntityPickup](EntityPickup.md) will be removed by this function. Use the override to avoid this.

___
### SetActiveVarData () {: aria-label='Functions' }
#### void SetActiveVarData ( int VarData, [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### SetBagOfCraftingContent () {: aria-label='Functions' }
#### void SetBagOfCraftingContent ( [BagOfCraftingPickup](enums/BagOfCraftingPickup.md)[] ContentTable ) {: .copyable aria-label='Functions' }
Sets the content of the bag to the content of the table. Table must use valid [BagOfCraftingPickup](enums/BagOfCraftingPickup.md) ids. Table can be shorter than 8, in which case the remaining indexes are set to empty.
___
### SetBagOfCraftingOutput () {: aria-label='Functions' }
#### void SetBagOfCraftingOutput ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }

___
### SetBagOfCraftingSlot () {: aria-label='Functions' }
#### void SetBagOfCraftingSlot ( int SlotID, int PickupID ) {: .copyable aria-label='Functions' }

___
### SetBladderCharge () {: aria-label='Functions' }
#### void SetBladderCharge ( int Charge ) {: .copyable aria-label='Functions' }
Used by the [Kidney Stone](https://bindingofisaacrebirth.fandom.com/wiki/Kidney_Stone) item.

???+ bug "Bug"
    The player's head turns pitch black when this function is used without Kidney Stone.

___
### SetCambionConceptionState () {: aria-label='Functions' }
#### void SetCambionConceptionState ( int State ) {: .copyable aria-label='Functions' }

___
### SetCanShoot () {: aria-label='Functions' }
#### boolean SetCanShoot ( boolean CanShoot ) {: .copyable aria-label='Functions' }
Instantaneously disables (or enables) the player's ability to shoot. The base game primarily uses this for special challenges.
___
### SetControllerIndex () {: aria-label='Functions' }
#### void SetControllerIndex ( int Idx ) {: .copyable aria-label='Functions' }        

___
### SetDamageModifier () {: aria-label='Functions' }
#### void SetDamageModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### SetEdenDamage () {: aria-label='Functions' }
#### void SetEdenDamage ( float Value ) {: .copyable aria-label='Functions' }

___
### SetEdenFireDelay () {: aria-label='Functions' }
#### void SetEdenFireDelay ( float Value ) {: .copyable aria-label='Functions' }

___
### SetEdenLuck () {: aria-label='Functions' }
#### void SetEdenLuck ( float Value ) {: .copyable aria-label='Functions' }

___
### SetEdenRange () {: aria-label='Functions' }
#### void SetEdenRange ( float Value ) {: .copyable aria-label='Functions' }

___
### SetEdenShotSpeed () {: aria-label='Functions' }
#### void SetEdenShotSpeed ( float Value ) {: .copyable aria-label='Functions' }

___
### SetEdenSpeed () {: aria-label='Functions' }
#### void SetEdenSpeed ( float Value ) {: .copyable aria-label='Functions' }

___
### SetEveSumptoriumCharge () {: aria-label='Functions' }
#### void SetEveSumptoriumCharge ( int ChargeNum ) {: .copyable aria-label='Functions' }

___
### SetFireDelayModifier () {: aria-label='Functions' }
#### void SetFireDelayModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### SetFootprintColor () {: aria-label='Functions' }
#### void SetFootprintColor ( [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor.html) color, boolean RightFoot = false ) {: .copyable aria-label='Functions' }

___
### SetImmaculateConceptionState () {: aria-label='Functions' }
#### void SetImmaculateConceptionState ( int State ) {: .copyable aria-label='Functions' }

___
### SetItemState () {: aria-label='Functions' }
#### void SetItemState ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }
This is used for thrown/2-input items (e.g. Bob's Rotten Head).

___
### SetKeepersSackBonus () {: aria-label='Functions' }
#### void SetKeepersSackBonus ( int Bonus ) {: .copyable aria-label='Functions' }

This is the number of coins spent while possessing [Keeper's Sack]((https://bindingofisaacrebirth.fandom.com/wiki/Keeper's_Sack).

___
### SetLaserColor () {: aria-label='Functions' }
#### void SetLaserColor ( [Color](Color.md) color ) {: .copyable aria-label='Functions' }

___
### SetLuckModifier () {: aria-label='Functions' }
#### void SetLuckModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### SetMaggySwingCooldown () {: aria-label='Functions' }
#### void SetMaggySwingCooldown ( int Cooldown ) {: .copyable aria-label='Functions' }

___
### SetMaxBladderCharge () {: aria-label='Functions' }
#### void SetMaxBladderCharge ( int Charge ) {: .copyable aria-label='Functions' }

___
### SetMegaBlastDuration () {: aria-label='Functions' }
#### void SetMegaBlastDuration ( int Duration ) {: .copyable aria-label='Functions' }

___
### SetNextUrethraBlockFrame () {: aria-label='Functions' }
#### void SetNextUrethraBlockFrame ( int Frame ) {: .copyable aria-label='Functions' }

___
### SetPonyCharge () {: aria-label='Functions' }
#### void SetPonyCharge ( int Time ) {: .copyable aria-label='Functions' }

___
### SetPoopSpell () {: aria-label='Functions' }
#### void SetPoopSpell ( int Slot, [PoopSpellType](https://wofsauge.github.io/IsaacDocs/rep/enums/PoopSpellType.html) PoopSpellType ) {: .copyable aria-label='Functions' }

___
### SetPurityState () {: aria-label='Functions' }
#### void SetPurityState ( [PurityState](enums/PurityState.md) State ) {: .copyable aria-label='Functions' }
Set the state in which the [Purity](https://bindingofisaacrebirth.fandom.com/wiki/Purity) item effect currently is.

___
### SetRedStewBonusDuration () {: aria-label='Functions' }
#### void SetRedStewBonusDuration ( int Duration ) {: .copyable aria-label='Functions' }

___
### SetShotSpeedModifier () {: aria-label='Functions' }
#### void SetShotSpeedModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### SetSpeedModifier () {: aria-label='Functions' }
#### void SetSpeedModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### SetTearPoisonDamage () {: aria-label='Functions' }
#### void SetTearPoisonDamage ( float Damage ) {: .copyable aria-label='Functions' }

___
### SetTearRangeModifier () {: aria-label='Functions' }
#### void SetTearRangeModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### SetUrethraBlock () {: aria-label='Functions' }
#### void SetUrethraBlock ( boolean Blocked ) {: .copyable aria-label='Functions' }

___
### SetWeapon () {: aria-label='Functions' }
#### void SetWeapon ( [Weapon](Weapon.md) Weapon, int WeaponSlot ) {: .copyable aria-label='Functions' }

___
### ShootBlueCandle () {: aria-label='Functions' }
#### void ShootBlueCandle ( [Vector](Vector.md) Direction ) {: .copyable aria-label='Functions' }

___
### ShuffleCostumes () {: aria-label='Functions' }
#### void ShuffleCostumes ( int Seed ) {: .copyable aria-label='Functions' }
Randomizes the current costumes.

___
### SpawnAquariusCreep () {: aria-label='Functions' }
#### [EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/EntityEffect.html) SpawnAquariusCreep ( [TearParams](https://wofsauge.github.io/IsaacDocs/rep/TearParams.html) TearParams = self.TearParams) {: .copyable aria-label='Functions' }
Spawns a creep effect that acts like the ones created by Aquarius, including inhereting the player's `TearParams`. Supports passing a custom `TearParams` instead.


???+ info "Info"
    For reference, this is how the game calculates the `TearParams` for this normally:
	
	``player->GetTearHitParams(&params, WeaponType.WEAPON_TEARS, (*player->GetTearPoisonDamage() * 0.666f) / player->_damage, -(int)(-Isaac::Random(2) != 0) & 2 - 1, nil)``

___
### SpawnClot () {: aria-label='Functions' }
#### void SpawnClot ( [Vector](Vector.md) pos, boolean AllowPlayerDeath = false ) {: .copyable aria-label='Functions' } 

Acts like a use of Sumptorium, removing health and spawning a clot with the type of health removed. If `AllowPlayerDeath` is set, a clot will spawn even if the health drained will kill the player.

___
### SpawnSaturnusTears () {: aria-label='Functions' }
#### int SpawnSaturnusTears ( ) {: .copyable aria-label='Functions' }

___
### SwapForgottenForm () {: aria-label='Functions' }
#### void SwapForgottenForm ( boolean Force = false, boolean NoEffects = false) {: .copyable aria-label='Functions' }
If the player has a [SubPlayer](EntityPlayer.md#getsubplayer), swaps between them. `Force` will swap even if the subplayer doesn't have any health, or while a room/stage transition is active. `NoEffects` will disable the dust effect & fade from white when switching from The Soul to The Forgotten.


???+ warning "Warning"
    This function currently only has an effect for The Forgotten and The Soul.

___
### SyncConsumableCounts () {: aria-label='Functions' }
#### void SyncConsumableCounts ( [EntityPlayer](EntityPlayer.md) Player, int CollectibleFlags ) {: .copyable aria-label='Functions' }      

___
### Teleport () {: aria-label='Functions' }
#### void Teleport ( [Vector](Vector.md) Position, boolean DoEffects = true, boolean TeleportTwinPlayers = false ) {: .copyable aria-label='Functions' }
Teleports the player to a position within the room. 

`DoEffects` controls whether the teleport animation and sound plays. `TeleportTwinPlayers` controls whether twin players (e.g. Esau, Tainted Lazarus w/ Birthright) are teleported alongside this one.

___
### TriggerRoomClear () {: aria-label='Functions' }
#### void TriggerRoomClear ( ) {: .copyable aria-label='Functions' }

___
### TryAddToBagOfCrafting () {: aria-label='Functions' }
#### void TryAddToBagOfCrafting ( [EntityPickup](EntityPickup.md) Pickup ) {: .copyable aria-label='Functions' }

___
### TryDecreaseGlowingHourglassUses () {: aria-label='Functions' }
#### void TryDecreaseGlowingHourglassUses ( int Uses, boolean ForceHourglass = false ) {: .copyable aria-label='Functions' }
`ForceHourglass` instantly removes all the charges and turns Glowing Hourglass into it's regular Hourglass form.

???+ bug "Bug"
	`Uses` are only decreased by 1 regardless of how large of a number you tell it to remove.
	
___
### TryFakeDeath () {: aria-label='Functions' }
#### boolean TryFakeDeath ( ) {: .copyable aria-label='Functions' }

___
### TryForgottenThrow () {: aria-label='Functions' }
#### boolean TryForgottenThrow ( [Vector](Vector.md) Direction ) {: .copyable aria-label='Functions' }        

___
### TryPreventDeath () {: aria-label='Functions' }
#### boolean TryPreventDeath ( ) {: .copyable aria-label='Functions' }
Adds a heart container to a character if there are none left, depending on its [HealthType](enums/HealthType.md).

Returns `true` on success, otherwise `false`.

___
### TryRemoveSmeltedTrinket () {: aria-label='Functions' }
#### void TryRemoveSmeltedTrinket ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) ID ) {: .copyable aria-label='Functions' }    

___
### UpdateIsaacPregnancy () {: aria-label='Functions' }
#### void UpdateIsaacPregnancy ( boolean UpdateCambion ) {: .copyable aria-label='Functions' }
Set ``true`` if you want to update the [Cambion Conception](https://bindingofisaacrebirth.fandom.com/wiki/Cambion_Conception) costume, otherwise updates the [Immaculate Conception](https://bindingofisaacrebirth.fandom.com/wiki/Immaculate_Conception) costume.

___
### VoidHasCollectible () {: aria-label='Functions' }
#### boolean VoidHasCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) ID ) {: .copyable aria-label='Functions' }

___
