---
tags:
  - Class
---
# Class "GridEntity"

## Functions

### Get·Alt·Rock·Type () {: aria-label='Functions' }
#### int GetAltRockType ( int backdropId ) {: .copyable aria-label='Functions' }

___
### Get·Render·Position () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetRenderPosition ( ) {: .copyable aria-label='Functions' }

___
### Hurt·Damage () {: aria-label='Functions' }
#### void HurtDamage ( [Entity](Entity.md) entity, int damage, int damageFlags, float unknown, boolean unknown ) {: .copyable aria-label='Functions' }

___
### Hurt·Surroundings () {: aria-label='Functions' }
#### void HurtSurroundings ( int damage, int gridType, int damageFlags, float unknown, boolean unknown ) {: .copyable aria-label='Functions' }

___
### Is·Breakable·Rock () {: aria-label='Functions' }
#### void IsBreakableRock ( ) {: .copyable aria-label='Functions' }

___
### Play·Break·Sound () {: aria-label='Functions' }
#### void PlayBreakSound ( int gridType, int backdropId ) {: .copyable aria-label='Functions' }

___
### Register·Rock·Destroyed () {: aria-label='Functions' }
#### void RegisterRockDestroyed ( int gridType ) {: .copyable aria-label='Functions' }

___
### Render·Top () {: aria-label='Functions' }
#### void RenderTop ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Offset ) {: .copyable aria-label='Functions' }

___

### To·Decoration () {: aria-label='Functions' }
#### (GridEntityDecoration)[GridEntityDoor.md] ToDecoration ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityDecoration](GridEntityDecoration.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### To·Fire () {: aria-label='Functions' }
#### (GridEntityFire)[GridEntityDoor.md] ToFire ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityFire](GridEntityFire.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### To·Gravity () {: aria-label='Functions' }
#### (GridEntityGravity)[GridEntityDoor.md] ToGravity ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityGravity](GridEntityGravity.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### To·Stairs () {: aria-label='Functions' }
#### (GridEntityStairs)[GridEntityDoor.md] ToStairs ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityStairs](GridEntityStairs.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### To·Statue () {: aria-label='Functions' }
#### (GridEntityStatue)[GridEntityDoor.md] ToStatue ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityStatue](GridEntityStatue.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### To·TrapDoor () {: aria-label='Functions' }
#### (GridEntityTrapDoor)[GridEntityDoor.md] ToTrapDoor ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityTrapDoor](GridEntityTrapDoor.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### To·Wall () {: aria-label='Functions' }
#### (GridEntityWall)[GridEntityDoor.md] ToWall ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityWall](GridEntityWall.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### To·Web () {: aria-label='Functions' }
#### (GridEntityWeb)[GridEntityDoor.md] ToWeb ( ) {: .copyable aria-label='Functions' }

Used to cast a [GridEntity](GridEntity.md) object to a [GridEntityWeb](GridEntityWeb.md) object.

???+ note "Return behavior"
    If the conversion is not successful, this function returns `nil`.
	
___

### Try·Spawn·Ladder () {: aria-label='Functions' }
#### void TrySpawnLadder ( ) {: .copyable aria-label='Functions' }

___
### Try·Spawn·Worms () {: aria-label='Functions' }
#### void TrySpawnWorms ( ) {: .copyable aria-label='Functions' }

___
### Update·Collision () {: aria-label='Functions' }
#### void UpdateCollision ( ) {: .copyable aria-label='Functions' }

___
### Update·Neighbors () {: aria-label='Functions' }
#### void UpdateNeighbors ( ) {: .copyable aria-label='Functions' }

___