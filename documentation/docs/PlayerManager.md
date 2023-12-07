# Class "PlayerManager"

???+ info
    You can get this class by using the following functions:

    * [Game:GetPlayerManager](Game.md#getplayermanager)

    ???+ example "Example Code"
        ```lua
        local manager = Game():GetPlayerManager()
        ```
        
## Functions

### Anyone·Has·Collectible () {: aria-label='Functions' }
#### boolean AnyoneHasCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }
Returns true when any player has the item and false when no one does.

___
### Anyone·Has·Trinket () {: aria-label='Functions' }
#### boolean AnyoneHasTrinket ( [Trinket](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Trinket ) {: .copyable aria-label='Functions' }
Returns true when any player has the trinket and false when no one does.

___
### First·Collectible·Owner () {: aria-label='Functions' }
#### [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) FirstCollectibleOwner ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, RNG RNG, boolean Unknown ) {: .copyable aria-label='Functions' }
Returns `nil` if the specified collectible has never been picked up.

___
### First·Trinket·Owner () {: aria-label='Functions' }
#### [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) FirstTrinketOwner ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Trinket, RNG RNG, boolean Unknown ) {: .copyable aria-label='Functions' }
Returns `nil` if the specified trinket has never been picked up.

___
### Get·Esau·Jr·State () {: aria-label='Functions' }
#### [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) GetEsauJrState ( int Index = 0 ) {: .copyable aria-label='Functions' }

___
### Get·Num·Collectibles () {: aria-label='Functions' }
#### int GetNumCollectibles ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }
Returns the number of collectibles held by all players.

___
### Get·Players () {: aria-label='Functions' }
#### [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html)[] GetPlayers ( ) {: .copyable aria-label='Functions' }
Returns a table containing all players.

___
### Get·Total·Trinket·Multiplier () {: aria-label='Functions' }
#### int GetTotalTrinketMultiplier ( [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) Trinket ) {: .copyable aria-label='Functions' }

___
### Is·Coop·Play () {: aria-label='Functions' }
#### boolean IsCoopPlay ( ) {: .copyable aria-label='Functions' }
Returns `true` if the game is in co-op mode.

___
### Spawn·Co·Player2 () {: aria-label='Functions' }
#### [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) SpawnCoPlayer2 ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) PlayerType ) {: .copyable aria-label='Functions' }

___
### Trigger·Room·Clear () {: aria-label='Functions' }
#### void TriggerRoomClear ( ) {: .copyable aria-label='Functions' }

___
