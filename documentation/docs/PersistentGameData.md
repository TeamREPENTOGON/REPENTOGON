# Class "PersistentGameData"

???+ info
    You can get this class by using the following functions:

    * [Isaac:GetPersistentGameData](Isaac.md#getpersistentgamedata)

    ???+ example "Example Code"
        ```lua
        local persistentGameData = Isaac:GetPersistentGameData()
        ```
        
## Functions

### Try·Unlock () {: aria-label='Functions' }
#### boolean TryUnlock ( [Achievement](enums/Achievement.md) Unlock ) {: .copyable aria-label='Functions' }
Returns `true` if successful, `false` if unlocking failed or the secret was already unlocked.

### Unlocked () {: aria-label='Functions' }
#### boolean Unlocked ( [Achievement](enums/Achievement.md) Unlock ) {: .copyable aria-label='Functions' }
Checks if achievement is unlocked.

### Increase·Event·Counter () {: aria-label='Functions' }
#### void IncreaseEventCounter ( [EventCounter](enums/EventCounter.md) EventCounter, int Num ) {: .copyable aria-label='Functions' }

### Get·Event·Counter () {: aria-label='Functions' }
#### void GetEventCounter ( [EventCounter](enums/EventCounter.md) EventCounter ) {: .copyable aria-label='Functions' }

___

