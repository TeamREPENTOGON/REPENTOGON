#include "IsaacRepentance.h"
#include "SigScan.h"
#include "LuaCore.h"


extern std::bitset<500> CallbackState;
extern int ambushWaves;
extern void __stdcall LogMessageCallback(const char* logMessage);

static DWORD SetPageMemoryRW(void* addr, MEMORY_BASIC_INFORMATION* info) {
	VirtualQuery(addr, info, sizeof(*info));
	DWORD old_protect;
	VirtualProtect(info->BaseAddress, info->RegionSize, PAGE_READWRITE, &old_protect);
	return old_protect;
}

/* This patch hooks KAGE_LogMessage by hand. LibZHL can't properly hook functions with varargs, and we need varargs to properly get log messages.
*  So, we'll just do it manually, not a big deal.
*  We manually call a trampoline function that takes a const char* as an input, and prints it to our ImGui log.
*  At the point we hook, KAGE_LogMessage has already fully processed its varargs, and is storing the result in the ECX register.
*  We pass ECX to our trampoline as a result.
   */
void ASMPatchLogMessage() {
	SigScan scanner("8d51??8a014184c075??2bca80b9????????0a");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Adding LogMessage callback for ImGui at %p\n", addr);

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
	    .AddRelativeJump((char*)addr + 10);
	sASMPatcher.PatchAt(addr, &patch);
}

/* Ambush waves have a hardcoded amount. This patch works around it by feeding the game a pointer to an int we control instead of the hardcoded 3.
*  Boss rooms can't be changed in this manner as they have more setup, and will crash if forced above 2.
*  The game already sets up a check to see if we're in a boss ambush room in the EAX register at the point we hook.
*  We compare against the pre-set check. If we're a boss room, move 2 to the EBX register, otherwise, move our pointer to the EBX register.
*/
void ASMPatchAmbushWaveCount() {
	SigScan scanner("33db83f801");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching hardcoded ambush wave count at %p\n", addr);

	char ptrMov[] = {
		0x8B, 0X1D, 0, 0, 0, 0, 0
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

/* Mega Satan has a seeded 50% chance to end the game forcefully. 
*  Historically, this means mods that want to skip to the Void 100% of the time have to do so manually. This stops achievements from registering.
*  Mods can work around that with our completion mark API, but that doesn't help in the case of Steam achievements!
*  We hook the function responsible for this right when it calls RNG::RandomInt() to determine if the game should end or spawn a Void portal.
*  The stack is already set up to call RandomInt with the right seed and values and whatnot.
*  We use a trampoline to call a Lua callback, check if it's true or false, force a Void portal if it's true, or call RNG::RandomInt() if it's false to preserve vanilla behavior.
*/

bool overrideMegaSatanEnding = false;
void __stdcall MegaSatanCallbackTrampoline() {
	overrideMegaSatanEnding = false;

	int callbackid = 1201;
	if (CallbackState.test(callbackid - 1000)) {

		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua::LuaCaller caller(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults res = caller.push(callbackid)
			.pushnil() // i don't think we really need any params here?
			.call(1);

		if (!res) {
			if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) {
					overrideMegaSatanEnding = true;
				}
			}
		}
	}
}

void ASMPatchMegaSatanEnding() {
	SigScan scanner("e8????????85c00f85????????3845??0f84????????3845??0f84????????a1");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	SigScan randomIntScanner("558bec568bf18b16");
	randomIntScanner.Scan();
	void* randomIntAddr = randomIntScanner.GetAddress();

	printf("[REPENTOGON] Patching hardcoded Mega Satan game ending at %p\n", addr);

	char ptrMov[] = {
		0xA1, 0, 0, 0, 0, 0
	};

	void* ptr = &overrideMegaSatanEnding;

	memcpy(ptrMov + 1, &ptr, sizeof(ptr));

	ASMPatch patch;
	patch.AddBytes("\x50\x53\x51\x52\x56\x57") // Push EAX, EBX, ECX, EDX, ESI and EDI
		.AddInternalCall(MegaSatanCallbackTrampoline) // call MegaSatanCallbackTrampoline()
		.AddBytes("\x5F\x5E\x5A\x59\x5B\x58") // Pop EDI, ESI, EDX, ECX, EBX and EAX
		.AddBytes(ptrMov) // mov eax, dword ptr ds:[XXXXXXXX]
		.AddBytes("\x85\xC0") // test eax, eax
		.AddBytes("\x75\x0A") // jne (current addr + 0xA)
		.AddInternalCall(randomIntAddr) // call RNG::RandomInt()
		.AddRelativeJump((char*)addr + 0x5) // jmp isaac-ng.XXXXXXXX
		.AddBytes("\x83\xC4\x04")
		.AddRelativeJump((char*)addr + 0x1F);
	sASMPatcher.PatchAt(addr, &patch);
}

/* Console::RunCommand takes a Player param for commands that need to run on a player.
*  RunCommand will attempt to derive this from PlayerManager if not set.
*  This prevents commands from being run on the main menu.
*  We will handle this on our end in two parts- one hook, and one patch. This is the patch side of that.
*  We just skip over the check by replacing the conditional JMP with an unconditional one.
*/
void ASMPatchConsoleRunCommand() {
	SigScan scanner("75??8b0d????????5781c140ba0100");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Console::RunCommand Player requirement at %p\n", addr);

	MEMORY_BASIC_INFORMATION info;
	DWORD old_protect = SetPageMemoryRW(addr, &info);
	DWORD _dummy;

	char override_base[] = {
		0xEB
	};
	memcpy(addr, override_base, 1);


	VirtualProtect(info.BaseAddress, info.RegionSize, old_protect, &_dummy);

	FlushInstructionCache(GetModuleHandle(NULL), NULL, 0);
}

static void PerformASMPatches() {
	ASMPatchLogMessage();
	ASMPatchAmbushWaveCount();
	ASMPatchMegaSatanEnding();
	ASMPatchConsoleRunCommand();
}