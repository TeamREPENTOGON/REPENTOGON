#include "IsaacRepentance.h"
#include "SigScan.h"
#include "LuaCore.h"
#include "LuaEntityNPC.h"
#include "LuaRender.h"

#include "ASMPatches.h"
#include "ASMPatcher.hpp"

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

/* * MC_PRE_LASER_COLLISION * *
* There is no function you can hook that would allow you to skip a laser "collision".
* This section patches in PRE_LASER_COLLISION callbacks right when the game would
* first consider an entity to be hit by the laser. Two patches are needed because
* "sample" lasers (lasers that are capable of curving) use different collision code.
* POST_LASER_COLLISION is just done as a function hook in CustomCallbacks.cpp
*/

bool __stdcall RunPreLaserCollisionCallback(Entity_Laser* laser, Entity* entity) {
	const int callbackid = 1248;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*laser->GetVariant())
			.push(laser, lua::Metatables::ENTITY_LASER)
			.push(entity, lua::Metatables::ENTITY)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				return lua_toboolean(L, -1);
			}
		}
	}
	return false;
}

// This patch injects the PRE_LASER_COLLISION callback for "sample" lasers (lasers that curve, ie have multiple sample points).
void PatchPreSampleLaserCollision() {
	SigScan scanner("8b4d??8b41??83f809");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.AddBytes("\x8B\x4D\x90")  // mov ecx,dword ptr ss:[ebp-70] (Put the entity in ECX)
		.AddBytes("\x8B\x45\x8C")  // mov eax,dword ptr ss:[ebp-74] (Put the laser in EAX)
		.AddBytes("\x51\x50") // Push ECX, EAX for function inputs
		.AddInternalCall(RunPreLaserCollisionCallback) // Run PRE_LASER_COLLISION callback
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr - 0x4B)  // Ignore collision
		.AddBytes("\x8B\x4D\x90")  // mov ecx,dword ptr ss:[ebp-70] (Put the entity in ECX) (Restored overridden command)
		.AddBytes("\x8B\x41\x28")  // mov eax,dword ptr ds:[ecx+28] (Put the entity's type in EAX) (Restored overridden command)
		.AddRelativeJump((char*)addr + 0x6);  // Allow collision
	sASMPatcher.PatchAt(addr, &patch);
}

// This patch injects the PRE_LASER_COLLISION callback for "non-sample" lasers (ones that can ONLY ever be a straight line).
void PatchPreLaserCollision() {
	SigScan scanner("f30f1095????????8bcf56");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.AddBytes("\x56\x57") // Push EDI (entity) and ESI (laser) for function input
		.AddInternalCall(RunPreLaserCollisionCallback) // Run PRE_LASER_COLLISION callback
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x49A) // Ignore collision
		.AddBytes("\xF3\x0F\x10\x95\x34\xFF\xFF\xFF")  // movss xmm2,dword ptr ss:[EBP-0xCC] (Restored overridden command)
		.AddRelativeJump((char*)addr + 0x8); // Allow collision
	sASMPatcher.PatchAt(addr, &patch);
}

// End of PRE_LASER_COLLISION patches

/* * MC_ENTITY_TAKE_DMG REIMPLEMENTATION * *
* This section patches in a new ENTITY_TAKE_DMG callback with table returns for overridding.
* A patch is used because ENTITY_TAKE_DMG runs in the middle of the TakeDamage functions,
* not right at the start, so we can't just run the callback to modify the function inputs.
* By patching over where the callback would normally be triggered, we have access to the
* TakeDamage function parameters in memory and can modify them directly.
* We need to patch into both Entity::TakeDamage AND EntityPlayer::TakeDamage.
*/

