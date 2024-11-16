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

void (__stdcall*loaderFinish)() = NULL;

void __stdcall SanityCheckInitZHL() {
	ZHL::Log("SanityCheckInitZHL: patch should be okay\n");
}

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
	ZHL::Log("Found KAGE::EngineStartup call address %p\n", addr);
	ZHL::Log("Dumping ASM: ");
	ZHL::DumpMemory(addr, 0x20, false, true);
	ASMPatch patch;
	ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.PreserveRegisters(registers);
	patch.AddInternalCall(SanityCheckInitZHL);
	patch.AddInternalCall(loaderFinish);
	patch.RestoreRegisters(registers);
	ByteBuffer buffer;
	buffer.AddString("\x32\xd2\xb9");
	buffer.AddPointer(movOffsetValue);
	patch.AddBytes(buffer);
	patch.AddRelativeJump((char*)addr + 0x7);
	size_t patchLen = 0;
	void* patchedAddr = sASMPatcher.PatchAt(addr, &patch, &patchLen);

	ZHL::Log("Dumping ASM of the call to KAGE::Engine post patch: ");
	ZHL::DumpMemory(addr, 0x20, false, true);
	ZHL::Log("Dumping ASM of the patch: ");
	ZHL::DumpMemory(patchedAddr, patchLen, false, true);
}

extern "C" {
	__declspec(dllexport) int InitZHL(void (__stdcall*loaderFinishPtr)())
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
		ZHL::ClearLogFile();
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