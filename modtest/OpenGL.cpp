#include "IsaacRepentance.h"
#include "HookSystem.h"

HOOK_METHOD(OpenGL, wglSwapBuffers, (HDC hdc) -> bool) {
	return super(hdc);
}