---
tags:
  - Class
---
# Class "Color"

???+ info
    This class can be accessed using its constructor:
    ???+ example "Example Code"
        ```lua
        local redColor = Color(1,0,0,1)
        ```

## Modified Constructors

### Color () {: aria-label='Modified Constructors' }
#### [Color](Color.md) Color ( float R = 1, float G = 1, float B = 1, float A = 1, float RO = 0, float GO = 0, float BO = 0, float RC = 0, float GC = 0, float BC = 0, float AC = 0 ) {: .copyable aria-label='Modified Constructors' }

All arguments are now optional. `Colorize` can now be set through the constructor.

___
## Functions

### GetColorize () {: aria-label='Functions' }
#### table GetColorize ( ) {: .copyable aria-label='Functions' }
Returns a table corresponding to the color's current Colorize values: `{R, G, B, A}`

___
### GetOffset () {: aria-label='Functions' }
#### table GetOffset ( ) {: .copyable aria-label='Functions' }
Returns a table corresponding to the color's current Offset values: `{R, G, B}`

While the [Color](Color.md) class does already contain [.RO](Color.md#ro), [.GO](Color.md#go) and [.BO](Color.md#bo) variables for this, GetOffset() has been measured to be ~30% faster in cases where access to all three values is necessary and is recommended in that scenario as a result. Performance is near identical when accessing two variables, and worse when accessing one. In cases where only one or two offset values are needed, stick with the variables.

___
### GetTint () {: aria-label='Functions' }
#### table GetTint ( ) {: .copyable aria-label='Functions' }
Returns a table corresponding to the color's current Tint values: `{R, G, B, A}`

___
### Print () {: aria-label='Functions' }
#### string Print ( ) {: .copyable aria-label='Functions' }
Returns a string representation of the color object.

___
### __tostring () {: aria-label='Functions' }
#### string __tostring ( ) {: .copyable aria-label='Operators' }
Creates a string representation of the color object.

This enables direct printing of the object by doing `print(myColorObj)`

___

## Constants

### Color.EmberFade {: aria-label='Constants' }
#### [Color](Color.md) EmberFade {: .copyable aria-label='Constants' }
Used for enemies like Crackles and Coal Spiders. This color has a hardcoded special property; gibs start orange and fade into grey.

???- info "Info"
    Color of (0, 0, 0, 1.1)

    Colorize of (0, 0, 0, 0)

    Offset of (1, 0.514, 0.004)

___
### Color.LaserAlmond {: aria-label='Constants' }
#### [Color](Color.md) LaserAlmond {: .copyable aria-label='Constants' }
Used for lasers with the Almond Milk effect.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (5.6, 5.2, 3.8, 1)

    Offset of (0, 0, 0)

___
### Color.LaserChocolate {: aria-label='Constants' }
#### [Color](Color.md) LaserChocolate {: .copyable aria-label='Constants' }
Used for lasers with the Chocolate Milk effect.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (3, 1.7, 1.7, 1)

    Offset of (0, 0, 0)

___
### Color.LaserCoal {: aria-label='Constants' }
#### [Color](Color.md) LaserCoal {: .copyable aria-label='Constants' }
Used for lasers with the A Lump of Coal effect.

???- info "Info"
    Color of (3, 3, 3, 1)

    Colorize of (1.3, 1.2, 1.2, 1)

    Offset of (-0.5, -0.5, -0.5)

___
### Color.LaserFireMind {: aria-label='Constants' }
#### [Color](Color.md) LaserFireMind {: .copyable aria-label='Constants' }
Used for lasers fired by players with Fire Mind.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (5, 3, 1, 1)

    Offset of (0, 0, 0)

___
### Color.LaserHoming {: aria-label='Constants' }
#### [Color](Color.md) LaserHoming {: .copyable aria-label='Constants' }
Used for homing lasers.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (3, 1, 3.5, 0)

    Offset of (0, 0, 0)

___
### Color.LaserIpecac {: aria-label='Constants' }
#### [Color](Color.md) LaserIpecac {: .copyable aria-label='Constants' }
Used for lasers with the Ipecac effect.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (1.8, 3, 1, 1)

    Offset of (0, 0, 0)

___
### Color.LaserMother {: aria-label='Constants' }
#### [Color](Color.md) LaserMother {: .copyable aria-label='Constants' }
Used for Mother's mega laser.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (2, 2.2, 1, 1)

    Offset of (0, 0, 0)

___
### Color.LaserNumberOne {: aria-label='Constants' }
#### [Color](Color.md) LaserNumberOne {: .copyable aria-label='Constants' }
Used for lasers fired by players with Number One.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (5, 4.9, 1, 1)

    Offset of (0, 0, 0)

___
### Color.LaserSoy {: aria-label='Constants' }
#### [Color](Color.md) LaserSoy {: .copyable aria-label='Constants' }
Used for lasers with the Soy Milk effect.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (5.6, 5, 4.2, 1)

    Offset of (0, 0, 0)

___
### Color.LaserPoison {: aria-label='Constants' }
#### [Color](Color.md) LaserPoison {: .copyable aria-label='Constants' }
Used for poisonous lasers fired by players with items like Scorpio or Common Cold.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (1.8, 4, 1, 1)

    Offset of (0, 0, 0)

___
### Color.ProjectileCageBlue {: aria-label='Constants' }
#### [Color](Color.md) ProjectileCageBlue {: .copyable aria-label='Constants' }
Used for `ProjectileVariant.PROJECTILE_PUKE`s fired by The Cage.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0.8, 1, 0.85, 1)

    Offset of (0, 0, 0)

