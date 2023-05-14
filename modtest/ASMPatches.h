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

//TODO port these patches to ASMPatcher

// Much of this is adapted from Sylmir's WIP Delirium patches, just adapted for use with the LibZHL sigscanner.
// Thanks Sylmir <3
extern int ambushWaves;
extern bool overrideMegaSatanEnding;
extern void __stdcall LogMessageCallback(const char* logMessage);

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

/* This patch hooks KAGE_LogMessage by hand.LibZHL can't properly hook functions with varargs, and we need varargs to properly get log messages.
   So, we'll just do it manually, not a big deal.
   We manually call a trampoline function that takes a const char* as an input, and prints it to our ImGui log.
   At the point we hook, KAGE_LogMessage has already fully processed its varargs, and is storing the result in the ECX register.
   */

void ASMPatchLogMessage() {
	SigScan scanner("8d51??8a014184c075??2bca80b9????????0a"); // KAGE_LogMessage
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.AddBytes("\x51"); // Push ECX to the stack, so we can restore it as the function expects later
	patch.AddBytes("\x51"); // Push ECX one more time, for use on our function (this will be consumed).
	patch.AddInternalCall(LogMessageCallback);
	patch.AddBytes("\x59"); // Pop ECX

	// Reintroduce overwritten assembly + some extra to work around some broken jumps
	patch.AddBytes("\x8D\x51\x01"); // lea edx,dword pointer ds:[ecx+1]
	patch.AddBytes("\x8A\x01"); // mov al,byte ptr ds:[ecx]
	patch.AddBytes("\x41"); // inc ecx
	patch.AddBytes("\x84\xC0"); // test al, al
	patch.AddBytes("\x75\xF9"); // jne xxxxxxxx
	patch.AddRelativeJump((char*)addr + 0x11);
	sASMPatcher.PatchAt(addr, &patch);
}

static void PerformASMPatches() {
	// OLD SCHOOL GAMER
	PatchHardcodedAmbushWaveCount();
	PatchHardcodedMegaSatanGameEnding();
	//PatchInputActionMenuCheck();

	// NEW SCHOOL GAMER
	ASMPatchLogMessage();
}