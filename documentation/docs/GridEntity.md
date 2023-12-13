---
tags:
  - Class
---
# Class "GridEntity"

## Functions

### GetAltRockType () {: aria-label='Functions' }
#### int GetAltRockType ( int backdropId ) {: .copyable aria-label='Functions' }

___
### GetRenderPosition () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetRenderPosition ( ) {: .copyable aria-label='Functions' }

___
### HurtDamage () {: aria-label='Functions' }
#### void HurtDamage ( [Entity](Entity.md) entity, int damage, int damageFlags, float unknown, boolean unknown ) {: .copyable aria-label='Functions' }

___
### HurtSurroundings () {: aria-label='Functions' }
#### void HurtSurroundings ( int damage, int gridType, int damageFlags, float unknown, boolean unknown ) {: .copyable aria-label='Functions' }

___
### IsBreakableRock () {: aria-label='Functions' }
#### void IsBreakableRock ( ) {: .copyable aria-label='Functions' }

___
### PlayBreakSound () {: aria-label='Functions' }
#### void PlayBreakSound ( int gridType, int backdropId ) {: .copyable aria-label='Functions' }

___
### RegisterRockDestroyed () {: aria-label='Functions' }
#### void RegisterRockDestroyed ( int gridType ) {: .copyable aria-label='Functions' }

___
### RenderTop () {: aria-label='Functions' }
#### void RenderTop ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset ) {: .copyable aria-label='Functions' }

___

### ToDecoration () {: aria-label='Functions' }
#### [GridEntityDecoration](GridEntityDecoration.md) ToDecoration ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityDecoration](GridEntityDecoration.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### ToFire () {: aria-label='Functions' }
#### [GridEntityFire](GridEntityFire.md) ToFire ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityFire](GridEntityFire.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### ToGravity () {: aria-label='Functions' }
#### [GridEntityGravity](GridEntityGravity.md) ToGravity ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityGravity](GridEntityGravity.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### ToStairs () {: aria-label='Functions' }
#### [GridEntityStairs](GridEntityStairs.md) ToStairs ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityStairs](GridEntityStairs.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### ToStatue () {: aria-label='Functions' }
#### [GridEntityStatue](GridEntityStatue.md) ToStatue ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityStatue](GridEntityStatue.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### ToTrapDoor () {: aria-label='Functions' }
#### [GridEntityTrapDoor](GridEntityTrapDoor.md) ToTrapDoor ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityTrapDoor](GridEntityTrapDoor.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### ToWall () {: aria-label='Functions' }
#### [GridEntityWall](GridEntityWall.md)ToWall ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityWall](GridEntityWall.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### ToWeb () {: aria-label='Functions' }
#### [GridEntityWeb](GridEntityWeb.md) ToWeb ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityWeb](GridEntityWeb.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
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

___