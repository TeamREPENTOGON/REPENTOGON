---
tags:
  - File
---
# File "items.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new items.

## Items n Trinkets
With REPENTOGON, the following xml attributes have changed functionality for regular items and trinkets:

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|achievement|int|Ties the item/trinket to be unlocked by anachievement. For modded ones, use the provided achievement name xml attribute(define one if it doesnt have one already). |

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

## CustomTags

Repentogon adds a new `customtags` attribute that can contain a space-separated list of strings that can then be checked in-game. Mods can freely add arbitrary string tags to this attribute in the XML and check if they were added to an item in-game. This can be used for something similar to vanilla's [item tags](https://wofsauge.github.io/IsaacDocs/rep/enums/ItemConfig.html) without needing to individually list each item in lua code, and mods can also use `customtags` from other mods in their own XMLs for easier mod compatability.

Supported for all item types (collectibles, trinkets, and null items).

Capitalization does not matter and is ignored entirely when adding or checking for `customtags`.

Example XML:
```xml
customtags="yourcustomtag nometronome"
```

`customtags` can be checked in-game via the item's [ItemConfigItem](https://repentogon.com/ItemConfig_Item.html):

Example Code:
```lua
local item = Isaac.GetItemConfig():GetCollectible(id)

-- Returns true if the entity has the tag string specified.
-- Capitalization does not matter.
if item:HasCustomTag("yourcustomtag") then
  -- ...
end

-- Returns a table containing all customtags specified for this entity.
-- Tags are provided in all lowercase.
local customTags = item:GetCustomTags()
```

Repentogon itself adds some tags that can be used to apply certain features to items with little to no required lua code at all:

| customtag | Effects |
|:--|:--|
| nometronome | Prevents the item from being picked by Metronome. |
| revive | Each copy of the collectible/trinket/null effect held is counted as an extra life (on the HUD, etc) and prevents the game from deleting the run save upon death, making reviving the player safe. EntityPlayer functions such as `player:GetExtraLives()` and `player:WillPlayerRevive()` respect this. Note that this tag does NOT automatically revive the player, you are still responsible for removing your item/effect and reviving the player when desired, either by calling `player:Revive()` or returning false on either the `MC_PRE_TRIGGER_PLAYER_DEATH` or `MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES` callbacks. See below for some sample code. |
| reviveeffect | Same as "revive" but instead applies to the corresponding TemporaryEffect of the collectible or trinket. You may want to specify `persistent="true"` on the collectible/trinket to make its TemporaryEffect persist between rooms. There is no difference between "revive" and "reviveeffect" for null items. |
| chancerevive | If the "revive" or "reviveeffect" tag is present, also displays a "?" next to the extra life count, like Guppy's Collar. |
| hiddenrevive | If the "revive" or "reviveeffect" tag is present, this prevents it from being counted on the hud. Overrides "chancerevive". |

Sample revive XML:
```xml
<passive name="Revive Item" description="test" gfx="image.png" customtags="revive" />
<trinket name="Trinket whose TemporaryEffect grants a persistent revive, but...?" description="test" gfx="image.png" persistent="true" customtags="reviveeffect chancerevive" />
<null name="Null Item that counts as a revive, but is not shown on the HUD" persistent="true" customtags="revive hiddenrevive" />
```

Sample simple revive code:
```lua
-- "Real" held collectible-based revive with higher priority than all vanilla revives, like 1UP.
mod:AddCallback(ModCallbacks.MC_PRE_TRIGGER_PLAYER_DEATH, function(_, player)
	if player:HasCollectible(yourItemID, true) then
		player:RemoveCollectible(yourItemID)
		player:SetMinDamageCooldown(120)  -- Grant iframes to the player.
		return false  -- Causes the player to revive on the spot with half a heart.
	end
end)

-- Null Item-based revive effect with lower priority than vanilla revives such as 1UP.
-- Null Items (or any item TemporaryEffect) can be a good way to handle multiple extra lives.
local nullID = Isaac.GetNullItemIdByName("my null item")
mod:AddCallback(ModCallbacks.MC_TRIGGER_PLAYER_DEATH_POST_CHECK_REVIVES, function(_, player)
	if player:GetEffects():HasNullEffect(nullID) then
		player:GetEffects():RemoveNullEffect(nullID)
		player:Revive()  -- Calling Revive manually like this is acceptable, and will still prevent later callbacks from running.
		-- You can manipulate the player's post-revive health here, such as fully healing them, or setting them to a single red heart container.
	end
end)
```

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/items.html). 
