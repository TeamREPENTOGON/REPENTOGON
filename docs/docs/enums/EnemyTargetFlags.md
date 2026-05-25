---
tags:
  - Enum
---
# Enum "WaterClipFlag"
???+ tip "Bitset Calculator"
    [](#){: .bitsetCalculator }
|Value|Enumerator|Comment|
|:--|:--|:--|
|1 << 0 |CAN_CHANGE_TARGET {: .copyable } | Allow switching to a better target even if we already have one. |
|1 << 1 |AVOID_CLOSE_ENEMIES {: .copyable } | Don't prioritize enemies close to our owner. |
|1 << 2 |PRIORITIZE_HIGH_HP {: .copyable } | Prioritize enemies with higher HP. |
|1 << 3 |PRIORITIZE_LOW_HP {: .copyable } | Prioritize enemies with lower HP. |
|1 << 4 |DEPRIORITIZE_CURRENT_TARGET {: .copyable } | Give lower priority to our current target (makes switching targets more likely). |
