---
tags:
  - Global
  - Class
---
# Global Class "RoomConfig"

???+ info
    You can get this class by using the RoomConfig global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local roomConfigStage = RoomConfig.GetStage(StbType.BASEMENT)
        ```
        
## Functions

### AddRooms () {: aria-label='Functions' }
#### [RoomConfigRoom](RoomConfigRoom.md)[] AddRooms ( [StbType](enums/StbType.md) Stage, int Mode, table[] Rooms ) {: .copyable aria-label='Functions' }

Adds the provided Lua Rooms to the specified RoomConfigSet. For details on how to generate Lua Rooms, refer to the [Custom StageAPI Github page](https://github.com/Meowlala/BOIStageAPI15/tree/master).

The function returns a table containing the placed RoomConfigRoom objects, in the same order as the input `Rooms` table. If a room at a given index could not be converted into a valid RoomConfigRoom, the corresponding entry in the returned table will be nil instead.

___
### GetRandomRoom () {: aria-label='Functions' }
#### [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) GetRandomRoom ( int Seed, boolean ReduceWeight, [StbType](enums/StbType.md) Stage, [RoomType](https://wofsauge.github.io/IsaacDocs/rep/enums/RoomType.html) Type, [RoomShape](https://wofsauge.github.io/IsaacDocs/rep/enums/RoomShape.html) Shape = RoomShape.NUM_ROOMSHAPES, int MinVariant = 0, int MaxVariant = -1, int MinDifficulty = 0, int MaxDifficulty = 10, int RequiredDoors = 0, int Subtype = -1, int Mode = -1 ) {: .copyable aria-label='Functions' }
Returns a RoomConfigRoom corresponding to the given params.

___

### GetRoomByStageTypeAndVariant () {: aria-label='Functions' }
#### [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) GetRoomByStageTypeAndVariant ( [StbType](enums/StbType.md) Stage, [RoomType](https://wofsauge.github.io/IsaacDocs/rep/enums/RoomType.html) Type, int Variant, int Mode = -1 ) {: .copyable aria-label='Functions' }
Returns a RoomConfigRoom corresponding to the given params.

___
### GetStage () {: aria-label='Functions' }
#### [RoomConfigStage](RoomConfigStage.md) GetStage ( [StbType](enums/StbType.md) Stage ) {: .copyable aria-label='Functions' }

___
