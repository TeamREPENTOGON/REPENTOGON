---
tags:
  - Class
---
# Class "ItemConfig"

## Functions

### CanRerollCollectible () {: aria-label='Functions' }
#### boolean CanRerollCollectible ( int ID ) {: .copyable aria-label='Functions' }
Returns `true` if the collectible can be rerolled.

???- bug "Bug"
    Despite not being a static function, this function must be called with a `.`, like `Isaac.GetItemConfig().CanRerollCollectible(1)`.

___    
### GetTaggedItems () {: aria-label='Functions' }
#### [ItemConfig_Item](ItemConfig_Item.md)[] GetTaggedItems ( int Tags ) {: .copyable aria-label='Functions' }
Returns a table of [ItemConfig_Item](ItemConfig_Item.md) objects with the given tags

___
