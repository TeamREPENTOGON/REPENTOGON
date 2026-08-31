#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"

template<typename T>
struct LuaClassTraits;

namespace LuaClasses
{
    namespace detail
    {
        template<typename T, typename = void>
        struct HasUserdataValueVftable : std::false_type {};
        
        template<typename T>
        struct HasUserdataValueVftable<T, std::void_t<decltype(LuaClassTraits<T>::UserdataValueVftable)>>
            : std::true_type {};

        template<typename T>
        constexpr bool HasUserdataValueVftable_v = HasUserdataValueVftable<T>::value;
    }
}



template<typename T, auto MT, auto CONST_MT>
struct LuabridgeType
{
private:
    using Traits = LuaClassTraits<T>;

public:
    static bool IsUnderlyingType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TUSERDATA;
    }

    static T* Get(lua_State* L, int index)
    {
        return lua::GetLuabridgeUserdata<T*>(L, index, MT, Traits::Name);
    }

    static const T* GetConst(lua_State* L, int index)
    {
        return lua::GetLuabridgeUserdata<T*>(L, index, CONST_MT, Traits::Name);
    }

    static T* GetOpt(lua_State* L, int index)
    {
        return !lua_isnoneornil(L, index) ? Get(L, index) : nullptr;
    }

    static const T* GetConstOpt(lua_State* L, int index)
    {
        return !lua_isnoneornil(L, index) ? GetConst(L, index) : nullptr;
    }

    static T* Place(lua_State* L)
    {
        void* key = lua::GetMetatableKey(MT);
        if constexpr (LuaClasses::detail::HasUserdataValueVftable<T>::value)
        {
            return lua::luabridge::UserdataValue<T>::place_with_vftable(L, key, Traits::UserdataValueVftable);
        }
        else
        {
            return lua::luabridge::UserdataValue<T>::place(L, key);
        }
    }

    static T* PlaceConst(lua_State* L)
    {
        void* key = lua::GetMetatableKey(CONST_MT);
        if constexpr (LuaClasses::detail::HasUserdataValueVftable<T>::value)
        {
            return lua::luabridge::UserdataValue<T>::place_with_vftable(L, key, Traits::UserdataValueVftable);
        }
        else
        {
            return lua::luabridge::UserdataValue<T>::place(L, key);
        }
    }

    static void Push(lua_State* L, const T& value)
    {
        new (Place(L))  T(value);
    }
    
    static void PushConst(lua_State* L, const T& value)
    {
        new (PlaceConst(L))  T(value);
    }

    static void PushPtr(lua_State* L, T* ptr)
    {
        lua::luabridge::UserdataPtr::push(L, ptr, lua::GetMetatableKey(MT));
    }

    static void PushConstPtr(lua_State* L, T* ptr)
    {
        lua::luabridge::UserdataPtr::push(L, ptr, lua::GetMetatableKey(CONST_MT));
    }

    static constexpr lua::LuaClassInterface Interface
    {
        [](lua_State* L, const void* value)
        {
            Push(L, *static_cast<const T*>(value));
        },

        [](lua_State* L, void* value)
        {
            PushPtr(L, static_cast<T*>(value));
        }
    };
};

template<typename T, const char*& MT>
struct LuabridgeRGONType
{
    static bool IsUnderlyingType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TUSERDATA;
    }

    static T* Get(lua_State* L, int index)
    {
        luaL_checkudata(L, index, MT);
        return lua::UserdataToData<T*>(lua_touserdata(L, index));
    }

    static T* GetOpt(lua_State* L, int index)
    {
        return !lua_isnoneornil(L, index) ? Get(L, index) : nullptr;
    }

    static T* Place(lua_State* L)
    {
        return lua::luabridge::UserdataValue<T>::place(L, MT);
    }

    static void Push(lua_State* L, const T& value)
    {
        lua::luabridge::UserdataValue<T>::push(L, (void*)MT, value);
    }

    static void PushPtr(lua_State* L, T* ptr)
    {
        lua::luabridge::UserdataPtr::push(L, ptr, MT);
    }

    static constexpr lua::LuaClassInterface Interface
    {
        [](lua_State* L, const void* value)
        {
            Push(L, *static_cast<const T*>(value));
        },

        [](lua_State* L, void* value)
        {
            PushPtr(L, static_cast<T*>(value));
        }
    };
};

