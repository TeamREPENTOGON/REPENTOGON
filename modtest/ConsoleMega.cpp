#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "OpenGL.h"
#include <GL/GL.h>

// Don't get too excited yet, I'm just filling out the hooks I'll need.

extern HDC glHdc;
extern HWND glWindow;
extern HGLRC glContext;

HOOK_METHOD(Console, FillOmnicomplete, () -> void) {
	// TODO
	super();
}

HOOK_METHOD(Console, ProcessInput, () -> void) {
	//TODO
	super();
}

HOOK_METHOD(Console, Render, () -> void) {
	//TODO
	super();

}

/*HOOK_METHOD(Console, RenderHistory, (Font* font, float unk, float unk2, bool unk3) -> std_deque_console_historyentry) {
	//TODO
	printf("%g, %g, %s\n", unk, unk2, unk3 ? "true" : "false");
	float what = super(font, unk, unk2, unk3);
	printf("%g\n", what);
	return what;
}*/