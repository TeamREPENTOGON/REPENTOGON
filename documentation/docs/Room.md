# Class "Room"

## Functions

### Can·Spawn·Obstacle·At·Position () {: aria-label='Functions' }
#### boolean CanSpawnObstacleAtPosition ( int GridIndex, boolean Force ) {: .copyable aria-label='Functions' }

___

### Get·Camera () {: aria-label='Functions' }
#### [Camera](Camera.md) GetCamera ( ) {: .copyable aria-label='Functions' }
Returns a [Camera](Camera.md) object.

___
### Get·Floor·Color () {: aria-label='Functions' }
#### [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) GetWaterColor ( ) {: .copyable aria-label='Functions' }

___
### Get·Shop·Item·Price () {: aria-label='Functions' }
#### int GetShopItemPrice ( int EntityVariant, int EntitySubType, int ShopItemID ) {: .copyable aria-label='Functions' }
Returns the price of the item.

___
### Get·Water·Amount () {: aria-label='Functions' }
#### float GetWaterAmount ( ) {: .copyable aria-label='Functions' }
Returns the "amount" of water currently in the room. Generally this will be 0 for no water and 1 for water, but values in between 0 and 1 (and technically above 1 in modded scenarios) are possible.

___
### Get·Water·Color () {: aria-label='Functions' }
#### [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) GetWaterColor ( ) {: .copyable aria-label='Functions' }

___
### Remove·Grid·Entity·Immediate () {: aria-label='Functions' }
#### void RemoveGridEntityImmediate ( int GridIndex, int PathTrail, boolean KeepDecoration ) {: .copyable aria-label='Functions' }
*Immediately* removes the GridEntity at the given index. This means grids can be properly removed and immediately replaced, *without* the need to call Room:Update.

___
### Set·Water·Amount () {: aria-label='Functions' }
#### void GetWaterAmount ( float Amount ) {: .copyable aria-label='Functions' }
Sets the amount of water that should be in the current room. Vanilla typically uses 0 for no water and 1 for water. You can technically go higher than this for some interesting results.

___
### Set·Water·Color () {: aria-label='Functions' }
#### void SetWaterColor ( [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) WaterColor ) {: .copyable aria-label='Functions' }

___
### Set·Water·Current () {: aria-label='Functions' }
#### void SetWaterCurrent ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) WaterCurrent ) {: .copyable aria-label='Functions' }
Sets the strength of the water current in the room. Current sounds and particles are automatically handled.

The game typically uses `1` or `-1` for its values depending on current strength and direction. You can technically go higher than this for some interesting results. Arbitrary directions are fully supported as well.

`Vector(0, 0)` will remove the current.
___