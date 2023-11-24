# Class "RNG"

## Modified Functions

### Random·Int () {: aria-label='Modified Functions' }
#### int RandomInt ( int min, int max ) {: .copyable aria-label='Modified Functions' }
Can now emulate `math.random` by accepting a second argument to generate a number between the first argument and it, inclusive. Negative values are usable in this mode and will properly generate a number between `min` and `max` regardless of sign.

___

### Set·Seed () {: aria-label='Modified Functions' }
#### void SetSeed ( int Seed, int ShiftIdx = 35 ) {: .copyable aria-label='Modified Functions' }
An error is now thrown if Seed is below 0.

An error is now thrown if ShiftIdx is not between 0 and 80, inclusive.

ShiftIdx is now optional, with the default value set to 35.

___
## Functions 


### Phantom·Float () {: aria-label='Functions' }
#### float PhantomFloat ( ) {: .copyable aria-label='Functions' }
Generates a random float between 0 (inclusive) and 1 (exclusive).

This does not advance the internal state of the RNG object.

___
### Phantom·Int () {: aria-label='Functions' }
#### int PhantomInt ( int Max ) {: .copyable aria-label='Functions' }
Behaves identically to RandomInt without advancing the internal state of the RNG object.

___
### Phantom·Vector () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) PhantomVector ( ) {: .copyable aria-label='Functions' }
Returns a random vector with length `1`. Multiply this vector by a number for larger random vectors.

This does not advance the internal state of the RNG object.

___
### Random·Vector () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) RandomVector ( ) {: .copyable aria-label='Functions' }
Returns a random vector with length `1`. Multiply this vector by a number for larger random vectors.

___
