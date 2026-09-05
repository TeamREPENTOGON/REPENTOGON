#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "MiscFunctions.h"

template<typename T>
struct LuaClassTraits;

template<typename T>
struct LuaArrayProxy
{
    size_t size = 0;
    T* data = nullptr;
};

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

        static void* try_checkudata(lua_State* L, int ud, const char* tname)
        {
            void* p = lua_touserdata(L, ud);
            if (p == NULL)
                return NULL;

            // if this fails nothing is pushed on the stack.
            if (!lua_getmetatable(L, ud))
                return NULL;

            lua_getfield(L, LUA_REGISTRYINDEX, tname);
            bool matches = lua_rawequal(L, -1, -2);
            lua_pop(L, 2);

            return matches ? p : NULL;
        }
    }

    class GetClassError
    {
        const char* expected = nullptr;
        int actualType = LUA_TNONE;

    public:
        GetClassError(const char* expected, int actualType)
            : expected(expected), actualType(actualType)
        {
        }

        GetClassError(const GetClassError& other) = default;
        GetClassError(GetClassError&& other) = default;

        std::string message() const
        {
            return REPENTOGON::StringConcat(expected, " expected, got ", lua_typename(NULL, actualType));
        }
    };
}

template<typename Traits>
struct LuabridgeType
{
private:
    static constexpr lua::Metatables MT = Traits::MT;
    static constexpr lua::Metatables CONST_MT = Traits::CONST_MT;
    using T = typename Traits::Type;

public:
    static bool IsUnderlyingType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TUSERDATA;
    }

    static T* Get(lua_State* L, int index)
    {
        return lua::GetLuabridgeUserdata<T*>(L, index, MT, Traits::Name);
    }

    static REPENTOGON::Result<T*, LuaClasses::GetClassError> TryGet(lua_State* L, int index)
    {
        std::optional<T*> p = lua::TestUserdata<T*>(L, index, MT);
        if (!p) {
            return REPENTOGON::err(LuaClasses::GetClassError(Traits::Name, lua_type(L, index)));
        }

        return REPENTOGON::ok(*p);
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
        new (Place(L)) T(value);
    }
    
    static void PushConst(lua_State* L, const T& value)
    {
        new (PlaceConst(L)) T(value);
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
        void* p = luaL_checkudata(L, index, MT);
        return lua::UserdataToData<T*>(p);
    }

    static REPENTOGON::Result<T*, LuaClasses::GetClassError> TryGet(lua_State* L, int index)
    {
        void* p = LuaClasses::detail::try_checkudata(L, index, MT);
        if (!p)
        {
            return REPENTOGON::err(LuaClasses::GetClassError(MT, lua_type(L, index)));
        }

        return REPENTOGON::ok(lua::UserdataToData<T*>(p));
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

// template for raw userdata representing the class as a Value.
template<typename T, const char*& MT>
struct LuaUserdataValue
{
public:
    static bool IsUnderlyingType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TUSERDATA;
    }

    static T* Get(lua_State* L, int index)
    {
        return lua::GetRawUserdata<T*>(L, index, MT);
    }

    static REPENTOGON::Result<T*, LuaClasses::GetClassError> TryGet(lua_State* L, int index)
    {
        void* ud = LuaClasses::detail::try_checkudata(L, index, MT);
        if (!ud)
        {
            return REPENTOGON::err(LuaClasses::GetClassError(MT, lua_type(L, index)));
        }

        return REPENTOGON::ok((T*)ud);
    }

    static T* GetOpt(lua_State* L, int index)
    {
        return !lua_isnoneornil(L, index) ? Get(L, index) : nullptr;
    }

    static T* Place(lua_State* L)
    {
        T* result = (T*)lua_newuserdata(L, sizeof(T));
        luaL_setmetatable(L, MT);
        return result;
    }

    static void Push(lua_State* L, const T& value)
    {
        new (Place(L)) T(value);
    }

    // we cannot push a pointer when using this template
};

