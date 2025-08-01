#include "IsaacRepentance.h"

#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "../XMLData.h"
#include "../../LuaInit.h"
#include "Log.h"
#include "../../Patches/MainMenuBlock.h"

static inline void* get_sig_address(const char* signature, const char* location, const char* callbackName)
{
	SigScan scanner(signature);
	if (!scanner.Scan())
	{
		ZHL::Log("[ASSERT] [ASMPatch] Did not find \"%s\" for %s patch\n", signature, callbackName);
		assert(false);
	}
	return scanner.GetAddress();
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
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PreSampleLaserCollision);

	printf("[REPENTOGON] Patching Entity_Laser::damage_entities for MC_PRE_LASER_COLLISION (sample laser) at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EAX) // Collider Entity
		.AddBytes("\xFF\x75\x8C")  // push dword ptr [ebp-0x74] (Entity_Laser)
		.AddInternalCall(RunPreLaserCollisionCallback) // Run PRE_LASER_COLLISION callback
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr - 0x30)  // Ignore collision
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0x6);  // Allow collision
	sASMPatcher.PatchAt(addr, &patch);
}

// This patch injects the PRE_LASER_COLLISION callback for "non-sample" lasers (ones that can ONLY ever be a straight line).
void PatchPreLaserCollision() {
	SigScan scanner("f30f1095????????8bcf56");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Entity_Laser::damage_entities for MC_PRE_LASER_COLLISION (non-sample laser) at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.AddBytes("\x56\x57") // Push EDI (entity) and ESI (laser) for function input
		.AddInternalCall(RunPreLaserCollisionCallback) // Run PRE_LASER_COLLISION callback
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x494) // Ignore collision
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x8))  // Restore the bytes we overwrote
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
bool __stdcall ProcessPreDamageCallback(Entity* entity, float* damage, int* damageHearts, uint64_t* damageFlags, EntityRef* source, int* damageCountdown, const bool isPlayer) {
	const int callbackid = 11;

	if (VanillaCallbackState.test(callbackid)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		unsigned int entityType = *entity->GetType();

		lua::LuaCaller caller(L);
		caller.push(callbackid)
			.push(entityType)
			.push(entity, lua::Metatables::ENTITY);
		if (isPlayer && damageHearts) {
			caller.push(*damageHearts);
		} else {
			caller.push(*damage);
		}
		caller.push(*damageFlags)
			.push(source, lua::Metatables::ENTITY_REF)
			.push(*damageCountdown);
		lua::LuaResults lua_result = caller.call(1);

		if (!lua_result) {
			if (lua_isboolean(L, -1)) {
				return lua_toboolean(L, -1);
			} else if (lua_istable(L, -1)) {
				const uint64_t originalDamageFlags = *damageFlags;

				// New table return format for overriding values.
				lua_pushnil(L);
				while (lua_next(L, -2) != 0) {
					if (lua_isstring(L, -2)) {
						const std::string key = lua_tostring(L, -2);
						if (key == "Damage" && lua_isnumber(L, -1)) {
							float newDamage = (float)lua_tonumber(L, -1);
							if (newDamage < 0) {
								newDamage = 0;
							}
							if (isPlayer && damageHearts) {
								*damageHearts = (int)std::round(newDamage);
							}
							*damage = newDamage;
						} else if (key == "DamageFlags" && lua_isinteger(L, -1)) {
							*damageFlags = lua_tointeger(L, -1);
						} else if (key == "DamageCountdown" && lua_isnumber(L, -1)) {
							*damageCountdown = (int)lua_tonumber(L, -1);
						}
					}
					lua_pop(L, 1);
				}

				// Retroactively fix the behaviour of certain DamageFlags.
				// For example, ones that the game normally checks for BEFORE this callback runs.
				const uint64_t flagsAdded = (*damageFlags) & ~originalDamageFlags;
				//const uint64_t flagsRemoved = originalDamageFlags & ~(*damageFlags);
				//const uint64_t flagsModified = flagsAdded | flagsRemoved;

				if (isPlayer) {
					if (damageHearts && (flagsAdded & DAMAGE_FAKE) != 0) {
						*damageHearts = 0;
					}

					// DamageFlag.DAMAGE_NO_MODIFIERS affects the damage value before the callback runs, but
					// IDK if I want to bother trying to fix that retroactively. Just change the damage yourself.
				} else {
					if ((flagsAdded & DAMAGE_FIRE) != 0) {
						if (*entity->GetFireDamageCountdown() > 0) {
							return false;
						}
						*entity->GetFireDamageCountdown() = 10;
					}
					if ((flagsAdded & DAMAGE_COUNTDOWN) != 0 && *entity->GetDamageCountdown() > 0) {
						return false;
					}
					// If the DAMAGE_COUNTDOWN flag is currently present, set the entity's
					// DamageCountdown to the now potentially-modified value.
					if ((*damageFlags & DAMAGE_COUNTDOWN) != 0) {
						*entity->GetDamageCountdown() = *damageCountdown;
					}
				}
			}
		}
	}

	if (isPlayer && damageHearts) {
		*damage = (float)*damageHearts;
	}

	return true;
}

