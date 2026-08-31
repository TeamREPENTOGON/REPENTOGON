#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#undef min

enum eGridEntityClass
{
    GRID_CLASS_NULL = 0,
    GRID_CLASS_DECORATION = 1,
    GRID_CLASS_ROCK = 2,
    GRID_CLASS_PIT = 3,
    GRID_CLASS_SPIKES = 4,
    GRID_CLASS_WEB = 5,
    GRID_CLASS_LOCK = 6,
    GRID_CLASS_TNT = 7,
    GRID_CLASS_FIRE = 8,
    GRID_CLASS_POOP = 9,
    GRID_CLASS_WALL = 10,
    GRID_CLASS_DOOR = 11,
    GRID_CLASS_TRAPDOOR = 12,
    GRID_CLASS_STAIRS = 13,
    GRID_CLASS_GRAVITY = 14,
    GRID_CLASS_PRESSURE_PLATE = 15,
    GRID_CLASS_STATUE = 16,
    GRID_CLASS_TELEPORTER = 17,
};

namespace GridEntityUtils
{
    constexpr eGridEntityClass GetGridEntityClass(int gridEntityType)
    {
        switch (gridEntityType)
        {
            case GridEntityType::GRID_DECORATION:
                return GRID_CLASS_DECORATION;
            case GridEntityType::GRID_ROCK:
            case GridEntityType::GRID_ROCKB:
            case GridEntityType::GRID_ROCKT:
            case GridEntityType::GRID_ROCK_BOMB:
            case GridEntityType::GRID_ROCK_ALT:
            case GridEntityType::GRID_ROCK_SS:
            case GridEntityType::GRID_PILLAR:
            case GridEntityType::GRID_ROCK_SPIKED:
            case GridEntityType::GRID_ROCK_ALT2:
            case GridEntityType::GRID_ROCK_GOLD:
                return GRID_CLASS_ROCK;
            case GridEntityType::GRID_PIT:
                return GRID_CLASS_PIT;
            case GridEntityType::GRID_SPIKES:
            case GridEntityType::GRID_SPIKES_ONOFF:
                return GRID_CLASS_SPIKES;
            case GridEntityType::GRID_SPIDERWEB:
                return GRID_CLASS_WEB;
            case GridEntityType::GRID_LOCK:
                return GRID_CLASS_LOCK;
            case GridEntityType::GRID_TNT:
                return GRID_CLASS_TNT;
            case GridEntityType::GRID_FIREPLACE:
                return GRID_CLASS_FIRE;
            case GridEntityType::GRID_POOP:
                return GRID_CLASS_POOP;
            case GridEntityType::GRID_WALL:
                return GRID_CLASS_WALL;
            case GridEntityType::GRID_DOOR:
                return GRID_CLASS_DOOR;
            case GridEntityType::GRID_TRAPDOOR:
                return GRID_CLASS_TRAPDOOR;
            case GridEntityType::GRID_STAIRS:
                return GRID_CLASS_STAIRS;
            case GridEntityType::GRID_GRAVITY:
                return GRID_CLASS_GRAVITY;
            case GridEntityType::GRID_PRESSURE_PLATE:
                return GRID_CLASS_PRESSURE_PLATE;
            case GridEntityType::GRID_STATUE:
                return GRID_CLASS_STATUE;
            case GridEntityType::GRID_TELEPORTER:
                return GRID_CLASS_TELEPORTER;
            default:
                return GRID_CLASS_NULL;
        }
    }

    eGridEntityClass GetGridEntityClass(const GridEntity& entity)
    {
        return GetGridEntityClass(entity._desc._type);
    }

    constexpr const lua::LuaClassInterface& GetLuaClassInterface(eGridEntityClass entityClass)
    {
        constexpr lua::LuaClassInterface const* classInterfaces[] = {
            &LuaGridEntity::Interface,
            &LuaGridEntityDecoration::Interface,
            &LuaGridEntityRock::Interface,
            &LuaGridEntityPit::Interface,
            &LuaGridEntitySpikes::Interface,
            &LuaGridEntityWeb::Interface,
            &LuaGridEntityLock::Interface,
            &LuaGridEntityTNT::Interface,
            &LuaGridEntityFire::Interface,
            &LuaGridEntityPoop::Interface,
            &LuaGridEntityWall::Interface,
            &LuaGridEntityDoor::Interface,
            &LuaGridEntityTrapDoor::Interface,
            &LuaGridEntityStairs::Interface,
            &LuaGridEntityGravity::Interface,
            &LuaGridEntityPressurePlate::Interface,
            &LuaGridEntityStatue::Interface,
            &LuaGridEntityTeleporter::Interface,
        };

        return *classInterfaces[entityClass];
    }

    constexpr const lua::LuaClassInterface& GetLuaClassInterface(int entityType)
    {
        return GetLuaClassInterface(GetGridEntityClass(entityType));
    }

    const lua::LuaClassInterface& GetLuaClassInterface(const GridEntity& entity)
    {
        return GetLuaClassInterface(GetGridEntityClass(entity));
    }
}