template<typename T, const char*& MT, typename PtrType = T*>
struct LuaUserdataPtr
{
private:
    static constexpr bool IS_DEFAULT_PTR = std::is_same_v<PtrType, T*>;
    static T* get_value(const PtrType* ptr) {
        if constexpr (IS_DEFAULT_PTR) {
            return *ptr;
        }
        else {
            return ptr->get();
        }
    }
public:
    static bool IsUnderlyingType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TUSERDATA;
    }

    static T* Get(lua_State* L, int index)
    {
        PtrType* ptr = lua::GetRawUserdata<PtrType*>(L, index, MT);
        return get_value(ptr);
    }

    static REPENTOGON::Result<T*, LuaClasses::GetClassError> TryGet(lua_State* L, int index)
    {
        void* ud = LuaClasses::detail::try_checkudata(L, index, MT);
        if (!ud)
        {
            return REPENTOGON::err(LuaClasses::GetClassError(MT, lua_type(L, index)));
        }

        return REPENTOGON::ok(get_value((PtrType*)ud));
    }

    static T* GetOpt(lua_State* L, int index)
    {
        return !lua_isnoneornil(L, index) ? Get(L, index) : nullptr;
    }

    // we cannot push a value when using this type

    template<typename U = PtrType, std::enable_if_t<std::is_same_v<U, T*>, int> = 0>
    static void PushPtr(lua_State* L, T* ptr)
    {
        PtrType* result = (PtrType*)lua_newuserdata(L, sizeof(PtrType));
        *result = ptr;
        luaL_setmetatable(L, MT);
    }

    template <typename U = PtrType, std::enable_if_t<!std::is_same_v<U, T*>, int> = 0, class... Args>
    static void PushPtr(lua_State* L, Args&&... args)
    {
        PtrType* result = (PtrType*)lua_newuserdata(L, sizeof(PtrType));
        // perfect forward construction of pointer type
        new (result) PtrType(std::forward<Args>(args)...);
        luaL_setmetatable(L, MT);
    }
};

