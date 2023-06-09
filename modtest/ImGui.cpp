#include "ASMPatcher.hpp"
#include "CustomImGui.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LogViewer.h"
#include "ConsoleMega.h"
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

LogViewer logViewer;
ConsoleMega console;
CustomImGui customImGui;

LRESULT CALLBACK windowProc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Toggle the overlay using the delete key
    if (uMsg == WM_KEYDOWN && wParam == VK_DELETE) {
        menuShown = !menuShown;
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
        ImGui::CaptureMouseFromApp();
        ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);
        imguiInitialized = true;
        logViewer.AddLog("[REPENTOGON]", "Initialized Dear ImGui\n");
        printf("[REPENTOGON] Dear ImGui initialized! Any further logs can be seen in the in-game log viewer.\n");
    }

    if (menuShown) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Tools")) {
                if (ImGui::MenuItem("Console (UNFINISHED)", NULL, &console.enabled)) { }
                if (ImGui::MenuItem("Log Viewer", NULL, &logViewer.enabled)) { }
                ImGui::EndMenu();
            }
            customImGui.DrawMenu();
            ImGui::EndMainMenuBar();
        }
        if (logViewer.enabled) {
            logViewer.Draw();
        }

        if (console.enabled) {
            console.Draw();
        }
        customImGui.DrawWindows();

        ImGui::Render();

        // Draw the overlay
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    else if (logViewer.pinned) {

        // Show the log viewer and draw the overlay without hooking any input
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        logViewer.Draw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    return super(hdc);
}

/*
 * LOG VIEWER PRINTS
 */

void __stdcall LogMessageCallback(const char* logMessage)
{
    logViewer.AddLog("[GAME]", logMessage);
};

HOOK_METHOD(Console, Print, (const std::string& text, unsigned int color, unsigned int unk) -> void)
{
    logViewer.AddLog("[CONSOLE]", text.c_str());
    super(text, color, unk);
}