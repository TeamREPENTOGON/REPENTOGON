#include <Windows.h>
#include <DbgHelp.h>
#include <ehdata_forceinclude.h>
#include <stdio.h>

#include "ASMPatcher.hpp"
#include "CrashHandler.h"
#include "HookSystem.h"
#include "HookSystem_private.h"
#include "IsaacRepentance.h"
#include "Log.h"
#include "SigScan.h"

#include <fstream>
#include <filesystem>
#include <sstream>

void (*loaderFinish)() = NULL;

/* Because ZHL is not initialized when we enter InitZHL, we cannot hook 
 * anything, so we need to ASM patch instead.
 */
void HookMain() {
	SigScan scan("32d2b9????????e84cd51000");
	if (!scan.Scan() || !loaderFinish)
		return;

	void* addr = scan.GetAddress();
	char* movOffsetAddr = (char*)addr + 0x3;
	void* movOffsetValue;
	memcpy(&movOffsetValue, movOffsetAddr, 0x4);
	printf("Found KAGE::EngineStartup hook address %p\n", addr);
	ASMPatch patch;
	ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.PreserveRegisters(registers);
	patch.AddInternalCall(loaderFinish);
	patch.RestoreRegisters(registers);
	ByteBuffer buffer;
	buffer.AddString("\x32\xd2\xb9");
	buffer.AddPointer(movOffsetValue);
	patch.AddBytes(buffer);
	patch.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

extern "C" {
	__declspec(dllexport) int InitZHL(void (*loaderFinishPtr)())
	{
#ifdef ZHL_LOG_FILE
		std::ofstream f(ZHL_LOG_FILE, std::ios_base::app);
		f.close();

		std::filesystem::path log_file(ZHL_LOG_FILE);
		if (!std::filesystem::is_regular_file(log_file)) {
			std::ostringstream err;
			err << "The specified ZHL_LOG_FILE " << ZHL_LOG_FILE << " is not a regular file";
			MessageBox(0, err.str().c_str(), "Error", MB_ICONERROR);
			ExitProcess(1);
		}
#endif
		if (!FunctionDefinition::Init())
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
				MessageBox(0, "Multiple function / variables were not found.\nIf you're a user facing this issue, you might be using an unsupported version of the game.", "Error", MB_ICONERROR);
			}

			return 1;
		}

		ASMPatch::_Init();
		ASMPatch::SavedRegisters::_Init();

		loaderFinish = loaderFinishPtr;

		HookMain();

		return 0;
	}
}