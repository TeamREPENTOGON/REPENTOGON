# Class "EntityNPC"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"

## Functions

### Fire·Boss·Projectiles·Ex() {: aria-label='Functions' }
#### table FireBossProjectilesEx ( int numProjectiles, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) targetPos, float trajectoryModifier, [ProjectileParams](https://wofsauge.github.io/IsaacDocs/rep/ProjectileParams.html) params ) {: .copyable aria-label='Functions' }
Same as [FireBossProjectiles](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html#firebossprojectiles), but returns a table containing the list of spawned projectiles.

___
### Fire·Grid·Entity () {: aria-label='Functions' }
#### [EntityProjectile](https://wofsauge.github.io/IsaacDocs/rep/EntityProjectile.html) FireGridEntity ( [Sprite](Sprite.md) sprite, [GridEntityDesc](https://wofsauge.github.io/IsaacDocs/rep/GridEntityDesc.html) desc, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) velocity, int backdrop = 1 ) {: .copyable aria-label='Functions' }

___
### Fire·Projectiles·Ex() {: aria-label='Functions' }
#### table FireProjectilesEx ([Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) velocity, ProjectilesMode mode, [ProjectileParams](https://wofsauge.github.io/IsaacDocs/rep/ProjectileParams.html) params) {: .copyable aria-label='Functions' }
Same as [FireProjectiles](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html#fireprojectiles), but returns a table containing the list of spawned projectiles.

___
### Get·Controller·Id() {: aria-label='Functions' }
#### int GetControllerId ( ) {: .copyable aria-label='Functions' }
Returns the ControllerId for the NPC, which indicates which player is controlling it. Will return -1 when its not being controlled by any player.

___
### Get·Dirt·Color() {: aria-label='Functions' }
#### [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) GetDirtColor ( ) {: .copyable aria-label='Functions' }
Returns the dynamic dirt color of the entity. This lets entities like Nightcrawler blend in to the environment.

___
### Get·Hit·List() {: aria-label='Functions' }
#### table GetHitList ( ) {: .copyable aria-label='Functions' }

___
### Get·Shield·Strength() {: aria-label='Functions' }
#### float GetShieldStrength ( ) {: .copyable aria-label='Functions' }

___
### Play·Sound () {: aria-label='Functions' }
#### void PlaySound ( int id, float volume = 1.0, int framedelay = 2, boolean loop = false, float pitch = 1.0 ) {: .copyable aria-label='Functions' }

___
### Set·Controller·Id() {: aria-label='Functions' }
#### int SetControllerId ( int ControllerId ) {: .copyable aria-label='Functions' }
Sets the ControllerId for the NPC, which indicates which player will control it. Set it to -1 for no player controls(back to normal behaviour).

___
### Set·Shield·Strength() {: aria-label='Functions' }
#### void SetShieldStrength ( float Strength ) {: .copyable aria-label='Functions' }

___
### Spawn·Blood·Cloud () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) SpawnBloodCloud ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position, [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) color ) {: .copyable aria-label='Functions' }

___
### Spawn·Blood·Effect () {: aria-label='Functions' }
#### [EntityEffect](EntityEffect.md) SpawnBloodEffect ( int subtype = 0, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position = entityPosition, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) offset = nil, [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) color = default, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) velocity = nil ) {: .copyable aria-label='Functions' }

___
### Spawn·Blood·Splash () {: aria-label='Functions' }
#### void SpawnBloodSplash ( ) {: .copyable aria-label='Functions' }

___
### Throw·Maggot·At·Pos () {: aria-label='Functions' }
#### [EntityNPC](EntityNPC.md) ThrowMaggotAtPos ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) origin, float velocity = -8.0f, float yOffset = 0.0f ) {: .copyable aria-label='Functions' }

___
### Try·Force·Target() {: aria-label='Functions' }
#### boolean TryForceTarget ( [Entity](Entity.md) target, int duration ) {: .copyable aria-label='Functions' }

___
### Try·Throw () {: aria-label='Functions' }
#### boolean TryThrow ( [EntityRef](https://wofsauge.github.io/IsaacDocs/rep/EntityRef.html) ref, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) dir, int force ) {: .copyable aria-label='Functions' }

___
### Update·Dirt·Color() {: aria-label='Functions' }
#### void UpdateDirtColor ( boolean Immediate ) {: .copyable aria-label='Functions' }
Instructs the entity to update its dirt color. This is generally done automatically on vanilla entities, but up until now, modded ones have been quite limited in this reagard.

If `Immediate` is set, the dirt color will be set to exactly what is beneath the entity. Otherwise, it will be updated smoothly over the course of multiple frames.

___

## Variables
### V1 {: aria-label='Variables' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) V1 {: .copyable aria-label='Variables' }
Fix of original function that now correctly returns a pointer to the vector.
___
### V2 {: aria-label='Variables' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) V2 {: .copyable aria-label='Variables' }
Fix of original function that now correctly returns a pointer to the vector.
___
