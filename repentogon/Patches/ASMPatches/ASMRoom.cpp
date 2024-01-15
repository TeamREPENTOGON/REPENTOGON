#include "IsaacRepentance.h"
#include "SigScan.h"
#include "LuaCore.h"

#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "../../REPENTOGONOptions.h"

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
		(char)0x8B, 0X1D, 0, 0, 0, 0, 0
	};

	void* ptr = &ambushWaves;
	memcpy(ptrMov + 2, &ptr, sizeof(ptr));

	ASMPatch patch;
	patch.AddBytes("\x83\xF8\x01") // cmp eax, 1
		.AddBytes("\x75\x07") // jne (current addr + 0x7)
		.AddBytes("\xBB\x02").AddZeroes(3) // mov ebx, 2
		.AddBytes("\xEB\x06") // jmp (current addr + 0x6);
		.AddBytes(ByteBuffer().AddAny(ptrMov, 6)) // mov ebx, dword ptr ds:[0xXXXXXXXX]
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

	void* ptr = &overrideMegaSatanEnding;

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.AddInternalCall(MegaSatanCallbackTrampoline) // call MegaSatanCallbackTrampoline()
		.RestoreRegisters(reg)
		.AddBytes("\xA0").AddBytes(ByteBuffer().AddAny((char*)&ptr, 4)) // mov al, byte ptr ds:[XXXXXXXX]
		.AddBytes("\x84\xC0") // test al, al
		.AddBytes("\x75\x0A") // jne (current addr + 0xA)
		.AddInternalCall(randomIntAddr) // call RNG::RandomInt()
		.AddRelativeJump((char*)addr + 0x5) // jmp isaac-ng.XXXXXXXX
		.AddBytes("\x83\xC4\x04")
		.AddRelativeJump((char*)addr + 0x1F);
	sASMPatcher.PatchAt(addr, &patch);
}

void __stdcall SetRoomClearDelay(Room* room) {
	room->_roomClearDelay = repentogonOptions.quickRoomClear ? 3 : 10;
}

int roomSaveStateDelay = 0x7;
void __stdcall SetRoomSaveStateDelay() {
	roomSaveStateDelay = repentogonOptions.quickRoomClear ? 0x1 : 0x7;
}

bool __stdcall CheckDeepGaperClearDelay(Room* room) {
	return room->_roomClearDelay - 1 < (repentogonOptions.quickRoomClear ? 2 : 9);
}

void ASMPatchRoomUpdate(void* addr) {
	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.Push(ASMPatch::Registers::EAX) // preserve unmodified eax
		.AddBytes("\x9f") // store status flags
		.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EDI) // push Room
		.AddInternalCall(SetRoomClearDelay) // call SetRoomClearDelay()
		.RestoreRegisters(reg)
		.AddBytes("\x9e") // restore status flags
		.Pop(ASMPatch::Registers::EAX) // restore unmodified eax
		.AddRelativeJump((char*)addr + 0xa); // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchRoomSaveState(void* addr) {
	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS - ASMPatch::SavedRegisters::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.AddInternalCall(SetRoomSaveStateDelay) // call SetRoomClearDelay()
		.RestoreRegisters(reg)
		.MoveFromMemory(ASMPatch::Registers::ESI, 0x11ec, ASMPatch::Registers::EAX)
		.AddBytes("\x48") // dec eax
		.AddBytes("\x3b\x05").AddBytes(ByteBuffer().AddPointer(&roomSaveStateDelay))  // cmp eax, dword ptr ds:[roomSaveStateDelay]
		.AddRelativeJump((char*)addr + 0xa); // jump to ja
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchDeepGaperClearCheck(void* addr) {
	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EAX, 0x18190) // push Room
		.AddInternalCall(CheckDeepGaperClearDelay) // call CheckDeepGaperClearDelay()
		.AddBytes("\x84\xc0") // TEST AL, AL
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x27) // jump to canshutdoors = true
		.AddRelativeJump((char*)addr + 0xf); // jump to next check
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchRoomClearDelay() {
	SigScan scanner1("c787????????0a00000075??8b469"); // Room::Update
	SigScan scanner2("8b86????????4883f807"); // Room::SaveState
	SigScan scanner3("8b80????????83b8????????09"); // ai_deep_gaper
	scanner1.Scan();
	scanner2.Scan();
	scanner3.Scan();
	void* addrs[3] = { scanner1.GetAddress(), scanner2.GetAddress(), scanner3.GetAddress() };
	printf("[REPENTOGON] Patching Room clear delay setters/checks at %p, %p, %p\n", addrs[0], addrs[1], addrs[2]);
	ASMPatchRoomUpdate(addrs[0]);
	ASMPatchRoomSaveState(addrs[1]);
	ASMPatchDeepGaperClearCheck(addrs[2]);
}

/*
// idk what's causing this one to crash on launch
void ASMPatchRoomClearDelay() {
	SigScan scanner1("c787????????0a00000075??8b469");
	SigScan scanner2("b8140000000f44c8898f");
	scanner1.Scan();
	scanner2.Scan();
	void* addrs[2] = { scanner1.GetAddress(), scanner2.GetAddress() };

	printf("[REPENTOGON] Patching Room Clear delay at %p, %p, %p\n", addrs[0], addrs[1], ((char*)addrs[1] + 0x2));

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch1, patch2, patch3;

	patch1.Push(ASMPatch::Registers::EAX) // preserve unmodified eax
		.AddBytes("\x9f") // store status flags
		.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EDI) // push Room
		.AddInternalCall(SetRoomClearDelay) // call SetRoomClearDelay()
		.RestoreRegisters(reg)
		.AddBytes("\x9e") // restore status flags
		.Pop(ASMPatch::Registers::EAX) // restore unmodified eax
		.AddRelativeJump((char*)addrs[0] + 0xa); // jmp isaac-ng.XXXXXXXX

	patch2.AddBytes("\x75\x0c"); // jne 0xc

	patch3.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EDI) // push Room
		.AddInternalCall(SetRoomClearDelay) // call SetRoomClearDelay()
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)addrs[1] + 0xd); // jmp isaac-ng.XXXXXXXX

	sASMPatcher.PatchAt(addrs[0], &patch1);
	sASMPatcher.FlatPatch(addrs[1], &patch2);
	sASMPatcher.PatchAt(((char*)addrs[1] + 0x2), &patch3);
}
*/