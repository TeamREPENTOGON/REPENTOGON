---
tags:
  - Global
  - Class
---
# Global Class "PlayerManager"

???+ info
    You can get this class by using the `PlayerManager` global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local hasTrinket = PlayerManager.AnyoneHasTrinket(TrinketType.TRINKET_SWALLOWED_PENNY)
        ```

## Functions

### AnyoneHasCollectible () {: aria-label='Functions' }
#### boolean AnyoneHasCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }
Returns true when any player has the item and false when no one does.

___
### AnyoneHasTrinket () {: aria-label='Functions' }
#### boolean AnyoneHasTrinket ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Trinket ) {: .copyable aria-label='Functions' }
Returns true when any player has the trinket and false when no one does.

___
### AnyoneIsPlayerType () {: aria-label='Functions' }
#### boolean AnyoneIsPlayerType ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }

___
### AnyPlayerTypeHasBirthright () {: aria-label='Functions' }
#### boolean AnyPlayerTypeHasBirthright ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType, ) {: .copyable aria-label='Functions' }

___
### AnyPlayerTypeHasCollectible () {: aria-label='Functions' }
#### boolean AnyPlayerTypeHasCollectible ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType, [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, boolean IgnoreModifiers = false ) {: .copyable aria-label='Functions' }

___
### AnyPlayerTypeHasTrinket () {: aria-label='Functions' }
#### boolean AnyPlayerTypeHasTrinket ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType, [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Trinket, boolean IgnoreModifiers = false ) {: .copyable aria-label='Functions' }

___
### FirstBirthrightOwner () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) FirstBirthrightOwner ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }

___
### FirstCollectibleOwner () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) FirstCollectibleOwner ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, boolean LazSharedGlobalTag = true ) {: .copyable aria-label='Functions' }
Returns `nil` if the specified collectible has never been picked up.

___
### FirstPlayerByType () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) FirstPlayerByType ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }

___
### FirstTrinketOwner () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) FirstTrinketOwner ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Trinket, [RNG](RNG.md) RNG = nil, boolean LazSharedGlobalTag = true ) {: .copyable aria-label='Functions' }
Returns `nil` if the specified trinket has never been picked up.

___
### GetEsauJrState () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) GetEsauJrState ( int Index = 0 ) {: .copyable aria-label='Functions' }

___
### GetNumCollectibles () {: aria-label='Functions' }
#### int GetNumCollectibles ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }
Returns the number of collectibles held by all players.

___
### GetPlayers () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md)[] GetPlayers ( ) {: .copyable aria-label='Functions' }
Returns a table containing all players.

___
### GetTotalTrinketMultiplier () {: aria-label='Functions' }
#### int GetTotalTrinketMultiplier ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Trinket ) {: .copyable aria-label='Functions' }

___
### IsCoopPlay () {: aria-label='Functions' }
#### boolean IsCoopPlay ( ) {: .copyable aria-label='Functions' }
Returns `true` if the game is in co-op mode.

___
### RemoveCoPlayer () {: aria-label='Functions' }
#### void RemoveCoPlayer ( [EntityPlayer](EntityPlayer.md) Player ) {: .copyable aria-label='Functions' }
Can be used to safely remove extra player entities, such as Strawman.

???- info "Tip"
    Don't pass the main player to this!

___
### SpawnCoPlayer2 () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) SpawnCoPlayer2 ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }

___
### SpawnSelectedBaby () {: aria-label='Functions' }
#### void SpawnSelectedBaby ( [BabySubType](https://wofsauge.github.io/IsaacDocs/rep/enums/BabySubType.html) BabyType, int ControllerIndex ) {: .copyable aria-label='Functions' }

___
### TriggerRoomClear () {: aria-label='Functions' }
#### void TriggerRoomClear ( ) {: .copyable aria-label='Functions' }

___
