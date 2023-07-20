---
tags:
  - Globals
  - Class
search:
  boost: 0.25
---
# Class "ProjectileParams"
## Variables
### Damage {: aria-label='Variables' }
[ ](#){: .tooltip .badge }
#### float Damage {: .copyable aria-label='Variables'}

Number of half hearts of damage dealt by the projectile. Cannot be set to a negative value.

???+ info
    This value ignores the full heart damage modifier applied to projectiles with a [Scale](https://wofsauge.github.io/IsaacDocs/rep/ProjectileParams.html#scale) above `1.15`, but non-boss champions will still double it (with a caveat, read warning below!).
    

???+ warning "Warning"
    Enemies with a [ChampionColorIdx](https://wofsauge.github.io/IsaacDocs/rep/enums/ChampionColor.html) above `-1` will cap the damage at `2.0`!
___
