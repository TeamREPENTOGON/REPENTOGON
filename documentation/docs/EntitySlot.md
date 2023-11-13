# Class "EntitySlot"

It's Real.

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### Create·Drops·From·Explosion() {: aria-label='Functions' }
#### void CreateDropsFromExplosion ( ) {: .copyable aria-label='Functions' }
This forces the EntitySlot to drop what it typically would if blown up.

___
### Get·Donation·Value() {: aria-label='Functions' }
#### int GetDonationValue ( ) {: .copyable aria-label='Functions' }

___
### Get·Prize·Type() {: aria-label='Functions' }
#### int GetPrizeType ( ) {: .copyable aria-label='Functions' }

___
### Get·State() {: aria-label='Functions' }
#### int GetState ( ) {: .copyable aria-label='Functions' }

___
### Get·Timeout() {: aria-label='Functions' }
#### int GetTimeout ( ) {: .copyable aria-label='Functions' }

___
### Get·Touch() {: aria-label='Functions' }
#### int GetTouch ( ) {: .copyable aria-label='Functions' }

___
### Random·Coin·Jam·Anim() {: aria-label='Functions' }
#### string RandomCoinJamAnim ( ) {: .copyable aria-label='Functions' }
Returns a random string from the following options: `CoinJam`, `CoinJam2`, `CoinJam3,` `CoinJam4`. Presumably only used on the donation machines.

___
### Set·Donation·Value() {: aria-label='Functions' }
#### void SetDonationValue ( int DonationValue ) {: .copyable aria-label='Functions' }

___
### Set·Prize·Collectible() {: aria-label='Functions' }
#### void SetPrizeCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) ID ) {: .copyable aria-label='Functions' }
Seems to only be used by Crane Game and Hell Game. This sets the collectible that the games will pay out with, and updates the rendered collectible accordingly.

___
### Set·Prize·Type() {: aria-label='Functions' }
#### void SetPrizeType ( int PrizeType ) {: .copyable aria-label='Functions' }

___
### Set·State() {: aria-label='Functions' }
#### void SetState ( int State ) {: .copyable aria-label='Functions' }

___
### Set·Timeout() {: aria-label='Functions' }
#### void SetTimeout ( int Timeout ) {: .copyable aria-label='Functions' }

___
### Set·Touch() {: aria-label='Functions' }
#### void SetTouch ( int Touch ) {: .copyable aria-label='Functions' }

___