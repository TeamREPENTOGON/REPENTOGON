# Class "RNG"


## Modified Functions

### Set·Seed () {: aria-label='Modified Functions' }
#### void SetSeed ( int Seed, int ShiftIdx = 35 ) {: .copyable aria-label='Modified Functions' }
An error is now thrown if Seed is below 0.

An error is now thrown if ShiftIdx is not between 0 and 80, inclusive.

ShiftIdx is now optional, with the default value set to 35.

___
