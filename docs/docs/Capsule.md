---
tags:
  - Class
---
# Class "Capsule"

An example mod using the Capsule class can be found [here.](./examples/Capsules.md)

???+ info
    You can get this class by using the following functions:

    * [Entity:GetCollisionCapsule()](Entity.md#getcollisioncapsule)
    * [Entity:GetNullCapsule()](Entity.md#getnullcapsule)
        
## Constructors
### Capsule () {: aria-label='Constructors' }
#### [Capsule](Capsule.md) Capsule ( [Vector](Vector.md) Position, [Vector](Vector.md) SizeMult, float Rotation, float Size ) {: .copyable aria-label='Constructors' }
#### [Capsule](Capsule.md) Capsule ( [Vector](Vector.md) Position, [Vector](Vector.md) TargetPosition, float Size ) {: .copyable aria-label='Constructors' }

## Functions

### Collide () {: aria-label='Functions' }
#### boolean Collide ( [Capsule](Capsule.md) Capsule, [Vector](Vector.md) Point ) {: .copyable aria-label='Functions' }

___
### GetDirection () {: aria-label='Functions' }
#### [Vector](Vector.md) GetDirection ( ) {: .copyable aria-label='Functions' }

___
### GetF1 () {: aria-label='Functions' }
#### float GetF1 ( ) {: .copyable aria-label='Functions' }
Returns the size of the capsule *(consistent with `size` in both constructors)*

___
### GetF2 () {: aria-label='Functions' }
#### float GetF2 ( ) {: .copyable aria-label='Functions' }

___
### GetPosition () {: aria-label='Functions' }
#### [Vector](Vector.md) GetPosition ( ) {: .copyable aria-label='Functions' }

___
### GetVec2 () {: aria-label='Functions' }
#### [Vector](Vector.md) GetVec2 ( ) {: .copyable aria-label='Functions' }
Returns the starting position of the capsule *(can be set with `position`)*

___
### GetVec3 () {: aria-label='Functions' }
#### [Vector](Vector.md) GetVec3 ( ) {: .copyable aria-label='Functions' }
Returns the ending position of the capsule *(can be set with `targetposition`)*

___
