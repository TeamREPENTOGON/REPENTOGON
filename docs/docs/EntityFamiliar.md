---
tags:
  - Class
---
# Class "EntityFamiliar"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### CanBeDamagedByEnemies () {: aria-label='Functions' }
#### boolean CanBeDamagedByEnemies ( ) {: .copyable aria-label='Functions' }

___
### CanBeDamagedByLasers () {: aria-label='Functions' }
#### boolean CanBeDamagedByLasers ( ) {: .copyable aria-label='Functions' }

___
### CanBeDamagedByProjectiles () {: aria-label='Functions' }
#### boolean CanBeDamagedByProjectiles ( ) {: .copyable aria-label='Functions' }

___
### CanBlockProjectiles () {: aria-label='Functions' }
#### boolean CanBlockProjectiles ( ) {: .copyable aria-label='Functions' }

___
### CanCharm () {: aria-label='Functions' }
#### boolean CanCharm ( ) {: .copyable aria-label='Functions' }

___
### GetDirtColor () {: aria-label='Functions' }
#### [Color](Color.md) GetDirtColor ( ) {: .copyable aria-label='Functions' }

___
### GetFollowerPriority () {: aria-label='Functions' }
#### [FollowerPriority](enums/FollowerPriority.md) GetFollowerPriority ( ) {: .copyable aria-label='Functions' }

___
### GetItemConfig () {: aria-label='Functions' }
#### [ItemConfigItem](ItemConfig_Item.md) GetItemConfig ( ) {: .copyable aria-label='Functions' }
Returns the ItemConfigItem corresponding to the item that granted this familiar.

Returns nil if the familiar was not spawned by an item.

___
### GetMoveDelayNum () {: aria-label='Functions' }
#### int GetMoveDelayNum ( ) {: .copyable aria-label='Functions' }
Returns the amount in frames that the familiar's movements are delayed from the player's. 30 frames = 1 second.

___
### GetMultiplier () {: aria-label='Functions' }
#### float GetMultiplier ( ) {: .copyable aria-label='Functions' }
Returns the "multiplier" for the familiar, from effects such as BFFs or Hive Mind. Typically used to multiply things such as familiar damage.

___
### GetPathFinder () {: aria-label='Functions' }
#### [PathFinder](https://wofsauge.github.io/IsaacDocs/rep/PathFinder.html) GetPathFinder ( ) {: .copyable aria-label='Functions' }

___
### GetWeapon () {: aria-label='Functions' }
#### [Weapon](Weapon.md) GetWeapon ( ) {: .copyable aria-label='Functions' }
Returns `nil` for familiars that don't mimic the player's attacks (Incubus, etc).

___
### InvalidateCachedMultiplier () {: aria-label='Functions' }
#### void InvalidateCachedMultiplier ( ) {: .copyable aria-label='Functions' }
Makes it so that the next time [GetMultiplier](EntityFamiliar.md#getmultiplier) is called, [MC_EVALUATE_FAMILIAR_MULTIPLIER](enums/ModCallbacks.md#mc_evaluate_familiar_multiplier) is triggered to recalculate/allow modifying the multiplier.

___
### IsCharmed () {: aria-label='Functions' }
#### boolean IsCharmed ( ) {: .copyable aria-label='Functions' }

___
### RemoveFromPlayer () {: aria-label='Functions' }
#### void RemoveFromPlayer ( ) {: .copyable aria-label='Functions' }

___
### SetMoveDelayNum () {: aria-label='Functions' }
#### void SetMoveDelayNum ( int Delay ) {: .copyable aria-label='Functions' }
Sets the amount in frames that the familiar's movements are delayed from the player's. 30 frames = 1 second.

___
### TriggerRoomClear () {: aria-label='Functions' }
#### void TriggerRoomClear ( ) {: .copyable aria-label='Functions' }

___
### TryAimAtMarkedTarget () {: aria-label='Functions' }
#### [Vector](Vector.md) TryAimAtMarkedTarget ( [Vector](Vector.md) AimDirection, [Direction](https://wofsauge.github.io/IsaacDocs/rep/enums/Direction.html) Direction) {: .copyable aria-label='Functions' }
Returns `nil` if unsuccessful.
___
### UpdateDirtColor () {: aria-label='Functions' }
#### void UpdateDirtColor ( ) {: .copyable aria-label='Functions' }

___