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
        if (void* pp = lua::TestCData(L, index, lua::ffi::CData[PTR_ID])) {
            return REPENTOGON::ok(*static_cast<T**>(pp));
        }
        if (void* pv = lua::TestCData(L, index, lua::ffi::CData[ID])) {
            return REPENTOGON::ok(static_cast<T*>(pv));
        }
        return REPENTOGON::err(LuaClasses::GetClassError(Traits::Name, lua_type(L, index)));
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

struct Lua_EntitySaveState {
    std::vector<EntitySaveState>* vec;
    int index;

    EntitySaveState& Get() const { return (*vec)[index]; }
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
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::VECTOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::VECTOR_PTR;
    };

    struct LuaPosVel
    {
        static constexpr const char* Name = "PosVel";
        using Type = PosVel;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::POS_VEL;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::POS_VEL_PTR;
    };

    struct LuaBitSet128
    {
        static constexpr const char* Name = "BitSet128";
        using Type = BitSet128;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::BITSET_128;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::BITSET_128_PTR;
    };

    struct LuaKColor
    {
        static constexpr const char* Name = "KColor";
        using Type = KColor;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::KCOLOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::KCOLOR_PTR;
    };

    struct LuaColor
    {
        static constexpr const char* Name = "Color";
        using Type = ColorMod;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::COLOR;
        static constexpr lua::ffi::CDataID CONST_C_DATA_ID = lua::ffi::CDataID::CONST_COLOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::COLOR_PTR;
    };

    struct LuaSprite
    {
        static constexpr const char* Name = "Sprite";
        using Type = ANM2;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::SPRITE;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::SPRITE_PTR;
    };

    struct LuaFont
    {
        static constexpr const char* Name = "Font";
        using Type = Font;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::FONT;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::FONT_PTR;
    };

    struct LuaFontRenderSettings
    {
        static constexpr const char* Name = "FontRenderSettings";
        using Type = FontSettings;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::FONT_RENDER_SETTINGS;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::FONT_RENDER_SETTINGS_PTR;
    };

    struct LuaRNG
    {
        static constexpr const char* Name = "RNG";
        using Type = RNG;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::RNG;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::RNG_PTR;
    };

    struct LuaItemConfig
    {
        static constexpr const char* Name = "ItemConfig";
        using Type = ItemConfig;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ITEM_CONFIG;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ITEM_CONFIG_PTR;
    };

    struct LuaItem
    {
        static constexpr const char* Name = "Item";
        using Type = ItemConfig_Item;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ITEM;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ITEM_PTR;
    };

    struct LuaCard
    {
        static constexpr const char* Name = "Card";
        using Type = ItemConfig_Card;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ITEM_CONFIG_CARD;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ITEM_CONFIG_CARD_PTR;
    };

    struct LuaPillEffect
    {
        static constexpr const char* Name = "PillEffect";
        using Type = ItemConfig_Pill;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ITEM_CONFIG_PILL_EFFECT;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ITEM_CONFIG_PILL_EFFECT_PTR;
    };

    struct LuaCostume
    {
        static constexpr const char* Name = "Costume";
        using Type = ItemConfig_Costume;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::COSTUME;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::COSTUME_PTR;
    };

    struct LuaRoomConfigRoom
    {
        static constexpr const char* Name = "RoomConfigRoom";
        using Type = RoomConfig_Room;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ROOM_CONFIG_ROOM;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ROOM_CONFIG_ROOM_PTR;
    };

    struct LuaSeeds
    {
        static constexpr const char* Name = "Seeds";
        using Type = Seeds;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::SEEDS;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::SEEDS_PTR;
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
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ROOM;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ROOM_PTR;
    };

    struct LuaRoomDescriptor
    {
        static constexpr const char* Name = "RoomDescriptor";
        using Type = RoomDescriptor;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ROOM_DESCRIPTOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ROOM_DESCRIPTOR_PTR;
    };

    struct LuaRoomDescriptorList
    {
        static constexpr const char* Name = "RoomDescriptorList";
        using Type = LuaArrayProxy<RoomDescriptor>;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ROOM_DESCRIPTOR_LIST;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ROOM_DESCRIPTOR_LIST_PTR;
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
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::HUD;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::HUD_PTR;
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
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ENTITY_REF;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ENTITY_REF_PTR;
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
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::TEAR_PARAMS;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::TEAR_PARAMS_PTR;
    };

    struct LuaProjectileParams
    {
        static constexpr const char* Name = "ProjectileParams";
        using Type = ProjectileParams;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::PROJECTILE_PARAMS;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::PROJECTILE_PARAMS_PTR;
    };

    struct LuaActiveItemDesc
    {
        static constexpr const char* Name = "ActiveItemDesc";
        using Type = ActiveItemDesc;
        static constexpr lua::Metatables MT = lua::Metatables::ACTIVE_ITEM_DESC;
        static constexpr lua::Metatables CONST_MT = lua::Metatables::CONST_ACTIVE_ITEM_DESC;
    };

    struct LuaGridEntity
    {
        static constexpr const char* Name = "GridEntity";
        using Type = GridEntity;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_PTR;
    };

    struct LuaGridEntityRock
    {
        static constexpr const char* Name = "GridEntityRock";
        using Type = GridEntity_Rock;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_ROCK;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_ROCK_PTR;
    };

    struct LuaGridEntityPit
    {
        static constexpr const char* Name = "GridEntityPit";
        using Type = GridEntity_Pit;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_PIT;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_PIT_PTR;
    };

    struct LuaGridEntitySpikes
    {
        static constexpr const char* Name = "GridEntitySpikes";
        using Type = GridEntity_Spikes;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_SPIKES;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_SPIKES_PTR;
    };

    struct LuaGridEntityTNT
    {
        static constexpr const char* Name = "GridEntityTNT";
        using Type = GridEntity_TNT;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_TNT;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_TNT_PTR;
    };

    struct LuaGridEntityPoop
    {
        static constexpr const char* Name = "GridEntityPoop";
        using Type = GridEntity_Poop;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_POOP;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_POOP_PTR;
    };

    struct LuaGridEntityDoor
    {
        static constexpr const char* Name = "GridEntityDoor";
        using Type = GridEntity_Door;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_DOOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_DOOR_PTR;
    };

    struct LuaGridEntityPressurePlate
    {
        static constexpr const char* Name = "GridEntityPressurePlate";
        using Type = GridEntity_PressurePlate;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_PRESSURE_PLATE;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_PRESSURE_PLATE_PTR;
    };

    struct LuaGridEntityDecoration
    {
        static constexpr const char* Name = "GridEntityDecoration";
        using Type = GridEntity_Decoration;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_DECORATION;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_DECORATION_PTR;
    };

    struct LuaGridEntityWeb
    {
        static constexpr const char* Name = "GridEntityWeb";
        using Type = GridEntity_Web;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_WEB;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_WEB_PTR;
    };

    struct LuaGridEntityLock
    {
        static constexpr const char* Name = "GridEntityLock";
        using Type = GridEntity_Lock;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_LOCK;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_LOCK_PTR;
    };

    struct LuaGridEntityFire
    {
        static constexpr const char* Name = "GridEntityFire";
        using Type = GridEntity_Fire;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_FIRE;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_FIRE_PTR;
    };

    struct LuaGridEntityWall
    {
        static constexpr const char* Name = "GridEntityWall";
        using Type = GridEntity_Wall;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_WALL;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_WALL_PTR;
    };

    struct LuaGridEntityTrapDoor
    {
        static constexpr const char* Name = "GridEntityTrapDoor";
        using Type = GridEntity_TrapDoor;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_TRAP_DOOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_TRAP_DOOR_PTR;
    };

    struct LuaGridEntityStairs
    {
        static constexpr const char* Name = "GridEntityStairs";
        using Type = GridEntity_Stairs;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_STAIRS;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_STAIRS_PTR;
    };

    struct LuaGridEntityGravity
    {
        static constexpr const char* Name = "GridEntityGravity";
        using Type = GridEntity_Gravity;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_GRAVITY;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_GRAVITY_PTR;
    };

    struct LuaGridEntityStatue
    {
        static constexpr const char* Name = "GridEntityStatue";
        using Type = GridEntity_Statue;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_STATUE;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_STATUE_PTR;
    };

    struct LuaGridEntityTeleporter
    {
        static constexpr const char* Name = "GridEntityTeleporter";
        using Type = GridEntity_Teleporter;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_TELEPORTER;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_TELEPORTER_PTR;
    };

    struct LuaGridEntityDesc
    {
        static constexpr const char* Name = "GridEntityDesc";
        using Type = GridEntityDesc;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITY_DESC;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITY_DESC_PTR;
    };

    struct LuaBlendMode
    {
        static constexpr const char* Name = "BlendMode";
        using Type = BlendMode;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::BLEND_MODE;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::BLEND_MODE_PTR;
    };

    struct LuaColorModifier
    {
        static constexpr const char* Name = "ColorModifier";
        using Type = ColorModState;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::COLOR_MODIFIER;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::COLOR_MODIFIER_PTR;
    };

    struct LuaEntityDesc
    {
        static constexpr const char* Name = "EntityDesc";
        using Type = EntityDesc;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ENTITY_DESC;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ENTITY_DESC_PTR;
    };

    struct LuaChallengeParam
    {
        static constexpr const char* Name = "ChallengeParam";
        using Type = ChallengeParam;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::CHALLENGE_PARAM;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::CHALLENGE_PARAM_PTR;
    };

    struct LuaCapsule
    {
        static constexpr const char* Name = "Capsule";
        using Type = Capsule;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::CAPSULE;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::CAPSULE_PTR;
    };

    struct LuaShape
    {
        static constexpr const char* Name = "Shape";
        using Type = Shape;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::SHAPE;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::SHAPE_PTR;
    };

    struct LuaColorParams
    {
        static constexpr const char* Name = "ColorParams";
        using Type = ColorParams;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::COLOR_PARAMS;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::COLOR_PARAMS_PTR;
    };

    struct LuaLootList
    {
        static constexpr const char* Name = "LootList";
        using Type = LootList;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::LOOT_LIST;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::LOOT_LIST_PTR;
    };
    
    struct LuaWeightedOutcomePicker
    {
        static constexpr const char* Name = "WeightedOutcomePicker";
        using type = WeightedOutcomePicker;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::WEIGHTED_OUTCOME_PICKER;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::WEIGHTED_OUTCOME_PICKER_PTR;
    };
    
    struct LuaMultiShotParams
    {
        static constexpr const char* Name = "MultiShotParams";
        using Type = Weapon_MultiShotParams;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::MULTI_SHOT_PARAMS;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::MULTI_SHOT_PARAMS_PTR;
    };

    struct LuaHistory
    {
        static constexpr const char* Name = "History";
        using Type = History;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::HISTORY;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::HISTORY_PTR;
    };

    struct LuaHistoryItem
    {
        static constexpr const char* Name = "HistoryItem";
        using Type = History_HistoryItem;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::HISTORY_ITEM;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::HISTORY_ITEM_PTR;
    };

    struct LuaPocketItem
    {
        static constexpr const char* Name = "PocketItem";
        using Type = PocketItem;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::POCKET_ITEM;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::POCKET_ITEM_PTR;
    };

    struct LuaLevelGenerator
    {
        static constexpr const char* Name = "LevelGenerator";
        using Type = LevelGenerator;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::LEVEL_GENERATOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::LEVEL_GENERATOR_PTR;
    };

    struct LuaLevelGeneratorRoom
    {
        static constexpr const char* Name = "LevelGeneratorRoom";
        using Type = LevelGenerator_Room;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::LEVEL_GENERATOR_ROOM;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::LEVEL_GENERATOR_ROOM_PTR;
    };

    struct LuaLevelGeneratorEntry
    {
        static constexpr const char* Name = "LevelGeneratorEntry";
        using Type = LevelGenerator_Room;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::LEVEL_GENERATOR_ENTRY;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::LEVEL_GENERATOR_ENTRY_PTR;
    };

    struct LuaBossPool
    {
        static constexpr const char* Name = "BossPool";
        using Type = BossPool_Pool;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::BOSS_POOL;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::BOSS_POOL_PTR;
    };

    struct LuaCostumeSpriteDesc
    {
        static constexpr const char* Name = "CostumeSpriteDesc";
        using Type = CostumeSpriteDesc;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::COSTUME_SPRITE_DESC;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::COSTUME_SPRITE_DESC_PTR;
    };

    struct LuaGenericPrompt
    {
        static constexpr const char* Name = "GenericPrompt";
        using Type = GenericPrompt;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GENERIC_PROMPT;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GENERIC_PROMPT_PTR;
    };

    struct LuaPlayerHUD
    {
        static constexpr const char* Name = "PlayerHUD";
        using Type = PlayerHUD;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::PLAYER_HUD;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::PLAYER_HUD_PTR;
    };

    struct LuaHistoryHUD
    {
        static constexpr const char* Name = "HistoryHUD";
        using Type = HistoryHUD;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::HISTORY_HUD;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::HISTORY_HUD_PTR;
    };

    struct LuaWeapon
    {
        static constexpr const char* Name = "Weapon";
        using Type = Weapon;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::WEAPON;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::WEAPON_PTR;
    };

    struct LuaEntitySaveState
    {
        static constexpr const char* Name = "EntitySaveState";
        using Type = Lua_EntitySaveState;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ENTITY_SAVE_STATE;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ENTITY_SAVE_STATE_PTR;
    };

    struct LuaEntitiesSaveStateVector
    {
        static constexpr const char* Name = "EntitiesSaveStateVector";
        using Type = std::vector<EntitySaveState>;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ENTITIES_SAVE_STATE_VECTOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ENTITIES_SAVE_STATE_VECTOR_PTR;
    };

    struct LuaGridEntitiesSaveStateVector
    {
        static constexpr const char* Name = "GridEntitiesSaveStateVector";
        using Type = std::vector<GridEntityDesc>;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::GRID_ENTITIES_SAVE_STATE_VECTOR;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::GRID_ENTITIES_SAVE_STATE_VECTOR_PTR;
    };

    struct LuaEntityConfigEntity
    {
        static constexpr const char* Name = "EntityConfigEntity";
        using Type = EntityConfig_Entity;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ENTITY_CONFIG_ENTITY;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ENTITY_CONFIG_ENTITY_PTR;
    };

    struct LuaEntityConfigPlayer
    {
        static constexpr const char* Name = "EntityConfigPlayer";
        using Type = EntityConfig_Player;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ENTITY_CONFIG_PLAYER;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ENTITY_CONFIG_PLAYER_PTR;
    };

    struct LuaEntityConfigBaby
    {
        static constexpr const char* Name = "EntityConfigBaby";
        using Type = EntityConfig_Baby;
        static constexpr lua::ffi::CDataID C_DATA_ID = lua::ffi::CDataID::ENTITY_CONFIG_BABY;
        static constexpr lua::ffi::CDataID C_DATA_PTR = lua::ffi::CDataID::ENTITY_CONFIG_BABY_PTR;
    };
}

