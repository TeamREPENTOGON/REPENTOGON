#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "HookSystem.h"

/* /////////////////////
// Grid Collision Callbacks
*/ /////////////////////

struct GridCollisionCallbackInfo {
	int precallbackid;
	int postcallbackid;
	lua::Metatables metatable;
};

const std::unordered_map<int, const GridCollisionCallbackInfo> gridCollisionCallbacks = {
	{1, { 1171, 1172, lua::Metatables::ENTITY_PLAYER }},
	{2, { 1173, 1174, lua::Metatables::ENTITY_TEAR }},
	{3, { 1175, 1176, lua::Metatables::ENTITY_FAMILIAR }},
	{4, { 1177, 1178, lua::Metatables::ENTITY_BOMB }},
	{5, { 1179, 1180, lua::Metatables::ENTITY_PICKUP }},
	{9, { 1181, 1182, lua::Metatables::ENTITY_PROJECTILE }},
};

// An entity may check the same grid index for collision multiple times per frame.
// Let's not fire the callbacks each time.
std::unordered_map<Entity*, std::unordered_map<int, bool>> cachedGridCollisionResult;

HOOK_METHOD(Game, Update, () -> void) {
	super();
	cachedGridCollisionResult.clear();
}

bool RunGridCollisionCallbacks(Entity* entity, const int gridIndex, const lua::Metatables metatable, const int param, const int precallbackid, const int postcallbackid) {
	GridEntity* gridEntity = g_Game->GetCurrentRoom()->GetGridEntity(gridIndex);

	// MC_PRE_X_GRID_COLLISION
	if (CallbackState.test(precallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(precallbackid)
			.push(param)
			.push(entity, metatable)
			.push(gridIndex)
			.push(gridEntity, lua::Metatables::GRID_ENTITY)
			.call(1);

		// why does lua_toboolean return an int wtf
		if (!result && lua_isboolean(L, -1) && (bool)lua_toboolean(L, -1) == true) {
			return true;
		}
	}

	// MC_X_GRID_COLLISION
	if (CallbackState.test(postcallbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(postcallbackid)
			.push(param)
			.push(entity, metatable)
			.push(gridIndex)
			.push(gridEntity, lua::Metatables::GRID_ENTITY)
			.call(0);
	}

	return false;
}

bool __stdcall TriggerGridCollisionCallbacks(Entity* entity, const int gridIndex) {
	if (cachedGridCollisionResult.count(entity) != 0 && cachedGridCollisionResult[entity].count(gridIndex) != 0) {
		return cachedGridCollisionResult[entity][gridIndex];
	}

	bool result = false;

	const int type = *entity->GetType();

	if (gridCollisionCallbacks.count(type) != 0) {
		const GridCollisionCallbackInfo info = gridCollisionCallbacks.at(type);
		result = RunGridCollisionCallbacks(entity, gridIndex, info.metatable, *entity->GetVariant(), info.precallbackid, info.postcallbackid);
	}
	else if (entity->ToNPC()) {
		result = RunGridCollisionCallbacks(entity, gridIndex, lua::Metatables::ENTITY_NPC, type, 1183, 1184);
	}

	cachedGridCollisionResult[entity][gridIndex] = result;
	return result;
}

// EntityGridCollisionClass.GRIDCOLL_BULLET (4)
bool __stdcall BulletGridCollisionCallbackHook(Entity* entity, GridEntity* gridEntity) {
	if (TriggerGridCollisionCallbacks(entity, *gridEntity->GetGridIndex())) {
		return true;
	}
	entity->_collidesWithGrid = true;
	return false;
}
void ASMPatchBulletGridCollisionCallback() {
	SigScan scanner("c687????????018b87????????8987????????8b87????????8987????????5f");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // Push the GridEntity
		.Push(ASMPatch::Registers::EDI)  // Push the Entity
		.AddInternalCall(BulletGridCollisionCallbackHook)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0xF73)  // If call returned true, ignore collision
		.AddRelativeJump((char*)addr + 0x7);

	sASMPatcher.PatchAt(addr, &patch);
}

// EntityGridCollisionClass.GRIDCOLL_GROUND (5)
int __stdcall GroundGridCollisionCallbackHook(Entity* entity, const int gridIndex) {
	Room* room = g_Game->GetCurrentRoom();
	const int gridCol = room->GetGridCollision(gridIndex);
	if (gridCol != 0 && TriggerGridCollisionCallbacks(entity, gridIndex)) {
		// Ignore collision
		return 0;
	}
	return gridCol;
}
void ASMPatchGroundGridCollisionCallback() {
	SigScan scanner("508bcee8????????85c074??f30f1055");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS - ASMPatch::SavedRegisters::EAX, true);
	ASMPatch patch;

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // Push the grid index
		.AddBytes("\x8B\x45\xE8")  // mov eax, dword ptr [ebp-0x18]
		.Push(ASMPatch::Registers::EAX)  // Push the Entity*
		.AddInternalCall(GroundGridCollisionCallbackHook)  // Ignore collision if result is 0
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x8);

	sASMPatcher.PatchAt(addr, &patch);
}

