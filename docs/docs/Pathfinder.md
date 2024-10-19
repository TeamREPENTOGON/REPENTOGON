---
tags:
  - Class
---
# Class "Pathfinder"

???+ info
    You can get this class by using the following functions:
	
	* [EntityFamiliar:GetPathfinder()](EntityFamiliar.md#getpathfinder)
    * [EntityNPC:GetPathfinder()](EntityNPC.md#getpathfinder)

    ???+ example "Example Code"
        ```lua
        local pathfinder = npc:GetPathfinder()
        ```

???+ warning "Warning"
	The following functions are only affected when using the new `GetPathfinder` method! `EntityNPC.Pathfinder` retains old behavior for compatibility with existing mods.
___
## Modified Functions

### Find·Grid·Path () {: aria-label='Functions' }
[ ](#){: .tooltip .badge }
#### void FindGridPath ( [Vector](Vector.md) Pos, float Speed, int PathMarker, boolean UseDirectPath ) {: .copyable aria-label='Functions' }
`UseDirectPath` now works as intended (moves directly towards target when unobstructed by grids).

___
### Move·Randomly () {: aria-label='Functions' }
[ ](#){: .tooltip .badge }
#### boolean MoveRandomly ( boolean IgnoreStatusEffects ) {: .copyable aria-label='Functions' }
Now works as intended. Make sure to use [Entity:MultiplyFriction](https://wofsauge.github.io/IsaacDocs/rep/Entity.html#multiplyfriction) with a < 1 value after calling this, otherwise the Entity will constantly accelerate.

___
### Move·Randomly·Axis·Aligned () {: aria-label='Functions' }
[ ](#){: .tooltip .badge }
#### void MoveRandomlyAxisAligned ( float Speed, boolean IgnoreStatusEffects ) {: .copyable aria-label='Functions' }
Now works as intended. Make sure to use [Entity:MultiplyFriction](https://wofsauge.github.io/IsaacDocs/rep/Entity.html#multiplyfriction) with a < 1 value after calling this, otherwise the Entity will constantly accelerate.

___
### Move·Randomly·Boss () {: aria-label='Functions' }
[ ](#){: .tooltip .badge }
#### void MoveRandomlyBoss ( boolean IgnoreStatusEffects ) {: .copyable aria-label='Functions' }
Now works as intended. Make sure to use [Entity:MultiplyFriction](https://wofsauge.github.io/IsaacDocs/rep/Entity.html#multiplyfriction) with a < 1 value after calling this, otherwise the Entity will constantly accelerate.

___