#include "IsaacRepentance.h"

#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "HookSystem.h"
#include "../../LuaInit.h"
#include "Log.h"

// Refresh the HistoryHUD at the start of a new room if player has smelted Modeling Clay or Error trinkets.
// Note: Not needed anymore if we update to v1.9.7.13 or later.
HOOK_METHOD(Entity_Player, TriggerNewRoom_TemporaryEffects, () -> void) {
	super();

	if (this->_smeltedTrinkets[TRINKET_ERROR]._trinketNum > 0 || this->_smeltedTrinkets[TRINKET_ERROR]._goldenTrinketNum > 0
			|| this->_smeltedTrinkets[TRINKET_MODELING_CLAY]._trinketNum > 0 || this->_smeltedTrinkets[TRINKET_MODELING_CLAY]._goldenTrinketNum > 0) {
		HistoryHUD* historyHud = &g_Game->GetHUD()->_historyHUD;
		const int idx = historyHud->GetPlayerIdx(this);
		if (idx == 0 || idx == 1) {
			historyHud->Recompute(idx, false);
		}
	}
}

// Force refreshing the HistoryHUD_Item images for smelted Modeling Clay or Error trinkets on recompute.
// Note: Not needed anymore if we update to v1.9.7.13 or later.
bool __stdcall ShouldRefreshHistoryHudImage(int id, bool isTrinket) {
	if (isTrinket) {
		id = id & TRINKET_ID_MASK;
		return id == TRINKET_MODELING_CLAY || id == TRINKET_ERROR;
	}
	return false;
}
void ASMPatchHistoryHudRecomputeRefresh() {
	void* sigAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::HistoryHudRecomputeGfx_RefreshCheck);
	void* patchAddr = (char*)sigAddr + 0x6;
	void* jumpAddr = (char*)patchAddr + *(int*)((char*)sigAddr + 0x2);

	ZHL::Log("[REPENTOGON] Patching HistoryHUD::Recompute Image Refresh @ %p\n", patchAddr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::EDI, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBP, -0x1d)  // isTrinket
		.Push(ASMPatch::Registers::EBP, -0x1c)  // id
		.AddInternalCall(ShouldRefreshHistoryHudImage)
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, jumpAddr) // jump for true
		.AddBytes(ByteBuffer().AddAny((char*)patchAddr, 0x6))
		.AddRelativeJump((char*)patchAddr + 0x6);
	sASMPatcher.PatchAt(patchAddr, &patch);
}

// Allows the correct image to be rendered on the HistoryHUD for smelted Modeling Clay or Error trinkets.
// Note: Not needed anymore if we update to v1.9.7.13 or later.
ItemConfig_Item* GetModelingClayErrorItemConfig(Entity_Player* player, int id) {
	ItemConfig* itemConfig = g_Manager->GetItemConfig();
	if (id == TRINKET_MODELING_CLAY) {
		return itemConfig->GetCollectible(player->_modelingClayEffect);
	} else if (id == TRINKET_ERROR) {
		const int errorEffect = g_Game->GetCurrentRoomDesc()->GetErrorTrinketEffect();
		if (errorEffect == TRINKET_MODELING_CLAY) {
			if (player->_modelingClayEffect > 0) {
				return itemConfig->GetCollectible(player->_modelingClayEffect);
			} else {
				return itemConfig->GetTrinket(TRINKET_MODELING_CLAY);
			}
		} else {
			return itemConfig->GetTrinket(errorEffect);
		}
	}
	return nullptr;
}
ItemConfig_Item* __stdcall HistoryHudRecomputeTrampoline(Entity_Player* player, int id, bool isTrinket) {
	if (!isTrinket) {
		return g_Manager->GetItemConfig()->GetCollectible(id);
	}

	id = id & TRINKET_ID_MASK;
	if (ItemConfig_Item* item = GetModelingClayErrorItemConfig(player, id)) {
		return item;
	}
	return g_Manager->GetItemConfig()->GetTrinket(id);
}
void ASMPatchHistoryHudRecompute() {
	ASMPatchHistoryHudRecomputeRefresh();

	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::HistoryHudRecomputeGfx);
	void* jumpAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::HistoryHudRecomputeGfx_Jump);

	ZHL::Log("[REPENTOGON] Patching HistoryHUD::Recompute Image Selection @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::EDI, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // isTrinket
		.Push(ASMPatch::Registers::EDX)  // id
		.Push(ASMPatch::Registers::EBP, -0x24)  // Entity_Player* - Hard coding offsets doesn't really matter when this patch can probably just be removed if we ever update
		.AddInternalCall(HistoryHudRecomputeTrampoline)
		.CopyRegister(ASMPatch::Registers::EDI, ASMPatch::Registers::EAX)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump(jumpAddr);
	sASMPatcher.PatchAt(addr, &patch);
}