void InjectPreDamageCallback(void* addr, bool isPlayer) {
	printf("[REPENTOGON] Patching %s::TakeDamage for MC_ENTITY_TAKE_DMG at %p\n", isPlayer ? "Entity_Player" : "Entity", addr);

	const int lowerFlagsOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_FlagsOffset) + 2);
	const int upperFlagsOffset = lowerFlagsOffset + 4;
	const int damageOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_AmountFloatOffset) + 4);
	const int sourceOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_SourceOffset) + 2);
	const int countdownOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_CountdownOffset) + 2);
	const int damageHeartsOffset = *(int*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_PlayerAmountHeartsOffset) + 4);
	const int isRedHeartDamageOffset = *(int*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_PlayerRedHeartDamagedOffset) + 2);
	const int playerCopiedLowerFlagsOffset = *(int*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_PlayerLowerFlagsOffset) + 2);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.AddBytes("\x58\x58\x58\x58\x58")  // Pop EAX x5 to remove the overridden function's inputs from the stack.
		.PreserveRegisters(savedRegisters)
		.AddBytes("\x31\xC0");  // xor eax,eax
	if (isPlayer) {
		patch.AddBytes("\xB0\x01");  // mov al, 0x1
	}
	patch.Push(ASMPatch::Registers::EAX)  // Push a boolean to identify if this is EntityPlayer:TakeDamage
		.AddBytes("\x8D\x9D").AddBytes(ByteBuffer().AddAny((char*)&countdownOffset, 0x4)).Push(ASMPatch::Registers::EBX)  // lea ebx [ebp+?] ; push ebx
		.Push(ASMPatch::Registers::EBP, sourceOffset)
		.AddBytes("\x8D\x9D").AddBytes(ByteBuffer().AddAny((char*)&lowerFlagsOffset, 0x4)).Push(ASMPatch::Registers::EBX);
	if (isPlayer) {
		patch.AddBytes("\x8D\x9D").AddBytes(ByteBuffer().AddAny((char*)&damageHeartsOffset, 0x4)).Push(ASMPatch::Registers::EBX);
	} else {
		patch.Push(0x0);
	}
	patch.AddBytes("\x8D\x9D").AddBytes(ByteBuffer().AddAny((char*)&damageOffset, 0x4)).Push(ASMPatch::Registers::EBX)
		.Push(ASMPatch::Registers::EDI)  // Push Entity pointer
		.AddInternalCall(ProcessPreDamageCallback);  // Run the new MC_ENTITY_TAKE_DMG callback
	if (isPlayer) {
		// For the player, also copy the (potentially modified) lower DamageFlags to [EBP+?]
		patch.AddBytes("\x8B\x9D").AddBytes(ByteBuffer().AddAny((char*)&lowerFlagsOffset, 0x4))  // mov ebx,dword ptr [EBP+?]
			.AddBytes("\x89\x9D").AddBytes(ByteBuffer().AddAny((char*)&playerCopiedLowerFlagsOffset, 0x4))  // mov dword ptr [EBP+?],ebx
			// Update a boolean that was previously populated based on whether the DamageFlags contained DAMAGE_RED_HEARTS or DAMAGE_NO_PENALTIES.
			// Affects whether LevelStateFlag.STATE_REDHEART_DAMAGED is set, potentially among other things.
			.AddBytes("\xC6\x85").AddBytes(ByteBuffer().AddAny((char*)&isRedHeartDamageOffset, 0x4)).AddBytes("\x01")
			.AddBytes("\x81\xE3\x20").AddZeroes(2).AddBytes("\x10")  // and ebx,0x10000020 (DAMAGE_RED_HEARTS | DAMAGE_NO_PENALTIES)
			.AddBytes("\x74\x07")  // JZ (set to 0 if neither flag is present)
			.AddBytes("\xC6\x85").AddBytes(ByteBuffer().AddAny((char*)&isRedHeartDamageOffset, 0x4)).AddZeroes(1);
	}
	patch.RestoreRegisters(savedRegisters);
	if (!isPlayer) {
		// For non-players, also copy the (potentially modified) DamageFlags into EBX and ESI
		patch.AddBytes("\x8B\x9D").AddBytes(ByteBuffer().AddAny((char*)&lowerFlagsOffset, 0x4))  // mov ebx,dword ptr [EBP+?]
			.AddBytes("\x8B\xB5").AddBytes(ByteBuffer().AddAny((char*)&upperFlagsOffset, 0x4));  // mov esi,dword ptr [EBP+?]
	}
	patch.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// Patches overtop the existing calls to LuaEngine::Callback::EntityTakeDamage,
// within the Entity::TakeDamage and EntityPlayer::TakeDamage functions respectively.
void PatchPreEntityTakeDamageCallbacks() {
	InjectPreDamageCallback(sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_PreEntityCallback), false);
	InjectPreDamageCallback(sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_PrePlayerCallback), true);
}

// Even though I patched overtop the calls to this callback, super kill it for good measure.
HOOK_METHOD(LuaEngine, EntityTakeDamage, (Entity* entity, float damage, unsigned long long damageFlags, EntityRef* source, int damageCountdown) -> bool) { return true; }

// End of ENTITY_TAKE_DMG patches

/* * MC_POST_ENTITY_TAKE_DMG * *
* A hook worked just fine for ""POST_TAKE_DMG"" initially, but now that we support overidding
* values via ENTITY_TAKE_DMG (see above) we need the POST_TAKE_DMG callback to actually show
* those modified values. That means another patch!!
* Like before, we need to patch into both Entity::TakeDamage and EntityPlayer::TakeDamage.
*/

void __stdcall ProcessPostDamageCallback(Entity* entity, const float damage, const uint64_t damageFlags, EntityRef* source, const int damageCountdown, const bool isPlayer) {
	const int callbackid = 1006;
	if (CallbackState.test(callbackid - 1000)) {
		if (isPlayer && source->_type == 33 && source->_variant == 4) {
			// The white fireplace is a unique case where the game considers the player to have taken "damage"
			// but no on-damage effets are triggered. Don't run the post-damage callback in this case.
			return;
		}

		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller caller(L);
		caller.push(callbackid)
			.push(*entity->GetType())
			.push(entity, lua::Metatables::ENTITY);
		if (isPlayer) {
			caller.push((int)std::round(damage));
		} else {
			caller.push(damage);
		}
		caller.push(damageFlags)
			.push(source, lua::Metatables::ENTITY_REF)
			.push(damageCountdown)
			.call(1);
	}
};

void InjectPostDamageCallback(void* addr, bool isPlayer) {
	printf("[REPENTOGON] Patching %s::TakeDamage for MC_POST_ENTITY_TAKE_DMG at %p\n", isPlayer ? "Entity_Player" : "Entity", addr);
	const int numOverriddenBytes = (isPlayer ? 10 : 5);

	const int lowerFlagsOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_FlagsOffset) + 2);
	const int upperFlagsOffset = lowerFlagsOffset + 4;
	const int damageOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_AmountFloatOffset) + 4);
	const int sourceOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_SourceOffset) + 2);
	const int countdownOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_CountdownOffset) + 2);
	const int damageHeartsOffset = *(int*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_PlayerAmountHeartsOffset) + 4);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	if (isPlayer) {
		patch.AddBytes(ByteBuffer().AddAny((char*)addr, numOverriddenBytes));  // Restore the commands we overwrote
		// The EntityPlayer::TakeDamage patch needs to ask Al if the damage occured.
		// The Entity::TakeDamage patch doesn't need to do this since it is at a location that only runs after damage.
		patch.AddBytes("\x84\xC0") // test al, al
			.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + numOverriddenBytes);
	}
	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\x31\xC0");  // xor eax,eax
	if (isPlayer) {
		patch.AddBytes("\xB0\x01");  // Move al, 0x1
	}
	patch.Push(ASMPatch::Registers::EAX)  // Push a boolean to identify if this is EntityPlayer:TakeDamage
		.Push(ASMPatch::Registers::EBP, countdownOffset)
		.Push(ASMPatch::Registers::EBP, sourceOffset)
		.Push(ASMPatch::Registers::EBP, upperFlagsOffset)
		.Push(ASMPatch::Registers::EBP, lowerFlagsOffset)
		.Push(ASMPatch::Registers::EBP, damageOffset)
		.Push(ASMPatch::Registers::EDI)  // Push Entity pointer
		.AddInternalCall(ProcessPostDamageCallback)  // Run the MC_POST_(ENTITY_)TAKE_DMG callback
		.RestoreRegisters(savedRegisters);
	if (!isPlayer) {
		patch.AddBytes(ByteBuffer().AddAny((char*)addr, numOverriddenBytes));  // Restore the commands we overwrote
	}
	patch.AddRelativeJump((char*)addr + numOverriddenBytes);
	sASMPatcher.PatchAt(addr, &patch);
}

