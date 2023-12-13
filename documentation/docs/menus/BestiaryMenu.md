---
tags:
  - Global
  - Class
---
# Global Class "BestiaryMenu"

???+ info
    You can get this class by using the `BestiaryMenu` global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local sprite = BestiaryMenu.GetBestiaryMenuSprite()
        ```

## Functions

### GetBestiaryMenuSprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetBestiaryMenuSprite ( ) {: .copyable aria-label='Functions' }
Paper sprite and all other decoration.
___
### GetDeathScreenSprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetDeathScreenSprite ( ) {: .copyable aria-label='Functions' }
Selectable elements that show the DeathScreen sprite of the enemies.
___
### GetEnemySprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetEnemySprite ( ) {: .copyable aria-label='Functions' }
Used for the animated enemy preview.
___
### GetSelectedPage () {: aria-label='Functions' }
#### int GetSelectedPage ( ) {: .copyable aria-label='Functions' }

___
### GetLastEnemyPageID () {: aria-label='Functions' }
#### int GetLastEnemyPageID ( ) {: .copyable aria-label='Functions' }
Gets the id of the last page enemies are displayed. Pages afterwards are reserved for bosses.
___
### GetNumBossPages () {: aria-label='Functions' }
#### int GetNumBossPages ( ) {: .copyable aria-label='Functions' }

___
### GetNumMonsterPages () {: aria-label='Functions' }
#### int GetNumMonsterPages ( ) {: .copyable aria-label='Functions' }

___
### GetNumPages () {: aria-label='Functions' }
#### int GetNumPages ( ) {: .copyable aria-label='Functions' }

___
### GetSelectedElement () {: aria-label='Functions' }
#### int GetSelectedElement ( ) {: .copyable aria-label='Functions' }

___
### SetSelectedPage () {: aria-label='Functions' }
#### void SetSelectedPage ( int page ) {: .copyable aria-label='Functions' }

___
### SetSelectedElement () {: aria-label='Functions' }
#### void SetSelectedElement ( int element ) {: .copyable aria-label='Functions' }

___