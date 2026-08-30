#include "IsaacRepentance.h"
#include "LuaCore.h"

template<typename T>
struct LuaClassName;

template<typename T, auto MT, auto CONST_MT>
struct LuabridgeType
{
    static bool IsUnderlyingType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TUSERDATA;
    }

    static T* Get(lua_State* L, int index)
    {
        return lua::GetLuabridgeUserdata<T*>(L, index, MT, LuaClassName<T>::Name);
    }

    static const T* GetConst(lua_State* L, int index)
    {
        return lua::GetLuabridgeUserdata<T*>(L, index, CONST_MT, LuaClassName<T>::Name);
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
        return lua::luabridge::UserdataValue<T>::place(L, lua::GetMetatableKey(MT));
    }

    static T* PlaceConst(lua_State* L)
    {
        return lua::luabridge::UserdataValue<T>::place(L, lua::GetMetatableKey(CONST_MT));
    }

    static void Push(lua_State* L, const T& value)
    {
        lua::luabridge::UserdataValue<T>::push(L, lua::GetMetatableKey(MT), value);
    }
    
    static void PushConst(lua_State* L, const T& value)
    {
        lua::luabridge::UserdataValue<T>::push(L, lua::GetMetatableKey(CONST_MT), value);
    }

    static void PushPtr(lua_State* L, T* ptr)
    {
        lua::luabridge::UserdataPtr::push(L, ptr, lua::GetMetatableKey(MT));
    }

    static void PushConstPtr(lua_State* L, T* ptr)
    {
        lua::luabridge::UserdataPtr::push(L, ptr, lua::GetMetatableKey(CONST_MT));
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
        return lua::GetCData<T*>(L, index, lua::ffi::CData[ID], LuaClassName<T>::Name);
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
};

template<>
struct LuaClassName<Vector>
{
    static constexpr const char* Name = "Vector";
};

template<>
struct LuaClassName<PosVel>
{
    static constexpr const char* Name = "PosVel";
};

template<>
struct LuaClassName<BitSet128>
{
    static constexpr const char* Name = "BitSet128";
};

template<>
struct LuaClassName<KColor>
{
    static constexpr const char* Name = "KColor";
};

template<>
struct LuaClassName<ColorMod>
{
    static constexpr const char* Name = "Color";
};

template<>
struct LuaClassName<ANM2>
{
    static constexpr const char* Name = "Sprite";
};

template<>
struct LuaClassName<Font>
{
    static constexpr const char* Name = "Font";
};

template<>
struct LuaClassName<FontSettings>
{
    static constexpr const char* Name = "FontRenderSettings";
};

template<>
struct LuaClassName<RNG>
{
    static constexpr const char* Name = "RNG";
};

template<>
struct LuaClassName<Music>
{
    static constexpr const char* Name = "MusicManager";
};

template<>
struct LuaClassName<SoundEffects>
{
    static constexpr const char* Name = "SFXManager";
};

template<>
struct LuaClassName<ItemConfig>
{
    static constexpr const char* Name = "ItemConfig";
};

template<>
struct LuaClassName<ItemConfig_Item>
{
    static constexpr const char* Name = "Item";
};

template<>
struct LuaClassName<ItemConfig_Card>
{
    static constexpr const char* Name = "Card";
};

template<>
struct LuaClassName<ItemConfig_Pill>
{
    static constexpr const char* Name = "PillEffect";
};

template<>
struct LuaClassName<ItemConfig_Costume>
{
    static constexpr const char* Name = "Costume";
};

template<>
struct LuaClassName<RoomConfig_Room>
{
    static constexpr const char* Name = "RoomConfigRoom";
};

template<>
struct LuaClassName<Seeds>
{
    static constexpr const char* Name = "Seeds";
};

template<>
struct LuaClassName<Game>
{
    static constexpr const char* Name = "Game";
};

template<>
struct LuaClassName<Level>
{
    static constexpr const char* Name = "Level";
};

template<>
struct LuaClassName<Room>
{
    static constexpr const char* Name = "Room";
};

template<>
struct LuaClassName<RoomDescriptor>
{
    static constexpr const char* Name = "RoomDescriptor";
};

template<>
struct LuaClassName<ItemPool>
{
    static constexpr const char* Name = "ItemPool";
};

template<>
struct LuaClassName<HUD>
{
    static constexpr const char* Name = "HUD";
};

template<>
struct LuaClassName<Entity>
{
    static constexpr const char* Name = "Entity";
};

template<>
struct LuaClassName<Entity_Player>
{
    static constexpr const char* Name = "EntityPlayer";
};

template<>
struct LuaClassName<Entity_Tear>
{
    static constexpr const char* Name = "EntityTear";
};

template<>
struct LuaClassName<Entity_Familiar>
{
    static constexpr const char* Name = "EntityFamiliar";
};

template<>
struct LuaClassName<Entity_Bomb>
{
    static constexpr const char* Name = "EntityBomb";
};

template<>
struct LuaClassName<Entity_Pickup>
{
    static constexpr const char* Name = "EntityPickup";
};

template<>
struct LuaClassName<Entity_Laser>
{
    static constexpr const char* Name = "EntityLaser";
};

template<>
struct LuaClassName<Entity_Knife>
{
    static constexpr const char* Name = "EntityKnife";
};

template<>
struct LuaClassName<Entity_Projectile>
{
    static constexpr const char* Name = "EntityProjectile";
};

template<>
struct LuaClassName<Entity_NPC>
{
    static constexpr const char* Name = "EntityNPC";
};

template<>
struct LuaClassName<Entity_Effect>
{
    static constexpr const char* Name = "EntityEffect";
};

template<>
struct LuaClassName<EntityRef>
{
    static constexpr const char* Name = "EntityRef";
};

template<>
struct LuaClassName<EntityPtr>
{
    static constexpr const char* Name = "EntityPtr";
};

template<>
struct LuaClassName<NPCAI_Pathfinder>
{
    static constexpr const char* Name = "Pathfinder";
};

template<>
struct LuaClassName<TearParams>
{
    static constexpr const char* Name = "TearParams";
};

template<>
struct LuaClassName<ProjectileParams>
{
    static constexpr const char* Name = "ProjectileParams";
};

template<>
struct LuaClassName<TemporaryEffects>
{
    static constexpr const char* Name = "TemporaryEffects";
};

template<>
struct LuaClassName<ActiveItemDesc>
{
    static constexpr const char* Name = "ActiveItemDesc";
};

template<>
struct LuaClassName<GridEntity>
{
    static constexpr const char* Name = "GridEntity";
};

template<>
struct LuaClassName<GridEntity_Rock>
{
    static constexpr const char* Name = "GridEntityRock";
};

template<>
struct LuaClassName<GridEntity_Pit>
{
    static constexpr const char* Name = "GridEntityPit";
};

template<>
struct LuaClassName<GridEntity_Spikes>
{
    static constexpr const char* Name = "GridEntitySpikes";
};

template<>
struct LuaClassName<GridEntity_TNT>
{
    static constexpr const char* Name = "GridEntityTNT";
};

template<>
struct LuaClassName<GridEntity_Poop>
{
    static constexpr const char* Name = "GridEntityPoop";
};

template<>
struct LuaClassName<GridEntity_Door>
{
    static constexpr const char* Name = "GridEntityDoor";
};

template<>
struct LuaClassName<GridEntity_PressurePlate>
{
    static constexpr const char* Name = "GridEntityPressurePlate";
};

template<>
struct LuaClassName<GridEntityDesc>
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
