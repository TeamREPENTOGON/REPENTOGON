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
For cards, returns [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html).

For pills, returns [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html).

For pocket active items, returns the corresponding [ActiveSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/ActiveSlot.html) (either `ActiveSlot.SLOT_POCKET` or `ActiveSlot.SLOT_POCKET2`).

Returns `0` if the slot is empty.

___
### GetType () {: aria-label='Functions' }
#### [PocketItemType](enums/PocketItemType.md) GetType ( ) {: .copyable aria-label='Functions' }
Returns the [PocketItemType](enums/PocketItemType.md). Returns `1` when the slot is empty.

___
