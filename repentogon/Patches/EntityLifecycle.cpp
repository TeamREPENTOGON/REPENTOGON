#include "EntityLifecycle.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"
#include "Log.h"
#include "LuaCore.h"
#include "../LuaInterfaces/_Internals.h"

namespace EntityLifecycle::Data
{
    static int s_luaNewEntityCallback = LUA_NOREF;
    static int s_luaDeleteEntityCallback = LUA_NOREF;
}

void EntityLifecycle::detail::Init::BindLuaCallbacks(lua_State* L, int tblIdx)
{
    assert(L == g_LuaEngine->_state);

    tblIdx = lua_absindex(L, tblIdx);
    if (!lua_istable(L, tblIdx))
    {
        ZHL::Log("[ERROR] EntityLifecycle::BindLuaCallbacks: expected a table\n");
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
            ZHL::Log("[ERROR] EntityLifecycle::BindLuaCallbacks: Expected '%s' to be a function\n", fieldName);
            LuaInternals::RaiseInitError();
            lua_pop(L, 1);
            assert(false);
            return;
        }

        outRef = luaL_ref(L, LUA_REGISTRYINDEX);
    };

    BindCallback(L, tblIdx, "NewEntity", Data::s_luaNewEntityCallback);
    BindCallback(L, tblIdx, "DeleteEntity", Data::s_luaDeleteEntityCallback);
}

namespace EntityLifecycle::Core
{
    static void NotifyNewEntity(const Entity& entity)
    {
        LuaEngine* lua = g_LuaEngine;

        lua_State* L = lua->_state;
        lua_rawgeti(L, LUA_REGISTRYINDEX, Data::s_luaNewEntityCallback);
        lua_pushinteger(L, lua->GetMaskedPointer((uintptr_t)&entity));

        if (lua_pcall(L, 1, 0, 0) != LUA_OK)
        {
            ZHL::Log("[ERROR] EntityLifecycle::NotifyNewEntity Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }

    static void NotifyDeleteEntity(const Entity& entity)
    {
        LuaEngine* lua = g_LuaEngine;

        lua_State* L = lua->_state;
        lua_rawgeti(L, LUA_REGISTRYINDEX, Data::s_luaDeleteEntityCallback);
        lua_pushinteger(L, lua->GetMaskedPointer((uintptr_t)&entity));

        if (lua_pcall(L, 1, 0, 0) != LUA_OK)
        {
            ZHL::Log("[ERROR] EntityLifecycle::NotifyDeleteEntity Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
}

HOOK_METHOD(EntityFactory, Create, (unsigned int type, bool force) -> Entity*)
{
    Entity* entity = super(type, force);
    if (entity)
    {
        EntityLifecycle::Core::NotifyNewEntity(*entity);
    }

    return entity;
}

// Quick way of deleting entities overridden by EntityFactory
HOOK_METHOD(Entity, Remove, () -> void)
{
    super();
    if (this->_removedByFactory)
    {
        EntityLifecycle::Core::NotifyDeleteEntity(*this);
    }
}

HOOK_METHOD(Entity_Player, constructor, () -> void)
{
    super();
    EntityLifecycle::Core::NotifyNewEntity(*this);
}

// Entity_Player should be the only necessary one but just to be safe.
HOOK_METHOD(Entity, destructor, () -> void)
{
    EntityLifecycle::Core::NotifyDeleteEntity(*this);
    super();
}

static void __fastcall asm_remove_and_delete_entity(Entity& entity)
{
    entity.Remove(); // restore call to Remove
    EntityLifecycle::Core::NotifyDeleteEntity(entity);
}

static void common_remove_and_delete_entity_patch(const char* id, const char* logIdentifier)
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(id);
    ZHL::Log("[REPENTOGON] Patching %s for EntityLifecycle at %p\n", logIdentifier, addr);

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
    EntityLifecycle::Core::NotifyDeleteEntity(entity);
    entity.ClearReferences(); // restore call to ClearReferences
}

static void Patch_EntityListUpdate_RemoveEntityMainEL()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityList_Update_RemoveEntityMainEL);
    ZHL::Log("[REPENTOGON] Patching EntityList::Update for EntityLifecycle at %p\n", addr);

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
        EntityLifecycle::Core::NotifyDeleteEntity(entity);
    }
}

static void Patch_EntityListUpdate_RemoveEntityPersistentEL()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityList_Update_RemoveEntityPersistentEL);
    ZHL::Log("[REPENTOGON] Patching EntityList::Update for EntityLifecycle at %p\n", addr);

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

 void EntityLifecycle::detail::Patches::ApplyPatches()
 {
    Patch_EntityListDestructor_RemoveMainEL();
    Patch_EntityListDestructor_RemovePersistentEL();
    Patch_EntityListReset_RemoveNonPersistentEntity();
    Patch_EntityListUpdate_RemoveEntityMainEL();
    Patch_EntityListUpdate_RemoveEntityPersistentEL();
}