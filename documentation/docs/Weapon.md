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
        local weapon = Isaac.GetPlayer(0):GetWeapon(WeaponType.WEAPON_TEARS)
        ```

## Functions

### Clear·Item·Anim() {: aria-label='Functions' }
#### void ClearItemAnim ( int itemID ) {: .copyable aria-label='Functions' }

___
### Get·Charge () {: aria-label='Functions' }
#### int GetCharge ( ) {: .copyable aria-label='Functions' }

___
### Get·Direction () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetDirection ( ) {: .copyable aria-label='Functions' }

___
### Get·Fire·Delay () {: aria-label='Functions' }
#### int GetFireDelay ( ) {: .copyable aria-label='Functions' }

___
### Get·Max·Fire·Delay () {: aria-label='Functions' }
#### int GetMaxFireDelay ( ) {: .copyable aria-label='Functions' }

___
### Get·Modifiers () {: aria-label='Functions' }
#### int GetModifiers ( ) {: .copyable aria-label='Functions' }

___
### Get·Num·Fired () {: aria-label='Functions' }
#### int GetNumFired ( ) {: .copyable aria-label='Functions' }

___
### Get·Owner () {: aria-label='Functions' }
#### [Entity](Entity.md) GetOwner ( ) {: .copyable aria-label='Functions' }

___
### Get·Weapon·Type () {: aria-label='Functions' }
#### [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) GetWeaponType ( ) {: .copyable aria-label='Functions' }

___
### Is·Axis·Aligned () {: aria-label='Functions' }
#### boolean IsAxisAligned ( ) {: .copyable aria-label='Functions' }

___
### Is·Item·Anim·Finished () {: aria-label='Functions' }
#### boolean IsItemAnimFinished ( int item ) {: .copyable aria-label='Functions' }

___
### Play·Item·Anim() {: aria-label='Functions' }
#### void PlayItemAnim ( int itemID, int anim, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position, float charge ) {: .copyable aria-label='Functions' }

___
### Set·Charge() {: aria-label='Functions' }
#### void SetCharge ( int charge ) {: .copyable aria-label='Functions' }

___
### Set·Fire·Delay() {: aria-label='Functions' }
#### void SetFireDelay ( float delay ) {: .copyable aria-label='Functions' }

___
### Set·Head·Lock·Time() {: aria-label='Functions' }
#### void SetHeadLockTime ( int time ) {: .copyable aria-label='Functions' }

___