---
tags:
  - Class
---
# Class "RoomConfigHolder"

???+ info
    You can get this class by using the RoomConfigHolder global table.
    ???+ example "Example Code"
        ```lua
        local roomconf = RoomConfigHolder.GetRoomByStageTypeAndVariant (StbType.BASEMENT,RoomType.ROOM_DEFAULT,1,0)
        ```

## Functions

### GetRandomRoom () {: aria-label='Functions' }
#### [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) GetRandomRoom ( int Seed, boolean ReduceWeight, [StbType](enums/StbType.md) Stage, [RoomType](https://wofsauge.github.io/IsaacDocs/rep/enums/RoomType.html) Type, [RoomShape](https://wofsauge.github.io/IsaacDocs/rep/enums/RoomShape.html) Shape = RoomShape.NUM_ROOMSHAPES, int MinVariant = 0, int MaxVariant = -1, int MinDifficulty = 0, int MaxDifficulty = 10, int RequiredDoors = 0, int Subtype = -1, int Mode = -1 ) {: .copyable aria-label='Functions' }
Returns a RoomConfigRoom corresponding to the given params.

___

### GetRoomByStageTypeAndVariant () {: aria-label='Functions' }
#### [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) GetRoomByStageTypeAndVariant ( [StbType](enums/StbType.md) Stage, [RoomType](https://wofsauge.github.io/IsaacDocs/rep/enums/RoomType.html) Type, int Variant, int Mode = -1 ) {: .copyable aria-label='Functions' }
Returns a RoomConfigRoom corresponding to the given params.

___
