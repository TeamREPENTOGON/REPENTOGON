#include <stdio.h>
#include <stdexcept>

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"

// Much of this is adapted from Sylmir's WIP Delirium patches, just adapted for use with the LibZHL sigscanner.
// Thanks Sylmir <3
DWORD SetPageMemoryRW(void* addr, MEMORY_BASIC_INFORMATION* info) {
    VirtualQuery(addr, info, sizeof(*info));
    DWORD old_protect;
    VirtualProtect(info->BaseAddress, info->RegionSize, PAGE_READWRITE, &old_protect);
    return old_protect;
}

int ambushWaves = 3;

void PatchHardcodedAmbushWaveCount() {
	printf("[REPENTOGON] ASM patch: hardcoded wave count\n");


	// This signature takes us straight to the small block that calculates how many ambush waves should happen.
	SigScan scanner("33db83f801");
	void* addr;
	bool result = scanner.Scan();
	if (!result) {
		throw std::runtime_error("Failed to find address!\n");
	}
	else {
		addr = scanner.GetAddress();
		printf("Found address for hardcoded challenge waves: %p\n", addr);
	}

	// First, we allocate memory for our new ASM to live in
	// 0x17 is the length of our code block below
	void* region = VirtualAlloc(NULL, 0x17, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (region == NULL) {
		throw std::runtime_error("Unable to allocate memory\n");
	}

	printf("Allocated memory for PatchHardcodedAmbushWaveCount at %p\n", region);

	// Set permissions on memory
	MEMORY_BASIC_INFORMATION info;
	DWORD old_protect = SetPageMemoryRW(addr, &info);

	/* This is the assembly code which will live in our new memory region, manually denoted in hexadecimal format.
	   We piggyback off of the game code already setting up the boss room check for us in the EAX register-
		 we simply execute the comparison, and act accordingly.
	   If the comparison matches, move 2 to the EBX register, where the wave count temporarily lives.
	   Otherwise, move a pointer to ambushWaves to the EBX register.

	   We create a skeleton, with placeholder values (denoted as XXXXXXXX in the comments).
	   These values are calculated, filled, and copied to the memory block we allocate.
	 */

	char _code[] = {
		// cmp eax, 1 (Check against the pre-set comparison.)
		0x83, 0xF8, 0x01, // 0x3
		// jne XXXXXXXX (If we're not a boss challenge room, jump 0x7 bytes ahead.)
		0x75, 0x07, // 0x5
		// mov ebx, 2 (Move the value 2 to the EBX register.)
		0xBB, 0X02, 0, 0, 0, // 0xA
		// jmp isaac-ng.XXXXXXXX (Jump 0x6 bytes ahead)
		0xEB, 0x06, // 0xC
		// mov ebx, dword ptr ds:[0xXXXXXXXX] (Move the value stored in our ambushWaves pointer to the EBX register.)
		0x8B, 0x1D, 0, 0, 0, 0, // 0x12
		// jmp isaac-ng.XXXXXXXX (Jump to the original code.)
		0xE9, 0, 0, 0, 0 // 0x17
	};

	// Create a pointer to ambushWaves, so we can dynamically update it in Lua
	void* ptr = &ambushWaves;

	// Allocate new memory, and copy code to our block.
	char* code = (char*)malloc(sizeof(_code));
	memcpy(code, _code, 0x17);

	// Copy the pointer address to the proper position in the block.
	memcpy(code + 0x12 - 4, &ptr, sizeof(ptr));

	// Copy the JMP address to the proper position in the block.
	// We need to jump 0xB past our original block to bypass the vanilla calculation properly.
	void* offset = (char*)addr + 0xB;
	ptrdiff_t jmp = (char*)offset - ((char*)region + 0x17);

	memcpy(code + 0x17 - 4, &jmp, sizeof(jmp));

	// Finally, copy the resulting code to our allocated memory.
	memcpy(region, code, 0x1C);


	MEMORY_BASIC_INFORMATION new_region_info;
	VirtualQuery(region, &new_region_info, sizeof(new_region_info));
	DWORD _dummy;
	VirtualProtect(new_region_info.BaseAddress, new_region_info.RegionSize, PAGE_EXECUTE_READ, &_dummy);

	// At the original address, override the instruction with a JMP to our block.
	char override_base[] = {
		0xe9, 0x00, 0x00, 0x00, 0x00
	};

	ptrdiff_t jump_dist = (char*)region - ((char*)addr + 0x5);
	memcpy(override_base + 1, &jump_dist, sizeof(jump_dist));
	memcpy(addr, override_base, 5);

	VirtualProtect(info.BaseAddress, info.RegionSize, old_protect, &_dummy);

	FlushInstructionCache(GetModuleHandle(NULL), NULL, 0);
}