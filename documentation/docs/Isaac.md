# Class "Isaac"

## Functions

### Get·Persistent·Game·Data () {: aria-label='Functions' }
#### [PersistentGameData](PersistentGameData.md) GetPersistentGameData ( ) {: .copyable aria-label='Functions' }
Should probably be moved to Game for consistency.

### World·To·Menu·Position () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) WorldToMenuPosition ( [MainMenu](enums/MainMenu.md) MenuId, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position ) {: .copyable aria-label='Functions' }
Converts the World position from input to a pinned main menu position that varies depending on the enum selected. It's important to reconvert this every frame, in a similar fashion to WorldToRender, in order to properly render when menus are changed or the window is resized.

### Get·String () {: aria-label='Functions' }
#### string GetString ( string category, string key) {: .copyable aria-label='Functions' }
Returns the translation string associated with the given key in the given category. The translation is given in the currently selected language.

### Get·Localized·String () {: aria-label='Functions' }
#### string GetLocalizedString ( string category, string key, int language ) {: .copyable aria-label='Functions' }
Returns the translation string associated with the given key in the given category. The translation is given in the language ID given as parameter.
___
## Variables

### Level·Generator·Entry {: aria-label='Variables' }
#### [LevelGeneratorEntry](LevelGeneratorEntry.md) LevelGeneratorEntry  {: .copyable aria-label='Variables' }
Fetches a blank LevelGeneratorEntry from the game.

___
