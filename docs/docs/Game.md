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
### Move·To·Random·Room () {: aria-label='Modified Functions' }
#### void MoveToRandomRoom ( boolean IAmErrorRoom, int Seed, [EntityPlayer](EntityPlayer.md) Player ) {: .copyable aria-label='Modified Functions' }
Now no longer crashes the game when given a seed equal `0`.
___
### StartStageTransition ()  {: aria-label='Modified Functions' }
#### void StartStageTransition ( boolean SameStage, int TransitionOverride, [EntityPlayer](EntityPlayer.md) Player = nil ) {: .copyable aria-label='Modified Functions' }
Fixed the crash that sometimes occurred due to an incorrect call on the C++ side.
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
### AddShopVisits () {: aria-label='Functions' }
#### void AddShopVisits ( int Count ) {: .copyable aria-label='Functions' }
Adds the amount of shops the player has entered this run.

___
### ClearErasedEnemies () {: aria-label='Functions' }
#### void ClearErasedEnemies ( ) {: .copyable aria-label='Functions' }
Clears out all enemies listed as an erased enemy, allowing them to spawn again.

___
### DevolveEnemy () {: aria-label='Functions' }
#### void DevolveEnemy ( [Entity](Entity.md) ) {: .copyable aria-label='Functions' }
Devolves an enemy, as if the item D10 was used on it.

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
### GetDizzyAmount () {: aria-label='Functions' }
#### float GetDizzyAmount ( ) {: .copyable aria-label='Functions' }
Returns the current dizzy amount akin to Wavy Cap

___
### GetLerpColorModifier () {: aria-label='Functions' }
#### [ColorModifier](ColorModifier.md) GetLerpColorModifier ( ) {: .copyable aria-label='Functions' }
Returns the lerped color modifier. This is formatted as the absolute rate of change (ie, all values are positive).

___
### GetPauseMenuState () {: aria-label='Functions' }
#### [PauseMenuStates](enums/PauseMenuStates.md) GetPauseMenuState ( ) {: .copyable aria-label='Functions' }

___
### GetPlanetariumsVisited () {: aria-label='Functions' }
#### int GetPlanetariumsVisited ( ) {: .copyable aria-label='Functions' }
Returns the amount of planetariums the player has entered this run.

___
### GetShopVisits () {: aria-label='Functions' }
#### int GetShopVisits ( ) {: .copyable aria-label='Functions' }
Returns the amount of shops the player has entered this run.

___
### GetTargetColorModifier () {: aria-label='Functions' }
#### [ColorModifier](ColorModifier.md) GetTargetColorModifier ( ) {: .copyable aria-label='Functions' }
Returns the target ColorModifier. If currently lerping between two [ColorModifier](ColorModifier.md) states, returns the target state. It is otherwise the same as [GetCurrentColorModifier](Game.md#getcurrentcolormodifier).

___
### IsErased () {: aria-label='Functions' }
#### boolean IsErased ( [Entity](Entity.md) Entity ) {: .copyable aria-label='Functions' }
#### boolean IsErased ( int Type, int Variant = -1, int SubType = -1 ) {: .copyable aria-label='Functions' }
Checks if an entity was erased.

___
### IsGreedBoss () {: aria-label='Functions' }
#### boolean IsGreedBoss ( ) {: .copyable aria-label='Functions' }
Returns `true` if the next or current wave is a boss wave. Returns `false` otherwise or if not in Greed Mode.

___
### IsGreedFinalBoss () {: aria-label='Functions' }
#### boolean IsGreedFinalBoss ( ) {: .copyable aria-label='Functions' }
Returns `true` if the next or current wave is the optional "nightmare" wave. Returns `false` otherwise or if not in Greed Mode.

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
Returns `true` if the current run is a rerun.

___
### RecordPlayerCompletion () {: aria-label='Functions' }
#### void RecordPlayerCompletion ( [CompletionType](enums/CompletionType.md) Type ) {: .copyable aria-label='Functions' }
Sets marks and unlocks achievements associated with this type for all players. Used by the game to award marks as well as tainted completion paper groups.

___
### SetBloom () {: aria-label='Functions' }
#### void SetBloom ( float Duration, float Amount ) {: .copyable aria-label='Functions' }

___
### SetColorModifier () {: aria-label='Functions' }
#### void SetColorModifier ( [ColorModifier](ColorModifier.md) ColorModifier, boolean Lerp = true, float Rate = 0.015 ) {: .copyable aria-label='Functions' }

___
### SetDizzyAmount () {: aria-label='Functions' }
#### void SetDizzyAmount ( float TargetIntensity , float CurrentIntensity ) {: .copyable aria-label='Functions' }
Sets the dizzy amount akin to Wavy Cap.

The current intensity of the effect will gradually move towards the "TargetIntensity".

Providing "CurrentIntensity" to this function is optional. If provided, the current intensity is instantly changed to that amount. If unspecified, the current intensity will remain unchanged.
???+ warning "Warning"
    Best to stay within `0`-`1` and increment by 0.1 while using this function. `1` has the most extreme effect on the screen while `0` removes the effect.

___
### SetDonationModAngel () {: aria-label='Functions' }
#### void SetDonationModAngel ( int Amount ) {: .copyable aria-label='Functions' }

___
### SetDonationModGreed () {: aria-label='Functions' }
#### void SetDonationModGreed ( int Amount ) {: .copyable aria-label='Functions' }

___
### ShowGenericLeaderboard () {: aria-label='Functions' }
#### void ShowGenericLeaderboard ( ) {: .copyable aria-label='Functions' }

___
### SpawnBombCrater () {: aria-label='Functions' }
#### [Entity](Entity.md) SpawnBombCrater ( [Vector](Vector.md) Position, float Radius = 1.0 ) {: .copyable aria-label='Functions' }

___