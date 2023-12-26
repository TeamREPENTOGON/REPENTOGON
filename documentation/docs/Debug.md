---
tags:
  - Global
  - Class
---
# Global Class "Debug"

???+ info
    You can get this class by using the `Debug` global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local loadedFiles = Debug.ListLoadedFiles()
        ```
        
## Functions

### ForceUnload () {: aria-label='Functions' }
#### void ForceUnload ( string ModuleName ) {: .copyable aria-label='Functions' }

___
### GetSignature () {: aria-label='Functions' }
#### string GetSignature ( int Address ) {: .copyable aria-label='Functions' }

___
### ListLoadedFiles () {: aria-label='Functions' }
#### string[] ListLoadedFiles ( ) {: .copyable aria-label='Functions' }
Returns a list of all files loaded into the LUA environment.
___