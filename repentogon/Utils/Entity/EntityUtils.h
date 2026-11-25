#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#undef min

enum eEntityClass
{
    CLASS_NULL = 0,
    CLASS_PLAYER = eEntityType::ENTITY_PLAYER,
    CLASS_TEAR = eEntityType::ENTITY_TEAR,
    CLASS_FAMILIAR = eEntityType::ENTITY_FAMILIAR,
    CLASS_BOMB = eEntityType::ENTITY_BOMB,
    CLASS_PICKUP = eEntityType::ENTITY_PICKUP,
    CLASS_SLOT = eEntityType::ENTITY_SLOT,
    CLASS_LASER = eEntityType::ENTITY_LASER,
    CLASS_KNIFE = eEntityType::ENTITY_KNIFE,
    CLASS_PROJECTILE = eEntityType::ENTITY_PROJECTILE,
    CLASS_NPC = 10,
    CLASS_EFFECT = 11,
};

namespace EntityUtils
{
    constexpr eEntityClass GetEntityClass(int entityType)
    {
        if (entityType == eEntityType::ENTITY_EFFECT)
        {
            return eEntityClass::CLASS_EFFECT;
        }

        return (eEntityClass)std::min(entityType, 10);
    }

    eEntityClass GetEntityClass(const Entity& entity)
    {
        return GetEntityClass(entity._type);
    }

    constexpr const lua::LuaClassInterface& GetLuaClassInterface(eEntityClass entityClass)
    {
        constexpr lua::LuaClassInterface const* classInterfaces[] = {
            &LuaEntity::Interface,
            &LuaEntityPlayer::Interface,
            &LuaEntityTear::Interface,
            &LuaEntityFamiliar::Interface,
            &LuaEntityBomb::Interface,
            &LuaEntityPickup::Interface,
            &LuaEntitySlot::Interface,
            &LuaEntityLaser::Interface,
            &LuaEntityKnife::Interface,
            &LuaEntityProjectile::Interface,
            &LuaEntityNPC::Interface,
            &LuaEntityEffect::Interface,
        };

        return *classInterfaces[entityClass];
    }

    constexpr const lua::LuaClassInterface& GetLuaClassInterface(int entityType)
    {
        return GetLuaClassInterface(GetEntityClass(entityType));
    }

    constexpr const lua::LuaClassInterface& GetLuaClassInterface(const Entity& entity)
    {
        return GetLuaClassInterface(GetEntityClass(entity));
    }
}