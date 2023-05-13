#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "SigScan.h"
#include "ASMPatcher.hpp"
#include "ASMLogMessage.h"

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

    ImGuiTextBuffer logBuf;
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

    void AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = logBuf.size();
        va_list args;
        va_start(args, fmt);
        logBuf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = logBuf.size(); old_size < new_size; old_size++)
            if (logBuf[old_size] == '\n')
                offsets.push_back(old_size + 1);
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
            filter.Draw("Filter", -100.0f);

            ImGui::Text("Show:");
            ImGui::SameLine();
            ImGui::Checkbox("Game", &showGame);
            ImGui::SameLine();
            ImGui::Checkbox("REPENTOGON", &showRepentogon);
            ImGui::SameLine();
            ImGui::Checkbox("Console", &showConsole);
            ImGui::Separator();

            if (ImGui::BeginChild("LogViewScrollable", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
                const char* buf_begin = logBuf.begin();
                const char* buf_end = logBuf.end();

                if (filter.IsActive())
                {
                    for (int line_no = 0; line_no < offsets.Size; line_no++)
                    {
                        const char* line_start = buf_begin + offsets[line_no];
                        const char* line_end = (line_no + 1 < offsets.Size) ? (buf_begin + offsets[line_no + 1] - 1) : buf_end;
                        if (filter.PassFilter(line_start, line_end))
                            ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                else
                {
                    ImGui::TextUnformatted(buf_begin, buf_end);
                }

                if (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
                ImGui::EndChild();
            }

            ImGui::End();
        }
    }
};

static LogViewer logViewer;

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
        logViewer.AddLog("[REPENTOGON] Initialized Dear ImGui\n");
	}

    if (menuShown) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Console (UNIMPLEMENTED)", NULL, &imguiConsoleEnabled)) {}
                if (ImGui::MenuItem("Log Viewer (UNFINISHED)", NULL, &logViewer.enabled)) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (logViewer.enabled) {
            logViewer.Draw();
        }

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

static void __stdcall LogMessageCallback(const char* logMessage) {
    logViewer.AddLog("[GAME] %s", logMessage);
};

void ASMPatchLogMessage() {
    SigScan scanner("8d51??8a014184c075??2bca80b9????????0a");
    scanner.Scan();
    void* addr = scanner.GetAddress();

    ASMPatch patch;
    patch.AddBytes("\x55\x89\xe5\x50\x52\x53\x51\x56\x57"); // Push the entire stack
    patch.AddBytes("\x51"); // Push ECX one more time, for use on our function (this will be consumed)
    patch.AddInternalCall(LogMessageCallback);
    patch.AddBytes("\x5f\x5e\x59\x5b\x5a\x58\x89\xec\x5d"); // Pop the entire stack
    patch.AddBytes("\x8D\x51\x01"); // lea edx,dword pointer ds:[ecx+1]
    patch.AddBytes("\x8A\x01"); // mov al,byte ptr ds:[ecx]
    patch.AddBytes("\x41"); // inc ecx
    patch.AddBytes("\x84\xC0"); // test al, al
    patch.AddBytes("\x75\xF9"); // jne xxxxxxxx (Work around a broken jmp)
    patch.AddRelativeJump((char*)addr + 0x11);
    sASMPatcher.PatchAt(addr, &patch);
}

HOOK_METHOD(Console, Print, (const IsaacString& text, unsigned int color, unsigned int unk) -> void) {
    logViewer.AddLog("[CONSOLE] %s", text.Get()); //TODO figure out a clean way to pass through the color
    super(text, color, unk);
}