template<typename T, const char*& MT>
struct UserdataPtr
{
    static bool IsUnderlyingType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TUSERDATA;
    }

    static T* Get(lua_State* L, int index)
    {
        return *lua::GetRawUserdata<T**>(L, idx, MT);
    }

    static T* GetOpt(lua_State* L, int index)
    {
        return !lua_isnoneornil(L, index) ? Get(L, index) : nullptr;
    }

    static void PushPtr(lua_State* L, T* ptr)
    {
        void** result = (void**)lua_newuserdata(L, sizeof(void*));
        *result = ptr;
        luaL_setmetatable(L, MT);
    }
};

template<typename T, auto ID, auto PTR_ID>
struct CDataType
{
    static bool IsUnderlyingType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TCDATA;
    }

    static T* Get(lua_State* L, int index)
    {
        return lua::GetCData<T*>(L, index, lua::ffi::CData[ID], LuaClassTraits<T>::Name);
    }

    static T* GetOpt(lua_State* L, int index)
    {
        return !lua_isnoneornil(L, index) ? Get(L, index) : nullptr;
    }

    static T* Place(lua_State* L)
    {
        return lua::ffi::placeCdata<T>(L, lua::ffi::CData[ID]);
    }

    static void Push(lua_State* L, const T& value)
    {
        lua::ffi::pushCdata(L, lua::ffi::CData[ID], value);
    }

    static void PushPtr(lua_State* L, T* ptr)
    {
        lua::ffi::pushCdataPtr(L, ptr, lua::ffi::CData[PTR_ID]);
    }

    static constexpr lua::LuaClassInterface Interface
    {
        [](lua_State* L, const void* value)
        {
            Push(L, *static_cast<const T*>(value));
        },

        [](lua_State* L, void* value)
        {
            PushPtr(L, static_cast<T*>(value));
        }
    };
};

template<>
struct LuaClassTraits<Vector>
{
    static constexpr const char* Name = "Vector";
};

template<>
struct LuaClassTraits<PosVel>
{
    static constexpr const char* Name = "PosVel";
};

template<>
struct LuaClassTraits<BitSet128>
{
    static constexpr const char* Name = "BitSet128";
};

template<>
struct LuaClassTraits<KColor>
{
    static constexpr const char* Name = "KColor";
};

template<>
struct LuaClassTraits<ColorMod>
{
    static constexpr const char* Name = "Color";
};

template<>
struct LuaClassTraits<ANM2>
{
    static constexpr const char* Name = "Sprite";
    inline static void**& UserdataValueVftable = __ptr_UserdataValue_ANM2_vftable;
};

template<>
struct LuaClassTraits<Font>
{
    static constexpr const char* Name = "Font";
};

template<>
struct LuaClassTraits<FontSettings>
{
    static constexpr const char* Name = "FontRenderSettings";
};

template<>
struct LuaClassTraits<RNG>
{
    static constexpr const char* Name = "RNG";
};

template<>
struct LuaClassTraits<Music>
{
    static constexpr const char* Name = "MusicManager";
};

template<>
struct LuaClassTraits<SoundEffects>
{
    static constexpr const char* Name = "SFXManager";
};

template<>
struct LuaClassTraits<ItemConfig>
{
    static constexpr const char* Name = "ItemConfig";
};

template<>
struct LuaClassTraits<ItemConfig_Item>
{
    static constexpr const char* Name = "Item";
};

template<>
struct LuaClassTraits<ItemConfig_Card>
{
    static constexpr const char* Name = "Card";
};

template<>
struct LuaClassTraits<ItemConfig_Pill>
{
    static constexpr const char* Name = "PillEffect";
};

template<>
struct LuaClassTraits<ItemConfig_Costume>
{
    static constexpr const char* Name = "Costume";
};

template<>
struct LuaClassTraits<RoomConfig_Room>
{
    static constexpr const char* Name = "RoomConfigRoom";
};

template<>
struct LuaClassTraits<Seeds>
{
    static constexpr const char* Name = "Seeds";
};

template<>
struct LuaClassTraits<Game>
{
    static constexpr const char* Name = "Game";
};

template<>
struct LuaClassTraits<Level>
{
    static constexpr const char* Name = "Level";
};

template<>
struct LuaClassTraits<Room>
{
    static constexpr const char* Name = "Room";
};

template<>
struct LuaClassTraits<RoomDescriptor>
{
    static constexpr const char* Name = "RoomDescriptor";
};

template<>
struct LuaClassTraits<ItemPool>
{
    static constexpr const char* Name = "ItemPool";
};

template<>
struct LuaClassTraits<HUD>
{
    static constexpr const char* Name = "HUD";
};

template<>
struct LuaClassTraits<Entity>
{
    static constexpr const char* Name = "Entity";
};

template<>
struct LuaClassTraits<Entity_Player>
{
    static constexpr const char* Name = "EntityPlayer";
};

