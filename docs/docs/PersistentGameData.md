---
tags:
  - Class
---
# Class "PersistentGameData"

???+ info
    You can get this class by using the following functions:

    * [Isaac.GetPersistentGameData()](Isaac.md#getpersistentgamedata)

    ???+ example "Example Code"
        ```lua
        local persistentGameData = Isaac.GetPersistentGameData()
        ```
???+ warning "Warning"
    This class' functions should not be called until the game fully initializes! Make sure not to try using them outside of callbacks
    
## Functions

### AddBossKilled () {: aria-label='Functions' }
#### void AddBossKilled ( [BossType](enums/BossType.md) Boss) {: .copyable aria-label='Functions' }
Marks this boss as killed and unlocks its relevant alternate floor if conditions are met.

___
### GetBestiaryDeathCount () {: aria-label='Functions' }
#### int GetBestiaryDeathCount ( int Type, int Variant ) {: .copyable aria-label='Functions' }

___
### GetBestiaryEncounterCount () {: aria-label='Functions' }
#### int GetBestiaryEncounterCount ( int Type, int Variant ) {: .copyable aria-label='Functions' }

___
### GetBestiaryKillCount () {: aria-label='Functions' }
#### int GetBestiaryKillCount ( int Type, int Variant ) {: .copyable aria-label='Functions' }

___
### GetEventCounter () {: aria-label='Functions' }
#### int GetEventCounter ( [EventCounter](enums/EventCounter.md) EventCounter ) {: .copyable aria-label='Functions' }

___
### IncreaseEventCounter () {: aria-label='Functions' }
#### void IncreaseEventCounter ( [EventCounter](enums/EventCounter.md) EventCounter, int Count ) {: .copyable aria-label='Functions' }

___
### IsBossKilled () {: aria-label='Functions' }
#### boolean IsBossKilled ( [BossType](enums/BossType.md) Boss) {: .copyable aria-label='Functions' }
Returns whether this boss was marked as killed (for purposes of unlock tracking).

___
### IsChallengeCompleted () {: aria-label='Functions' }
#### boolean IsChallengeCompleted ( [Challenge](https://wofsauge.github.io/IsaacDocs/rep/enums/Challenge.html) ChallengeId ) {: .copyable aria-label='Functions' }
Checks if a challenge is completed.

___
### IsItemInCollection () {: aria-label='Functions' }
#### boolean IsItemInCollection ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) CollectibleId ) {: .copyable aria-label='Functions' }
Checks if an item is in the collection. Aka. its at least picked up once in any run.

___
### TryUnlock () {: aria-label='Functions' }
#### boolean TryUnlock ( [Achievement](enums/Achievement.md) Unlock, boolean BlockPaperPopup = false ) {: .copyable aria-label='Functions' }
Returns `true` if successful, `false` if unlocking failed or the secret was already unlocked.

Setting `BlockPaperPopup` to `true` prevents popping up the achievement paper for modded achievements.

___
### Unlocked () {: aria-label='Functions' }
#### boolean Unlocked ( [Achievement](enums/Achievement.md) Unlock ) {: .copyable aria-label='Functions' }
Checks if achievement is unlocked.

___

