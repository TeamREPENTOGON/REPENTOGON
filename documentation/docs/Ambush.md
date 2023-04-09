# Class "Ambush"

???+ info
    You can get this class by using the following functions:

    * [Game:GetAmbush](Game.md#getambush)

    ???+ example "Example Code"
        ```lua
        local ambush = Game():GetAmbush()
        ```
        
## Functions

### Get·Current·Wave () {: aria-label='Functions' }
#### int GetCurrentWave ( ) {: .copyable aria-label='Functions' }

___
### Get·Max·Bossrush·Waves () {: aria-label='Functions' }
#### int GetMaxBossrushWaves ( ) {: .copyable aria-label='Functions' }
Gets the maximum amount of boss rush waves. In vanilla play this is always 15. Mods can increase or decrease this.

___
### Get·Max·Challenge·Waves () {: aria-label='Functions' }
#### int GetMaxChallengeWaves ( ) {: .copyable aria-label='Functions' }
Gets the maximum amount of normal challenge room waves. In vanilla play this is always 3. Mods can increase or decrease this.

___
### Set·Max·Bossrush·Waves () {: aria-label='Functions' }
#### void SetMaxBossrushWaves ( int Waves ) {: .copyable aria-label='Functions' }
Sets the maximum amount of boss rush waves. This caps out at 25 for now, until (if) inserting waves and whatnot is figured out.

___
### Set·Max·Challenge·Waves () {: aria-label='Functions' }
#### void SetMaxChallengeWaves ( int Waves ) {: .copyable aria-label='Functions' }
Sets how many waves should be in a normal challenge room.

???+ bug "Bug"
	Currently this value is not reset on game restart. This will be fixed as soon as we figure out how to cleanly run code on init on the C++ side!
	
___
### Spawn·Wave () {: aria-label='Functions' }
#### void SpawnWave ( ) {: .copyable aria-label='Functions' }

___
### Spawn·Bossrush·Wave () {: aria-label='Functions' }
#### void SpawnBossrush·Wave ( ) {: .copyable aria-label='Functions' }

___
### Start·Challenge () {: aria-label='Functions' }
#### void StartChallenge ( ) {: .copyable aria-label='Functions' }
Triggers Challenge/Boss Rush ambush.

___