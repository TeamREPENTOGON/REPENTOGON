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

___
### Drop·Collectible () {: aria-label='Functions' }
#### void DropCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }

___
### Get·Active·Max·Charge () {: aria-label='Functions' }
#### int GetActiveMaxCharge ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, int VarData ) {: .copyable aria-label='Functions' }

___
### Get·Active·Min·Usable·Charge () {: aria-label='Functions' }
#### int GetActiveMinUsableCharge ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### Get·Health·Type () {: aria-label='Functions' }
#### [HealthType](enums/HealthType.md) GetHealthType ( ) {: .copyable aria-label='Functions' }

___
### Get·Total·Active·Charge () {: aria-label='Functions' }
#### int GetTotalActiveCharge ( [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }

___
### Init·Post·Level·Init·Stats () {: aria-label='Functions' }
#### void InitPostLevelInitStats ( ) {: .copyable aria-label='Functions' }
Call this after spawning characters with "special" tears (Forgotten, Lilith, Azazel etc) with InitTwin, or they won't have their proper tear type.

___
### Init·Twin () {: aria-label='Functions' }
#### [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) InitTwin ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }
I believe this is used by J&E, Strawman etc

___
### Set·Active·Var·Data () {: aria-label='Functions' }
#### void SetActiveVarData ( int VarData, [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) Slot ) {: .copyable aria-label='Functions' }


___
### Set·Can·Shoot () {: aria-label='Functions' }
#### bool SetCanShoot ( boolean CanShoot ) {: .copyable aria-label='Functions' }
Instantaneously disables (or enables) the player's ability to shoot. The base game primarily uses this for special challenges.
___
### Set·Item·State () {: aria-label='Functions' }
#### void SetItemState ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) CollectibleType ) {: .copyable aria-label='Functions' }
This is used for thrown/2-input items (e.g. Bob's Rotten Head).


### Increment·Player·Form·Counter () {: aria-label='Functions' }
#### void IncrementPlayerFormCounter ( [PlayerForm](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Form, int Num ) {: .copyable aria-label='Functions' }


___
