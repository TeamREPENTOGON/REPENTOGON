---
tags:
  - Class
---
# Class "Entity"

## Functions

### AddBaited () {: aria-label='Functions' }
#### void AddBaited ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### AddBleeding () {: aria-label='Functions' }
#### void AddBleeding ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### AddBrimstoneMark () {: aria-label='Functions' }
#### void AddBrimstoneMark ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### AddIce () {: aria-label='Functions' }
#### void AddIce ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### AddKnockback () {: aria-label='Functions' }
#### void AddKnockback ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) PushDirection, int Duration, boolean TakeImpactDamage ) {: .copyable aria-label='Functions' }

___
### AddMagnetized () {: aria-label='Functions' }
#### void AddMagnetized ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### AddWeakness () {: aria-label='Functions' }
#### void AddWeakness ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### ComputeStatusEffectDuration () {: aria-label='Functions' }
#### int ComputeStatusEffectDuration ( int initial, [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) ref ) {: .copyable aria-label='Functions' }

___
### CopyStatusEffects () {: aria-label='Functions' }
#### void CopyStatusEffects ( ) {: .copyable aria-label='Functions' }
#### void CopyStatusEffects ( [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Target ) {: .copyable aria-label='Functions' }

If no Entity is specified, this will recursively copy status effects to all [Child](https://wofsauge.github.io/IsaacDocs/rep/Entity.html#child) Entities.

___
### ForceCollide () {: aria-label='Functions' }
#### boolean ForceCollide ( [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Entity, bool Low ) {: .copyable aria-label='Functions' }

___
### GetAnimationState () {: aria-label='Functions' }
#### [AnimationState](AnimationState.md) GetAnimationState ( ) {: .copyable aria-label='Functions' }

___
### GetBossStatusEffectCooldown () {: aria-label='Functions' }
#### int GetBossStatusEffectCooldown ( ) {: .copyable aria-label='Functions' }

___
### GetCollisionCapsule () {: aria-label='Functions' }
#### [Capsule](Capsule.md) GetCollisionCapsule ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) vec ) {: .copyable aria-label='Functions' }

___
### GetDebugShape () {: aria-label='Functions' }
#### [Shape](renderer/Shape.md) GetDebugShape ( boolean unknown ) {: .copyable aria-label='Functions' }

___
### GetEntityConfigEntity () {: aria-label='Functions' }
#### [EntityConfigEntity](EntityConfigEntity.md) GetEntityConfigEntity ( ) {: .copyable aria-label='Functions' }
Returns the corresponding [EntityConfig](EntityConfig.md) entry for this entity.

___
### GetHitListIndex () {: aria-label='Functions' }
#### int GetHitListIndex ( ) {: .copyable aria-label='Functions' }

___
### GetMinecart () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) GetMinecart ( ) {: .copyable aria-label='Functions' }
Returns the minecart the entity is riding.

???+ Return behavior
    If the entity is not riding a minecart, this function returns `nil`.

___
### GetNullCapsule () {: aria-label='Functions' }
#### [Capsule](Capsule.md) GetNullCapsule ( string NullLayerName ) {: .copyable aria-label='Functions' }

___
### GetNullOffset () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetNullOffset ( string NullLayerName ) {: .copyable aria-label='Functions' }
Returns position of null layer mark. Alternatively returns Vector.Zero if the layer is not visible, has no frame available for the current animation, or for other unknown reasons.

___
### GetOverlayAnimationState () {: aria-label='Functions' }
#### [AnimationState](AnimationState.md) GetOverlayAnimationState ( ) {: .copyable aria-label='Functions' }

___
### GetPauseTime () {: aria-label='Functions' }
#### int GetPauseTime ( ) {: .copyable aria-label='Functions' }

___
### GetPosVel () {: aria-label='Functions' }
#### [PosVel](https://wofsauge.github.io/IsaacDocs/rep/PlayerTypes_PosVel.html) GetPosVel ( ) {: .copyable aria-label='Functions' }
Returns 2 values, both Vectors. 1st the Position of the entity, 2nd the Velocity of the entity.

___
### GetPredictedTargetPosition () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetPredictedTargetPosition ( [Entity](Entity.md) target, int strength ) {: .copyable aria-label='Functions' }

___
### GetShadowSize () {: aria-label='Functions' }
#### float GetShadowSize ( ) {: .copyable aria-label='Functions' }

___
### GetSpeedMultiplier () {: aria-label='Functions' }
#### float GetSpeedMultiplier ( ) {: .copyable aria-label='Functions' }

___
### GetType () {: aria-label='Functions' }
#### [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) GetType ( ) {: .copyable aria-label='Functions' }

___
### GiveMinecart () {: aria-label='Functions' }
#### [EntityNPC](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html) GiveMinecart ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) velocity ) {: .copyable aria-label='Functions' }

___
### IgnoreEffectFromFriendly () {: aria-label='Functions' }
#### boolean IgnoreEffectFromFriendly ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) ref ) {: .copyable aria-label='Functions' }

___
### MakeBloodPoof () {: aria-label='Functions' }
#### void MakeBloodPoof ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) pos = entityPosition, [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) color = default, float strength = 1.0f ) {: .copyable aria-label='Functions' }

___
### MakeGroundPoof () {: aria-label='Functions' }
#### void MakeGroundPoof ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) pos = entityPosition, [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) color = default, float strength = 1.0f ) {: .copyable aria-label='Functions' }

___
### SetBossStatusEffectCooldown () {: aria-label='Functions' }
#### void SetBossStatusEffectCooldown ( int Cooldown ) {: .copyable aria-label='Functions' }

___
### SetDead () {: aria-label='Functions' }
#### void SetDead ( boolean isDead ) {: .copyable aria-label='Functions' }

___
### SetInvincible () {: aria-label='Functions' }
#### void SetInvincible ( boolean isInvincible ) {: .copyable aria-label='Functions' }

___
### SetPauseTime () {: aria-label='Functions' }
#### void SetPauseTime ( int Duration ) {: .copyable aria-label='Functions' }

___
### SetShadowSize () {: aria-label='Functions' }
#### float SetShadowSize ( float Size ) {: .copyable aria-label='Functions' }

___
### SetSpeedMultiplier () {: aria-label='Functions' }
#### void SetSpeedMultiplier ( float amount ) {: .copyable aria-label='Functions' }

___
### SpawnBloodEffect () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) SpawnBloodEffect ( int subtype = 0, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position = entityPosition, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) offset = nil, [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) color = default, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) velocity = nil ) {: .copyable aria-label='Functions' }

___
### SpawnGroundImpactEffects () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) SpawnGroundImpactEffects ( [Vector](Vector.md) position, [Vector](Vector.md) vel, int strength ) {: .copyable aria-label='Functions' }

___
### TeleportToRandomPosition () {: aria-label='Functions' }
#### void TeleportToRandomPosition ( ) {: .copyable aria-label='Functions' }

___
### ToSlot () {: aria-label='Functions' }
#### [EntitySlot](EntitySlot.md) ToSlot ( ) {: .copyable aria-label='Functions' }
Used to cast an [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) object to an [EntitySlot](EntitySlot.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.

___
### TryThrow () {: aria-label='Functions' }
#### boolean TryThrow ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) ThrowDirection, float Force ) {: .copyable aria-label='Functions' }

___