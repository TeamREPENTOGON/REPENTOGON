#include "IsaacRepentance.h"
#include "SigScan.h"
#include "LuaCore.h"
#include "../LuaInterfaces/Entities/EntityNPC.h"
#include "../LuaInterfaces/Entities/EntityPlayer.h"
#include "../LuaInterfaces/LuaRender.h"
#include "XMLData.h"
#include "ModReloading.h"

#include "ASMPatches.h"
#include "ASMPatcher.hpp"
#include "Log.h"
//#include "../REPENTOGONOptions.h"

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

	ASMPatch patch;
	patch.AddBytes("\xEB");
	sASMPatcher.FlatPatch(addr, &patch);
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

// Re-implementation of the inlined Entity_Familiar::CanBeDamagedByLaser() that allows for overriding via an XML tag.
bool __stdcall FamiliarCanBeDamagedByLaserReimplementation(Entity_Familiar* fam) {
	const unsigned int var = *fam->GetVariant();
	const unsigned int subt = *fam->GetSubType();
	// Check a new XML attribute for whether or not to allow laser collisions.
	XMLAttributes xmlData = XMLStuff.EntityData->GetNodesByTypeVarSub(3, var, subt, false);
	const std::string laserCollisionsTag = xmlData["familiarallowlasercollision"];
	if (laserCollisionsTag == "true") {
		return true;
	}
	else if (laserCollisionsTag == "false") {
		return false;
	}
	// Re-implementation of Entity_Familiar::CanBeDamagedByLaser()
	return var == 61 || var == 67 || var == 211 || (var == 206 && subt == 427);
}

// Entity_Familiar::CanBeDamagedByLaser() is inlined into Entity_Laser::CanDamageEntity().
// This patch injects a re-implementation of that function.
void PatchFamiliarCanBeDamagedByLaser() {
	SigScan scanner("8b47??83f83e74??83f84374??3dd3000000");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.AddBytes("\x57") // Push EDI (the familiar) for function input
		.AddInternalCall(FamiliarCanBeDamagedByLaserReimplementation) // call function
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x24) // Jump for TRUE (can be hit)
		.AddRelativeJump((char*)addr + 0x28); // Jump for FALSE (can't be hit)
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall FamiliarGetMultiplierTrampoline(Entity_Familiar* fam) {
	const unsigned int var = *fam->GetVariant();
	const unsigned int subt = *fam->GetSubType();

	XMLAttributes xmlData = XMLStuff.EntityData->GetNodesByTypeVarSub(3, var, subt, false);
	const std::string ignoreBFFSTag = xmlData["familiarignorebffs"];

	Entity_Player* plr = *fam->GetPlayer();

	if (ignoreBFFSTag != "true" && plr && plr->HasCollectible(0xF7, false)) {
	 	return true;
	}

	return false;
}

// This patch allows for disabling the check for BFFS in Entity_Familiar::GetMultiplier() with a custom XML tag.
void ASMPatchFamiliarGetMultiplier() {
	SigScan scanner("e8????????84c074??8b46??83f82b");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Entity_Familiar::GetMultiplier at %p\n", addr);
	
	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes("\x83\xC4\x08") // add esp, 4
		.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::ESI)
		.AddInternalCall(FamiliarGetMultiplierTrampoline) // call FamiliarGetMultiplierTrampoline()
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)addr + 0x16) // jump for false
		.AddRelativeJump((char*)addr + 0x9); // jump for true
	sASMPatcher.PatchAt(addr, &patch);
}

/* * MC_ENTITY_TAKE_DMG REIMPLEMENTATION * *
* This section patches in a new ENTITY_TAKE_DMG callback with table returns for overridding.
* A patch is used because ENTITY_TAKE_DMG runs in the middle of the TakeDamage functions,
* not right at the start, so we can't just run the callback to modify the function inputs.
* By patching over where the callback would normally be triggered, we have access to the
* TakeDamage function parameters in memory and can modify them directly.
* We need to patch into both Entity::TakeDamage AND EntityPlayer::TakeDamage.
*/
extern int entityTakeDmgCallbackKey;

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
		lua_rawgeti(L, LUA_REGISTRYINDEX, entityTakeDmgCallbackKey);

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
							float newDamage = (float) lua_tonumber(L, -1);
							if (newDamage < 0) {
								newDamage = 0;
							}
							if (isPlayer) {
								*damageHearts = (int) newDamage;
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
							*damageCountdown = (int) lua_tonumber(L, -1);
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
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, numOverriddenBytes));  // Restore the commands we overwrote
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

	printf("[REPENTOGON] Patching Level::Init() Curse evaluation for Blue Womb at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x83\x67\x0C\xFA");
	sASMPatcher.FlatPatch(addr, &patch);
}

