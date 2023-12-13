---
tags:
  - Class
---
# Class "ItemPool"

## Functions

### CanSpawnCollectible () {: aria-label='Functions' }
#### boolean CanSpawnCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, boolean unkFlag ) {: .copyable aria-label='Functions' }

___
### GetCardEx () {: aria-label='Functions' }
#### [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) GetCardEx ( int Seed, int SpecialChance, int RuneChance, int SuitChance, boolean AllowNonCards ) {: .copyable aria-label='Functions' }
More sophisticated version of [ItemPool:GetCard()](https://wofsauge.github.io/IsaacDocs/rep/ItemPool.html#getcard) that allows to define individual chances.
___
### GetCollectibleFromList () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) GetCollectibleFromList ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)[] ItemList, int Seed = Random(), [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) DefaultItem = CollectibleType.COLLECTIBLE_BREAKFAST, boolean AddToBlacklist = true, boolean ExcludeLockedItems = false ) {: .copyable aria-label='Functions' }

___
### GetCollectiblesFromPool () {: aria-label='Functions' }
#### table GetCollectiblesFromPool ( [ItemPoolType](https://wofsauge.github.io/IsaacDocs/rep/enums/ItemPoolType.html) PoolType ) {: .copyable aria-label='Functions' }
Returns a table of collectibles registered in the specified pool. The table contains the following fields

|Field|Type|Comment|
|:--|:--|:--|
| decreaseBy | float | |
| initialWeight | float | |
| itemID | [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) | |
| removeOn | float | |
| weight | float | |

___
### GetNumAvailableTrinkets () {: aria-label='Functions' }
#### int GetNumAvailableTrinkets ( ) {: .copyable aria-label='Functions' }
Returns the amount of trinkets available in the item pool.

___
### GetPillColor () {: aria-label='Functions' }
#### [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) GetPillColor ( [PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.html) ID ) {: .copyable aria-label='Functions' }
Returns a PillColor matching the specificed PillEffect, returns -1 if the Effect is not in the rotation.

Currently not applied by pill modifications, such as PHD/False PHD.

___
### GetRemovedCollectibles () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)[] GetRemovedCollectibles ( ) {: .copyable aria-label='Functions' }
Returns a table of [collectibles](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) removed from all pools.

___
### GetRoomBlacklistedCollectibles () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)[] GetRoomBlacklistedCollectibles ( ) {: .copyable aria-label='Functions' }

___
### HasCollectible () {: aria-label='Functions' }
#### boolean HasCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }
Returns ``true`` if collectible is available in item pools, ``false`` otherwise.

___
### HasTrinket () {: aria-label='Functions' }
#### boolean HasTrinket ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Trinket ) {: .copyable aria-label='Functions' }
Returns ``true`` if trinket is currently available in trinket pool, ``false`` otherwise.

___
### UnidentifyPill () {: aria-label='Functions' }
#### void UnidentifyPill ( [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) Pill ) {: .copyable aria-label='Functions' }
Will reset a pill back to unidentified (???) state.

___