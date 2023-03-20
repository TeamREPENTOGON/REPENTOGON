# Class "Game"

This class contains both new functions and modified reimplementations of existing ones.

## Modified Functions

### Get·Ambush () {: aria-label='Modified Functions' }
#### [Ambush](Ambush.md) GetAmbush ( ) {: .copyable aria-label='Modified Functions' }
Now returns a proper [Ambush](Ambush.md) metatable.

___
### Get·Item·Overlay () {: aria-label='Modified Functions' }
#### [ItemOverlay](ItemOverlay.md) GetItemOverlay ( ) {: .copyable aria-label='Modified Functions' }
Now returns a proper [ItemOverlay](ItemOverlay.md) metatable.

___
### Get·Last·Devil·Room·Stage () {: aria-label='Functions' }
#### [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) GetLastDevilRoomStage ( ) {: .copyable aria-label='Functions' }
Now returns integer instead of unusable userdata.
___
### Get·Last·Level·With·Damage () {: aria-label='Functions' }
#### [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) GetLastLevelWithDamage ( ) {: .copyable aria-label='Functions' }
Now returns integer instead of unusable userdata.

___
### Get·Last·Level·Without·Half·Hp () {: aria-label='Functions' }
#### [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) GetLastLevelWithoutHalfHp ( ) {: .copyable aria-label='Functions' }
Now returns integer instead of unusable userdata.

___

## Functions

### Achievement·Unlocks·Disallowed () {: aria-label='Functions' }
#### boolean AchievementUnlocksDisallowed ( ) {: .copyable aria-label='Functions' }
Returns `true` if achievements can't be unlocked this run (challenges, seeded, etc)

___
### Get·Console () {: aria-label='Functions' }
#### [Console](Console.md) GetConsole ( ) {: .copyable aria-label='Functions' }

___
### Get·Debug·Flag () {: aria-label='Functions' }
#### boolean GetDebugFlag ( [DebugFlag](enums/DebugFlag.md) flag ) {: .copyable aria-label='Functions' }

___
### Get·Procedural·Item·Manager () {: aria-label='Functions' }
#### [ProceduralItemManager](ProceduralItemManager.md) GetProceduralItemManager ( ) {: .copyable aria-label='Functions' }

___
### Get·Planetariums·Visited () {: aria-label='Functions' }
#### int GetPlanetariumsVisited ( ) {: .copyable aria-label='Functions' }
Returns the amount of planetariums the player has entered this run.

___
### Get·Player·Manager () {: aria-label='Functions' }
#### [PlayerManager](PlayerManager.md) GetPlayerManager ( ) {: .copyable aria-label='Functions' }

___
### Get·Room·Config·Holder () {: aria-label='Functions' }
#### [RoomConfigHolder](RoomConfigHolder.md) GetRoomConfigHolder ( ) {: .copyable aria-label='Functions' }

___
### Is·Hard·Mode () {: aria-label='Functions' }
#### boolean IsHardMode ( ) {: .copyable aria-label='Functions' }
Returns `true` if hard mode or greedier

___
### Toggle·Debug·Flag () {: aria-label='Functions' }
#### boolean ToggleDebugFlag ( [DebugFlag](enums/DebugFlag.md) flag ) {: .copyable aria-label='Functions' }
Toggles the given debug flag. Returns the state of the debug flag after toggling (under the hood we just return the value of GetDebugFlag after running this).

__