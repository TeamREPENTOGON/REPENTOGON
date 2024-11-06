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

### GetRandomRoom () {: aria-label='Functions' }
#### [RoomConfigRoom](RoomConfigRoom.md) GetRandomRoom ( boolean reduceWeight, int stage, int type, int shape = 13, int minVariant = 0, int maxVariant = -1, int minDifficulty = 0, int maxDifficulty = 10, int doors = 0, int subtype = -1, int mode = -1 ) {: .copyable aria-label='Functions' }

___
### GetRoomByStageTypeAndVariant () {: aria-label='Functions' }
#### [RoomConfigRoom](RoomConfigRoom.md) GetRoomByStageTypeAndVariant ( int type, int variant, int mode = -1 ) {: .copyable aria-label='Functions' }

___
### GetStage () {: aria-label='Functions' }
#### [RoomConfigStage](RoomConfigStage.md) GetStage ( [StbType](enums/StbType.md) Stage ) {: .copyable aria-label='Functions' }

___
