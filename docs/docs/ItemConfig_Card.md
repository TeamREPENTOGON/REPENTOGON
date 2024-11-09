---
tags:
  - Class
---
# Class "ItemConfigCard"

## Functions

### SetAvailabilityCondition
#### void SetAvailabilityCondition ( function AvailabilityCondition ) {: .copyable aria-label='Modified Functions' }
Sets an additional function that runs when [IsAvailable()](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Card.html#isavailable) is called, internally or through the Lua API.

The function must return a boolean that determines whether or not the card is available.

This is function is only checked after the GreedModeAllowed, Achievement and Hidden check have all being passed, as such this cannot be used to overwrite those evaluations 

???+ warning "Function Errors"
    If at any point the function errors whilst being executed, then it will be treated the same as if `true` was returned.

    If this is not your intended behavior then it is suggested to wrap your actual function around a `pcall` or `xpcall` and return false if they fail.


    ```lua
    local function AvailabilityCondition()
        -- Your code here
    end


    local function AvailabilityWrapper()
        -- Call function with pcall to catch errors
        local success, result = pcall(AvailabilityCondition)

        if success then
            return result  -- Return result if no error
        else
            local errorMessage = 'Error whilst checking Availability of card "Your Card": ' .. result
            Console.PrintError(errorMessage) -- Print the error message
            Isaac.DebugString(errorMessage) -- Log the error message
            return false
        end
    end

    Isaac.GetItemConfig():GetCard(YourCardId):SetAvailabilityCondition(AvailabilityWrapper)
    ```
___
### ClearAvailabilityCondition
#### void ClearAvailabilityCondition ( ) {: .copyable aria-label='Modified Functions' }
Sets the availability condition to `nil`, useful if the condition is no longer needed and increase performance.
___
### GetAvailabilityCondition
#### function GetAvailabilityCondition ( ) {: .copyable aria-label='Modified Functions' }
Returns `nil` if no AvailabilityCondition is set or if it has been cleared.
___
## Variables

### Hidden {: aria-label='Variables' }
#### const boolean Hidden {: .copyable aria-label='Variables' }
___
### InitialWeight {: aria-label='Variables' }
#### const float InitialWeight {: .copyable aria-label='Variables' }
___
### ModdedCardFront {: aria-label='Variables' }
#### [Sprite](Sprite.md) ModdedCardFront {: .copyable aria-label='Variables' }
___
### Weight {: aria-label='Variables' }
#### float Weight {: .copyable aria-label='Variables' }
Can be set to a value to increase or decrease the chance of this card being randomly picked
___