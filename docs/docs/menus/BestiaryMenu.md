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
### GetNumBossPages () {: aria-label='Functions' }
#### int GetNumBossPages ( ) {: .copyable aria-label='Functions' }
Returns the number of bestiary pages used for bosses. The boss pages come after the monster pages.

___
### GetNumMonsterPages () {: aria-label='Functions' }
#### int GetNumMonsterPages ( ) {: .copyable aria-label='Functions' }
Returns the number of bestiary pages used for enemies. Enemy pages come first, the pages afterwards are reserved for bosses.

___
### GetNumPages () {: aria-label='Functions' }
#### int GetNumPages ( ) {: .copyable aria-label='Functions' }
Returns the total number of pages currently in the bestiary, with both monster and boss pages included.

___
### GetSelectedElement () {: aria-label='Functions' }
#### int GetSelectedElement ( ) {: .copyable aria-label='Functions' }

___
### SetSelectedPage () {: aria-label='Functions' }
#### void SetSelectedPage ( int Page ) {: .copyable aria-label='Functions' }

___
### SetSelectedElement () {: aria-label='Functions' }
#### void SetSelectedElement ( int Element ) {: .copyable aria-label='Functions' }

___