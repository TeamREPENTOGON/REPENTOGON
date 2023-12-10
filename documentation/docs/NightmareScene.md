---
tags:
  - Global
  - Class
---
# Global Class "NightmareScene"

???+ info
    You can get this class by using the `NightmareScene` global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local sprite = NightmareScene.GetBubbleSprite()
        ```

???+ warning "Warning"
    This class' functions should not be called until the game fully initializes! Make sure not to try using them outside of callbacks
    
## Functions

### Get·Background·Sprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetBackgroundSprite ( ) {: .copyable aria-label='Functions' }

___
### Get·Bubble·Sprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetBubbleSprite ( ) {: .copyable aria-label='Functions' }

___
### Get·Progress·Bar·Map () {: aria-label='Functions' }
#### int[] GetProgressBarMap ( ) {: .copyable aria-label='Functions' }

___
### Get·Progress·Bar·Sprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetProgressBarSprite ( ) {: .copyable aria-label='Functions' }

___
### Is·Dogma·Nightmare () {: aria-label='Functions' }
#### boolean IsDogmaNightmare ( ) {: .copyable aria-label='Functions' }

___