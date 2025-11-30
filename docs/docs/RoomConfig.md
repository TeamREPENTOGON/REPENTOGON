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
### GetStage () {: aria-label='Functions' }
#### [RoomConfigStage](RoomConfigStage.md) GetStage ( [StbType](enums/StbType.md) Stage ) {: .copyable aria-label='Functions' }

___
