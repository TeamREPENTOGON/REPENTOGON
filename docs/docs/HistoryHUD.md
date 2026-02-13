---
tags:
  - Class
---
# Class "HistoryHUD"

???+ info
    You can get this class by using the following functions:

    * [HUD.GetHistoryHUD()](HUD.md#gethistoryhud)

    ???+ example "Example Code"
        ```lua
        local playerHud = Game():GetHUD():GetHistoryHUD(0)
        ```

## Functions

### GetCollectibleOffsets () {: aria-label='Functions' }
#### [Vector](Vector.md)[] GetCollectibleOffsets ( int PlayerIndex, [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)[] IDs = nil, boolean IncludeNotVisible = false ) {: .copyable aria-label='Functions' }
Variant of `GetCollectibles` that only returns the current rendering offsets of the items, relative to the [HistoryHUD](HistoryHUD.md). Provided for efficiency if that is all that you need.

___
### GetCollectibles () {: aria-label='Functions' }
#### [HistoryHUDItems](HistoryHUDItem.md)[] GetCollectibles ( int PlayerIndex, [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)[] IDs = nil, boolean IncludeNotVisible = false ) {: .copyable aria-label='Functions' }
Returns only [HistoryHUDItems](HistoryHUDItem.md) for the specified [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)s, or all collectibles if no filter IDs are provided (smelted trinkets are never included).

A single [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) can be used in place of the table.

___
### GetItems () {: aria-label='Functions' }
#### [HistoryHUDItems](HistoryHUDItem.md)[] Getitems ( int PlayerIndex, boolean IncludeNotVisible = false ) {: .copyable aria-label='Functions' }
By default, only returns items that would actually be rendered on the HUD, based on the current screen size and the [ExtraHUDStyle](https://wofsauge.github.io/IsaacDocs/rep/Options.html#extrahudstyle) settling.

___
### GetPlayer () {: aria-label='Functions' }
#### [EntityPlayer](EntityPlayer.md) GetPlayer ( int PlayerIndex ) {: .copyable aria-label='Functions' }
`PlayerIndex` is 0 or 1, with 1 being used only for "twins", such as Jacob & Esau.

___
### GetPosition () {: aria-label='Functions' }
#### [Vector](Vector.md) GetPosition ( ) {: .copyable aria-label='Functions' }

___
### GetTrinketOffsets () {: aria-label='Functions' }
#### [Vector](Vector.md)[] GetTrinketOffsets ( int PlayerIndex, [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html)[] IDs = nil, boolean IncludeNotVisible = false ) {: .copyable aria-label='Functions' }
Variant of `GetCollectibles` that only returns the current rendering offsets of the items, relative to the [HistoryHUD](HistoryHUD.md). Provided for efficiency if that is all that you need.

___
### GetTrinkets () {: aria-label='Functions' }
#### [HistoryHUDItems](HistoryHUDItem.md)[] GetTrinkets ( int PlayerIndex, [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html)[] IDs = nil, boolean IncludeNotVisible = false ) {: .copyable aria-label='Functions' }
Returns only [HistoryHUDItems](HistoryHUDItem.md) for the specified smelted [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html)s, or all collectibles if no filter IDs are provided (collectibles are never included).

A single [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) can be used in place of the table.

___