bool __stdcall ProcessPreDamageCallback(Entity* entity, char* ebp, bool isPlayer) {
	int callbackid = 1007;
	if (CallbackState.test(callbackid - 1000)) {
		// Obtain inputs as offsets from EBP (same way the compiled code reads them).
		// As pointers so we can modify them :)
		unsigned __int64* damageFlags = (unsigned __int64*)(ebp + 0x0C);
		float* damage = (float*)(ebp + 0x08);
		int* damageHearts = isPlayer ? (int*)(ebp - 0x100) : nullptr;
		EntityRef** source = (EntityRef**)(ebp + 0x14);
		int* damageCountdown = (int*)(ebp + 0x18);

		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		unsigned int entityType = *entity->GetType();

		lua::LuaCaller caller(L);
		caller.push(callbackid)
			.push(entityType)
			.push(entity, lua::Metatables::ENTITY);
		if (isPlayer) {
			caller.push(*damageHearts);
		}
		else {
			caller.push(*damage);
		}
		caller.push(*damageFlags)
			.push(*source, lua::Metatables::ENTITY_REF)
			.push(*damageCountdown);
		lua::LuaResults lua_result = caller.call(1);

		if (!lua_result) {
			if (lua_isboolean(L, -1)) {
				return lua_toboolean(L, -1);
			}
			else if (lua_istable(L, -1)) {
				unsigned __int64 originalDamageFlags = *damageFlags;
				// New table return format for overriding values.
				lua_pushnil(L);
				while (lua_next(L, -2) != 0) {
					if (lua_isstring(L, -2)) {
						const std::string key = lua_tostring(L, -2);
						if (key == "Damage" && lua_isnumber(L, -1)) {
							float newDamage = lua_tonumber(L, -1);
							if (newDamage < 0) {
								newDamage = 0;
							}
							if (isPlayer) {
								*damageHearts = newDamage;
							}
							*damage = newDamage;
						}
						else if (key == "DamageFlags" && lua_isinteger(L, -1)) {
							*damageFlags = lua_tointeger(L, -1);
						}
						// Couldn't get this to work. Maybe garbage collection related?
						/*else if (key == "Source" && lua_isuserdata(L, -1)) {
							*source = &lua::GetUserdata<EntityRef>(L, -1, lua::Metatables::ENTITY_REF, "EntityRef");
							void* newSource = lua::CheckUserdata(L, -1, lua::Metatables::ENTITY_REF, "EntityRef");
							if (newSource != nullptr) {
								*source = (EntityRef*)newSource;
							}
						}*/
						else if (key == "DamageCountdown" && lua_isnumber(L, -1)) {
							*damageCountdown = lua_tonumber(L, -1);
						}
					}
					lua_pop(L, 1);
				}

				if (!isPlayer) {
					// If certain DamageFlags were added/present, retroactively obey their behaviour.
					// (Normally these are handled prior to when this callback runs.)

					// Check if DamageFlag.DAMAGE_FIRE was added
					if ((originalDamageFlags & (1 << 1)) < (*damageFlags & (1 << 1))) {
						if (*entity->GetFireDamageCountdown() > 0) {
							return false;
						}
						*entity->GetFireDamageCountdown() = 10;
					}
					// Check if DamageFlag.DAMAGE_COUNTDOWN was added
					if ((originalDamageFlags & (1 << 6)) < (*damageFlags & (1 << 6))) {
						if (*entity->GetDamageCountdown() > 0) {
							return false;
						}
					}
					// If the DAMAGE_COUNTDOWN flag is currently present, set the entity's
					// DamageCountdown to the now potentially-modified value.
					if ((*damageFlags & (1 << 6)) != 0) {
						*entity->GetDamageCountdown() = *damageCountdown;
					}
				}
			}
		}
	}

	return true;
}

