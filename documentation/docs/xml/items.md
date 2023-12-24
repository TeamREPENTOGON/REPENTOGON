---
tags:
  - File
---
# File "items.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new items.

## "Null" Items

With REPENTOGON, you can now define custom "null" items. "Null" items are not typical items that can be spawned or visually seen in-game. They are typically used by the game to handle various costumes, buffs and effects (both temporary and persistent).

For example, touching the white fire in downpour/dross actually grants a null item effect to you that applies the costume/flight/effects ([NullItemID](https://wofsauge.github.io/IsaacDocs/rep/enums/NullItemID.html).ID_LOST_CURSE) and removes itself when exiting the mirror dimension. Obtaining a transformation adds a null item effect to the player that grants the actual costume/effects of the transformation (such as [NullItemID](https://wofsauge.github.io/IsaacDocs/rep/enums/NullItemID.html).ID_GUPPY) which may remove itself if the transformation's requirements are lost.

Null items can be very useful for implementing temporary or persistent effects that can be freely added or removed.

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|id|int|Assign the same "id" to a null item and a corresponding costume with type="none" costume in [costumes2.xml](costumes.md) to have the null item automatically apply that costume, similarly to what could already be done for typical item costumes.<br /><br />Note that the id used must be at least 1. This id is not the one that will be used in-game, and is only for associating the null item with a costume.|
|cache|string|Can be used to specify stats that should be re-evaluated when the effect of the null item is added or removed, same as regular items.<br /><br />Possible values : [firedelay, damage, speed, range, tearcolor, tearflag, color, size. shotspeed, all, luck, flying, weapon, familiars]|
|persistent|bool|By default null item effects only last for the current room. Set persistent to true to have the effect persist between rooms.|
|cooldown|int|If set, defines how long the effect of the null item lasts, in update frames (30 frames ~= 1 second). After this duration, the effect removes itself.|


Null item examples:
```xml
<!-- Null item whose effect stays on the player permanantly, unless removed. -->
<null id="1" name="my null item 1" persistent="true" />

<!-- Null item that triggers a damage & tears cache evaluation when added or removed. Only lasts for the current room. -->
<null id="2" name="my null item 2" cache="damage firedelay" />

<!-- Null item whose effect lasts for 10 seconds, then removes itself. -->
<null id="3" name="my null item 3" cooldown="300" persistent="true" />
```

You can use `Isaac.GetNullItemIdByName("my null item")` to get the in-game ID of your null item.

Additionally, you can use the functions in the [TemporaryEffects](https://wofsauge.github.io/IsaacDocs/rep/TemporaryEffects.html) class to add, remove, count, or check the current cooldown of null item effects on the player.

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/items.html). 
