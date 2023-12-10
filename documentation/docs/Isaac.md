---
tags:
  - Global
  - Class
---
# Global Class "Isaac"
## Modified Functions

### Find·By·Type () {: aria-label='Modified Functions' }
#### [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html)[] FindByType ( [EntityType](https://wofsauge.github.io/IsaacDocs/rep/enums/EntityType.html) Type, int Variant = -1, int SubType = -1, boolean Cache = false, boolean IgnoreFriendly = false ) {: .copyable aria-label='Modified Functions' }
Same as vanilla, but much faster.
___
### Find·In·Radius () {: aria-label='Modified Functions' }
#### [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html)[] FindInRadius ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position, float Radius, int Partitions = 0xFFFFFFFF  ) {: .copyable aria-label='Modified Functions' }
Same as in vanilla, but much faster and with fixed search for effects.
___
### Get·Room·Entities () {: aria-label='Modified Functions' }
#### [Entity](https://wofsauge.github.io/IsaacDocs/rep/Entity.html)[] GetRoomEntities ( ) {: .copyable aria-label='Modified Functions' }
Same as vanilla, but much faster.

___
## Functions

### All·Marks·Filled () {: aria-label='Functions' }
#### int AllMarksFilled ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) playertype) {: .copyable aria-label='Functions' }
Checks if a given character has completed all marks and returns the highest difficulty it was accomplished in if it applies(1-normal/2-hard/0-none). Supports both modded and vanilla characters. 

___
### All·Tainted·Completion () {: aria-label='Functions' }
#### int AllTaintedCompletion ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) playertype, [TaintedMarksGroup](enums/TaintedMarksGroup.md) complgroup) {: .copyable aria-label='Functions' }
Checks if a given character has completed all the tainted unlock-related marks and returns the highest difficulty it was accomplished in if it applies(1-normal/2-hard/0-none). Supports both modded and vanilla characters. 

___
### Can·Start·True·Coop () {: aria-label='Functions' }
#### boolean CanStartTrueCoop ( ) {: .copyable aria-label='Functions' }

___
### Clear·Boss·Hazards () {: aria-label='Functions' }
#### void ClearBossHazards ( boolean npcs = false ) {: .copyable aria-label='Functions' }

___
### Clear·Completion·Marks () {: aria-label='Functions' }
#### void ClearCompletionMarks ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) playertype) {: .copyable aria-label='Functions' }
Deletes all completion marks for a given character. Supports both modded and vanilla characters. 

___
### Create·Timer () {: aria-label='Functions' }
#### [EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/EntityEffect.html) CreateTimer ( function Function, int Interval, int Times, boolean Persistent ) {: .copyable aria-label='Functions' }
Spawns a timer EntityEffect. This entity will starting running `Function` function after `Interval` frames, and will repeat it `Times` amount of times. `Persistent` controls whether or not this timer "dies" in the current room, or persists across rooms.

???- info "Timer behavior"
    This timer is called every game update. This means the timer only takes into consideration frames in which the game is actively running, not paused, and uses update frames for its Delay parameter (30 frames per second). 
	
	If your use case requires that a timer takes paused time into account, stick with a custom timer running on a RENDER callback.

___
### Create·Weapon () {: aria-label='Functions' }
#### [Weapon](Weapon.md) CreateWeapon ( [WeaponType](https://wofsauge.github.io/IsaacDocs/rep/enums/WeaponType.html) Type, [Entity](Entity.md) Owner ) {: .copyable aria-label='Functions' }

___
### Destroy·Weapon () {: aria-label='Functions' }
#### void DestroyWeapon ( ) {: .copyable aria-label='Functions' }

___
### Draw·Line () {: aria-label='Functions' }
#### void DrawLine ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Pos1, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Pos2, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor) Col1, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor) Col2, int Thickness ) {: .copyable aria-label='Functions' }
Draws a line between the two given positions this render frame.

___
### Draw·Quad () {: aria-label='Functions' }
#### void DrawQuad ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) TopLeftPos, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) TopRightPos, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) BottomLeftPos, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) BottomRightPos, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor) Color, int Thickness ) {: .copyable aria-label='Functions' }
Draws a line between the two given positions this render frame. Internally the game uses its own struct for this, DestinationQuad, but I haven't gotten to adding that to Lua yet :crocodile:

___
### Fill·Completion·Marks () {: aria-label='Functions' }
#### void FillCompletionMarks ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) playertype) {: .copyable aria-label='Functions' }
Completes all completion marks for a given character. Supports both modded and vanilla characters. 
___
### Get·Achievement·Id·By·Name () {: aria-label='Functions' }
#### table GetAchievementIdByName ( string name ) {: .copyable aria-label='Functions' }
Gets the Achievement ID By Name.
___
### Get·Clipboard () {: aria-label='Functions' }
#### string GetClipboard ( ) {: .copyable aria-label='Functions' }
Gets the contents of the clipboard as long as they are in text form, otherwise it will just return nil.
___
### Get·Collectible·Spawn·Position () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetCollectibleSpawnPosition ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position ) {: .copyable aria-label='Functions' }

