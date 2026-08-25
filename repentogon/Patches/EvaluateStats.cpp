#include "EvaluateStats.h"

#include "IsaacRepentance.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "ASMPatches.h"
#include "EntityPlus.h"
#include "SigScan.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "ItemConfigEx.h"
#include "EntityConfigEx.h"
#include "../LuaInit.h"

namespace EvaluateStats {

static void ApplyPlayerStatModifier(const int playerType, const PlayerStat stat, float& value) {
	if (const EntityConfigEx::PlayerEx* ex = EntityConfigEx::GetPlayerEx(playerType); ex && ex->HasStatModifier(stat)) {
		float modifier = ex->GetStatModifier(stat);
		if (stat == PlayerStat::TEARS_MULTIPLIER || stat == PlayerStat::DAMAGE_MULTIPLIER) {
			if (modifier > 0) {
				value *= modifier;
			}
		} else {
			if (stat == PlayerStat::RANGE_MODIFIER) {
				modifier *= 40;
			}
			value += modifier;
		}
	}
}

// MC_EVALUATE_STAT
float RunEvaluateStatCallback(Entity_Player* player, const float currentStatValue, const LuaEvaluateStatStage evalStatStage, const bool positiveOnly) {
	const int callbackid = 1226;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push((int)evalStatStage)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push((int)evalStatStage)
			.push(currentStatValue)
			.call(1);

		if (!result && lua_isnumber(L, -1)) {
			const float newStatValue = (float)lua_tonumber(L, -1);
			if (!positiveOnly || newStatValue > 0) {
				return newStatValue;
			}
		}
	}

	return currentStatValue;
}


// After all basic tears ups (sad onion etc), before Math and the tears cap are applied.
// The float tears value at this point is purely the "tears up" value, so its 0 if the player has no items.
void __stdcall TearsUpHook(Entity_Player* player, float* tears) {
	*tears += ItemConfigEx::CalculateStatChange(player, ItemStat::TEARS_UP);
	*tears = RunEvaluateStatCallback(player, *tears, LuaEvaluateStatStage::TEARS_UP, false);

	// Write new tears value to xmm1 and multiply by 1.3 (the multiplication is overridden by the patch)
	float toXmm1 = (*tears) * 1.3f;
	__asm {
		movd xmm1, toXmm1
	}
}
void PatchTearsUp() {
	void* baseAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EvaluateItems_TearsUp);
	void* ebpOffsetAddr = (char*)baseAddr + 0x4;
	void* patchAddr = (char*)baseAddr + 0x8;

	printf("[REPENTOGON] Patching EvaluateItems for base tears at %p\n", patchAddr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | (ASMPatch::SavedRegisters::Registers::XMM_REGISTERS & ~ASMPatch::SavedRegisters::Registers::XMM1), true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\x8D\x9D").AddBytes(ByteBuffer().AddAny((char*)ebpOffsetAddr, 0x4))  // lea ebx, [ebp+?]
		.Push(ASMPatch::Registers::EBX)  // float* tears
		.Push(ASMPatch::Registers::EAX)  // Entity_Player
		.AddInternalCall(TearsUpHook)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)patchAddr + 0x8);
	sASMPatcher.PatchAt(patchAddr, &patch);
}


// This is where the flat modifier to the player's base tears is applied (Keeper's -1.9, Jacob's +0.277, Eden's random modifier, etc).
void __stdcall PlayerTearsModifierHook(int playerType, float* tears) {
	ApplyPlayerStatModifier(playerType, PlayerStat::TEARS_MODIFIER, *tears);
}
void PatchPlayerTearsModifier() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EvaluateItems_PlayerTears);

	const int ebpOffset = *(int*)((char*)addr + 0x4);

	printf("[REPENTOGON] Patching EvaluateItems for player tears modifier at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x8))  // Restore overridden bytes
		.PreserveRegisters(savedRegisters)
		.LoadEffectiveAddress(ASMPatch::Registers::EBP, ebpOffset, ASMPatch::Registers::EAX)
		.Push(ASMPatch::Registers::EAX)
		.Push(ASMPatch::Registers::EDX)
		.AddInternalCall(PlayerTearsModifierHook)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}


// After the tears cap and other math is applied, but before multipliers.
void __stdcall FlatTearsHook(Entity_Player* player, float* tears) {
	*tears += ItemConfigEx::CalculateStatChange(player, ItemStat::FLAT_TEARS);
	*tears = RunEvaluateStatCallback(player, *tears, LuaEvaluateStatStage::FLAT_TEARS, true);
	ApplyPlayerStatModifier(player->GetPlayerType(), PlayerStat::TEARS_MULTIPLIER, *tears);
	*tears *= ItemConfigEx::CalculateStatMult(player, ItemStat::TEARS_MULT);
}
void PatchFlatTears() {
	void* baseAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EvaluateItems_FlatTears);
	void* ebpOffsetAddr = (char*)baseAddr + 0x4;
	void* patchAddr = (char*)baseAddr + 0x8;

	printf("[REPENTOGON] Patching EvaluateItems for flat tears & tear multipliers at %p\n", patchAddr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\x8D\x9D").AddBytes(ByteBuffer().AddAny((char*)ebpOffsetAddr, 0x4))  // lea ebx, [ebp+?]
		.Push(ASMPatch::Registers::EBX)  // float* tears
		.Push(ASMPatch::Registers::ESI)  // Entity_Player*
		.AddInternalCall(FlatTearsHook)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)patchAddr, 0x7))
		.AddRelativeJump((char*)patchAddr + 0x7);
	sASMPatcher.PatchAt(patchAddr, &patch);
}


