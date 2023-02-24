# Class "EntityPlayer"

## Functions

### AddActiveCharge () {: aria-label='Functions' }
#### int AddActiveCharge ( int Charge, ActiveSlot Slot, bool Unknown, bool Overcharge, bool Force ) {: .copyable aria-label='Functions' }

___
### AddCacheFlags () {: aria-label='Functions' }
#### void AddCacheFlags ( CacheFlag CacheFlag, bool EvaluateItems = false ) {: .copyable aria-label='Functions' }
Now accepts an optional `bool` to determine if EntityPlayer:EvaluateItems() should be automatically called after adding cache flags. In most cases, you'll want to do this.

___
### DropCollectible () {: aria-label='Functions' }
#### void DropCollectible ( CollectibleType Collectible ) {: .copyable aria-label='Functions' }

___
### GetActiveMaxCharge () {: aria-label='Functions' }
#### int GetActiveMaxCharge ( CollectibleType Collectible, int VarData ) {: .copyable aria-label='Functions' }

___
### GetActiveMinUsableCharge () {: aria-label='Functions' }
#### int GetActiveMinUsableCharge ( ActiveSlot Slot ) {: .copyable aria-label='Functions' }

___
### GetHealthType () {: aria-label='Functions' }
#### HealthType GetHealthType ( ) {: .copyable aria-label='Functions' }

___
### GetMultiShotParams () {: aria-label='Functions' }
#### MultiShotParams GetMultiShotParams ( WeaponType WeaponType ) {: .copyable aria-label='Functions' }
Now returns a proper `MultiShotParams` object.

___
### GetMultiShotPositionVelocity () {: aria-label='Functions' }
#### PosVel GetMultiShotPositionVelocity ( int LoopIndex, WeaponType WeaponType, Vector ShotDirection, Vector ShotSpeed, MultiShotParams Params ) {: .copyable aria-label='Functions' }
This magically vanished from the API some time after 1.7.8.

Compared to the vanilla function, this implementation has been further augmented to throw an error if LoopIndex is higher than MultiShotParams.NumTears.

___
### GetTotalActiveCharge () {: aria-label='Functions' }
#### int GetTotalActiveCharge ( ActiveSlot Slot ) {: .copyable aria-label='Functions' }

___
### InitPostLevelInitStats () {: aria-label='Functions' }
#### void InitPostLevelInitStats ( ) {: .copyable aria-label='Functions' }
Call this after spawning characters with "special" tears (Forgotten, Lilith, Azazel etc) with InitTwin, or they won't have their proper tear type.

___
### InitTwin () {: aria-label='Functions' }
#### EntityPlayer InitTwin ( PlayerType PlayerType ) {: .copyable aria-label='Functions' }
I believe this is used by J&E, Strawman etc

___
### SetActiveVarData () {: aria-label='Functions' }
#### void SetActiveVarData ( int VarData, ActiveSlot Slot ) {: .copyable aria-label='Functions' }

___
### SetItemState () {: aria-label='Functions' }
#### void SetItemState ( CollectibleType CollectibleType ) {: .copyable aria-label='Functions' }
This is used for thrown/2-input items (e.g. Bob's Rotten Head).

___
