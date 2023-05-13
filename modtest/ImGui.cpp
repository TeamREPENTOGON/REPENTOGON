#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "SigScan.h"

#include "ASMPatcher.hpp"

#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include <gl/GL.h>

// this blogpost https://werwolv.net/blog/dll_injection was a big help, thanks werwolv! 

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND window;
WNDPROC windowProc;
bool menuShown = false;
bool imguiConsoleEnabled = false;
static bool imguiInitialized = false;

static ImGuiTextBuffer logBuf;

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

struct LogViewer {
    bool enabled;
    bool pinned;
    bool showGame;
    bool showRepentogon;
    bool showConsole;
    bool autoscroll;

    ImGuiTextFilter filter;
    ImVector<int> offsets;

    LogViewer()
    {
        enabled = false;
        pinned = false;
        showGame = true;
        showRepentogon = true;
        showConsole = true;
        autoscroll = true;
    }

    void Draw() {
        if (ImGui::Begin("Log Viewer", &enabled))
        {
            ImGui::Checkbox("Pin Window", &pinned);
            ImGui::SameLine();
            HelpMarker("Pinning a window will keep it visible even after closing Dev Tools.");
            ImGui::SameLine();
            ImGui::Checkbox("Autoscroll", &autoscroll);
            ImGui::SameLine();
            filter.Draw("Filter (unimplemented)", -100.0f);

            ImGui::Text("Show:");
            ImGui::SameLine();
            ImGui::Checkbox("Game (buggy)", &showGame);
            ImGui::SameLine();
            ImGui::Checkbox("REPENTOGON", &showRepentogon);
            ImGui::SameLine();
            ImGui::Checkbox("Console", &showConsole);
            ImGui::Separator();
            if (ImGui::BeginChild("LogViewScrollable", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
                const char* buf_begin = logBuf.begin();
                const char* buf_end = logBuf.end();
                ImGui::TextUnformatted(buf_begin, buf_end);

                if (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
                ImGui::EndChild();
            }

            ImGui::End();
        }
    }
};


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

HOOK_GLOBAL(OpenGL::wglSwapBuffers, (HDC hdc) -> bool, __stdcall) {
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
        ImGui::CaptureMouseFromApp();
        ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);
		imguiInitialized = true;

        static LogViewer log;
        logBuf.append("[REPENTOGON] Initialized Dear ImGui\n");
	}

    static LogViewer log;

    if (menuShown) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Console (UNIMPLEMENTED)", NULL, &imguiConsoleEnabled)) {}
                if (ImGui::MenuItem("Log Viewer (UNFINISHED)", NULL, &log.enabled)) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (log.enabled) {
            log.Draw();
        }

        ImGui::Render();

        // Draw the overlay
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    else if (log.pinned) {

        // Show the log viewer and draw the overlay without hooking any input
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        log.Draw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

	return super(hdc);
}

/*HOOK_STATIC(KAGE, LogMessage, (int flag, const char* string) -> void) {
    //super(flag, string); // for this to work we need to get varargs working in libzhl

    logBuf.append("[GAME] *snip*\n");
}*/

HOOK_METHOD(Console, Print, (const IsaacString& text, unsigned int color, unsigned int unk) -> void) {
    logBuf.appendf("[CONSOLE] %s", text.Get()); //TODO figure out a clean way to pass through the color
    super(text, color, unk);
}