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
#### void AddKnockback ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, [Vector](Vector.md) PushDirection, int Duration, boolean TakeImpactDamage ) {: .copyable aria-label='Functions' }

???+ info "Duration info"
    The Duration has a maximum of 0.5 seconds / 15 frames.

___
### AddMagnetized () {: aria-label='Functions' }
#### void AddMagnetized ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### AddWeakness () {: aria-label='Functions' }
#### void AddWeakness ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, int Duration ) {: .copyable aria-label='Functions' }

___
### ComputeStatusEffectDuration () {: aria-label='Functions' }
#### int ComputeStatusEffectDuration ( int InitialLength, [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source ) {: .copyable aria-label='Functions' }

___
### CopyStatusEffects () {: aria-label='Functions' }
#### void CopyStatusEffects ( [Entity](Entity.md) Target, boolean Overwrite = false ) {: .copyable aria-label='Functions' }
If `Target` is left unspecified, this will recursively copy status effects to all [Child](https://wofsauge.github.io/IsaacDocs/rep/Entity.html#child) Entities. `Overwrite` will additionally remove all other status effects from the target and set the properties of existing effects to match the entity.

___
### ForceCollide () {: aria-label='Functions' }
#### boolean ForceCollide ( [Entity](Entity.md) Entity, boolean Force ) {: .copyable aria-label='Functions' }

Returns true if the rest of the collision logic should be **Skipped**; either because the entities did not collide in the first place (if `Force` is false),
or because one of the entities' internal collision logic decided so (which can be influenced by one of the `PRE_COLLISION` callbacks).

if `Force` is set to false, then the game will check if the entities' [EntityCollisionClass](https://wofsauge.github.io/IsaacDocs/rep/Entity.html#entitycollisionclass) are
compatible and if both entities are not [Dead](https://wofsauge.github.io/IsaacDocs/rep/Entity.html#isdead) before triggering the collision.

???- warning "Collision Logic Changes"
    The collision logic for some entity types changes when forced.<br>
    For example: Slots do not make the player pay the regular prize for activation.

___
### GetBaitedCountdown () {: aria-label='Functions' }
#### int GetBaitedCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetBleedingCountdown () {: aria-label='Functions' }
#### int GetBleedingCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetBossStatusEffectCooldown () {: aria-label='Functions' }
#### int GetBossStatusEffectCooldown ( ) {: .copyable aria-label='Functions' }

___
### GetBrimstoneMarkCountdown () {: aria-label='Functions' }
#### int GetBrimstoneMarkCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetBurnCountdown () {: aria-label='Functions' }
#### int GetBurnCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetBurnDamageTimer () {: aria-label='Functions' }
#### int GetBurnDamageTimer ( ) {: .copyable aria-label='Functions' }

___
### GetCharmedCountdown () {: aria-label='Functions' }
#### int GetCharmedCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetCollisionCapsule () {: aria-label='Functions' }
#### [Capsule](Capsule.md) GetCollisionCapsule ( [Vector](Vector.md) Vector ) {: .copyable aria-label='Functions' }

___
### GetColorParams () {: aria-label='Functions' }
#### [ColorParams](ColorParams.md)[] GetColorParams ( ) {: .copyable aria-label='Functions' }
Returns a table of all colors currently queued by `SetColor` alongside their parameters.

___
### GetConfusionCountdown () {: aria-label='Functions' }
#### int GetConfusionCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetDamageCountdown () {: aria-label='Functions' }
#### int GetDamageCountdown ( ) {: .copyable aria-label='Functions' }
If the entity recently took damage with the DAMAGE_COUNTDOWN [DamageFlag](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html), this returns how many more frames must pass before they can take damage with the DAMAGE_COUNTDOWN [DamageFlag](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) again.

Note that this is NOT the same as the player's invincibility frames (`EntityPlayer:GetDamageCooldown()`). The DAMAGE_COUNTDOWN [DamageFlag](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) and this associated countdown are typically used to control how rapidly an enemy will take damage from the few sources that uses that flag, such as the the collision damage effects from the "My Little Unicorn", "The Nail", and "The Gamekid".

___
### GetDebugShape () {: aria-label='Functions' }
#### [Shape](renderer/Shape.md) GetDebugShape ( boolean Unknown ) {: .copyable aria-label='Functions' }

___
### GetEntityConfigEntity () {: aria-label='Functions' }
#### [EntityConfigEntity](EntityConfigEntity.md) GetEntityConfigEntity ( ) {: .copyable aria-label='Functions' }
Returns the corresponding [EntityConfig](EntityConfig.md) entry for this entity.

___
### GetFearCountdown () {: aria-label='Functions' }
#### int GetFearCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetFireDamageCooldown () {: aria-label='Functions' }
#### int GetFireDamageCooldown ( ) {: .copyable aria-label='Functions' }

___
### GetFreezeCountdown () {: aria-label='Functions' }
#### int GetFreezeCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetHitListIndex () {: aria-label='Functions' }
#### int GetHitListIndex ( ) {: .copyable aria-label='Functions' }

___
### GetIceCountdown () {: aria-label='Functions' }
#### int GetIceCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetKnockbackCountdown () {: aria-label='Functions' }
#### int GetKnockbackCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetKnockbackDirection () {: aria-label='Functions' }
#### [Vector](Vector.md) GetKnockbackDirection ( ) {: .copyable aria-label='Functions' }

___
### GetMagnetizedCountdown () {: aria-label='Functions' }
#### int GetMagnetizedCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetMidasFreezeCountdown () {: aria-label='Functions' }
#### int GetMidasFreezeCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetMinecart () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) GetMinecart ( ) {: .copyable aria-label='Functions' }
Returns the minecart the entity is riding.

???+ note "Return behavior"
    If the entity is not riding a minecart, this function returns `nil`.

___
### GetNullCapsule () {: aria-label='Functions' }
#### [Capsule](Capsule.md) GetNullCapsule ( string NullLayerName ) {: .copyable aria-label='Functions' }

___
### GetNullOffset () {: aria-label='Functions' }
#### [Vector](Vector.md) GetNullOffset ( string NullLayerName ) {: .copyable aria-label='Functions' }
Returns position of null layer mark. Alternatively returns Vector.Zero if the layer is not visible, has no frame available for the current animation, or for other unknown reasons.

___
### GetPauseTime () {: aria-label='Functions' }
#### int GetPauseTime ( ) {: .copyable aria-label='Functions' }

___
### GetPoisonCountdown () {: aria-label='Functions' }
#### int GetPoisonCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetPoisonDamageTimer () {: aria-label='Functions' }
#### int GetPoisonDamageTimer ( ) {: .copyable aria-label='Functions' }

___
### GetPosVel () {: aria-label='Functions' }
#### [PosVel](https://wofsauge.github.io/IsaacDocs/rep/PlayerTypes_PosVel.html) GetPosVel ( ) {: .copyable aria-label='Functions' }
Returns 2 values, both Vectors. 1st the Position of the entity, 2nd the Velocity of the entity.

___
### GetPredictedTargetPosition () {: aria-label='Functions' }
#### [Vector](Vector.md) GetPredictedTargetPosition ( [Entity](Entity.md) Target, float Delay ) {: .copyable aria-label='Functions' }
The prediction is the target's current position plus their velocity multiplied by the distance between this and the target.
`Delay` acts as a multiplier for how far ahead the prediction should be. For example, `1.0` would predict where the target's velocity would place them on the next update.

___
### GetShadowSize () {: aria-label='Functions' }
#### float GetShadowSize ( ) {: .copyable aria-label='Functions' }

___
### GetShrinkCountdown () {: aria-label='Functions' }
#### int GetShrinkCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetSlowingCountdown () {: aria-label='Functions' }
#### int GetSlowingCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetSpeedMultiplier () {: aria-label='Functions' }
#### float GetSpeedMultiplier ( ) {: .copyable aria-label='Functions' }
???+ warning "Depreciation notice"
    This variable is actually the Entity's time scale. A properly named replacement function will be added in a future version.

___
### GetType () {: aria-label='Functions' }
#### [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) GetType ( ) {: .copyable aria-label='Functions' }

___
### GetWeaknessCountdown () {: aria-label='Functions' }
#### int GetWeaknessCountdown ( ) {: .copyable aria-label='Functions' }

___
### GiveMinecart () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) GiveMinecart ( [Vector](Vector.md) Position, [Vector](Vector.md) Velocity ) {: .copyable aria-label='Functions' }

___
### IgnoreEffectFromFriendly () {: aria-label='Functions' }
#### boolean IgnoreEffectFromFriendly ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source ) {: .copyable aria-label='Functions' }
Used to determine if this entity should ignore any status effect coming from `Source`.

___
### MakeBloodPoof () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) MakeBloodPoof ( [Vector](Vector.md) Position = self.Position, [Color](Color.md) Color = default, float Scale = 1.0 ) {: .copyable aria-label='Functions' }
This function spawns two blood poof effects of subtypes 3 and 4; the second of which will be the the Child of the one returned.

___
### MakeGroundPoof () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) MakeGroundPoof ( [Vector](Vector.md) Position = self.Position, [Color](Color.md) Color = default, float Scale = 1.0 ) {: .copyable aria-label='Functions' }
This function spawns two dust poof effects of subtypes 1 and 2; the second of which will be the the Child of the one returned.

___
### SetBaitedCountdown () {: aria-label='Functions' }
#### void SetBaitedCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetBleedingCountdown () {: aria-label='Functions' }
#### void SetBleedingCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetBossStatusEffectCooldown () {: aria-label='Functions' }
#### void SetBossStatusEffectCooldown ( int Cooldown ) {: .copyable aria-label='Functions' }

___
### SetBrimstoneMarkCountdown () {: aria-label='Functions' }
#### void SetBrimstoneMarkCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetBurnCountdown () {: aria-label='Functions' }
#### void SetBurnCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetBurnDamageTimer () {: aria-label='Functions' }
#### void SetBurnDamageTimer ( int Timer ) {: .copyable aria-label='Functions' }

___
### SetCharmedCountdown () {: aria-label='Functions' }
#### void SetCharmedCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetColorParams () {: aria-label='Functions' }
#### void SetColorParams ( [ColorParams](ColorParams.md)[] Params ) {: .copyable aria-label='Functions' }
Sets colors to be used alongside their parameters.

___
### SetConfusionCountdown () {: aria-label='Functions' }
#### void SetConfusionCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetDamageCountdown () {: aria-label='Functions' }
#### void SetDamageCountdown ( int countdown ) {: .copyable aria-label='Functions' }
Sets how many frames must pass before the entity can take damage that has the DAMAGE_COUNTDOWN [DamageFlag](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html).

Note that this is NOT the same as the player's invincibility frames (`EntityPlayer:GetDamageCooldown()`). The DAMAGE_COUNTDOWN [DamageFlag](https://wofsauge.github.io/IsaacDocs/rep/enums/DamageFlag.html) and this associated countdown are typically used to control how rapidly an enemy will take damage from the few sources that uses that flag, such as the the collision damage effects from the "My Little Unicorn", "The Nail", and "The Gamekid".

___
### SetDead () {: aria-label='Functions' }
#### void SetDead ( boolean IsDead ) {: .copyable aria-label='Functions' }

___
### SetFearCountdown () {: aria-label='Functions' }
#### void SetFearCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetFireDamageCooldown () {: aria-label='Functions' }
#### void SetFireDamageCooldown ( int Cooldown ) {: .copyable aria-label='Functions' }

___
### SetFreezeCountdown () {: aria-label='Functions' }
#### void SetFreezeCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetIceCountdown () {: aria-label='Functions' }
#### void SetIceCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetInvincible () {: aria-label='Functions' }
#### void SetInvincible ( boolean IsInvincible ) {: .copyable aria-label='Functions' }

___
### SetKnockbackCountdown () {: aria-label='Functions' }
#### void SetKnockbackCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetKnockbackDirection () {: aria-label='Functions' }
#### void SetKnockbackDirection ( [Vector](Vector.md) Direction ) {: .copyable aria-label='Functions' }

___
### SetMagnetizedCountdown () {: aria-label='Functions' }
#### void SetMagnetizedCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetMidasFreezeCountdown () {: aria-label='Functions' }
#### void SetMidasFreezeCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetPauseTime () {: aria-label='Functions' }
#### void SetPauseTime ( int Duration ) {: .copyable aria-label='Functions' }

___
### SetPoisonCountdown () {: aria-label='Functions' }
#### void SetPoisonCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetPoisonDamageTimer () {: aria-label='Functions' }
#### void SetPoisonDamageTimer ( int Timer ) {: .copyable aria-label='Functions' }

___
### SetShadowSize () {: aria-label='Functions' }
#### float SetShadowSize ( float Size ) {: .copyable aria-label='Functions' }

___
### SetShrinkCountdown () {: aria-label='Functions' }
#### void SetShrinkCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetSlowingCountdown () {: aria-label='Functions' }
#### void SetSlowingCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SetSpeedMultiplier () {: aria-label='Functions' }
#### void SetSpeedMultiplier ( float Amount ) {: .copyable aria-label='Functions' }
???+ warning "Depreciation notice"
    This variable is actually the Entity's time scale. A properly named replacement function will be added in a future version.

___
### SetWeaknessCountdown () {: aria-label='Functions' }
#### void SetWeaknessCountdown ( int Countdown ) {: .copyable aria-label='Functions' }

___
### SpawnBloodEffect () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) SpawnBloodEffect ( int SubType = 0, [Vector](Vector.md) position = self.Position, [Vector](Vector.md) Offset = Vector.Zero, [Color](Color.md) Color = Default, [Vector](Vector.md) Velocity = Vector.Zero ) {: .copyable aria-label='Functions' }

___
### SpawnWaterImpactEffects () {: aria-label='Functions' }
#### void SpawnWaterImpactEffects ( [Vector](Vector.md) Position, [Vector](Vector.md) Velocity = Vector.Zero, float Strength ) {: .copyable aria-label='Functions' }

???+ warning "Warning"
    This function will only spawn effects if the Room's [water amount](Room.md#getwateramount) is above or equal to `0.2`.

___
### TeleportToRandomPosition () {: aria-label='Functions' }
#### void TeleportToRandomPosition ( ) {: .copyable aria-label='Functions' }

___
### ToDelirium () {: aria-label='Functions' }
#### [EntityDelirium](EntityDelirium.md) ToDelirium ( ) {: .copyable aria-label='Functions' }
Convert an [Entity](Entity.md) userdata to an [EntityDelirium](EntityDelirium.md) userdata.
The conversion will only succeed if the source entity is an instance of Delirium (in its normal form or in a transformed form).

???+ note "Return behavior"
    If the conversion fails, this function returns `nil`.

___
### ToSlot () {: aria-label='Functions' }
#### [EntitySlot](EntitySlot.md) ToSlot ( ) {: .copyable aria-label='Functions' }
Used to cast an [Entity](Entity.md) object to an [EntitySlot](EntitySlot.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.

___

### TryThrow () {: aria-label='Functions' }
#### boolean TryThrow ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, [Vector](Vector.md) ThrowDirection, float Force ) {: .copyable aria-label='Functions' }

___