// These patches overwrite suitably-sized commands near where the respective TakeDamage functions would return.
// The overridden bytes are restored by the patch.
void PatchPostEntityTakeDamageCallbacks() {
	// Sig is 6 bytes before where we actually want to patch to avoid bleeding past the end of the function.
	InjectPostDamageCallback((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_PostEntityCallback) + 0x6, false);
	InjectPostDamageCallback(sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TakeDmg_PostPlayerCallback), true);
}

// End of POST_ENTITY_TAKE_DMG patches


// MC_PRE_PLAYER_USE_BOMB
bool __stdcall ProcessPrePlayerUseBombCallback(Entity_Player* player) {
	const int callbackid = 1220;
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
	// Address of the conditional jump before the player places a bomb.
	SigScan scanner("0f8f????????6a0068050200008bcfe8");
	scanner.Scan();
	void* jumpAddr = scanner.GetAddress();

	// Get the address we should jump to if we want to cancel the player's bomb placement.
	// Pull the jump offset from the existing conditional jump.
	void* cancelAddr = (char*)jumpAddr + 0x6 + *(int*)ByteBuffer().AddAny((char*)jumpAddr + 0x2, 4).GetData();

	// Address that we want to patch at, immediately after the existing conditional jump.
	void* patchAddr = (char*)jumpAddr + 0x6;

	printf("[REPENTOGON] Patching Entity_Player::control_bombs (pre) at %p\n", patchAddr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Push the player
		.AddInternalCall(ProcessPrePlayerUseBombCallback)  // Run MC_PRE_PLAYER_USE_BOMB
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, cancelAddr) // Jump for false (cancel the bomb placement)
		.AddBytes(ByteBuffer().AddAny((char*)patchAddr, 0x7))  // Restore the 7 bytes we overwrote
		.AddRelativeJump((char*)patchAddr + 0x7);  // Jump for true (allow the bomb placement)
	sASMPatcher.PatchAt(patchAddr, &patch);
}

// MC_POST_PLAYER_USE_BOMB
void __stdcall ProcessPostPlayerUseBombCallback(Entity_Player* player, Entity_Bomb* bomb) {
	const int callbackid = 1221;
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
	SigScan scanner("8b4424??40894424??3b4424??0f8c????????5f");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Entity_Player::control_bombs (post) at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX)  // Push the BOMB
		.Push(ASMPatch::Registers::EDI)  // Push the PLAYER
		.AddInternalCall(ProcessPostPlayerUseBombCallback)  // Run MC_POST_PLAYER_USE_BOMB
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the 5 bytes we overwrote
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// MC_PRE_M_MORPH_ACTIVE
int __stdcall RunPreMMorphActiveCallback(Entity_Player* player, int collectibleId) {
	const int callbackId = 1190;
	if (CallbackState.test(callbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(collectibleId)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return 0;
				}
			}
			else if (lua_isinteger(L, -1)) {
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

bool resSplit = false;
void __stdcall TrySplitTrampoline(Entity_NPC* npc, bool result) {
	resSplit = result;

	if (npc != nullptr) {
		const int callbackid = 1191;

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
	SigScan scanner("f30f100d????????0f2f8f");
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
		.Push(ASMPatch::Registers::EDI)  // Push NPC
		.AddInternalCall(TrySplitTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\xA0").AddBytes(ByteBuffer().AddAny((char*)&ptr, 4)) // mov al, byte ptr ds:[XXXXXXXX]
		.AddBytes(ByteBuffer().AddAny((char*)addr, numOverriddenBytes))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + numOverriddenBytes);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchInputAction()
{
	const char* signatures[] = {
		"837f??0275??833d????????0074??8d45??505351",  // Manager::GetActionValue
		"8379??0275??833d????????00",  // Console::ProcessInput
		"837f??0275??833d????????0074??8d45??50536a00",  // Manager::IsActionPressed
		"837f??0275??833d????????0074??8d45??50536a01",  // Manager::IsActionTriggered
		"8378??0275??833d????????0074??8d45??506a0e",  // Cutscene::Update (ACTION_MENUCONFIRM)
		"8378??0275??833d????????0074??8d45??506a0f",  // Cutscene::Update (ACTION_MENUBACK)
		NULL
	};

	const char** start = signatures;
	while (*start) {
		const char* signature = *start;
		SigScan scanner(signature);
		if (!scanner.Scan()) {
			ZHL::Log("[fatal] ASMPatchInputAction: could not find signature %s\n", signature);
			throw std::runtime_error("Could not find signature");
		}
		else {
			ZHL::Log("[info] ASMPatchInputAction: found signature at %p\n", scanner.GetAddress());
		}

		ASMPatch patch(ByteBuffer().AddByte(0x90, 6));
		sASMPatcher.FlatPatch(scanner.GetAddress(), &patch);

		++start;
	}
}

void __stdcall RunPostNightmareSceneRender(NightmareScene* ns) {
	const int callbackid = 1102;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			//.push(ns, lua::metatables::NightmareSceneMT)
			.call(1);
	}
}

void ASMPatchPostNightmareSceneCallback() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;

	SigScan scanner_transition("f30f108f????????0f57d20f2fca0f86");
	scanner_transition.Scan();
	void* addr = scanner_transition.GetAddress();
	printf("[REPENTOGON] Patching NightmareScene::Render at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // NightmareScene
		.AddInternalCall(RunPostNightmareSceneRender)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 8))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 8);
	sASMPatcher.PatchAt(addr, &patch);
}

void __stdcall RunPrePickupVoided(std::vector<Entity_Pickup*>* voidedItems, Entity_Pickup** pPickup) {
	Entity_Pickup* pickup = *pPickup;
	const int callbackid = 1265;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*pickup->GetVariant())
			.push(pickup, lua::Metatables::ENTITY_PICKUP)
			.push(false)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				if (!lua_toboolean(L, -1)) {
					return;
				}
			}
		}
	}

	voidedItems->push_back(pickup);
}

void ASMPatchPrePickupVoided() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan scanner("E8????????8BCF46E8????????3BF072??8BBD????????33");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ZHL::Logger logger;
	logger.Log("[REPENTOGON] Patching Entity_Player::UseActiveItem VOID at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX) // push the pickup
		.Push(ASMPatch::Registers::ECX) // push the vector
		.AddInternalCall(RunPrePickupVoided)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x83\xC4\x04") // add esp, 4
		.AddRelativeJump((char*)addr + 5);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall RunPrePickupVoidedBlackRune(Entity_Pickup* pickup) {
	const int callbackid = 1265;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*pickup->GetVariant())
			.push(pickup, lua::Metatables::ENTITY_PICKUP)
			.push(true)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				return (bool)lua_toboolean(L, -1);
			}
		}
	}

	return true;
}

