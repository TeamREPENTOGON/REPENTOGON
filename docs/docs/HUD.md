---
tags:
  - Class
---
# Class "HUD"

???+ info
    You can get this class by using the following functions:

    * [Game:GetHUD()](https://wofsauge.github.io/IsaacDocs/rep/Game.html#gethud)
    
    ???+ example "Example Code"
        ```lua
        local hud = Game():GetHUD()
        local sprite = hud:GetChargeBarSprite()
        ```

## Functions

### FlashRedHearts () {: aria-label='Functions' }
#### void FlashRedHearts ( [EntityPlayer](EntityPlayer.md) Player ) {: .copyable aria-label='Functions' }

___
### GetBossHPBarFill () {: aria-label='Functions' }
#### float GetBossHPBarFill ( ) {: .copyable aria-label='Functions' }
Get the fill of the boss hp bar
___
### GetCardsPillsSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetCardsPillsSprite ( ) {: .copyable aria-label='Functions' }
Sprite object used to render pills, cards and rune sprites in the hud.
___
### GetChargeBarSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetChargeBarSprite ( ) {: .copyable aria-label='Functions' }

___
### GetCoopMenuSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetCoopMenuSprite ( ) {: .copyable aria-label='Functions' }
Sprite object used to render the coop player selection menu.
___
### GetCraftingSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetCraftingSprite ( ) {: .copyable aria-label='Functions' }
Sprite object used for bag of crafting hud.
___
### GetFortuneSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetFortuneSprite ( ) {: .copyable aria-label='Functions' }
Sprite object used for the fortune popup window.
___
### GetHeartsSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetHeartsSprite ( ) {: .copyable aria-label='Functions' }

___
### GetInventorySprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetInventorySprite ( ) {: .copyable aria-label='Functions' }
Sprite object used for Tainted Isaac inventory system.
___
### GetPickupsHUDSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetPickupsHUDSprite ( ) {: .copyable aria-label='Functions' }

___
### GetPlayerHUD () {: aria-label='Functions' }
#### [PlayerHUD](PlayerHUD.md) GetPlayerHUD ( int Index = 0 ) {: .copyable aria-label='Functions' }

___
### GetPoopSpellSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetPoopSpellSprite ( ) {: .copyable aria-label='Functions' }
Tainted blue baby poop spell sprite
___
### GetStreakSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetStreakSprite ( ) {: .copyable aria-label='Functions' }
Sprite object used for text streak popups. For example: picking up items, displaying floor name, etc.
___
### IsVisible () {: aria-label='Functions' }
#### boolean IsVisible ( ) {: .copyable aria-label='Functions' }
Is hud visible
___
### SetBossHPBarFill () {: aria-label='Functions' }
#### void SetBossHPBarFill ( float percent ) {: .copyable aria-label='Functions' }
Sets the fill of the boss hp bar. Accepts values between 0 and 1. Numbers below 0 cause the boss hp bar to not be rendered.
___
### SetIsVisible () {: aria-label='Functions' }
#### void SetIsVisible ( float isVisible ) {: .copyable aria-label='Functions' }  
Set hud visible /invisible.
___