// Render mod names with a smaller font
void PatchModMenu_Font(const char* sig) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.AddBytes("\x8D\x89\x14\xA5\x10").AddZeroes(1);
	sASMPatcher.FlatPatch(addr, &patch);
}

void ASMPatchModsMenu() {
	printf("[REPENTOGON] Patching Menu_Mods::Render() \n");

	PatchModMenu_Font("8d89????????f30f1140");
	PatchModMenu_Font("8d89????????f30f1100");
}

const int stageidToAchievement[36] = {
	-1, // Special Rooms
	-1, // Basement
	86, // Cellar
	342, // Burning Basement
	-1, // Caves
	87, // Catacombs
	343, // Flooded Caves
	-1, // Depths
	88, // Necropolis
	344, // Dank Depths
	4, // Womb
	4, // Utero
	345, // Scarred Womb
	234, // Blue Womb
	32, // Sheol (this is for ??? character unlock but it works)
	32, // Cathedral
	78, // Dark Room (this is for the Negative)
	57, // Chest (this is for the Polaroid)
	-1, // Unused AB Greed stageids
	-1,
	-1,
	-1,
	-1,
	-1,
	-1, // The Shop (floor)
	-1, // Ultra Greed (floor)
	//320, // The Void
	-1, // The Void - changed to always be unlocked so Void rooms always show up
	407, // Downpour (this is A Secret Exit)
	412, // Dross
	407, // Mines
	413, // Ashpit
	407, // Mausoleum
	414, // Gehenna
	411, // Corpse (this is Rotten Hearts)
	-1, // Mortis
	415, // Home (this is Red Key)
};

bool IsFloorUnlocked(unsigned int stageId) {
	int achievement = stageidToAchievement[stageId];
	if (achievement != -1) {
		PersistentGameData* pgd = &g_Manager->_persistentGameData;
		return pgd->Unlocked(achievement);
	}
	return true;
}

// The void now draws from all floors
void __stdcall VoidGenerationOverride(RoomConfigHolder* _this, std::vector<RoomConfig*>* rooms, int type, int shape, int minVariant, 
	int maxVariant, int minDifficulty, int maxDifficulty, unsigned int* doors, unsigned int subtype, int mode) {
	for (int i = 1; i < 37; ++i) {

		if (!repentogonOptions.betterVoidGeneration && (i > 17 && i != 26) && i != 13)
			continue;
		else if (repentogonOptions.betterVoidGeneration && ((i > 17 && i < 26) || i == 34 || i == 35 || !IsFloorUnlocked(i)))
			continue;

		// Configure the subtype here because backwards uses a 
		// specific set of subtypes. I'll need the RNG object to 
		// make this seed dependant.
		/* if (i == 36) {

		} */
		std::vector<RoomConfig*> stageRooms = _this->GetRooms(i, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subtype, mode);
		rooms->insert(rooms->begin(), stageRooms.begin(), stageRooms.end());
	}
}