template<>
struct LuaClassTraits<Entity_Tear>
{
    static constexpr const char* Name = "EntityTear";
};

template<>
struct LuaClassTraits<Entity_Familiar>
{
    static constexpr const char* Name = "EntityFamiliar";
};

template<>
struct LuaClassTraits<Entity_Bomb>
{
    static constexpr const char* Name = "EntityBomb";
};

template<>
struct LuaClassTraits<Entity_Pickup>
{
    static constexpr const char* Name = "EntityPickup";
};

template<>
struct LuaClassTraits<Entity_Laser>
{
    static constexpr const char* Name = "EntityLaser";
};

template<>
struct LuaClassTraits<Entity_Knife>
{
    static constexpr const char* Name = "EntityKnife";
};

template<>
struct LuaClassTraits<Entity_Projectile>
{
    static constexpr const char* Name = "EntityProjectile";
};

template<>
struct LuaClassTraits<Entity_NPC>
{
    static constexpr const char* Name = "EntityNPC";
};

template<>
struct LuaClassTraits<Entity_Effect>
{
    static constexpr const char* Name = "EntityEffect";
};

template<>
struct LuaClassTraits<EntityRef>
{
    static constexpr const char* Name = "EntityRef";
};

template<>
struct LuaClassTraits<EntityPtr>
{
    static constexpr const char* Name = "EntityPtr";
};

template<>
struct LuaClassTraits<NPCAI_Pathfinder>
{
    static constexpr const char* Name = "Pathfinder";
};

template<>
struct LuaClassTraits<TearParams>
{
    static constexpr const char* Name = "TearParams";
};

template<>
struct LuaClassTraits<ProjectileParams>
{
    static constexpr const char* Name = "ProjectileParams";
};

template<>
struct LuaClassTraits<TemporaryEffects>
{
    static constexpr const char* Name = "TemporaryEffects";
};

template<>
struct LuaClassTraits<ActiveItemDesc>
{
    static constexpr const char* Name = "ActiveItemDesc";
};

template<>
struct LuaClassTraits<GridEntity>
{
    static constexpr const char* Name = "GridEntity";
};

template<>
struct LuaClassTraits<GridEntity_Rock>
{
    static constexpr const char* Name = "GridEntityRock";
};

template<>
struct LuaClassTraits<GridEntity_Pit>
{
    static constexpr const char* Name = "GridEntityPit";
};

template<>
struct LuaClassTraits<GridEntity_Spikes>
{
    static constexpr const char* Name = "GridEntitySpikes";
};

template<>
struct LuaClassTraits<GridEntity_TNT>
{
    static constexpr const char* Name = "GridEntityTNT";
};

template<>
struct LuaClassTraits<GridEntity_Poop>
{
    static constexpr const char* Name = "GridEntityPoop";
};

template<>
struct LuaClassTraits<GridEntity_Door>
{
    static constexpr const char* Name = "GridEntityDoor";
};

template<>
struct LuaClassTraits<GridEntity_PressurePlate>
{
    static constexpr const char* Name = "GridEntityPressurePlate";
};

template<>
struct LuaClassTraits<GridEntityDesc>
{
    static constexpr const char* Name = "GridEntityDesc";
};