using LuaIntValues = LuabridgeType<LuaTraits::LuaIntValues>;
using LuaVector = CDataType<LuaTraits::LuaVector>;
using LuaPosVel = CDataType<LuaTraits::LuaPosVel>;
using LuaBitSet128 = CDataType<LuaTraits::LuaBitSet128>;
using LuaKColor = CDataType<LuaTraits::LuaKColor>;
using LuaColor = CDataType<LuaTraits::LuaColor>;
using LuaSprite = CDataType<LuaTraits::LuaSprite>;
using LuaFont = CDataType<LuaTraits::LuaFont>;
using LuaFontRenderSettings = CDataType<LuaTraits::LuaFontRenderSettings>;
using LuaRNG = CDataType<LuaTraits::LuaRNG>;
using LuaItemConfig = CDataType<LuaTraits::LuaItemConfig>;
using LuaItem = CDataType<LuaTraits::LuaItem>;
using LuaCard = CDataType<LuaTraits::LuaCard>;
using LuaPillEffect = CDataType<LuaTraits::LuaPillEffect>;
using LuaCostume = CDataType<LuaTraits::LuaCostume>;
using LuaRoomConfigRoom = CDataType<LuaTraits::LuaRoomConfigRoom>;
using LuaSeeds = CDataType<LuaTraits::LuaSeeds>;
using LuaGame = LuabridgeType<LuaTraits::LuaGame>;
using LuaLevel = LuabridgeType<LuaTraits::LuaLevel>;
using LuaRoom = CDataType<LuaTraits::LuaRoom>;
using LuaRoomDescriptor = CDataType<LuaTraits::LuaRoomDescriptor>;
using LuaRoomDescriptorList = CDataType<LuaTraits::LuaRoomDescriptorList>;
using LuaItemPool = LuabridgeType<LuaTraits::LuaItemPool>;
using LuaHUD = CDataType<LuaTraits::LuaHUD>;
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
using LuaEntityRef = CDataType<LuaTraits::LuaEntityRef>;
using LuaEntityPtr = LuabridgeType<LuaTraits::LuaEntityPtr>;
using LuaEntityList = LuabridgeType<LuaTraits::LuaEntityList>;
using LuaPathfinder = LuabridgeType<LuaTraits::LuaPathfinder>;
using LuaTearParams = CDataType<LuaTraits::LuaTearParams>;
using LuaProjectileParams = CDataType<LuaTraits::LuaProjectileParams>;
using LuaActiveItemDesc = LuabridgeType<LuaTraits::LuaActiveItemDesc>;
using LuaGridEntity = CDataType<LuaTraits::LuaGridEntity>;
using LuaGridEntityRock = CDataType<LuaTraits::LuaGridEntityRock>;
using LuaGridEntityPit = CDataType<LuaTraits::LuaGridEntityPit>;
using LuaGridEntitySpikes = CDataType<LuaTraits::LuaGridEntitySpikes>;
using LuaGridEntityTNT = CDataType<LuaTraits::LuaGridEntityTNT>;
using LuaGridEntityPoop = CDataType<LuaTraits::LuaGridEntityPoop>;
using LuaGridEntityDoor = CDataType<LuaTraits::LuaGridEntityDoor>;
using LuaGridEntityPressurePlate = CDataType<LuaTraits::LuaGridEntityPressurePlate>;
using LuaGridEntityDesc = CDataType<LuaTraits::LuaGridEntityDesc>;
using LuaBlendMode = CDataType<LuaTraits::LuaBlendMode>;

