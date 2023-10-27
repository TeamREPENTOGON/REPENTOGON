# Class "WeightedOutcomePicker"

???+ info
    You can get this class by using the following functions:

    * WeightedOutcomePicker()
    
## Functions

### Add·Outcome·Weight () {: aria-label='Functions' }
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

### Pick·Outcome () {: aria-label='Functions' }
#### int PickOutcome ( RNG RNG ) {: .copyable aria-label='Functions' }
Returns a random outcome from the list in WeightedOutcomePicker. Accepts [RNG](https://wofsauge.github.io/IsaacDocs/rep/RNG.html).

___