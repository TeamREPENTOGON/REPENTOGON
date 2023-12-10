---
tags:
  - Class
---
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
### Compute·Status·Effect·Duration () {: aria-label='Functions' }
#### int ComputeStatusEffectDuration ( int initial, [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) ref ) {: .copyable aria-label='Functions' }

___
### Copy·Status·Effects () {: aria-label='Functions' }
#### void CopyStatusEffects ( ) {: .copyable aria-label='Functions' }
#### void CopyStatusEffects ( [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Target ) {: .copyable aria-label='Functions' }

If no Entity is specified, this will recursively copy status effects to all [Child](https://wofsauge.github.io/IsaacDocs/rep/Entity.html#child) Entities.

___
### Force·Collide () {: aria-label='Functions' }
#### boolean ForceCollide ( [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) Entity, bool Low ) {: .copyable aria-label='Functions' }

___
### Get·Animation·State () {: aria-label='Functions' }
#### [AnimationState](AnimationState.md) GetAnimationState ( ) {: .copyable aria-label='Functions' }

___
### Get·Boss·Status·Effect·Cooldown () {: aria-label='Functions' }
#### int GetBossStatusEffectCooldown ( ) {: .copyable aria-label='Functions' }

___
### Get·Collision·Capsule () {: aria-label='Functions' }
#### [Capsule](renderer/Capsule.md) GetCollisionCapsule ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) vec ) {: .copyable aria-label='Functions' }

___
### Get·Debug·Shape () {: aria-label='Functions' }
#### [Shape](renderer/Shape.md) GetDebugShape ( boolean unknown ) {: .copyable aria-label='Functions' }

___
### Get·Entity·Config·Entity () {: aria-label='Functions' }
#### [EntityConfigEntity](EntityConfigEntity.md) GetEntityConfigEntity ( ) {: .copyable aria-label='Functions' }
Returns the corresponding [EntityConfig](EntityConfig.md) entry for this entity.

___
### Get·Hit·List·Index () {: aria-label='Functions' }
#### int GetHitListIndex ( ) {: .copyable aria-label='Functions' }

___
### Get·Minecart () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) GetMinecart ( ) {: .copyable aria-label='Functions' }
Returns the minecart the entity is riding.

???+ Return behavior
    If the entity is not riding a minecart, this function returns `nil`.

___
### Get·Null·Capsule () {: aria-label='Functions' }
#### [Capsule](renderer/Capsule.md) GetNullCapsule ( string NullLayerName ) {: .copyable aria-label='Functions' }

___
### Get·Null·Offset () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetNullOffset ( string NullLayerName ) {: .copyable aria-label='Functions' }
Returns position of null layer mark. Alternatively returns Vector.Zero if the layer is not visible, has no frame available for the current animation, or for other unknown reasons.

___
### Get·Overlay·Animation·State () {: aria-label='Functions' }
#### [AnimationState](AnimationState.md) GetOverlayAnimationState ( ) {: .copyable aria-label='Functions' }

___
### Get·Pause·Time () {: aria-label='Functions' }
#### int GetPauseTime ( ) {: .copyable aria-label='Functions' }

___
### Get·Pos·Vel () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetPosVel ( ) {: .copyable aria-label='Functions' }
Returns 2 values, both Vectors. 1st the Position of the entity, 2nd the Velocity of the entity.

___
### Get·Predicted·Target·Position () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetPredictedTargetPosition ( [Entity](Entity.md) target, int strength ) {: .copyable aria-label='Functions' }

___
### Get·Shadow·Size () {: aria-label='Functions' }
#### float GetShadowSize ( ) {: .copyable aria-label='Functions' }

___
### Get·Speed·Multiplier () {: aria-label='Functions' }
#### float GetSpeedMultiplier ( ) {: .copyable aria-label='Functions' }

___
### Get·Type () {: aria-label='Functions' }
#### [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) GetType ( ) {: .copyable aria-label='Functions' }

___
### Give·Minecart () {: aria-label='Functions' }
#### [EntityNPC](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html) GiveMinecart ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) velocity ) {: .copyable aria-label='Functions' }

___
### Ignore·Effect·From·Friendly () {: aria-label='Functions' }
#### boolean IgnoreEffectFromFriendly ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) ref ) {: .copyable aria-label='Functions' }

___
### Make·Blood·Poof () {: aria-label='Functions' }
#### void MakeBloodPoof ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) pos = entityPosition, [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) color = default, float strength = 1.0f ) {: .copyable aria-label='Functions' }

___
### Make·Ground·Poof () {: aria-label='Functions' }
#### void MakeGroundPoof ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) pos = entityPosition, [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) color = default, float strength = 1.0f ) {: .copyable aria-label='Functions' }

___
### Set·Boss·Status·Effect·Cooldown () {: aria-label='Functions' }
#### void SetBossStatusEffectCooldown ( int Cooldown ) {: .copyable aria-label='Functions' }

___
### Set·Dead () {: aria-label='Functions' }
#### void SetDead ( boolean isDead ) {: .copyable aria-label='Functions' }

___
### Set·Invincible () {: aria-label='Functions' }
#### void SetInvincible ( boolean isInvincible ) {: .copyable aria-label='Functions' }

___
### Set·Pause·Time () {: aria-label='Functions' }
#### void SetPauseTime ( int Duration ) {: .copyable aria-label='Functions' }

___
### Set·Shadow·Size () {: aria-label='Functions' }
#### float SetShadowSize ( float Size ) {: .copyable aria-label='Functions' }

___
### Set·Speed·Multiplier () {: aria-label='Functions' }
#### void SetSpeedMultiplier ( float amount ) {: .copyable aria-label='Functions' }

___
### Spawn·Blood·Effect () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) SpawnBloodEffect ( int subtype = 0, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position = entityPosition, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) offset = nil, [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) color = default, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) velocity = nil ) {: .copyable aria-label='Functions' }

___
### Spawn·Ground·Impact·Effects () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) SpawnGroundImpactEffects ( [Vector](Vector.md) position, [Vector](Vector.md) vel, int strength ) {: .copyable aria-label='Functions' }

___
### Teleport·To·Random·Position () {: aria-label='Functions' }
#### void TeleportToRandomPosition ( ) {: .copyable aria-label='Functions' }

___
### To·Slot () {: aria-label='Functions' }
#### [EntitySlot](EntitySlot.md) ToSlot ( ) {: .copyable aria-label='Functions' }
Used to cast an [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html) object to an [EntitySlot](EntitySlot.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.

___
### Try·Throw () {: aria-label='Functions' }
#### boolean TryThrow ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) ThrowDirection, float Force ) {: .copyable aria-label='Functions' }

___