void ASMPatchVoidGeneration() {
	SigScan scanner("83f82374758b5dd4be010000008b7dd0");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ZHL::Log("Patching void generation at %p\n", addr);

	ASMPatch patch;
	ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::XMM0 | ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.PreserveRegisters(registers);
	patch.Push(ASMPatch::Registers::EBP, -0x38);
	patch.Push(ASMPatch::Registers::EBP, -0x68);
	patch.Push(ASMPatch::Registers::EBP, -0x40);
	patch.Push(0x14);
	patch.Push(ASMPatch::Registers::EBP, -0x70);
	patch.Push(ASMPatch::Registers::EBP, -0x74);
	patch.Push(ASMPatch::Registers::EBP, -0x78);
	patch.Push(ASMPatch::Registers::EBP, -0x7C);
	patch.Push(ASMPatch::Registers::EBP, -0x80);
	// patch.Push(ASMPatch::Registers::EBP, -0x64);
	patch.LoadEffectiveAddress(ASMPatch::Registers::EBP, -0x64, ASMPatch::Registers::EBX);
	patch.Push(ASMPatch::Registers::EBX);
	patch.Push(ASMPatch::Registers::EBP, -0x30);
	patch.AddInternalCall(VoidGenerationOverride);
	patch.RestoreRegisters(registers);
	// ASMPatch::ByteBuffer mov;
	// mov.AddString("c7459414").AddZeroes(3);
	patch.AddBytes("\xc7\x45\x94\x14").AddZeroes(3);
	// patch.AddBytes(mov);
	patch.AddRelativeJump((char*)addr + 108);

	sASMPatcher.PatchAt(addr, &patch);
}

/*
* It's Over.
* Hush's AI freaks out since Repentance due to an internal restructure of the code.
* The values Hush uses to track HP percentage internally was reduced by 100, but HP checks weren't.
* This makes Hush enter "panic" state at 50% HP and not 0.5%. Oops!
*/
float hushPanicLevel = 0.005f;
void __stdcall SetHushPanicLevel() {
	// This goes out to the masochists that want to deliberately play bugged Hush (hereby dubbed VINH MODE)
	hushPanicLevel = repentogonOptions.hushPanicStateFix ? 0.005f : 0.5f;
}

void PerformHushPatch(void* addr) { 

	void* panicPtr = &hushPanicLevel;
	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.AddInternalCall(SetHushPanicLevel) // call SetHushPanicLevel()
		.RestoreRegisters(reg)
		.AddBytes("\xF3\x0F\x10\x05").AddBytes(ByteBuffer().AddAny((char*)&panicPtr, 4))  // movss xmm0, dword ptr ds:[0xXXXXXXXX]
		.AddRelativeJump((char*)addr + 0x8); // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchHushBug() {
	SigScan scanner1("f30f1005????????0f2f85????????0f86????????33c9");
	SigScan scanner2("f30f1005????????0f2f85????????0f86????????837f??00");
	scanner1.Scan();
	scanner2.Scan();
	void* addrs[2] = { scanner1.GetAddress(), scanner2.GetAddress() };
	printf("[REPENTOGON] Patching the Hush panic state bug at %p, %p\n", addrs[0], addrs[1]);
	PerformHushPatch(addrs[0]);
	PerformHushPatch(addrs[1]);
}

/*
* Patch that fixes Null Items loaded from modded item.xmls
* 
* The game currently does not handle nullitems from modded xmls any differently than
* vanilla ones. That means it will just attempt to write the item's ItemConfig entry
* directly into the ItemConfig's nullitems std::vector, at the index of the ID specified
* in the xml (or 0, if not specified). This means that the game will either overwrite
* a vanilla nullitem at best, or write into memory outside of the bounds of the std::vector
* at worst, because the vector is only pre-sized to 132 to fit the vanilla nullitems.
* 
* This patch makes it so that any nullitem loaded from a mod is assigned the next available
* id and properly pushed to the std::vector.
*/
bool lastItemsXmlWasMod = false;

// Couldn't identify a clear way to identify if a mod is currently being loaded from within
// my patch, so I just track whether the last items.xml opened by the game is one from a mod.
HOOK_METHOD(ItemConfig, Load, (char* xmlpath, ModEntry* modentry)->void) {
	lastItemsXmlWasMod = (modentry != nullptr);
	super(xmlpath, modentry);
}

bool __stdcall FixLoadingNullItemFromXml(ItemConfig_Item* newNullItem) {
	if (lastItemsXmlWasMod) {
		// We are loading a Null Item from a mod's items.xml. The game will not
		// handle it properly, so assign it the next available ID ourselves, and
		// push it to the ItemConfig's nullitems std::vector properly.
		ItemConfig* itemConfig = g_Manager->GetItemConfig();
		newNullItem->id = itemConfig->GetNullItems()->size();
		itemConfig->GetNullItems()->push_back(newNullItem);
		return true;
	}
	return false;
}

