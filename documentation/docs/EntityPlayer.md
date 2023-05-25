# Class "EntityPlayer"

This class contains both new functions and modified reimplementations of existing ones.

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Modified Functions

### Add·Cache·Flags () {: aria-label='Modified Functions' }
#### void AddCacheFlags ( [CacheFlag](https://wofsauge.github.io/IsaacDocs/rep/enums/CacheFlag.html) CacheFlag, boolean EvaluateItems = false ) {: .copyable aria-label='Modified Functions' }
Now accepts an optional `bool` to determine if [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html):EvaluateItems() should be automatically called after adding cache flags. In most cases, you'll want to do this.

___
### Get·Multi·Shot·Params () {: aria-label='Modified Functions' }
#### [MultiShotParams](MultiShotParams.md) GetMultiShotParams ( [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) WeaponType ) {: .copyable aria-label='Modified Functions' }
Now returns a proper `MultiShotParams` object.

___
### Get·Multi·Shot·Position·Velocity () {: aria-label='Modified Functions' }
#### [PosVel](https://wofsauge.github.io/IsaacDocs/rep/PosVel.html) GetMultiShotPositionVelocity ( int LoopIndex, [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) WeaponType, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) ShotDirection, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) ShotSpeed, [MultiShotParams](MultiShotParams.md) Params ) {: .copyable aria-label='Modified Functions' }
This magically vanished from the API some time after 1.7.8.

