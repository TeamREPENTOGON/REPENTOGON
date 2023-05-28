# Class "StageTransition"

This class gives access to data exclusive to the stage transition screen.

Please note that the `StageTransition` is only used to configure how the stage transition plays. If you want to manipulate the content of the screen during a stage transition, you need to use the NightmareScene class.

## Functions 

### Get·Same·Stage () {: aria-label='Functions' }
#### boolean GetSameStage ( ) {: .copyable aria-label='Functions' }

Indicate if the stage transition screen will display Isaac's head moving from one stage to the other (`false`) or not (`true`).
___

### Set·Same·Stage () {: aria-label='Functions' }
#### void SetSameStage ( boolean on ) {: .copyable aria-label='Functions' }
Configure whether the stage transition will display Isaac's head moving from one stage to the other (`false`) or not (`true`).

This function is useful if you want to move the player to the first stage, or want to repeat the last stage on the progress bar of the transition screen, and have it be less jarring. 

* If transitioning back to the first floor, and `SameStage` is not set to `true`, Isaac's head will appear outside of the progress bar. Otherwise, Isaac's head will appear on the first floor.
* If repeating the last floor, and `SameStage` is not set to `true`, Isaac's head will move from the previous stage to the last one. Otherwise, Isaac's head will appear on the last floor.

???+ warning "Warning"
    Calling this method before the current stage transition has called `SetNextStage` will override the transition itself. This means that instead of merely displaying Isaac's head not moving, it will actually change whether the next stage will be a repeat of the current one, or the actual next stage. Ideally, you should use this function in the context of the `ModCallbacks.MC_PRE_LEVEL_SELECT` callback.
___