___
### Get·Completion·Mark () {: aria-label='Functions' }
#### int GetCompletionMark ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) playertype, [CompletionType](enums/CompletionType.md) marktype) {: .copyable aria-label='Functions' }
Gets a completion mark value for a specific character. Supports both modded and vanilla characters. 

___
### Get·Completion·Marks () {: aria-label='Functions' }
#### table GetCompletionMarks ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) playertype) {: .copyable aria-label='Functions' }
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
### Get·Cursor·Sprite () {: aria-label='Functions' }
#### [Sprite](Sprite.md) GetCursorSprite ( ) {: .copyable aria-label='Functions' }
Returns the cursor sprite that is rendered when ``Options.MouseControl`` is set to true. 
___
### Get·Cutscene·Id·By·Name () {: aria-label='Functions' }
#### table GetCutsceneIdByName ( string name ) {: .copyable aria-label='Functions' }
Gets the Cutscene ID By Name.
___
### Get·Entity·Sub·Type·By·Name () {: aria-label='Functions' }
#### int GetEntitySubTypeByName ( string name) {: .copyable aria-label='Functions' }
Gets the entity SubType by entity name.

___
### Get·Giant·Book·Id·By·Name () {: aria-label='Functions' }
#### int GetGiantBookIdByName ( string name) {: .copyable aria-label='Functions' }
Gets a GiantBook Id by name. For vanilla giantbooks, the png filename, from the gfx xml attribute, is used as the giantbook name.

___
### Get·Im·Gui () {: aria-label='Functions' }
#### [ImGui](ImGui.md) GetImGui ( ) {: .copyable aria-label='Functions' }

___
### Get·Loaded·Modules () {: aria-label='Functions' }
#### table GetLoadedModules ( ) {: .copyable aria-label='Functions' }
Returns a key - value table containing all loaded script files, where the key is the name or path of a given script file, and the value the return value of that file after loading. (In most cases its true or a table)

___
### Get·Localized·String () {: aria-label='Functions' }
#### string GetLocalizedString ( string category, string key, int language ) {: .copyable aria-label='Functions' }
Returns the translation string associated with the given key in the given category. The translation is given in the language ID given as parameter.

___
### Get·Null·Item·Id·By·Name () {: aria-label='Functions' }
#### int GetNullItemIdByName ( ) {: .copyable aria-label='Functions' }

___
### Get·Persistent·Game·Data () {: aria-label='Functions' }
#### [PersistentGameData](PersistentGameData.md) GetPersistentGameData ( ) {: .copyable aria-label='Functions' }
Should probably be moved to Game for consistency.

___
### Get·Render·Position () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetRenderPosition ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) position, boolean scale = true ) {: .copyable aria-label='Functions' }       

___
### Get·String () {: aria-label='Functions' }
#### string GetString ( string category, string key ) {: .copyable aria-label='Functions' }
Returns the translation string associated with the given key in the given category. The translation is given in the currently selected language.
___
### Level·Generator·Entry () {: aria-label='Functions' }
#### [LevelGeneratorEntry](LevelGeneratorEntry.md) LevelGeneratorEntry ( ) {: .copyable aria-label='Functions' }
Creates a new blank [LevelGeneratorEntry](LevelGeneratorEntry.md) object.

___
### Play·Cutscene () {: aria-label='Functions' }
#### int PlayCutscene ( int cutsceneid ) {: .copyable aria-label='Functions' }
Plays the Cutscene of the provided ID. Use Isaac.GetCutsceneIdByName to get the IDs, or the enum for the vanilla ones if you prefer.
___
### Set·Clipboard () {: aria-label='Functions' }
#### boolean SetClipboard ( string clipboarddata) {: .copyable aria-label='Functions' }
Sets the contents of the clipboard to the provided string.

___
### Set·Completion·Mark () {: aria-label='Functions' }
#### void SetCompletionMark ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html) playertype, [CompletionType](enums/CompletionType.md) marktype, int value) {: .copyable aria-label='Functions' }
Sets a completion mark of a character to match a specific value from 0 to 2. Supports both modded and vanilla characters. 


___
### Set·Completion·Marks () {: aria-label='Functions' }
#### void SetCompletionMarks ( table marks) {: .copyable aria-label='Functions' }
Sets the completion marks of a character to match an input table. Supports both modded and vanilla characters. Requires a table containing all the marks for the character, getting it from Isaac.GetCompletionMarks is adviced for convenience.

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
### Show·Error·Dialog () {: aria-label='Functions' }
#### [DialogReturn](enums/DialogReturn.md) ShowErrorDialog ( string title, string text, [DialogIcons](enums/DialogIcons.md) icon = DialogIcons.ERROR, [DialogButtons](enums/DialogButtons.md) buttons = DialogButtons.OK ) {: .copyable aria-label='Functions' }
Displays a Win32 message box. Can be controlled with the `icon` and `buttons` parameters. Returns a [`DialogReturn`](enums/DialogReturn.md) value that indicates the button pressed.
___
### World·To·Menu·Position () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) WorldToMenuPosition ( [MainMenu](enums/MainMenuType.md) MenuId, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position ) {: .copyable aria-label='Functions' }
Converts the World position from input to a pinned main menu position that varies depending on the enum selected. It's important to reconvert this every frame, in a similar fashion to WorldToRender, in order to properly render when menus are changed or the window is resized.
