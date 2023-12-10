---
tags:
  - Global
  - Class
---
# Global Class "MenuManager"

???+ info
    You can get this class by using the `MenuManager` global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local sprite = MenuManager.GetBackWidgetSprite()
        ```

## Functions

### Get·Active·Menu () {: aria-label='Functions' }
#### [MainMenuType](../enums/MainMenuType.md) GetActiveMenu ( ) {: .copyable aria-label='Functions' }
Returns the MainMenuType of the currently active (visible) section of the main menu.

___
### Get·Back·Widget·Sprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetBackWidgetSprite ( ) {: .copyable aria-label='Functions' }

___
### Get·Color·Modifier·Lerp·Amount () {: aria-label='Functions' }
#### [ColorModifier](../ColorModifier.md) GetColorModifierLerpAmount ( ) {: .copyable aria-label='Functions' }

???+ info "Info"
    This is formatted as the absolute rate of change (ie, all values are positive).

___
### Get·Current·Color·Modifier () {: aria-label='Functions' }
#### [ColorModifier](../ColorModifier.md) GetCurrentColorModifier ( ) {: .copyable aria-label='Functions' }

___
### Get·Select·Widget·Sprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetSelectWidgetSprite ( ) {: .copyable aria-label='Functions' }

___
### Get·Shadow·Sprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetShadowSprite ( ) {: .copyable aria-label='Functions' }
Shadow decoration that looks like isaacs head.

___
### Get·Target·Color·Modifier () {: aria-label='Functions' }
#### [ColorModifier](../ColorModifier.md) GetTargetColorModifier ( ) {: .copyable aria-label='Functions' }

___
### Set·Active·Menu () {: aria-label='Functions' }
#### int SetActiveMenu ([MainMenuType](../enums/MainMenuType.md) menu ) {: .copyable aria-label='Functions' }
Changes the active menu on the main menu to match the input menutype.

___
### Set·Color·Modifier () {: aria-label='Functions' }
#### void SetColorModifier ( [ColorModifier](../ColorModifier.md) colorModifier, boolean lerp = true, float rate = 0.015 ) {: .copyable aria-label='Functions' }

___
