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
### CanReroll () {: aria-label='Functions' }
#### boolean CanReroll ( ) {: .copyable aria-label='Functions' }

___
### GetAlternatePedestal () {: aria-label='Functions' }
#### int GetAlternatePedestal ( ) {: .copyable aria-label='Functions' }

___
### GetCollectibleCycle () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)[] GetCollectibleCycle ( ) {: .copyable aria-label='Functions' }
Returns a table of all [CollectibleTypes](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) being used inside its collectible cycle (i.e. Cracked Crown).

___
### GetDropDelay () {: aria-label='Functions' }
#### int GetDropDelay ( ) {: .copyable aria-label='Functions' }

___
### GetLootList () {: aria-label='Functions' }
#### [LootList](LootList.md) GetLootList ( ) {: .copyable aria-label='Functions' }
Returns a **read-only** version of the pickup's [LootList](LootList.md). Loot inside pickups can be seen through use of the Guppy's Eye collectible.
___
### GetPickupGhost () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) GetPickupGhost ( ) {: .copyable aria-label='Functions' }
Returns the `EffectVariant.PICKUP_GHOST` EntityEffect visible through Guppy's Eye. If not visible, returns `nil`.

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
Returns `true` if the pickup is a collectible pedestal and is hidden. Always returns `false` for non-collectible EntityPickups.

???+ warning "Warning"
    This value does not account for curse of the blind, it only reflects the blind state of pickups that are normally blind without curses involved. Ex: alt path's extra item.
___
### MakeShopItem () {: aria-label='Functions' }
#### void MakeShopItem ( int ShopItemID ) {: .copyable aria-label='Functions' }

___
### RemoveCollectibleCycle () {: aria-label='Functions' }
#### void RemoveCollectibleCycle ( ) {: .copyable aria-label='Functions' }

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
Will try to flip the collectible, such as when using the Flip item on a collectible pedestal with a second, holographic collectible present behind the first one. Returns `true` if successful, `false` otherwise or if used on non-collectible EntityPickups.

___
### TryInitOptionCycle () {: aria-label='Functions' }
#### boolean TryInitOptionCycle ( int NumCycle ) {: .copyable aria-label='Functions' }
Causes the collectible pedestal to start cycling through the specified amount of collectibles, including its own collectible type.

___
### TryRemoveCollectible () {: aria-label='Functions' }
#### boolean TryRemoveCollectible ( ) {: .copyable aria-label='Functions' }
Attempts to remove the collectible from an item pedestal.

Returns `true` if a collectible was successfully removed from the pedestal. Returns `false` if the pedestal was already empty, or if called on a non-collectible EntityPickup.

___
### UpdatePickupGhost () {: aria-label='Functions' }
#### void UpdatePickupGhost ( ) {: .copyable aria-label='Functions' }
Updates the `EffectVariant.PICKUP_GHOST` EntityEffect in accordance to the pickup's current [LootList](LootList.md)

___