using LuaVector = CDataType<Vector, lua::ffi::CDataID::VECTOR, lua::ffi::CDataID::VECTOR_PTR>;
using LuaPosVel = CDataType<PosVel, lua::ffi::CDataID::POS_VEL, lua::ffi::CDataID::POS_VEL_PTR>;
using LuaBitSet128 = CDataType<BitSet128, lua::ffi::CDataID::BITSET_128, lua::ffi::CDataID::BITSET_128_PTR>;
using LuaKColor = CDataType<KColor, lua::ffi::CDataID::KCOLOR, lua::ffi::CDataID::KCOLOR_PTR>;
using LuaColor = CDataType<ColorMod, lua::ffi::CDataID::COLOR, lua::ffi::CDataID::COLOR_PTR>;
using LuaSprite = LuabridgeType<ANM2, lua::Metatables::SPRITE, lua::Metatables::CONST_SPRITE>;
using LuaFont = LuabridgeType<Font, lua::Metatables::FONT, lua::Metatables::CONST_FONT>;
using LuaFontRenderSettings = LuabridgeType<FontSettings, lua::Metatables::FONTRENDERSETTINGS, lua::Metatables::CONST_FONTRENDERSETTINGS>;
using LuaRNG = LuabridgeType<RNG, lua::Metatables::RNG, lua::Metatables::CONST_RNG>;
using LuaMusicManager = LuabridgeType<Music, lua::Metatables::MUSIC_MANAGER, lua::Metatables::CONST_MUSIC_MANAGER>;
using LuaSFXManager = LuabridgeType<SoundEffects, lua::Metatables::SFX_MANAGER, lua::Metatables::CONST_SFX_MANAGER>;
using LuaItemConfig = LuabridgeType<ItemConfig, lua::Metatables::CONFIG, lua::Metatables::CONST_CONFIG>;
using LuaItem = CDataType<ItemConfig_Item, lua::ffi::CDataID::ITEM, lua::ffi::CDataID::ITEM_PTR>;
using LuaCard = LuabridgeType<ItemConfig_Card, lua::Metatables::CARD, lua::Metatables::CONST_CARD>;
using LuaPillEffect = LuabridgeType<ItemConfig_Pill, lua::Metatables::PILL_EFFECT, lua::Metatables::CONST_PILL_EFFECT>;
using LuaCostume = LuabridgeType<ItemConfig_Costume, lua::Metatables::COSTUME, lua::Metatables::CONST_COSTUME>;
using LuaRoomConfigRoom = LuabridgeType<RoomConfig_Room, lua::Metatables::ROOM_CONFIG_ROOM, lua::Metatables::CONST_ROOM_CONFIG_ROOM>;
using LuaSeeds = LuabridgeType<Seeds, lua::Metatables::SEEDS, lua::Metatables::CONST_SEEDS>;
using LuaGame = LuabridgeType<Game, lua::Metatables::GAME, lua::Metatables::CONST_GAME>;
using LuaLevel = LuabridgeType<Level, lua::Metatables::LEVEL, lua::Metatables::CONST_LEVEL>;
using LuaRoom = LuabridgeType<Room, lua::Metatables::ROOM, lua::Metatables::CONST_ROOM>;
using LuaRoomDescriptor = LuabridgeType<RoomDescriptor, lua::Metatables::ROOM_DESCRIPTOR, lua::Metatables::CONST_ROOM_DESCRIPTOR>;
using LuaItemPool = LuabridgeType<ItemPool, lua::Metatables::ITEM_POOL, lua::Metatables::CONST_ITEM_POOL>;
using LuaHUD = LuabridgeType<HUD, lua::Metatables::HUD, lua::Metatables::CONST_HUD>;
using LuaEntity = LuabridgeType<Entity, lua::Metatables::ENTITY, lua::Metatables::CONST_ENTITY>;
using LuaEntityPlayer = LuabridgeType<Entity_Player, lua::Metatables::ENTITY_PLAYER, lua::Metatables::CONST_ENTITY_PLAYER>;
using LuaEntityTear = LuabridgeType<Entity_Tear, lua::Metatables::ENTITY_TEAR, lua::Metatables::CONST_ENTITY_TEAR>;
using LuaEntityFamiliar = LuabridgeType<Entity_Familiar, lua::Metatables::ENTITY_FAMILIAR, lua::Metatables::CONST_ENTITY_FAMILIAR>;
using LuaEntityBomb = LuabridgeType<Entity_Bomb, lua::Metatables::ENTITY_BOMB, lua::Metatables::CONST_ENTITY_BOMB>;
using LuaEntityPickup = LuabridgeType<Entity_Pickup, lua::Metatables::ENTITY_PICKUP, lua::Metatables::CONST_ENTITY_PICKUP>;
using LuaEntityLaser = LuabridgeType<Entity_Laser, lua::Metatables::ENTITY_LASER, lua::Metatables::CONST_ENTITY_LASER>;
using LuaEntityKnife = LuabridgeType<Entity_Knife, lua::Metatables::ENTITY_KNIFE, lua::Metatables::CONST_ENTITY_KNIFE>;
using LuaEntityProjectile = LuabridgeType<Entity_Projectile, lua::Metatables::ENTITY_PROJECTILE, lua::Metatables::CONST_ENTITY_PROJECTILE>;
using LuaEntityNPC = LuabridgeType<Entity_NPC, lua::Metatables::ENTITY_NPC, lua::Metatables::CONST_ENTITY_NPC>;
using LuaEntityEffect = LuabridgeType<Entity_Effect, lua::Metatables::ENTITY_EFFECT, lua::Metatables::CONST_ENTITY_EFFECT>;
using LuaEntityRef = LuabridgeType<EntityRef, lua::Metatables::ENTITY_REF, lua::Metatables::CONST_ENTITY_REF>;
using LuaEntityPtr = LuabridgeType<EntityPtr, lua::Metatables::ENTITY_PTR, lua::Metatables::CONST_ENTITY_PTR>;
using LuaPathfinder = LuabridgeType<NPCAI_Pathfinder, lua::Metatables::PATHFINDER, lua::Metatables::CONST_PATHFINDER>;
using LuaTearParams = LuabridgeType<TearParams, lua::Metatables::TEAR_PARAMS, lua::Metatables::CONST_TEAR_PARAMS>;
using LuaProjectileParams = LuabridgeType<ProjectileParams, lua::Metatables::PROJECTILE_PARAMS, lua::Metatables::CONST_PROJECTILE_PARAMS>;
using LuaTemporaryEffects = LuabridgeType<TemporaryEffects, lua::Metatables::_TEMPORARY_EFFECTS, lua::Metatables::_CONST_TEMPORARY_EFFECTS>;
using LuaActiveItemDesc = LuabridgeType<ActiveItemDesc, lua::Metatables::ACTIVE_ITEM_DESC, lua::Metatables::CONST_ACTIVE_ITEM_DESC>;
using LuaGridEntity = LuabridgeType<GridEntity, lua::Metatables::GRID_ENTITY, lua::Metatables::CONST_GRID_ENTITY>;
using LuaGridEntityRock = LuabridgeType<GridEntity_Rock, lua::Metatables::GRID_ENTITY_ROCK, lua::Metatables::CONST_GRID_ENTITY_ROCK>;
using LuaGridEntityPit = LuabridgeType<GridEntity_Pit, lua::Metatables::GRID_ENTITY_PIT, lua::Metatables::CONST_GRID_ENTITY_PIT>;
using LuaGridEntitySpikes = LuabridgeType<GridEntity_Spikes, lua::Metatables::GRID_ENTITY_SPIKES, lua::Metatables::CONST_GRID_ENTITY_SPIKES>;
using LuaGridEntityTNT = LuabridgeType<GridEntity_TNT, lua::Metatables::GRID_ENTITY_TNT, lua::Metatables::CONST_GRID_ENTITY_TNT>;
using LuaGridEntityPoop = LuabridgeType<GridEntity_Poop, lua::Metatables::GRID_ENTITY_POOP, lua::Metatables::CONST_GRID_ENTITY_POOP>;
using LuaGridEntityDoor = LuabridgeType<GridEntity_Door, lua::Metatables::GRID_ENTITY_DOOR, lua::Metatables::CONST_GRID_ENTITY_DOOR>;
using LuaGridEntityPressurePlate = LuabridgeType<GridEntity_PressurePlate, lua::Metatables::GRID_ENTITY_PRESSURE_PLATE, lua::Metatables::CONST_GRID_ENTITY_PRESSURE_PLATE>;
using LuaGridEntityDesc = LuabridgeType<GridEntityDesc, lua::Metatables::GRID_ENTITY_DESC, lua::Metatables::CONST_GRID_ENTITY_DESC>;

