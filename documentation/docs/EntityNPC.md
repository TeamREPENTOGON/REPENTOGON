# Class "EntityNPC"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### Get·Dirt·Color() {: aria-label='Functions' }
#### [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) GetDirtColor ( ) {: .copyable aria-label='Functions' }
Returns the dynamic dirt color of the entity. This lets entities like Nightcrawler blend in to the environment.

___
### Update·Dirt·Color() {: aria-label='Functions' }
#### void UpdateDirtColor ( boolean Immediate ) {: .copyable aria-label='Functions' }
Instructs the entity to update its dirt color. This is generally done automatically on vanilla entities, but up until now, modded ones have been quite limited in this reagard.

If `Immediate` is set, the dirt color will be set to exactly what is beneath the entity. Otherwise, it will be updated smoothly over the course of multiple frames.

___
### Fire·Projectiles·Ex() {: aria-label='Functions' }
#### table FireProjectilesEx ([Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) velocity, ProjectilesMode mode, [ProjectileParams](https://wofsauge.github.io/IsaacDocs/rep/ProjectileParams.html) params) {: .copyable aria-label='Functions' }
Same as [FireProjectiles](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html#fireprojectiles), but returns a table containing the list of spawned projectiles.

___
### Fire·Boss·Projectiles·Ex() {: aria-label='Functions' }
#### table FireBossProjectilesEx ( int numProjectiles, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) targetPos, float trajectoryModifier, [ProjectileParams](https://wofsauge.github.io/IsaacDocs/rep/ProjectileParams.html) params ) {: .copyable aria-label='Functions' }
Same as [FireBossProjectiles](https://wofsauge.github.io/IsaacDocs/rep/EntityNPC.html#firebossprojectiles), but returns a table containing the list of spawned projectiles.

___
### Get·Controller·Id() {: aria-label='Functions' }
#### int GetControllerId ( ) {: .copyable aria-label='Functions' }
Returns the ControllerId for the NPC, which indicates which player is controlling it. Will return -1 when its not being controlled by any player.

___
### Get·Hit·List() {: aria-label='Functions' }
#### table GetHitList ( ) {: .copyable aria-label='Functions' }

___
### Get·Shield·Strength() {: aria-label='Functions' }
#### float GetShieldStrength ( ) {: .copyable aria-label='Functions' }

___
### Set·Controller·Id() {: aria-label='Functions' }
#### int SetControllerId ( int ControllerId ) {: .copyable aria-label='Functions' }
Sets the ControllerId for the NPC, which indicates which player will control it. Set it to -1 for no player controls(back to normal behaviour).

___
### Set·Shield·Strength() {: aria-label='Functions' }
#### void SetShieldStrength ( float Strength ) {: .copyable aria-label='Functions' }

___
### Try·Force·Target() {: aria-label='Functions' }
#### boolean TryForceTarget ( [Entity](Entity.md) target, int duration ) {: .copyable aria-label='Functions' }

___
