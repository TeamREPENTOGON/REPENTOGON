---
tags:
  - Class
---
# Class "EntityTear"

## Class Diagram
--8<-- "docs/snippets/EntityClassDiagram_NewFunkyMode.md"
## Modified Functions

### ResetSpriteScale () {: aria-label='Modified Functions' }
#### void ResetSpriteScale ( boolean Force = false ) {: .copyable aria-label='Modified Functions' }
Now accepts a `Force` argument to force the tear into re-evaluating what tear scale animation it should play.

___
## Functions

### GetDeadEyeIntensity () {: aria-label='Functions' }
#### float GetDeadEyeIntensity ( ) {: .copyable aria-label='Functions' }
Returns the intensity value of the tear as a result of the Dead Eye collectible. It is between `0` and `1`.

___
### GetDeadEyeSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetDeadEyeSprite ( ) {: .copyable aria-label='Functions' }
Returns the red aura sprite used by the Dead Eye collectible.

___
### GetTearEffectSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetTearEffectSprite ( ) {: .copyable aria-label='Functions' }
Returns the tear effect sprite used by tear variants like Fire Mind and Mysterious Liquid.

___
### GetTearHaloSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetTearHaloSprite ( ) {: .copyable aria-label='Functions' }
Returns the tear halo sprite used by Godhead tears.

___
### MakeMultidimensionalCopy () {: aria-label='Functions' }
#### [EntityTear](EntityTear.md) MakeMultidimensionalCopy ( ) {: .copyable aria-label='Functions' }
Copies the tear and applies a black and white effect to it. This effect is the same one used by the Multidimensional Baby familiar.

___