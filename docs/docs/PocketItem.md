---
tags:
  - Class
---
# Class "PocketItem"

???+ info
    You can get this class by using the following functions:

    * [EntityPlayer:GetPocketItem()](EntityPlayer.md#getpocketitem)

    ???+ example "Example Code"
        ```lua
        local pocket = Isaac.GetPlayer(0):GetPocketItem(0)
        ```
## Functions

### GetSlot () {: aria-label='Functions' }
#### int GetSlot ( ) {: .copyable aria-label='Functions' }
Returns the ID of the `PocketItem`. Returns `0` if the slot is empty.
For cards, returns [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html).
For pills, returns [PillEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/PillEffect.html).
For items, returns [CollectibleEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html).

___
### GetType () {: aria-label='Functions' }
#### [PocketItemType](PocketItem.md) GetType ( ) {: .copyable aria-label='Functions' }
Returns the [PocketItemType](PocketItem.md). Returns `1` if the slot is empty.

___