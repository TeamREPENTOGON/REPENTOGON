# Class "EntitySlot"

It's Real.

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### Create·Drops·From·Explosion() {: aria-label='Functions' }
#### void CreateDropsFromExplosion ( ) {: .copyable aria-label='Functions' }
This forces the EntitySlot to drop what it typically would if blown up.

___
### Set·Prize·Collectible() {: aria-label='Functions' }
#### void SetPrizeCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) ID ) {: .copyable aria-label='Functions' }
Seems to only be used by Crane Game and Hell Game. This sets the collectible that the games will pay out with, and updates the rendered collectible accordingly.

___
### Random·Coin·Jam·Anim() {: aria-label='Functions' }
#### string RandomCoinJamAnim ( ) {: .copyable aria-label='Functions' }
Returns a random string from the following options: `CoinJam`, `CoinJam2`, `CoinJam3,` `CoinJam4`. Presumably only used on the donation machines.

___