void ASMPatchFixLoadingNullItemsFromXml() {
	// This is the spot in "parse_items_xml()" where a Null Item is going to be written into the ItemConfig's Null Items vector.
	SigScan scanner("8b34??85f60f84????????8bbe????????85ff74??8d8f????????e8????????8d8f????????e8????????689801000057e8????????83c408c786????????000000008d8e????????e8????????8d4e??e8????????8d4e??e8????????8d4e??e8????????68e000000056e8????????8bbd");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the commands we overwrote
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Push the newly created ItemConfig_Item
		.AddInternalCall(FixLoadingNullItemFromXml)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)addr + 0x5) // Jump for false (no new entry was added, continue as normal)
		.AddRelativeJump((char*)addr + 0x89);  // Jump for true (new entry was added, skip some internal code)
	sASMPatcher.PatchAt(addr, &patch);
}
// End of modded Null Items patch.

// MC_PRE_PLAYER_USE_BOMB
bool __stdcall ProcessPrePlayerUseBombCallback(Entity_Player* player) {
	int callbackid = 1220;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults lua_result = lua::LuaCaller(L).push(callbackid)
			.push(*player->GetVariant())
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.call(1);

		if (!lua_result && lua_isboolean(L, -1)) {
			return lua_toboolean(L, -1);
		}
	}
	return true;
}

void ASMPatchPrePlayerUseBomb() {
	SigScan scanner("6a0068050200008bcf");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Push the player
		.AddInternalCall(ProcessPrePlayerUseBombCallback)  // Run MC_PRE_PLAYER_USE_BOMB
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)addr + 0x7C5) // Jump for false (negate the bomb placement)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x7))  // Restore the 7 bytes we overwrote
		.AddRelativeJump((char*)addr + 0x7);  // Jump for true (allow the bomb placement)
	sASMPatcher.PatchAt(addr, &patch);
}

// MC_POST_PLAYER_USE_BOMB
void __stdcall ProcessPostPlayerUseBombCallback(Entity_Player* player, Entity_Bomb* bomb) {
	int callbackid = 1221;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(callbackid)
			.push(*player->GetVariant())
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(bomb, lua::Metatables::ENTITY_BOMB)
			.call(0);
	}
}

void ASMPatchPostPlayerUseBomb() {
	SigScan scanner("8b7424??46897424??3b7424??0f8c");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI)  // Push the BOMB
		.Push(ASMPatch::Registers::EDI)  // Push the PLAYER
		.AddInternalCall(ProcessPostPlayerUseBombCallback)  // Run MC_POST_PLAYER_USE_BOMB
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the 5 bytes we overwrote
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// MC_PRE_M_MORPH_ACTIVE
int __stdcall RunPreMMorphActiveCallback(Entity_Player* player, int collectibleId) {
	int callbackId = 1190;
	if (CallbackState.test(callbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackId)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(collectibleId)
			.call(1);
		
		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return 0;
				}
			} else if (lua_isinteger(L, -1)) {
				return (int)lua_tointeger(L, -1);
			}
		}
	}

	return collectibleId;
}

// MC_PRE_M_MORPH_ACTIVE
// This callback triggers when an active gets rerolled by 'M (trinket id 138) and allows for overriding its behavior.
void ASMPatchPreMMorphActiveCallback() {
	SigScan scanner("85f674??6a016a00");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Entity_Player::TriggerActiveItemUsed at %p\n", addr);

	ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS - ASMPatch::SavedRegisters::Registers::ESI, true);
	ASMPatch patch;
	patch.PreserveRegisters(registers)
		.Push(ASMPatch::Registers::ESI) // push the item id
		.Push(ASMPatch::Registers::EDI) // push the player
		.AddInternalCall(RunPreMMorphActiveCallback) // run MC_PRE_M_MORPH_ACTIVE
		.AddBytes("\x89\xC6") // mov esi, eax
		.AddBytes("\x85\xF6") // test esi, esi
		.RestoreRegisters(registers)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0x27) // jump for 0 (don't reroll the active)
		.AddBytes(ByteBuffer().AddAny((char*)addr + 4, 0x2)) // restore the instruction we overwrote
		.AddRelativeJump((char*)addr + 0x6); // jump for everything else (reroll the active)

	sASMPatcher.PatchAt(addr, &patch);
}

