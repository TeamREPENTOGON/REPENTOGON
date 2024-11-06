---
tags:
  - Enum
---
# Enum "ImGuiChildFlags"
|Value|Enumerator|Comment|
|:--|:--|:--|
|0 |None {: .copyable } |  |
|1 << 0 |Border {: .copyable } | Show an outer border and enable WindowPadding. (IMPORTANT: this is always == 1 == true for legacy reason) |
|1 << 1 |AlwaysUseWindowPadding {: .copyable } | Pad with style.WindowPadding even if no border are drawn (no padding by default for non-bordered child windows because it makes more sense) |
|1 << 2 |ResizeX {: .copyable } | Allow resize from right border (layout direction). Enable .ini saving (unless ImGuiWindowFlags_NoSavedSettings passed to window flags) |
|1 << 3 |ResizeY {: .copyable } | Allow resize from bottom border (layout direction). |
|1 << 4 |AutoResizeX {: .copyable } | Enable auto-resizing width. Read "IMPORTANT: Size measurement" details above. |
|1 << 5 |AutoResizeY {: .copyable } | Enable auto-resizing height. Read "IMPORTANT: Size measurement" details above. |
|1 << 6 |AlwaysAutoResize {: .copyable } | Combined with AutoResizeX/AutoResizeY. Always measure size even when child is hidden, always return true, always disable clipping optimization! NOT RECOMMENDED. |
|1 << 7 |FrameStyle {: .copyable } | Style the child window like a framed item: use FrameBg, FrameRounding, FrameBorderSize, FramePadding instead of ChildBg, ChildRounding, ChildBorderSize, WindowPadding. |
