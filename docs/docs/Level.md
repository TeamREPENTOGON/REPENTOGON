---
tags:
  - Class
---
# Class "Level"

## Functions

### CanSpawnDoorOutline () {: aria-label='Functions' }
#### boolean CanSpawnDoorOutline ( int RoomIdx, [DoorSlot](https://wofsauge.github.io/IsaacDocs/rep/enums/DoorSlot.html) DoorSlot ) {: .copyable aria-label='Functions' }

___
### GetDimension () {: aria-label='Functions' }
#### [Dimension](enums/Dimension.md) GetDimension ( ) {: .copyable aria-label='Functions' }

___
### GetForceSpecialQuest () {: aria-label='Functions' }
#### [SpecialQuest](enums/SpecialQuest.md) GetForceSpecialQuest ( ) {: .copyable aria-label='Functions' }
If set, the level will automatically attempt to place the Knife Piece puzzle door for this [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html).
???+ info "Info"
	This is set to `SpecialQuest.DEFAULT` immediately before calling `MC_PRE_LEVEL_INIT`.

___
### HasAbandonedMineshaft () {: aria-label='Functions' }
#### boolean HasAbandonedMineshaft ( ) {: .copyable aria-label='Functions' }
Returns `true` if the floor has the mineshaft room used for the second Knife Piece puzzle.

___
### HasMirrorDimension () {: aria-label='Functions' }
#### boolean HasMirrorDimension ( ) {: .copyable aria-label='Functions' }
Returns `true` if the floor has the mirror dimension used for the first Knife Piece puzzle.

___
### HasPhotoDoor () {: aria-label='Functions' }
#### boolean HasPhotoDoor ( ) {: .copyable aria-label='Functions' }
Returns `true` if the floor has the photo door used to enter Mausoleum/Gehenna leading to the Ascent sequence.

___
### IsStageAvailable () {: aria-label='Functions' }
#### void IsStageAvailable ( [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) Level, [StageType](https://wofsauge.github.io/IsaacDocs/rep/enums/StageType.html) Stage ) {: .copyable aria-label='Functions' }

___
### PlaceRoom () {: aria-label='Functions' }
#### boolean PlaceRoom ( [LevelGeneratorEntry](LevelGeneratorEntry.md) Room, [RoomConfigRoom](https://wofsauge.github.io/IsaacDocs/rep/RoomConfig_Room.html) RoomConfig, int Seed ) {: .copyable aria-label='Functions' }
Places a room into the game. Returns `true` if successful.

___
### SetForceSpecialQuest () {: aria-label='Functions' }
#### void SetForceSpecialQuest ( [SpecialQuest](enums/SpecialQuest.md) Quest ) {: .copyable aria-label='Functions' }
Sets whether the level should attempt to place the Knife Piece puzzle door for this [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html).
???+ info "Info"
	This is set to `SpecialQuest.DEFAULT` immediately before calling `MC_PRE_LEVEL_INIT`.

___
### SetName () {: aria-label='Functions' }
#### void SetName ( string Name ) {: .copyable aria-label='Functions' }

___
