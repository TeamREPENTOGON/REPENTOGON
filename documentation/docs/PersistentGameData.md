# Class "PersistentGameData"

???+ info
    You can get this class by using the following functions:

    * [Isaac:GetPersistentGameData](Isaac.md#getpersistentgamedata)

    ???+ example "Example Code"
        ```lua
        local persistentGameData = Isaac.GetPersistentGameData()
        ```
???+ warning "Warning"
    This class' functions should not be called until the game fully initializes! Make sure not to try using them outside of callbacks
    
## Functions

### Get·Bestiary·Death·Count () {: aria-label='Functions' }
#### void GetBestiaryDeathCount ( int entType, int entVariant ) {: .copyable aria-label='Functions' }

___
### Get·Bestiary·Encounter·Count () {: aria-label='Functions' }
#### void GetBestiaryEncounterCount ( int entType, int entVariant ) {: .copyable aria-label='Functions' }

___
### Get·Bestiary·Kill·Count () {: aria-label='Functions' }
#### void GetBestiaryKillCount ( int entType, int entVariant ) {: .copyable aria-label='Functions' }

___
### Get·Event·Counter () {: aria-label='Functions' }
#### void GetEventCounter ( [EventCounter](enums/EventCounter.md) EventCounter ) {: .copyable aria-label='Functions' }

___
### Increase·Event·Counter () {: aria-label='Functions' }
#### void IncreaseEventCounter ( [EventCounter](enums/EventCounter.md) EventCounter, int Num ) {: .copyable aria-label='Functions' }

___
### Is·Challenge·Completed () {: aria-label='Functions' }
#### boolean IsChallengeCompleted ( [Challenge](https://wofsauge.github.io/IsaacDocs/rep/enums/Challenge.html) ChallengeId ) {: .copyable aria-label='Functions' }
Checks if a challenge is completed.

___
### Is·Item·In·Collection () {: aria-label='Functions' }
#### boolean IsItemInCollection ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) CollectibleId ) {: .copyable aria-label='Functions' }
Checks if an item is in the collection. Aka. its at least picked up once in any run.

___
### Try·Unlock () {: aria-label='Functions' }
#### boolean TryUnlock ( [Achievement](enums/Achievement.md) Unlock ) {: .copyable aria-label='Functions' }
Returns `true` if successful, `false` if unlocking failed or the secret was already unlocked.

___
### Unlocked () {: aria-label='Functions' }
#### boolean Unlocked ( [Achievement](enums/Achievement.md) Unlock ) {: .copyable aria-label='Functions' }
Checks if achievement is unlocked.

___

