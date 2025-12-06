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
### AddRooms () {: aria-label='Functions' }
#### [RoomConfigRoom](RoomConfigRoom.md)[] AddRooms ( table[] Rooms ) {: .copyable aria-label='Functions' }

Adds the provided Lua Rooms to the RoomConfigSet. For details on how to generate Lua Rooms, refer to the [Custom StageAPI Github page](https://github.com/Meowlala/BOIStageAPI15/tree/master).

The function returns a table containing the placed RoomConfigRoom objects, in the same order as the input `Rooms` table. If a room at a given index could not be converted into a valid RoomConfigRoom, the corresponding entry in the returned table will be nil instead.

___
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