// MC_POST_HISTORYHUD_RECOMPUTE (1029)
HOOK_METHOD(HistoryHUD, Recompute, (int idx, bool immediate) -> void) {
	super(idx, immediate);

	const int callbackid = 1029;
	if (immediate && CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(callbackid)
			.push(idx)
			.push(this, lua::metatables::HistoryHUDMT)
			.push(idx)
			.call(1);
	}
}

std::set<int> collectiblesToHide;
std::set<int> trinketsToHide;

LUALIB_API void CheckPreHistoryHudRenderReturnTable(lua_State* L, const std::string& key, std::set<int>& out) {
	lua_pushstring(L, key.c_str());
	lua_gettable(L, -2);
	if (lua_istable(L, -1)) {
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			if (lua_isinteger(L, -2)) {
				out.insert((int)lua_tointeger(L, -2));
			}
			lua_pop(L, 1);
		}
	}
	lua_pop(L, 1);
}

// MC_PRE/POST_HISTORYHUD_RENDER (1027/1028)
HOOK_METHOD(HistoryHUD, Render, (Vector* pos, bool mini) -> void) {
	collectiblesToHide.clear();
	trinketsToHide.clear();

	const int precallbackid = 1027;
	if (CallbackState.test(precallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(precallbackid)
			.pushnil()
			.push(this, lua::metatables::HistoryHUDMT)
			.pushUserdataValue(*pos, lua::Metatables::VECTOR)
			.push(mini)
			.call(1);

		if (!results) {
			if (lua_istable(L, -1)) {
				CheckPreHistoryHudRenderReturnTable(L, "HideCollectibles", collectiblesToHide);
				CheckPreHistoryHudRenderReturnTable(L, "HideTrinkets", trinketsToHide);
			} else if (lua_isboolean(L, -1) && !lua_toboolean(L, -1)) {
				return;
			}
		}
	}

	super(pos, mini);

	const int postcallbackid = 1028;
	if (CallbackState.test(postcallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults results = lua::LuaCaller(L).push(postcallbackid)
			.pushnil()
			.push(this, lua::metatables::HistoryHUDMT)
			.pushUserdataValue(*pos, lua::Metatables::VECTOR)
			.push(mini)
			.call(1);
	}
}

// Weird function because I was having trouble finding a clean way to fetch the current historyHudPlayerIndex from my patch.
Entity_Player* FindPlayerForHistoryHudItem(HistoryHUD_Item* hudItem) {
	for (const HistoryHUD_Player& hudPlayer : g_Game->GetHUD()->_historyHUD._playerHistoryHuds) {
		if (!hudPlayer._historyHudItems.empty() && hudItem >= &hudPlayer._historyHudItems.front() && hudItem <= &hudPlayer._historyHudItems.back()) {
			return hudPlayer._player;
		}
	}
	return nullptr;
}

// Modify color/offsets for Modeling Clay, Error, and Monkey Paw.
// Note: Some of this may not be needed anymore if we update to v1.9.7.13 or later.
bool __stdcall RenderHistoryItemTrampoline(HistoryHUD_Item* hudItem, SourceQuad* sourceQuad, ColorMod* colormod, KColor* color) {
	const int id = hudItem->_historyItem._itemID & TRINKET_ID_MASK;
	if (hudItem->_historyItem._isTrinket) {
		if (Entity_Player* player = FindPlayerForHistoryHudItem(hudItem)) {
			if (player->_modelingClayEffect > 0 && (id == TRINKET_MODELING_CLAY || (id == TRINKET_ERROR && g_Game->GetCurrentRoomDesc()->GetErrorTrinketEffect() == TRINKET_MODELING_CLAY))) {
				// Modeling Clay colorize
				colormod->_colorize[0] = 0.5976471f;
				colormod->_colorize[1] = 0.33411765f;
				colormod->_colorize[2] = 0.31058824f;
				colormod->_colorize[3] = 1.f;
			}
			if (id == TRINKET_MONKEY_PAW && sourceQuad->_topLeft.x == 0) {
				// Monkey Paw cropoffset
				const float cropOffset = (3 - player->_monkeyPawCounter) * 32.f;
				sourceQuad->_topLeft.x += cropOffset;
				sourceQuad->_topRight.x += cropOffset;
				sourceQuad->_bottomLeft.x += cropOffset;
				sourceQuad->_bottomRight.x += cropOffset;
			}
		}
		if (id == TRINKET_ERROR && g_Game->GetCurrentRoomDesc()->GetErrorTrinketEffect() > 0) {
			// ERROR color
			color->_red = 0.5176471f;
			color->_green = 0.10196079f;
			color->_blue = 0.10196079f;
		}
		return trinketsToHide.find(id) != trinketsToHide.end();
	}
	return collectiblesToHide.find(id) != collectiblesToHide.end();
}
void ASMPatchRenderHistoryItem() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PreRenderHistoryItem);
	void* postAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PostRenderHistoryItem);
	void* stackOffsetsAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PreRenderHistoryItemStackOffsets);
	void* colorModOffsetAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PreRenderHistoryItemColorModOffset);

	printf("[REPENTOGON] Patching HistoryHUD:Render @ %p\n", addr);

	const int32_t kColorOffset = *(int32_t*)((char*)stackOffsetsAddr + 0x3);
	//const int8_t destQuadOffset = *(int8_t*)((char*)stackOffsetsAddr + 0xB) - 0x4;
	const int32_t srcQuadOffset = *(int32_t*)((char*)stackOffsetsAddr + 0x13) - 0x8;
	const int32_t colorModOffset = *(int32_t*)((char*)colorModOffsetAddr + 0x3) - 0xC;

	ASMPatch::SavedRegisters savedRegisters((ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::XMM_REGISTERS) & ~ASMPatch::SavedRegisters::EAX, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore overwritten bytes
		.CopyRegister(ASMPatch::Registers::EAX, ASMPatch::Registers::ESP)
		.PreserveRegisters(savedRegisters)
		.LoadEffectiveAddress(ASMPatch::Registers::EAX, kColorOffset, ASMPatch::Registers::EDX).Push(ASMPatch::Registers::EDX)  // KColor*
		.LoadEffectiveAddress(ASMPatch::Registers::EAX, colorModOffset, ASMPatch::Registers::EDX).Push(ASMPatch::Registers::EDX)  // ColorMod*
		//.LoadEffectiveAddress(ASMPatch::Registers::EAX, destQuadOffset, ASMPatch::Registers::EDX).Push(ASMPatch::Registers::EDX)  // DestinationQuad*
		.LoadEffectiveAddress(ASMPatch::Registers::EAX, srcQuadOffset, ASMPatch::Registers::EDX).Push(ASMPatch::Registers::EDX)  // SourceQuad*
		.Push(ASMPatch::Registers::ESI)  // HistoryHUD_Item*
		.AddInternalCall(RenderHistoryItemTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x84\xC0")  // TEST AL, AL
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, postAddr)  // Jump for true (skip render)
		.AddRelativeJump((char*)addr + 0x5);  // Jump for false (continue with render)
	sASMPatcher.PatchAt(addr, &patch);
}
