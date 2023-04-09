# Class "Isaac"

## Functions

### Create·Timer () {: aria-label='Functions' }
#### [EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/EntityEffect.html) CreateTimer ( function Function, int Delay, int Times, boolean Persistent ) {: .copyable aria-label='Functions' }
Spawns a timer EntityEffect. This entity will run Function function after Delay frames, and will repeat this Times amount of times. Persistent controls whether or not this timer "dies" in the current room, or persists across rooms.

???- info "Timer behavior"
    This timer is called every game update. This means the timer only takes into consideration frames in which the game is actively running, not paused, and uses update frames for its Delay parameter (30 frames per second). 
	
	If your use case requires that a timer takes paused time into account, stick with a custom timer running on a RENDER callback.
___
### Get·Localized·String () {: aria-label='Functions' }
#### string GetLocalizedString ( string category, string key, int language ) {: .copyable aria-label='Functions' }
Returns the translation string associated with the given key in the given category. The translation is given in the language ID given as parameter.

___
### Get·Persistent·Game·Data () {: aria-label='Functions' }
#### [PersistentGameData](PersistentGameData.md) GetPersistentGameData ( ) {: .copyable aria-label='Functions' }
Should probably be moved to Game for consistency.

___
### Get·String () {: aria-label='Functions' }
#### string GetString ( string category, string key) {: .copyable aria-label='Functions' }
Returns the translation string associated with the given key in the given category. The translation is given in the currently selected language.

___

### World·To·Menu·Position () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) WorldToMenuPosition ( [MainMenu](enums/MainMenu.md) MenuId, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position ) {: .copyable aria-label='Functions' }
Converts the World position from input to a pinned main menu position that varies depending on the enum selected. It's important to reconvert this every frame, in a similar fashion to WorldToRender, in order to properly render when menus are changed or the window is resized.

## Variables

### Level·Generator·Entry {: aria-label='Variables' }
#### [LevelGeneratorEntry](LevelGeneratorEntry.md) LevelGeneratorEntry  {: .copyable aria-label='Variables' }
Fetches a blank LevelGeneratorEntry from the game.

___
