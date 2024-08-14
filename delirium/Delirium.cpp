#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "Log.h"
#include "LuaCore.h"
#include "Delirium.h"

extern "C" __declspec(dllexport) int ModInit(int argc, char** argv) {
	delirium::PatchSkipFrames();
	delirium::PatchCompanion();
	delirium::PatchVadeRetro();
	return 0;
}