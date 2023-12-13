---
tags:
  - Class
---
# Class "EntityNPC"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"

## Functions

### FireBossProjectilesEx () {: aria-label='Functions' }
#### [EntityProjectile](EntityProjectile.md)[] FireBossProjectilesEx ( int numProjectiles, [Vector](Vector.md) targetPos, float trajectoryModifier, [ProjectileParams](https://wofsauge.github.io/IsaacDocs/rep/ProjectileParams.html) params ) {: .copyable aria-label='Functions' }
Same as [FireBossProjectiles](EntityNPC.md#firebossprojectiles), but returns a table containing the list of spawned projectiles.

___
### FireGridEntity () {: aria-label='Functions' }
#### [EntityProjectile](EntityProjectile.md) FireGridEntity ( [Sprite](Sprite.md) sprite, [GridEntityDesc](https://wofsauge.github.io/IsaacDocs/rep/GridEntityDesc.html) desc, [Vector](Vector.md) velocity, int backdrop = 1 ) {: .copyable aria-label='Functions' }

___
### FireProjectilesEx () {: aria-label='Functions' }
#### [EntityProjectile](EntityProjectile.md)[] FireProjectilesEx ([Vector](Vector.md) position, [Vector](Vector.md) velocity, ProjectilesMode mode, [ProjectileParams](https://wofsauge.github.io/IsaacDocs/rep/ProjectileParams.html) params) {: .copyable aria-label='Functions' }
Same as [FireProjectiles](EntityNPC.md#fireprojectiles), but returns a table containing the list of spawned projectiles.

___
### GetControllerId () {: aria-label='Functions' }
#### int GetControllerId ( ) {: .copyable aria-label='Functions' }
Returns the ControllerId for the NPC, which indicates which player is controlling it. Will return -1 when its not being controlled by any player.

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
### PlaySound () {: aria-label='Functions' }
#### void PlaySound ( int id, float volume = 1.0, int framedelay = 2, boolean loop = false, float pitch = 1.0 ) {: .copyable aria-label='Functions' }

___
### SetControllerId () {: aria-label='Functions' }
#### int SetControllerId ( int ControllerId ) {: .copyable aria-label='Functions' }
Sets the ControllerId for the NPC, which indicates which player will control it. Set it to -1 for no player controls(back to normal behaviour).

___
### SetShieldStrength () {: aria-label='Functions' }
#### void SetShieldStrength ( float Strength ) {: .copyable aria-label='Functions' }

___
### SpawnBloodCloud () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) SpawnBloodCloud ( [Vector](Vector.md) position, [Color](Color.md) color ) {: .copyable aria-label='Functions' }

___
### SpawnBloodSplash () {: aria-label='Functions' }
#### void SpawnBloodSplash ( ) {: .copyable aria-label='Functions' }

___
### ThrowMaggotAtPos () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) ThrowMaggotAtPos ( [Vector](Vector.md) origin, float velocity = -8.0f, float yOffset = 0.0f ) {: .copyable aria-label='Functions' }

___
### TryForceTarget () {: aria-label='Functions' }
#### boolean TryForceTarget ( [Entity](Entity.md) target, int duration ) {: .copyable aria-label='Functions' }

___
### TryThrow () {: aria-label='Functions' }
#### boolean TryThrow ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) ref, [Vector](Vector.md) dir, int force ) {: .copyable aria-label='Functions' }

___
### UpdateDirtColor () {: aria-label='Functions' }
#### void UpdateDirtColor ( boolean Immediate ) {: .copyable aria-label='Functions' }
Instructs the entity to update its dirt color. This is generally done automatically on vanilla entities, but up until now, modded ones have been quite limited in this reagard.

If `Immediate` is set, the dirt color will be set to exactly what is beneath the entity. Otherwise, it will be updated smoothly over the course of multiple frames.

___

## Variables
### V1 {: aria-label='Variables' }
#### [Vector](Vector.md) V1 {: .copyable aria-label='Variables' }
Fix of original function that now correctly returns a pointer to the vector.
___
### V2 {: aria-label='Variables' }
#### [Vector](Vector.md) V2 {: .copyable aria-label='Variables' }
Fix of original function that now correctly returns a pointer to the vector.
___
