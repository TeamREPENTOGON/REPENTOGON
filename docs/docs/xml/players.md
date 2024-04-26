---
tags:
  - File
---
# File "players.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new characters.

With REPENTOGON, the vanilla HP variables can be omitted and fully replaced by our more advanced variables.

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|items|string|This now supports modded items by using the names of the items instead of the ids. Same format as in vanilla, a comma separated list of values|
|heartcontainers|int|The amount of empty heart containers the character should start with. 1 unit = 1/2 a heart|
|redhearts|int|The amount of red hearts the character should start with. 1 unit = 1/2 a heart|
|soulhearts|int|The amount of soul hearts the character should start with. 1 unit = 1/2 a heart|
|blackhearts|int|The amount of black hearts the character should start with. These do not replace soul hearts and are applied **after** soul hearts. 1 unit = 1/2 a heart|
|goldenhearts|int|The amount of golden hearts the character should start with. 1 unit = 1 golden heart|
|bonehearts|int|The amount of empty bone hearts the character should start with. 1 unit = 1 bone heart|
|eternalheart|int|If a character should start with an eternal heart. 1 = yes|
|brokenhearts|int|The amount of broken hearts the character should start with. 1 unit = 1 broken heart|
|rottenhearts|int|The amount of rotten hearts the character should start with. 1 unit = 1 rotten heart|
|healthtype|int|The health type the player should start with. The int corresponds to the [HealthType](../enums/HealthType.md) enum.|
|healthlimit|int|A maximum amount of HP the character should be able to have. 1 unit = 1/2 heart|
|speedmodifier|float|An inherent offset to the speed stat the character should start with. Base this offset off of Isaac's stats.|
|firedelaymodifier|float|An inherent offset to the fire delay stat the character should start with. Base this offset off of Isaac's stats.|
|damagemodifier|float|An inherent offset to the damage stat the character should start with. Base this offset off of Isaac's stats.|
|rangemodifier|float|An inherent offset to the range stat the character should start with. Base this offset off of Isaac's stats.|
|shotspeedmodifier|float|An inherent offset to the shot speed stat the character should start with. Base this offset off of Isaac's stats.|
|luckmodifier|float|An inherent offset to the luck stat the character should start with. Base this offset off of Isaac's stats.|
|gigabombs|int|The amount of giga bombs the character should start with. These do not replace normal bombs.|
|completionparent|string|Used for characters that have shared completion marks with a preexistent character, like Dark Judas and Judas. The name of the parent character should be used for this attribute|
|nomarks|bool|if it has any value other than 'false', it will hide the completion marks for the given character|
|achievement|int or string|This is now respected on modded characters. For mods, it's best to define this as the *name* of the achievement (for example, `achievement='TaintedPeter'`, TaintedPeter being the name attribute of the achievement). Characters with this tag should have a second frame in their `characterportraits(alt).anm2` which should be the "locked" picture for the character. In addition, all stat/item layers on the main menu will be hidden and the `Unlocked By` layer will be visible. See the [achievements XML section](achievements.md) for more info.|
|modcostume|int|Set this to match the "id" attribute of a costume with type="none" defined in the same mod (in [costumes2.xml](costumes.md)) to have that costume permanantly applied to the character (including during the mineshaft chase sequence). You cannot set both this and the "costume" attribute for vanilla costumes at the same time.|

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/players.html).
