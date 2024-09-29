---
tags:
  - Global
  - Class
---
# Global Class "Ambush"

???+ info
    You can get this class by using the `Ambush` global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local currwave = Ambush.GetCurrentWave()
        ```
        
## Functions

### GetCurrentWave () {: aria-label='Functions' }
#### int GetCurrentWave ( ) {: .copyable aria-label='Functions' }
Returns the current wave number of the current challenge room or boss rush room.

___
### GetMaxBossChallengeWaves () {: aria-label='Functions' }
#### int GetMaxBossChallengeWaves ( ) {: .copyable aria-label='Functions' }
Returns the maximum amount of boss challenge room waves. 

By default, the maximum amount of challenge room waves are `2`. It's important to note that mods can modify the maximum amount of challenge room waves.

___
### GetMaxBossrushWaves () {: aria-label='Functions' }
#### int GetMaxBossrushWaves ( ) {: .copyable aria-label='Functions' }
Returns the maximum amount of boss rush waves. 

By default, the maximum amount of boss rush waves are `15`. It's important to note that mods can modify the maximum amount of boss rush waves.

___
### GetMaxChallengeWaves () {: aria-label='Functions' }
#### int GetMaxChallengeWaves ( ) {: .copyable aria-label='Functions' }
Returns the maximum amount of challenge room waves. 

By default, the maximum amount of challenge room waves are `3`. It's important to note that mods can modify the maximum amount of challenge room waves.

___
### GetNextWave () {: aria-label='Functions' }
#### [RoomConfigRoom](RoomConfigRoom.md) GetNextWave ( ) {: .copyable aria-label='Functions' }
Returns the [RoomConfigRoom](RoomConfigRoom.md) of the next challenge room wave. Calling this function outside of a challenge room will result in an error.

___
### GetNextWaves () {: aria-label='Functions' }
#### [RoomConfigRoom](RoomConfigRoom.md)[] GetNextWaves ( ) {: .copyable aria-label='Functions' }
Returns a table containing the [RoomConfigRoom](RoomConfigRoom.md) of the next challenge room waves.

___
### SetMaxBossChallengeWaves () {: aria-label='Functions' }
#### void SetMaxBossChallengeWaves ( int Waves ) {: .copyable aria-label='Functions' }
Sets the maximum amount of boss challenge room waves.

???+ bug "Bug"
	Currently this value is not reset on game restart. This will be fixed as soon as we figure out how to cleanly run code on init on the C++ side!
	
___
### SetMaxBossrushWaves () {: aria-label='Functions' }
#### void SetMaxBossrushWaves ( int Waves ) {: .copyable aria-label='Functions' }
Sets the maximum amount of boss rush waves. As of now, there is a maximum cap of `25` waves.

___
### SetMaxChallengeWaves () {: aria-label='Functions' }
#### void SetMaxChallengeWaves ( int Waves ) {: .copyable aria-label='Functions' }
Sets the maximum amount of challenge room waves.

???+ bug "Bug"
	Currently this value is not reset on game restart. This will be fixed as soon as we figure out how to cleanly run code on init on the C++ side!
	
___
### SpawnBossrushWave () {: aria-label='Functions' }
#### void SpawnBossrushWave ( ) {: .copyable aria-label='Functions' }
Spawns a boss rush wave in the current room.

???+ bug "Bug"
	Calling this function will do nothing unless a boss rush has been triggered at least once during the current game session.

___
### SpawnWave () {: aria-label='Functions' }
#### void SpawnWave ( ) {: .copyable aria-label='Functions' }
Spawns a challenge room wave associated with the current floor.

???+ bug "Bug"
	Calling this function crashes the game if the current game mode is Greed or Greedier.

    The game also crashes if the current floor is Blue Womb.

___
### StartChallenge () {: aria-label='Functions' }
#### void StartChallenge ( ) {: .copyable aria-label='Functions' }
Triggers the challenge room or boss rush.

???+ bug "Bug"
	Calling this function outside of the boss rush room or a challenge room will do nothing except permanently close the doors, resulting in a softlock.

___