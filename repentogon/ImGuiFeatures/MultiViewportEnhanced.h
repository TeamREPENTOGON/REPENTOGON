#pragma once
#include <windows.h>

struct RgonImGuiMultiViewportConfig {
	HGLRC mainGLContextForCreateImGuiWindow;
	HWND mainGameWindowForCreateImGuiWindow;
};

extern RgonImGuiMultiViewportConfig rgonImGuiMultiViewportConfig;

void ImGui_ImplRepentogon_InitMultiViewport();
void ImGui_ImplRepentogon_FixFullScreenViewportForNextWindow();