// this is so repetitive, might restructure it later
void ASMPatchPrePickupVoidedBlackRune() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan scanner("e8????????83f8640f85????????8b8d");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ZHL::Logger logger;
	logger.Log("[REPENTOGON] Patching Entity_Player::UseCard BLACK RUNE at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX) // push the pickup
		.AddInternalCall(RunPrePickupVoidedBlackRune)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0x1b9) // jump for false
		.AddInternalCall(((char*)addr + 0x5) + *(ptrdiff_t*)((char*)addr + 0x1)) // restore the commands we overwrote (god this is ugly)
		.AddRelativeJump((char*)addr + 0x5);

	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall RunPrePickupVoidedAbyss(Entity_Pickup* pickup) {
	const int callbackid = 1266;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*pickup->GetVariant())
			.push(pickup, lua::Metatables::ENTITY_PICKUP)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				return (bool)lua_toboolean(L, -1);
			}
		}
	}

	return true;
}

void ASMPatchPrePickupVoidedAbyss() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan scanner("50e8????????508bcee8????????8bd0");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ZHL::Logger logger;
	logger.Log("[REPENTOGON] Patching Entity_Player::UseActiveItem ABYSS at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX) // push the pickup
		.AddInternalCall(RunPrePickupVoidedAbyss)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0xAA) // jump for false
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x1)) // restore push eax
		.AddInternalCall(((char*)addr + 0x6) + *(ptrdiff_t*)((char*)addr + 0x2)) // restore the function call
		.AddRelativeJump((char*)addr + 0x6);

	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall RunPrePickupComposted(Entity_Pickup* pickup) {
	const int callbackid = 1267;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(*pickup->GetVariant())
			.push(pickup, lua::Metatables::ENTITY_PICKUP)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				return (bool)lua_toboolean(L, -1);
			}
		}
	}

	return true;
}

void ASMPatchPrePickupComposted() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	SigScan scanner("8b40??ffd0ff85????????8d8e");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ZHL::Logger logger;
	logger.Log("[REPENTOGON] Patching Entity_Player::UseActiveItem COMPOSTED at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX) // push the pickup
		.AddInternalCall(RunPrePickupComposted)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0x58) // jump for false
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5)) // restore mov eax, dword[eax + 0x28]
		.AddRelativeJump((char*)addr + 0x5);

	sASMPatcher.PatchAt(addr, &patch);
}

void __stdcall RunPostChampionRegenCallback(Entity_NPC* npc) {
	// These calls get skipped over due to the placement of the patch, so just do them here.
	npc->_pathfinder.Reset();
	npc->ResetColor();

	const int callbackid = 1223;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(callbackid)
			.push(*npc->GetType())
			.push(npc, lua::Metatables::ENTITY_NPC)
			.call(0);
	}
}

void ASMPatchPostChampionRegenCallback() {
	// Scan for the address to jump to after we're done.
	// This is where the code would end up after the regen is done.
	SigScan exitScanner("8d77??83bf????????19");
	exitScanner.Scan();
	void* exitAddr = exitScanner.GetAddress();

	// The easiest place to patch in was over the call to NPCAI_Pathfinder::Reset()
	// It's the last thing that is done in the block that ONLY handles the end of the regen, after the Entity_NPC's Sprite is reset, etc.
	// There's actually two of them because Clickety Clacks have some special handling for being dark red champions, but we can patch both the same.
	const std::vector<std::string> signatures = {
		"e8????????eb??8bcfe8????????d80d",  // NPCAI_Pathfinder::Reset() call for most entities
		"e8????????e9????????68????????8d8f"  // NPCAI_Pathfinder::Reset() call for clickety clacks specifically
	};

	for (const std::string& sig : signatures) {
		ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
		ASMPatch patch;

		SigScan scanner(sig.c_str());
		scanner.Scan();
		void* addr = scanner.GetAddress();

		printf("[REPENTOGON] Patching Entity_NPC::Update for dark red champion regen callback at %p\n", addr);

		patch.AddBytes("\x8B\x07")  // mov eax, dword ptr ds:[edi]
			.AddBytes("\x8B\xCF")  // mov ecx,edi
			.PreserveRegisters(savedRegisters)
			.Push(ASMPatch::Registers::EDI) // Push the Entity_NPC
			.AddInternalCall(RunPostChampionRegenCallback)
			.RestoreRegisters(savedRegisters)
			.AddBytes("\x31\xF6")  // xor esi,esi
			.AddRelativeJump((char*)exitAddr);

		sASMPatcher.PatchAt(addr, &patch);
	}
}

// MC_PRE_PLAYERHUD_TRINKET_RENDER (1264)
std::unordered_map<short, Vector[2]> trinketRenderCropOffsetCache;

bool __stdcall RunTrinketRenderCallback(PlayerHUD* playerHUD, uint32_t slot, Vector* position, float* scale) {
	Entity_Player* player = playerHUD->_player;

	Vector cropOffset(0, 0);

	// This is the only thing vanilla does with CropOffset
	if (player && playerHUD->_trinket[slot].id == TRINKET_MONKEY_PAW) {
		cropOffset.x = (3 - player->_monkeyPawCounter) * 32.0f;
	}

	const int callbackid = 1264;
	if (player && CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(slot)
			.push(slot)
			.pushUserdataValue(*position, lua::Metatables::VECTOR)
			.push(*scale)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.pushUserdataValue(cropOffset, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_istable(L, -1)) {
				lua_pushnil(L);
				while (lua_next(L, -2) != 0) {
					if (lua_isstring(L, -2) && lua_isuserdata(L, -1)) {
						const std::string key = lua_tostring(L, -2);
						if (key == "Position") {
							Vector *new_pos = lua::GetLuabridgeUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
							*(position) = *(new_pos);
						}
						else if (key == "CropOffset") {
							cropOffset = *lua::GetLuabridgeUserdata<Vector*>(L, -1, lua::Metatables::VECTOR, "Vector");
						}
					}
					else if (lua_isstring(L, -2) && lua_isnumber(L, -1)) {
						const std::string key = lua_tostring(L, -2);
						if (key == "Scale") {
							*scale = (float)lua_tonumber(L, -1);
						}
					}
					lua_pop(L, 1);
				}
			}
			else if (lua_isboolean(L, -1))
			{
				if (lua_toboolean(L, -1)) {
					return false;
				}
			}
		}
	}

	trinketRenderCropOffsetCache[playerHUD->_playerHudIndex][slot] = cropOffset;

	return true;
};

// Hook that runs the callback
HOOK_METHOD(PlayerHUD, RenderTrinket, (uint32_t slot, Vector* pos, float unused, float scale, bool unk) -> void) {
	if (slot < 0 || slot > 1 || this->_trinket[slot].id == TRINKET_NULL || this->_trinket[slot].image == nullptr) {
		return;
	}
	Vector posCopy = *pos;  // Copy this in case the callback modifies it.
	if (RunTrinketRenderCallback(this, slot, &posCopy, &scale)) {
		super(slot, &posCopy, unused, scale, unk);
	}
}

