---
tags:
  - File
---
# File "entities2.xml"

**Resource-Folder**{: .xmlInfo .red }: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new entities.

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
| coinvalue | int | How much this coin pickup is worth when using GetCoinValue (either by the game or a lua call). |
| customtags | string | Space-separated list of strings. See "CustomTags" section below. |
| nosplit | boolean | Allows preventing this NPC from being split by Meat Cleaver. |

## CustomTags

Repentogon adds a new `customtags` attribute that can contain a space-separated list of strings that can then be checked in-game. Mods can freely add arbitrary string tags to this attribute in the XML and check if they were added to an entity in-game. This makes it easier for mods to add common entity behaviour/traits similar to the existing [EntityTag](https://repentogon.com/enums/EntityTag.html)s without needing to individually list each entity in lua code, and mods can also use `customtags` from other mods in their own XMLs for easier mod compatability.

Capitalization does not matter and is ignored entirely when adding or checking for `customtags`.

Example XML:
```
customtags="yourcustomtag familiarignorebffs familiarblockprojectiles"
```

`customtags` can be checked in-game using the [EntityConfig](https://repentogon.com/EntityConfig.html):

Example Code:
```
local entConfig = EntityConfig.GetEntity(entType, entVar, entSub)

-- Returns true if the entity has the tag string specified.
-- Capitalization does not matter.
if entConfig:HasCustomTag("yourcustomtag") then
  -- ...
end

-- Returns a table containing all customtags specified for this entity.
-- Tags are provided in all lowercase.
local customTags = entConfig:GetCustomTags()
```

Repentogon itself adds some tags that can be used to apply certain behaviours to custom familiars with no required lua code at all:

| customtag | Effects |
|:--|:--|
| familiarignorebffs | The BFFS item will no longer affect this familiar. |
| familiarcantakedamage | Familiars with baseHP above 0 will be able to take damage and die, such as from enemy contact, lasers or explosions. Note that they will only take damage from projectiles if they also have the "familiarblockprojectiles" tag. |
| familiarblockprojectiles | The familiar automatically destroys enemy projectiles on contact. |

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/entities2.html). 