void __stdcall SetRoomClearDelay(Room* room) {
	room->_roomClearDelay = repentogonOptions.quickRoomClear ? 3 : 10;
}

void __stdcall CheckSaveStateClearDelay(void * addr) {
	ASMPatch flat;
	flat.AddBytes(ByteBuffer().AddByte(repentogonOptions.quickRoomClear ? 0x1 : 0x8));
	sASMPatcher.FlatPatch(addr, &flat);
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
		.Push((int)((char*)addr + 0x9)) // push address of number to compare
		.AddInternalCall(CheckSaveStateClearDelay) // call CheckSaveStateClearDelay()
		.RestoreRegisters(reg)
		.MoveFromMemory(ASMPatch::Registers::ESI, 0x11ec, ASMPatch::Registers::EAX)
		.AddRelativeJump((char*)addr + 0x6); // jump to DEC
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

void ASMPatchMenuOptionsLanguageChange() {
	// before lua reset in MenuOptions::Update
	// this only happens when switching the language in the Options menu.
	SigScan scanner("743e8b35????????68????????6a00e8????????83c408"); 
	scanner.Scan();
	void* addr1 = scanner.GetAddress();
	printf("[REPENTOGON] Patch options menu language change operate at %p.\n", addr1);
	ASMPatch patch;
	patch.AddInternalCall(GameRestart);
	sASMPatcher.PatchAt(addr1, &patch);
}

bool resSplit = false;
void __stdcall TrySplitTrampoline(Entity_NPC* npc, bool result) {
	resSplit = result;

	if (npc != nullptr) {
		int callbackid = 1191;

		if (CallbackState.test(callbackid - 1000)) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);

			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
				.push(npc->_type)
				.push(npc, lua::Metatables::ENTITY_NPC)
				.push(resSplit)
				
				.call(1);

			if (!result) {
				if (lua_isboolean(L, -1)) {
					resSplit = lua_toboolean(L, -1);
					return;
				}
			}
		}
		XMLAttributes xmlData = XMLStuff.EntityData->GetNodesByTypeVarSub(npc->_type, npc->_variant, npc->_subtype, false);
		const std::string nosplit = xmlData["nosplit"];

		if (nosplit == "true") {
			resSplit = true;
		}
		else if (nosplit == "false") {
			resSplit = false;
		}
	}
}

void ASMPatchTrySplit() {
	SigScan scanner("f30f100d????????0f2f8b");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	printf("[REPENTOGON] Patching Entity_NPC::TrySplit at %p\n", addr);
	void* ptr = &resSplit;
	const int numOverriddenBytes = 8;
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS + ASMPatch::SavedRegisters::Registers::XMM0 + ASMPatch::SavedRegisters::Registers::XMM1, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters);
	patch.AddBytes("\x0f\xb6\xc0") // MOVZX EAX,AL
		.Push(ASMPatch::Registers::EAX)  // Push current result
		.Push(ASMPatch::Registers::EBX)  // Push NPC
		.AddInternalCall(TrySplitTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\xA0").AddBytes(ByteBuffer().AddAny((char*)&ptr, 4)) // mov al, byte ptr ds:[XXXXXXXX]
		.AddBytes(ByteBuffer().AddAny((char*)addr, numOverriddenBytes))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + numOverriddenBytes);
	sASMPatcher.PatchAt(addr, &patch);
}

//////////////////////////////////////////////
// !!!!! GRID CALLBACK STUFF HERE !!!!!
//////////////////////////////////////////////

/* /////////////////
// Room::spawn_entity
*/ /////////////////

