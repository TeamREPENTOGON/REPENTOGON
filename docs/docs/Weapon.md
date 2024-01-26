---
tags:
  - Class
---
# Class "Weapon"

???+ info
    You can get this class by using the following functions:

    * [EntityPlayer:GetWeapon()](EntityPlayer.md#getweapon)

    ???+ example "Example Code"
        ```lua
        local weapon = Isaac.GetPlayer(0):GetWeapon(1)
        ```

## Functions

### ClearItemAnim () {: aria-label='Functions' }
#### void ClearItemAnim ( int ItemID ) {: .copyable aria-label='Functions' }

___
### GetCharge () {: aria-label='Functions' }
#### int GetCharge ( ) {: .copyable aria-label='Functions' }

___
### GetDirection () {: aria-label='Functions' }
#### [Vector](Vector.md) GetDirection ( ) {: .copyable aria-label='Functions' }

___
### GetFireDelay () {: aria-label='Functions' }
#### int GetFireDelay ( ) {: .copyable aria-label='Functions' }

___
### GetMaxFireDelay () {: aria-label='Functions' }
#### int GetMaxFireDelay ( ) {: .copyable aria-label='Functions' }

___
### GetModifiers () {: aria-label='Functions' }
#### int GetModifiers ( ) {: .copyable aria-label='Functions' }

___
### GetNumFired () {: aria-label='Functions' }
#### int GetNumFired ( ) {: .copyable aria-label='Functions' }

___
### GetOwner () {: aria-label='Functions' }
#### [Entity](Entity.md) GetOwner ( ) {: .copyable aria-label='Functions' }

___
### GetWeaponType () {: aria-label='Functions' }
#### [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) GetWeaponType ( ) {: .copyable aria-label='Functions' }

___
### IsAxisAligned () {: aria-label='Functions' }
#### boolean IsAxisAligned ( ) {: .copyable aria-label='Functions' }

___
### IsItemAnimFinished () {: aria-label='Functions' }
#### boolean IsItemAnimFinished ( int ItemID ) {: .copyable aria-label='Functions' }

___
### PlayItemAnim () {: aria-label='Functions' }
#### void PlayItemAnim ( int ItemID, [Direction](https://wofsauge.github.io/IsaacDocs/rep/enums/Direction.html) AnimDirection, [Vector](Vector.md) Position, float Charge ) {: .copyable aria-label='Functions' }

___
### SetCharge () {: aria-label='Functions' }
#### void SetCharge ( int Charge ) {: .copyable aria-label='Functions' }

___
### SetFireDelay () {: aria-label='Functions' }
#### void SetFireDelay ( float Delay ) {: .copyable aria-label='Functions' }

___
### SetHeadLockTime () {: aria-label='Functions' }
#### void SetHeadLockTime ( int Time ) {: .copyable aria-label='Functions' }

___
### SetModifiers () {: aria-label='Functions' }
#### void SetModifiers ( int modifiers ) {: .copyable aria-label='Functions' }

___