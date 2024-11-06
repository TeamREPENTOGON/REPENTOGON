---
tags:
  - Enum
---
# Enum "ImGuiWindowFlags"
|Value|Enumerator|Comment|
|:--|:--|:--|
|0 |None {: .copyable } |  |
|1 << 0 |NoTitleBar {: .copyable } | Disable title-bar |
|1 << 1 |NoResize {: .copyable } | Disable user resizing with the lower-right grip |
|1 << 2 |NoMove {: .copyable } | Disable user moving the window |
|1 << 3 |NoScrollbar {: .copyable } | Disable scrollbars (window can still scroll with mouse or programmatically) |
|1 << 4 |NoScrollWithMouse {: .copyable } | Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set. |
|1 << 5 |NoCollapse {: .copyable } | Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node). |
|1 << 6 |AlwaysAutoResize {: .copyable } | Resize every window to its content every frame |
|1 << 7 |NoBackground {: .copyable } | Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f). |
|1 << 8 |NoSavedSettings {: .copyable } | Never load/save settings in .ini file |
|1 << 9 |NoMouseInputs {: .copyable } | Disable catching mouse, hovering test with pass through |
|1 << 10 |MenuBar {: .copyable } | Has a menu-bar |
|1 << 11 |HorizontalScrollbar {: .copyable } | Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section. |
|1 << 12 |NoFocusOnAppearing {: .copyable } | Disable taking focus when transitioning from hidden to visible state |
|1 << 13 |NoBringToFrontOnFocus {: .copyable } | Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus) |
|1 << 14 |AlwaysVerticalScrollbar {: .copyable } | Always show vertical scrollbar (even if ContentSize.y < Size.y) |
|1 << 15 |AlwaysHorizontalScrollbar {: .copyable } | Always show horizontal scrollbar (even if ContentSize.x < Size.x) |
|1 << 16 |NoNavInputs {: .copyable } | No gamepad/keyboard navigation within the window |
|1 << 17 |NoNavFocus {: .copyable } | No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB) |
|1 << 18 |UnsavedDocument {: .copyable } | Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed |
|(1 << 16) | (1 << 17) |NoNav {: .copyable } | ImGuiWindowFlags.NoNavInputs &#124; ImGuiWindowFlags.NoNavFocus |
|(1 << 0) | (1 << 1) | (1 << 3) | (1 << 5) |NoDecoration {: .copyable } | ImGuiWindowFlags.NoTitleBar &#124; ImGuiWindowFlags.NoResize &#124; ImGuiWindowFlags.NoScrollbar &#124; ImGuiWindowFlags.NoCollapse |
|(1 << 9) | (1 << 16) | (1 << 17) |NoInputs {: .copyable } | ImGuiWindowFlags.NoMouseInputs &#124; ImGuiWindowFlags.NoNavInputs &#124; ImGuiWindowFlags.NoNavFocus |
