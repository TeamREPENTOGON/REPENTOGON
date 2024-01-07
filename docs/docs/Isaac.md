---
tags:
  - Global
  - Class
---
# Global Class "Isaac"
## Modified Functions

### FindByType () {: aria-label='Modified Functions' }
#### [Entity](Entity.md)[] FindByType ( [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) Type, int Variant = -1, int SubType = -1, boolean Cache = false, boolean IgnoreFriendly = false ) {: .copyable aria-label='Modified Functions' }
Same as vanilla, but much faster.
___
### FindInRadius () {: aria-label='Modified Functions' }
#### [Entity](Entity.md)[] FindInRadius ( [Vector](Vector.md) Position, float Radius, int Partitions = 0xFFFFFFFF  ) {: .copyable aria-label='Modified Functions' }
Same as in vanilla, but much faster and with fixed search for effects.
___
### GetRoomEntities () {: aria-label='Modified Functions' }
#### [Entity](Entity.md)[] GetRoomEntities ( ) {: .copyable aria-label='Modified Functions' }
Same as vanilla, but much faster.

___
## Functions

### AllMarksFilled () {: aria-label='Functions' }
#### int AllMarksFilled ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) Character) {: .copyable aria-label='Functions' }
Checks if a given character has completed all marks and returns the highest difficulty it was accomplished in if it applies(1-normal/2-hard/0-none). Supports both modded and vanilla characters. 

___
### AllTaintedCompletion () {: aria-label='Functions' }
#### int AllTaintedCompletion ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) Character, [TaintedMarksGroup](enums/TaintedMarksGroup.md) Group) {: .copyable aria-label='Functions' }
Checks if a given character has completed all the tainted unlock-related marks and returns the highest difficulty it was accomplished in if it applies(1-normal/2-hard/0-none). Supports both modded and vanilla characters. 

___
### CanStartTrueCoop () {: aria-label='Functions' }
#### boolean CanStartTrueCoop ( ) {: .copyable aria-label='Functions' }

___
### CenterCursor () {: aria-label='Functions' }
#### void CenterCursor ( ) {: .copyable aria-label='Functions' }
Moves the windows mouse cursor to the center of the game's window. This is incredibly niche but useful if you want to use cursor controls for anything fancy and have full control over it. It wont move the cursor if Isaac.exe becomes out of focus.

???- info "Note"
    Take in mind the screen center is NOT the center of the room necessarily, it's just the center of the game's window(center of the actual screen if you are on fullscreen).

___
### ClearBossHazards () {: aria-label='Functions' }
#### void ClearBossHazards ( boolean IgnoreNPCs = false ) {: .copyable aria-label='Functions' }

Kills all projectiles and, if `IgnoreNPCs` is false, all non-friendly NPCs capable of keeping doors closed. 

___
### ClearCompletionMarks () {: aria-label='Functions' }
#### void ClearCompletionMarks ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) Character) {: .copyable aria-label='Functions' }
Deletes all completion marks for a given character. Supports both modded and vanilla characters. 

___
### CreateTimer () {: aria-label='Functions' }
#### [EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/EntityEffect.html) CreateTimer ( function Function, int Interval, int Times, boolean Persistent ) {: .copyable aria-label='Functions' }
Spawns a timer EntityEffect. This entity will starting running `Function` function after `Interval` frames, and will repeat it `Times` amount of times. `Persistent` controls whether or not this timer "dies" in the current room, or persists across rooms.

???- info "Timer behavior"
    This timer is called every game update. This means the timer only takes into consideration frames in which the game is actively running, not paused, and uses update frames for its Delay parameter (30 frames per second). 
	
	If your use case requires that a timer takes paused time into account, stick with a custom timer running on a RENDER callback.

___
### CreateWeapon () {: aria-label='Functions' }
#### [Weapon](Weapon.md) CreateWeapon ( [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) Type, [Entity](Entity.md) Owner ) {: .copyable aria-label='Functions' }

___
### DestroyWeapon () {: aria-label='Functions' }
#### void DestroyWeapon ( [Weapon](Weapon.md) Weapon ) {: .copyable aria-label='Functions' }

___
### DrawLine () {: aria-label='Functions' }
#### void DrawLine ( [Vector](Vector.md) StartPos, [Vector](Vector.md) EndPos, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor) StartColor, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor) EndColor, int Thickness ) {: .copyable aria-label='Functions' }
Draws a line between the two given positions this render frame.