___
### Color.ProjectileCorpseClusterDark {: aria-label='Constants' }
#### [Color](Color.md) ProjectileCorpseClusterDark {: .copyable aria-label='Constants' }
Used for clustered `ProjectileVariant.PROJECTILE_NORMAL`s fired in Corpse by enemies like Mother.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0.63, 0.85, 0.32, 0)

    Offset of (0, 0, 0)

___
### Color.ProjectileCorpseClusterLight {: aria-label='Constants' }
#### [Color](Color.md) ProjectileCorpseClusterLight {: .copyable aria-label='Constants' }
Used for clustered `ProjectileVariant.PROJECTILE_NORMAL`s fired in Corpse by enemies like Mother.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0.63, 0.85, 0.32, 0)

    Offset of (0, 0, 0)

___
### Color.ProjectileCorpseGreen {: aria-label='Constants' }
#### [Color](Color.md) ProjectileCorpseGreen {: .copyable aria-label='Constants' }
Used for green `ProjectileVariant.PROJECTILE_NORMAL`s fired in Corpse by enemies like Mother.

Also used for the green laser fired by Chimera.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (1.5, 2, 1, 1)

    Offset of (0, 0, 0)

___
### Color.ProjectileCorpsePink {: aria-label='Constants' }
#### [Color](Color.md) ProjectileCorpsePink {: .copyable aria-label='Constants' }
Used for pink-ish white-ish `ProjectileVariant.PROJECTILE_NORMAL`s fired in Corpse by enemies like Mother.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (4, 3.5, 3.2, 1)

    Offset of (0, 0, 0)

___
### Color.ProjectileCorpseWhite {: aria-label='Constants' }
#### [Color](Color.md) ProjectileCorpseWhite {: .copyable aria-label='Constants' }
Used for white-ish grey-ish `ProjectileVariant.PROJECTILE_NORMAL`s fired in Corpse by enemies like Mother.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (2.7, 3, 2, 1)

    Offset of (0, 0, 0)

___
### Color.ProjectileCorpseYellow {: aria-label='Constants' }
#### [Color](Color.md) ProjectileCorpseYellow {: .copyable aria-label='Constants' }
Used for yellow `ProjectileVariant.PROJECTILE_NORMAL`s fired in Corpse by enemies like The Scourge.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (3.5, 2.5, 1, 1)

    Offset of (0, 0, 0)

___
### Color.ProjectileFireWave {: aria-label='Constants' }
#### [Color](Color.md) ProjectileFireWave {: .copyable aria-label='Constants' }
Used for fire-pillar-wave-spawning `ProjectileVariant.PROJECTILE_NORMAL`s fired by enemies like Crackle.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (1, 0.3, 0)

___
### Color.ProjectileHoming {: aria-label='Constants' }
#### [Color](Color.md) ProjectileHoming {: .copyable aria-label='Constants' }
Used for homing `ProjectileVariant.PROJECTILE_NORMAL`s fired by enemies like Psychic Maw.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0.8, 0.15, 1, 1)

    Offset of (0.26, 0.05, 0.4)

___
### Color.ProjectileHushBlue {: aria-label='Constants' }
#### [Color](Color.md) ProjectileHushBlue {: .copyable aria-label='Constants' }
Used for blue `ProjectileVariant.PROJECTILE_HUSH`s fired by Hush.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0, 0.2, 0.4)

___
### Color.ProjectileHushGreen {: aria-label='Constants' }
#### [Color](Color.md) ProjectileHushGreen {: .copyable aria-label='Constants' }
Used for blue `ProjectileVariant.PROJECTILE_HUSH`s fired by Hush.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0.2, 0.2, 0)

___
### Color.ProjectileHushYellow {: aria-label='Constants' }
#### [Color](Color.md) ProjectileHushYellow {: .copyable aria-label='Constants' }
Used for blue `ProjectileVariant.PROJECTILE_HUSH`s fired by Hush.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0.4, 0.2, 0)

___
### Color.ProjectileIpecac {: aria-label='Constants' }
#### [Color](Color.md) ProjectileIpecac {: .copyable aria-label='Constants' }
Used for explosive `ProjectileVariant.PROJECTILE_NORMAL`s fired by enemies like Gurgles.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0.4, 2, 0.5, 1)

    Offset of (0, 0, 0)

