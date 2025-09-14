#include <Windows.h>
#include <DbgHelp.h>
#include <ehdata_forceinclude.h>
#include <stdio.h>

#include "ASMDefinition.h"
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
#include <emmintrin.h>  

void (__stdcall*loaderFinish)() = NULL;

void __stdcall SanityCheckInitZHL() {
	ZHL::Log("SanityCheckInitZHL: patch should be okay\n");
}

/* Because ZHL is not initialized inside libzhl, we cannot use HOOK_GLOBAL here,
 * because the static constructor would be invoked before ZHL is ready.
 */
void HookMain(void** poisonPtr, char* poisonValue) {
	FunctionDefinition* definition = FunctionDefinition::Find("IsaacMain", typeid(int (*)(int, char**)));
	if (!definition) {
		ZHL::Log("[CRITICAL][HookMain] main was not found in the executable\n");
		return;
	}

	if (!loaderFinish) {
		ZHL::Log("[CRITICAL][HookMain] No termination hook given\n");
	}

	void* addr = definition->GetAddress();
	{
		ZHL::Log("[INFO][HookMain] Flat patching main at %p to heal a single byte\n");
		ASMPatch patch;
		patch.AddBytes("\x55"); // push ebp, this restores the poison byte
		sASMPatcher.FlatPatch(addr, &patch);
	}

	char* patchAddr = (char*)addr + 0x3; /* Patch immediately after ebp and esp are setup. */
	char brokenBytes[7]; /* Backup all bytes that will be broken by the patch. */
	static_assert (sizeof(char) == 1);
	memcpy(brokenBytes, patchAddr, sizeof(brokenBytes));

	ZHL::Log("[INFO][HookMain] Found main at address %p\n", addr);
	ZHL::Log("[INFO][HookMain] Dumping 32 bytes of ASM for log sanity: ");
	ZHL::DumpMemory(addr, 0x20, false, true);
	ASMPatch patch;
	ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.PreserveRegisters(registers);
	patch.AddInternalCall(SanityCheckInitZHL);
	patch.AddInternalCall(loaderFinish);
	patch.RestoreRegisters(registers);

	/* At the end of the patch, add the instructions that are broken by the
	 * jump towards the patch, then jump to the next instruction that should
	 * have been executed originally.
	 */
	ByteBuffer buffer;
	buffer.AddAny(brokenBytes, sizeof(brokenBytes));
	patch.AddBytes(buffer);
	patch.AddRelativeJump((char*)patchAddr + 0x7);

	{
		ZHL::Log("[INFO][HookMain] Flat patching main to poison the return byte of LoadMods\n");
		void* target = (char*)patchAddr + 0x7;
		*poisonPtr = target;
		*poisonValue = *(char*)target;

		_mm_sfence(); // Store fence to enforce the above writes to complete

		ASMPatch patch;
		patch.AddBytes("\xcc"); // int3, poison the first byte after the return
		sASMPatcher.FlatPatch((char*)patchAddr + 0x7, &patch);
	}

	size_t patchLen = 0;
	void* patchedAddr = sASMPatcher.PatchAt(patchAddr, &patch, &patchLen);

	ZHL::Log("[INFO][HookMain] Dumping 32 bytes of ASM for log sanity: ");
	ZHL::DumpMemory(addr, 0x20, false, true);
}

extern "C" {
	__declspec(dllexport) int InitZHL(void (__stdcall*loaderFinishPtr)(),
		void** poisonPtr, char* poisonValue)
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
		InitializeSymbolHandler();
		bool clearLog = false;
		if (!FunctionDefinition::Init())
		{
			auto const& missing = Definition::GetMissing();
			if (missing.size() == 1) {
				MessageBox(0, FunctionDefinition::GetLastError(), "Error", MB_ICONERROR);
			}
			else {
				std::ofstream out("repentogon.log");
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
				MessageBox(0, "Multiple function / variables were not found.\n"
					"If you are an end-user facing this issue, you might be using an unsupported version of the game.",
					"Error", MB_ICONERROR);
			}

			return 1;
		}

		if (!sASMDefinitionHolder->Init()) {
			std::vector<const char*> const& misses = sASMDefinitionHolder->GetMisses();
			std::ostringstream stream;
			stream << "Unable to find address for " << misses.front();
			if (misses.size() == 1) {
				MessageBox(0, stream.str().c_str(), "Error", MB_ICONERROR);
			} else {
				std::ofstream stream("repentogon.log", clearLog ? std::ios_base::out : std::ios_base::app);
				for (const char* miss : misses) {
					stream << "Unable to find address for " << miss << std::endl;
				}
				MessageBox(0, "Addresses for ASM patches were not found.\n"
					"If you are an end-user facing this issue, you might be using an unsupported version of the game.",
					"Error", MB_ICONERROR);
			}
		}

		FunctionDefinition::UpdateHooksStateFromJSON("hooks.json");
		ASMPatch::_Init();
		ASMPatch::SavedRegisters::_Init();

		loaderFinish = loaderFinishPtr;

		HookMain(poisonPtr, poisonValue);

		return 0;
	}

	__declspec(dllexport) int OfflineScan(void*) {
		char filename[4096];
		time_t now = time(nullptr);
		tm* nowtm = localtime(&now);
		strftime(filename, 4096, "offline_scanning_%Y-%m-%d_%H-%M-%S.log", nowtm);
		FILE* f = fopen(filename, "w");
		if (!f) {
			char buffer[4096];
			sprintf(buffer, "Unable to open log file %s", filename);
			MessageBoxA(0, buffer, "Fatal error", MB_ICONERROR);
			return -1;
		}

		std::vector<SigScanEntry> entries;
		Definition::OfflineInit(entries);

		for (SigScanEntry const& entry : entries) {
			size_t nMatches = entry.locations.size();

			fprintf(f, "%s %s, signature %s, %u matches: ",
				entry.isFunction ? "Function" : "Variable",
				entry.name.c_str(),
				entry.signature.c_str(),
				nMatches);

			if (nMatches) {
				for (void* addr : entry.locations) {
					fprintf(f, "%p ", addr);
				}
			}

			fprintf(f, "\n");
		}

		fclose(f);

		return 0;
	}
}