___
### DrawQuad () {: aria-label='Functions' }
#### void DrawQuad ( [Vector](Vector.md) TopLeftPos, [Vector](Vector.md) TopRightPos, [Vector](Vector.md) BottomLeftPos, [Vector](Vector.md) BottomRightPos, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor) Color, int Thickness ) {: .copyable aria-label='Functions' }
Draws a line between the two given positions this render frame. Internally the game uses its own struct for this, DestinationQuad, but I haven't gotten to adding that to Lua yet :crocodile:

___
### FillCompletionMarks () {: aria-label='Functions' }
#### void FillCompletionMarks ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) Character) {: .copyable aria-label='Functions' }
Completes all completion marks for a given character. Supports both modded and vanilla characters. 
___
### FindInCapsule () {: aria-label='Functions' }
#### [Entity](Entity.md)[] FindInCapsule ( [Capsule](Capsule.md) Capsule, [EntityPartitions](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityPartition.html) Partitions = -1 ) {: .copyable aria-label='Functions' }
Return entities inside of given capsule, filtered by partitions mask.
___
### GetAchievementIdByName () {: aria-label='Functions' }
#### table GetAchievementIdByName ( string name ) {: .copyable aria-label='Functions' }
Gets the Achievement ID By Name.

???+ warning "Warning"
    Modded achievements are not loaded until past the main menu is initialized, so dont use this function on modload to get your ids. A suggested place to do so is the MC_POST_SAVESLOT_LOAD callback.
___
### GetClipboard () {: aria-label='Functions' }
#### string GetClipboard ( ) {: .copyable aria-label='Functions' }
Gets the contents of the clipboard as long as they are in text form, otherwise it will just return nil.
___
### GetCollectibleSpawnPosition () {: aria-label='Functions' }
#### [Vector](Vector.md) GetCollectibleSpawnPosition ( [Vector](Vector.md) Position ) {: .copyable aria-label='Functions' }

___
### GetCompletionMark () {: aria-label='Functions' }
#### int GetCompletionMark ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) Character, [CompletionType](enums/CompletionType.md) Mark) {: .copyable aria-label='Functions' }
Gets a completion mark value for a specific character. Supports both modded and vanilla characters. 

___
### GetCompletionMarks () {: aria-label='Functions' }
#### table GetCompletionMarks ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) Character) {: .copyable aria-label='Functions' }
Returns a table containing all the marks for the character. Supports both modded and vanilla characters.

???- info "Table structure & usage"
	- The table has the following fields: 
		* PlayerType: containing the [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) asociated to the marks
		* MomsHeart: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Isaac: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Satan: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* BossRush: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* BlueBaby: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Lamb: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* MegaSatan: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* UltraGreed: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Hush: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* UltraGreedier: Mostly redundant with UltraGreed when it has a value of 2, no need to set it
		* Delirium: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Mother: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Beast: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
			```lua
			local marks = Isaac.GetCompletionMarks(0)
			if (marks.MomsHeart > 0) then
				print("got mom")
			end
			if (marks.Lamb >= 2) then
				print("GOATED ON H4RD")
			end
			if (Isaac.GetCompletionMarks(0).Delirium > 0) then --doing it the lazy way, fitting deliriums theme
				print("Got Deli")
			end
			```

___
### GetCursorSprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetCursorSprite ( ) {: .copyable aria-label='Functions' }
Returns the cursor sprite that is rendered when ``Options.MouseControl`` is set to true. 

___
### GetCutsceneIdByName () {: aria-label='Functions' }
#### table GetCutsceneIdByName ( string Name ) {: .copyable aria-label='Functions' }
Gets the Cutscene ID By Name.

___
### GetEntitySubTypeByName () {: aria-label='Functions' }
#### int GetEntitySubTypeByName ( string Name ) {: .copyable aria-label='Functions' }
Gets the entity SubType by entity name.

___
### GetGiantBookIdByName () {: aria-label='Functions' }
#### int GetGiantBookIdByName ( string Name ) {: .copyable aria-label='Functions' }
Gets a GiantBook Id by name. For vanilla giantbooks, the png filename, from the gfx xml attribute, is used as the giantbook name.

___
### GetLoadedModules () {: aria-label='Functions' }
#### table GetLoadedModules ( ) {: .copyable aria-label='Functions' }
Returns a key - value table containing all loaded script files, where the key is the name or path of a given script file, and the value the return value of that file after loading. (In most cases its true or a table)

___
### GetLocalizedString () {: aria-label='Functions' }
#### string GetLocalizedString ( string Category, string Key, int Language ) {: .copyable aria-label='Functions' }
#### string GetLocalizedString ( string Category, string Key, string LanguageCode ) {: .copyable aria-label='Functions' }
Returns the translation string associated with the given key in the given category. The translation is given in the language ID/language code given as parameter.

