# Class "ItemPool"

## Functions

### Get·Card·Ex () {: aria-label='Functions' }
#### [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) GetCardEx ( int Seed, int SpecialChance, int RuneChance, int SuitChance, boolean AllowNonCards ) {: .copyable aria-label='Functions' }
More sophisticated version of [ItemPool:GetCard()](https://wofsauge.github.io/IsaacDocs/rep/ItemPool.html#getcard) that allows to define individual chances.
___
### Get·Collectible·From·List () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) GetCollectibleType ( table ItemList, int Seed = Random(), [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) DefaultItem = CollectibleType.COLLECTIBLE_BREAKFAST, boolean AddToBlacklist = true, boolean ExcludeLockedItems = false ) {: .copyable aria-label='Functions' }

___
### Get·Collectibles·From·Pool () {: aria-label='Functions' }
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
### Get·Num·Available·Trinkets () {: aria-label='Functions' }
#### int GetNumAvailableTrinkets ( ) {: .copyable aria-label='Functions' }
Returns the amount of trinkets available in the item pool.

___
### Get·Removed·Collectibles () {: aria-label='Functions' }
#### table GetRemovedCollectibles ( ) {: .copyable aria-label='Functions' }
Returns a table of [collectibles](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) removed from all pools.

___

### Has·Collectible () {: aria-label='Functions' }
#### boolean HasCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }
Returns ``true`` if collectible is available in item pools, ``false`` otherwise.

___
