#include "HookSystem.h"
#include "LuaJITState.h"

extern "C" __declspec(dllexport) int ModInit(int argc, char** argv) {
	printf("luajit test\n");

    ZHL::Init();
	LuaJITState::Init();

	return 0;
}