#include "ASMPatcher.hpp"
#include "ConsoleMega.h"
#include "CustomImGui.h"
#include "HelpMenu.h"
#include "HookSystem.h"
#include "PerformanceWindow.h"
#include "IsaacRepentance.h"
#include "LogViewer.h"
#include "SigScan.h"
#include "IconsFontAwesome6.h"

#include <Windows.h>
#include <format>
#include <gl/GL.h>
#include <sstream>
#include <algorithm>

#include "imgui.h"
#include "imgui_freetype.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

// this blogpost https://werwolv.net/blog/dll_injection was a big help, thanks werwolv!

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND window;
WNDPROC windowProc;
bool menuShown = false;
bool leftMouseClicked = false;
static bool imguiInitialized = false;
static bool show_app_style_editor = false;
static bool shutdownInitiated = false;
static bool imguiResized;
static ImVec2 imguiSizeModifier;

HelpMenu helpMenu;
LogViewer logViewer;
PerformanceWindow performanceWindow;
ConsoleMega console;
CustomImGui customImGui;

int handleWindowFlags(int flags)
{
    // disable interactive parts of imgui
    if (!menuShown)
        flags = flags | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;
    return flags;
}

std::list<ImGuiKey>* GetPressedKeys()
{
    std::list<ImGuiKey>* keys = new std::list<ImGuiKey>();
    for (ImGuiKey key = ImGuiKey_Tab; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) {
        if (!ImGui::IsKeyDown(key))
            continue;
        keys->push_back(key);
    }
    return keys;
}

void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void AddWindowContextMenu(bool* pinned)
{
    if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
    {
        ImGui::MenuItem("Pin Window", NULL, pinned);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal) && ImGui::BeginTooltip()) {
          ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
          ImGui::TextUnformatted("Pinning a window will keep it visible even after closing Dev Tools.");
          ImGui::PopTextWrapPos();
          ImGui::EndTooltip();
        }

        ImGui::EndPopup();
    }
}

ImGuiKey AddChangeKeyButton(bool isController, bool& wasPressed)
{
    if (ImGui::Button("Change")) {
        wasPressed = true;
    }

    if (wasPressed && ImGui::BeginTooltip()) {
        ImGui::SetWindowFocus();
        int firstKey = static_cast<int>(ImGuiKey_Tab);
        int lastKey = static_cast<int>(ImGuiKey_KeypadEqual);
        if (isController) {
            firstKey = static_cast<int>(ImGuiKey_GamepadStart);
            lastKey = static_cast<int>(ImGuiKey_GamepadRStickDown);
            ImGui::Text("Press a button on your controller.");
        } else {
            ImGui::Text("Press a key on your keyboard.");
        }
        ImGui::Text("Press ESC to cancel input");

        std::list<ImGuiKey>* keys = GetPressedKeys();
        for (auto key = keys->begin(); key != keys->end(); ++key) {
            if (*key == static_cast<int>(ImGuiKey_Escape)) {
                wasPressed = false;
                return ImGuiKey_None;
            }
            if (*key >= firstKey && *key <= lastKey) {
                wasPressed = false;
                return *key;
            }
        }
        ImGui::EndTooltip();
    }
    return ImGuiKey_None;
}

static std::vector<WPARAM> pressedKeys;

