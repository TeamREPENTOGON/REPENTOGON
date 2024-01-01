---
tags:
  - Class
---
# Class "EntitySlot"

It's Real.

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### CreateDropsFromExplosion () {: aria-label='Functions' }
#### void CreateDropsFromExplosion ( ) {: .copyable aria-label='Functions' }
This forces the EntitySlot to drop what it typically would if blown up.

___
### GetDonationValue () {: aria-label='Functions' }
#### int GetDonationValue ( ) {: .copyable aria-label='Functions' }

___
### GetPrizeType () {: aria-label='Functions' }
#### int GetPrizeType ( ) {: .copyable aria-label='Functions' }

___
### GetState () {: aria-label='Functions' }
#### int GetState ( ) {: .copyable aria-label='Functions' }

___
### GetTimeout () {: aria-label='Functions' }
#### int GetTimeout ( ) {: .copyable aria-label='Functions' }

___
### GetTouch () {: aria-label='Functions' }
#### int GetTouch ( ) {: .copyable aria-label='Functions' }

___
### RandomCoinJamAnim () {: aria-label='Functions' }
#### string RandomCoinJamAnim ( ) {: .copyable aria-label='Functions' }
Returns a random string from the following options: `CoinJam`, `CoinJam2`, `CoinJam3,` `CoinJam4`. Presumably only used on the donation machines.

___
### SetDonationValue () {: aria-label='Functions' }
#### void SetDonationValue ( int DonationValue ) {: .copyable aria-label='Functions' }

___
### SetPrizeCollectible () {: aria-label='Functions' }
#### void SetPrizeCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }
Seems to only be used by Crane Game and Hell Game. This sets the collectible that the games will pay out with, and updates the rendered collectible accordingly.

___
### SetPrizeType () {: aria-label='Functions' }
#### void SetPrizeType ( int PrizeType ) {: .copyable aria-label='Functions' }

___
### SetState () {: aria-label='Functions' }
#### void SetState ( int State ) {: .copyable aria-label='Functions' }

___
### SetTimeout () {: aria-label='Functions' }
#### void SetTimeout ( int Timeout ) {: .copyable aria-label='Functions' }

___
### SetTouch () {: aria-label='Functions' }
#### void SetTouch ( int Touch ) {: .copyable aria-label='Functions' }

___