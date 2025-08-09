#pragma once
#include "../REPENTOGONOptions.h"
#include "../SaveSyncing/SaveSyncing.h"
#include "IconsFontAwesome6.h"
#include "ImGuiEx.h"
#include "IsaacRepentance.h"
#include "imgui.h"
#include "Lang.h"

extern int handleWindowFlags(int flags);
extern void HelpMarker(const char* desc);
extern bool WindowBeginEx(const char* name, bool* p_open, ImGuiWindowFlags flags);

static bool PromptUserYesNo(const char* prompt) {
    const int response = MessageBoxA(0, prompt, "REPENTOGON", MB_ICONINFORMATION | MB_YESNO);
    return response == IDOK || response == IDYES;
}

struct GameOptionsWindow : ImGuiWindowObject {
    GameOptionsWindow()
        : ImGuiWindowObject("Game Options")
    {
    }
    const char* extraHudModes[3] = { "Off", "Normal", "Mini" };
    const char* announcerModes[3] = { "Random", "Off", "Always on" };
    const char* popUpModes[3] = { "Off", "Large", "Small" };
    const char* consoleFontModes[3] = { "Default", "Small", "Tiny" };
    const char* offOnModes[2] = { "Off", "On" };
    const char* unifontRenderMode[5] = { "Normal: 13px, only non-latin chars", "LargePerfect: 16px", "Medium: 14px", "TinyPerfect: 16px and 0.5 scale", "TinyLow: 8px" };
    const char* jacobAndEsauControlModes[2] = { "Classic", "Better" };

    const char* saveImportExportSlots[4] = { "ALL", "1", "2", "3"};
    int selectedSaveImportExportSlot = 0;
    bool saveManagementResetMenu = false;
    std::optional<bool> saveManagementSyncResult = std::nullopt;
    std::optional<bool> saveManagementImportResult = std::nullopt;
    std::optional<bool> saveManagementExportResult = std::nullopt;

    void InitAfterLanguageAvaliable(){
        extraHudModes[0] = LANG.OPT_EXTRA_HUD_MODES_OFF;
        extraHudModes[1] = LANG.OPT_EXTRA_HUD_MODES_NORMAL;
        extraHudModes[2] = LANG.OPT_EXTRA_HUD_MODES_MINI;

        announcerModes[0] = LANG.OPT_ANNOUNCER_MODES_RANDOM;
        announcerModes[1] = LANG.OPT_ANNOUNCER_MODES_OFF;
        announcerModes[2] = LANG.OPT_ANNOUNCER_MODES_ALWAYS_ON;

        popUpModes[0] = LANG.OPT_POPUP_MODES_OFF;
        popUpModes[1] = LANG.OPT_POPUP_MODES_BIG;
        popUpModes[2] = LANG.OPT_POPUP_MODES_SMALL;

        consoleFontModes[0] = LANG.OPT_CONSOLE_FOND_MODES_DEFAULT;
        consoleFontModes[1] = LANG.OPT_CONSOLE_FOND_MODES_SMALL;
        consoleFontModes[2] = LANG.OPT_CONSOLE_FOND_MODES_TINY;

        offOnModes[0] = LANG.OPT_OFF_ON_MODES_OFF;
        offOnModes[1] = LANG.OPT_OFF_ON_MODES_ON;

        jacobAndEsauControlModes[0] = LANG.OPT_JACOB_ESAU_CONTROLS_MODES_CLASSIC;
		jacobAndEsauControlModes[1] = LANG.OPT_JACOB_ESAU_CONTROLS_MODES_BETTER;

        unifontRenderMode[0] = LANG.OPT_UNIFONT_RENDER_MODE_NORMAL;
        unifontRenderMode[1] = LANG.OPT_UNIFONT_RENDER_MODE_LARGE;
        unifontRenderMode[2] = LANG.OPT_UNIFONT_RENDER_MODE_MEDIUM;
        unifontRenderMode[3] = LANG.OPT_UNIFONT_RENDER_MODE_TINY;
        unifontRenderMode[4] = LANG.OPT_UNIFONT_RENDER_MODE_TINY_LOW;

        saveImportExportSlots[0] = LANG.OPT_SAVE_MANAGEMENT_ALL_SLOTS;
    }

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
        ImGui::SetItemTooltip(LANG.OPT_RESET_BTN_HINT);
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

