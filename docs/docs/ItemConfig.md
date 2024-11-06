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
### ClearAvailabilityCondition () {: aria-label='Functions' }
#### void ClearAvailabilityCondition ( ) {: .copyable aria-label='Functions' }

___
### GetAvailabilityCondition () {: aria-label='Functions' }
#### function GetAvailabilityCondition ( ) {: .copyable aria-label='Functions' }

___
### GetCustomCacheTags () {: aria-label='Functions' }
#### string GetCustomCacheTags ( ) {: .copyable aria-label='Functions' }

___
### GetCustomTags () {: aria-label='Functions' }
#### string GetCustomTags ( ) {: .copyable aria-label='Functions' }

___
### GetTaggedItems () {: aria-label='Functions' }
#### [ItemConfig_Item](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html)[] GetTaggedItems ( int Tags ) {: .copyable aria-label='Functions' }
Returns a table of [ItemConfig_Item](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html) objects with the given tags

???- bug "Bug"
    Calling this function with will crash the game. This will be fixed in the next update.

___
### HasCustomCacheTag () {: aria-label='Functions' }
#### boolean HasCustomCacheTag ( const tag ) {: .copyable aria-label='Functions' }

___
### HasCustomTag () {: aria-label='Functions' }
#### boolean HasCustomTag ( const tag ) {: .copyable aria-label='Functions' }

___
### IsValidTrinket () {: aria-label='Functions' }
#### boolean IsValidTrinket ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) TrinketType ) {: .copyable aria-label='Functions' }

___
### ModdedCardFront () {: aria-label='Functions' }
#### [Sprite](Sprite.md) ModdedCardFront ( ) {: .copyable aria-label='Functions' }

___
### SetAvailabilityCondition () {: aria-label='Functions' }
#### void SetAvailabilityCondition ( function condition ) {: .copyable aria-label='Functions' }

___

## Variables

### Hidden {: aria-label='Variables' }
#### boolean Hidden {: .copyable aria-label='Variables' }

___
### InitialWeight {: aria-label='Variables' }
#### float InitialWeight {: .copyable aria-label='Variables' }

___