// RGON Classes

using LuaHistoryHUD = CDataType<LuaTraits::LuaHistoryHUD>;
using LuaPlayerHUD = CDataType<LuaTraits::LuaPlayerHUD>;
using LuaBossPool = CDataType<LuaTraits::LuaBossPool>;
using LuaEntitySlot = LuabridgeRGONType<Entity_Slot, lua::metatables::EntitySlotMT>;
using LuaEntityDelirium = LuabridgeRGONType<Entity_NPC, lua::metatables::DeliriumMetatable>;
using LuaWeapon = CDataType<LuaTraits::LuaWeapon>;
using LuaMultiShotParams = CDataType<LuaTraits::LuaMultiShotParams>;
using LuaLootList = CDataType<LuaTraits::LuaLootList>;
using LuaGridEntityDecoration = CDataType<LuaTraits::LuaGridEntityDecoration>;
using LuaGridEntityWeb = CDataType<LuaTraits::LuaGridEntityWeb>;
using LuaGridEntityLock = CDataType<LuaTraits::LuaGridEntityLock>;
using LuaGridEntityFire = CDataType<LuaTraits::LuaGridEntityFire>;
using LuaGridEntityWall = CDataType<LuaTraits::LuaGridEntityWall>;
using LuaGridEntityTrapDoor = CDataType<LuaTraits::LuaGridEntityTrapDoor>;
using LuaGridEntityStairs = CDataType<LuaTraits::LuaGridEntityStairs>;
using LuaGridEntityGravity = CDataType<LuaTraits::LuaGridEntityGravity>;
using LuaGridEntityStatue = CDataType<LuaTraits::LuaGridEntityStatue>;
using LuaGridEntityTeleporter = CDataType<LuaTraits::LuaGridEntityTeleporter>;
using LuaColorModifier = CDataType<LuaTraits::LuaColorModifier>;
using LuaEntityDesc = CDataType<LuaTraits::LuaEntityDesc>;
using LuaChallengeParam = CDataType<LuaTraits::LuaChallengeParam>;
using LuaCapsule = CDataType<LuaTraits::LuaCapsule>;
using LuaShape = CDataType<LuaTraits::LuaShape>;
using LuaColorParams = CDataType<LuaTraits::LuaColorParams>;
using LuaWeightedOutcomePicker = CDataType<LuaTraits::LuaWeightedOutcomePicker>;
using LuaHistory = CDataType<LuaTraits::LuaHistory>;
using LuaHistoryItem = CDataType<LuaTraits::LuaHistoryItem>;
using LuaPocketItem = CDataType<LuaTraits::LuaPocketItem>;
using LuaLevelGenerator = CDataType<LuaTraits::LuaLevelGenerator>;
using LuaLevelGeneratorRoom = CDataType<LuaTraits::LuaLevelGeneratorRoom>;
using LuaLevelGeneratorEntry = CDataType<LuaTraits::LuaLevelGeneratorEntry>;
using LuaCostumeSpriteDesc = CDataType<LuaTraits::LuaCostumeSpriteDesc>;
using LuaGenericPrompt = CDataType<LuaTraits::LuaGenericPrompt>;
using LuaEntitySaveState = CDataType<LuaTraits::LuaEntitySaveState>;
using LuaEntitiesSaveStateVector = CDataType<LuaTraits::LuaEntitiesSaveStateVector>;
using LuaGridEntitiesSaveStateVector = CDataType<LuaTraits::LuaGridEntitiesSaveStateVector>;
using LuaEntityConfigEntity = CDataType<LuaTraits::LuaEntityConfigEntity>;
using LuaEntityConfigPlayer = CDataType<LuaTraits::LuaEntityConfigPlayer>;
using LuaEntityConfigBaby = CDataType<LuaTraits::LuaEntityConfigBaby>;
