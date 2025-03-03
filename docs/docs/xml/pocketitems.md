---
tags:
  - File
---
# File "pocketitems.xml"

**Resource-Folder**{: .xmlInfo .red}: Using this file in a resource folder of a mod will replace the original file.

**Content-Folder**{: .xmlInfo .green }: Using this file in a content folder will add new pocketitems.

## Cards

With REPENTOGON, the following xml attributes have been added for cards: 

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|hidden|bool|`Optional` `Default = false`<br />Prevents the card from being randomly chosen by the game or when using the [ItemPool:GetCard()](https://wofsauge.github.io/IsaacDocs/rep/ItemPool.html?h=getcard#getcard) and [ItemPool:GetCardEx()](https://repentogon.com/ItemPool.html?h=card+ex#getcardex) functions.|
|weight|float|`Optional` `Default = 1.0`<br />Relative "likelihood" for this card to be chosen over others of the same [card type](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Card.html?h=itemconfi#cardtype) pool.<br />Does not alter the chance for the specific card type pool to be chosen.|

## Cards & Pill Effects

With REPENTOGON, the following xml attributes have changed functionality for regular cards and pills: 

| Variable-Name | Possible Values | Description |
|:--|:--|:--|
|achievement|int or string|Ties the card/pill-effect to be unlocked by an achievement. For modded ones, use the provided achievement name xml attribute(define one if it doesn't have one already). |

???+ note "More Info"
    For more information about this xml, check the original docs entry [here](https://wofsauge.github.io/IsaacDocs/rep/xml/pocketitems.html).
