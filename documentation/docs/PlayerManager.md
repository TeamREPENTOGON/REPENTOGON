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
Returns true when any player has the item and false when noone does.

___
### Get·Num·Collectibles () {: aria-label='Functions' }
#### int GetNumCollectibles ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }
Returns the number of collectibles held by all players.

___
### First·Collectible·Owner () {: aria-label='Functions' }
#### [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) FirstCollectibleOwner ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible, RNG RNG, boolean Unknown ) {: .copyable aria-label='Functions' }
Returns `nil` if the specified collectible has never been picked up.

___
### Is·Coop·Play () {: aria-label='Functions' }
#### boolean IsCoopPlay ( ) {: .copyable aria-label='Functions' }
Returns `true` if the game is in co-op mode.

___
### Spawn·Co·Player2 () {: aria-label='Functions' }
#### [EntityPlayer](https://wofsauge.github.io/IsaacDocs/rep/EntityPlayer.html) SpawnCoPlayer2 ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) ) {: .copyable aria-label='Functions' }

___
