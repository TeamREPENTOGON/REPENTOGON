---
tags:
  - Class
---
# Class "EntityNPC"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"

## Modified Functions

### PlaySound () {: aria-label='Modified Functions' }
#### void PlaySound ( int ID, float Volume = 1.0, int FrameDelay = 2, boolean Loop = true, float Pitch = 1.0 ) {: .copyable aria-label='Modified Functions' }
All arguments besides `ID` are now optional.

___

## Functions

### FireBossProjectilesEx () {: aria-label='Functions' }
#### [EntityProjectile](EntityProjectile.md)[] FireBossProjectilesEx ( int NumProjectiles, [Vector](Vector.md) TargetPos, float TrajectoryModifier, [ProjectileParams](https://wofsauge.github.io/IsaacDocs/rep/ProjectileParams.html) Params ) {: .copyable aria-label='Functions' }
Same as [FireBossProjectiles](EntityNPC.md#firebossprojectiles), but returns a table containing the list of spawned projectiles.

___
### FireGridEntity () {: aria-label='Functions' }
#### [EntityProjectile](EntityProjectile.md) FireGridEntity ( [Sprite](Sprite.md) GridEntitySprite, [GridEntityDesc](https://wofsauge.github.io/IsaacDocs/rep/GridEntityDesc.html) GridEntityDesc, [Vector](Vector.md) Velocity, [BackdropType](https://wofsauge.github.io/IsaacDocs/rep/enums/BackdropType.html) Backdrop = BackdropType.BASEMENT ) {: .copyable aria-label='Functions' }

___
### FireProjectilesEx () {: aria-label='Functions' }
#### [EntityProjectile](EntityProjectile.md)[] FireProjectilesEx ([Vector](Vector.md) Position, [Vector](Vector.md) Velocity, ProjectilesMode Mode, [ProjectileParams](https://wofsauge.github.io/IsaacDocs/rep/ProjectileParams.html) Params) {: .copyable aria-label='Functions' }
Same as [FireProjectiles](EntityNPC.md#fireprojectiles), but returns a table containing the list of spawned projectiles.

___
### GetControllerId () {: aria-label='Functions' }
#### int GetControllerId ( ) {: .copyable aria-label='Functions' }
Returns the ControllerId for the NPC, which indicates which player is controlling it. Will return `-1` when not being controlled by any player.

___
### GetDirtColor () {: aria-label='Functions' }
#### [Color](Color.md) GetDirtColor ( ) {: .copyable aria-label='Functions' }
Returns the dynamic dirt color of the entity. This lets entities like Nightcrawler blend in to the environment.

___
### GetHitList () {: aria-label='Functions' }
#### int[] GetHitList ( ) {: .copyable aria-label='Functions' }

___
### GetShieldStrength () {: aria-label='Functions' }
#### float GetShieldStrength ( ) {: .copyable aria-label='Functions' }

___
### SetControllerId () {: aria-label='Functions' }
#### int SetControllerId ( int ControllerId ) {: .copyable aria-label='Functions' }
Sets the ControllerId for the NPC, which indicates which player will control it. Set it to `-1` for no player controls (back to normal behaviour).

___
### SetShieldStrength () {: aria-label='Functions' }
#### void SetShieldStrength ( float Strength ) {: .copyable aria-label='Functions' }

___
### ShootMaggotProjectile () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) ShootMaggotProjectile ( [Vector](Vector.md) Target, float Velocity = -24.0, float YOffset = -8.0 ) {: .copyable aria-label='Functions' }

___
### SpawnBloodCloud () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) SpawnBloodCloud ( [Vector](Vector.md) Position, [Color](Color.md) Color ) {: .copyable aria-label='Functions' }

___
### SpawnBloodSplash () {: aria-label='Functions' }
#### void SpawnBloodSplash ( ) {: .copyable aria-label='Functions' }

___
### ThrowLeech () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) ThrowLeech ( [Entity](Entity.md) Source, [Vector](Vector.md) Target, float YPosOffset = -10.0, boolean Big = false ) {: .copyable aria-label='Functions' }

___
### ThrowMaggotAtPos () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) ThrowMaggotAtPos ( [Vector](Vector.md) Origin, float Velocity = -8.0, float YOffset = 0.0 ) {: .copyable aria-label='Functions' }

___
### ThrowRockSpider () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) ThrowRockSpider ( [Entity](Entity.md) Source, [Vector](Vector.md) Target, int Variant = 0, float YPosOffset = -10.0 ) {: .copyable aria-label='Functions' }

___
### ThrowStrider () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) ThrowStrider ( [Entity](Entity.md) Source, [Vector](Vector.md) Target ) {: .copyable aria-label='Functions' }

___
### TryForceTarget () {: aria-label='Functions' }
#### boolean TryForceTarget ( [Entity](Entity.md) Target, int Duration ) {: .copyable aria-label='Functions' }
Used by Lost Fly to force this NPC to focus on a specific target.

___
### TryThrow () {: aria-label='Functions' }
#### boolean TryThrow ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) Source, [Vector](Vector.md) Direction, float Force ) {: .copyable aria-label='Functions' }

???+ info "Info"
    `Force` only applies to NPC poop (it's modified and then used as V1.y, with V1.x being -20.0) and may be incorrect. This needs further investigation.

___
### UpdateDirtColor () {: aria-label='Functions' }
#### void UpdateDirtColor ( boolean Immediate ) {: .copyable aria-label='Functions' }
Instructs the entity to update its dirt color. This is generally done automatically on vanilla entities, but up until now, modded ones have been quite limited in this regard.

If `Immediate` is set, the dirt color will be set to exactly what is beneath the entity. Otherwise, it will be updated smoothly over the course of multiple frames.

___

## Variables
### V1 {: aria-label='Variables' }
#### [Vector](Vector.md) V1 {: .copyable aria-label='Variables' }
Fix of original function that now correctly returns a pointer to the Vector.
___
### V2 {: aria-label='Variables' }
#### [Vector](Vector.md) V2 {: .copyable aria-label='Variables' }
Fix of original function that now correctly returns a pointer to the Vector.
___
