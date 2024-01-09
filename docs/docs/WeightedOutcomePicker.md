---
tags:
  - Class
---
# Class "WeightedOutcomePicker"

???+ info
    This class can be obtained using its constructor:
    ???+ example "Example Code"
        ```lua
        local wop = WeightedOutcomePicker()
        ```

## Constructors

### WeightedOutcomePicker () {: aria-label='Constructors' }
#### [WeightedOutcomePicker](WeightedOutcomePicker.md) WeightedOutcomePicker ( ) {: .copyable aria-label='Constructors' }
___
    
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

### RemoveOutcome () {: aria-label='Functions' }
#### void RemoveOutcome ( int Value ) {: .copyable aria-label='Functions' }
Removes an outcome from the outcome picker with the given `Value`.

___

### GetOutcomes () {: aria-label='Functions' }
#### table[] GetOutcomes ( ) {: .copyable aria-label='Functions' }
Returns a table containing a list of all outcomes in the picker.

??- info "Table structure & usage"
    - The returned table contains a list of outcomes, where each outcome is a table containing the following fields: 
        * Value: value of the outcome
        * Weight: weight of the outcome
            ```lua
            for i, outcome in ipairs(p:GetOutcomes()) do
                print(outcome.Value, outcome.Weight)
            end
            ```
___
