---
tags:
  - Class
---
# Class "ColorParams"

???+ info
    This class can be accessed using its constructor:
    ???+ example "Example Code"
        ```lua
        local fiveSecondRedColor = ColorParams(Color(1,0,0,1),255,150,false,false)
        ```

## Functions
### GetColor () {: aria-label='Functions' }
#### [Color](Color.md) GetColor ( ) {: .copyable aria-label='Functions' }

___
### GetDuration () {: aria-label='Functions' }
#### int GetDuration ( ) {: .copyable aria-label='Functions' }
Defines the time in update frames that these parameters should last. Has no effect on how many frames are left, but does affect fadeout speed (calculated as `Lifespan / Duration`) if `Fadeout` is enabled.

___
### GetFadeout () {: aria-label='Functions' }
#### boolean GetFadeout ( ) {: .copyable aria-label='Functions' }

___
### GetLifespan () {: aria-label='Functions' }
#### int GetLifespan ( ) {: .copyable aria-label='Functions' }
Defines how many update frames are _left_ before this expires. This is decremented by `1` each non-interpolation update at a rate of `30` per second. Altering this will directly effect how many frames are left before these parameters expire.

___
### GetPriority () {: aria-label='Functions' }
#### int GetPriority ( ) {: .copyable aria-label='Functions' }

___
### GetShared () {: aria-label='Functions' }
#### boolean GetShared ( ) {: .copyable aria-label='Functions' }

___
### SetColor () {: aria-label='Functions' }
#### void SetColor ( [Color](Color.md) Color ) {: .copyable aria-label='Functions' }

___
### SetDuration () {: aria-label='Functions' }
#### void SetDuration ( int Duration ) {: .copyable aria-label='Functions' }

___
### SetFadeout () {: aria-label='Functions' }
#### void SetFadeout ( boolean Value ) {: .copyable aria-label='Functions' }

___
### SetLifespan () {: aria-label='Functions' }
#### void SetLifespan ( int Duration ) {: .copyable aria-label='Functions' }

___
### SetPriority () {: aria-label='Functions' }
#### void SetPriority ( int Priority ) {: .copyable aria-label='Functions' }

___
### SetShared () {: aria-label='Functions' }
#### void SetFadeout ( boolean Value ) {: .copyable aria-label='Functions' }

___