// EntityGridCollisionClass.GRIDCOLL_NOPITS (6)
bool __stdcall NoPitsGridCollisionCallbackHook(Entity* entity, const int gridRow, const int gridCol) {
	const int gridIndex = gridRow * g_Game->GetCurrentRoom()->_gridWidth + gridCol;
	return TriggerGridCollisionCallbacks(entity, gridIndex);
}
void ASMPatchNoPitsGridCollisionCallback() {
	SigScan scanner("8b75??8b87????????f30f1045");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\xFF\x75\xDC")  // push dword ptr [ebp-0x24] (column)
		.AddBytes("\xFF\x75\xE4")  // push dword ptr [ebp-0x1C] (row)
		.Push(ASMPatch::Registers::EDI)  // Push the Entity*
		.AddInternalCall(NoPitsGridCollisionCallbackHook)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x35)  // If call returned true, ignore collision
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x9))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 0x9);

	sASMPatcher.PatchAt(addr, &patch);
}

// EntityGridCollisionClass.GRIDCOLL_PITSONLY (7)
int __stdcall PitsOnlyCollisionCallbackHook(Entity* entity, const int gridIndex) {
	Room* room = g_Game->GetCurrentRoom();
	const int gridCol = room->GetGridCollision(gridIndex);
	if (gridCol != 1 && TriggerGridCollisionCallbacks(entity, gridIndex)) {
		// Ignore collision
		return 1;
	}
	return gridCol;
}
void ASMPatchPitsOnlyGridCollisionCallback() {
	SigScan scanner("508bcee8????????f30f104d??83f801");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS - ASMPatch::SavedRegisters::EAX, true);
	ASMPatch patch;

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // Push the grid index
		.AddBytes("\x8B\x45\xE8")  // mov eax, dword ptr [ebp-0x18]
		.Push(ASMPatch::Registers::EAX)  // Push the Entity*
		.AddInternalCall(PitsOnlyCollisionCallbackHook)  // Entity colides if this returns anything other than 1
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x8);

	sASMPatcher.PatchAt(addr, &patch);
}

// EntityGridCollisionClass.GRIDCOLL_WALLS (3)
// Important for flying enemies colliding with pillars.
int __stdcall WallCollisionCallbackHook(Entity* entity, const int gridIndex) {
	Room* room = g_Game->GetCurrentRoom();
	const int gridCol = room->GetGridCollision(gridIndex);
	if ((gridCol == 4 || gridCol == 5) && room->_roomType != 16 && TriggerGridCollisionCallbacks(entity, gridIndex)) {
		// Ignore collision
		return 0;
	}
	return gridCol;
}
void ASMPatchWallCollisionCallback() {
	SigScan scanner("508bcee8????????83f80474??83f80575");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS - ASMPatch::SavedRegisters::EAX, true);
	ASMPatch patch;

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // Push the grid index
		.AddBytes("\x8B\x45\xE8")  // mov eax, dword ptr [ebp-0x18]
		.Push(ASMPatch::Registers::EAX)  // Push the Entity*
		.AddInternalCall(WallCollisionCallbackHook)  // Entity collides if this returns 4 or 5, and roomtype != 0x10 (16, crawlspace)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x8);

	sASMPatcher.PatchAt(addr, &patch);
}

// I wasn't able to find a safe place to obtain the player's collided grid index at the time of collision.
// (I found the index a couple of times as offsets from EBP, but hard to guaruntee that those are safe to use.)
// So instead, I have two patches, one that stores the grid index here, and one that actually runs the callbacks.
// The patch that triggers the callback CANNOT be reached without triggering the first patch.
int playerGridCollisionCurrentGridIndex = 0;

void __stdcall PlayerGridCollisionTrackGridIndex(const int gridIndex) {
	playerGridCollisionCurrentGridIndex = gridIndex;
}
void ASMPatchPlayerGridCollisionTrackGridIndex() {
	SigScan scanner("81ffc00100007d??8b45");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Push the grid index
		.AddInternalCall(PlayerGridCollisionTrackGridIndex)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 0x6);

	sASMPatcher.PatchAt(addr, &patch);
}

int __stdcall PlayerGridCollisionCallbackHook(Entity_Player* player) {
	return TriggerGridCollisionCallbacks(player, playerGridCollisionCurrentGridIndex);
}
void ASMPatchPlayerGridCollisionCallback() {
	SigScan scanner("8b55??8b87????????8d0cd5");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)  // Push the Entity_Player
		.AddInternalCall(PlayerGridCollisionCallbackHook)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr - 0x2C3)  // If call returned true, ignore collision
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x9))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 0x9);

	sASMPatcher.PatchAt(addr, &patch);
}

/* /////////////////////
// Exposed function to apply the patches
*/ /////////////////////

void PatchGridCollisionCallback()
{
	ASMPatchBulletGridCollisionCallback();
	ASMPatchGroundGridCollisionCallback();
	ASMPatchNoPitsGridCollisionCallback();
	ASMPatchPitsOnlyGridCollisionCallback();
	ASMPatchWallCollisionCallback();
	
	ASMPatchPlayerGridCollisionTrackGridIndex();
	ASMPatchPlayerGridCollisionCallback();
}