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
        local sprite = PlayerManager.AnyoneHasTrinket(TrinketType.TRINKET_SWALLOWED_PENNY)
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
#### boolean AnyPlayerTypeHasBirthright ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }

___
### FirstBirthrightOwner () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) FirstBirthrightOwner ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }

___
### FirstCollectibleOwner () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) FirstCollectibleOwner ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, RNG RNG, boolean Unknown ) {: .copyable aria-label='Functions' }
Returns `nil` if the specified collectible has never been picked up.

___
### FirstPlayerByType () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) FirstPlayerByType ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }

___
### FirstTrinketOwner () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) FirstTrinketOwner ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Trinket, RNG RNG, boolean Unknown ) {: .copyable aria-label='Functions' }
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
### SpawnCoPlayer2 () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) SpawnCoPlayer2 ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }

___
### TriggerRoomClear () {: aria-label='Functions' }
#### void TriggerRoomClear ( ) {: .copyable aria-label='Functions' }

___
