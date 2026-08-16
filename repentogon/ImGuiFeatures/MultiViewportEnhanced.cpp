#include "MultiViewportEnhanced.h"
#include "imgui_internal.h"
#include <map>
#include "IsaacRepentance.h"
#include "../REPENTOGONOptions.h"
/*

The following struct SHOULD be checked when upgrade imgui.
It's copy from ImGui's win32 impl, not a public api.

*/
struct ImGui_ImplWin32_ViewportData
{
	HWND    Hwnd;               // Stored in ImGuiViewport::PlatformHandle + PlatformHandleRaw
	HWND    HwndParent;
	bool    HwndOwned;
	DWORD   DwStyle;
	DWORD   DwExStyle;

	ImGui_ImplWin32_ViewportData() { Hwnd = HwndParent = nullptr; HwndOwned = false;  DwStyle = DwExStyle = 0; }
	~ImGui_ImplWin32_ViewportData() { IM_ASSERT(Hwnd == nullptr); }
};

RgonImGuiMultiViewportConfig rgonImGuiMultiViewportConfig = { 0,0 };

std::map<HWND, HGLRC> RepentogonRendererMap;

void Repentogon_Renderer_CreateWindow(ImGuiViewport* vp) {
	ImGui_ImplWin32_ViewportData* vd = (ImGui_ImplWin32_ViewportData*)vp->PlatformUserData;
	
	SetWindowLongPtr(vd->Hwnd, GWLP_HWNDPARENT, (LONG_PTR)rgonImGuiMultiViewportConfig.mainGameWindowForCreateImGuiWindow);

	HDC dc = GetDC(vd->Hwnd);
	
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	const int pf = ::ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pf, &pfd);

	typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
	const PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	const int attribs[] =
	{
		0x2091, 2,      // WGL_CONTEXT_MAJOR_VERSION_ARB
		0x2092, 0,      // WGL_CONTEXT_MINOR_VERSION_ARB
		0
	};
	HGLRC newRC = nullptr;
	if (wglCreateContextAttribsARB)
		newRC = wglCreateContextAttribsARB(dc, 0, attribs);

	if (!newRC) {
		ZHL::Log("[REPENTOGON] wglCreateContextAttribusARB error when create multiview window, error code = %lu\n", GetLastError());
	}

	RepentogonRendererMap[vd->Hwnd] = newRC;// wglCreateContext(dc);
	wglMakeCurrent(dc, newRC);

	wglShareLists(rgonImGuiMultiViewportConfig.mainGLContextForCreateImGuiWindow,newRC);


	ReleaseDC(vd->Hwnd, dc);
}
void Repentogon_Renderer_DestroyWindow(ImGuiViewport* vp) {
	ImGui_ImplWin32_ViewportData* vd = (ImGui_ImplWin32_ViewportData*)vp->PlatformUserData;
	HDC dc = GetDC(vd->Hwnd);
	auto it = RepentogonRendererMap.find(vd->Hwnd);
	if (it != RepentogonRendererMap.end()) {
		wglDeleteContext(it->second);
		RepentogonRendererMap.erase(it);
	}
	else {
		ZHL::Log("[REPENTOGON] can't destroy not exist window(hwnd=%lu, dc=%lu).\n", vd->Hwnd, dc);
	}
	ReleaseDC(vd->Hwnd, dc);
}
void Repentogon_Platform_RenderWindow(ImGuiViewport* vp, void* render_arg) {
	//this function should switch opengl context to the new window
	//then imgui will render it
	ImGui_ImplWin32_ViewportData* vd = (ImGui_ImplWin32_ViewportData*)vp->PlatformUserData;
	HDC dc = GetDC(vd->Hwnd);
	auto it = RepentogonRendererMap.find(vd->Hwnd);
	if (it != RepentogonRendererMap.end()) {
		wglMakeCurrent(dc, RepentogonRendererMap[vd->Hwnd]);
	}
	ReleaseDC(vd->Hwnd, dc);
}
// this is not in opengl nor in win32 impl, which is required by multi-viewport
void Repentogon_Platform_SwapBuffers(ImGuiViewport* vp, void* render_arg) {
	//should switch context to the new window, and run swapbuffer
	ImGui_ImplWin32_ViewportData* vd = (ImGui_ImplWin32_ViewportData*)vp->PlatformUserData;
	HDC dc = GetDC(vd->Hwnd);
	SwapBuffers(dc);
	ReleaseDC(vd->Hwnd, dc);
}

void ImGui_ImplRepentogon_InitMultiViewport() {
	ImGui::GetPlatformIO().Renderer_CreateWindow = Repentogon_Renderer_CreateWindow;
	ImGui::GetPlatformIO().Renderer_DestroyWindow = Repentogon_Renderer_DestroyWindow;
	ImGui::GetPlatformIO().Platform_RenderWindow = Repentogon_Platform_RenderWindow;
	ImGui::GetPlatformIO().Platform_SwapBuffers = Repentogon_Platform_SwapBuffers;
}
void ImGui_ImplRepentogon_DisableViewportAsNeedForNextWindow() {
	bool useMultiview = true;
	if (!repentogonOptions.enableImGuiMultiView)
		useMultiview = false;
	if (useMultiview && g_Manager) {
		auto opts = g_Manager->GetOptions();
		if (opts) {
			if (opts->_isFullscreen) {
				useMultiview = false;
			}
		}
	}
	if (!useMultiview)
		ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
}