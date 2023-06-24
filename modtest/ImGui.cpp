#include "ASMPatcher.hpp"
#include "ConsoleMega.h"
#include "CustomImGui.h"
#include "HelpMenu.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LogViewer.h"
#include "SigScan.h"

#include <Windows.h>
#include <format>
#include <gl/GL.h>
#include <sstream>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

// this blogpost https://werwolv.net/blog/dll_injection was a big help, thanks werwolv!

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND window;
WNDPROC windowProc;
bool menuShown = false;
static bool imguiInitialized = false;
static bool show_app_style_editor = false;

HelpMenu helpMenu;
LogViewer logViewer;
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

bool wasChangeButtonPressed = false;
ImGuiKey AddChangeGamepadButton()
{
    if (ImGui::Button("Change")) {
        wasChangeButtonPressed = true;
    }

    if (wasChangeButtonPressed && ImGui::BeginTooltip()) {
        ImGui::SetWindowFocus();
        ImGui::Text("Press a button on your controller.");
        ImGui::Text("Press ESC to cancel input");

        std::list<ImGuiKey>* keys = GetPressedKeys();
        for (auto key = keys->begin(); key != keys->end(); ++key) {
            if (*key >= static_cast<int>(ImGuiKey_GamepadStart) && *key <= static_cast<int>(ImGuiKey_GamepadRStickDown)) {
                wasChangeButtonPressed = false;
                return *key;
            }
            if (*key == static_cast<int>(ImGuiKey_Escape)) {
                wasChangeButtonPressed = false;
                return ImGuiKey_None;
            }
        }
        ImGui::EndTooltip();
    }
    return ImGuiKey_None;
}

LRESULT CALLBACK windowProc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Toggle the overlay using the grave key
    if (uMsg == WM_KEYDOWN && wParam == VK_OEM_3 && g_Manager->GetDebugConsoleEnabled()) {
        menuShown = !menuShown;

        // Induce a game pause by setting the debug console's state to 2 (shown). We'll suppress rendering in another hook.
        if (menuShown) {
            *g_Game->GetConsole()->GetState() = 2;

            // Console should steal focus by default, if visible.
            // Everybody (myself included) has been muscle-memoried into pressing ` and typing a command, we should respect that!
            if (console.enabled) {
                console.reclaimFocus = true;
            }
        } else
            *g_Game->GetConsole()->GetState() = 0;

        return false;
    }

    // If the overlay is shown, direct input to the overlay only
    if (menuShown) {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        return true;
    }

    // Otherwise call the game's wndProc function
    return CallWindowProc(windowProc, hWnd, uMsg, wParam, lParam);
}

HOOK_GLOBAL(OpenGL::wglSwapBuffers, (HDC hdc)->bool, __stdcall)
{

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

        imguiInitialized = true;
        logViewer.AddLog("[REPENTOGON]", "Initialized Dear ImGui\n");
        printf("[REPENTOGON] Dear ImGui initialized! Any further logs can be seen in the in-game log viewer.\n");
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (menuShown) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Tools")) {
                ImGui::MenuItem("Debug Console", NULL, &console.enabled);
                ImGui::MenuItem("Log Viewer", NULL, &logViewer.enabled);
                ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
                ImGui::EndMenu();
            }
            customImGui.DrawMenu();
            helpMenu.Draw();

            ImGui::EndMainMenuBar();
        }

        if (console.enabled) {
            console.Draw();
        }
        customImGui.DrawWindows();
    }
    logViewer.Draw(menuShown);

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

extern int handleWindowFlags(int flags);
extern ImGuiKey AddChangeGamepadButton();
