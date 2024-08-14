---
tags:
  - Class
search:
  boost: 0.25
---
# Class "RoomConfigSet"

???+ info
    You can get this class by using the following function:

    * [RoomConfigStage.GetRoomSet()](RoomConfigStage.md#getroomset)

    ???+ example "Example Code"
        `local roomConfigSet = RoomConfig.GetStage(StbType.BASEMENT):GetRoomSet(0)`

## Operators
### __len () {: aria-label='Operators' }
[ ](#){: .abrep .tooltip .badge }
#### int __len ( ) {: .copyable aria-label='Operators' }

The length (#) operation. Returns the amount of entities in the list.

___
## Functions
### Get () {: aria-label='Functions' }
[ ](#){: .abrep .tooltip .badge }
#### [RoomConfigRoom](RoomConfigRoom.md) Get ( int idx ) {: .copyable aria-label='Functions' }

Returns a [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) at the index of the list provided.

___
## Variables
### Size {: aria-label='Variables' }
[ ](#){: .const .tooltip .badge } [ ](#){: .abrep .tooltip .badge }
#### const int Size  {: .copyable aria-label='Variables' }

The amount of entities in the list.

___