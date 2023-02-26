# Class "[MultiShotParams](MultiShotParams.md)"

???+ info
    You can get this class by using the following functions:

    * [EntityPlayer:GetMultiShotParams](EntityPlayer.md#getmultishotparams)

    ???+ example "Example Code"
        ```lua
        local params = Game():GetPlayer(0):GetMultiShotParams()
        ```

**[MultiShotParams](MultiShotParams.md)** contains information the game uses to properly calculate the position and velocity of every tear fired, among other things. 

This class has much more than GetNumTears, but they have not been discovered yet. If you happen to know what they are, this would be of great help for the project (look at IsaacRepentanceStripped.h to see how many params there are)

## Functions

### Get·Num·Tears () {: aria-label='Functions' }
#### int GetNumTears ( ) {: .copyable aria-label='Functions' }
Returns the amount of tears the player can currently simultaneously fire.

___