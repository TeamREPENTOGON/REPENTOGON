# Class "Entity"

## Functions

### Add·Baited () {: aria-label='Functions' }
#### void AddBaited ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### Add·Bleeding () {: aria-label='Functions' }
#### void AddBleeding ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### Add·Brimstone·Mark () {: aria-label='Functions' }
#### void AddBrimstoneMark ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### Add·Ice () {: aria-label='Functions' }
#### void AddIce ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### Add·Knockback () {: aria-label='Functions' }
#### void AddKnockback ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) PushDirection, int Duration, boolean TakeImpactDamage ) {: .copyable aria-label='Functions' }

___
### Add·Magnetized () {: aria-label='Functions' }
#### void AddMagnetized ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### Add·Weakness () {: aria-label='Functions' }
#### void AddWeakness ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### Get·Animation·State () {: aria-label='Functions' }
#### [AnimationState](AnimationState.md) GetAnimationState ( ) {: .copyable aria-label='Functions' }
___
### Get·Null·Offset () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetNullOffset ( string NullLayerName ) {: .copyable aria-label='Functions' }
Returns position of null layer mark. Alternatively returns Vector.Zero if layer is not visible or for other unknown reasons.
___
### Get·Overlay·Animation·State () {: aria-label='Functions' }
#### [AnimationState](AnimationState.md) GetOverlayAnimationState ( ) {: .copyable aria-label='Functions' }
___
### Get·Shadow·Size () {: aria-label='Functions' }
#### float GetShadowSize ( ) {: .copyable aria-label='Functions' }

___
### Set·Shadow·Size () {: aria-label='Functions' }
#### float SetShadowSize ( float Size ) {: .copyable aria-label='Functions' }

___
### To·Slot () {: aria-label='Functions' }
#### [EntitySlot](EntitySlot.md) ToSlot ( ) {: .copyable aria-label='Functions' }
Used to cast an [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) object to an [EntitySlot](EntitySlot.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.

___