GridEntity* __stdcall RoomSpawnTrampoline(GridEntityType type, unsigned int variant, int vardata, unsigned int seed, unsigned int idx, unsigned int teleState) {
	const int callbackid = 1192;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push((unsigned int)type)
			.push((unsigned int)type)
			.push((unsigned int)variant)
			.push((int)vardata)
			.push((unsigned int)idx)
			.push((unsigned int)seed)
			.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				type = (GridEntityType)lua::callbacks::ToInteger(L, 1);
				variant = (unsigned int)lua::callbacks::ToInteger(L, 2);
				vardata = lua::callbacks::ToInteger(L, 3);
				seed = (unsigned int)lua::callbacks::ToInteger(L, 4);
			}
			else if (lua_isboolean(L, -1) && !lua_toboolean(L, -1))
			{
				return nullptr;
			}
		}
	}

	GridEntity* ent = CreateGridEntity(type, idx);
	if (ent != nullptr) {
		g_Game->_room->_gridEntities[idx] = ent;
		GridEntityDesc* desc = ent->GetDesc();
		desc->_variant = variant;
		desc->_varData = vardata;

		if (type == GRID_TELEPORTER) {
			desc->_state = teleState != 0 ? teleState : 0;
		}

		ent->Init(seed);
	}

	return ent;
}

void ASMPatchRoomSpawnEntity() {
	SigScan scanner_spawn_entity("e8????????8bc885c90f84????????837d??17");
	scanner_spawn_entity.Scan();
	void* addr = scanner_spawn_entity.GetAddress();

	printf("[REPENTOGON] Patching Room::spawn_entity at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS - ASMPatch::SavedRegisters::Registers::ECX, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBP, -0xa) //teleporter state
		.Push(ASMPatch::Registers::EDX) // idx
		.Push(ASMPatch::Registers::EBP, -0x40) // seed
		.Push(ASMPatch::Registers::EDI) // vardata
		.Push(ASMPatch::Registers::ESI) // variant
		.Push(ASMPatch::Registers::ECX) // type
		.AddInternalCall(RoomSpawnTrampoline)
		.AddBytes("\x85\xC0") // test eax,eax (does GridEntity exist)
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr - 0xf2a) // jump to misc + return false
		.AddRelativeJump((char*)addr + 0x3a); // jump to misc + return true
	sASMPatcher.PatchAt(addr, &patch);
}

/* /////////////////////
// Room::SpawnGridEntity
*/ /////////////////////

void __stdcall PostGridInitTrampoline(GridEntity* grid) {
	const int callbackid = 1101;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(grid->GetType())
			.push(grid, lua::Metatables::GRID_ENTITY)
			.call(1);
	}
}

void ASMPatchSpawnGridEntityPostInit(void* addr, unsigned int overriddenBytes) {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, overriddenBytes))  // Restore the commands we overwrote
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // GridEntity
		.AddInternalCall(PostGridInitTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + overriddenBytes);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchPostGridInit()
{
	SigScan scanner1("8947??b0015f5e5b5dc21400");
	SigScan scanner2("8947??b0015f5e5b595dc20800");
	scanner1.Scan();
	scanner2.Scan();
	void* addrs[2] = { scanner1.GetAddress(), scanner2.GetAddress() };
	
	printf("[REPENTOGON] Patching SpawnGridEntity POST_GRID_INIT at %p, %p\n", addrs[0], addrs[1]);
	ASMPatchSpawnGridEntityPostInit(addrs[0], 5);
	ASMPatchSpawnGridEntityPostInit(addrs[1], 8);
}

/* /////////////////////
// shared SpawnGridEntity trampoline
*/ /////////////////////

bool __stdcall SpawnGridEntityTrampoline(int idx, unsigned int type, unsigned int variant, unsigned int seed, int vardata) {
	return g_Game->_room->SpawnGridEntity(idx, type, variant, seed, vardata);
}

/* /////////////////////
// Generic inline patch
*/ /////////////////////