___
### GetNullItemIdByName () {: aria-label='Functions' }
#### int GetNullItemIdByName ( ) {: .copyable aria-label='Functions' }

___
### GetPersistentGameData () {: aria-label='Functions' }
#### [PersistentGameData](PersistentGameData.md) GetPersistentGameData ( ) {: .copyable aria-label='Functions' }

___
### GetRenderPosition () {: aria-label='Functions' }
#### [Vector](Vector.md) GetRenderPosition ( [Vector](Vector.md) Position, boolean Scale = true ) {: .copyable aria-label='Functions' }       

___
### GetString () {: aria-label='Functions' }
#### string GetString ( string Category, string Key ) {: .copyable aria-label='Functions' }
Returns the translation string associated with the given key in the given category. The translation is given in the currently selected language.

___
### IsInGame () {: aria-label='Functions' }
#### boolean IsInGame ( ) {: .copyable aria-label='Functions' }
Returns `true` if `Game` is non-nil and the current state is correct.

___

### LevelGeneratorEntry () {: aria-label='Functions' }
#### [LevelGeneratorEntry](LevelGeneratorEntry.md) LevelGeneratorEntry ( ) {: .copyable aria-label='Functions' }
Creates a new blank [LevelGeneratorEntry](LevelGeneratorEntry.md) object.

___
### PlayCutscene () {: aria-label='Functions' }
#### int PlayCutscene ( int ID ) {: .copyable aria-label='Functions' }
Plays the Cutscene of the provided ID. Use Isaac.GetCutsceneIdByName to get the IDs, or the enum for the vanilla ones if you prefer.
___
### SetClipboard () {: aria-label='Functions' }
#### boolean SetClipboard ( string ClipboardData ) {: .copyable aria-label='Functions' }
Sets the contents of the clipboard to the provided string.

___
### SetCompletionMark () {: aria-label='Functions' }
#### void SetCompletionMark ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) Character, [CompletionType](enums/CompletionType.md) Mark, int Value) {: .copyable aria-label='Functions' }
Sets a completion mark of a character to match a specific value from `0` to `2`. Supports both modded and vanilla characters. 

___
### SetCompletionMarks () {: aria-label='Functions' }
#### void SetCompletionMarks ( table Marks ) {: .copyable aria-label='Functions' }
Sets the completion marks of a character to match an input table. Supports both modded and vanilla characters. Requires a table containing all the marks for the character, getting it from [GetCompletionMarks](Isaac.md#GetCompletionMarks) is adviced for convenience.

???- info "Table structure & usage"
	- The table needs the following fields: 
		* PlayerType: containing the [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) asociated to the marks
		* MomsHeart: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Isaac: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Satan: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* BossRush: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* BlueBaby: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Lamb: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* MegaSatan: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* UltraGreed: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Hush: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* UltraGreedier: Mostly redundant with UltraGreed when it has a value of 2, no need to set it
		* Delirium: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Mother: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
		* Beast: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html) 0-2 indicating the completion
			```lua
			local marks = Isaac.GetCompletionMarks(0) --getting the current table
			marks.MomsHeart = 2 --Isaac now will have the hard mark on MHeart
			marks.Satan = 1 --Isaac will now have the normal mark on Satan
			marks.BlueBaby = 0 --Removes the BlueBaby Mark if its present
			Isaac.SetCompletionMarks(marks) --Impacts the changes on the player
			```
___
### ShowErrorDialog () {: aria-label='Functions' }
#### [DialogReturn](enums/DialogReturn.md) ShowErrorDialog ( string Title, string Text, [DialogIcons](enums/DialogIcons.md) Icon = DialogIcons.ERROR, [DialogButtons](enums/DialogButtons.md) Buttons = DialogButtons.OK ) {: .copyable aria-label='Functions' }
Displays a Win32 message box. Can be controlled with the `icon` and `buttons` parameters. Returns a [`DialogReturn`](enums/DialogReturn.md) value that indicates the button pressed.
___
### WorldToMenuPosition () {: aria-label='Functions' }
#### [Vector](Vector.md) WorldToMenuPosition ( [MainMenu](enums/MainMenuType.md) MenuId, [Vector](Vector.md) Position ) {: .copyable aria-label='Functions' }
Converts the World position from input to a pinned main menu position that varies depending on the enum selected. It's important to reconvert this every frame, in a similar fashion to WorldToRender, in order to properly render when menus are changed or the window is resized.
___
### TriggerWindowResize () {: aria-label='Functions' }
#### void TriggerWindowResize ( ) {: .copyable aria-label='Functions' }
Simulates a window resize, useful to refresh some option changes like `maxrender`.
___
