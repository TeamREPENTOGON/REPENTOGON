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
	bool result = scanner.Scan();
	if (!result) {
		throw std::runtime_error("Failed to find address!\n");
	}
	else {
		addr = scanner.GetAddress();
	}

	void* region = VirtualAlloc(NULL, 0x5, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (region == NULL) {
		throw std::runtime_error("Unable to allocate memory\n");
	}

	MEMORY_BASIC_INFORMATION info;
	DWORD old_protect = SetPageMemoryRW(addr, &info);
	char _code[] = {
		0xE9, 0, 0, 0, 0
	};

	char* code = (char*)malloc(sizeof(_code));
	memcpy(code, _code, 0x5);

	void* offset = (char*)addr + 0x6;
	ptrdiff_t jmp = (char*)offset - ((char*)region + 0x5);

	memcpy(code + 0x5 - 4, &jmp, sizeof(jmp));

	memcpy(region, code, 0x5);


	MEMORY_BASIC_INFORMATION new_region_info;
	VirtualQuery(region, &new_region_info, sizeof(new_region_info));
	DWORD _dummy;
	VirtualProtect(new_region_info.BaseAddress, new_region_info.RegionSize, PAGE_EXECUTE_READ, &_dummy);

	char override_base[] = {
		0xe9, 0x00, 0x00, 0x00, 0x00, 0x90
	};

	ptrdiff_t jump_dist = (char*)region - ((char*)addr + 0x5);
	memcpy(override_base + 1, &jump_dist, sizeof(jump_dist));
	memcpy(addr, override_base, 6);

	VirtualProtect(info.BaseAddress, info.RegionSize, old_protect, &_dummy);

	FlushInstructionCache(GetModuleHandle(NULL), NULL, 0);

	return addr;
}

// INPUT_ACTION is called in four separate places, and the check is duplicated and identical across all four.
// In all four cases, we jump past the check.
// This will allow mods to check button presses on the main menu again, like in AB+.
void PatchInputActionMenuCheck() {
	printf("[REPENTOGON] ASM patch: MC_INPUT_ACTION on main menu\n");
	printf("Found all four main menu patch addresses: %p, %p, %p, %p\n",
		PerformInputActionMenuPatch("837f??0275??833d????????0074??8d45??505351"),
		PerformInputActionMenuPatch("8378??0275??833d????????00"),
		PerformInputActionMenuPatch("837f??0275??833d????????0074??8d45??50536a00"),
		PerformInputActionMenuPatch("837f??0275??833d????????0074??8d45??50536a01"));
}