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

### Get·Bestiary·Menu·Sprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetBestiaryMenuSprite ( ) {: .copyable aria-label='Functions' }
Paper sprite and all other decoration.
___
### Get·Death·Screen·Sprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetDeathScreenSprite ( ) {: .copyable aria-label='Functions' }
Selectable elements that show the DeathScreen sprite of the enemies.
___
### Get·Enemy·Sprite () {: aria-label='Functions' }
#### [Sprite](../Sprite.md) GetEnemySprite ( ) {: .copyable aria-label='Functions' }
Used for the animated enemy preview.
___
### Get·Selected·Page () {: aria-label='Functions' }
#### int GetSelectedPage ( ) {: .copyable aria-label='Functions' }

___
### Get·Last·Enemy·Page·I·D () {: aria-label='Functions' }
#### int GetLastEnemyPageID ( ) {: .copyable aria-label='Functions' }
Gets the id of the last page enemies are displayed. Pages afterwards are reserved for bosses.
___
### Get·Num·Boss·Pages () {: aria-label='Functions' }
#### int GetNumBossPages ( ) {: .copyable aria-label='Functions' }

___
### Get·Num·Monster·Pages () {: aria-label='Functions' }
#### int GetNumMonsterPages ( ) {: .copyable aria-label='Functions' }

___
### Get·Num·Pages () {: aria-label='Functions' }
#### int GetNumPages ( ) {: .copyable aria-label='Functions' }

___
### Get·Selected·Element () {: aria-label='Functions' }
#### int GetSelectedElement ( ) {: .copyable aria-label='Functions' }

___
### Set·Selected·Page () {: aria-label='Functions' }
#### void SetSelectedPage ( int page ) {: .copyable aria-label='Functions' }

___
### Set·Selected·Element () {: aria-label='Functions' }
#### void SetSelectedElement ( int element ) {: .copyable aria-label='Functions' }

___