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

### GetDamageMultiplier () {: aria-label='Functions' }
#### float GetDamageMultiplier ( ) {: .copyable aria-label='Functions' }

___
### GetDisableFollowParent () {: aria-label='Functions' }
#### boolean GetDisableFollowParent ( ) {: .copyable aria-label='Functions' }

___
### GetHitList () {: aria-label='Functions' }
#### int GetHitList ( ) {: .copyable aria-label='Functions' }

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
#### boolean GetTimeout ( ) {: .copyable aria-label='Functions' }

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
#### void SetTimeout ( boolean Value ) {: .copyable aria-label='Functions' }

___