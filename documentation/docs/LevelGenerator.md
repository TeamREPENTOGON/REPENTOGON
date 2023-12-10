# Class "LevelGenerator"

This class gives access to the level generator, the tool that generates a floor.
This class cannot be instanciated. The only way to access it is through callback parameters.

## Functions

### Get·All·Rooms () {: aria-label='Functions' }
#### [LevelGeneratorRoom](LevelGeneratorRoom.md)[] GetAllRooms ( ) {: .copyable aria-label='Functions' }
Returns a table of [LevelGeneratorRoom](LevelGeneratorRoom.md) representing all the rooms that have been generated so far.

___
### Get·Dead·Ends () {: aria-label='Functions' }
#### [LevelGeneratorRoom](LevelGeneratorRoom.md)[] GetDeadEnds ( ) {: .copyable aria-label='Functions' }
Returns a table of [LevelGeneratorRoom](LevelGeneratorRoom.md) representing all the dead end rooms that have been generated so far.

___
### Get·Non·Dead·Ends () {: aria-label='Functions' }
#### [LevelGeneratorRoom](LevelGeneratorRoom.md)[] GetNonDeadEnds ( ) {: .copyable aria-label='Functions' }
Returns a table of [LevelGeneratorRoom](LevelGeneratorRoom.md) representing all the non dead end rooms that have been generator so far.

___
### Place·Room () {: aria-label='Functions' }
#### int PlaceRoom ( int column, int row, int shape, [LevelGeneratorRoom](LevelGeneratorRoom.md) origin ) {: .copyable aria-label='Functions' }
Place a [LevelGeneratorRoom](LevelGeneratorRoom.md) object onto the level-grid. 
The `origin` parameter is used as the index of the connecting room. For instance, if you want to place a room on top of the starting room, then `origin` can be the starting room of the floor.

The column and row values need to be between 0 and 12 (inclusive).

Returns the GenerationIndex of the placed room. If placement was not possible, returns nil.

???+ note "Multiple origins"
    The `origin` parameter is used to ensure consistency with the existing rooms. During the generation phase, the game always picks a room 
    and starts placing rooms around it. This parameter reflects this behavior.
    
    When placing a room, you may be wondering which of its neighbors must act as the origin. This has consequences on the placement of special 
    rooms, as the game places special rooms in a specific order based on the distance between each dead end room and the start of the floor. 
    The distance from the start of the floor of the new room is equal to the distance from the start of the floor of `origin` plus one. 
    In practice you are free to chose any neighboring room as a point of origin. 
___