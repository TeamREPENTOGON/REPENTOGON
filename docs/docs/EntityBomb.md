---
tags:
  - Class
---
# Class "EntityBomb"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Functions

### GetCostumeLayerSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetCostumeLayerSprite ( [BombCostumeLayer](enums/BombCostumeLayer.md) LayerID ) {: .copyable aria-label='Functions' }

___
### GetExplosionCountdown () {: aria-label='Functions' }
#### int GetExplosionCountdown ( ) {: .copyable aria-label='Functions' }

___
### GetFallingSpeed () {: aria-label='Functions' }
#### float GetFallingSpeed ( ) {: .copyable aria-label='Functions' }

___
### GetHeight () {: aria-label='Functions' }
#### float GetHeight ( ) {: .copyable aria-label='Functions' }

___
### GetHitList () {: aria-label='Functions' }
#### int[] GetHitList ( ) {: .copyable aria-label='Functions' }

___
### GetRocketAngle () {: aria-label='Functions' }
#### float GetRocketAngle ( ) {: .copyable aria-label='Functions' }
Target angle for rocket bombs. It influences both their movement and the orientation of their sprite.

___
### GetRocketSpeed () {: aria-label='Functions' }
#### float GetRocketSpeed ( ) {: .copyable aria-label='Functions' }
Target speed for rocket bombs. Naturally increases by 1 every frame.

___
### GetScale () {: aria-label='Functions' }
#### float GetScale ( ) {: .copyable aria-label='Functions' }
Used to apply animation set for bomb costume.

___
### IsLoadingCostumes () {: aria-label='Functions' }
#### boolean IsLoadingCostumes ( ) {: .copyable aria-label='Functions' }

___
### SetFallingSpeed () {: aria-label='Functions' }
#### void SetFallingSpeed ( float Speed ) {: .copyable aria-label='Functions' }

___
### SetHeight () {: aria-label='Functions' }
#### void SetHeight ( float Height ) {: .copyable aria-label='Functions' }

___
### SetLoadCostumes () {: aria-label='Functions' }
#### void SetLoadCostumes ( boolean Load = true ) {: .copyable aria-label='Functions' }

___
### SetRocketAngle () {: aria-label='Functions' }
#### void SetRocketAngle ( float Angle ) {: .copyable aria-label='Functions' }
Set the target angle for a rocket bomb. It influences both their movement and the orientation of their sprite.

___
### SetRocketSpeed () {: aria-label='Functions' }
#### void SetRocketSpeed ( float Speed ) {: .copyable aria-label='Functions' }
Set the target speed for a rocket bomb. Note that this will naturally increase by 1 every frame.

___
### SetScale () {: aria-label='Functions' }
#### void SetScale ( float Scale ) {: .copyable aria-label='Functions' }
Should be used with [SetLoadCostumes](#setloadcostumes) method.

___
### UpdateDirtColor () {: aria-label='Functions' }
#### void UpdateDirtColor ( ) {: .copyable aria-label='Functions' }

___
