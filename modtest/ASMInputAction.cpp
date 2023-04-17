#include <stdio.h>
#include <stdexcept>

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMCore.h"

// This process is better documented in ASMAmbush.cpp.
void* PerformInputActionMenuPatch(const char* signature) {
	SigScan scanner(signature);
	void* addr;
	if (!scanner.Scan()) {
		throw std::runtime_error("Failed to find address!\n");
	}
	else {
		addr = scanner.GetAddress();
	}


	MEMORY_BASIC_INFORMATION info;
	DWORD old_protect = SetPageMemoryRW(addr, &info);
	DWORD _dummy;

	char override_base[] = {
		// jmp isaac-ng.XXXXXXXX
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90
	};
	memcpy(addr, override_base, 6);


	VirtualProtect(info.BaseAddress, info.RegionSize, old_protect, &_dummy);

	FlushInstructionCache(GetModuleHandle(NULL), NULL, 0);

	return addr;
}

// INPUT_ACTION is called in four separate places, and the check is duplicated and identical across all four.
// In all four cases, we can just replace the check with six NOPs to remove the check.
// This will allow mods to check button presses on the main menu again, like in AB+.
void PatchInputActionMenuCheck() {
	printf("[REPENTOGON] ASM patch: MC_INPUT_ACTION on main menu\n");
	printf("Found all four main menu patch addresses: %p, %p, %p, %p\n",
		PerformInputActionMenuPatch("837f??0275??833d????????0074??8d45??505351"),
		PerformInputActionMenuPatch("8378??0275??833d????????00"),
		PerformInputActionMenuPatch("837f??0275??833d????????0074??8d45??50536a00"),
		PerformInputActionMenuPatch("837f??0275??833d????????0074??8d45??50536a01"));
}