    void AddSaveManagementButton(const char* label, const char* mark, const char* prompt, std::optional<bool>& result, std::function<bool()> buttonClickedFunc) {
        ImGui::BeginDisabled(result.has_value());
        AddNewTableRow();
        if (ImGui::Button(label)) {
            if (PromptUserYesNo(prompt)) {
                result = buttonClickedFunc();
                g_MenuManager->GetMenuSave()->Reset();
                saveManagementResetMenu = true;
            }
        }
        ImGui::SameLine();
        HelpMarker(mark);
        ImGui::EndDisabled();
        if (result.has_value()) {
            const bool success = *result;
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Text, success ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255));
            ImGui::Text(success ? LANG.OPT_SAVE_MANAGEMENT_SUCCESS : LANG.OPT_SAVE_MANAGEMENT_FAILED);
            ImGui::PopStyleColor();
        }
    }

    void Draw(bool isImGuiActive)
    {
        if (!enabled || !isImGuiActive && !pinned) {
            return;
        }
        ImGui::SetNextWindowSize(ImVec2(675, 375), ImGuiCond_FirstUseEver);

        if (WindowBeginEx(windowName.c_str(), &enabled, handleWindowFlags(0))) {
            AddWindowContextMenu();
            if (ImGui::BeginTabBar("GameOptionsTabBar", ImGuiTabBarFlags_None)) {
                int resetCounter = 0;
                if (ImGui::BeginTabItem(LANG.OPT_OPTIONS)) {
                    if (ImGui::BeginTable("Options_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::SliderFloat(LANG.OPT_SFX_VOLUME, &g_Manager->GetOptions()->_sfxVolume, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_sfxVolume, 0.3f);
                        AddNewTableRow();
                        ImGui::SliderFloat(LANG.OPT_MUSIC_VOLUME, &g_Manager->GetOptions()->_musicVolume, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_musicVolume, 0.3f);
                        AddNewTableRow();
                        ImGui::SliderFloat(LANG.OPT_MAP_OPACITY, &g_Manager->GetOptions()->_mapOpacity, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_mapOpacity, 0.3f);
                        AddNewTableRow();
                        // ImGui::Checkbox("Fullscreen", &g_Manager->GetOptions()->_isFullscreen); // value gets set somewhere else
                        ImGui::Checkbox(LANG.OPT_FILTER, &g_Manager->GetOptions()->_filterEnabled);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_FILTER_MARK);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_filterEnabled, false);
                        AddNewTableRow();
                        ImGui::SliderInt(LANG.OPT_POP_UPS, &g_Manager->GetOptions()->_popupsMode, 0, 2, popUpModes[g_Manager->GetOptions()->_popupsMode], ImGuiSliderFlags_NoInput);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_popupsMode, 1);
                        AddNewTableRow();
                        ImGui::SliderInt(LANG.OPT_ANNOUNCER_VOICE_MODE, &g_Manager->GetOptions()->_announcerVoiceMode, 0, 2, announcerModes[g_Manager->GetOptions()->_announcerVoiceMode], ImGuiSliderFlags_NoInput);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_announcerVoiceMode, 0);
                        AddNewTableRow();
                        ImGui::SliderInt(LANG.OPT_JACOB_ESAU_CONTROLS_MODE, &g_Manager->GetOptions()->_jacobEsauControls, 0, 1, jacobAndEsauControlModes[g_Manager->GetOptions()->_jacobEsauControls], ImGuiSliderFlags_NoInput);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_jacobEsauControls, 0);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_ASCENT_VOICEOVER, &g_Manager->GetOptions()->_ascentVoiceover);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_ascentVoiceover, true);
                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem(LANG.OPT_HUD_HUDS)) {
                    if (ImGui::BeginTable("HUDs_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::SliderFloat(LANG.OPT_HUD_HUD_OFFSET, &g_Manager->GetOptions()->_hudOffset, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_hudOffset, 1.0f);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_HUD_FOUND_HUD, &g_Manager->GetOptions()->_enableFoundHUD);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_HUD_FOUND_HUD_MARKER);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableFoundHUD, false);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_HUD_CHARGE_BARS, &g_Manager->GetOptions()->_enableChargeBars);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableChargeBars, false);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_HUD_BOSS_HEALTH_BAR_ON_BOTTOM, &g_Manager->GetOptions()->_bossHpOnBottom);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_bossHpOnBottom, true);
                        AddNewTableRow();
                        ImGui::SliderInt(LANG.OPT_HUD_EXTRA_HUD, &g_Manager->GetOptions()->_historyHudMode, 0, 2, extraHudModes[g_Manager->GetOptions()->_historyHudMode], ImGuiSliderFlags_NoInput);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_HUD_EXTRA_HUD_MARKER);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_historyHudMode, 0);
                        AddNewTableRow();

                        ImGui::SeparatorText(LANG.OPT_CONSOLE);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_CONSOLE_ENABLE_DEBUG_CONSOLE, &g_Manager->GetOptions()->_enableDebugConsole);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableDebugConsole, false);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_CONSOLE_FADED_CONSOLE_DISPLAY, &g_Manager->GetOptions()->_enableFadedConsoleDisplay);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableFadedConsoleDisplay, false);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_CONSOLE_SAVE_COMMAND_HISTORY, &g_Manager->GetOptions()->_enableSaveCommandHistory);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableSaveCommandHistory, true);
                        AddNewTableRow();
                        ImGui::SliderInt(LANG.OPT_CONSOLE_CONSOLE_FONT, &g_Manager->GetOptions()->_consoleFont, 0, 2, consoleFontModes[g_Manager->GetOptions()->_consoleFont], ImGuiSliderFlags_NoInput);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_consoleFont, 0);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_CONSOLE_ENABLE_UNICODE_FONT, &repentogonOptions.enableUnifont);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_CONSOLE_ENABLE_UNICODE_FONT_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.enableUnifont, true);
                        AddNewTableRow();
                        ImGui::SliderInt(LANG.OPT_CONSOLE_UNIFONT_RENDER_MODE, &repentogonOptions.unifontRenderMode, 0, 4, unifontRenderMode[repentogonOptions.unifontRenderMode], ImGuiSliderFlags_NoInput);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_CONSOLE_UNIFONT_RENDER_MODE_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.unifontRenderMode, 0);
                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem(LANG.OPT_VIS)) {
                    if (ImGui::BeginTable("Visual Effects_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::SliderFloat(LANG.OPT_VIS_GAMMA, &g_Manager->GetOptions()->_gamma, 0.0f, 2.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_gamma, 1.0f);
                        AddNewTableRow();
                        ImGui::SliderFloat(LANG.OPT_VIS_EXPOSURE, &g_Manager->GetOptions()->_exposure, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_exposure, 1.0f);
                        AddNewTableRow();
                        ImGui::SliderFloat(LANG.OPT_VIS_BRIGHTNESS, &g_Manager->GetOptions()->_brightness, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_brightness, 0.0f);
                        AddNewTableRow();
                        ImGui::SliderFloat(LANG.OPT_VIS_CONTRAST, &g_Manager->GetOptions()->_contrast, 0.0f, 1.0f, "%.2f");
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_contrast, 1.0f);
                        AddNewTableRow();
                        ImGui::InputInt(LANG.OPT_VIS_MAX_SCALE, &g_Manager->GetOptions()->_maxScale, 1);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_maxScale, 99);
                        AddNewTableRow();
                        ImGui::InputInt(LANG.OPT_VIS_MAX_RENDER_SCALE, &g_Manager->GetOptions()->_maxRenderScale, 1);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_maxRenderScale, 2);
                        AddNewTableRow();
                        ImGui::SliderInt(LANG.OPT_VIS_ACTIVE_CAMERA, &g_Manager->GetOptions()->_cameraStyle, 0, 1, offOnModes[g_Manager->GetOptions()->_cameraStyle], ImGuiSliderFlags_NoInput);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_cameraStyle, 1);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_VSYNC, &g_Manager->GetOptions()->_enableVSync);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableVSync, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_BORDERLESS_FULLSCREEN, &g_Manager->GetOptions()->_enableBorderlessFullscreen);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableBorderlessFullscreen, false);
                        AddNewTableRow();
