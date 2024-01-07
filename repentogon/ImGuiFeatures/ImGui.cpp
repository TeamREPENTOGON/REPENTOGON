#include "ASMPatcher.hpp"
#include "ConsoleMega.h"
#include "CustomImGui.h"
#include "GameOptions.h"
#include "HelpMenu.h"
#include "HookSystem.h"
#include "PerformanceWindow.h"
#include "NotificationHandler.h"
#include "IsaacRepentance.h"
#include "LogViewer.h"
#include "SavedataHandler.h"
#include "SigScan.h"
#include "IconsFontAwesome6.h"
#include "UnifontSupport.h"
#include "Lang.h"

#include <Windows.h>
#include <format>
#include <gl/GL.h>
#include <sstream>
#include <algorithm>

#include "imgui.h"
#include "imgui_freetype.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "../MiscFunctions.h"
#include "../REPENTOGONOptions.h"

// this blogpost https://werwolv.net/blog/dll_injection was a big help, thanks werwolv!

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND window;
WNDPROC windowProc;
bool menuShown = false;
bool leftMouseClicked = false;
bool settingsLoaded = false;
std::string iniFilePath;
static bool imguiInitialized = false;
static bool show_app_style_editor = false;
static bool shutdownInitiated = false;
static bool imguiResized;
static ImVec2 imguiSizeModifier;

HelpMenu helpMenu;
LogViewer logViewer;
PerformanceWindow performanceWindow;
GameOptionsWindow gameOptionsWindow;
ConsoleMega console;
CustomImGui customImGui;
NotificationHandler notificationHandler;

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
		ImGui::MenuItem(LANG.IMGUI_WIN_CTX_MENU_PIN_WINDOW, NULL, pinned);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal) && ImGui::BeginTooltip()) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(LANG.IMGUI_WIN_CTX_MENU_PIN_WINDOW_DESC);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::EndPopup();
	}
}

