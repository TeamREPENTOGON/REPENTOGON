---
tags:
  - Class
---
# Class "WeightedOutcomePicker"

An example mod using the WeightedOutcomePicker class can be found [here.](./examples/WeightedOutcomes.md)

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

### AddOutcomeFloat () {: aria-label='Functions' }
#### void AddOutcomeFloat ( int Value, float Weight, int ScaleFactor = 100 ) {: .copyable aria-label='Functions' }
Adds an outcome to the outcome selector with the specified `Weight`. The internal weight is still an integer calculated like this: `fWeight * scaleFactor`, where `ScaleFactor` is the maximum weight (equivalent to 1.0).

???+ example "Example Code"
    ```lua
    local picker = WeightedOutcomePicker()

    picker:AddOutcomeFloat(1, 1.0) -- ~45%
    picker:AddOutcomeFloat(2, 1.0) -- ~45%
    picker:AddOutcomeFloat(3, 0.2) -- ~9%
    ```

___

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

### ClearOutcomes () {: aria-label='Functions' }
#### void ClearOutcomes ( ) {: .copyable aria-label='Functions' }
Clears all outcomes from the outcome picker.

___

### GetNumOutcomes () {: aria-label='Functions' }
#### int GetNumOutcomes ( ) {: .copyable aria-label='Functions' }
Returns the number of outcomes in the outcome picker.

___

### GetOutcomes () {: aria-label='Functions' }
#### table[] GetOutcomes ( ) {: .copyable aria-label='Functions' }
Returns a table containing a list of all outcomes in the outcome picker.

???- info "Table structure & usage"
    - The returned table contains a list of outcomes, where each outcome is a table containing the following fields: 
        * Value: value of the outcome
        * Weight: weight of the outcome
            ```lua
            for i, outcome in ipairs(p:GetOutcomes()) do
                print(outcome.Value, outcome.Weight)
            end
            ```

___

### PickOutcome () {: aria-label='Functions' }
#### int PickOutcome ( [RNG](RNG.md) RNG ) {: .copyable aria-label='Functions' }
Returns a random outcome from the list in WeightedOutcomePicker. Accepts [RNG](RNG.md).

___

### RemoveOutcome () {: aria-label='Functions' }
#### void RemoveOutcome ( int Value ) {: .copyable aria-label='Functions' }
Removes an outcome from the outcome picker with the given `Value`.

___