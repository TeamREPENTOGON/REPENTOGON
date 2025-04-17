---
tags:
  - Enum
---
# Enum "GetCollectibleFlag"
???+ tip "Bitset Calculator"
    [](#){: .bitsetCalculator }
|Value|Enumerator|Comment|
|:--|:--|:--|
|1 << 0 |BAN_ACTIVES {: .copyable } | Bans items with [Type](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html?h=itemconfig#type): `ITEM_ACTIVE`. |
|1 << 1 |IGNORE_MODIFIERS {: .copyable } | If set the game does not attempt to morph the collectible into either `Magic Skin` or `Rosary`. Does not prevent a morph into `Bible`. |
|1 << 2 |BAN_PASSIVES {: .copyable } | Only allows items with [Type](https://wofsauge.github.io/IsaacDocs/rep/ItemConfig_Item.html?h=itemconfig#type): `ITEM_ACTIVE`. |
