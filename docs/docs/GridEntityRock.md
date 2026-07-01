---
tags:
  - Class
---
# Class "GridEntityRock"

## Functions

### GetAltRockType () {: aria-label='Functions' }
#### int GetAltRockType ( [BackdropType](https://wofsauge.github.io/IsaacDocs/rep/enums/BackdropType.html) Backdrop = 0 ) {: .copyable aria-label='Functions' }

___  
### PlayBreakSound () {: aria-label='Functions' }
#### void PlayBreakSound ( [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type, [BackdropType](https://wofsauge.github.io/IsaacDocs/rep/enums/BackdropType.html) Backdrop = 0 ) {: .copyable aria-label='Functions' }    

___  
### RegisterRockDestroyed () {: aria-label='Functions' }
#### void RegisterRockDestroyed ( [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type ) {: .copyable aria-label='Functions' }

___
### RenderTop () {: aria-label='Functions' }
#### void RenderTop ( [Vector](Vector.md) Offset ) {: .copyable aria-label='Functions' }

___
### SpawnDrops () {: aria-label='Functions' }
#### static void SpawnDrops ( [Vector](Vector.md) Position, [GridEntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/GridEntityType.html) Type, int GridVariant, int Seed, boolean Unknown, [BackdropType](https://wofsauge.github.io/IsaacDocs/rep/enums/BackdropType.html) Backdrop = 0 ) {: .copyable aria-label='Functions' }

___
### TrySpawnLadder () {: aria-label='Functions' }
#### void TrySpawnLadder ( ) {: .copyable aria-label='Functions' }

___
### TrySpawnWorms () {: aria-label='Functions' }
#### void TrySpawnWorms ( ) {: .copyable aria-label='Functions' }

___
### UpdateCollision () {: aria-label='Functions' }
#### void UpdateCollision ( ) {: .copyable aria-label='Functions' }

___
### UpdateNeighbors () {: aria-label='Functions' }
#### void UpdateNeighbors ( ) {: .copyable aria-label='Functions' }
If the rock is currently "connected" to other rocks as a "big rock", this function will break its connections to its neighbors. The graphics of the neighboring rocks will be updated appropriately, but note that THIS rock's graphics will not be updated.

The game calls this function when a rock is destroyed or lifted. For it to succeed, the rock must still have the appropriate Variant for a big rock (1000+).

___