void ASMPatchInlinedSpawnGridEntity_Generic(void* addr, ASMPatch::Registers idxReg, int idxOffset, ASMPatch::Registers seedReg, int seedOffset, unsigned int jumpOffset, GridEntityType type, unsigned int variant, int vardata) {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push((int32_t)vardata); // vardata
	if (seedOffset != 0) {
		patch.Push(seedReg, seedOffset);
	}
	else
	{
		patch.Push(seedReg);
	}
	patch.Push((int32_t)variant) // variant
		.Push((int32_t)type); // type
	if (idxOffset != 0) {
		patch.Push(idxReg, idxOffset);
	}
	else
	{
		patch.Push(idxReg);
	}
	patch.AddInternalCall(SpawnGridEntityTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + jumpOffset);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchInlinedSpawnGridEntity()
{
	SigScan scanner_megafatty("8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c40489b5????????8bcec745??06000000e8????????c706");
	SigScan scanner_larryjr("8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c40489b5????????8bcec745??08000000");
	SigScan scanner_chub("8b0d????????8945??8b81");
	SigScan scanner_dingle1("8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c40489b5????????8bcec745??06000000e8????????8b8d");
	SigScan scanner_dingle2("8b0d????????8985????????8b81????????8985????????85f678??81fec00100007c??68????????6a03e8????????8b85????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c40489b5????????8bcec745??1c000000");
	SigScan scanner_gideon("8b0d????????8bbf");
	SigScan scanner_raglich_arm("8945??a1????????8b80");
	SigScan scanner_singe_explosive_fart("8bd885f678");
	SigScan scanner_player_collide_with_grid("8945??81ffc00100007d??8b75");
	SigScan scanner_make_wall1("8945??85ff78??81ffc00100007c??68????????6a03e8????????83c40881bc??????????840300007f??6854010000e8????????8bf083c4048975??8bcec745??00000000");
	SigScan scanner_make_wall2("8945??85ff78??81ffc00100007c??68????????6a03e8????????83c40881bc??????????840300007f??6854010000e8????????8bf083c4048975??8bcec745??27000000");
	SigScan scanner_card_against_humanity("8945??a1????????8bb0????????8975");
	SigScan scanner_pickup_grid_entity1("8945??81ffbf010000");
	SigScan scanner_pressure_plate_reward1("8bf885ff78??81ffc00100007c??68????????6a03e8????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c4048975??8bcec745??1c000000");
	SigScan scanner_pressure_plate_reward2("8bf885ff78??81ffc00100007c??68????????6a03e8????????83c40881bc??????????840300000f8f????????6874010000e8????????8bf083c4048975??8bcec745??06000000");
	scanner_megafatty.Scan();
	scanner_larryjr.Scan();
	scanner_chub.Scan();
	scanner_dingle1.Scan();
	scanner_dingle2.Scan();
	scanner_gideon.Scan();
	scanner_raglich_arm.Scan();
	scanner_singe_explosive_fart.Scan();
	scanner_player_collide_with_grid.Scan();
	scanner_make_wall1.Scan();
	scanner_make_wall2.Scan();
	scanner_card_against_humanity.Scan();
	scanner_pickup_grid_entity1.Scan();
	scanner_pressure_plate_reward1.Scan();
	scanner_pressure_plate_reward2.Scan();
	void* addrs[22] = {
		scanner_megafatty.GetAddress(),
		scanner_larryjr.GetAddress(),
		scanner_chub.GetAddress(),
		scanner_dingle1.GetAddress(),
		scanner_dingle2.GetAddress(),
		scanner_gideon.GetAddress(),
		scanner_raglich_arm.GetAddress(),
		0x0,
		scanner_singe_explosive_fart.GetAddress(),
		scanner_player_collide_with_grid.GetAddress(),
		0x0,
		0x0,
		0x0,
		0x0,
		scanner_make_wall1.GetAddress(),
		scanner_make_wall2.GetAddress(),
		scanner_card_against_humanity.GetAddress(),
		scanner_pickup_grid_entity1.GetAddress(),
		scanner_pressure_plate_reward1.GetAddress(),
		scanner_pressure_plate_reward2.GetAddress()
	};
	printf("[REPENTOGON] Patching inlined SpawnGridEntity starting from %p, read log for rest\n", addrs[0]);
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[0], ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0x0, 0xf0, GRID_POOP, 1, 0); // ai_mega_fatty
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[1], ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0x0, 0xf6, GRID_POOP, 0, 0); // ai_larryjr
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[2], ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0x0, 0x109, GRID_POOP, 1, 0); // ai_chub
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[3], ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0x0, 0x139, GRID_POOP, 1, 0); // ai_dingle (1)
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[4], ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0x0, 0xf0, GRID_POOP, 1, 0); // ai_dingle (2)
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[5], ASMPatch::Registers::EDI, 0xaf4, ASMPatch::Registers::EAX, 0x0, 0xc0, GRID_STAIRS, 1, 0); // CreateGideonDungeon
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[6], ASMPatch::Registers::EDI, 0, ASMPatch::Registers::EAX, 0x0, 0xc5, GRID_PIT, 0, 0); // ai_raglich_arm
	// There's a Rotgut function here that needs a patch
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[8], ASMPatch::Registers::ESI, 0, ASMPatch::Registers::EAX, 0x0, 0xbd, GRID_PIT, 0, 0); // DoExplosiveFart

	// This will need special handling, when trying to break a steel block it fails a GridPath check (I think) and returns without doing anything
	//ASMPatchInlinedSpawnGridEntity_Generic(addrs[9], ASMPatch::Registers::EDI, 0, ASMPatch::Registers::EAX, 0x0, 0x145, GRID_ROCK, 0, 0); // PlayerCollideWithGrid

	// So will BombDamage (Custom handling of valid grid idx range)
	// And BombFlagTearEffects (grid idx range)
	// BombFlagTearEffects again (??? can't tell)
	// And get_giant_part (Room path check)

	ASMPatchInlinedSpawnGridEntity_Generic(addrs[14], ASMPatch::Registers::EDI, 0, ASMPatch::Registers::EAX, 0x0, 0xa9, GRID_DECORATION, 10, 0); // make_wall (crawlspace ladder)
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[15], ASMPatch::Registers::EDI, 0, ASMPatch::Registers::EAX, 0x0, 0xa9, GRID_GRAVITY, 0, 0); // make_wall (crawlspace gravity)
	// Possibly will need another, unique patch for the default case of spawning a wall, idk if it's worth it
	ASMPatchInlinedSpawnGridEntity_Generic(addrs[16], ASMPatch::Registers::EDI, 0, ASMPatch::Registers::EAX, 0x0, 0xe1, GRID_POOP, 0, 0); // make_wall (crawlspace gravity)

	// This needs special handling bc the code loads g_Game back into EAX near the end of the inline
	//ASMPatchInlinedSpawnGridEntity_Generic(addrs[17], ASMPatch::Registers::EDI, 0, ASMPatch::Registers::EAX, 0x0, 0xd5, GRID_DECORATION, 1000, 0); // PickupGridEntity
	// So will the other use, inlining in this function is wacky
	// TrySpawnSanguineSpikes needs special handling to advance the seed before use
	
	// These aren't done yet
	//ASMPatchInlinedSpawnGridEntity_Generic(addrs[20], ASMPatch::Registers::EAX, 0, ASMPatch::Registers::EBP, -0x1c, 0x1164, GRID_POOP, 0, 0); // pressure plate reward (1)
	//ASMPatchInlinedSpawnGridEntity_Generic(addrs[21], ASMPatch::Registers::EAX, 0, ASMPatch::Registers::EBP, -0x1c, 0x1164, GRID_POOP, 3, 0); // pressure plate reward (2)
}

