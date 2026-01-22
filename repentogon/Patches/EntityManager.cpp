#include "EntityManager.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"
#include "Log.h"
#include "LuaCore.h"
#include "../LuaInterfaces/_Internals.h"
#include "../LuaInterfaces/CustomCallbacks.h"

namespace EntityManager::Data
{
    // Tracks if an entity has been removed since the last remove commit.
    static int s_stagedRemove = false;
}

namespace EntityManager::LuaCallbacks
{
    static int s_newEntityCallback = LUA_NOREF;
    static int s_deleteEntityCallback = LUA_NOREF;
}

void EntityManager::detail::Init::BindLuaCallbacks(lua_State* L, int tblIdx)
{
    assert(L == g_LuaEngine->_state);

    tblIdx = lua_absindex(L, tblIdx);
    if (!lua_istable(L, tblIdx))
    {
        ZHL::Log("[ERROR] EntityManager::BindLuaCallbacks: expected a table\n");
        LuaInternals::RaiseInitError();
        assert(false);
        return;
    }

    auto BindCallback = [](lua_State* L, int tblIdx, const char* fieldName, int& outRef)
    {
        // reset ref
        luaL_unref(L, LUA_REGISTRYINDEX, outRef);
        outRef = LUA_NOREF;

        // bind callback to ref
        lua_getfield(L, tblIdx, fieldName);
        if (!lua_isfunction(L, -1))
        {
            ZHL::Log("[ERROR] EntityManager::BindLuaCallbacks: Expected '%s' to be a function\n", fieldName);
            LuaInternals::RaiseInitError();
            lua_pop(L, 1);
            assert(false);
            return;
        }

        outRef = luaL_ref(L, LUA_REGISTRYINDEX);
    };

    BindCallback(L, tblIdx, "NewEntity", LuaCallbacks::s_newEntityCallback);
    BindCallback(L, tblIdx, "DeleteEntity", LuaCallbacks::s_deleteEntityCallback);
}

