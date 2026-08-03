#pragma once
#include <windows.h>
extern HGLRC mainGLContextForCreateImGuiWindow;

struct ImGuiViewport;
void Repentogon_Renderer_CreateWindow(ImGuiViewport* vp);
void Repentogon_Renderer_DestroyWindow(ImGuiViewport* vp);
void Repentogon_Platform_RenderWindow(ImGuiViewport* vp, void* render_arg);
void Repentogon_Platform_SwapBuffers(ImGuiViewport* vp, void* render_arg);