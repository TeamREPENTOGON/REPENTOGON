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
#### void AddDebugFlags ( [DebugFlag](enums/DebugFlag.md) flag ) {: .copyable aria-label='Functions' }
Adds a debug flag to the game. Multiple can be added simultaneously with bitwise concatenation (e.g. `DebugFlag.ENTITY_POSITIONS | DebugFlag.HITSPHERES`)

___
### Devolve·Enemy () {: aria-label='Functions' }
#### void DevolveEnemy ( [Entity](Entity.md) ) {: .copyable aria-label='Functions' }

___
### Get·Challenge·Params () {: aria-label='Functions' }
#### [ChallengeParam](ChallengeParam.md) GetChallengeParams ( ) {: .copyable aria-label='Functions' }

___
### Get·Console () {: aria-label='Functions' }
#### [Console](Console.md) GetConsole ( ) {: .copyable aria-label='Functions' }

___
### Get·Current·Color·Modifier () {: aria-label='Functions' }
#### [ColorModifier](ColorModifier.md) GetCurrentColorModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·Debug·Flags () {: aria-label='Functions' }
#### int GetDebugFlags ( ) {: .copyable aria-label='Functions' }
Returns a [DebugFlag](enums/DebugFlag.md) bitmask.

___
### Get·Debug·Renderer () {: aria-label='Functions' }
#### [DebugRenderer](DebugRenderer.md) GetDebugRenderer ( ) {: .copyable aria-label='Functions' }
Used to render debug info like collision shapes.

___
### Get·Dimension () {: aria-label='Functions' }
#### int GetDimension ( ) {: .copyable aria-label='Functions' }

___
### Get·Lerp·Color·Modifier () {: aria-label='Functions' }
#### [ColorModifier](ColorModifier.md) GetLerpColorModifier ( ) {: .copyable aria-label='Functions' }

???+ info "Info"
    This is formatted as the absolute rate of change (ie, all values are positive).

___
### Get·Minimap () {: aria-label='Functions' }
#### [Minimap](Minimap.md) GetMinimap ( ) {: .copyable aria-label='Functions' }

___
### Get·Pause·Menu·State () {: aria-label='Functions' }
#### int GetPauseMenuState ( ) {: .copyable aria-label='Functions' }

___
### Get·Planetariums·Visited () {: aria-label='Functions' }
#### int GetPlanetariumsVisited ( ) {: .copyable aria-label='Functions' }
Returns the amount of planetariums the player has entered this run.

___
### Get·Player·Manager () {: aria-label='Functions' }
#### [PlayerManager](PlayerManager.md) GetPlayerManager ( ) {: .copyable aria-label='Functions' }

___
### Get·Procedural·Item·Manager () {: aria-label='Functions' }
#### [ProceduralItemManager](ProceduralItemManager.md) GetProceduralItemManager ( ) {: .copyable aria-label='Functions' }

___
### Get·Room·Config·Holder () {: aria-label='Functions' }
#### [RoomConfigHolder](RoomConfigHolder.md) GetRoomConfigHolder ( ) {: .copyable aria-label='Functions' }

___
### Get·Room·Transition () {: aria-label='Functions' }
#### [RoomTransition](RoomTransition.md) GetRoomTransition ( ) {: .copyable aria-label='Functions' }

___
### Get·Score·Sheet () {: aria-label='Functions' }
#### [ScoreSheet](ScoreSheet.md) GetScoreSheet ( ) {: .copyable aria-label='Functions' }

___
### Get·Stage·Transition () {: aria-label='Functions' }
#### [StageTransition](StageTransition.md) GetStageTransition ( ) {: .copyable aria-label='Functions' }

___
### Get·Target·Color·Modifier () {: aria-label='Functions' }
#### [ColorModifier](ColorModifier.md) GetTargetColorModifier ( ) {: .copyable aria-label='Functions' }

___
### Is·Erased () {: aria-label='Functions' }
#### boolean IsErased ( [Entity](Entity.md) entity) {: .copyable aria-label='Functions' }
#### boolean IsErased ( int type, int variant = -1, int subtype = -1 ) {: .copyable aria-label='Functions' }
Checks if an entity was erased.
___
### Is·Greed·Boss () {: aria-label='Functions' }
#### boolean IsGreedBoss ( ) {: .copyable aria-label='Functions' }

___
### Is·Greed·Final·Boss () {: aria-label='Functions' }
#### boolean IsGreedFinalBoss ( ) {: .copyable aria-label='Functions' }

___
### Is·Hard·Mode () {: aria-label='Functions' }
#### boolean IsHardMode ( ) {: .copyable aria-label='Functions' }
Returns `true` if hard mode or greedier

___
### Is·Pause·Menu·Open () {: aria-label='Functions' }
#### boolean IsPauseMenuOpen ( ) {: .copyable aria-label='Functions' }
Returns `true` if the pause menu is open

___
### Is·Rerun () {: aria-label='Functions' }
#### boolean IsRerun ( ) {: .copyable aria-label='Functions' }

___
### Set·Color·Modifier () {: aria-label='Functions' }
#### void SetColorModifier ( [ColorModifier](ColorModifier.md) colorModifier, boolean lerp = true, float rate = 0.015 ) {: .copyable aria-label='Functions' }

___
### Spawn·Bomb·Crater () {: aria-label='Functions' }
#### [Entity](Entity.md) SpawnBombCrater ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position ) {: .copyable aria-label='Functions' }

___