namespace EntityManager::Core
{
    static void NotifyNewEntity(const Entity& entity)
    {
        LuaEngine* lua = g_LuaEngine;

        lua_State* L = lua->_state;
        lua_rawgeti(L, LUA_REGISTRYINDEX, LuaCallbacks::s_newEntityCallback);
        lua_pushinteger(L, lua->GetMaskedPointer((uintptr_t)&entity));

        if (lua_pcall(L, 1, 0, 0) != LUA_OK)
        {
            ZHL::Log("[ERROR] EntityManager::NotifyNewEntity Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }

    static void NotifyDeleteEntity(const Entity& entity)
    {
        LuaEngine* lua = g_LuaEngine;

        lua_State* L = lua->_state;
        lua_rawgeti(L, LUA_REGISTRYINDEX, LuaCallbacks::s_deleteEntityCallback);
        lua_pushinteger(L, lua->GetMaskedPointer((uintptr_t)&entity));

        if (lua_pcall(L, 1, 0, 0) != LUA_OK)
        {
            ZHL::Log("[ERROR] EntityManager::NotifyDeleteEntity Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
}

void EntityManager::CommitRemovedEntities()
{
    if (!EntityManager::Data::s_stagedRemove)
    {
        return;
    }

    EntityList* entityList = g_Game->_room->GetEntityList();

    auto remove_entities = [](EntityList_EL* list, bool entityOwner)
    {
        for (int i = (int)list->_size - 1; i >= 0; i--)
        {
            Entity* entity = list->_data[i];
            if (entity->_exists)
            {
                continue;
            }

            if (entityOwner)
            {
                bool isPlayer = entity->_type == eEntityType::ENTITY_PLAYER;
                if (!isPlayer)
                {
                    // Players are handled by Free hook
                    EntityManager::Core::NotifyDeleteEntity(*entity);
                }
                bool wasValid = entity->_valid;
                entity->ClearReferences();
                entity->_valid = false;

                if (isPlayer && wasValid)
                {
                    unsigned int deallocate = 0x1;
                    entity->Free(deallocate);
                }
            }

            list->RemoveAt((size_t)i);
        }
    };

    remove_entities(entityList->GetUpdateEL(), false);
    remove_entities(entityList->GetRenderEL(), false);
    remove_entities(entityList->GetWispEL(), false);
    remove_entities(entityList->GetMainEL(), true);
    remove_entities(entityList->GetPersistentEL(), true);
    // other entity lists are not removed.

    EntityManager::Data::s_stagedRemove = false;
}

HOOK_METHOD(EntityFactory, Create, (unsigned int type, bool force) -> Entity*)
{
    Entity* entity = super(type, force);
    if (entity)
    {
        EntityManager::Core::NotifyNewEntity(*entity);
    }

    return entity;
}

// Quick way of deleting entities overridden by EntityFactory
HOOK_METHOD(Entity, Remove, () -> void)
{
    super();
    if (this->_removedByFactory)
    {
        EntityManager::Core::NotifyDeleteEntity(*this);
    }
    else
    {
        EntityManager::Data::s_stagedRemove = true;
    }
}

HOOK_METHOD(Entity_Player, constructor, () -> void)
{
    super();
    EntityManager::Core::NotifyNewEntity(*this);
}

// Entity_Player should be the only necessary one but just to be safe.
HOOK_METHOD(Entity, destructor, () -> void)
{
    EntityManager::Core::NotifyDeleteEntity(*this);
    super();
}

HOOK_METHOD(EntityList, Reset, () -> void)
{
    super();
    // Reset immediately removes entities.
    EntityManager::Data::s_stagedRemove = false;
}

static void __fastcall asm_remove_and_delete_entity(Entity& entity)
{
    entity.Remove(); // restore call to Remove
    EntityManager::Core::NotifyDeleteEntity(entity);
}

static void common_remove_and_delete_entity_patch(const char* id, const char* logIdentifier)
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(id);
    ZHL::Log("[REPENTOGON] Patching %s for EntityManager at %p\n", logIdentifier, addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 8;
    const size_t RESTORED_BYTES = 3;

    patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES)) // Moves the entity into ECX
        .PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_remove_and_delete_entity)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void Patch_EntityListDestructor_RemoveMainEL()
{
    common_remove_and_delete_entity_patch(&AsmDefinitions::EntityList_destructor_RemoveEntityMainEL, "EntityList::destructor");
}

static void Patch_EntityListDestructor_RemovePersistentEL()
{
    common_remove_and_delete_entity_patch(&AsmDefinitions::EntityList_destructor_RemoveEntityPersistentEL, "EntityList::destructor");
}

static void Patch_EntityListReset_RemoveNonPersistentEntity()
{
    common_remove_and_delete_entity_patch(&AsmDefinitions::EntityList_Reset_RemoveNonPersistentEntity, "EntityList::Reset");
}

static void __fastcall asm_clear_references_and_delete_entity(Entity& entity)
{
    EntityManager::Core::NotifyDeleteEntity(entity);
    entity.ClearReferences(); // restore call to ClearReferences
}

static void Patch_EntityListUpdate_RemoveEntityMainEL()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityList_Update_RemoveEntityMainEL);
    ZHL::Log("[REPENTOGON] Patching EntityList::Update for EntityManager at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 7;

    patch.CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::EDX)
        .PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_clear_references_and_delete_entity)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall asm_delete_persistent_el_entity(Entity& entity)
{
    // Players have unique handling when being deleted from persistent data
    // If they are deleted they invoke free, which is then handled by the other hook.
    if (entity._type != ENTITY_PLAYER)
    {
        EntityManager::Core::NotifyDeleteEntity(entity);
    }
}

static void Patch_EntityListUpdate_RemoveEntityPersistentEL()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityList_Update_RemoveEntityPersistentEL);
    ZHL::Log("[REPENTOGON] Patching EntityList::Update for EntityManager at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 8;
    const size_t RESTORED_BYTES = 8;

    patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES)) // Restore + Move Entity into ECX
        .PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_delete_persistent_el_entity)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __fastcall asm_pre_commit_entity_remove(bool isTransition, bool& stagedRemove)
{
    if (!isTransition) // collide is not called when on transitions
    {
        CustomCallbacks::PRE_ROOM_COLLISION_PASS();
    }

    // Update has a local `stagedRemove` flag that indicates whether at least one entity
    // is staged for removal. It is used to skip the commit step, which would otherwise
    // iterate over every EL to check for removals.
    //
    // However, this flag is only updated after each entity runs its update. If an entity
    // is staged for removal after its update, the flag will not be set, and the entity
    // may persist for one extra update cycle if no future entities are removed.
    //
    // To avoid this order-dependent issue, we track staged removals using a global flag.
    // This ensures the commit step is evaluated correctly regardless of update order.
    stagedRemove = stagedRemove || EntityManager::Data::s_stagedRemove;
    EntityManager::Data::s_stagedRemove = false;
}

// This patch is placed right when the local 'stagedRemove' flag is checked.
static void Patch_EntityListUpdate_PreCommitRemovedEntities()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityList_Update_PreCommitRemovedEntities);
    ZHL::Log("[REPENTOGON] Patching EntityList::Update for EntityManager at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t cmpAddr = addr;
    int8_t cmp_m8_disp = *(int8_t*)cmpAddr + 2;
    intptr_t jeAddr = addr + 4;
    intptr_t skipCommitJmp_rel32 = *(int32_t*)(jeAddr + 2);
    intptr_t skipCommitAddr = jeAddr + 6 + skipCommitJmp_rel32;
    intptr_t commitRemovedEntitiesAddr = addr + 10;
    constexpr size_t RESTORED_BYTES = 4; // restore the cmp

    patch.PreserveRegisters(savedRegisters)
        .LoadEffectiveAddress(ASMPatch::Registers::EBP, cmp_m8_disp, ASMPatch::Registers::EDX) // stagedRemove
        .MoveFromMemory(ASMPatch::Registers::EBX, 8, ASMPatch::Registers::ECX) // isTransition
        .AddInternalCall(asm_pre_commit_entity_remove)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (void*)skipCommitAddr)
        .AddRelativeJump((void*)commitRemovedEntitiesAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

void EntityManager::detail::Patches::ApplyPatches()
{
    Patch_EntityListDestructor_RemoveMainEL();
    Patch_EntityListDestructor_RemovePersistentEL();
    Patch_EntityListReset_RemoveNonPersistentEntity();
    Patch_EntityListUpdate_RemoveEntityMainEL();
    Patch_EntityListUpdate_RemoveEntityPersistentEL();
    Patch_EntityListUpdate_PreCommitRemovedEntities();
}