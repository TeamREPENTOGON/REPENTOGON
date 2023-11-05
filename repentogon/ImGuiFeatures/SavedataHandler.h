#pragma once
#include "ImGuiEx.h"

std::vector<CustomImGuiWindowSettings*> SettingsList;

// custom implementation to support custom data structure
CustomImGuiWindowSettings* CreateNewWindowSettingsEx(const char* name)
{
  ImGuiContext& g = *GImGui;

  if (g.IO.ConfigDebugIniSettings == false)
  {
    // Skip to the "###" marker if any. We don't skip past to match the behavior of GetID()
    // Preserve the full string when ConfigDebugVerboseIniSettings is set to make .ini inspection easier.
    if (const char* p = strstr(name, "###"))
      name = p;
  }
  const size_t name_len = strlen(name);

  // Allocate chunk
  CustomImGuiWindowSettings* settings = new CustomImGuiWindowSettings();
  settings->ID = ImHashStr(name, name_len);
  settings->Name = std::string(name);
  SettingsList.push_back(settings);
  return settings;
}

// custom implementation to support custom data structure
CustomImGuiWindowSettings* FindWindowSettingsByIDEx(ImGuiID id)
{
  for (auto* settings : SettingsList)
    if (settings->ID == id && !settings->WantDelete)
      return settings;
  return NULL;
}

static void WindowSettingsHandler_ClearAll(ImGuiContext* ctx, ImGuiSettingsHandler*)
{
  ImGuiContext& g = *ctx;
  for (int i = 0; i != g.Windows.Size; i++)
    g.Windows[i]->SettingsOffset = -1;
  SettingsList.clear();
}

static void* WindowSettingsHandler_ReadOpen(ImGuiContext*, ImGuiSettingsHandler*, const char* name)
{
  ImGuiID id = ImHashStr(name);
  CustomImGuiWindowSettings* settings = FindWindowSettingsByIDEx(id);
  if (!settings)
    settings = CreateNewWindowSettingsEx(name);
  settings->ID = id;
  settings->WantApply = true;
  return (void*)settings;
}

static void WindowSettingsHandler_ReadLine(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line)
{
  CustomImGuiWindowSettings* settings = (CustomImGuiWindowSettings*)entry;
  int x, y;
  int i;
  float f;
  if (sscanf(line, "Pos=%i,%i", &x, &y) == 2) { settings->Pos = ImVec2ih((short)x, (short)y); }
  else if (sscanf(line, "Size=%i,%i", &x, &y) == 2) { settings->Size = ImVec2ih((short)x, (short)y); }
  else if (sscanf(line, "Collapsed=%d", &i) == 1) { settings->Collapsed = (i != 0); }
  //Repentogon additions
  else if (sscanf(line, "Visible=%d", &i) == 1) { settings->Visible = (i != 0); }
  else if (sscanf(line, "Pinned=%d", &i) == 1) { settings->Pinned = (i != 0); }
  else if (sscanf(line, "FontWindowScale=%f", &f) == 1) { settings->FontWindowScale = f; }
}

// altered version from imgui.cpp . Applies vanilla Window settings to the window
static void ApplyWindowSettings(ImGuiWindow* window, CustomImGuiWindowSettings* settings)
{
  window->Pos = ImFloor(ImVec2(settings->Pos.x, settings->Pos.y));
  if (settings->Size.x > 0 && settings->Size.y > 0)
    window->Size = window->SizeFull = ImFloor(ImVec2(settings->Size.x, settings->Size.y));
  window->Collapsed = settings->Collapsed;
  //Repentogon additions
  window->FontWindowScale = settings->FontWindowScale;
}

// Apply to existing windows (if any)
static void WindowSettingsHandler_ApplyAll(ImGuiContext* ctx, ImGuiSettingsHandler*)
{
  for (auto* settings : SettingsList) {
    if (settings->WantApply)
    {
      if (ImGuiWindow* window = ImGui::FindWindowByID(settings->ID)) {
        ApplyWindowSettings(window, settings);
        settings->WantApply = false;
      }
    }
  }
}

static void WindowSettingsHandler_WriteAll(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf)
{
  // Gather data from windows that were active during this session
  // (if a window wasn't opened in this session we preserve its settings)
  ImGuiContext& g = *ctx;
  for (int i = 0; i != g.Windows.Size; i++)
  {
    ImGuiWindow* window = g.Windows[i];
    if (window->Flags & ImGuiWindowFlags_NoSavedSettings)
      continue;

    CustomImGuiWindowSettings* settings = FindWindowSettingsByIDEx(window->ID);
    if (!settings)
    {
      settings = CreateNewWindowSettingsEx(window->Name);
    }
    IM_ASSERT(settings->ID == window->ID);
    settings->Pos = ImVec2ih(window->Pos);
    settings->Size = ImVec2ih(window->SizeFull);

    settings->Collapsed = window->Collapsed;
    settings->WantDelete = false;
    //Repentogon additions
    settings->Visible = window->Active;
    settings->FontWindowScale = window->FontWindowScale;
  }

  // Write to text buffer
  for (auto* settings : SettingsList)
  {
    if (settings->WantDelete)
      continue;
    const char* settings_name = settings->Name.c_str();
    buf->appendf("[%s][%s]\n", handler->TypeName, settings_name);
    buf->appendf("Pos=%d,%d\n", settings->Pos.x, settings->Pos.y);
    buf->appendf("Size=%d,%d\n", settings->Size.x, settings->Size.y);
    buf->appendf("Collapsed=%d\n", settings->Collapsed);
    //Repentogon additions
    buf->appendf("Visible=%d\n", settings->Visible);
    buf->appendf("Pinned=%d\n", settings->Pinned);
    buf->appendf("FontWindowScale=%f\n", settings->FontWindowScale);
    buf->append("\n");
  }
}

void RegisterSaveDataHandler() {
  ImGui::RemoveSettingsHandler("Window"); // remove original save handler
  ImGuiSettingsHandler ini_handler;
  ini_handler.TypeName = "Window";
  ini_handler.TypeHash = ImHashStr("Window");
  ini_handler.ClearAllFn = WindowSettingsHandler_ClearAll;
  ini_handler.ReadOpenFn = WindowSettingsHandler_ReadOpen;
  ini_handler.ReadLineFn = WindowSettingsHandler_ReadLine;
  ini_handler.ApplyAllFn = WindowSettingsHandler_ApplyAll;
  ini_handler.WriteAllFn = WindowSettingsHandler_WriteAll;
  ImGui::AddSettingsHandler(&ini_handler);
}