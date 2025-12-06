---
tags:
  - Class
---
# Class "EntityKnife"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### FireSplitTear () {: aria-label='Functions' }
#### [EntityTear](EntityTear.md) FireSplitTear ( [Vector](Vector.md) Position, [Vector](Vector.md) Velocity, float DamageMultiplier = 0.5, float SizeMultiplier = 0.6, int Variant = 0, [SplitTearType](enums/SplitTearType.md) splitType = SplitTearType.SPLIT_GENERIC ) {: .copyable aria-label='Functions' }
Fire a new tear that inherits many attributes from this knife (flags, damage, size, color, etc).

This will also trigger the `MC_POST_FIRE_SPLIT_TEAR` callback. For custom effects, a string may be passed in place of the [SplitTearType](enums/SplitTearType.md).

___
### GetHitboxParentKnife () {: aria-label='Functions' }
#### [EntityKnife](EntityKnife.md) GetHitboxParentKnife ( ) {: .copyable aria-label='Functions' }
For a "hitbox" [EntityKnife](EntityKnife.md) ([KnifeSubType.CLUB_HITBOX](enums/KnifeSubType.md)) created by melee weapon "swings" (Bone Club, Spirit Sword, etc) this function returns the "main" [EntityKnife](EntityKnife.md) of that melee weapon. Returns `nil` otherwise, and will return nil for hitbox entities spawned via other means.

___
### GetHitList () {: aria-label='Functions' }
#### int[] GetHitList ( ) {: .copyable aria-label='Functions' }
Returns an array of hit entities using their [Index](https://wofsauge.github.io/IsaacDocs/rep/Entity.html#index) field.

___
### GetIsSpinAttack () {: aria-label='Functions' }
#### boolean GetIsSpinAttack ( ) {: .copyable aria-label='Functions' }

___
### GetIsSwinging () {: aria-label='Functions' }
#### boolean GetIsSwinging ( ) {: .copyable aria-label='Functions' }

___
### IsMultidimensionalTouched () {: aria-label='Functions' }
#### boolean IsMultidimensionalTouched ( ) {: .copyable aria-label='Functions' }
Returns if the knife was created through the Multi Dimensional Baby effect.

___
### IsPrismTouched () {: aria-label='Functions' }
#### boolean IsPrismTouched ( ) {: .copyable aria-label='Functions' }
Returns if the knife was created through the Angelic Prism effect.

___
### SetHitboxParentKnife () {: aria-label='Functions' }
#### void SetHitboxParentKnife ( [EntityKnife](EntityKnife.md) Knife ) {: .copyable aria-label='Functions' }
Allows setting a custom value for `GetHitboxParentKnife`, which is intended to only be used for a "hitbox" [EntityKnife](EntityKnife.md) ([KnifeSubType.CLUB_HITBOX](enums/KnifeSubType.md)) to refer to the "main" [EntityKnife](EntityKnife.md) of a melee weapon (Bone Club, Spirit Sword, etc).

Note that setting this has no influence on any vanilla logic - this reference is only for the convenience of modders. Please use appropriately.

___
### SetIsSpinAttack () {: aria-label='Functions' }
#### void SetIsSpinAttack ( boolean isSpinAttack ) {: .copyable aria-label='Functions' }

___
### SetIsSwinging () {: aria-label='Functions' }
#### void SetIsSwinging ( boolean isSwinging ) {: .copyable aria-label='Functions' }

___
### SetMultidimensionalTouched () {: aria-label='Functions' }
#### void SetMultidimensionalTouched ( boolean IsTouched ) {: .copyable aria-label='Functions' }
Sets if the knife was created through the Angelic Prism effect.

___
### SetPrismTouched () {: aria-label='Functions' }
#### void SetPrismTouched ( boolean IsTouched ) {: .copyable aria-label='Functions' }
Sets if the knife was created through the Angelic Prism effect.

___
