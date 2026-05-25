---
tags:
  - Class
---
# Class "History"

???+ info
    You can get this class by using the following functions:

    * [EntityPlayer:GetHistory()](EntityPlayer.md#gethistory)

    ???+ example "Example Code"
        ```lua
        local history = Isaac.GetPlayer(0):GetHistory()
        ```
        
## Functions

### GetCollectiblesHistory () {: aria-label='Functions' }
#### [HistoryItems](HistoryItem.md)[] GetCollectiblesHistory ( ) {: .copyable aria-label='Functions' }
Returns table of [HistoryItems](HistoryItem.md) for the player's collectibles AND smelted trinkets.

___
### RemoveHistoryItemByIndex () {: aria-label='Functions' }
#### boolean RemoveHistoryItemByIndex ( int Index ) {: .copyable aria-label='Functions' }
Removes an item from the item history tracker on the right-hand side of the screen. Note that this does not remove the item effect from the player.

Returns `true` if an item was removed, `false` if not.

___
### SearchCollectibles () {: aria-label='Functions' }
#### [HistoryItems](HistoryItem.md)[] SearchCollectibles ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)[] IDs = nil ) {: .copyable aria-label='Functions' }
Returns only [HistoryItems](HistoryItem.md) for the provided [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)(s). A single ID may be passed in place of a table.

If no collectibles are specified, returns all collectibles (no smelted trinkets included).

___
### SearchTrinkets () {: aria-label='Functions' }
#### [HistoryItems](HistoryItem.md)[] SearchTrinkets ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html)[] = nil ) {: .copyable aria-label='Functions' }
Returns only [HistoryItems](HistoryItem.md) for the provided (smelted) [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html)(s). A single ID may be passed in place of a table.

If no trinkets are specified, returns all smelted trinkets (no collectibles included).

___
