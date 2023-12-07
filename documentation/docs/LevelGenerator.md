# Class "LevelGenerator"
        

## Functions

### Get·All·Rooms () {: aria-label='Functions' }
#### [LevelGeneratorRoom](LevelGeneratorRoom.md)[] GetAllRooms ( ) {: .copyable aria-label='Functions' }
Returns a table of [LevelGeneratorRoom](LevelGeneratorRoom.md) objects.

___
### Get·Dead·Ends () {: aria-label='Functions' }
#### [LevelGeneratorRoom](LevelGeneratorRoom.md)[] GetDeadEnds ( ) {: .copyable aria-label='Functions' }
Returns a table of [LevelGeneratorRoom](LevelGeneratorRoom.md) objects.

___
### Get·Non·Dead·Ends () {: aria-label='Functions' }
#### [LevelGeneratorRoom](LevelGeneratorRoom.md)[] GetNonDeadEnds ( ) {: .copyable aria-label='Functions' }
Returns a table of [LevelGeneratorRoom](LevelGeneratorRoom.md) objects.

___
### Place·Room () {: aria-label='Functions' }
#### int PlaceRoom ( int column, int row, int shape, [LevelGeneratorRoom](LevelGeneratorRoom.md) LGR ) {: .copyable aria-label='Functions' }
Place a [LevelGeneratorRoom](LevelGeneratorRoom.md) object onto the level-grid. 

The column and row values need to be between 0 and 12.

Returns the GenerationIndex of the placed room. If placement was not possible, returns nil.
___