//                        ImGui::SeparatorText(LANG.OPT_VIS_EFFECTS);
                        ImGui::SeparatorTextEx(0, LANG.OPT_VIS_EFFECTS, ImGui::FindRenderedTextEnd(LANG.OPT_VIS_EFFECTS), 24.0f*ImGui::GetCurrentWindow()->FontWindowScale);
                        //     ^ separatortextex is used to apply an offset for the help marker icon
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_VIS_EFFECTS_MARK);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_ANIM_INTERP, &g_Manager->GetOptions()->_enableInterpolation);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_VIS_ANIM_INTERP_MARK);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableInterpolation, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_COLOR_CORRECTION, &g_Manager->GetOptions()->_enableColorCorrection);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableColorCorrection, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_COLOR_MODIFIER, &g_Manager->GetOptions()->_enableColorModifier);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableColorModifier, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_LIGHTING, &g_Manager->GetOptions()->_enableLighting);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableLighting, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_SHOCKWAVES, &g_Manager->GetOptions()->_enableShockwave);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableShockwave, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_CAUSTICS, &g_Manager->GetOptions()->_enableCaustics);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableCaustics, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_PIXELATION, &g_Manager->GetOptions()->_enablePixelation);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_VIS_PIXELATION_MARK);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enablePixelation, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_BLOOM, &g_Manager->GetOptions()->_enableBloom);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableBloom, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_ENABLE_WATER_SURFACE_RENDERING, &g_Manager->GetOptions()->_getsSetToEnableWaterSurface);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_VIS_ENABLE_WATER_SURFACE_RENDERING_MARK);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_getsSetToEnableWaterSurface, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_VIS_WATER_SURFACE, &g_Manager->GetOptions()->_enableWaterSurface);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableWaterSurface, true);

                        ImGui::EndTable();
                    }

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem(LANG.OPT_ACC)) {
                    if (ImGui::BeginTable("Accessibility_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_ACC_AIM_LOCK, &g_Manager->GetOptions()->_enableAimLock);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableAimLock, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_ACC_RUMBLE, &g_Manager->GetOptions()->_enableRumble);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableRumble, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_ACC_MOUSE_CONTROL, &g_Manager->GetOptions()->_enableMouseControl);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableMouseControl, false);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_ACC_BULLET_VISIBILITY, &g_Manager->GetOptions()->_enableBulletVisibility);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableBulletVisibility, false);
                        AddNewTableRow();
                        ImGui::SliderInt(LANG.OPT_ACC_TOUCH_MODE, &g_Manager->GetOptions()->_touchMode, 0, 1, offOnModes[g_Manager->GetOptions()->_touchMode], ImGuiSliderFlags_NoInput);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_touchMode, 1);

                        ImGui::EndTable();
                    }

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem(LANG.OPT_MISC)) {
                    if (ImGui::BeginTable("Misc_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_MISC_ENABLE_MODS, &g_Manager->GetOptions()->_enableMods);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableMods, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_MISC_ENABLE_CTRL_HOTPLUG, &g_Manager->GetOptions()->_controlHotplugEnabled);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_controlHotplugEnabled, true);
                        AddNewTableRow();
                        //ImGui::Checkbox(LANG.OPT_MISC_USE_STEAM_CLOUD, &g_Manager->GetOptions()->_enableSteamCloud); //Dont add this back!, it can fuck up saves pretty badly to toggle this ingame, leaving it commented so noone is tempted to add it back
                        //AddResetButton(++resetCounter, g_Manager->GetOptions()->_enableSteamCloud, true);
                        //AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_MISC_PAUSE_ON_FOCUS_LOST, &g_Manager->GetOptions()->_enablePauseOnFocusLost);
                        AddResetButton(++resetCounter, g_Manager->GetOptions()->_enablePauseOnFocusLost, true);

                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(LANG.OPT_REPENTOGON)) {
                    if (ImGui::BeginTable("REPENTOGON_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_BETTER_VOID_GEN, &repentogonOptions.betterVoidGeneration);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_BETTER_VOID_GEN_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.betterVoidGeneration, false);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_HUSH_LASER_FIX, &repentogonOptions.hushLaserSpeedFix);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_HUSH_LASER_FIX_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.hushLaserSpeedFix, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_STAT_HUD_PLANETARIUM, &repentogonOptions.statHUDPlanetarium);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_STAT_HUD_PLANETARIUM_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.statHUDPlanetarium, true);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_SKIP_INTRO, &repentogonOptions.skipIntro);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_SKIP_INTRO_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.skipIntro, false);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_QUICKER_ROOM_CLEAR, &repentogonOptions.quickRoomClear);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_QUICKER_ROOM_CLEAR_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.quickRoomClear, false);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_PREVENT_MOD_UPDATES, &repentogonOptions.preventModUpdates);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_PREVENT_MOD_UPDATES_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.preventModUpdates, false);
                        //AddNewTableRow();
                        //ImGui::Checkbox(LANG.OPT_REPENTOGON_FAST_LASERS, &repentogonOptions.fastLasers);
                        //ImGui::SameLine();
                        //HelpMarker(LANG.OPT_REPENTOGON_FAST_LASERS_MARK);
                        //AddResetButton(++resetCounter, repentogonOptions.fastLasers, false);
                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_INTERPOLV2, &repentogonOptions.interpolV2);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_INTERPOLV2_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.interpolV2, false);
                        AddNewTableRow();
                        ImGui::SliderInt(LANG.OPT_REPENTOGON_MARSDOUBLETAP, &repentogonOptions.marsDoubleTapWindow, 2, 20, "%d", ImGuiSliderFlags_AlwaysClamp);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_MARSDOUBLETAP_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.marsDoubleTapWindow, 10);

                        AddNewTableRow();
                        ImGui::SliderInt(LANG.OPT_REPENTOGON_CONSOLE_AUTOFILL_LIMIT, &repentogonOptions.consoleAutofillLimit, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_CONSOLE_AUTOFILL_LIMIT_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.consoleAutofillLimit, 10);

                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_FILE_MAP, &repentogonOptions.fileMap);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_FILE_MAP_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.fileMap, true);

                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_ECO_MODE, &repentogonOptions.ecoMode);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_ECO_MODE_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.ecoMode, false);

                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_DEBUG_FIND_IN_RADIUS, &repentogonOptions.renderDebugFindInRadius);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_DEBUG_FIND_IN_RADIUS_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.renderDebugFindInRadius, false);

                        AddNewTableRow();
                        ImGui::Checkbox(LANG.OPT_REPENTOGON_DISABLE_EXIT_PROMPT, &repentogonOptions.disableExitPrompt);
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_REPENTOGON_DISABLE_EXIT_PROMPT_MARK);
                        AddResetButton(++resetCounter, repentogonOptions.disableExitPrompt, false);

                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }

                if (SaveSyncing::USE_SEPARATE_REPENTOGON_SAVE_FILES && ImGui::BeginTabItem(LANG.OPT_SAVE_MANAGEMENT)) {
                    if (ImGui::BeginTable("SaveManagement_Table", 2, ImGuiTableFlags_SizingStretchProp)) {
                        // Save Management options/utilities that can immediately modify save data are only available on the title/save menus when no save file is loaded.
                        const bool saveManagementAllowed = !Isaac::IsInGame() && g_MenuManager && (g_MenuManager->_selectedMenuID == 1 || g_MenuManager->_selectedMenuID == 2);
                        if (!saveManagementAllowed) {
                            AddNewTableRow();
                            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                            ImGui::Text(LANG.OPT_SAVE_MANAGEMENT_DISABLED_REASON);
                            ImGui::PopStyleColor();
                        }

                        AddNewTableRow();
                        if (ImGui::Checkbox(LANG.OPT_SAVE_MANAGEMENT_ENABLE_SAVE_SYNCING, SaveSyncing::syncStatus.GetEnabledPtr())) {
                            SaveSyncing::syncStatus.SaveToJson();
                        }
                        ImGui::SameLine();
                        HelpMarker(LANG.OPT_SAVE_MANAGEMENT_ENABLE_SAVE_SYNCING_MARK);
                        AddResetButton(++resetCounter, *SaveSyncing::syncStatus.GetEnabledPtr(), true);

                        ImGui::BeginDisabled(!saveManagementAllowed);

                        AddSaveManagementButton(
                            LANG.OPT_SAVE_MANAGEMENT_SYNC,
                            LANG.OPT_SAVE_MANAGEMENT_SYNC_MARK,
                            LANG.OPT_SAVE_MANAGEMENT_SYNC_PROMPT,
                            saveManagementSyncResult,
                            []() { return SaveSyncing::PerformVanillaSaveSynchronization(false); });

                        AddNewTableRow();
                        ImGui::SeparatorText(LANG.OPT_SAVE_MANAGEMENT_IMPORT_EXPORT);

                        AddNewTableRow();
                        ImGui::SetNextItemWidth(150);
                        if (ImGui::Combo(LANG.OPT_SAVE_MANAGEMENT_SAVE_SLOT, &selectedSaveImportExportSlot, saveImportExportSlots, IM_ARRAYSIZE(saveImportExportSlots))) {
                            saveManagementImportResult = std::nullopt;
                            saveManagementExportResult = std::nullopt;
                        }

                        AddSaveManagementButton(
                            LANG.OPT_SAVE_MANAGEMENT_IMPORT_FROM_REPENTANCE,
                            LANG.OPT_SAVE_MANAGEMENT_IMPORT_FROM_REPENTANCE_MARK,
                            LANG.OPT_SAVE_MANAGEMENT_IMPORT_FROM_REPENTANCE_PROMPT,
                            saveManagementImportResult,
                            [this]() { return SaveSyncing::ImportFrom(SaveSyncing::REPENTANCE, selectedSaveImportExportSlot); });
                        AddSaveManagementButton(
                            LANG.OPT_SAVE_MANAGEMENT_EXPORT_TO_REPENTANCE,
                            LANG.OPT_SAVE_MANAGEMENT_EXPORT_TO_REPENTANCE_MARK,
                            LANG.OPT_SAVE_MANAGEMENT_EXPORT_TO_REPENTANCE_PROMPT,
                            saveManagementExportResult,
                            [this]() { return SaveSyncing::ExportTo(SaveSyncing::REPENTANCE, selectedSaveImportExportSlot); });

                        ImGui::EndDisabled();
                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                } else if (saveManagementResetMenu) {
                    saveManagementSyncResult = std::nullopt;
                    saveManagementImportResult = std::nullopt;
                    saveManagementExportResult = std::nullopt;
                    saveManagementResetMenu = false;
                }
                ImGui::EndTabBar();
            }
        }

        ImGui::End(); // end window element
    }
};

extern GameOptionsWindow gameOptionsWindow;
