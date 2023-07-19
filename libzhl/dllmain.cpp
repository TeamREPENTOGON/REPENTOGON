#include <Windows.h>
#include <stdio.h>
#include "HookSystem_private.h"
#include "ASMPatcher.hpp"

#include <fstream>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		if(!FunctionDefinition::Init())
		{
			auto const& missing = Definition::GetMissing();
			if (missing.size() == 1) {
				MessageBox(0, FunctionDefinition::GetLastError(), "Error", MB_ICONERROR);
			}
			else {
				std::ofstream out("libzhl.log");
				for (auto const& [isFunction, name] : missing) {
					out << name << " (";
					if (isFunction) {
						out << "function";
					}
					else {
						out << "variable";
					}
					out << ")" << std::endl;
				}
				MessageBox(0, "Multiple function / variables were not found", "Error", MB_ICONERROR);
			}
			ExitProcess(1);
		}
	}

	ASMPatch::_Init();
	ASMPatch::SavedRegisters::_Init();
	return TRUE;
}