LRESULT CALLBACK windowProc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (shutdownInitiated)
        return CallWindowProc(windowProc, hWnd, uMsg, wParam, lParam);

    // Enable the overlay using the grave key, disable using ESC
    if (uMsg == WM_KEYDOWN && g_Manager->GetDebugConsoleEnabled()) {
        ImGui::CloseCurrentPopup();

        switch(wParam) {
            case VK_OEM_3: {
                menuShown = true;

                break;
            }

            case VK_ESCAPE: {
                if (menuShown) {
                    menuShown = false;
                    return true;
                }

                break;
            }
        }
    }

    if (menuShown && *g_Game->GetConsole()->GetState() != 2) {
        // Release keys we've tracked as being pressed. Call the game's wndProc to accomplish this
        std::vector keys = pressedKeys;
        for (WPARAM key : keys) {
            CallWindowProc(windowProc, hWnd, WM_KEYUP, key, lParam);
            // We're working on a copy of the vector, so we don't necessarily care about popping the original one in order
            pressedKeys.pop_back();
        }

        // Pop the left mouse button, too, if it was previously clicked.
        if (leftMouseClicked) {
            leftMouseClicked = false;
            CallWindowProc(windowProc, hWnd, WM_LBUTTONUP, 0, lParam);
        }


        // Induce a game pause by setting the debug console's state to 2 (shown). We'll suppress rendering in another hook.
        *g_Game->GetConsole()->GetState() = 2;

        // Console should steal focus by default, if visible.
        // Everybody (myself included) has been muscle-memoried into pressing ` and typing a command, we should respect that!
        console.enabled = true;
        console.reclaimFocus = true;
    }

    if (!menuShown && *g_Game->GetConsole()->GetState() != 0) {
        std::vector keys = pressedKeys;
        for (WPARAM key : keys) {
            ImGui_ImplWin32_WndProcHandler(hWnd, WM_KEYUP, key, lParam);
            pressedKeys.pop_back();
        }

        if (leftMouseClicked) {
            leftMouseClicked = false;
            ImGui_ImplWin32_WndProcHandler(hWnd, WM_LBUTTONUP, 0, lParam);
        }

        *g_Game->GetConsole()->GetState() = 0;
    }


    // Track what keys are being pressed so we can release them the next time ImGui state changes
    switch (uMsg) {
        case WM_KEYDOWN: {
            // When a key is pushed down, it can trigger multiple WM_KEYDOWN events in a row.
            // Ensure we're not entering duplicates.
            std::vector<WPARAM>::iterator it = find(pressedKeys.begin(), pressedKeys.end(), wParam);
            if (it == pressedKeys.end()) {
                pressedKeys.push_back(wParam);
            }

            break;
        }
        case WM_LBUTTONDOWN: {
            leftMouseClicked = true;
            break;

        }
        case WM_KEYUP: {
            pressedKeys.erase(std::remove(pressedKeys.begin(), pressedKeys.end(), wParam), pressedKeys.end());

            break;
        }
    }

    std::vector<UINT> blockedMsgs = {WM_KEYUP, WM_KEYDOWN, WM_KEYFIRST, WM_KEYLAST, WM_MOUSEMOVE, 
        WM_LBUTTONUP, WM_LBUTTONDOWN, WM_LBUTTONDBLCLK, 
        WM_MBUTTONDOWN, WM_MBUTTONUP, WM_MBUTTONDBLCLK, 
        WM_RBUTTONDOWN, WM_RBUTTONUP, WM_RBUTTONDBLCLK};

    // If the overlay is shown, direct input to the overlay only
    // Otherwise call the game's WndProc function
    if (menuShown) {
        // Block the game's WndProc on input related uMsgs but allow any others through
        if (std::find(blockedMsgs.begin(), blockedMsgs.end(), uMsg) == blockedMsgs.end()) {
            CallWindowProc(windowProc, hWnd, uMsg, wParam, lParam);
        }
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        return true;
    }

   
    
    return CallWindowProc(windowProc, hWnd, uMsg, wParam, lParam);
}