template<typename Traits>
struct CDataType
{
private:
    static constexpr lua::ffi::CDataID ID = Traits::C_DATA_ID;
    static constexpr lua::ffi::CDataID PTR_ID = Traits::C_DATA_PTR;
    using T = typename Traits::Type;

public:
    static bool IsUnderlyingType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TCDATA;
    }

    static T* Get(lua_State* L, int index)
    {
        return lua::GetCData<T*>(L, index, lua::ffi::CData[ID], Traits::Name);
    }

    static REPENTOGON::Result<T*, LuaClasses::GetClassError> TryGet(lua_State* L, int index)
    {
        void* p = lua::TestCData(L, index, ID);
        if (!p) {
            return REPENTOGON::err(LuaClasses::GetClassError(Traits::Name, lua_type(L, index)));
        }

        return REPENTOGON::ok(static_cast<T*>(p));
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

namespace LuaTraits
{
    struct LuaIntValues
    {
        static constexpr const char* Name = "intValues";
        using Type = LuaArrayProxy<int>;
        static constexpr lua::Metatables MT = lua::Metatables::INT_VALUES;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_INT_VALUES;
    };

    struct LuaVector
    {
        static constexpr const char* Name = "Vector";
        using Type = Vector;
        static constexpr lua::Metatables MT = lua::Metatables::VECTOR;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_VECTOR;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::VECTOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::VECTOR_PTR;
    };

    struct LuaVectorList
    {
        static constexpr const char* Name = "VectorList";
        using Type = std::vector<Vector>;
        static constexpr lua::Metatables MT = lua::Metatables::VECTOR_LIST;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_VECTOR_LIST;
    };

    struct LuaPosVel
    {
        static constexpr const char* Name = "PosVel";
        using Type = PosVel;
        static constexpr lua::Metatables MT = lua::Metatables::POS_VEL;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_POS_VEL;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::POS_VEL;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::POS_VEL_PTR;
    };

    struct LuaBitSet128
    {
        static constexpr const char* Name = "BitSet128";
        using Type = BitSet128;
        static constexpr lua::Metatables MT = lua::Metatables::BITSET_128;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_BITSET_128;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::BITSET_128;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::BITSET_128_PTR;
    };

    struct LuaKColor
    {
        static constexpr const char* Name = "KColor";
        using Type = KColor;
        static constexpr lua::Metatables MT = lua::Metatables::KCOLOR;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_KCOLOR;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::KCOLOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::KCOLOR_PTR;
    };

    struct LuaColor
    {
        static constexpr const char* Name = "Color";
        using Type = ColorMod;
        static constexpr lua::Metatables MT = lua::Metatables::COLOR;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_COLOR;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::COLOR;
        static constexpr lua::ffi::CDataID CONST_C_DATA_ID = lua::ffi::CDataID::CONST_COLOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::COLOR_PTR;
    };

    struct LuaSprite
    {
        static constexpr const char* Name = "Sprite";
        using Type = ANM2;
        static constexpr lua::Metatables MT = lua::Metatables::SPRITE;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_SPRITE;
        inline static void**& UserdataValueVftable = __ptr_UserdataValue_ANM2_vftable;
    };

    struct LuaFont
    {
        static constexpr const char* Name = "Font";
        using Type = Font;
        static constexpr lua::Metatables MT = lua::Metatables::FONT;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_FONT;
        // Needs custom value vftable
    };

    struct LuaFontRenderSettings
    {
        static constexpr const char* Name = "FontRenderSettings";
        using Type = FontSettings;
        static constexpr lua::Metatables MT = lua::Metatables::FONTRENDERSETTINGS;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_FONTRENDERSETTINGS;
    };

    struct LuaRNG
    {
        static constexpr const char* Name = "RNG";
        using Type = RNG;
        static constexpr lua::Metatables MT = lua::Metatables::RNG;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_RNG;
    };

    struct LuaMusicManager
    {
        static constexpr const char* Name = "MusicManager";
        using Type = Music;
        static constexpr lua::Metatables MT = lua::Metatables::MUSIC_MANAGER;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_MUSIC_MANAGER;
    };

    struct LuaSFXManager
    {
        static constexpr const char* Name = "SFXManager";
        using Type = SoundEffects;
        static constexpr lua::Metatables MT = lua::Metatables::SFX_MANAGER;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_SFX_MANAGER;
    };

    struct LuaItemConfig
    {
        static constexpr const char* Name = "ItemConfig";
        using Type = ItemConfig;
        static constexpr lua::Metatables MT = lua::Metatables::CONFIG;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_CONFIG;
    };

    struct LuaItem
    {
        static constexpr const char* Name = "Item";
        using Type = ItemConfig_Item;
        static constexpr lua::Metatables MT = lua::Metatables::ITEM;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ITEM;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ITEM;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ITEM_PTR;
    };

    struct LuaCard
    {
        static constexpr const char* Name = "Card";
        using Type = ItemConfig_Card;
        static constexpr lua::Metatables MT = lua::Metatables::CARD;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_CARD;
    };

    struct LuaPillEffect
    {
        static constexpr const char* Name = "PillEffect";
        using Type = ItemConfig_Pill;
        static constexpr lua::Metatables MT = lua::Metatables::PILL_EFFECT;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_PILL_EFFECT;
    };

    struct LuaCostume
    {
        static constexpr const char* Name = "Costume";
        using Type = ItemConfig_Costume;
        static constexpr lua::Metatables MT = lua::Metatables::COSTUME;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_COSTUME;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::COSTUME;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::COSTUME_PTR;
    };

    struct LuaRoomConfigRoom
    {
        static constexpr const char* Name = "RoomConfigRoom";
        using Type = RoomConfig_Room;
        static constexpr lua::Metatables MT = lua::Metatables::ROOM_CONFIG_ROOM;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ROOM_CONFIG_ROOM;
    };

    struct LuaSpawn
    {
        static constexpr const char* Name = "Spawn";
        using Type = RoomSpawn;
        static constexpr lua::Metatables MT = lua::Metatables::SPAWN;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_SPAWN;
    };

    struct LuaRoomConfigSpawns
    {
        static constexpr const char* Name = "RoomConfigSpawns";
        using Type = LuaArrayProxy<RoomSpawn>;
        static constexpr lua::Metatables MT = lua::Metatables::ROOM_CONFIG_SPAWNS;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ROOM_CONFIG_SPAWNS;
    };

    struct LuaEntry
    {
        static constexpr const char* Name = "Entry";
        using Type = RoomEntry;
        static constexpr lua::Metatables MT = lua::Metatables::ENTRY;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTRY;
    };

    struct LuaRoomConfigEntries
    {
        static constexpr const char* Name = "RoomConfigEntries";
        using Type = LuaArrayProxy<RoomEntry>;
        static constexpr lua::Metatables MT = lua::Metatables::ROOM_CONFIG_ENTRIES;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ROOM_CONFIG_ENTRIES;
    };

    struct LuaSeeds
    {
        static constexpr const char* Name = "Seeds";
        using Type = Seeds;
        static constexpr lua::Metatables MT = lua::Metatables::SEEDS;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_SEEDS;
    };

    struct LuaGame
    {
        static constexpr const char* Name = "Game";
        using Type = Game;
        static constexpr lua::Metatables MT = lua::Metatables::GAME;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_GAME;
    };

    struct LuaLevel
    {
        static constexpr const char* Name = "Level";
        using Type = Level;
        static constexpr lua::Metatables MT = lua::Metatables::LEVEL;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_LEVEL;
    };

    struct LuaRoom
    {
        static constexpr const char* Name = "Room";
        using Type = Room;
        static constexpr lua::Metatables MT = lua::Metatables::ROOM;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ROOM;
    };

    struct LuaRoomDescriptor
    {
        static constexpr const char* Name = "RoomDescriptor";
        using Type = RoomDescriptor;
        static constexpr lua::Metatables MT = lua::Metatables::ROOM_DESCRIPTOR;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ROOM_DESCRIPTOR;
    };

    struct LuaRoomDescriptorList
    {
        static constexpr const char* Name = "RoomDescriptor";
        using Type = LuaArrayProxy<RoomDescriptor>;
        static constexpr lua::Metatables MT = lua::Metatables::ARRAY_PROXY_ROOM_DESCRIPTOR;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ARRAY_PROXY_ROOM_DESCRIPTOR;
    };

    struct LuaItemPool
    {
        static constexpr const char* Name = "ItemPool";
        using Type = ItemPool;
        static constexpr lua::Metatables MT = lua::Metatables::ITEM_POOL;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ITEM_POOL;
    };

    struct LuaHUD
    {
        static constexpr const char* Name = "HUD";
        using Type = HUD;
        static constexpr lua::Metatables MT = lua::Metatables::HUD;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_HUD;
    };

    struct LuaEntity
    {
        static constexpr const char* Name = "Entity";
        using Type = Entity;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY;
    };

    struct LuaEntityPlayer
    {
        static constexpr const char* Name = "EntityPlayer";
        using Type = Entity_Player;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_PLAYER;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_PLAYER;
    };

    struct LuaEntityTear
    {
        static constexpr const char* Name = "EntityTear";
        using Type = Entity_Tear;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_TEAR;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_TEAR;
    };

    struct LuaEntityFamiliar
    {
        static constexpr const char* Name = "EntityFamiliar";
        using Type = Entity_Familiar;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_FAMILIAR;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_FAMILIAR;
    };

    struct LuaEntityBomb
    {
        static constexpr const char* Name = "EntityBomb";
        using Type = Entity_Bomb;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_BOMB;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_BOMB;
    };

    struct LuaEntityPickup
    {
        static constexpr const char* Name = "EntityPickup";
        using Type = Entity_Pickup;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_PICKUP;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_PICKUP;
    };

    struct LuaEntityLaser
    {
        static constexpr const char* Name = "EntityLaser";
        using Type = Entity_Laser;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_LASER;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_LASER;
    };

    struct LuaEntityKnife
    {
        static constexpr const char* Name = "EntityKnife";
        using Type = Entity_Knife;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_KNIFE;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_KNIFE;
    };

    struct LuaEntityProjectile
    {
        static constexpr const char* Name = "EntityProjectile";
        using Type = Entity_Projectile;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_PROJECTILE;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_PROJECTILE;
    };

    struct LuaEntityNPC
    {
        static constexpr const char* Name = "EntityNPC";
        using Type = Entity_NPC;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_NPC;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_NPC;
    };

    struct LuaEntityEffect
    {
        static constexpr const char* Name = "EntityEffect";
        using Type = Entity_Effect;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_EFFECT;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_EFFECT;
    };

    struct LuaEntityRef
    {
        static constexpr const char* Name = "EntityRef";
        using Type = EntityRef;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_REF;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_REF;
    };

    struct LuaEntityPtr
    {
        static constexpr const char* Name = "EntityPtr";
        using Type = EntityPtr;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_PTR;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_PTR;
        // Needs custom value vftable
    };

    struct LuaEntityList
    {
        static constexpr const char* Name = "EntityList";
        using Type = EntityList_EL;
        static constexpr lua::Metatables MT = lua::Metatables::ENTITY_LIST;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ENTITY_LIST;
        // Needs custom value vftable
    };

    struct LuaPathfinder
    {
        static constexpr const char* Name = "Pathfinder";
        using Type = NPCAI_Pathfinder;
        static constexpr lua::Metatables MT = lua::Metatables::PATHFINDER;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_PATHFINDER;
        // Needs custom value vftable
    };

    struct LuaTearParams
    {
        static constexpr const char* Name = "TearParams";
        using Type = TearParams;
        static constexpr lua::Metatables MT = lua::Metatables::TEAR_PARAMS;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_TEAR_PARAMS;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::TEAR_PARAMS;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::TEAR_PARAMS_PTR;
    };

    struct LuaProjectileParams
    {
        static constexpr const char* Name = "ProjectileParams";
        using Type = ProjectileParams;
        static constexpr lua::Metatables MT = lua::Metatables::PROJECTILE_PARAMS;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_PROJECTILE_PARAMS;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::PROJECTILE_PARAMS;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::PROJECTILE_PARAMS_PTR;
    };

    struct LuaTemporaryEffects
    {
        static constexpr const char* Name = "TemporaryEffects";
        using Type = TemporaryEffects;
        static constexpr lua::Metatables MT = lua::Metatables::_TEMPORARY_EFFECTS;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::_CONST_TEMPORARY_EFFECTS;
    };

    struct LuaTemporaryEffect
    {
        static constexpr const char* Name = "TemporaryEffect";
        using Type = TemporaryEffect;
        static constexpr lua::Metatables MT = lua::Metatables::TEMPORARY_EFFECT;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_TEMPORARY_EFFECT;
    };

    struct LuaEffectList
    {
        static constexpr const char* Name = "EffectList";
        using Type = std::vector<TemporaryEffect>;
        static constexpr lua::Metatables MT = lua::Metatables::EFFECT_LIST;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_EFFECT_LIST;
    };

    struct LuaActiveItemDesc
    {
        static constexpr const char* Name = "ActiveItemDesc";
        using Type = ActiveItemDesc;
        static constexpr lua::Metatables MT = lua::Metatables::ACTIVE_ITEM_DESC;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ACTIVE_ITEM_DESC;
    };

    struct LuaQueueItemData
    {
        static constexpr const char* Name = "QueueItemData";
        using Type = QueueItemData;
        static constexpr lua::Metatables MT = lua::Metatables::QUEUE_ITEM_DATA;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_QUEUE_ITEM_DATA;
    };

    struct LuaGridEntity
    {
        static constexpr const char* Name = "GridEntity";
        using Type = GridEntity;
        static constexpr lua::Metatables MT = lua::Metatables::GRID_ENTITY;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_GRID_ENTITY;
    };

    struct LuaGridEntityRock
    {
        static constexpr const char* Name = "GridEntityRock";
        using Type = GridEntity_Rock;
        static constexpr lua::Metatables MT = lua::Metatables::GRID_ENTITY_ROCK;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_GRID_ENTITY_ROCK;
    };

    struct LuaGridEntityPit
    {
        static constexpr const char* Name = "GridEntityPit";
        using Type = GridEntity_Pit;
        static constexpr lua::Metatables MT = lua::Metatables::GRID_ENTITY_PIT;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_GRID_ENTITY_PIT;
    };

    struct LuaGridEntitySpikes
    {
        static constexpr const char* Name = "GridEntitySpikes";
        using Type = GridEntity_Spikes;
        static constexpr lua::Metatables MT = lua::Metatables::GRID_ENTITY_SPIKES;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_GRID_ENTITY_SPIKES;
    };

    struct LuaGridEntityTNT
    {
        static constexpr const char* Name = "GridEntityTNT";
        using Type = GridEntity_TNT;
        static constexpr lua::Metatables MT = lua::Metatables::GRID_ENTITY_TNT;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_GRID_ENTITY_TNT;
    };

    struct LuaGridEntityPoop
    {
        static constexpr const char* Name = "GridEntityPoop";
        using Type = GridEntity_Poop;
        static constexpr lua::Metatables MT = lua::Metatables::GRID_ENTITY_POOP;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_GRID_ENTITY_POOP;
    };

    struct LuaGridEntityDoor
    {
        static constexpr const char* Name = "GridEntityDoor";
        using Type = GridEntity_Door;
        static constexpr lua::Metatables MT = lua::Metatables::GRID_ENTITY_DOOR;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_GRID_ENTITY_DOOR;
    };

    struct LuaGridEntityPressurePlate
    {
        static constexpr const char* Name = "GridEntityPressurePlate";
        using Type = GridEntity_PressurePlate;
        static constexpr lua::Metatables MT = lua::Metatables::GRID_ENTITY_PRESSURE_PLATE;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_GRID_ENTITY_PRESSURE_PLATE;
    };

    struct LuaGridEntityDesc
    {
        static constexpr const char* Name = "GridEntityDesc";
        using Type = GridEntityDesc;
        static constexpr lua::Metatables MT = lua::Metatables::GRID_ENTITY_DESC;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_GRID_ENTITY_DESC;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_DESC;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_DESC_PTR;
    };
}

using LuaIntValues = LuabridgeType<LuaTraits::LuaIntValues>;
using LuaVector = CDataType<LuaTraits::LuaVector>;
using LuaVectorList = LuabridgeType<LuaTraits::LuaVectorList>;
using LuaPosVel = CDataType<LuaTraits::LuaPosVel>;
using LuaBitSet128 = CDataType<LuaTraits::LuaBitSet128>;
using LuaKColor = CDataType<LuaTraits::LuaKColor>;
using LuaColor = CDataType<LuaTraits::LuaColor>;
using LuaSprite = LuabridgeType<LuaTraits::LuaSprite>;
using LuaFont = LuabridgeType<LuaTraits::LuaFont>;
using LuaFontRenderSettings = LuabridgeType<LuaTraits::LuaFontRenderSettings>;
using LuaRNG = LuabridgeType<LuaTraits::LuaRNG>;
using LuaMusicManager = LuabridgeType<LuaTraits::LuaMusicManager>;
using LuaSFXManager = LuabridgeType<LuaTraits::LuaSFXManager>;
using LuaItemConfig = LuabridgeType<LuaTraits::LuaItemConfig>;
using LuaItem = CDataType<LuaTraits::LuaItem>;
using LuaCard = LuabridgeType<LuaTraits::LuaCard>;
using LuaPillEffect = LuabridgeType<LuaTraits::LuaPillEffect>;
using LuaCostume = CDataType<LuaTraits::LuaCostume>;
using LuaRoomConfigRoom = LuabridgeType<LuaTraits::LuaRoomConfigRoom>;
using LuaSpawn = LuabridgeType<LuaTraits::LuaSpawn>;
using LuaRoomConfigSpawns = LuabridgeType<LuaTraits::LuaRoomConfigSpawns>;
using LuaEntry = LuabridgeType<LuaTraits::LuaEntry>;
using LuaRoomConfigEntries = LuabridgeType<LuaTraits::LuaRoomConfigEntries>;
using LuaSeeds = LuabridgeType<LuaTraits::LuaSeeds>;
using LuaGame = LuabridgeType<LuaTraits::LuaGame>;
using LuaLevel = LuabridgeType<LuaTraits::LuaLevel>;
using LuaRoom = LuabridgeType<LuaTraits::LuaRoom>;
using LuaRoomDescriptor = LuabridgeType<LuaTraits::LuaRoomDescriptor>;
using LuaRoomDescriptorList = LuabridgeType<LuaTraits::LuaRoomDescriptorList>;
using LuaItemPool = LuabridgeType<LuaTraits::LuaItemPool>;
using LuaHUD = LuabridgeType<LuaTraits::LuaHUD>;
using LuaEntity = LuabridgeType<LuaTraits::LuaEntity>;
using LuaEntityPlayer = LuabridgeType<LuaTraits::LuaEntityPlayer>;
using LuaEntityTear = LuabridgeType<LuaTraits::LuaEntityTear>;
using LuaEntityFamiliar = LuabridgeType<LuaTraits::LuaEntityFamiliar>;
using LuaEntityBomb = LuabridgeType<LuaTraits::LuaEntityBomb>;
using LuaEntityPickup = LuabridgeType<LuaTraits::LuaEntityPickup>;
using LuaEntityLaser = LuabridgeType<LuaTraits::LuaEntityLaser>;
using LuaEntityKnife = LuabridgeType<LuaTraits::LuaEntityKnife>;
using LuaEntityProjectile = LuabridgeType<LuaTraits::LuaEntityProjectile>;
using LuaEntityNPC = LuabridgeType<LuaTraits::LuaEntityNPC>;
using LuaEntityEffect = LuabridgeType<LuaTraits::LuaEntityEffect>;
using LuaEntityRef = LuabridgeType<LuaTraits::LuaEntityRef>;
using LuaEntityPtr = LuabridgeType<LuaTraits::LuaEntityPtr>;
using LuaEntityList = LuabridgeType<LuaTraits::LuaEntityList>;
using LuaPathfinder = LuabridgeType<LuaTraits::LuaPathfinder>;
using LuaTearParams = CDataType<LuaTraits::LuaTearParams>;
using LuaProjectileParams = CDataType<LuaTraits::LuaProjectileParams>;
using LuaTemporaryEffects = LuabridgeType<LuaTraits::LuaTemporaryEffects>;
using LuaTemporaryEffect = LuabridgeType<LuaTraits::LuaTemporaryEffect>;
using LuaEffectList = LuabridgeType<LuaTraits::LuaEffectList>;
using LuaActiveItemDesc = LuabridgeType<LuaTraits::LuaActiveItemDesc>;
using LuaQueueItemData = LuabridgeType<LuaTraits::LuaQueueItemData>;
using LuaGridEntity = LuabridgeType<LuaTraits::LuaGridEntity>;
using LuaGridEntityRock = LuabridgeType<LuaTraits::LuaGridEntityRock>;
using LuaGridEntityPit = LuabridgeType<LuaTraits::LuaGridEntityPit>;
using LuaGridEntitySpikes = LuabridgeType<LuaTraits::LuaGridEntitySpikes>;
using LuaGridEntityTNT = LuabridgeType<LuaTraits::LuaGridEntityTNT>;
using LuaGridEntityPoop = LuabridgeType<LuaTraits::LuaGridEntityPoop>;
using LuaGridEntityDoor = LuabridgeType<LuaTraits::LuaGridEntityDoor>;
using LuaGridEntityPressurePlate = LuabridgeType<LuaTraits::LuaGridEntityPressurePlate>;
using LuaGridEntityDesc = CDataType<LuaTraits::LuaGridEntityDesc>;

struct WeaponData {
	Weapon* weapon;
	// Arbitrarily default to EntityPlayer here. This thing is super unsafe and the API
	// should be tweaked to prevent crashes.
	std::variant<Entity_Player*, Entity_Familiar*> owner = (Entity_Player*)nullptr;
	int8_t slot = -1;

    WeaponData() = default;
    WeaponData(Weapon* weapon, Entity* owner)
    {
        this->weapon = weapon;

        if (Entity_Familiar* familiar = owner->ToFamiliar()) {
            this->owner = familiar;
        }
        else if (Entity_Player* player = owner->ToPlayer()) {
            this->owner = player;
            for (int i = 0; i < 4; ++i) {
                if (*(player->GetWeapon(i)) == weapon) {
                    this->slot = i;
                    break;
                }
            }
        }
    }

    Weapon* get() const { return this->weapon; }
	static Weapon* GetWeaponFromLua(lua_State* L, int idx);
};

struct LuaLevelGeneratorRoomData {
	LevelGenerator* context = nullptr;
	LevelGenerator_Room* room = nullptr;
	bool isValue = false;

    LevelGenerator_Room* get() const { return this->room; }
};

// RGON Classes

using LuaHistoryHUD = LuaUserdataPtr<HistoryHUD, lua::metatables::HistoryHUDMT>;
using LuaLevelGenerator = LuaUserdataPtr<LevelGenerator, lua::metatables::LevelGeneratorMT>;
struct LuaLevelGeneratorRoom; // forward declaration
using LuaBossPool = LuaUserdataPtr<BossPool_Pool, lua::metatables::BossPoolMT>;
using LuaEntitySlot = LuabridgeRGONType<Entity_Slot, lua::metatables::EntitySlotMT>;
using LuaEntityDelirium = LuabridgeRGONType<Entity_NPC, lua::metatables::DeliriumMetatable>;
using LuaWeapon = LuaUserdataPtr<Weapon, lua::metatables::WeaponMT, WeaponData>;
using LuaMultiShotParams = LuaUserdataValue<Weapon_MultiShotParams, lua::metatables::MultiShotParamsMT>;
using LuaLootList = LuaUserdataValue<LootList, lua::metatables::LootListMT>;
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

struct LuaLevelGeneratorRoom
{
private:
    static constexpr int UNDERLYING_TYPE = LUA_TUSERDATA;
    inline static const char*& MT = lua::metatables::LevelGeneratorRoomMT;
    using DataType = LuaLevelGeneratorRoomData;
    using T = LevelGenerator_Room;

public:
    static bool IsUnderlyingType(lua_State* L, int index)
    {
        return lua_type(L, index) == UNDERLYING_TYPE;
    }

    static LevelGenerator_Room* Get(lua_State* L, int index)
    {
        DataType* data = lua::GetRawUserdata<DataType*>(L, index, MT);
        return data->get();
    }

    static REPENTOGON::Result<T*, LuaClasses::GetClassError> TryGet(lua_State* L, int index)
    {
        DataType* ud = (DataType*)LuaClasses::detail::try_checkudata(L, index, MT);
        if (!ud)
        {
            return REPENTOGON::err(LuaClasses::GetClassError(MT, lua_type(L, index)));
        }

        return REPENTOGON::ok(ud->get());
    }

    static T* GetOpt(lua_State* L, int index)
    {
        return !lua_isnoneornil(L, index) ? Get(L, index) : nullptr;
    }

    // we cannot push a value when using this type

    static void PushPtr(lua_State* L, LevelGenerator* context, LevelGenerator_Room* room)
    {
        DataType* result = (DataType*)lua_newuserdata(L, sizeof(DataType));
        result->isValue = false;
        result->context = context;
        result->room = room;
        luaL_setmetatable(L, MT);
    }
};