___
### Color.ProjectileMegaSatanBlack {: aria-label='Constants' }
#### [Color](Color.md) ProjectileMegaSatanBlack {: .copyable aria-label='Constants' }
Used for black `ProjectileVariant.PROJECTILE_NORMAL`s fired by Mega Satan.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0.6, 0.6, 0.6, 1)

    Offset of (0, 0, 0)

___
### Color.ProjectileMegaSatanWhite {: aria-label='Constants' }
#### [Color](Color.md) ProjectileMegaSatanWhite {: .copyable aria-label='Constants' }
Used for white `ProjectileVariant.PROJECTILE_NORMAL`s fired by Mega Satan.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (2, 2, 2, 1)

    Offset of (0, 0, 0)

___
### Color.ProjectileSoy {: aria-label='Constants' }
#### [Color](Color.md) ProjectileSoy {: .copyable aria-label='Constants' }
Used for soy `ProjectileVariant.PROJECTILE_NORMAL`s fired by enemies like Soy Creep.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (1, 1, 1, 1)

    Offset of (0.8, 0.7, 0.5)

___
### Color.ProjectileTar {: aria-label='Constants' }
#### [Color](Color.md) ProjectileTar {: .copyable aria-label='Constants' }
Used for tar `ProjectileVariant.PROJECTILE_NORMAL`s fired by enemies like Clot.

???- info "Info"
    Color of (1, 1, 1, 1)

    Colorize of (0.5, 0.5, 0.5, 1)

    Offset of (0, 0, 0)

___
### Color.TearAlmond {: aria-label='Constants' }
#### [Color](Color.md) TearAlmond {: .copyable aria-label='Constants' }
Used for tears fired by players with Soy Milk.

???- info "Info"
    Color of (1.8, 1.7, 1, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0, 0, 0)

___
### Color.TearChocolate {: aria-label='Constants' }
#### [Color](Color.md) TearChocolate {: .copyable aria-label='Constants' }
Used for tears fired by players with Chocolate Milk.

???- info "Info"
    Color of (0.33, 0.18, 0.18, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0.258824, 0.156863, 0.156863)

___
### Color.TearCoal {: aria-label='Constants' }
#### [Color](Color.md) TearCoal {: .copyable aria-label='Constants' }
Used for tears fired by players with A Lump of Coal.

???- info "Info"
    Color of (0.2, 0.09, 0.065, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0, 0, 0)

___
### Color.TearCommonCold {: aria-label='Constants' }
#### [Color](Color.md) TearCommonCold {: .copyable aria-label='Constants' }
Used for poison tears fired by players with Common Cold.

???- info "Info"
    Color of (0.4, 0.97, 0.5, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0, 0, 0)

___
### Color.TearHoming {: aria-label='Constants' }
#### [Color](Color.md) TearHoming {: .copyable aria-label='Constants' }
Used for homing tears fired by players with Spoon Bender.

???- info "Info"
    Color of (0.4, 0.15, 0.38, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0.278431, 0, 0.454902)

___
### Color.TearIpecac {: aria-label='Constants' }
#### [Color](Color.md) TearIpecac {: .copyable aria-label='Constants' }
Used for explosive tears fired by players with Ipecac.

???- info "Info"
    Color of (0.5, 0.9, 0.4, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0, 0, 0)

___
### Color.TearNumberOne {: aria-label='Constants' }
#### [Color](Color.md) TearNumberOne {: .copyable aria-label='Constants' }
Used for tears fired by players with Number One.

???- info "Info"
    Color of (1, 1, 0, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0.176471, 0.0588235, 0)

___
### Color.TearScorpio {: aria-label='Constants' }
#### [Color](Color.md) TearScorpio {: .copyable aria-label='Constants' }
Used for poison tears fired by players with Scorpio.

???- info "Info"
    Color of (0.196078, 1, 0.196078, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0, 0, 0)

___
### Color.TearSerpentsKiss {: aria-label='Constants' }
#### [Color](Color.md) TearSerpentsKiss {: .copyable aria-label='Constants' }
Used for poison tears fired by players with Serpent's Kiss.

???- info "Info"
    Color of (0.5, 0.97, 0.5, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0, 0, 0)

___
### Color.TearSoy {: aria-label='Constants' }
#### [Color](Color.md) TearSoy {: .copyable aria-label='Constants' }
Used for tears fired by players with Soy Milk.

???- info "Info"
    Color of (1.5, 2, 2, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (0, 0, 0)

___
### Color.TearTar {: aria-label='Constants' }
#### [Color](Color.md) TearTar {: .copyable aria-label='Constants' }
Used for tar tears fired by familiars like Little Gish.

???- info "Info"
    Color of (0.95, 0.8, 0.6, 1)

    Colorize of (0, 0, 0, 0)

    Offset of (-0.588235, -0.588235, -0.588235)

___
