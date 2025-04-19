#include "EvaluateStats.h"

#include "IsaacRepentance.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "ASMPatches.h"
#include "EntityPlus.h"
#include "SigScan.h"
#include "ASMPatcher.hpp"
#include "../LuaInit.h"

namespace EvaluateStats {

void CheckXmlItemStatsCacheFlags(XMLItem* xmlData, const bool checkEffectStats) {
	for (const auto& [stat, itemStats] : checkEffectStats ? xmlData->effectstatups : xmlData->statups) {
		for (const auto& [id, statChange] : itemStats) {
			if (statChange != 0) {
				ItemConfig_Item* item;
				if (xmlData == XMLStuff.NullItemData) {
					item = g_Manager->GetItemConfig()->GetNullItem(id);
				} else if (xmlData == XMLStuff.TrinketData) {
					item = g_Manager->GetItemConfig()->GetTrinket(id);
				} else {
					item = g_Manager->GetItemConfig()->GetCollectible(id);
					if (!checkEffectStats && item && item->type == 3) {
						item->passiveCache = true;
					}
				}
				if (item && evaluteStatCacheFlags.find(stat) != evaluteStatCacheFlags.end()) {
					item->cacheFlags |= evaluteStatCacheFlags.at(stat);
				}
			}
		}
	}
}

// Scans the XMLData for xml-defined stats and, if present, updates the CacheFlags of the item appropriately.
void UpdateItemConfig() {
	CheckXmlItemStatsCacheFlags(XMLStuff.ItemData, true);
	CheckXmlItemStatsCacheFlags(XMLStuff.ItemData, false);
	CheckXmlItemStatsCacheFlags(XMLStuff.TrinketData, true);
	CheckXmlItemStatsCacheFlags(XMLStuff.TrinketData, false);
	CheckXmlItemStatsCacheFlags(XMLStuff.NullItemData, true);
}

// Wee
float CalculateStatChange(Entity_Player* player, const EvaluateStatStage stat) {
	TemporaryEffects* effects = &player->_temporaryeffects;
	float totalChange = 0;
	for (const auto& [id, change] : XMLStuff.ItemData->statups[stat]) {
		totalChange += player->GetCollectibleNum(id, false) * change;
	}
	for (const auto& [id, change] : XMLStuff.ItemData->effectstatups[stat]) {
		totalChange += effects->GetCollectibleEffectNum(id) * change;
	}
	for (const auto& [id, change] : XMLStuff.TrinketData->statups[stat]) {
		totalChange += player->GetTrinketMultiplier(id) * change;
	}
	for (const auto& [id, change] : XMLStuff.TrinketData->effectstatups[stat]) {
		totalChange += effects->GetTrinketEffectNum(id) * change;
	}
	for (const auto& [id, change] : XMLStuff.NullItemData->effectstatups[stat]) {
		totalChange += effects->GetNullEffectNum(id) * change;
	}
	return totalChange;
}

// MC_EVALUATE_STAT
float RunEvaluateStatCallback(Entity_Player* player, const float currentStatValue, const EvaluateStatStage stat) {
	const int callbackid = 1226;

	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push((int)stat)
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push((int)stat)
			.push(currentStatValue)
			.call(1);

		if (!result && lua_isnumber(L, -1)) {
			const float newStatValue = (float)lua_tonumber(L, -1);
			if (newStatValue > 0) {
				return newStatValue;
			}
		}
	}

	return currentStatValue;
}


// After all basic tears ups (sad onion etc), before Math and the tears cap are applied.
// The float tears value at this point is purely the "tears up" value, so its 0 if the player has no items.
void __stdcall TearsUpHook(Entity_Player* player, float* tears) {
	*tears += CalculateStatChange(player, STAT_TEARS_UP);
	*tears = RunEvaluateStatCallback(player, *tears, STAT_TEARS_UP);

	// Write new tears value to xmm1 and multiply by 1.3 (the multiplication is overridden by the patch)
	float toXmm1 = (*tears) * 1.3f;
	__asm {
		movd xmm1, toXmm1
	}
}
void PatchTearsUp() {
	SigScan scanner("f30f118d????????f30f590d????????8d95????????8d8d");
	scanner.Scan();
	void* baseAddr = scanner.GetAddress();
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


// After the tears cap and other math is applied, but before multipliers.
void __stdcall FlatTearsHook(Entity_Player* player, float* tears) {
	*tears += CalculateStatChange(player, STAT_FLAT_TEARS);
	*tears = RunEvaluateStatCallback(player, *tears, STAT_FLAT_TEARS);
}
void PatchFlatTears() {
	SigScan scanner("f30f1195????????6a0068f50000008bcee8????????84c00f85");
	scanner.Scan();
	void* baseAddr = scanner.GetAddress();
	void* ebpOffsetAddr = (char*)baseAddr + 0x4;
	void* patchAddr = (char*)baseAddr + 0x8;

	printf("[REPENTOGON] Patching EvaluateItems for flat tears at %p\n", patchAddr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\x8D\x9D").AddBytes(ByteBuffer().AddAny((char*)ebpOffsetAddr, 0x4))  // lea ebx, [ebp+?]
		.Push(ASMPatch::Registers::EBX)  // float* tears
		.Push(ASMPatch::Registers::ESI)  // Entity_Player*
		.AddInternalCall(FlatTearsHook)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)patchAddr + 0x5);
	sASMPatcher.PatchAt(patchAddr, &patch);
}


// After (most) standard damage ups, right before Odd Mushroom (Thin)'s 0.9x multiplier.
void __stdcall DamageUpHook(Entity_Player* player, float* damage) {
	*damage += CalculateStatChange(player, STAT_DAMAGE_UP);
	*damage = RunEvaluateStatCallback(player, *damage, STAT_DAMAGE_UP);
}
void PatchDamageUp() {
	SigScan scanner("8b8d????????6a006a78");
	scanner.Scan();
	void* addr = scanner.GetAddress();

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


// After diminishing returns are applied to damage, but before (most) multipliers.
// At this point, the game is manipulating the player's damage field directly.
void __stdcall FlatDamageHook(Entity_Player* player) {
	player->_damage += CalculateStatChange(player, STAT_FLAT_DAMAGE);
	player->_damage = RunEvaluateStatCallback(player, player->_damage, STAT_FLAT_DAMAGE);
}
void PatchFlatDamage() {
	SigScan scanner("6a00689f0100008bcee8????????84c074??689f010000");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching EvaluateItems for flat damage at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
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
	player->_tearrange += CalculateStatChange(player, STAT_RANGE_UP);
}
void PatchRangeUp() {
	SigScan scanner("6a006a068bcfe8????????84c075");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching EvaluateItems for range at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
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
	player->_movespeed += CalculateStatChange(player, STAT_SPEED_UP);
}
void PatchSpeedUp() {
	SigScan scanner("f30f1095????????f30f109f????????f30f5919");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching EvaluateItems for move speed at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
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
	player->_shotspeed += CalculateStatChange(player, STAT_SHOTSPEED_UP);
}
void PatchShotSpeedUp() {
	SigScan scanner("f30f5806f30f1106e8????????84c075??6a058bcf");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching EvaluateItems for shot speed at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
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
		player->_luck += CalculateStatChange(player, STAT_LUCK_UP);
	}
	super(player, cacheFlag);
}


void ApplyASMPatches() {
	PatchTearsUp();
	PatchFlatTears();
	PatchDamageUp();
	PatchFlatDamage();
	PatchRangeUp();
	PatchSpeedUp();
	PatchShotSpeedUp();
}

}  // namespace EvaluateStats
