# Class "EntityPickup"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### Add·Collectible·Cycle () {: aria-label='Functions' }
#### boolean AddCollectibleCycle ( int id ) {: .copyable aria-label='Functions' }

___
### Can·Reroll () {: aria-label='Functions' }
#### boolean CanReroll ( ) {: .copyable aria-label='Functions' }

___
### Get·Drop·Delay () {: aria-label='Functions' }
#### int GetDropDelay ( ) {: .copyable aria-label='Functions' }

___
### Get·Price·Sprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetPriceSprite ( ) {: .copyable aria-label='Functions' }

___
### Get·Random·Pickup·Velocity () {: aria-label='Functions' }
#### [Vector](Vector.md) GetRandomPickupVelocity ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position, [RNG](RNG.md) rng, int velType = 0 ) {: .copyable aria-label='Functions' }

???+ warning "Warning"
    This is a static function and must be called via `EntityPickup.GetRandomPickupVelocity(position, rng, velType)`.

___
### Get·Var·Data () {: aria-label='Functions' }
#### int GetVarData ( ) {: .copyable aria-label='Functions' }

___
### Is·Blind () {: aria-label='Functions' }
#### boolean IsBlind ( ) {: .copyable aria-label='Functions' }

Returns whether a collectible is being hidden by the Curse of the Blind question mark graphics. Always returns false for non-collectible EntityPickups.

___
### Make·Shop·Item () {: aria-label='Functions' }
#### void MakeShopItem ( int shopItemID ) {: .copyable aria-label='Functions' }

___
### Set·Alternate·Pedestal () {: aria-label='Functions' }
#### void SetAlternatePedestal ( int PedestalType ) {: .copyable aria-label='Functions' }

Sets the graphics of the item pedestal. Does nothing for non-collectible EntityPickups.

___
### Set·Drop·Delay () {: aria-label='Functions' }
#### void SetDropDelay ( int Delay ) {: .copyable aria-label='Functions' }

___
### Set·Force·Blind () {: aria-label='Functions' }
#### void SetForceBlind ( boolean SetBlind ) {: .copyable aria-label='Functions' }

Hides pedestal items similar to Curse of the Blind. Does nothing for non-collectible EntityPickups.

___
### Set·New·Options·Pickup·Index () {: aria-label='Functions' }
#### int SetNewOptionsPickupIndex ( ) {: .copyable aria-label='Functions' }
Returns the new pickup index.
___
### Set·Var·Data () {: aria-label='Functions' }
#### void SetVarData ( int VarData ) {: .copyable aria-label='Functions' }

___
### Try·Flip () {: aria-label='Functions' }
#### boolean TryFlip ( ) {: .copyable aria-label='Functions' }

___
### Try·Init·Option·Cycle () {: aria-label='Functions' }
#### boolean TryInitOptionCycle ( int NumCycle ) {: .copyable aria-label='Functions' }

___
### Try·Remove·Collectible () {: aria-label='Functions' }
#### boolean TryRemoveCollectible ( ) {: .copyable aria-label='Functions' }

Attempts to remove the collectible from an item pedestal.

Returns true if a collectible was successfully removed from the pedestal. Returns false if the pedestal was already empty, or if called on a non-collectible EntityPickup.

___