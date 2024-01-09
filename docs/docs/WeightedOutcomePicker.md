---
tags:
  - Class
---
# Class "WeightedOutcomePicker"

An example mod using the WeightedOutcomePicker class can be found [here.](./examples/WeightedOutcomes.md)

???+ info
    This class can be get by using its constructor:
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