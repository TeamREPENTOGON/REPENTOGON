---
tags:
  - Class
---
# Class "EntityPickup"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### AddCollectibleCycle () {: aria-label='Functions' }
#### boolean AddCollectibleCycle ( int id ) {: .copyable aria-label='Functions' }

___
### GetDropDelay () {: aria-label='Functions' }
#### int GetDropDelay ( ) {: .copyable aria-label='Functions' }

___
### GetPriceSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetPriceSprite ( ) {: .copyable aria-label='Functions' }

___
### GetRandomPickupVelocity () {: aria-label='Functions' }
#### [Vector](Vector.md) GetRandomPickupVelocity ( [Vector](Vector.md) Position, [RNG](RNG.md) RNG = nil, int VelocityType = 0 ) {: .copyable aria-label='Functions' }

`VelocityType` seems to only affect pickups in Challenge Rooms, causing them to have a weaker velocity.

???+ warning "Warning"
    This is a static function and must be called via `EntityPickup.GetRandomPickupVelocity(Position, RNG, VelocityType)`.

___
### GetVarData () {: aria-label='Functions' }
#### int GetVarData ( ) {: .copyable aria-label='Functions' }

___
### IsBlind () {: aria-label='Functions' }
#### boolean IsBlind ( ) {: .copyable aria-label='Functions' }

Returns whether a collectible is being hidden by the Curse of the Blind question mark graphics. Always returns false for non-collectible EntityPickups.

___
### MakeShopItem () {: aria-label='Functions' }
#### void MakeShopItem ( int ShopItemID ) {: .copyable aria-label='Functions' }

___
### SetAlternatePedestal () {: aria-label='Functions' }
#### void SetAlternatePedestal ( int PedestalType ) {: .copyable aria-label='Functions' }

Sets the graphics of the item pedestal. Does nothing for non-collectible EntityPickups.

___
### SetDropDelay () {: aria-label='Functions' }
#### void SetDropDelay ( int Delay ) {: .copyable aria-label='Functions' }

___
### SetForceBlind () {: aria-label='Functions' }
#### void SetForceBlind ( boolean SetBlind ) {: .copyable aria-label='Functions' }

Hides pedestal items similar to Curse of the Blind. Does nothing for non-collectible EntityPickups.

___
### SetNewOptionsPickupIndex () {: aria-label='Functions' }
#### int SetNewOptionsPickupIndex ( ) {: .copyable aria-label='Functions' }
Returns the new pickup index.
___
### SetVarData () {: aria-label='Functions' }
#### void SetVarData ( int VarData ) {: .copyable aria-label='Functions' }

___
### TryFlip () {: aria-label='Functions' }
#### boolean TryFlip ( ) {: .copyable aria-label='Functions' }

___
### TryInitOptionCycle () {: aria-label='Functions' }
#### boolean TryInitOptionCycle ( int NumCycle ) {: .copyable aria-label='Functions' }

___
### TryRemoveCollectible () {: aria-label='Functions' }
#### boolean TryRemoveCollectible ( ) {: .copyable aria-label='Functions' }

Attempts to remove the collectible from an item pedestal.

Returns true if a collectible was successfully removed from the pedestal. Returns false if the pedestal was already empty, or if called on a non-collectible EntityPickup.

___