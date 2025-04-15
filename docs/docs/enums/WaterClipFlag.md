---
tags:
  - Enum
---
# Enum "WaterClipFlag"
???+ tip "Bitset Calculator"
    [](#){: .bitsetCalculator }
|Value|Enumerator|Comment|
|:--|:--|:--|
|1 << 0 |  | (Unused) |
|1 << 1 |DISABLE_RENDER_ABOVE_WATER {: .copyable } | When set on an Entity, also enables rendering below water. |
|1 << 2 |ENABLE_RENDER_BELOW_WATER {: .copyable } | Entity only. Allows the "below water" rendering to happen at the same time as "above water". |
|1 << 3 |DISABLE_RENDER_BELOW_WATER {: .copyable } | Takes priority over other flags. |
|1 << 4 |  | (Unused) |
|1 << 5 |DISABLE_RENDER_REFLECTION {: .copyable } | Entity only (GridEntities can't have reflections). |
|1 << 6 |IGNORE_WATER_RENDERING {: .copyable } | Entity only. Overrides other flags so the entity renders above water only, with no reflection. |
|1 << 7 |FORCE_WATER_RIPPLE_WHEN_MOVING {: .copyable } | Allows Entities with no collision to still spawn water ripples when moving. |
