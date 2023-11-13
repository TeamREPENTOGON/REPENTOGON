# Class "[MultiShotParams](MultiShotParams.md)"

???+ info
    You can get this class by using the following functions:

    * [EntityPlayer:GetMultiShotParams](EntityPlayer.md#getmultishotparams)

    ???+ example "Example Code"
        ```lua
        local params = Game():GetPlayer(0):GetMultiShotParams(0)
        ```

**[MultiShotParams](MultiShotParams.md)** contains information the game uses to properly calculate the position and velocity of every tear fired, among other things.

## Functions

### Get·Multi·Eye·Angle () {: aria-label='Functions' }
#### float GetMultiEyeAngle ( ) {: .copyable aria-label='Functions' }
When more than one eye is active, defines the angle the eyes are offset to eachother. Similar to cross eye effect. 

Example: for The Wiz, this is 45.
___
### Get·Num·Eyes·Active () {: aria-label='Functions' }
#### int GetNumEyesActive ( ) {: .copyable aria-label='Functions' }
Returns the number of eyes simultaniously shooting. Examples: For The Wiz, its 2, for mutant Spider its 1.
___
### Get·Num·Lanes·Per·Eye () {: aria-label='Functions' }
#### int GetNumLanesPerEye ( ) {: .copyable aria-label='Functions' }
Returns the amount of lanes used to spread the shot tears onto.
Lane positions are calculated by dividing the area, defined by the shooting direction +- the spreadAngle, by the number of lanes. This will create a pattern similar to a symetrical hand fan.
Normally the number of lanes should be the same number as the amount of tears divided by the number of eyes. 

A smaller number of lanes than the amount of tears will cause tears to overlap each other. A higher lane count than tears will make the fan pattern asymetrical.
___
### Get·Num·Random·Dir·Tears () {: aria-label='Functions' }
#### int GetNumRandomDirTears ( ) {: .copyable aria-label='Functions' }
Returns the amount of tears additionally shot in random directions. Same effect as "Eye Sore" collectible.
___
### Get·Num·Tears () {: aria-label='Functions' }
#### int GetNumTears ( ) {: .copyable aria-label='Functions' }
Returns the amount of tears the player can currently simultaneously fire.
___
### Get·Spread·Angle·Knife () {: aria-label='Functions' }
#### float GetSpreadAngleKnife ( ) {: .copyable aria-label='Functions' }
Get the spread angle for knife and meelee attacks.
___
### Get·Spread·Angle·Laser () {: aria-label='Functions' }
#### float GetSpreadAngleLaser ( ) {: .copyable aria-label='Functions' }
Get the spread angle for laser and brimstone attacks.
___
### Get·Spread·Angle·Tears () {: aria-label='Functions' }
#### float GetSpreadAngleTears ( ) {: .copyable aria-label='Functions' }
Get the spread angle for tear, dr fetus and default attacks.
___
### Get·Spread·Angle·Tech·X () {: aria-label='Functions' }
#### float GetSpreadAngleTechX ( ) {: .copyable aria-label='Functions' }
Get the spread angle for tech X attacks.
___
### Is·Cross·Eyed () {: aria-label='Functions' }
#### boolean IsCrossEyed ( ) {: .copyable aria-label='Functions' }
Returns if a cross eye effect is active, aka. player shoots in 2 directions with 45° offset to eachother.
___
### Is·Shooting·Backwards () {: aria-label='Functions' }
#### boolean IsShootingBackwards ( ) {: .copyable aria-label='Functions' }
Returns if an additional shot backwards will be triggered. Similar effect to Mom's Eye.
___
### Is·Shooting·Sideways () {: aria-label='Functions' }
#### boolean IsShootingSideways ( ) {: .copyable aria-label='Functions' }
Returns if two additional shots sideways will be triggered. Similar effect to Loki's horns.
___
### Set·Is·Cross·Eyed () {: aria-label='Functions' }
#### void SetIsCrossEyed ( boolean value ) {: .copyable aria-label='Functions' }
Set if a cross eye effect is active, aka. player shoots in 2 directions with 45° offset to eachother.
___
### Set·Is·Shooting·Backwards () {: aria-label='Functions' }
#### void SetIsShootingBackwards ( boolean value ) {: .copyable aria-label='Functions' }
Set if an additional shot backwards will be triggered. Similar effect to Mom's Eye.
___
### Set·Is·Shooting·Sideways () {: aria-label='Functions' }
#### void SetIsShootingSideways ( boolean value ) {: .copyable aria-label='Functions' }
Set if two additional shots sideways will be triggered. Similar effect to Loki's horns.
___
### Set·Multi·Eye·Angle () {: aria-label='Functions' }
#### void SetMultiEyeAngle ( float angle ) {: .copyable aria-label='Functions' }
When more than one eye is active, defines the angle the eyes are offset to eachother. Similar to cross eye effect. 

Example: for The Wiz, this is 45.
___
### Set·Num·Eyes·Active () {: aria-label='Functions' }
#### void SetNumEyesActive ( int value ) {: .copyable aria-label='Functions' }
Set the number of eyes simultaniously shooting. Examples: For The Wiz, its 2, for mutant Spider its 1.
___
### Set·Num·Lanes·Per·Eye () {: aria-label='Functions' }
#### void SetNumLanesPerEye ( int value ) {: .copyable aria-label='Functions' }

___
### Set·Num·Random·Dir·Tears () {: aria-label='Functions' }
#### void SetNumRandomDirTears ( int value ) {: .copyable aria-label='Functions' }
Set the amount of tears additionally shot in random directions. Same effect as "Eye Sore" collectible.
___
### Set·Num·Tears () {: aria-label='Functions' }
#### void SetNumTears ( int value ) {: .copyable aria-label='Functions' }
Set the amount of tears the player can currently simultaneously fire.
___
### Set·Spread·Angle·Knife () {: aria-label='Functions' }
#### void SetSpreadAngleKnife ( float angle ) {: .copyable aria-label='Functions' }
Set the spread angle for knife and meelee attacks.
___
### Set·Spread·Angle·Laser () {: aria-label='Functions' }
#### void SetSpreadAngleLaser ( float angle ) {: .copyable aria-label='Functions' }
Set the spread angle for laser and brimstone attacks.
___
### Set·Spread·Angle·Tears () {: aria-label='Functions' }
#### void SetSpreadAngleTears ( float angle ) {: .copyable aria-label='Functions' }
Set the spread angle for tear, dr fetus and default attacks.
___
### Set·Spread·Angle·Tech·X () {: aria-label='Functions' }
#### void SetSpreadAngleTechX ( float angle ) {: .copyable aria-label='Functions' }
Set the spread angle for tech X attacks.
___