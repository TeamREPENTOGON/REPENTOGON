#include <stdio.h>
#include <stdexcept>
#include <Windows.h>

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"

static DWORD SetPageMemoryRW(void* addr, MEMORY_BASIC_INFORMATION* info) {
	VirtualQuery(addr, info, sizeof(*info));
	DWORD old_protect;
	VirtualProtect(info->BaseAddress, info->RegionSize, PAGE_READWRITE, &old_protect);
	return old_protect;
}

extern int ambushWaves;
extern bool overrideMegaSatanEnding;
extern void __stdcall LogMessageCallback(const char* logMessage);

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

/* This patch hooks KAGE_LogMessage by hand. LibZHL can't properly hook functions with varargs, and we need varargs to properly get log messages.
   So, we'll just do it manually, not a big deal.
   We manually call a trampoline function that takes a const char* as an input, and prints it to our ImGui log.
   At the point we hook, KAGE_LogMessage has already fully processed its varargs, and is storing the result in the ECX register.
   We pass ECX to our trampoline as a result.
   */
void ASMPatchLogMessage() {
	SigScan scanner("8d51??8a014184c075??2bca80b9????????0a");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.AddBytes("\x51") // Push ECX to the stack, so we can restore it as the function expects later
	    .AddBytes("\x51") // Push ECX one more time, for use on our function (this will be consumed).
	    .AddInternalCall(LogMessageCallback)
	    .AddBytes("\x59") // Pop ECX

	// Reintroduce overwritten assembly + some extra to work around some broken jumps
	    .AddBytes("\x8D\x51\x01") // lea edx,dword pointer ds:[ecx+1]
	    .AddBytes("\x8A\x01") // mov al,byte ptr ds:[ecx]
	    .AddBytes("\x41") // inc ecx
	    .AddBytes("\x84\xC0") // test al, al
	    .AddBytes("\x75\xF9") // jne xxxxxxxx
	    .AddRelativeJump((char*)addr + 0x11);
	sASMPatcher.PatchAt(addr, &patch);
}

/* Ambush waves have a hardcoded amount. This patch works around it by feeding the game a pointer to an int we control instead of the hardcoded 3.
   Boss rooms can't be changed in this manner as they have more setup.
   The game already sets up a check to see if we're in a boss ambush room in the EAX register at the point we hook.
   We compare against the pre-set check. If we're a boss room, move 2 to the EBX register, otherwise, move our pointer to the EBX register.
*/
void ASMPatchAmbushWaveCount() {
	SigScan scanner("33db83f801");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching hardcoded ambush wave count at %p\n", addr);

	char ptrMov[] = {
		0x8B, 0X1D, 0, 0, 0, 0
	};

	void* ptr = &ambushWaves;
	memcpy(ptrMov + 2, &ptr, sizeof(ptr));

	ASMPatch patch;
	patch.AddBytes("\x83\xF8\x01") // cmp eax, 1
		.AddBytes("\x75\x07") // jne (current addr + 0x7)
		.AddBytes("\xBB\x02").AddZeroes(3) // mov ebx, 2
        .AddBytes("\xEB\x06") // jmp (current addr + 0x6);
		.AddBytes(ptrMov) // mov ebx, dword ptr ds:[0xXXXXXXXX]
		.AddRelativeJump((char*)addr + 0xB);
	sASMPatcher.PatchAt(addr, &patch);
}

static void PerformASMPatches() {
	// OLD SCHOOL GAMER
	PatchHardcodedMegaSatanGameEnding();
	//PatchInputActionMenuCheck();

	// NEW SCHOOL GAMER
	ASMPatchLogMessage();
	ASMPatchAmbushWaveCount();
}