# Class "Isaac"

## Functions


### All·Marks·Filled () {: aria-label='Functions' }
#### int AllMarksFilled ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html?h=playerty) playertype) {: .copyable aria-label='Functions' }
Checks if a given character has completed all marks and returns the highest difficulty it was accomplished in if it applies(1-normal/2-hard/0-none). Supports both modded and vanilla characters. 

___
### All·Tainted·Completion () {: aria-label='Functions' }
#### int AllTaintedCompletion ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html?h=playerty) playertype, [TaintedMarksGroup](https://repentogon.com/enums/TaintedMarksGroup.html) complgroup) {: .copyable aria-label='Functions' }
Checks if a given character has completed all the tainted unlock-related marks and returns the highest difficulty it was accomplished in if it applies(1-normal/2-hard/0-none). Supports both modded and vanilla characters. 

___
### Create·Timer () {: aria-label='Functions' }
#### [EntityEffect](https://wofsauge.github.io/IsaacDocs/rep/EntityEffect.html) CreateTimer ( function Function, int Delay, int Times, boolean Persistent ) {: .copyable aria-label='Functions' }
Spawns a timer EntityEffect. This entity will run Function function after Delay frames, and will repeat this Times amount of times. Persistent controls whether or not this timer "dies" in the current room, or persists across rooms.

???- info "Timer behavior"
    This timer is called every game update. This means the timer only takes into consideration frames in which the game is actively running, not paused, and uses update frames for its Delay parameter (30 frames per second). 
	
	If your use case requires that a timer takes paused time into account, stick with a custom timer running on a RENDER callback.

___
### Clear·Completion·Marks () {: aria-label='Functions' }
#### void ClearCompletionMarks ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html?h=playerty) playertype) {: .copyable aria-label='Functions' }
Deletes all completion marks for a given character. Supports both modded and vanilla characters. 

___
### Fill·Completion·Marks () {: aria-label='Functions' }
#### void FillCompletionMarks ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html?h=playerty) playertype) {: .copyable aria-label='Functions' }
Completes all completion marks for a given character. Supports both modded and vanilla characters. 

___
### Get·Completion·Mark () {: aria-label='Functions' }
#### int GetCompletionMark ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html?h=playerty) playertype, [CompletionType](https://repentogon.com/enums/CompletionType.html) marktype) {: .copyable aria-label='Functions' }
Gets a completion mark value for a specific character. Supports both modded and vanilla characters. 

___
### Get·Completion·Marks () {: aria-label='Functions' }
#### table GetCompletionMarks ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html?h=playerty) playertype) {: .copyable aria-label='Functions' }
Returns a table containing all the marks for the character. Supports both modded and vanilla characters.

???- info "Table structure & usage"
	- The table has the following fields: 
		* PlayerType: containing the [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html?h=playerty) asociated to the marks
		* MomsHeart: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Isaac: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Satan: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* BossRush: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* BlueBaby: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Lamb: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* MegaSatan: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* UltraGreed: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Hush: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* UltraGreedier: Mostly redundant with UltraGreed when it has a value of 2, no need to set it
		* Delirium: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Mother: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Beast: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
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
### Get·Daily·Challenge () {: aria-label='Functions' }
#### [DailyChallenge](DailyChallenge.md) GetDailyChallenge ( ) {: .copyable aria-label='Functions' }

___
### Draw·Line () {: aria-label='Functions' }
#### void DrawLine ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Pos1, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Pos2, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor) Col1, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor) Col2, int Thickness ) {: .copyable aria-label='Functions' }
Draws a line between the two given positions this render frame.

___
### Draw·Quad () {: aria-label='Functions' }
#### void DrawQuad ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) TopLeftPos, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) TopRightPos, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) BottomLeftPos, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) BottomRightPos, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor) Color, int Thickness ) {: .copyable aria-label='Functions' }
Draws a line between the two given positions this render frame. Internally the game uses its own struct for this, DestinationQuad, but I haven't gotten to adding that to Lua yet :crocodile:

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
### Set·Completion·Mark () {: aria-label='Functions' }
#### void SetCompletionMark ( [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html?h=playerty) playertype, [CompletionType](https://repentogon.com/enums/CompletionType.html) marktype, int value) {: .copyable aria-label='Functions' }
Sets a completion mark of a character to match a specific value from 0 to 2. Supports both modded and vanilla characters. 


___
### Set·Completion·Marks () {: aria-label='Functions' }
#### void SetCompletionMarks ( table marks) {: .copyable aria-label='Functions' }
Sets the completion marks of a character to match an input table. Supports both modded and vanilla characters. Requires a table containing all the marks for the character, getting it from Isaac.GetCompletionMarks is adviced for convenience.

???- info "Table structure & usage"
	- The table needs the following fields: 
		* PlayerType: containing the [PlayerType](https://wofsauge.github.io/IsaacDocs/rep/enums/PlayerType.html?h=playerty) asociated to the marks
		* MomsHeart: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Isaac: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Satan: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* BossRush: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* BlueBaby: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Lamb: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* MegaSatan: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* UltraGreed: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Hush: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* UltraGreedier: Mostly redundant with UltraGreed when it has a value of 2, no need to set it
		* Delirium: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Mother: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
		* Beast: value of [Difficulty](https://wofsauge.github.io/IsaacDocs/rep/enums/Difficulty.html?h=difficul) 0-2 indicating the completion
			```lua
			local marks = Isaac.GetCompletionMarks(0) --getting the current table
			marks.MomsHeart = 2 --Isaac now will have the hard mark on MHeart
			marks.Satan = 1 --Isaac will now have the normal mark on Satan
			marks.BlueBaby = 0 --Removes the BlueBaby Mark if its present
			Isaac.SetCompletionMarks(marks) --Impacts the changes on the player
			```
___
### World·To·Menu·Position () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) WorldToMenuPosition ( [MainMenu](enums/MainMenu.md) MenuId, [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position ) {: .copyable aria-label='Functions' }
Converts the World position from input to a pinned main menu position that varies depending on the enum selected. It's important to reconvert this every frame, in a similar fashion to WorldToRender, in order to properly render when menus are changed or the window is resized.

## Variables

### Level·Generator·Entry {: aria-label='Variables' }
#### [LevelGeneratorEntry](LevelGeneratorEntry.md) LevelGeneratorEntry  {: .copyable aria-label='Variables' }
Fetches a blank LevelGeneratorEntry from the game.

___