// Fetches the cached CropOffset for this PlayerHUDTrinket and puts it in xmm1/xmm2
void __stdcall GetPlayerHUDTrinketCropOffset(PlayerHUDTrinket* hudTrinket, Entity_Player* player) {
	short playerHudIndex = 0;
	int slot = 0;

	// Slightly unfortunate, but we need to find the corresponding PlayerHUD in order to tell which slot is being rendered.
	for (int i = 0; i < 8; i++) {
		PlayerHUD* playerHud = g_Game->GetPlayerHUD(i);
		if (playerHud && playerHud->_player == player) {
			playerHudIndex = playerHud->_playerHudIndex;
			if (&playerHud->_trinket[1] == hudTrinket) {
				slot = 1;
			}
			break;
		}
	}

	const Vector& cropOffset = trinketRenderCropOffsetCache[playerHudIndex][slot];

	float x = cropOffset.x;
	float y = cropOffset.y;

	__asm {
		movd xmm1, x
		movd xmm2, y
	}
}

// Patches overtop where the CropOffset is calculated to use our cached one instead.
void ASMPatchTrinketRender() {
	SigScan signature("75??8b45??b903000000");
	signature.Scan();

	void* addr = signature.GetAddress();

	printf("[REPENTOGON] Patching PlayerHUDTrinket::Render at %p\n", addr);

	ASMPatch patch;
	ASMPatch::SavedRegisters savedRegisters((ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::XMM_REGISTERS) & ~(ASMPatch::SavedRegisters::XMM1 | ASMPatch::SavedRegisters::XMM2), true);
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBP, 0x10)  // Entity_Player*
		.Push(ASMPatch::Registers::EDX)  // PlayerHUDTrinket*
		.AddInternalCall(GetPlayerHUDTrinketCropOffset)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x1F);

	sASMPatcher.PatchAt(addr, &patch);
}

//MC_PRE_PICKUP_UPDATE_GHOST_PICKUPS (1335)
bool __stdcall RunPickupUpdatePickupGhostsCallback(Entity_Pickup* pickup) {
	const int callbackid = 1335;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(pickup, lua::Metatables::ENTITY_PICKUP)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1)) {
				return (bool)lua_toboolean(L, -1);
			}
		}
	}

	return (g_Game->_playerManager.FirstCollectibleOwner(COLLECTIBLE_GUPPYS_EYE, nullptr, true) != nullptr &&
		((pickup->IsChest(pickup->_variant) && pickup->_subtype != 0) || (pickup->_variant == 69 && !pickup->_dead))
			&& (pickup->_timeout < 1) );
}

void ASMPatchPickupUpdatePickupGhosts() {
    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS - ASMPatch::SavedRegisters::Registers::EAX, true);
    ASMPatch patch;

    SigScan signature("e8????????85c00f84????????8b4e2ce8????????");
    signature.Scan();

    void* addr = signature.GetAddress();

    printf("[REPENTOGON] Patching Pickup::UpdatePickupGhosts at %p\n", addr);

    patch.Pop(ASMPatch::Registers::EAX)
        .Pop(ASMPatch::Registers::EAX)
        .Pop(ASMPatch::Registers::EAX)  // Pop everything that was pushed onto the stack intended for the overwritten call to FirstCollectibleOwner.
        .PreserveRegisters(savedRegisters)
        .Push(ASMPatch::Registers::ESI) // pickup
        .AddInternalCall(RunPickupUpdatePickupGhostsCallback)
        .AddBytes("\x84\xC0") // test al, al
        .RestoreRegisters(savedRegisters)
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x42) // jump for true
        //.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))
        .AddRelativeJump((char*)addr + 0xCC);
    sASMPatcher.PatchAt(addr, &patch);
}

// MC_POST_PROJECTILE_DEATH
void __stdcall RunProjectileDeathCallback(Entity_Projectile* proj) {
	const int callbackid = 1032;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(proj->_variant)
			.push(proj, lua::Metatables::ENTITY_PROJECTILE)
			.call(1);
	}

	// The patch is overriding the call to remove the projectile.
	proj->Remove();
}

void ASMPatchProjectileDeath() {
	SigScan signature("8bcfff50??837f??02");
	signature.Scan();
	void* addr = signature.GetAddress();

	printf("[REPENTOGON] Patching Entity_Projectile::Update at %p for MC_POST_PROJECTILE_DEATH\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.AddBytes("\x8B\xCF") // mov ecx, edi (restoring some overridden bytes)
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // Entity_Projectile*
		.AddInternalCall(RunProjectileDeathCallback)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// MC_POST_TEAR_DEATH
void __stdcall RunTearDeathCallback(Entity_Tear* tear) {
	const int callbackid = 1033;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(tear->_variant)
			.push(tear, lua::Metatables::ENTITY_TEAR)
			.call(1);
	}

	// The patch is overriding the call to remove the tear.
	tear->Remove();
}

void ASMPatchTearDeath() {
	SigScan signature("8b40??ffd0e9????????68????????8bd68d8d");
	signature.Scan();
	void* addr = signature.GetAddress();

	printf("[REPENTOGON] Patching Entity_Tear::Update at %p for MC_POST_TEAR_DEATH\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // Entity_Tear*
		.AddInternalCall(RunTearDeathCallback)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// MC_PRE_PLAYER_GIVE_BIRTH_CAMBION (1474)
// MC_PRE_PLAYER_GIVE_BIRTH_IMMACULATE (1475)
bool __stdcall RunPrePlayerGiveBirthCallback(Entity_Player* player, const uint32_t flag, const bool isCambion) {
	const int callbackid = isCambion ? 1474 : 1475;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(callbackid)
			.push(flag)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(flag)
			.call(1);

		if (!results && lua_isboolean(L, -1) && (bool)lua_toboolean(L, -1) == false) {
			return true;
		}
	}

	return false;
}

void PreBirthPatch(void* addr, const bool isCambion) {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(isCambion)
		.Push(ASMPatch::Registers::EAX)  // ConceptionFamiliarFlag
		.Push(ASMPatch::Registers::EDI)  // Entity_Player*
		.AddInternalCall(RunPrePlayerGiveBirthCallback)
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x19)  // Jump for true, negate birth
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0xA))  // Restore the 10 bytes we overwrote.
		.AddRelativeJump((char*)addr + 0xA);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchPrePlayerGiveBirth() {

	SigScan cambionSig("818f????????000200000bc2");
	cambionSig.Scan();
	void* cambionAddr = cambionSig.GetAddress();
	printf("[REPENTOGON] Patching Entity_Player::TakeDamage at %p for MC_PRE_PLAYER_GIVE_BIRTH_CAMBION\n", cambionAddr);
	PreBirthPatch(cambionAddr, true);  // Cambion

	SigScan immaculateSig("818f????????000200000bc6");
	immaculateSig.Scan();
	void* immaculateAddr = immaculateSig.GetAddress();
	printf("[REPENTOGON] Patching Entity_Player::TriggerHeartPickedUp at %p for MC_PRE_PLAYER_GIVE_BIRTH_IMMACULATE\n", immaculateAddr);
	PreBirthPatch(immaculateAddr, false);  // Immaculate

}

