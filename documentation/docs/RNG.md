---
tags:
  - Class
---
# Class "RNG"

## Modified Functions

### RandomInt () {: aria-label='Modified Functions' }
#### int RandomInt ( int min, int max ) {: .copyable aria-label='Modified Functions' }
Can now emulate `math.random` by accepting a second argument to generate a number between the first argument and it, inclusive. Negative values are usable in this mode and will properly generate a number between `min` and `max` regardless of sign.

___

### SetSeed () {: aria-label='Modified Functions' }
#### void SetSeed ( int Seed, int ShiftIdx = 35 ) {: .copyable aria-label='Modified Functions' }
An error is now thrown if Seed is below 0.

An error is now thrown if ShiftIdx is not between 0 and 80, inclusive.

ShiftIdx is now optional, with the default value set to 35.

___
## Functions 


### PhantomFloat () {: aria-label='Functions' }
#### float PhantomFloat ( ) {: .copyable aria-label='Functions' }
Generates a random float between 0 (inclusive) and 1 (exclusive).

This does not advance the internal state of the RNG object.

___
### PhantomInt () {: aria-label='Functions' }
#### int PhantomInt ( int Max ) {: .copyable aria-label='Functions' }
Behaves identically to RandomInt without advancing the internal state of the RNG object.

___
### PhantomVector () {: aria-label='Functions' }
#### [Vector](Vector.md) PhantomVector ( ) {: .copyable aria-label='Functions' }
Returns a random vector with length `1`. Multiply this vector by a number for larger random vectors.

This does not advance the internal state of the RNG object.

___
### RandomVector () {: aria-label='Functions' }
#### [Vector](Vector.md) RandomVector ( ) {: .copyable aria-label='Functions' }
Returns a random vector with length `1`. Multiply this vector by a number for larger random vectors.

___
