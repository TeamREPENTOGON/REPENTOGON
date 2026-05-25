#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../LuaInterfaces/_Internals.h"
#include <glad/glad.h>
#include "Log.h"

namespace GL {
	static HMODULE OpenGLLibrary;
}

static void* load_fn(const char* name) {
	void* addr = wglGetProcAddress(name);
	if (!addr) {
		addr = GetProcAddress(GL::OpenGLLibrary, name);
	}
	ZHL::Log("[OpenGL] %s -> %p\n", name, addr);
	return addr;
}

static void load_gl()
{
    GL::OpenGLLibrary = LoadLibrary("opengl32.dll");
    if (!GL::OpenGLLibrary || !gladLoadGLLoader(load_fn)) {
        ZHL::Log("[FATAL] Unable to load OpenGL\n");
        abort();
    }

    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(GL::OpenGLErrorCallback, 0);
}

HOOK_GLOBAL(KAGE_EngineStartup, (int numArgs, char** args) -> void, __cdecl)
{
    super(numArgs, args);
    ZHL::Log("REPENTOGON: Initializing OPENGL\n");
    load_gl();
}

HOOK_STATIC(Manager, Init, () -> void, __stdcall)
{
    super();
    LuaInternals::detail::RaiseInitError();
}