bool __stdcall RunPreTriggerBedSleepEffectCallback(Entity_Player* player) {
	const int callbackid = 1285;

		if (CallbackState.test(callbackid - 1000) && player) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);
			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
				.pushnil()
				.push(player, lua::Metatables::ENTITY_PLAYER)
				.call(1);


			if (!result) {
				if (lua_isboolean(L, -1)) {
					return (bool)lua_toboolean(L, -1);
				}
			}
		}

	return false;
}

void ASMPatchPreTriggerBedSleepEffect() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;

	SigScan scanner_transition("8b0883b9????????28");
	scanner_transition.Scan();
	void* addr = scanner_transition.GetAddress();
	printf("[REPENTOGON] Patching ItemOverlay::Update at %p for PreTriggerBedSleepEffect callback\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX) // Player
		.AddInternalCall(RunPreTriggerBedSleepEffectCallback)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x6B) // Skipping hearts gain
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x9))  // Restore instructions that we overwrote
		.AddRelativeJump((char*)addr + 0x9);
	sASMPatcher.PatchAt(addr, &patch);
}

void __stdcall RunPostTriggerBedSleepEffectCallback(Entity_Player* player) {
	const int callbackid = 1286;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.call(1);

	}
}

void ASMPatchPostTriggerBedSleepEffect() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;

	SigScan scanner_transition("8b35????????4381c6a8ba01008b46??2b06c1f8023bd80f82????????8b0d");
	scanner_transition.Scan();
	void* addr = scanner_transition.GetAddress();
	printf("[REPENTOGON] Patching ItemOverlay::Update at %p for PostBedSleep callback\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDX) // Player
		.AddInternalCall(RunPostTriggerBedSleepEffectCallback)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall RunPreBedSleepCallback(Entity_Player* player, Entity_Pickup* bed) {
	const int callbackid = 1288;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(bed, lua::Metatables::ENTITY_PICKUP)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(bed, lua::Metatables::ENTITY_PICKUP)
			.call(1);


		if (!result) {
			if (lua_isboolean(L, -1)) {
				return (bool)lua_toboolean(L, -1);
			}
		}
	}

	return false;

}

void ASMPatchPreBedSleep() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;

	SigScan scanner_transition("8bcae8????????83f80174??83f80274??8b8a????????8b82????????8d04");
	scanner_transition.Scan();
	void* addr = scanner_transition.GetAddress();
	printf("[REPENTOGON] Patching Entity_Pickup::handle_collision at %p for PreBedSleep callback\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI) // Bed
		.Push(ASMPatch::Registers::EDX) // Player
		.AddInternalCall(RunPreBedSleepCallback)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x4EA) // Skipping player's hearts check
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x2))  // Restore mov ecx, edi
		.AddInternalCall(((char*)addr + 0x7) + *(ptrdiff_t*)((char*)addr + 0x3)) // restore the function call
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesBedCallbacks() {
	ASMPatchPreTriggerBedSleepEffect();
	//ASMPatchPostTriggerBedSleepEffect();
	ASMPatchPreBedSleep();
}

//MC_PRE_PLAYER_POCKET_ITEM_SWAP(1287)
bool __stdcall RunPrePlayerPocketItemSwapCallback(Entity_Player* player) {
	const int callbackid = 1287;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.call(1);


		if (!result) {
			if (lua_isboolean(L, -1)) {
				return (bool)lua_toboolean(L, -1);
			}
		}
	}

	return false;
}

void ASMPatchPrePlayerPocketItemSwap() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;

	SigScan scanner_transition("8d9f????????833b00");
	scanner_transition.Scan();
	void* addr = scanner_transition.GetAddress();
	printf("[REPENTOGON] Patching Entity_Player::control_drop_pocket_items at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // Player
		.AddInternalCall(RunPrePlayerPocketItemSwapCallback)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0xC5) // Skipping pocket item swap
		.AddBytes(ByteBuffer().AddAny((char*)addr, 6))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 6);
	sASMPatcher.PatchAt(addr, &patch);
}

void __stdcall RunMainMenuRenderCallback(MenuManager* menuManager) {
	const int callbackid = 1023;
	MainMenuInputBlock::_enabled = false;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid).call(1);
	}
	MainMenuInputBlock::_enabled = true;
	if ((menuManager->_state | (1 << 8)) == menuManager->_state) {
		MainMenuInputBlock::_enabled = false;	//kill off if we are in the game transition
	};
}

void ASMPatchMainMenuCallback() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;

	SigScan scanner_transition("8b86????????f30f101d????????83f801");
	scanner_transition.Scan();
	void* addr = scanner_transition.GetAddress();
	printf("[REPENTOGON] Patching MenuManager::Render at %p for MainMenuRender callback\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI) // MenuManager
		.AddInternalCall(RunMainMenuRenderCallback)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

// Historically, MC_PRE_MOD_UNLOAD also runs during game shutdown, when the LuaEngine is being destroyed.
// However, this happens late into the shutdown process, after Game and Manager are already destroyed,
// so any code running on this callback at this time is very likely to crash the game.
// Here we trigger MC_PRE_MOD_UNLOAD earlier in the shutdown process (after the game has saved, but
// before stuff starts getting destroyed) as well as pass a boolean indicating the ongoing shutdown.
// Separately we've also ensured that the default MC_PRE_UNLOAD trigger (_UnloadMod) does not run
// the callback during shutdown.
void __stdcall RunPreModUnloadCallbacks() {
	const int callbackid = 73;  // MC_PRE_MOD_UNLOAD
	if (VanillaCallbackState.test(callbackid)) {
		for (const ModReference& mod : g_Mods) {
			lua_State* L = g_LuaEngine->_state;
			lua::LuaStackProtector protector(L);

			lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

			lua::LuaCaller(L).push(callbackid)
				.pushnil()
				.pushluaref(mod._luaTableRef->_ref)
				.push(true)
				.call(1);
		}
	}
}
void ASMPatchPreModUnloadCallbackDuringShutdown() {
	SigScan scanner("b9????????e8????????8b35????????85f674??8bcee8");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching earlier MC_PRE_MOD_UNLOAD in Isaac::Shutdown at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(RunPreModUnloadCallbacks)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the push we overwrote
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

void __stdcall RunPostRoomRenderEntitiesCallback() {
	const int callbackid = 1044;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(callbackid)
			.pushnil()
			.call(1);
	}
}
void ASMPatchPostRoomRenderEntitiesCallback() {
	SigScan scanner("33f68d87????????39b0????????76??8bbd????????6690");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching MC_POST_ROOM_RENDER_ENTITIES into Room::Render at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(RunPostRoomRenderEntitiesCallback)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x8))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}

