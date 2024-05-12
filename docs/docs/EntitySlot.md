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
Returns how many coins have been given to a beggar. The number returned varies based on the beggar.
???- info "Return info"
    - Beggar: How many coins have been donated.
    - Demon Beggar: How many hearts have been donated.
    - Battery Bum, Rotten Beggar: Increases with each payment by random amounts, up to 3, and resets back to 0 upon reward or payout.
    - All other slots: Remains at 0.

___
### GetPrizeType () {: aria-label='Functions' }
#### int GetPrizeType ( ) {: .copyable aria-label='Functions' }
Returns an integer that varies based on the slot machine.
???- info "Return info"
    - Slot Machine: Returns a number between `3`-`24`. `3`-`12` cause the machine to spit out a reward
        - `3`: Fly or pretty fly.
        - `4`: Bomb.
        - `5`-`6`: Heart.
        - `7`: Key.
        - `8`: Pill.
        - `9`: Unknown. This never appears to be used.
        - `10`-`12`: 1-2 coins.
        - `13`-`24`: Nothing.
    - Shell Game & Hell Game: Returns the `PickupVariant` of the potential prize.
    - Bomb Bum: Returns a number `1`-`3` based on the type of prize it had output.
        - `1`: Coins.
        - `2`: Heart.
        - `3`: Collectible.

___
### GetState () {: aria-label='Functions' }
#### int GetState ( ) {: .copyable aria-label='Functions' }
Returns the current state of the slot.
???+ info "Return info"
    All slots have a consistent state based on what it's doing that reflect as follows:
    
    - `1`: Idle.
    - `2`: Reward (Shell Game & Hell Game: Idle Reward)
    - `3`: Bombed
    - `4`: Payout
    - `5`: Reward (only for Shell Game & Hell Game)

___
### GetTimeout () {: aria-label='Functions' }
#### int GetTimeout ( ) {: .copyable aria-label='Functions' }
Returns the timeout in frames until the slot determines its prize. Not used by all slots.
???- info "Return info"
    - All beggars except Bomb Beggar: Increases randomly with each payment, returning either `1 << 16`, `1 << 17`, or the sum of them, resetting to 0 upon giving a reward.
    - Crane Game: For the first successful payout, minimum timeout is `1 << 16`, still increasing by 30 and counting down. Second payout is `1 << 17`. Third payout is `1 << 16` + `1 << 17`.
    - All other slots: Remains at 0.

___
### GetTouch () {: aria-label='Functions' }
#### int GetTouch ( ) {: .copyable aria-label='Functions' }
Returns the slot's touch counter. The touch counter goes up by one every frame a player is touching the slot and is reset once no player is touching it.

___
### RandomCoinJamAnim () {: aria-label='Functions' }
#### string RandomCoinJamAnim ( ) {: .copyable aria-label='Functions' }
Returns a random string from the following options: `CoinJam`, `CoinJam2`, `CoinJam3,` `CoinJam4`. Presumably only used on the donation machines.

___
### SetDonationValue () {: aria-label='Functions' }
#### void SetDonationValue ( int DonationValue ) {: .copyable aria-label='Functions' }
Sets the slot's donation value. See [GetDonationValue](EntitySlot.md#getdonationvalue) for more info.

___
### SetPrizeCollectible () {: aria-label='Functions' }
#### void SetPrizeCollectible ( [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) Collectible ) {: .copyable aria-label='Functions' }
Seems to only be used by Crane Game and Hell Game. This sets the collectible that the games will pay out with, and updates the rendered collectible accordingly.

___
### SetPrizeType () {: aria-label='Functions' }
#### void SetPrizeType ( int PrizeType ) {: .copyable aria-label='Functions' }
Sets the prize type of the slot. See [GetPrizeType](EntitySlot.md#getprizetype) for more info.

___
### SetState () {: aria-label='Functions' }
#### void SetState ( int State ) {: .copyable aria-label='Functions' }
Sets the state of the slot. See [GetState](EntitySlot.md#getstate) for more info.

___
### SetTimeout () {: aria-label='Functions' }
#### void SetTimeout ( int Timeout ) {: .copyable aria-label='Functions' }
Sets the timeout of the slot. See [GetTimeout](EntitySlot.md#gettimeout) for more info.

___
### SetTouch () {: aria-label='Functions' }
#### void SetTouch ( int Touch ) {: .copyable aria-label='Functions' }
Sets the slot's touch counter. The touch counter goes up by one every frame a player is touching the slot and is reset to zero once no player is touching it.
___