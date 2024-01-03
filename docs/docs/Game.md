---
tags:
  - Class
---
# Class "Game"

## Modified Functions


### GetLastDevilRoomStage () {: aria-label='Modified Functions' }
#### [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) GetLastDevilRoomStage ( ) {: .copyable aria-label='Modified Functions' }
Now returns integer instead of unusable userdata.

___
### GetLastLevelWithDamage () {: aria-label='Modified Functions' }
#### [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) GetLastLevelWithDamage ( ) {: .copyable aria-label='Modified Functions' }
Now returns integer instead of unusable userdata.

___
### GetLastLevelWithoutHalfHp () {: aria-label='Modified Functions' }
#### [LevelStage](https://wofsauge.github.io/IsaacDocs/rep/enums/LevelStage.html) GetLastLevelWithoutHalfHp ( ) {: .copyable aria-label='Modified Functions' }
Now returns integer instead of unusable userdata.

___
### GetPlayer ()  {: aria-label='Modified Functions' }
#### [EntityPlayer](EntityPlayer.md) GetPlayer ( int Index ) {: .copyable aria-label='Modified Functions' }
If no players exist, this now returns `nil` to prevent a crash. Invalid indexes will return index `0`.

___
### StartStageTransition ()  {: aria-label='Modified Functions' }
#### void StartStageTransition ( boolean SameStage, int TransitionOverride, [EntityPlayer](EntityPlayer.md) Player = nil ) {: .copyable aria-label='Modified Functions' }
Fixed the crash that sometimes occured due to an incorrect call on the C++ side.
`Player` is now optional (will default to `GetPlayer(0)`).

___

## Functions

### AchievementUnlocksDisallowed () {: aria-label='Functions' }
#### boolean AchievementUnlocksDisallowed ( ) {: .copyable aria-label='Functions' }
Returns `true` if achievements can't be unlocked this run (challenges, seeded, etc).

___
### AddDebugFlags () {: aria-label='Functions' }
#### void AddDebugFlags ( [DebugFlag](enums/DebugFlag.md) Flag ) {: .copyable aria-label='Functions' }
Adds a debug flag to the game. Multiple can be added simultaneously with bitwise concatenation (e.g. `DebugFlag.ENTITY_POSITIONS | DebugFlag.HITSPHERES`).

___
### DevolveEnemy () {: aria-label='Functions' }
#### void DevolveEnemy ( [Entity](Entity.md) ) {: .copyable aria-label='Functions' }

___
### GetChallengeParams () {: aria-label='Functions' }
#### [ChallengeParam](ChallengeParam.md) GetChallengeParams ( ) {: .copyable aria-label='Functions' }

___
### GetCurrentColorModifier () {: aria-label='Functions' }
#### [ColorModifier](ColorModifier.md) GetCurrentColorModifier ( ) {: .copyable aria-label='Functions' }
Gets a copy of the color correction introduced in Repentance. This stores the raw values currently being used (which can be affected by items such as Astral Projection) and not what the room is set to use (see [FXParams.ColorModifier](FXParams.md#colormodifier) for this).

___
### GetDebugFlags () {: aria-label='Functions' }
#### int GetDebugFlags ( ) {: .copyable aria-label='Functions' }
Returns a [DebugFlag](enums/DebugFlag.md) bitmask.

___
### GetDimension () {: aria-label='Functions' }
#### int GetDimension ( ) {: .copyable aria-label='Functions' }

___
### GetLerpColorModifier () {: aria-label='Functions' }
#### [ColorModifier](ColorModifier.md) GetLerpColorModifier ( ) {: .copyable aria-label='Functions' }

???+ info "Info"
    This is formatted as the absolute rate of change (ie, all values are positive).

___
### GetPauseMenuState () {: aria-label='Functions' }
#### int GetPauseMenuState ( ) {: .copyable aria-label='Functions' }

___
### GetPlanetariumsVisited () {: aria-label='Functions' }
#### int GetPlanetariumsVisited ( ) {: .copyable aria-label='Functions' }
Returns the amount of planetariums the player has entered this run.

___
### GetTargetColorModifier () {: aria-label='Functions' }
#### [ColorModifier](ColorModifier.md) GetTargetColorModifier ( ) {: .copyable aria-label='Functions' }
If currently lerping between two [ColorModifier](ColorModifier.md) states, returns the target state. Is otherwise the same as [GetCurrentColorModifier](Game.md#getcurrentcolormodifier). 

___
### IsErased () {: aria-label='Functions' }
#### boolean IsErased ( [Entity](Entity.md) Entity ) {: .copyable aria-label='Functions' }
#### boolean IsErased ( int Type, int Variant = -1, int SubType = -1 ) {: .copyable aria-label='Functions' }
Checks if an entity was erased.
___
### IsGreedBoss () {: aria-label='Functions' }
#### boolean IsGreedBoss ( ) {: .copyable aria-label='Functions' }

___
### IsGreedFinalBoss () {: aria-label='Functions' }
#### boolean IsGreedFinalBoss ( ) {: .copyable aria-label='Functions' }

___
### IsHardMode () {: aria-label='Functions' }
#### boolean IsHardMode ( ) {: .copyable aria-label='Functions' }
Returns `true` if current mode is Hard Mode or Greedier.

___
### IsPauseMenuOpen () {: aria-label='Functions' }
#### boolean IsPauseMenuOpen ( ) {: .copyable aria-label='Functions' }
Returns `true` if the pause menu is open.

___
### IsRerun () {: aria-label='Functions' }
#### boolean IsRerun ( ) {: .copyable aria-label='Functions' }

___
### SetColorModifier () {: aria-label='Functions' }
#### void SetColorModifier ( [ColorModifier](ColorModifier.md) ColorModifier, boolean Lerp = true, float Rate = 0.015 ) {: .copyable aria-label='Functions' }

___
### SpawnBombCrater () {: aria-label='Functions' }
#### [Entity](Entity.md) SpawnBombCrater ( [Vector](Vector.md) Position, float Radius = 1.0 ) {: .copyable aria-label='Functions' }

___