HOOK_GLOBAL(OpenGL::wglSwapBuffers, (HDC hdc)->bool, __stdcall)
{
    static std::map<int, ImFont*> fonts;

    if (!imguiInitialized) {
        HWND window = WindowFromDC(hdc);
        windowProc = (WNDPROC)SetWindowLongPtr(window,
            GWLP_WNDPROC, (LONG_PTR)windowProc_hook);

        ImGui::CreateContext();
        ImGui_ImplWin32_Init(window);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();
        ImGui::GetStyle().AntiAliasedFill = false;
        ImGui::GetStyle().AntiAliasedLines = false;
        ImGui::GetStyle().FrameRounding = 4.0f; // rounded edges (default was 0)
        ImGui::GetStyle().FramePadding.x = 6.0f; // more padding inside of objects to prevent ugly text clipping (default was 4)
        ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);

        ImGuiIO& io = ImGui::GetIO();
        // mouse, keyboard and gamepad support
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
        io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
        ImGui::CaptureMouseFromApp();
        ImGui::CaptureKeyboardFromApp();

        ImFontConfig cfg;
        cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags_MonoHinting;
        cfg.OversampleH = 1;
        cfg.OversampleV = 1;
        cfg.PixelSnapH = 1;

		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		for (int i = 1; i <= 12; ++i) {
			// ImGui font scaling will make it blurry, this is a suboptimal but (for better or worse) functional workaround.
			cfg.MergeMode = false;
			cfg.SizePixels = 13.0f * i;
			fonts.insert(std::pair<int, ImFont*>(i, io.Fonts->AddFontDefault(&cfg)));
			cfg.MergeMode = true;
			io.Fonts->AddFontFromFileTTF("resources-repentogon\\fonts\\Font Awesome 6 Free-Solid-900.otf", 16, &cfg, icon_ranges);
		}

        imguiInitialized = true;
        ImGui::GetIO().FontAllowUserScaling = true;
        logViewer.AddLog("[REPENTOGON]", "Initialized Dear ImGui\n");
        printf("[REPENTOGON] Dear ImGui initialized! Any further logs can be seen in the in-game log viewer.\n");
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static float oldPointScale = 0;
    if(g_PointScale != oldPointScale)
        if (g_PointScale >= 0 && g_PointScale < fonts.size() && (fonts.find((int)g_PointScale) != fonts.end()))
            ImGui::GetIO().FontDefault = fonts.at((int)g_PointScale);

    oldPointScale = g_PointScale;

    imguiResized = false;
    static ImVec2 oldSize = ImVec2(0, 0);
    if (oldSize.x != ImGui::GetMainViewport()->Size.x || oldSize.y != ImGui::GetMainViewport()->Size.y) { // no operator?
        imguiResized = true;
        imguiSizeModifier = ImVec2(ImGui::GetMainViewport()->Size.x / oldSize.x, ImGui::GetMainViewport()->Size.y / oldSize.y);
    }
    oldSize = ImGui::GetMainViewport()->Size;
    

    if (menuShown) {
        if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu(ICON_FA_SCREWDRIVER_WRENCH " Tools")) {
				ImGui::MenuItem(ICON_FA_TERMINAL" Debug Console", NULL, &console.enabled);
				ImGui::MenuItem(ICON_FA_NEWSPAPER" Log Viewer", NULL, &logViewer.enabled);
				ImGui::MenuItem(ICON_FA_GAUGE_HIGH" Performance", NULL, &performanceWindow.enabled);
				ImGui::MenuItem(ICON_FA_PENCIL" Style Editor", NULL, &show_app_style_editor);
				ImGui::EndMenu();
            }
            customImGui.DrawMenu();
            helpMenu.Draw();

            ImGui::EndMainMenuBar();
        }
    }

    console.Draw(menuShown);
    logViewer.Draw(menuShown);
    performanceWindow.Draw(menuShown);

    customImGui.DrawWindows(menuShown);

    if (show_app_style_editor) {
        ImGui::Begin("Dear ImGui Style Editor", &show_app_style_editor);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }

    ImGui::Render();

    // Draw the overlay
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return super(hdc);
}

HOOK_METHOD(Console, Render, ()->void)
{
    // We set console state to induce a game pause but we don't want the actual console rendering, just suppress.
    // TODO could we set an option for this? Some people might prefer the original console, I hope they feel better soon
}

/*
 * LOG VIEWER PRINTS
 */

void __stdcall LogMessageCallback(const char* logMessage)
{
    logViewer.AddLog("[GAME]", logMessage);
};

HOOK_METHOD(Console, Print, (const std::string& text, unsigned int color, unsigned int unk)->void)
{
    logViewer.AddLog("[CONSOLE]", text.c_str());
    super(text, color, unk);
}

HOOK_STATIC(Isaac, Shutdown, () -> void, __cdecl) {
    shutdownInitiated = true;
    super();
}

extern int handleWindowFlags(int flags);
extern ImGuiKey AddChangeKeyButton(bool isController, bool& wasPressed);
extern void AddWindowContextMenu(bool* pinned);
extern void HelpMarker(const char* desc);