// MC_PRE_ITEM_TEXT_DISPLAY
bool RunPreItemTextDisplayCallback(const char* title, const char* subtitle, bool isSticky, bool isCurseDisplay) {
	const int callbackId = 1484;
	if (CallbackState.test(callbackId - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackId)
			.pushnil()
			.push(title)
			.push(subtitle)
			.push(isSticky)
			.push(isCurseDisplay)
			.call(1);

		if (!result && lua_isboolean(L, -1)) {
			return (bool)lua_toboolean(L, -1);
		}
	}
	return true;
}
bool RunPreItemTextDisplayCallbackUTF16(wchar_t* title, wchar_t* subtitle, bool isSticky, bool isCurseDisplay) {
	int sizeNeededTitle = WideCharToMultiByte(CP_UTF8, 0, &title[0], wcslen(title), NULL, 0, NULL, NULL);
	std::string strTitle(sizeNeededTitle, 0);
	WideCharToMultiByte(CP_UTF8, 0, title, wcslen(title), &strTitle[0], sizeNeededTitle, NULL, NULL);

	int sizeNeededSubtitle = WideCharToMultiByte(CP_UTF8, 0, &subtitle[0], wcslen(subtitle), NULL, 0, NULL, NULL);
	std::string strSubtitle(sizeNeededSubtitle, 0);
	WideCharToMultiByte(CP_UTF8, 0, subtitle, wcslen(subtitle), &strSubtitle[0], sizeNeededSubtitle, NULL, NULL);

	return RunPreItemTextDisplayCallback(strTitle.c_str(), strSubtitle.c_str(), isSticky, isCurseDisplay);
}

// Only called for the stage name popup, and cards/pills/etc in co-op (as of Rep+ v1.9.7.10).
// Seems to be some inlining involved in other cases.
HOOK_METHOD(HUD_Message, Show, (const char* title, const char* subtitle, bool autoHide, bool isCurseDisplay) -> void) {
	const bool isSticky = !autoHide;  // What was once the "isSticky" boolean now has the opposite meaning in REP+
	if (RunPreItemTextDisplayCallback(title, subtitle, isSticky, isCurseDisplay)) {
		super(title, subtitle, autoHide, isCurseDisplay);
	}
}

// Called for custom lua text, transformations, and cards/pills outside of co-op.
HOOK_METHOD(HUD, ShowStackedItemTextCustomUTF8, (char* title, char* subtitle, bool unused, bool isCurseDisplay) -> void) {
	if (RunPreItemTextDisplayCallback(title, subtitle, false, isCurseDisplay)) {
		super(title, subtitle, unused, isCurseDisplay);
	}
}

// Called from HUD::ShowItemText outside co-op, and in Room::Init (for miniboss names?).
HOOK_METHOD(HUD, ShowStackedItemTextCustomUTF16, (wchar_t* title, wchar_t* subtitle, bool unused1, bool unused2) -> void) {
	if (RunPreItemTextDisplayCallbackUTF16(title, subtitle, false, false)) {
		super(title, subtitle, unused1, unused2);
	}
}

// Patch to trigger the callback for HUD::ShowItemText in co-op, as it seems to use some inlined code.
bool __stdcall PreItemTextDisplayTrampoline(HUD_Message* message, wchar_t* title, wchar_t* subtitle) {
	if (RunPreItemTextDisplayCallbackUTF16(title, subtitle, false, false)) {
		message->text_out();  // Function call overridden by the patch.
		return true;
	}
	return false;
}
void ASMPatchPreItemTextDisplayCallback() {
	SigScan patchScanner("e8????????6a00ffb5????????8bcfe8????????85f6");
	patchScanner.Scan();
	void* patchAddr = patchScanner.GetAddress();

	SigScan cancelScanner("ffb5????????e8????????83c40456");
	if (!cancelScanner.Scan()) {
		ZHL::Log("[ERROR] Unable to find signature for MC_PRE_ITEM_TEXT_DISPLAY patch's cancel jump\n");
		return;
	}
	void* cancelAddr = cancelScanner.GetAddress();

	printf("[REPENTOGON] Patching MC_PRE_ITEM_TEXT_DISPLAY into HUD::ShowItemText at %p\n", patchAddr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI)  // (subtitle)
		.AddBytes(ByteBuffer().AddAny((char*)patchAddr + 0x7, 0x6))  // PUSH dword ptr [EBP + ?] (title)
		.Push(ASMPatch::Registers::ECX)  // HUD_Message
		.AddInternalCall(PreItemTextDisplayTrampoline)
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, cancelAddr)  // Jump for false (cancel the text)
		.AddRelativeJump((char*)patchAddr + 0x5);  // Jump for true (continue normally)
	sASMPatcher.PatchAt(patchAddr, &patch);
}

static bool _hideActiveItemImage = false;
static bool _hideActiveItemOutline = false;
static bool _hideActiveItemChargeBar = false;

static const ColorMod activeItemOutlineColor(0, 0, 0, 1, 1, 1, 1);

HOOK_METHOD(PlayerHUDActiveItem, RenderGfx, (SourceQuad* source, DestinationQuad* dest, const ColorMod& color) -> void) {
	if (_hideActiveItemImage) return;
	if (_hideActiveItemOutline && color == activeItemOutlineColor) {
		return;
	}
	super(source, dest, color);
}

bool __stdcall ShouldHideChargebar() {
	return _hideActiveItemChargeBar;
}

