# Class "Color"
 
## Constants
### Color.EmberFade {: aria-label='Constants' }
[ ](#){: .rep .tooltip .badge }
Used for enemies like Crackles and Coal Spiders. Gibs start orange and fade into grey.

Tint of (0, 0, 0, 1.1)
Colorize of (0, 0, 0, 0)
Offset of (1, 0.514, 0.004)

___
## Functions

### Get·Colorize () {: aria-label='Functions' }
#### table GetColorize ( ) {: .copyable aria-label='Functions' }
Returns a table corresponding to the color's current Colorize values: `{R, G, B, A}`

___
### Get·Offset () {: aria-label='Functions' }
#### table GetOffset ( ) {: .copyable aria-label='Functions' }
Returns a table corresponding to the color's current Offset values: `{R, G, B}`

While the [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) class does already contain [.RO](https://wofsauge.github.io/IsaacDocs/rep/Color.html#ro), [.GO](https://wofsauge.github.io/IsaacDocs/rep/Color.html#go) and [.BO](https://wofsauge.github.io/IsaacDocs/rep/Color.html#bo) variables for this, GetOffset() has been measured to be ~30 faster in cases where access to all three values is necessary and is recommended in that scenario as a result. Performance is near identical when accessing two variables, and worse when accessing one. In cases where only one or two offset values are needed, stick with the variables.

___
### Get·Tint () {: aria-label='Functions' }
#### table GetOffset ( ) {: .copyable aria-label='Functions' }
Returns a table corresponding to the color's current Tint values: `{R, G, B, A}`

___
	