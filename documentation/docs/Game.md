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
### Get·Last·Devil·Room·Stage () {: aria-label='Modified Functions' }
#### [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) GetLastDevilRoomStage ( ) {: .copyable aria-label='Modified Functions' }
Now returns integer instead of unusable userdata.
___
### Get·Last·Level·With·Damage () {: aria-label='Modified Functions' }
#### [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) GetLastLevelWithDamage ( ) {: .copyable aria-label='Modified Functions' }
Now returns integer instead of unusable userdata.

___
### Get·Last·Level·Without·Half·Hp () {: aria-label='Modified Functions' }
#### [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) GetLastLevelWithoutHalfHp ( ) {: .copyable aria-label='Modified Functions' }
Now returns integer instead of unusable userdata.

___
### Start·Stage·Transition ()  {: aria-label='Modified Functions' }
#### void StartStageTransition ( boolean SameStage, int TransitionOverride, [EntityPlayer](EntityPlayer.md) player ) {: .copyable aria-label='Modified Functions' }
Fixed the crash that sometimes occured due to an incorrect call on the C++ side.

___

## Functions

### Achievement·Unlocks·Disallowed () {: aria-label='Functions' }
#### boolean AchievementUnlocksDisallowed ( ) {: .copyable aria-label='Functions' }
Returns `true` if achievements can't be unlocked this run (challenges, seeded, etc)

___
### Add·Debug·Flags () {: aria-label='Functions' }
#### void AddDebugFlags ( [BitwiseDebugFlag](enums/BitwiseDebugFlag.md) flag ) {: .copyable aria-label='Functions' }
Adds a debug flag to the game. Multiple can be added simultaneously with bitwise concatenation (e.g. `BitwiseDebugFlag.ENTITY_POSITIONS | BitwiseDebugFlag.HITSPHERES`)

___
### Force·Mega·Satan·Void·Portal() {: aria-label='Functions' }
#### void ForceMegaSatanVoidPortal ( boolean Force ) {: .copyable aria-label='Functions' }
Set to `true` to forcefully inhibit the Mega Satan ending without affecting the ability to get Mega Satan completion marks or achievements. Set to `false` to retain vanilla behavior.

(Might be better as a callback, somehow?)

___
### Devolve·Enemy () {: aria-label='Functions' }
#### void DevolveEnemy ( [Entity](Entity.md) ) {: .copyable aria-label='Functions' }

___
### Get·Console () {: aria-label='Functions' }
#### [Console](Console.md) GetConsole ( ) {: .copyable aria-label='Functions' }

___
### Get·Debug·Flag () {: aria-label='Functions' }
#### boolean GetDebugFlag ( [DebugFlag](enums/DebugFlag.md) flag ) {: .copyable aria-label='Functions' }
Returns the current state of the debug flag. If a script needs to check multiple debug flags quickly, it's recommended to use GetDebugFlags() instead.

___
### Get·Debug·Flags () {: aria-label='Functions' }
#### int GetDebugFlags ( ) {: .copyable aria-label='Functions' }
Returns a [DebugFlag](enums/DebugFlag.md) bitmask.

___
### Get·Dimension () {: aria-label='Functions' }
#### int GetDimension ( ) {: .copyable aria-label='Functions' }

___
### Get·Main·Menu () {: aria-label='Functions' }
#### [MainMenu](MainMenu.md) GetMainMenu ( ) {: .copyable aria-label='Functions' }

___
### Get·Minimap () {: aria-label='Functions' }
#### [Minimap](Minimap.md) GetMinimap ( ) {: .copyable aria-label='Functions' }

___
### Get·Pause·Menu·State () {: aria-label='Functions' }
#### int GetPauseMenuState ( ) {: .copyable aria-label='Functions' }

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
### Get·Room·Transition () {: aria-label='Functions' }
#### [RoomTransition](RoomTransition.md) GetRoomTransition ( ) {: .copyable aria-label='Functions' }

___
### Is·Hard·Mode () {: aria-label='Functions' }
#### boolean IsHardMode ( ) {: .copyable aria-label='Functions' }
Returns `true` if hard mode or greedier

___
### Is·Pause·Menu·Open () {: aria-label='Functions' }
#### boolean IsPauseMenuOpen ( ) {: .copyable aria-label='Functions' }
Returns `true` if the pause menu is open

___
### Spawn·Bomb·Crater () {: aria-label='Functions' }
#### [Entity](Entity.md) SpawnBombCrater ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position ) {: .copyable aria-label='Functions' }

___
### Toggle·Debug·Flag () {: aria-label='Functions' }
#### boolean ToggleDebugFlag ( [DebugFlag](enums/DebugFlag.md) flag ) {: .copyable aria-label='Functions' }
Toggles the given debug flag. Returns the state of the debug flag after toggling (under the hood we just return the value of GetDebugFlag() after running this).

__
