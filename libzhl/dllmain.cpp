#include <Windows.h>
#include <stdio.h>
#include "HookSystem_private.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		if(!FunctionDefinition::Init())
		{
			MessageBox(0, FunctionDefinition::GetLastError(), "Error", MB_ICONERROR);
			ExitProcess(1);
		}
	}

	return TRUE;
}