void InjectPreDamageCallback(void* addr, bool isPlayer) {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS - ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.AddBytes("\x58\x58\x58\x58\x58")  // Pop EAX x5 to remove the overridden function's inputs from the stack.
		.PreserveRegisters(savedRegisters)
		.AddBytes("\x31\xC0");  // xor eax,eax
	if (isPlayer) {
		patch.AddBytes("\xB0\x01");  // Move Al, 0x1
	}
	patch.Push(ASMPatch::Registers::EAX)  // Push a boolean to identify if this is EntityPlayer:TakeDamge
		.Push(ASMPatch::Registers::EBP)  // Push EBP (We'll get other inputs as offsets from this)
		.Push(ASMPatch::Registers::EDI)  // Push Entity pointer
		.AddInternalCall(ProcessPreDamageCallback);  // Run the new MC_ENTITY_TAKE_DMG callback
	if (isPlayer) {
		// For the player, also copy the (potentially modified) lower DamageFlags to [EBP-0x104]
		patch.AddBytes("\x8B\x75\x0C")  // mov esi,dword ptr [EBP+0x0C]
			.AddBytes("\x89\xB5\xFC\xFE\xFF\xFF");  // mov dword ptr [EBP-0x104],esi
	}
	patch.RestoreRegisters(savedRegisters);
	if (!isPlayer) {
		// For non-players, also copy the (potentially modified) DamageFlags into EBX and ESI
		patch.AddBytes("\x8B\x5D\x0C")  // mov ebx,dword ptr [EBP+0x0C]
			.AddBytes("\x8B\x75\x10");  // mov esi,dword ptr [EBP+0x10]
	}
	patch.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// Patches overtop the existing calls to LuaEngine::Callback::EntityTakeDamage,
// within the Entity::TakeDamage and EntityPlayer::TakeDamage functions respectively.
void PatchPreEntityTakeDamageCallbacks() {
	SigScan entityTakeDmgScanner("e8????????84c00f84????????8d4c24??e8????????f30f1045");
	entityTakeDmgScanner.Scan();
	InjectPreDamageCallback(entityTakeDmgScanner.GetAddress(), false);

	SigScan playerTakeDmgScanner("e8????????84c00f84????????8bcfe8????????84c00f85????????83bf????????25");
	playerTakeDmgScanner.Scan();
	InjectPreDamageCallback(playerTakeDmgScanner.GetAddress(), true);
}

// End of ENTITY_TAKE_DMG patches

/* * MC_POST_ENTITY_TAKE_DMG * *
* A hook worked just fine for ""POST_TAKE_DMG"" initially, but now that we support overidding
* values via ENTITY_TAKE_DMG (see above) we need the POST_TAKE_DMG callback to actually show
* those modified values. That means another patch!!
* Like before, we need to patch into both Entity::TakeDamage and EntityPlayer::TakeDamage.
*/

void __stdcall ProcessPostDamageCallback(Entity* entity, char* ebp, bool isPlayer) {
	int callbackid = 1006;
	if (CallbackState.test(callbackid - 1000)) {
		// Obtain inputs as offsets from EBP (same way the compiled code reads them).
		unsigned __int64 damageFlags = *(unsigned __int64*)(ebp + 0x0C);
		float damage = *(float*)(ebp + 0x08);
		int damageHearts = isPlayer ? *(int*)(ebp - 0x100) : 0;
		EntityRef* source = *(EntityRef**)(ebp + 0x14);
		int damageCountdown = *(int*)(ebp + 0x18);

		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller caller(L);
		caller.push(callbackid)
			.push(*entity->GetType())
			.push(entity, lua::Metatables::ENTITY);
		if (isPlayer) {
			caller.push(damageHearts);
		}
		else {
			caller.push(damage);
		}
		caller.push(damageFlags)
			.push(source, lua::Metatables::ENTITY_REF)
			.push(damageCountdown)
			.call(1);
	}
};

void InjectPostDamageCallback(void* addr, bool isPlayer) {
	const int numOverriddenBytes = (isPlayer ? 10 : 5);
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ASMPatch::ByteBuffer().AddAny((char*)addr, numOverriddenBytes));  // Restore the commands we overwrote
	if (isPlayer) {
		// The EntityPlayer::TakeDamage patch needs to ask Al if the damage occured.
		// The Entity::TakeDamage patch doesn't need to do this since it is at a location that only runs after damage.
		patch.AddBytes("\x84\xC0") // Test Al (Al?)
		    .AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + numOverriddenBytes);
	}
	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\x31\xC0");  // xor eax,eax
	if (isPlayer) {
		patch.AddBytes("\xB0\x01");  // Move Al, 0x1
	}
	patch.Push(ASMPatch::Registers::EAX)  // Push a boolean to identify if this is EntityPlayer:TakeDamge
		.Push(ASMPatch::Registers::EBP)  // Push EBP (We'll get other inputs as offsets from this)
		.Push(ASMPatch::Registers::EDI)  // Push Entity pointer
		.AddInternalCall(ProcessPostDamageCallback)  // Run the MC_POST_(ENTITY_)TAKE_DMG callback
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + numOverriddenBytes);
	sASMPatcher.PatchAt(addr, &patch);
}

// These patches overwrite suitably-sized commands near where the respective TakeDamage functions would return.
// The overridden bytes are restored by the patch.
void PatchPostEntityTakeDamageCallbacks() {
	SigScan entityTakeDmgScanner("f30f1147??5f5e5b8be55dc21400");
	entityTakeDmgScanner.Scan();
	InjectPostDamageCallback(entityTakeDmgScanner.GetAddress(), false);

	SigScan playerTakeDmgScanner("8b4d??64890d????????595f5e8b4d??33cde8????????8be55dc2140068????????e8????????83c404833d????????ff0f85????????c745??0d000000");
	playerTakeDmgScanner.Scan();
	InjectPostDamageCallback(playerTakeDmgScanner.GetAddress(), true);
}

// End of POST_ENTITY_TAKE_DMG patches

// Prevents Curse of the Lost in Blue Womb in addition to Curse of Darkness (Vanilla)
// changes evaluated bitmask from 0xfe to 0xfa
void ASMPatchBlueWombCurse() {
	SigScan scanner("8367??fe6a00");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Level::Init() Curse evaluation for Blue Womb %p\n", addr);

	MEMORY_BASIC_INFORMATION info;
	DWORD old_protect = SetPageMemoryRW(addr, &info);
	DWORD _dummy;

	char override_base[] = {
		0x83, 0x67, 0x0c, 0xfa      // assembly before: dword ptr [EDI + 0xc],0xfffffffe
	};
	memcpy(addr, override_base, 4);

	VirtualProtect(info.BaseAddress, info.RegionSize, old_protect, &_dummy);

	FlushInstructionCache(GetModuleHandle(NULL), NULL, 0);
}

void PerformASMPatches() {
	ASMPatchLogMessage();
	ASMPatchAmbushWaveCount();
	ASMPatchMegaSatanEnding();
	ASMPatchConsoleRunCommand();
	ASMPatchBlueWombCurse();
	PatchFireProjectiles();
	PatchFireBossProjectiles();
	LuaRender::PatchglDrawElements();
	PatchPreSampleLaserCollision();
	PatchPreLaserCollision();
	PatchPreEntityTakeDamageCallbacks();
	PatchPostEntityTakeDamageCallbacks();
}
