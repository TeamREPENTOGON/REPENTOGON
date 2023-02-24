# Class "[MultiShotParams](MultiShotParams.md)"

???+ info
    You can get this class by using the following functions:

    * [Game:GetAmbush](Game.md#getambush)

    ???+ example "Example Code"
        ```lua
        local ambush = Game():GetAmbush()
        ```

**[MultiShotParams](MultiShotParams.md)** contains information the game uses to properly calculate the position and velocity of every tear fired, among other things. 

There are more params than just NumTears exposed, but they have not been discovered yet. If you happen to know what they are, this would be of great help for the project (look at IsaacRepentanceStripped.h to see how many params there are)

## Functions

### Get·Num·Tears () {: aria-label='Functions' }
#### int GetNumTears ( ) {: .copyable aria-label='Functions' }
Returns the amount of tears the player can currently simultaneously fire.

___