void PatchGridCallbackShit()
{
	ASMPatchRoomSpawnEntity();
	PatchPostGridInit();
	PatchInlinedSpawnGridEntity();
}

//////////////////////////////////////////////
// !!!!! GRID CALLBACK STUFF DONE !!!!!
//////////////////////////////////////////////

void PerformASMPatches() {
	ASMPatchLogMessage();
	ASMPatchAmbushWaveCount();
	ASMPatchMegaSatanEnding();
	ASMPatchConsoleRunCommand();
	ASMPatchBlueWombCurse();
	ASMPatchModsMenu();
	PatchFireProjectiles();
	PatchFireBossProjectiles();
	LuaRender::PatchglDrawElements();
	PatchPreSampleLaserCollision();
	PatchPreLaserCollision();
	PatchPreEntityTakeDamageCallbacks();
	PatchPostEntityTakeDamageCallbacks();
	PatchFamiliarCanBeDamagedByLaser();
	PatchCheckFamiliar();
	ASMPatchVoidGeneration();
	ASMPatchHushBug();
	ASMPatchFamiliarGetMultiplier();
	ASMPatchFixLoadingNullItemsFromXml();
	ASMPatchPrePlayerUseBomb();
	ASMPatchPostPlayerUseBomb();
	ASMPatchPreMMorphActiveCallback();
	PatchRoomClearDelay();
	ASMPatchMenuOptionsLanguageChange();
	ASMPatchTrySplit();
	PatchGridCallbackShit();
}