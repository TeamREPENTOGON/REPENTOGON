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

### Force·Unload () {: aria-label='Functions' }
#### void ForceUnload ( string moduleName ) {: .copyable aria-label='Functions' }

___
### Get·Signature () {: aria-label='Functions' }
#### string GetSignature ( int addr ) {: .copyable aria-label='Functions' }

___
### List·Loaded·Files () {: aria-label='Functions' }
#### string[] ListLoadedFiles ( ) {: .copyable aria-label='Functions' }
returns a list of all files loaded into the LUA environment.
___