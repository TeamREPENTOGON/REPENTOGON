---
tags:
  - Class
---
# Class "EntityLaser"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Modified Variables
___
### HomingType {: aria-label='Modified Variables' }
#### int HomingType  {: .copyable aria-label='Modified Variables' }
Same as default, but now returns a proper integer value instead of userdata.

___

## Functions

### FireSplitTear () {: aria-label='Functions' }
#### [EntityTear](EntityTear.md) FireSplitTear ( [Vector](Vector.md) Position, [Vector](Vector.md) Velocity, float DamageMultiplier = 0.5, float SizeMultiplier = 0.6, int Variant = 0, [SplitTearType](enums/SplitTearType.md) splitType = SplitTearType.SPLIT_GENERIC ) {: .copyable aria-label='Functions' }
Fire a new tear that inherits many attributes from this laser (flags, damage, size, color, etc).

This will also trigger the `MC_POST_FIRE_SPLIT_TEAR` callback. For custom effects, a string may be passed in place of the [SplitTearType](enums/SplitTearType.md).

___
### GetDamageMultiplier () {: aria-label='Functions' }
#### float GetDamageMultiplier ( ) {: .copyable aria-label='Functions' }

___
### GetDisableFollowParent () {: aria-label='Functions' }
#### boolean GetDisableFollowParent ( ) {: .copyable aria-label='Functions' }

___
### GetHitList () {: aria-label='Functions' }
#### int GetHitList ( ) {: .copyable aria-label='Functions' }
Returns an array of hit entities using their [Index](https://wofsauge.github.io/IsaacDocs/rep/Entity.html#index) field.

___
### GetNumChainedLasers () {: aria-label='Functions' }
#### int GetNumChainedLasers ( ) {: .copyable aria-label='Functions' }
Related to the effect used by the Monstro's Lung + Technology synergy. If >0, may cause an additional laser to spawn at the end point of this laser, up to this many times.

___
### GetOneHit () {: aria-label='Functions' }
#### boolean GetOneHit ( ) {: .copyable aria-label='Functions' }

___
### GetScale () {: aria-label='Functions' }
#### float GetScale ( ) {: .copyable aria-label='Functions' }

___
### GetShrink () {: aria-label='Functions' }
#### boolean GetShrink ( ) {: .copyable aria-label='Functions' }

___
### GetTimeout () {: aria-label='Functions' }
#### int GetTimeout ( ) {: .copyable aria-label='Functions' }

___
### IsMultidimensionalTouched () {: aria-label='Functions' }
#### boolean IsMultidimensionalTouched ( ) {: .copyable aria-label='Functions' }
Returns if the laser was created through the Multi Dimensional Baby effect.

___
### IsPrismTouched () {: aria-label='Functions' }
#### boolean IsPrismTouched ( ) {: .copyable aria-label='Functions' }
Returns if the laser was created through the Angelic Prism effect.

___
### RecalculateSamplesNextUpdate () {: aria-label='Functions' }
#### void RecalculateSamplesNextUpdate ( ) {: .copyable aria-label='Functions' }
Requests the laser's shape to be fully recalculated next time it updates. Can be used to force the laser to instantly change its MaxDistance/Radius instead of transitioning to it. No effect for OneHit or non-sample lasers.

___
### ResetSpriteScale () {: aria-label='Functions' }
#### void ResetSpriteScale ( ) {: .copyable aria-label='Functions' }

___
### RotateToAngle () {: aria-label='Functions' }
#### void RotateToAngle ( float Angle, float Speed = 8.0 ) {: .copyable aria-label='Functions' }

___
### SetDamageMultiplier () {: aria-label='Functions' }
#### void SetDamageMultiplier ( float DamageMult ) {: .copyable aria-label='Functions' }

___
### SetDisableFollowParent () {: aria-label='Functions' }
#### void SetDisableFollowParent ( boolean Value ) {: .copyable aria-label='Functions' }

___
### SetInitSound () {: aria-label='Functions' }
#### void SetInitSound ( [SoundEffect](https://wofsauge.github.io/IsaacDocs/rep/enums/SoundEffect.html) Sound ) {: .copyable aria-label='Functions' }
Set after a laser is spawned but before it updates (for example, [MC_POST_LASER_INIT](https://wofsauge.github.io/IsaacDocs/rep/enums/ModCallbacks.html#mc_post_laser_init)) to change the sound it makes. Can be set to `SoundEffect.SOUND_NULL` to make no sound play.

___
### SetNumChainedLasers () {: aria-label='Functions' }
#### void SetNumChainedLasers ( int Value ) {: .copyable aria-label='Functions' }
Controls the effect used by the Monstro's Lung + Technology synergy. If >0, may cause an additional laser to spawn at the end point of this laser, up to this many times. May not function for all laser variants.

___
### SetPrismTouched () {: aria-label='Functions' }
#### void SetPrismTouched ( boolean IsTouched ) {: .copyable aria-label='Functions' }
Sets if the laser was created through the Angelic Prism effect.

___
### SetScale () {: aria-label='Functions' }
#### void SetScale ( float Value ) {: .copyable aria-label='Functions' }

___
### SetShrink () {: aria-label='Functions' }
#### void SetShrink ( boolean Value ) {: .copyable aria-label='Functions' }

___
### SetTimeout () {: aria-label='Functions' }
#### void SetTimeout ( int Value ) {: .copyable aria-label='Functions' }

___