---
tags:
  - Global
  - Class
---
# Global Class "WeightedOutcomePicker"

???+ info
    You can get this class by using the `WeightedOutcomePicker` global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local sprite = WeightedOutcomePicker.AddOutcomeWeight(1, 65)
        ```
    
## Functions

### AddOutcomeWeight () {: aria-label='Functions' }
#### void AddOutcomeWeight ( int Value, int Weight ) {: .copyable aria-label='Functions' }
Adds an outcome to the outcome selector with the specified `Weight`.

???+ example "Example Code"
    ```lua
    local picker = WeightedOutcomePicker()

    picker:AddOutcomeWeight(1, 65) -- 65%
    picker:AddOutcomeWeight(2, 30) -- 30%
    picker:AddOutcomeWeight(3, 5) -- 5%
    ```

___

### PickOutcome () {: aria-label='Functions' }
#### int PickOutcome ( [RNG](https://wofsauge.github.io/IsaacDocs/rep/RNG.html) RNG ) {: .copyable aria-label='Functions' }
Returns a random outcome from the list in WeightedOutcomePicker. Accepts [RNG](https://wofsauge.github.io/IsaacDocs/rep/RNG.html).

___