---
tags:
  - Class
---
# Class "LevelGeneratorRoom"

???+ info
    This class is used during level generation. Its purpose is to represent a slot in the graph of rooms that is generated during the generation phase.
    
    This class is immutable and cannot be instanciated manually: you cannot change the value of the fields of an instance and you cannot instance it by yourself.
    
    Access to instances is always performed through methods of the [LevelGenerator](LevelGenerator.md) itself, or instances are given as parameters of the callbacks:  
    
    * [MC_PRE_LEVEL_PLACE_ROOM](enums/ModCallbacks.md#mc_pre_level_place_room)

## Functions

### Column () {: aria-label='Functions' }
#### int Column ( ) {: .copyable aria-label='Functions' }
Return the column of the room slot on the level grid (index is `0` based).

___
### DoorMask () {: aria-label='Functions' }
#### int DoorMask ( ) {: .copyable aria-label='Functions' }
Return a mask of the available doors of the room slot.
In order to check if a door at a given slot is available, use the DoorSlot enumeration.

???+ example "Example"
    This tests if the DoorSlot `LEFT0` is available.
    ```lua
    if room:DoorMask() & (1 << DoorSlot.LEFT0) then
        print ("Room has a door on slot LEFT0)
    end
    ```

___
### GenerationIndex () {: aria-label='Functions' }
#### int GenerationIndex ( ) {: .copyable aria-label='Functions' }
Index of the room during generation: `0` if the room was the first to be generated, `1` if it was the second etc.

___
### Neighbors () {: aria-label='Functions' }
#### int[] Neighbors ( ) {: .copyable aria-label='Functions' }
Returns a table of the generation indices of the neighboring rooms.

___
### Row () {: aria-label='Functions' }
#### int Row ( ) {: .copyable aria-label='Functions' }
Return the row of the room slot on the level grid (index is `0` based).

___
### Shape () {: aria-label='Functions' }
#### int Shape ( ) {: .copyable aria-label='Functions' }
Return the shape of the room, as per the RoomShape enumeration.

___