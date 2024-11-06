---
tags:
  - Class
---
# Class "ItemConfig"

## Functions

### CanRerollCollectible () {: aria-label='Functions' }
#### boolean CanRerollCollectible ( int ID ) {: .copyable aria-label='Functions' }
Returns `true` if the collectible can be rerolled.

___
### GetTaggedItems () {: aria-label='Functions' }
#### [ItemConfig_Item](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html)[] GetTaggedItems ( int Tags ) {: .copyable aria-label='Functions' }
Returns a table of [ItemConfig_Item](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html) objects with the given tags

???- bug "Bug"
    Calling this function with will crash the game. This will be fixed in the next update.

___
### IsValidTrinket () {: aria-label='Functions' }
#### boolean IsValidTrinket ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) TrinketType ) {: .copyable aria-label='Functions' }

