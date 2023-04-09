#include <stdio.h>
#include <stdexcept>

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMCore.h"

// This process is better documented in ASMAmbush.cpp- apart from the ASM, only deviations from this are commented here.
bool overrideMegaSatanEnding = false;

/* Patch 1
 Replace the RNG::RandomInt call with a JMP to our own code. 
 We MOV a bool pointer into EAX, test EAX,EAX and if true, clean the stack and JMP avoiding the ending
 Otherwise, leave the stack intact, call RandomInt and continue with vanilla checks

 For an eventual patch 2, I'd like to be able to stop the void portal from spawning. I'll look into this at some other point.
*/
void PatchHardcodedMegaSatanGameEnding() {
	printf("[REPENTOGON] ASM patch: Mega Satan hardcoded 50%% game ending\n");


	SigScan scanner("e8????????85c00f85????????3845??0f84????????3845??0f84????????a1");
	void* addr;
	bool result = scanner.Scan();
	if (!result) {
		throw std::runtime_error("Failed to find address!\n");
	}
	else {
		addr = scanner.GetAddress();
		printf("Found address for Mega Satan comparison: %p\n", addr);
	}

	// We already have LibZHL looking for this, so we shouldn't need to sanity check this result
	SigScan randomIntScanner("558bec568bf18b16"); //RNG::RandomInt
	randomIntScanner.Scan();
	void* randomIntAddr = randomIntScanner.GetAddress();

	void* region = VirtualAlloc(NULL, 0x1B, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (region == NULL) {
		throw std::runtime_error("Unable to allocate memory\n");
	}

	printf("Allocated memory for PatchHardcodedMegaSatanGameEnding at %p\n", region);

	MEMORY_BASIC_INFORMATION info;
	DWORD old_protect = SetPageMemoryRW(addr, &info);

	char _code[] = {
		// mov eax, dword ptr ds:[XXXXXXXX] (Move our bool pointer into the EAX register.)
		0xA1, 0, 0, 0, 0, // 0x5
		// test eax, eax (Check to see if our bool equals false.)
		0x85, 0xC0, //0x7
		// jne isaac-ng.XXXXXXXX (If our bool is true, jump 0xA bytes to clean the stack before we continue.)
		0x75, 0x0A, // 0x9
		// call isaac-ng.XXXXXXXX (If our bool is false, call RNG::RandomInt() with the values loaded into the stack and registers.)
		0xE8, 0, 0, 0, 0, // 0xE
		// jmp isaac-ng.XXXXXXXX (Jump back to vanilla checks.)
		0xE9, 0, 0, 0, 0, // 0x13
		// add esp,4 (Clear 4 bytes (the int previously headed to RandomInt) from the stack.)
		0x83, 0xC4, 0x04, // 0x16
		// jmp isaac-ng.XXXXXXXX (Jump to the normal room clear code.)
		0xE9, 0, 0, 0, 0 // 0x1B	
	};

	char* code = (char*)malloc(sizeof(_code));
	memcpy(code, _code, 0x1B);

	void* ptr = &overrideMegaSatanEnding;
	memcpy(code + 0x5 - 4, &ptr, sizeof(ptr));

	ptrdiff_t randomInt = (char*)randomIntAddr - ((char*)region + 0xE);
	memcpy(code + 0xE - 4, &randomInt, sizeof(&randomInt));

	void* retOffset = (char*)addr + 0x5;
	ptrdiff_t jmp = (char*)retOffset - ((char*)region + 0x13);
	memcpy(code + 0x13 - 4, &jmp, sizeof(jmp));

	void* voidOffset = (char*)addr + 0x1F;
	ptrdiff_t voidJmp = (char*)voidOffset - ((char*)region + 0x1B);
	memcpy(code + 0x1B - 4, &voidJmp, sizeof(voidJmp));

	memcpy(region, code, 0x1B);


	MEMORY_BASIC_INFORMATION new_region_info;
	VirtualQuery(region, &new_region_info, sizeof(new_region_info));
	DWORD _dummy;
	VirtualProtect(new_region_info.BaseAddress, new_region_info.RegionSize, PAGE_EXECUTE_READ, &_dummy);

	char override_base[] = {
		0xe9, 0x00, 0x00, 0x00, 0x00,
	};

	ptrdiff_t jump_dist = (char*)region - ((char*)addr + 0x5);
	memcpy(override_base + 1, &jump_dist, sizeof(jump_dist));
	memcpy(addr, override_base, 5);

	VirtualProtect(info.BaseAddress, info.RegionSize, old_protect, &_dummy);

	FlushInstructionCache(GetModuleHandle(NULL), NULL, 0);
}