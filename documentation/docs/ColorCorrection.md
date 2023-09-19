# Class "ColorCorrection"

???+ info
    You can get this class by using the following functions:

    * [FXParams:GetColorCorrection()](FXParams.md#getcolorcorrection)

    ???+ example "Example Code"
        ```lua
        local colorCorrection = Game():GetCurrentRoom():GetFXParams():GetColorCorrection()
        ```
        
## Functions

### Update () {: aria-label='Functions' }
#### void Update ( boolean Process = true, boolean Lerp = true, float Rate = 0.015) {: .copyable aria-label='Functions' }
`Process` will run the color correction through a series of modifications made by [Room](Room.md) for handling the Abandoned Mineshaft sequence, pitch black rooms, and tinting based on lava intensity. 

___
## Variables
### Color {: aria-label='Variables' }
[ ](#){: .tooltip .badge }
#### [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor.html) Color {: .copyable aria-label='Variables'}

???+ warning "Warning"
    `Alpha` acts as a multiplier and must be non-zero in order for this to have any effect!
___
### Brightness {: aria-label='Variables' }
[ ](#){: .tooltip .badge }
#### float Brightness {: .copyable aria-label='Variables'}

___
### Contrast {: aria-label='Variables' }
[ ](#){: .tooltip .badge }
#### float Contrast {: .copyable aria-label='Variables'}