// RGON Classes

using LuaHistoryHUD = UserdataPtr<HistoryHUD, lua::metatables::HistoryHUDMT>;
using LuaLevelGenerator = UserdataPtr<LevelGenerator, lua::metatables::LevelGeneratorMT>;
using LuaBossPool = UserdataPtr<BossPool_Pool, lua::metatables::BossPoolMT>;
using LuaEntitySlot = LuabridgeRGONType<Entity_Slot, lua::metatables::EntitySlotMT>;
using LuaEntityDelirium = LuabridgeRGONType<Entity_NPC, lua::metatables::DeliriumMetatable>;
using LuaGridEntityDecoration = LuabridgeRGONType<GridEntity_Decoration, lua::metatables::GridDecorationMT>;
using LuaGridEntityWeb = LuabridgeRGONType<GridEntity_Web, lua::metatables::GridWebMT>;
using LuaGridEntityLock = LuabridgeRGONType<GridEntity_Lock, lua::metatables::GridLockMT>;
using LuaGridEntityFire = LuabridgeRGONType<GridEntity_Fire, lua::metatables::GridFireMT>;
using LuaGridEntityWall = LuabridgeRGONType<GridEntity_Wall, lua::metatables::GridWallMT>;
using LuaGridEntityTrapDoor = LuabridgeRGONType<GridEntity_TrapDoor, lua::metatables::GridTrapDoorMT>;
using LuaGridEntityStairs = LuabridgeRGONType<GridEntity_Stairs, lua::metatables::GridStairsMT>;
using LuaGridEntityGravity = LuabridgeRGONType<GridEntity_Gravity, lua::metatables::GridGravityMT>;
using LuaGridEntityStatue = LuabridgeRGONType<GridEntity_Statue, lua::metatables::GridStatueMT>;
using LuaGridEntityTeleporter = LuabridgeRGONType<GridEntity_Teleporter, lua::metatables::GridTeleporterMT>;