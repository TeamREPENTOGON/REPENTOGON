#pragma once
#include "ImGuiEx.h"
#include "IsaacRepentance.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include <string>

extern int handleWindowFlags(int flags);
extern void HelpMarker(const char* desc);
extern bool WindowBeginEx(const char* name, bool* p_open, ImGuiWindowFlags flags);
extern bool imguiResized;
extern ImVec2 imguiSizeModifier;

struct GameOptionsWindow : ImGuiWindowObject {
    GameOptionsWindow()
        : ImGuiWindowObject("Game Options")
    {
    }
    const char* extraHudModes[3] = { "Off", "Normal", "Mini" };
    const char* announcerModes[3] = { "Random", "Off", "Always on" };
    const char* consoleFontModes[3] = { "Default", "Small", "Tiny" };
    const char* offOnModes[2] = { "Off", "On" };

    void Draw(bool isImGuiActive)
    {
        if (!enabled || !isImGuiActive && !pinned) {
            return;
        }
        if (WindowBeginEx(windowName.c_str(), &enabled, handleWindowFlags(0))) {
            if (imguiResized) {
                ImGui::SetWindowPos(ImVec2(ImGui::GetWindowPos().x * imguiSizeModifier.x, ImGui::GetWindowPos().y * imguiSizeModifier.y));
                ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x * imguiSizeModifier.x, ImGui::GetWindowSize().y * imguiSizeModifier.y));
            }
            AddWindowContextMenu();
            if (ImGui::BeginTabBar("GameOptionsTabBar", ImGuiTabBarFlags_None)) {
                if (ImGui::BeginTabItem("Options")) {
                    ImGui::SliderFloat("SFX Volume", &g_Manager->GetOptions()->_sfxVolume, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
                    ImGui::SliderFloat("Music Volume", &g_Manager->GetOptions()->_musicVolume, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
                    ImGui::SliderFloat("Map Opacity", &g_Manager->GetOptions()->_mapOpacity, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
                    // ImGui::Checkbox("Fullscreen", &g_Manager->GetOptions()->_isFullscreen); // value gets set somewhere else
                    ImGui::Checkbox("Filter", &g_Manager->GetOptions()->_filterEnabled);
                    ImGui::SameLine();
                    HelpMarker("Applies a shader that makes the game look no longer pixelated. Can cause issues with text rendering on screen!");
                    ImGui::Checkbox("Pop-Ups", &g_Manager->GetOptions()->_popupsEnabled);
                    ImGui::SliderInt("Announcer Voice Mode", &g_Manager->GetOptions()->_announcerVoiceMode, 0, 2, announcerModes[g_Manager->GetOptions()->_announcerVoiceMode], ImGuiSliderFlags_NoInput);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("HUDs")) {
                    ImGui::SliderFloat("HUD Offset", &g_Manager->GetOptions()->_hudOffset, 0.0f, 1.0f, "%.2f");
                    ImGui::Checkbox("Found HUD", &g_Manager->GetOptions()->_enableFoundHUD);
                    ImGui::SameLine();
                    HelpMarker("Displays the stats of the player on the left side of the screen.");
                    ImGui::Checkbox("Chargebars", &g_Manager->GetOptions()->_enableChargeBars);
                    ImGui::Checkbox("Boss Healthbar on bottom", &g_Manager->GetOptions()->_bossHpOnBottom);
                    ImGui::SliderInt("Extra Hud", &g_Manager->GetOptions()->_historyHudMode, 0, 2, extraHudModes[g_Manager->GetOptions()->_historyHudMode], ImGuiSliderFlags_NoInput);
                    ImGui::SameLine();
                    HelpMarker("Displays collected items of the player on the right side of the screen.");

                    ImGui::SeparatorText("Console");
                    ImGui::Checkbox("Enable Debug Console", &g_Manager->GetOptions()->_enableDebugConsole);
                    ImGui::Checkbox("Faded Console Display", &g_Manager->GetOptions()->_enableFadedConsoleDisplay);
                    ImGui::Checkbox("Save Command History", &g_Manager->GetOptions()->_enableSaveCommandHistory);
                    ImGui::SliderInt("Console Font", &g_Manager->GetOptions()->_consoleFont, 0, 2, consoleFontModes[g_Manager->GetOptions()->_consoleFont], ImGuiSliderFlags_NoInput);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Visual Effects")) {
                    ImGui::SliderFloat("Gamma", &g_Manager->GetOptions()->_gamma, 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat("Exposure", &g_Manager->GetOptions()->_exposure, 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat("Brightness", &g_Manager->GetOptions()->_brightness, 0.0f, 1.0f, "%.2f");
                    ImGui::SliderFloat("Contrast", &g_Manager->GetOptions()->_contrast, 0.0f, 1.0f, "%.2f");
                    ImGui::InputInt("Max Scale", &g_Manager->GetOptions()->_maxScale, 1);
                    ImGui::InputInt("Max Render Scale", &g_Manager->GetOptions()->_maxRenderScale, 1);
                    ImGui::SliderInt("Active Camera", &g_Manager->GetOptions()->_cameraStyle, 0, 1, offOnModes[g_Manager->GetOptions()->_cameraStyle], ImGuiSliderFlags_NoInput);
                    ImGui::Checkbox("VSync", &g_Manager->GetOptions()->_enableVSync);
                    ImGui::Checkbox("Borderless Fullscreen", &g_Manager->GetOptions()->_enableBorderlessFullscreen);
                    ImGui::SeparatorText("Effects");
                    ImGui::Checkbox("Animation Interpolation", &g_Manager->GetOptions()->_enableInterpolation);
                    ImGui::Checkbox("Color Correction", &g_Manager->GetOptions()->_enableColorCorrection);
                    ImGui::Checkbox("Color Modifier", &g_Manager->GetOptions()->_enableColorModifier);
                    ImGui::Checkbox("Lighting", &g_Manager->GetOptions()->_enableLighting);
                    ImGui::Checkbox("Shockwaves", &g_Manager->GetOptions()->_enableShockwave);
                    ImGui::Checkbox("Caustics", &g_Manager->GetOptions()->_enableCaustics);
                    ImGui::Checkbox("Pixelation", &g_Manager->GetOptions()->_enablePixelation);
                    ImGui::SameLine();
                    HelpMarker("e.g. Retro vision pill");
                    ImGui::Checkbox("Bloom", &g_Manager->GetOptions()->_enableBloom);
                    ImGui::Checkbox("Enable Water Surface rendering", &g_Manager->GetOptions()->_getsSetToEnableWaterSurface);
                    ImGui::SameLine();
                    HelpMarker("Needs to be set when changing Water Surface setting");
                    ImGui::Checkbox("Water Surface", &g_Manager->GetOptions()->_enableWaterSurface);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Accessibility")) {
                    ImGui::Checkbox("Aim Lock", &g_Manager->GetOptions()->_enableAimLock);
                    ImGui::Checkbox("Rumble", &g_Manager->GetOptions()->_enableRumble);
                    ImGui::Checkbox("Mouse Control", &g_Manager->GetOptions()->_enableMouseControl);
                    ImGui::Checkbox("Bullet Visibility", &g_Manager->GetOptions()->_enableBulletVisibility);
                    ImGui::SliderInt("Touch Mode", &g_Manager->GetOptions()->_touchMode, 0, 1, offOnModes[g_Manager->GetOptions()->_touchMode], ImGuiSliderFlags_NoInput);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Misc")) {
                    ImGui::Checkbox("Enable Mods", &g_Manager->GetOptions()->_enableMods);
                    ImGui::Checkbox("Enable Control Hotplug", &g_Manager->GetOptions()->_controlHotplugEnabled);
                    ImGui::Checkbox("Use Steam Cloud", &g_Manager->GetOptions()->_enableSteamCloud);
                    ImGui::Checkbox("Pause on focus lost", &g_Manager->GetOptions()->_enablePauseOnFocusLost);
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }

            ImGui::End(); // close window element
        }
    }
};

extern GameOptionsWindow gameOptionsWindow;
