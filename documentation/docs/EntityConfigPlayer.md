---
tags:
  - Class
---
# Class "EntityConfigPlayer"

???+ info
    You can get this class by using the following functions:

    * [EntityConfig.GetPlayer()](EntityConfig.md#getplayer)

    ???+ example "Example Code"
        ```lua
        local cainConfig = EntityConfig.GetPlayer(PlayerType.PLAYER_CAIN)
        ```
        
## Functions

### Can·Shoot () {: aria-label='Functions' }
#### boolean CanShoot ( ) {: .copyable aria-label='Functions' }

___
### Get·Achievement·ID () {: aria-label='Functions' }
#### [Achievement](enums/Achievement.md) GetAchievementID ( ) {: .copyable aria-label='Functions' }
Returns -1 if the character is not locked behind a vanilla achievement (or -2 for "hidden" vanilla characters).

___
### Get·Birthright·Description () {: aria-label='Functions' }
#### string GetBirthrightDescription ( ) {: .copyable aria-label='Functions' }

___
### Get·Black·Hearts () {: aria-label='Functions' }
#### int GetBlackHearts ( ) {: .copyable aria-label='Functions' }

___
### Get·Bombs () {: aria-label='Functions' }
#### int GetBombs ( ) {: .copyable aria-label='Functions' }

___
### Get·Broken·Hearts () {: aria-label='Functions' }
#### int GetBrokenHearts ( ) {: .copyable aria-label='Functions' }

___
### Get·Card () {: aria-label='Functions' }
#### [Card](https://wofsauge.github.io/IsaacDocs/rep/enums/Card.html) GetCard ( ) {: .copyable aria-label='Functions' }
Returns 0 if the character does not start with any vanilla card.

Does not include starting cards obtained via unlocks.

Does not include cards added by mods.

___
### Get·Coins () {: aria-label='Functions' }
#### int GetCoins ( ) {: .copyable aria-label='Functions' }

___
### Get·Collectibles () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)[] GetCollectibles ( ) {: .copyable aria-label='Functions' }
Returns a table of [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html)s for the character's starting items.

___
### Get·Costume·ID () {: aria-label='Functions' }
#### int GetCostumeID ( ) {: .copyable aria-label='Functions' }
Returns -1 if the character does not have any XML-defined starting costume (like Maggy's hair).

___
### Get·Costume·Suffix () {: aria-label='Functions' }
#### string GetCostumeSuffix ( ) {: .copyable aria-label='Functions' }
Directory suffix used for character-specific costume sprites.

___
### Get·Extra·Portrait·Path () {: aria-label='Functions' }
#### string GetExtraPortraitPath ( ) {: .copyable aria-label='Functions' }
Path to a `.anm2` file displayed on top of the character's level transition and boss VS screen portrait.

___
### Get·Keys () {: aria-label='Functions' }
#### int GetKeys ( ) {: .copyable aria-label='Functions' }

___
### Get·Modded·Controls·Sprite () {: aria-label='Functions' }
#### [Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) GetModdedControlsSprite ( ) {: .copyable aria-label='Functions' }
Returns the Sprite used for a modded character's starting room controls.

Note that this Sprite is shared by other characters from the same mod - there is an animation with the same name as this character.

Returns nil for vanilla characters, or characters with no corresponding animation.

___
### Get·Modded·Coop·Menu·Sprite () {: aria-label='Functions' }
#### [Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) GetModdedCoopMenuSprite ( ) {: .copyable aria-label='Functions' }
Returns the Sprite used for a modded character's icon in the co-op character select wheel.

Note that this Sprite is shared by other characters from the same mod - there is an animation with the same name as this character.

Returns nil for vanilla characters, or characters with no corresponding animation.

___
### Get·Modded·Game·Over·Sprite () {: aria-label='Functions' }
#### [Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) GetModdedGameOverSprite ( ) {: .copyable aria-label='Functions' }
Returns the Sprite used for a modded character's game over screen (ie, their name).

Note that this Sprite is shared by other characters from the same mod - there is an animation with the same name as this character.

Returns nil for vanilla characters, or characters with no corresponding animation.

___
### Get·Modded·Menu·Background·Sprite () {: aria-label='Functions' }
#### [Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) GetModdedMenuBackgroundSprite ( ) {: .copyable aria-label='Functions' }
Returns the Sprite used for a modded character's character select screen.

Note that this Sprite is shared by other characters from the same mod - there is an animation with the same name as this character.

Returns nil for vanilla characters, or characters with no corresponding animation.

___
### Get·Modded·Menu·Portrait·Sprite () {: aria-label='Functions' }
#### [Sprite](https://wofsauge.github.io/IsaacDocs/rep/Sprite.html) GetModdedMenuPortraitSprite ( ) {: .copyable aria-label='Functions' }
Returns the Sprite used for a modded character's character select portrait.

Note that this Sprite is shared by other characters from the same mod - there is an animation with the same name as this character.

Returns nil for vanilla characters, or characters with no corresponding animation.

___
### Get·Name () {: aria-label='Functions' }
#### string GetName ( ) {: .copyable aria-label='Functions' }

___
### Get·Name·Image·Path () {: aria-label='Functions' }
#### string GetNameImagePath ( ) {: .copyable aria-label='Functions' }
Path to the PNG file used for the character's name on the boss VS screen.

___
### Get·Pill () {: aria-label='Functions' }
#### [PillColor](https://wofsauge.github.io/IsaacDocs/rep/enums/PillColor.html) GetPill ( ) {: .copyable aria-label='Functions' }
Does not include starting pills obtained via unlocks.

___
### Get·Player·Type () {: aria-label='Functions' }
#### int GetPlayerType ( ) {: .copyable aria-label='Functions' }

___
### Get·Pocket·Active () {: aria-label='Functions' }
#### [CollectibleType](https://wofsauge.github.io/IsaacDocs/rep/enums/CollectibleType.html) GetPocketActive ( ) {: .copyable aria-label='Functions' }
Does not include items added by mods.

___
### Get·Portrait·Path () {: aria-label='Functions' }
#### string GetPortraitPath ( ) {: .copyable aria-label='Functions' }
Path to the PNG file used for the character's main level transition and boss VS screen portrait.

___
### Get·Red·Hearts () {: aria-label='Functions' }
#### int GetRedHearts ( ) {: .copyable aria-label='Functions' }

___
### Get·Skin·Color () {: aria-label='Functions' }
#### [SkinColor](https://wofsauge.github.io/IsaacDocs/rep/enums/SkinColor.html) GetSkinColor ( ) {: .copyable aria-label='Functions' }

___
### Get·Skin·Path () {: aria-label='Functions' }
#### string GetSkinPath ( ) {: .copyable aria-label='Functions' }
Path to the PNG file used for the character's primary sprite sheet.

___
### Get·Soul·Hearts () {: aria-label='Functions' }
#### int GetSoulHearts ( ) {: .copyable aria-label='Functions' }

___
### Get·Tainted·Counterpart () {: aria-label='Functions' }
#### [EntityConfigPlayer](EntityConfigPlayer.md) GetTaintedCounterpart ( ) {: .copyable aria-label='Functions' }
For non-tainted characters, returns their tainted counterpart, or returns nil if there is none.

For tainted characters, returns their non-tainted counterpart.

___
### Get·Trinket () {: aria-label='Functions' }
#### [TrinketType](https://wofsauge.github.io/IsaacDocs/rep/enums/TrinketType.html) GetTrinket ( ) {: .copyable aria-label='Functions' }
Does not include starting trinkets obtained via unlocks.

Does not include trinkets added by mods.

___
### Is·Hidden () {: aria-label='Functions' }
#### boolean IsHidden ( ) {: .copyable aria-label='Functions' }
Returns true if the character is not visible/selectable from the character select screen.

Doesn't include characters who are only hidden until unlocked.

___
### Is·Tainted () {: aria-label='Functions' }
#### boolean IsTainted ( ) {: .copyable aria-label='Functions' }

___