ImGuiKey AddChangeKeyButton(bool isController, bool& wasPressed)
{
	if (ImGui::Button(LANG.IMGUI_CHANGE_KEY_BTN_NAME)) {
		wasPressed = true;
	}

	if (wasPressed && ImGui::BeginTooltip()) {
		ImGui::SetWindowFocus();
		int firstKey = static_cast<int>(ImGuiKey_Tab);
		int lastKey = static_cast<int>(ImGuiKey_KeypadEqual);
		if (isController) {
			firstKey = static_cast<int>(ImGuiKey_GamepadStart);
			lastKey = static_cast<int>(ImGuiKey_GamepadRStickDown);
			ImGui::Text(LANG.IMGUI_CHANGE_KEY_BTN_PRESS_KEY_CTRL);
		}
		else {
			ImGui::Text(LANG.IMGUI_CHANGE_KEY_BTN_PRESS_KEY_KEYBOARD);
		}
		ImGui::Text(LANG.IMGUI_CHANGE_KEY_BTN_PRESS_ESC);

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

// extended ImGui::Begin function to allow for custom savedata loading
bool WindowBeginEx(const char* name, bool* p_open, ImGuiWindowFlags flags = 0) {
	ImGuiWindow* window = ImGui::FindWindowByName(name);
	const bool window_just_created = (window == NULL);

	bool open = ImGui::Begin(name, p_open, flags);

	if (window_just_created) { // load vanilla window settings after window was created
		CustomImGuiWindowSettings* settings = FindWindowSettingsByIDEx(ImHashStr(name));
		window = ImGui::FindWindowByName(name);
		if (settings != nullptr && window != nullptr)
		{
			ApplyWindowSettings(window, settings);
		}
	}
	return open;
}

void SaveSettings() {
	ImGuiContext& g = *GImGui;
	console.SaveSettings(FindWindowSettingsByIDEx(ImHashStr(console.windowName.c_str())));
		logViewer.SaveSettings(FindWindowSettingsByIDEx(ImHashStr(logViewer.windowName.c_str())));
		performanceWindow.SaveSettings(FindWindowSettingsByIDEx(ImHashStr(performanceWindow.windowName.c_str())));
		gameOptionsWindow.SaveSettings(FindWindowSettingsByIDEx(ImHashStr(gameOptionsWindow.windowName.c_str())));
	
	for (auto window = customImGui.windows->begin(); window != customImGui.windows->end(); ++window) {
		CustomImGuiWindowSettings* settings = FindWindowSettingsByIDEx(ImHashStr(window->name.c_str()));
		if (settings == nullptr) continue;
		settings->Visible = window->evaluatedVisibleState;
		settings->Pinned = window->data.windowPinned;
	}
}

void LoadSettings() {
	console.LoadSettings(FindWindowSettingsByIDEx(ImHashStr(console.windowName.c_str())));
	logViewer.LoadSettings(FindWindowSettingsByIDEx(ImHashStr(logViewer.windowName.c_str())));
	performanceWindow.LoadSettings(FindWindowSettingsByIDEx(ImHashStr(performanceWindow.windowName.c_str())));
	gameOptionsWindow.LoadSettings(FindWindowSettingsByIDEx(ImHashStr(gameOptionsWindow.windowName.c_str())));

	for (auto window = customImGui.windows->begin(); window != customImGui.windows->end(); ++window) {
		CustomImGuiWindowSettings* settings = FindWindowSettingsByIDEx(ImHashStr(window->name.c_str()));
		if (settings == nullptr) continue;
		window->SetVisible(settings->Visible);
		//window->data.windowPinned = settings->Pinned;  // Doesnt work right now. window gets positioned wrong on game startup :(
	}
}

// Original imgui implementation of this function was called by NewFrame()
void UpdateImGuiSettings()
{
	// Load settings on first frame
	ImGuiContext& g = *GImGui;
	if (!settingsLoaded) {
		ImGui::LoadIniSettingsFromDisk(iniFilePath.c_str());
		LoadSettings();
		settingsLoaded = true;
	}

	// Save settings when we get the signal from imgui
	if (g.IO.WantSaveIniSettings) {
		SaveSettings();
		ImGui::SaveIniSettingsToDisk(iniFilePath.c_str());
		g.IO.WantSaveIniSettings = false;
	}
}

inline void handleImguiInput(const char bytes[], int len) {
	wchar_t w;
	int has_val = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, bytes, len, &w, 1);
	if (has_val && ImGui::GetCurrentContext()) {
		ImGui::GetIO().AddInputCharacter(w);
	}
}

bool handleImguiInputDBCS(WPARAM wParam, LPARAM lParam) {
	static char bytes[2] = { 0,0 };
	if (bytes[0] == 0) {
		if (!IsDBCSLeadByte(wParam)) {
			return false;
		}
		bytes[0] = wParam;
		return true;
	}
	else {
		bytes[1] = wParam;
		handleImguiInput(bytes, 2);
		bytes[0] = 0;
		return true;
	}
}

// Not work for chinese IME in Win11, the only thing I got is '??'
// probably it works with other language.
bool handleImguiInputUTF8(WPARAM wParam, LPARAM lParam) {
	static int byte_length = 0;
	static int next_byte = 0;
	static char bytes[4] = { 0 };
	if (0 == (wParam & 0x80)) {
		byte_length = next_byte = 0;
		return false; // ascii code
	}
	if (byte_length) {
		if ((wParam & 0xC0) == 0x80) {
			bytes[next_byte++] = wParam;
			if (next_byte == byte_length) {
				handleImguiInput(bytes, byte_length);
				byte_length = next_byte = 0;
			}
			return true;
		}
		else {
			byte_length = next_byte = 0;
			return false;
		}
	}
	else {
		bytes[next_byte++] = wParam;
		if ((wParam & 0xE0) == 0xC0) { // 110xxxxx
			byte_length = 2;
		}
		else if ((wParam & 0xF0) == 0xE0) { // 1110xxxx
			byte_length = 3;
		}
		else if ((wParam & 0xF8) == 0xF0) { // 11110xxx
			byte_length = 4;
		}
		else {
			byte_length = 0;
		}
		return true;
	}
}

static std::vector<WPARAM> pressedKeys;

LRESULT CALLBACK windowProc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (shutdownInitiated)
		return CallWindowProc(windowProc, hWnd, uMsg, wParam, lParam);

	// Enable the overlay using the grave key, disable using ESC
	if (uMsg == WM_KEYDOWN) {
		ImGui::CloseCurrentPopup();

		switch (wParam) {
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

	std::vector<UINT> blockedMsgs = { WM_KEYUP, WM_KEYDOWN, WM_KEYFIRST, WM_KEYLAST, WM_MOUSEMOVE,
			WM_LBUTTONUP, WM_LBUTTONDOWN, WM_LBUTTONDBLCLK,
			WM_MBUTTONDOWN, WM_MBUTTONUP, WM_MBUTTONDBLCLK,
			WM_RBUTTONDOWN, WM_RBUTTONUP, WM_RBUTTONDBLCLK };

	// If the overlay is shown, direct input to the overlay only
	// Otherwise call the game's WndProc function
	if (menuShown) {
		// Block the game's WndProc on input related uMsgs but allow any others through
		if (std::find(blockedMsgs.begin(), blockedMsgs.end(), uMsg) == blockedMsgs.end()) {
			CallWindowProc(windowProc, hWnd, uMsg, wParam, lParam);
		}

		//we should manually handle multibyte input
		//https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
		if (uMsg == WM_CHAR) {
			switch (GetACP()) {
			case 932: //shift_jis		Japanese
			case 936: //gb2312			Chinese Simplified
			case 949: //ks_c_5601-1987	Korean
			case 950: //big5			Chinese Traditional
				if (handleImguiInputDBCS(wParam, lParam))
					return true;
				break;
			case 65001:
				if (handleImguiInputUTF8(wParam, lParam))
					return true;
				break;

			}
		}

		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}



	return CallWindowProc(windowProc, hWnd, uMsg, wParam, lParam);
}

ImFont* imFontUnifont = NULL;

HOOK_GLOBAL(OpenGL::wglSwapBuffers, (HDC hdc)->bool, __stdcall)
{
	static std::map<int, ImFont*> fonts;

	static float unifont_global_scale = 1;

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
		io.IniFilename = NULL; // Disable vanilla .ini file management
		iniFilePath = std::string(REPENTOGON::GetRepentogonDataPath()) + "imgui.ini";

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

		RegisterSaveDataHandler();

		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		static UnifontRange unifont_ranges;

		float font_base_size = 13;
		cfg.MergeMode = false;
		cfg.SizePixels = font_base_size;
		if (!repentogonOptions.enableUnifont) {
			imFontUnifont = io.Fonts->AddFontDefault(&cfg);
			cfg.MergeMode = true;
		}
		else {
			// the pixel perfect size for unifont is 16px
			float size_config[5][2] = {
				{13,1},
				{16,1},
				{14,1},//12px unifont can't tell 3 and 9, so use 14 here
				{16,0.5},
				{8,1}
			};
			font_base_size = size_config[repentogonOptions.unifontRenderMode][0];
			unifont_global_scale = size_config[repentogonOptions.unifontRenderMode][1];

			if (repentogonOptions.unifontRenderMode == UNIFONT_RENDER_NORMAL) {
				imFontUnifont = io.Fonts->AddFontDefault(&cfg); // this font is better for english word, but only perfect in 13px
			}
			else {
				cfg.SizePixels = font_base_size;
				imFontUnifont = io.Fonts->AddFontFromFileTTF("resources-repentogon\\fonts\\unifont-15.1.04.otf", font_base_size, &cfg, ImGui::GetIO().Fonts->GetGlyphRangesDefault());
			}
			cfg.MergeMode = true;
			io.Fonts->AddFontFromFileTTF("resources-repentogon\\fonts\\unifont-15.1.04.otf", font_base_size, &cfg, unifont_ranges.Get());
		}
		ImGui::GetIO().FontDefault = imFontUnifont;
		// icon font
		cfg.GlyphOffset = ImVec2(0, 1.5f); // move icon a bit down to center them in objects
		cfg.RasterizerDensity = 5; // increase DPI, to make icons look less fucked by the rasterizer
		io.Fonts->AddFontFromFileTTF("resources-repentogon\\fonts\\Font Awesome 6 Free-Solid-900.otf", font_base_size, &cfg, icon_ranges);
	
		imguiInitialized = true;
		ImGui::GetIO().FontAllowUserScaling = true;
		logViewer.AddLog("[REPENTOGON]", "Initialized Dear ImGui v%s\n", IMGUI_VERSION);
		printf("[REPENTOGON] Dear ImGui v%s initialized! Any further logs can be seen in the in-game log viewer.\n", IMGUI_VERSION);
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	UpdateImGuiSettings();
;
	if (g_PointScale > 0) {
		imFontUnifont->Scale = g_PointScale * unifont_global_scale;
		ImGui::GetStyle().FramePadding.y = 4 * g_PointScale * unifont_global_scale;
		ImGui::GetStyle().ItemSpacing.x = 6 * g_PointScale * unifont_global_scale;
	}
		

	static bool unifont_tex_nearest = false;
	if(!unifont_tex_nearest)
	{
		unifont_tex_nearest = true;
		// use nearest scale to ensure unifont is pixel perfect. must do this after ImGui_ImplOpenGL3_NewFrame()
		GLint last_texture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		glBindTexture(GL_TEXTURE_2D, (GLuint)imFontUnifont->ContainerAtlas->TexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, last_texture);
	}
	
	imguiResized = false;
	static ImVec2 oldSize = ImVec2(0, 0);
	if ((oldSize.x != ImGui::GetMainViewport()->Size.x || oldSize.y != ImGui::GetMainViewport()->Size.y) &&
		(oldSize.x > 1 && oldSize.y > 1) &&
		(ImGui::GetMainViewport()->Size.x > 1 && ImGui::GetMainViewport()->Size.y > 1)) { // no operator? (megamind stares intently at the camera)
		imguiResized = true;
		imguiSizeModifier = ImVec2(ImGui::GetMainViewport()->Size.x / oldSize.x, ImGui::GetMainViewport()->Size.y / oldSize.y);
	}
	oldSize = ImGui::GetMainViewport()->Size;

	if (menuShown) {
		if (ImGui::BeginMainMenuBar()) {
			ImGui::MenuItem(ICON_FA_CHEVRON_LEFT"",NULL,&menuShown);
			if (ImGui::BeginMenu(LANG.BAR_TOOLS)) {
				ImGui::MenuItem(LANG.BAR_DEBUG_CONSOLE, NULL, &console.enabled);
				ImGui::MenuItem(LANG.BAR_LOG_VIEWER, NULL, &logViewer.enabled);
				ImGui::MenuItem(LANG.BAR_GAME_OPTIONS, NULL, &gameOptionsWindow.enabled);
				ImGui::MenuItem(LANG.BAR_PERFORMANCE, NULL, &performanceWindow.enabled);
				ImGui::MenuItem(LANG.BAR_STYLE_EDITOR, NULL, &show_app_style_editor);
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
	gameOptionsWindow.Draw(menuShown);
	LANG.DrawReportWindow(menuShown);

	customImGui.DrawWindows(menuShown);

	if (show_app_style_editor) {
		WindowBeginEx(LANG.DEAR_IMGUI_STYLE_EDITOR_WIN_NAME, &show_app_style_editor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	notificationHandler.Draw(menuShown);

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
extern bool WindowBeginEx(const char* name, bool* p_open, ImGuiWindowFlags flags);
