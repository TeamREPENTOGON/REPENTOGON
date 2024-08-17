---
tags:
  - Class
---
# Class "RNG"

## Modified Constructors

### RNG () {: aria-label='Modified Constructors' }
#### [RNG](RNG.md) RNG ( int seed = 2853650767, int shiftIdx = 35 ) { :.copyable aria-label='Modified Constructors' }
Now takes an optional seed and an optional shiftIdx.
This can be used to avoid separating construction of the RNG object and a call to SetSeed.

The function validates both the seed and the shift index.

___
## Modified Functions

### RandomInt () {: aria-label='Modified Functions' }
#### int RandomInt ( int Min, int Max ) {: .copyable aria-label='Modified Functions' }
Can now emulate `math.random` by accepting a second argument to generate a number between the first argument and it, inclusive. Negative values are usable in this mode and will properly generate a number between `min` and `max` regardless of sign.

___
### SetSeed () {: aria-label='Modified Functions' }
#### void SetSeed ( int Seed, int ShiftIdx = 35 ) {: .copyable aria-label='Modified Functions' }
An error is now thrown if Seed is below 0.

An error is now thrown if ShiftIdx is not between 0 and 80, inclusive.

ShiftIdx is now optional, with the default value set to 35.

___
## Functions 

### GetShiftIdx () {: aria-label='Functions' }
#### int GetShiftIdx ( ) {: .copyable aria-label='Functions' }

___
### PhantomFloat () {: aria-label='Functions' }
#### float PhantomFloat ( ) {: .copyable aria-label='Functions' }
Generates a random float between `0` (inclusive) and `1` (exclusive).

This does not advance the internal state of the RNG object.

___
### PhantomInt () {: aria-label='Functions' }
#### int PhantomInt ( int Max ) {: .copyable aria-label='Functions' }
Behaves identically to RandomInt without advancing the internal state of the RNG object.

___
### PhantomNext () {: aria-label='Functions' }
#### int PhantomNext ( ) {: .copyable aria-label='Functions' }

___
### PhantomPrevious () {: aria-label='Functions' }
#### int PhantomPrevious ( ) {: .copyable aria-label='Functions' }

___
### PhantomVector () {: aria-label='Functions' }
#### [Vector](Vector.md) PhantomVector ( ) {: .copyable aria-label='Functions' }
Returns a random vector with length `1`. Multiply this vector by a number for larger random vectors.

This does not advance the internal state of the RNG object.

___
### Previous () {: aria-label='Functions' }
#### int Previous ( ) {: .copyable aria-label='Functions' }

___
### RandomVector () {: aria-label='Functions' }
#### [Vector](Vector.md) RandomVector ( ) {: .copyable aria-label='Functions' }
Returns a random vector with length `1`. Multiply this vector by a number for larger random vectors.

___
