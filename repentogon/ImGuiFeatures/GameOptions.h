#pragma once
#include "../REPENTOGONOptions.h"
#include "IconsFontAwesome6.h"
#include "ImGuiEx.h"
#include "IsaacRepentance.h"
#include "imgui.h"

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

    template <typename T>
    void AddResetButton(int id, T& valueRef, T defaultValue)
    {
        bool disable = valueRef == defaultValue;
        if (disable) {
            ImGui::BeginDisabled();
        }
        ImGui::TableSetColumnIndex(1);
        ImGui::PushID("RESET_" + id);
        if (ImGui::SmallButton(ICON_FA_ROTATE_LEFT)) {
            valueRef = defaultValue;
        }
        ImGui::SetItemTooltip("Reset to default value");
        ImGui::PopID();
        if (disable) {
          ImGui::EndDisabled();
        }
    }
    void AddNewTableRow()
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
    }

    void Draw(bool isImGuiActive)
    {
        if (!enabled || !isImGuiActive && !pinned) {
            return;
        }
        ImGui::SetNextWindowSize(ImVec2(675, 375), ImGuiCond_FirstUseEver);

        if (WindowBeginEx(windowName.c_str(), &enabled, handleWindowFlags(0))) {
            if (imguiResized) {
                ImGui::SetWindowPos(ImVec2(ImGui::GetWindowPos().x * imguiSizeModifier.x, ImGui::GetWindowPos().y * imguiSizeModifier.y));
                ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x * imguiSizeModifier.x, ImGui::GetWindowSize().y * imguiSizeModifier.y));
            }
            AddWindowContextMenu();
            if (ImGui::BeginTabBar("GameOptionsTabBar", ImGuiTabBarFlags_None)) {
                int resetCounter = 0;
                if (ImGui::BeginTabItem("Options")) {
                    if (ImGui::BeginTable("Options_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::SliderFloat("SFX Volume", &g_Manager->GetOptions()->_sfxVolume, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_sfxVolume, 0.3f);
                        AddNewTableRow();
                        ImGui::SliderFloat("Music Volume", &g_Manager->GetOptions()->_musicVolume, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_musicVolume, 0.3f);
                        AddNewTableRow();
                        ImGui::SliderFloat("Map Opacity", &g_Manager->GetOptions()->_mapOpacity, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_mapOpacity, 0.3f);
                        AddNewTableRow();
                        // ImGui::Checkbox("Fullscreen", &g_Manager->GetOptions()->_isFullscreen); // value gets set somewhere else
                        ImGui::Checkbox("Filter", &g_Manager->GetOptions()->_filterEnabled);
                        ImGui::SameLine();
                        HelpMarker("Applies a shader that makes the game look no longer pixelated. Can cause issues with text rendering on screen!");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_filterEnabled, false);
                        AddNewTableRow();
                        ImGui::Checkbox("Pop-Ups", &g_Manager->GetOptions()->_popupsEnabled);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_popupsEnabled, true);
                        AddNewTableRow();
                        ImGui::SliderInt("Announcer Voice Mode", &g_Manager->GetOptions()->_announcerVoiceMode, 0, 2, announcerModes[g_Manager->GetOptions()->_announcerVoiceMode], ImGuiSliderFlags_NoInput);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_announcerVoiceMode, 0);

                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("HUDs")) {
                    if (ImGui::BeginTable("HUDs_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::SliderFloat("HUD Offset", &g_Manager->GetOptions()->_hudOffset, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_hudOffset, 1.0f);
                        AddNewTableRow();
                        ImGui::Checkbox("Found HUD", &g_Manager->GetOptions()->_enableFoundHUD);
                        ImGui::SameLine();
                        HelpMarker("Displays the stats of the player on the left side of the screen.");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableFoundHUD, false);
                        AddNewTableRow();
                        ImGui::Checkbox("Chargebars", &g_Manager->GetOptions()->_enableChargeBars);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableChargeBars, false);
                        AddNewTableRow();
                        ImGui::Checkbox("Boss Healthbar on bottom", &g_Manager->GetOptions()->_bossHpOnBottom);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_bossHpOnBottom, true);
                        AddNewTableRow();
                        ImGui::SliderInt("Extra Hud", &g_Manager->GetOptions()->_historyHudMode, 0, 2, extraHudModes[g_Manager->GetOptions()->_historyHudMode], ImGuiSliderFlags_NoInput);
                        ImGui::SameLine();
                        HelpMarker("Displays collected items of the player on the right side of the screen.");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_historyHudMode, 0);
                        AddNewTableRow();

                        ImGui::SeparatorText("Console");
                        AddNewTableRow();
                        ImGui::Checkbox("Enable Debug Console", &g_Manager->GetOptions()->_enableDebugConsole);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableDebugConsole, false);
                        AddNewTableRow();
                        ImGui::Checkbox("Faded Console Display", &g_Manager->GetOptions()->_enableFadedConsoleDisplay);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableFadedConsoleDisplay, false);
                        AddNewTableRow();
                        ImGui::Checkbox("Save Command History", &g_Manager->GetOptions()->_enableSaveCommandHistory);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableSaveCommandHistory, true);
                        AddNewTableRow();
                        ImGui::SliderInt("Console Font", &g_Manager->GetOptions()->_consoleFont, 0, 2, consoleFontModes[g_Manager->GetOptions()->_consoleFont], ImGuiSliderFlags_NoInput);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_consoleFont, 0);
                        AddNewTableRow();
                        ImGui::Checkbox("Enable unicode font", &repentogonOptions.enableUnifont);
                        ImGui::SameLine();
                        HelpMarker("For improved non-latin language support. Will somewhat increase memory usage.");
                        AddResetButton(++resetCounter, repentogonOptions.enableUnifont, true);

                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Visual Effects")) {
                    if (ImGui::BeginTable("Visual Effects_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::SliderFloat("Gamma", &g_Manager->GetOptions()->_gamma, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_gamma, 1.0f);
                        AddNewTableRow();
                        ImGui::SliderFloat("Exposure", &g_Manager->GetOptions()->_exposure, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_exposure, 1.0f);
                        AddNewTableRow();
                        ImGui::SliderFloat("Brightness", &g_Manager->GetOptions()->_brightness, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_brightness, 0.0f);
                        AddNewTableRow();
                        ImGui::SliderFloat("Contrast", &g_Manager->GetOptions()->_contrast, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_contrast, 1.0f);
                        AddNewTableRow();
                        ImGui::InputInt("Max Scale", &g_Manager->GetOptions()->_maxScale, 1);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_maxScale, 99);
                        AddNewTableRow();
                        ImGui::InputInt("Max Render Scale", &g_Manager->GetOptions()->_maxRenderScale, 1);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_maxRenderScale, 2);
                        AddNewTableRow();
                        ImGui::SliderInt("Active Camera", &g_Manager->GetOptions()->_cameraStyle, 0, 1, offOnModes[g_Manager->GetOptions()->_cameraStyle], ImGuiSliderFlags_NoInput);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_cameraStyle, 1);
                        AddNewTableRow();
                        ImGui::Checkbox("VSync", &g_Manager->GetOptions()->_enableVSync);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableVSync, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Borderless Fullscreen", &g_Manager->GetOptions()->_enableBorderlessFullscreen);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableBorderlessFullscreen, false);
                        AddNewTableRow();
                        ImGui::SeparatorText("Effects");
                        AddNewTableRow();
                        ImGui::Checkbox("Animation Interpolation", &g_Manager->GetOptions()->_enableInterpolation);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableInterpolation, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Color Correction", &g_Manager->GetOptions()->_enableColorCorrection);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableColorCorrection, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Color Modifier", &g_Manager->GetOptions()->_enableColorModifier);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableColorModifier, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Lighting", &g_Manager->GetOptions()->_enableLighting);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableLighting, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Shockwaves", &g_Manager->GetOptions()->_enableShockwave);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableShockwave, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Caustics", &g_Manager->GetOptions()->_enableCaustics);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableCaustics, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Pixelation", &g_Manager->GetOptions()->_enablePixelation);
                        ImGui::SameLine();
                        HelpMarker("e.g. Retro vision pill");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enablePixelation, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Bloom", &g_Manager->GetOptions()->_enableBloom);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableBloom, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Enable Water Surface rendering", &g_Manager->GetOptions()->_getsSetToEnableWaterSurface);
                        ImGui::SameLine();
                        HelpMarker("Needs to be set when changing Water Surface setting");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_getsSetToEnableWaterSurface, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Water Surface", &g_Manager->GetOptions()->_enableWaterSurface);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableWaterSurface, true);

                        ImGui::EndTable();
                    }

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Accessibility")) {
                    if (ImGui::BeginTable("Accessibility_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::Checkbox("Aim Lock", &g_Manager->GetOptions()->_enableAimLock);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableAimLock, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Rumble", &g_Manager->GetOptions()->_enableRumble);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableRumble, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Mouse Control", &g_Manager->GetOptions()->_enableMouseControl);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableMouseControl, false);
                        AddNewTableRow();
                        ImGui::Checkbox("Bullet Visibility", &g_Manager->GetOptions()->_enableBulletVisibility);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableBulletVisibility, false);
                        AddNewTableRow();
                        ImGui::SliderInt("Touch Mode", &g_Manager->GetOptions()->_touchMode, 0, 1, offOnModes[g_Manager->GetOptions()->_touchMode], ImGuiSliderFlags_NoInput);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_touchMode, 1);

                        ImGui::EndTable();
                    }

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Misc")) {
                    if (ImGui::BeginTable("Misc_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::Checkbox("Enable Mods", &g_Manager->GetOptions()->_enableMods);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableMods, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Enable Control Hotplug", &g_Manager->GetOptions()->_controlHotplugEnabled);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_controlHotplugEnabled, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Use Steam Cloud", &g_Manager->GetOptions()->_enableSteamCloud);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableSteamCloud, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Pause on focus lost", &g_Manager->GetOptions()->_enablePauseOnFocusLost);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enablePauseOnFocusLost, true);

                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("REPENTOGON")) {
                    if (ImGui::BeginTable("REPENTOGON_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::Checkbox("Better Void Generation", &repentogonOptions.betterVoidGeneration);
                        ImGui::SameLine();
                        HelpMarker("The Void now pulls rooms from all unlocked floors, including alt path.");
                        AddResetButton(++resetCounter, repentogonOptions.betterVoidGeneration, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Hush Panic State Fix", &repentogonOptions.hushPanicStateFix);
                        ImGui::SameLine();
                        HelpMarker("Fixes the vanilla bug that causes Hush to no longer have any pause between attacks below 50% health.");
                        AddResetButton(++resetCounter, repentogonOptions.hushPanicStateFix, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Key Masters affect deal chances", &repentogonOptions.keyMasterDealChance);
                        ImGui::SameLine();
                        HelpMarker("Killing Key Masters will raise the chance to spawn a deal, now consistent with killing other bums.");
                        AddResetButton(++resetCounter, repentogonOptions.keyMasterDealChance, true);
                        AddNewTableRow();
                        ImGui::Checkbox("Quicker Room Clear", &repentogonOptions.quickRoomClear);
                        ImGui::SameLine();
                        HelpMarker("Rooms will open their doors and spawn rewards quicker. Bosses still require their death animation to play out.");
                        AddResetButton(++resetCounter, repentogonOptions.quickRoomClear, false);
                        AddNewTableRow();
                        ImGui::Checkbox("Prevent mod updates", &repentogonOptions.preventModUpdates);
                        ImGui::SameLine();
                        HelpMarker("The game will skip updating your mods folder to match your current workshop folder. Useful for temporarily emptying your mods folder during mod development.");
                        AddResetButton(++resetCounter, repentogonOptions.preventModUpdates, false);

                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }

            ImGui::End(); // close window element
        }
    }
};

extern GameOptionsWindow gameOptionsWindow;
