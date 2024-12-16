#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "../../LuaInterfaces/Room/Room.h"

/* Ambush waves have a hardcoded amount. This patch works around it by feeding the game a pointer to an int we control instead of the hardcoded 3.
*  An extra patch to spawn_wave is neccessary to make this work for boss challenge rooms because no difficulty 15 wave exists by default.
*  The game already sets up a check to see if we're in a boss ambush room in the EAX register at the point we hook.
*/
void ASMPatchAmbushWaveCount() {
	SigScan scanner("33db83f801");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching hardcoded ambush wave count at %p\n", addr);
	void* ambushPtr = &ambushWaves;
	void* bossAmbushPtr = &bossAmbushWaves;

	ASMPatch patch;
	patch.AddBytes("\x83\xF8\x01") // cmp eax, 1 (is this a boss challenge?)
		.AddBytes("\x75\x0B") // jne (current addr + 0xB)
		.AddBytes("\x8B\x1D").AddBytes(ByteBuffer().AddAny((char*)&bossAmbushPtr, 4)) // mov ebx, dword ptr ds:[0xXXXXXXXX]
		.AddRelativeJump((char*)addr + 0xB)
		// JNE DESTINATION
		.AddBytes("\x8B\x1D").AddBytes(ByteBuffer().AddAny((char*)&ambushPtr, 4)) // mov ebx, dword ptr ds:[0xXXXXXXXX]
		.AddRelativeJump((char*)addr + 0xB);
	sASMPatcher.PatchAt(addr, &patch);
}

/* This function overrides the call to GetRandomRoom in spawn_wave in order to avoid a crash when no difficulty 15 wave exists.
 * spawn_wave is thiscall. stdcall will mirror the stack cleaning
 * convention, and, as we don't need to preserve ecx under thiscall, nothing
 * more is required.
 */
static RoomConfig_Room* __stdcall OverrideGetRandomRoom(RoomConfig* config, unsigned int seed, bool reduceWeight,
	int stage, int roomType, int roomShape, unsigned int minVariant, int maxVariant, int minDifficulty,
	int maxDifficulty, unsigned int* requiredDoors, unsigned int roomSubtype, int mode);

RoomConfig_Room* __stdcall OverrideGetRandomRoom(RoomConfig* config, unsigned int seed, bool reduceWeight,
	int stage, int roomType, int roomShape, unsigned int minVariant, int maxVariant, int minDifficulty,
	int maxDifficulty, unsigned int* requiredDoors, unsigned int roomSubtype, int mode) {

	if (roomSubtype == 11 && maxDifficulty == 15)
	{
		RoomConfig_Room* room = config->GetRandomRoom(seed, reduceWeight, stage, roomType, roomShape, minVariant, maxVariant,
			minDifficulty, maxDifficulty, requiredDoors, roomSubtype, mode);
		if (room == nullptr) {
			room = config->GetRandomRoom(seed, reduceWeight, stage, roomType, roomShape, minVariant, maxVariant,
				10, 10, requiredDoors, roomSubtype, mode);
		}
		return room;
	}
	return config->GetRandomRoom(seed, reduceWeight, stage, roomType, roomShape, minVariant, maxVariant,
			minDifficulty, maxDifficulty, requiredDoors, roomSubtype, mode);
}

void PatchBossWaveDifficulty() {
	const char* signature = "e8????????8bf08975??85f60f85"; // call GetRandomRoom
	SigScan scanner(signature);
	if (!scanner.Scan()) {
		ZHL::Log("[ERROR] Unable to find signature to patch boss wave difficulty\n");
		return;
	}

	void* patchAddr = scanner.GetAddress();

	printf("[REPENTOGON] Patching spawn_wave at %p\n", patchAddr);
	ASMPatch patch;
	// ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	// patch.PreserveRegisters(registers);
	/* Registers need not be saved, as we are performing the equivalent of a
	 * function call. Push ecx so the override function can access the current
	 * RoomConfig object.
	 */
	patch.Push(ASMPatch::Registers::ECX);
	patch.AddInternalCall(&OverrideGetRandomRoom);
	patch.AddRelativeJump((char*)patchAddr + 5);
	// patch.RestoreRegisters(registers);

	sASMPatcher.PatchAt(patchAddr, &patch);
}

/* Mega Satan used to have a 50% chance to end the game instantly instead of spawning an ending chest + void portal.
* The MC_PRE_MEGA_SATAN_ENDING callback was created as a way to suppress this forced ending if desired, so that achievements could still register properly.
* As of REP+, this is no longer random - Mega Satan will not end the run as long as the Void is unlocked.
* However, the callback also allowed you to bypass the two other conditions (void being unlocked, and some other thing) so it's probably okay to keep the callback as it could be useful.
* Though doing this also spawns a void portal even if void isn't unlocked...
* If we wanted, we could separately prevent the void portal from spawning after this if void isn't unlocked yet.
*/
bool __stdcall MegaSatanCallbackTrampoline() {
	const int callbackid = 1201;
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
					return true;
				}
			}
		}
	}

	return false;
}

void ASMPatchMegaSatanEnding() {
	SigScan scanner("807d??0074??807d??0074??a1");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching MC_PRE_MEGA_SATAN_ENDING into Room:SpawnClearAward at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.AddInternalCall(MegaSatanCallbackTrampoline)
		.AddBytes("\x84\xC0")  // test al, al
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0xC)  // Callback returned true, suppress the ending.
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x4))  // Callback did not return true. Restore a boolean check that was overidden.
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0x45)  // Overidden jump to potentially trigger the ending.
		.AddRelativeJump((char*)addr + 0x6);  // Continue as normal.
	sASMPatcher.PatchAt(addr, &patch);
}

// Used for Better Void Generation to mark when a downpour/dross room shouldn't have water, since we add it manually
void ASMPatchWaterDisabler() {
	SigScan scanner("c685????????018b47??8148??00100000");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	void* boolPtr = &roomASM.WaterDisabled;
	printf("[REPENTOGON] Patching Room::Init water disabler bool at %p\n", addr);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x7))
		.AddBytes("\xc6\x05").AddBytes(ByteBuffer().AddAny((char*)&boolPtr, 4)).AddBytes("\x01")
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

/*
 * Quick Room Clear
 * reduces the delay in frames between the last enemy killed and the room being cleared
*/

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
		.Push(ASMPatch::Registers::EAX, 0x18300) // push Room
		.AddInternalCall(CheckDeepGaperClearDelay) // call CheckDeepGaperClearDelay()
		.AddBytes("\x84\xc0") // TEST AL, AL
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x27) // jump to canshutdoors = true
		.AddRelativeJump((char*)addr + 0xf); // jump to next check
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchRoomClearDelay() {
	SigScan scanner1("c787????????0a00000075??8b46"); // Room::Update
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

// this changes the function to check Game's time counter instead of frame counter, in parity with TrySpawnBossRushDoor
void ASMPatchTrySpawnBlueWombDoor() {
	SigScan scanner("83f8087c??8b87");
	scanner.Scan();
	void* addr = (char*)scanner.GetAddress() + 7;
	printf("[REPENTOGON] Patching Room::TrySpawnBlueWombDoor at %p\n", addr);
	ASMPatch patch;
	patch.AddBytes("\x24");
	sASMPatcher.FlatPatch(addr, &patch);
}