// After (most) standard damage ups, right before Odd Mushroom (Thin)'s 0.9x multiplier.
void __stdcall DamageUpHook(Entity_Player* player, float* damage) {
	*damage += ItemConfigEx::CalculateStatChange(player, ItemStat::DAMAGE_UP);
	*damage = RunEvaluateStatCallback(player, *damage, LuaEvaluateStatStage::DAMAGE_UP, false);
}
void PatchDamageUp() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EvaluateItems_DamageUp);

	printf("[REPENTOGON] Patching EvaluateItems for base damage at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // mov ECX,dword ptr [EBP+?] (player)
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // float* damage
		.Push(ASMPatch::Registers::ECX)  // Entity_Player*
		.AddInternalCall(DamageUpHook)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}


// After the "Damage Up" calculations and the "early" multipliers from Polyphemus and the character multiplier.
// Before all flat modifiers, including the character damage modifier.
void __stdcall PreFlatDamageHook(Entity_Player* player, float* damage) {
	const int playerType = player->GetPlayerType();
	if (!player->_temporaryeffects.HasTrinketEffect(TRINKET_AZAZELS_STUMP)) {
		ApplyPlayerStatModifier(player->GetPlayerType(), PlayerStat::DAMAGE_MULTIPLIER, *damage);
	}
	*damage = RunEvaluateStatCallback(player, *damage, LuaEvaluateStatStage::PRE_FLAT_DAMAGE, false);
	ApplyPlayerStatModifier(player->GetPlayerType(), PlayerStat::DAMAGE_MODIFIER, *damage);
}
void PatchPreFlatDamage() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EvaluateItems_PlayerDamage);

	printf("[REPENTOGON] Patching EvaluateItems for early damage multiplier & player damage modifiers at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore overridden bytes
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI)
		.Push(ASMPatch::Registers::EAX)
		.AddInternalCall(PreFlatDamageHook)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}


// After most of the flat damage modifiers, right before (most) multipliers are applied (specifically Brimstone, Crown of Light, Sacred Heart, etc).
// At this point, the game is manipulating the player's damage field directly.
void __stdcall FlatDamageHook(Entity_Player* player) {
	player->_damage += ItemConfigEx::CalculateStatChange(player, ItemStat::FLAT_DAMAGE);
	player->_damage = RunEvaluateStatCallback(player, player->_damage, LuaEvaluateStatStage::FLAT_DAMAGE, false);
	player->_damage *= ItemConfigEx::CalculateStatMult(player, ItemStat::DAMAGE_MULT);
}
void PatchFlatDamage() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EvaluateItems_FlatDamage);

	printf("[REPENTOGON] Patching EvaluateItems for flat damage at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI)  // Entity_Player*
		.AddInternalCall(FlatDamageHook)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x7))  // Restore overridden bytes
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}


// After (most) standard range ups, right before Number One (since it applies a 0.8x multiplier).
void __stdcall RangeUpHook(Entity_Player* player) {
	ApplyPlayerStatModifier(player->GetPlayerType(), PlayerStat::RANGE_MODIFIER, player->_tearrange);
	player->_tearrange += ItemConfigEx::CalculateStatChange(player, ItemStat::RANGE_UP);
}
void PatchRangeUp() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EvaluateItems_RangeUp);

	printf("[REPENTOGON] Patching EvaluateItems for range at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Entity_Player*
		.AddInternalCall(RangeUpHook)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore overridden bytes
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}


// Before the D8 movespeed multiplier
void __stdcall SpeedUpHook(Entity_Player* player) {
	ApplyPlayerStatModifier(player->GetPlayerType(), PlayerStat::SPEED_MODIFIER, player->_movespeed);
	player->_movespeed += ItemConfigEx::CalculateStatChange(player, ItemStat::SPEED_UP);
}
void PatchSpeedUp() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EvaluateItems_SpeedUp);

	printf("[REPENTOGON] Patching EvaluateItems for move speed at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Entity_Player*
		.AddInternalCall(SpeedUpHook)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x8))  // Restore overridden bytes
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}


// Before the My Reflection shotspeed multiplier
void __stdcall ShotSpeedUpHook(Entity_Player* player) {
	ApplyPlayerStatModifier(player->GetPlayerType(), PlayerStat::SHOTSPEED_MODIFIER, player->_shotspeed);
	player->_shotspeed += ItemConfigEx::CalculateStatChange(player, ItemStat::SHOTSPEED_UP);
}
void PatchShotSpeedUp() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EvaluateItems_ShotSpeedUp);

	printf("[REPENTOGON] Patching EvaluateItems for shot speed at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x8))  // Restore overridden bytes
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX)  // Entity_Player*
		.AddInternalCall(ShotSpeedUpHook)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}


// Luck is a simple stat, we can just handle it here.
HOOK_METHOD(LuaEngine, EvaluateItems, (Entity_Player* player, int cacheFlag) -> void) {
	if (cacheFlag & CACHE_LUCK) {
		ApplyPlayerStatModifier(player->GetPlayerType(), PlayerStat::LUCK_MODIFIER, player->_luck);
		player->_luck += ItemConfigEx::CalculateStatChange(player, ItemStat::LUCK_UP);
	}
	super(player, cacheFlag);
}


void ApplyASMPatches() {
	PatchPlayerTearsModifier();
	PatchTearsUp();
	PatchFlatTears();
	PatchDamageUp();
	PatchPreFlatDamage();
	PatchFlatDamage();
	PatchRangeUp();
	PatchSpeedUp();
	PatchShotSpeedUp();
}

}  // namespace EvaluateStats