// PRE/POST_PLAYERHUD_RENDER_ACTIVE_ITEM (1119/1079)
HOOK_METHOD(PlayerHUD, RenderActiveItem, (unsigned int activeSlot, const Vector& pos, int playerHudLayout, float size, float alpha, bool unused) -> void) {
	const bool isSchoolbagSlot = (activeSlot == 1);

	// If the slot is ActiveSlot.SLOT_SECONDARY (schoolbag), halve the size/scale.
	// The game does this inside RenderActiveItem.
	// Size adjustments for pocket slots are already accounted for.
	float actualSize = size;
	if (isSchoolbagSlot) {
		actualSize *= 0.5;
	}

	// The positions we send through the callback may be modified slightly to be more accurate to where the game actually rendered stuff.
	Vector itemPos = pos;
	Vector chargeBarPos = pos;

	// Player 1's esau gets different charge bar offsets.
	const bool playerOneEsau = this->_playerHudIndex == 4 && playerHudLayout == 1;

	if (!playerOneEsau) {
		chargeBarPos.x += (isSchoolbagSlot ? -2 : 34) * actualSize;
	}
	else if (isSchoolbagSlot) {
		chargeBarPos.x += 38 * actualSize;
	}
	chargeBarPos.y += 17 * actualSize;

	if (this->_activeItem[activeSlot].bookImage != nullptr) {
		// A book sprite was rendered under the item (Book of Virtues or Judas' Birthright).
		// Update the offsets we're sending through the callbacks to match where the corresponding sprites were actually rendered.
		itemPos.y -= 4;
		chargeBarPos.y += 3;
	}

	const int activeItemID = this->GetPlayer()->GetActiveItem(activeSlot);

	const int precallbackid = 1119;
	if (CallbackState.test(precallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaResults result = lua::LuaCaller(L).push(precallbackid)
			.push(activeItemID)
			.push(this->GetPlayer(), lua::Metatables::ENTITY_PLAYER)
			.push(activeSlot)
			.pushUserdataValue(itemPos, lua::Metatables::VECTOR)
			.push(alpha)
			.push(actualSize)
			.pushUserdataValue(chargeBarPos, lua::Metatables::VECTOR)
			.call(1);

		if (!result) {
			if (lua_isboolean(L, -1) && (bool)lua_toboolean(L, -1) == true) {
				return;
			} else if (lua_istable(L, -1)) {
				lua_pushnil(L);
				while (lua_next(L, -2) != 0) {
					if (lua_isstring(L, -2)) {
						const std::string key = lua_tostring(L, -2);
						if (key == "HideItem" && lua_isboolean(L, -1)) {
							_hideActiveItemImage = (bool)lua_toboolean(L, -1);
						} else if (key == "HideOutline" && lua_isboolean(L, -1)) {
							_hideActiveItemOutline = (bool)lua_toboolean(L, -1);
						} else if (key == "HideChargeBar" && lua_isboolean(L, -1)) {
							_hideActiveItemChargeBar = (bool)lua_toboolean(L, -1);
						}
					}
					lua_pop(L, 1);
				}
			}
		}
	}

	super(activeSlot, pos, playerHudLayout, size, alpha, unused);

	_hideActiveItemImage = false;
	_hideActiveItemOutline = false;
	_hideActiveItemChargeBar = false;

	const int postcallbackid = 1079;
	if (CallbackState.test(postcallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
		lua::LuaCaller(L).push(postcallbackid)
			.push(activeItemID)
			.push(this->GetPlayer(), lua::Metatables::ENTITY_PLAYER)
			.push(activeSlot)
			.pushUserdataValue(itemPos, lua::Metatables::VECTOR)
			.push(alpha)
			.push(actualSize)
			.pushUserdataValue(chargeBarPos, lua::Metatables::VECTOR)
			.call(1);
	}
}

void ASMPatchHideChargeBar() {
	SigScan scanner("8b55??85d20f8e????????8b4d");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching PlayerHUD:RenderActiveItem for hiding charge bars at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x3))  // mov EDX, dword ptr [EBP + ?] (active item max charge)
		.AddBytes("\x31\xC9")  // xor ecx,ecx
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(ShouldHideChargebar)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x0F\x45\xD1")  // cmovnz edx,ecx
		.AddBytes("\x85\xD2")  // test edx, edx
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// MC_POST_BACKWARDS_ROOM_RESTORE (1308)
static std::vector<int> s_BackwardsStageStack;

static inline int calc_stage_from_backwards_stage(BackwardsStageDesc* backwardsStage)
{
	constexpr size_t structSize = sizeof(BackwardsStageDesc);

	BackwardsStageDesc* backwardsStagesPtr = std::begin(g_Game->_backwardsStages);
	assert(backwardsStagesPtr <= backwardsStage && backwardsStage <= std::end(g_Game->_backwardsStages)); // Check that the pointer is within the _backwardsStages array

	size_t offset = (uintptr_t)(backwardsStage) - (uintptr_t)backwardsStagesPtr;
	assert(offset % structSize == 0); // Check that the BackwardsStageDesc struct is alligned

	int stage = offset / structSize;
	return stage + 1; // backwards stages start from 1
}

HOOK_METHOD(Level, place_rooms_backwards, (LevelGenerator* levelGen, BackwardsStageDesc* backwardsStage, uint32_t stageId, uint32_t minDifficulty, uint32_t maxDifficulty) -> bool)
{
	int stage = calc_stage_from_backwards_stage(backwardsStage);
	s_BackwardsStageStack.push_back(stage);
	bool success = super(levelGen, backwardsStage, stageId, minDifficulty, maxDifficulty);
	s_BackwardsStageStack.pop_back();
	return success;
}

static inline void run_post_backwards_room_restore(int stage, RoomDescriptor& roomDesc, const char* key, int index)
{
	constexpr int callbackId = 1308;
	if (!CallbackState.test(callbackId - 1000)) {
		return;
	}

	lua_State* L = g_LuaEngine->_state;
	lua::LuaStackProtector protector(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

	lua::LuaResults result = lua::LuaCaller(L).push(callbackId)
		.pushnil()
		.push(stage)
		.push(&roomDesc, lua::Metatables::ROOM_DESCRIPTOR)
		.pushfstring("%s_%d", key, index)
		.call(1);
}

static void __stdcall RunPostBackwardsBossRestore(int roomIndex)
{
	roomIndex++; // increase room index
	int stage = s_BackwardsStageStack.back();
	RoomDescriptor& roomDesc = g_Game->_gridRooms[g_Game->_nbRooms - 1];
	run_post_backwards_room_restore(stage, roomDesc, "boss", roomIndex);
}

static void __stdcall RunPostBackwardsTreasureRestore(int roomIndex)
{
	int stage = s_BackwardsStageStack.back();
	RoomDescriptor& roomDesc = g_Game->_gridRooms[g_Game->_nbRooms - 1];
	run_post_backwards_room_restore(stage, roomDesc, "treasure", roomIndex);
}

static void patch_post_backwards_boss_restore(const char* signature, const char* location, const char* callbackName)
{
	void* address = get_sig_address(signature, location, callbackName);
	ZHL::Log("[REPENTOGON] Patching %s at %p\n", location, address);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX) // roomIndex (post decrease)
		.AddInternalCall(RunPostBackwardsBossRestore)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)address, 0x6)) // restore
		.AddRelativeJump((char*)address + 0x6); // resume
	sASMPatcher.PatchAt(address, &patch);
}

static void patch_post_backwards_treasure_restore(const char* signature, const char* location, const char* callbackName)
{
	void* address = get_sig_address(signature, location, callbackName);
	ZHL::Log("[REPENTOGON] Patching %s at %p\n", location, address);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX) // roomIndex
		.AddInternalCall(RunPostBackwardsTreasureRestore)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)address, 0x7)) // restore
		.AddRelativeJump((char*)address + 0x7); // resume
	sASMPatcher.PatchAt(address, &patch);
}

void ASMPatchPostBackwardsRoomRestore()
{
	patch_post_backwards_boss_restore("894d??8955??85c90f89", "Level::place_rooms_backwards (Post Boss Room Restore)", "MC_POST_BACKWARDS_ROOM_RESTORE");
	patch_post_backwards_treasure_restore("4883ea0c", "Level::place_rooms_backwards (Post Treasure Room Restore)", "MC_POST_BACKWARDS_ROOM_RESTORE");
}