Compared to the vanilla function, this implementation has been further augmented to throw an error if LoopIndex is higher than [MultiShotParams:GetNumTears()](MultiShotParams.md#getnumtears).


## Functions

### Add·Active·Charge () {: aria-label='Functions' }
#### int AddActiveCharge ( int Charge, [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot, boolean Unknown, boolean Overcharge, boolean Force ) {: .copyable aria-label='Functions' }

Returns the true amount of charge added, which may have been capped by the targeted item's MaxCharge.

___
### Drop·Collectible () {: aria-label='Functions' }
#### void DropCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }

___
### Enable·Weapon·Type() {: aria-label='Functions' }
#### void EnableWeaponType ( [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) WeaponType, boolean Set ) {: .copyable aria-label='Functions' }

___
### Get·Active·Item·Desc () {: aria-label='Functions' }
#### [ActiveItemDesc](https://wofsauge.github.io/IsaacDocs/rep/PlayerTypes_ActiveItemDesc.html) GetActiveItemDesc ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot = ActiveSlot.SLOT_PRIMARY ) {: .copyable aria-label='Functions' }

___
### Get·Active·Max·Charge () {: aria-label='Functions' }
#### int GetActiveMaxCharge ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, int VarData ) {: .copyable aria-label='Functions' }

___
### Get·Active·Min·Usable·Charge () {: aria-label='Functions' }
#### int GetActiveMinUsableCharge ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### Get·Bag·Of·Crafting·Content () {: aria-label='Functions' }
#### table GetBagOfCraftingContent ( ) {: .copyable aria-label='Functions' }

___
### Get·Bag·Of·Crafting·Output () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) GetBagOfCraftingOutput ( ) {: .copyable aria-label='Functions' }

___
### Get·Bag·Of·Crafting·Slot () {: aria-label='Functions' }
#### int GetBagOfCraftingSlot ( int SlotID ) {: .copyable aria-label='Functions' }
Gets the current content of the bag in the given Slot ID.
___
### Get·D8·Damage·Modifier () {: aria-label='Functions' }
#### int GetD8DamageModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·D8·Speed·Modifier () {: aria-label='Functions' }
#### int GetD8SpeedModifier ( ) {: .copyable aria-label='Functions' }

___

### Get·D8·Range·Modifier () {: aria-label='Functions' }
#### int GetD8RangeModifier ( ) {: .copyable aria-label='Functions' }

___

### Get·D8·Fire·Delay·Modifier () {: aria-label='Functions' }
#### int GetD8FireDelayModifier ( ) {: .copyable aria-label='Functions' }

___

### Get·Damage·Modifier () {: aria-label='Functions' }
#### int GetDamageModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·Dead·Eye·Charge () {: aria-label='Functions' }
#### int GetDeadEyeCharge ( ) {: .copyable aria-label='Functions' }

___
### Get·Epiphora·Charge () {: aria-label='Functions' }
#### int GetEpiphoraCharge ( ) {: .copyable aria-label='Functions' }

___
### Get·Fire·Delay·Modifier () {: aria-label='Functions' }
#### int GetFireDelayModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·Health·Type () {: aria-label='Functions' }
#### [HealthType](enums/HealthType.md) GetHealthType ( ) {: .copyable aria-label='Functions' }

___
### Get·History () {: aria-label='Functions' }
#### [History](History.md) GetHistory ( ) {: .copyable aria-label='Functions' }

___
### Get·Marked·Target () {: aria-label='Functions' }
#### [EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/EntityEffect.html) GetMarkedTarget ( ) { : .copyable aria-label='Functions' }

Returns the entity effect representing the target of the [Marked](https://bindingofisaacrebirth.fandom.com/wiki/Marked) item. 
If the target is not displayed on the ground, this function returns nil.

___
### Get·Max·Pee·Burst·Cooldown () {: aria-label='Functions' }
#### int GetMaxPeeBurstCooldown ( ) {: .copyable aria-label='Functions' }

___
### Get·Metronome·Collectible·ID () {: aria-label='Functions' }
#### int GetMetronomeCollectibleID ( ) {: .copyable aria-label='Functions' }

___
### Get·Mega·Blast·Duration () {: aria-label='Functions' }
#### int GetMegaBlastDuration ( ) {: .copyable aria-label='Functions' }

___
### Get·Pee·Burst·Cooldown () {: aria-label='Functions' }
#### int GetPeeBurstCooldown ( ) {: .copyable aria-label='Functions' }

___
### Get·Red·Stew·Bonus·Duration () {: aria-label='Functions' }
#### int GetRedStewBonusDuration ( ) {: .copyable aria-label='Functions' }

___
### Get·Total·Active·Charge () {: aria-label='Functions' }
#### int GetTotalActiveCharge ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

_____
### Get·Weapon () {: aria-label='Functions' }
#### [Weapon](Weapon.md) GetWeapon ( ) {: .copyable aria-label='Functions' }

___
### Get·Weapon·Modifiers () {: aria-label='Functions' }
#### int GetWeaponModifiers ( ) {: .copyable aria-label='Functions' }

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
### Remove·Collectible·By·History·Index () {: aria-label='Functions' }
#### void RemoveCollectibleByHistoryIndex ( int Index ) {: .copyable aria-label='Functions' }

___
### Set·Active·Var·Data () {: aria-label='Functions' }
#### void SetActiveVarData ( int VarData, [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### Set·Bag·Of·Crafting·Content () {: aria-label='Functions' }
#### void SetBagOfCraftingContent ( table ContentTable ) {: .copyable aria-label='Functions' }
**NOT IMPLEMENTED**
___
### Set·Bag·Of·Crafting·Output () {: aria-label='Functions' }
#### void SetBagOfCraftingOutput ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) CollectibleID ) {: .copyable aria-label='Functions' }

___
### Set·Bag·Of·Crafting·Slot () {: aria-label='Functions' }
#### void SetBagOfCraftingSlot ( int SlotID, int PickupID ) {: .copyable aria-label='Functions' }

___
### Set·Can·Shoot () {: aria-label='Functions' }
#### boolean SetCanShoot ( boolean CanShoot ) {: .copyable aria-label='Functions' }
Instantaneously disables (or enables) the player's ability to shoot. The base game primarily uses this for special challenges.
___
### Set·Damage·Modifier () {: aria-label='Functions' }
#### void SetDamageModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### Set·Fire·Delay·Modifier () {: aria-label='Functions' }
#### void SetFireDelayModifier ( int Modifier ) {: .copyable aria-label='Functions' }

___
### Set·Item·State () {: aria-label='Functions' }
#### void SetItemState ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) CollectibleType ) {: .copyable aria-label='Functions' }
This is used for thrown/2-input items (e.g. Bob's Rotten Head).

___
### Set·Mega·Blast·Duration () {: aria-label='Functions' }
#### void SetMegaBlastDuration ( int Duration ) {: .copyable aria-label='Functions' }

___
### Set·Red·Stew·Bonus·Duration () {: aria-label='Functions' }
#### void SetRedStewBonusDuration ( int Duration ) {: .copyable aria-label='Functions' }

___
### Teleport () {: aria-label='Functions' }
#### void Teleport ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position, boolean DoEffects = true, boolean TeleportTwinPlayers = false ) {: .copyable aria-label='Functions' }
Teleports the player to a position within the room. 

`DoEffects` controls whether the teleport animation and sound plays. `TeleportTwinPlayers` controls whether twin players (e.g. Esau, Tainted Lazarus w/ Birthright) are teleported alongside this one.

___
### Try·Fake·Death () {: aria-label='Functions' }
#### boolean TryFakeDeath ( ) {: .copyable aria-label='Functions' }

___
### Try·Prevent·Death () {: aria-label='Functions' }
#### boolean TryPreventDeath ( ) {: .copyable aria-label='Functions' }
Adds a heart container to a character if there are none left, depending on its [HealthType](enums/HealthType.md).

Returns `true` on success, if not then `false`.

___
