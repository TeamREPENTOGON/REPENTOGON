# Class "RNG"

## Functions 

### Phantom·Int () {: aria-label='Functions' }
#### int PhantomInt ( int Max ) {: .copyable aria-label='Functions' }
Generates a random integer between 0 (inclusive) and `Max` (exclusive). 

This does not advance the internal state of the RNG object.

___

### Phantom·Float () {: aria-label='Functions' }
#### float PhantomFloat () {: .copyable aria-label='Functions' }
Generates a random float between 0 (inclusive) and 1 (exclusive).

This does not adcance the internal state of the RNG object

___

## Modified Functions

### Set·Seed () {: aria-label='Modified Functions' }
#### void SetSeed ( int Seed, int ShiftIdx = 35 ) {: .copyable aria-label='Modified Functions' }
An error is now thrown if Seed is below 0.

An error is now thrown if ShiftIdx is not between 0 and 80, inclusive.

ShiftIdx is now optional, with the default value set to 35.

___
