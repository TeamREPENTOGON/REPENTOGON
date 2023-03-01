#pragma once

#include "lua.hpp"

struct lua_State;

namespace lua {

    enum class Metatables {
        BEGIN_NORMAL,
        PILL_EFFECT,
        SPAWN,
        ENTITY_TEAR,
        HUD,
        SEEDS,
        ROOM,
        ACTIVE_ITEM_DESC,
        GRID_ENTITY_DESC,
        ENTITY,
        ENTITY_BOMB,
        CONFIG,
        VECTOR_LIST,
        MUSIC_MANAGER,
        EFFECT_LIST,
        ITEM_CONFIG_LIST,
        ENTITY_KNIFE,
        LEVEL,
        PILL_CONFIG_LIST,
        ROOM_DESCRIPTOR,
        ENTITY_REF,
        GRID_ENTITY_PRESSURE_PLATE,
        TEMPORARY_EFFECT,
        ENTITY_EFFECT,
        RNG,
        ENTITY_PLAYER,
        FONT,
        ENTITY_PICKUP,
        COSTUME,
        ENTITY_LIST,
        _TEMPORARY_EFFECTS, // Intended typo to disambiguate
        BITSET_128,
        ARRAY_PROXY_ROOM_DESCRIPTOR,
        GAME,
        PROJECTILE_PARAMS,
        GRID_ENTITY_ROCK,
        ENTITY_NPC,
        ENTITY_PROJECTILE,
        COLOR,
        GRID_ENTITY_TNT,
        ENTITY_FAMILIAR,
        KCOLOR,
        COSTUME_CONFIG_LIST,
        ROOM_CONFIG_ENTRIES,
        QUEUE_ITEM_DATA,
        INT_VALUES,
        ENTRY,
        ITEM_POOL,
        ITEM, 
        GRID_ENTITY_SPIKES,
        GRID_ENTITY,
        ROOM_CONFIG_SPAWNS,
        ROOM_CONFIG_ROOM,
        ENTITY_PTR,
        POS_VEL,
        PATHFINDER,
        TEAR_PARAMS,
        GRID_ENTITY_POOP,
        CARD,
        GRID_ENTITY_PIT,
        SPRITE,
        GRID_ENTITY_DOOR,
        ENTITY_LASER,
        SFX_MANAGER,
        CARD_CONFIG_LIST,
        VECTOR,
        BEGIN_CONST,
        CONST_PILL_EFFECT,
        CONST_SPAWN,
        CONST_ENTITY_TEAR,
        CONST_HUD,
        CONST_SEEDS,
        CONST_ROOM,
        CONST_ACTIVE_ITEM_DESC,
        CONST_GRID_ENTITY_DESC,
        CONST_ENTITY,
        CONST_ENTITY_BOMB,
        CONST_CONFIG,
        CONST_VECTOR_LIST,
        CONST_MUSIC_MANAGER,
        CONST_EFFECT_LIST,
        CONST_ITEM_CONFIG_LIST,
        CONST_ENTITY_KNIFE,
        CONST_LEVEL,
        CONST_PILL_CONFIG_LIST,
        CONST_ROOM_DESCRIPTOR,
        CONST_ENTITY_REF,
        CONST_GRID_ENTITY_PRESSURE_PLATE,
        CONST_TEMPORARY_EFFECT,
        CONST_ENTITY_EFFECT,
        CONST_RNG,
        CONST_ENTITY_PLAYER,
        CONST_FONT,
        CONST_ENTITY_PICKUP,
        CONST_COSTUME,
        CONST_ENTITY_LIST,
        _CONST_TEMPORARY_EFFECTS, // Intended typo to disambiguate
        CONST_BITSET_128,
        CONST_ARRAY_PROXY_ROOM_DESCRIPTOR,
        CONST_GAME,
        CONST_PROJECTILE_PARAMS,
        CONST_GRID_ENTITY_ROCK,
        CONST_ENTITY_NPC,
        CONST_ENTITY_PROJECTILE,
        CONST_COLOR,
        CONST_GRID_ENTITY_TNT,
        CONST_ENTITY_FAMILIAR,
        CONST_KCOLOR,
        CONST_COSTUME_CONFIG_LIST,
        CONST_ROOM_CONFIG_ENTRIES,
        CONST_QUEUE_ITEM_DATA,
        CONST_INT_VALUES,
        CONST_ENTRY,
        CONST_ITEM_POOL,
        CONST_ITEM, 
        CONST_GRID_ENTITY_SPIKES,
        CONST_GRID_ENTITY,
        CONST_ROOM_CONFIG_SPAWNS,
        CONST_ROOM_CONFIG_ROOM,
        CONST_ENTITY_PTR,
        CONST_POS_VEL,
        CONST_PATHFINDER,
        CONST_TEAR_PARAMS,
        CONST_GRID_ENTITY_POOP,
        CONST_CARD,
        CONST_GRID_ENTITY_PIT,
        CONST_SPRITE,
        CONST_GRID_ENTITY_DOOR,
        CONST_ENTITY_LASER,
        CONST_SFX_MANAGER,
        CONST_CARD_CONFIG_LIST,
        CONST_VECTOR
    };

    namespace metatables
    {
        extern const char* EntitySlotMT;;
    }

    void UnloadMetatables();
    void RegisterMetatable(Metatables metatable, void* key);
    void PushMetatable(lua_State* L, Metatables metatable);
    void* GetMetatableKey(Metatables metatable);
    Metatables GetMetatableIdxFromName(std::string const& name);

    void* TestUserdata(lua_State* L, int ud, lua::Metatables mt);
    void* CheckUserdata(lua_State* L, int ud, lua::Metatables mt, std::string const& name);

    template<typename T>
    T GetUserdata(lua_State* L, int idx, lua::Metatables mt, std::string const& name) {
        void* p = CheckUserdata(L, idx, mt, name);
        return *(T*)((char*)p + 0x4);
    }

    template<typename T>
    T GetUserdata(lua_State* L, int idx, const char* mt) {
        void* ud = luaL_checkudata(L, idx, mt);
        return (T)ud;
    }

    struct lua_global_tag_t {};

    class LuaResults;

    class LuaCaller {
    public:
        LuaCaller(lua_State* L);

        LuaCaller& push(bool x);
        LuaCaller& push(lua_CFunction fn, int n = 0);
        LuaCaller& pushfstring(const char* fmt, ...);
        LuaCaller& push(lua_global_tag_t);

        template<typename T>
        std::enable_if_t<std::is_integral_v<T>, LuaCaller&> push(T x) {
            lua_pushinteger(_L, x);
            ++_n;
            return *this;
        }
        
        template<typename T>
        std::enable_if_t<std::is_floating_point_v<T>, LuaCaller&> push(T x) {
            lua_pushnumber(_L, x);
            ++_n;
            return *this;
        }

        LuaCaller& push(void* p);
        LuaCaller& push(const char* s, size_t len = 0);
        LuaCaller& pushnil();
        LuaCaller& pushvalue(int idx);
        LuaCaller& push(const char* fmt, va_list va);
        LuaCaller& push(void* ptr, Metatables meta);
        LuaCaller& push(void* ptr, const char* meta); 
        template<typename T>
        LuaCaller& pushUserdataValue(T const& t, Metatables meta) {
            luabridge::UserdataValue<T>::push(_L, GetMetatableKey(meta), t);
            ++_n;
            return *this;
        }

        LuaResults call(int nresults);

    private:
        lua_State* _L;
        int _n = 0;
    };

    class LuaStackProtector {
    public:
        LuaStackProtector(lua_State* L, int n = 0);
        ~LuaStackProtector();

        LuaStackProtector(LuaStackProtector const&) = delete;
        LuaStackProtector& operator=(LuaStackProtector const&) = delete;

        LuaStackProtector(LuaStackProtector&&);
        LuaStackProtector& operator=(LuaStackProtector&&);

    private:
        lua_State* _L;
        int _orig;
        int _n;
    };

    class LuaResults {
    public:
        friend LuaResults LuaCaller::call(int);

        ~LuaResults();

        LuaResults& operator=(LuaResults const&) = delete;
        LuaResults(LuaResults const&) = delete;

        LuaResults(LuaResults&&);
        LuaResults& operator=(LuaResults&&);

        operator bool() const;

    private:
        LuaResults(lua_State* L, int n, int valid);

        lua_State* _L;
        int _n;
        int _valid;
    };


    template<typename T>
    T UserdataToData(void* ud) {
        return *(T*)((char*)ud + 4);
    }

    namespace luabridge {
        extern void* identityKey;
        extern lua_CFunction indexMetaMethod;
        extern lua_CFunction newIndexMetaMethod;

        inline void* getIdentityKey() {
            return identityKey;
        }

        class Userdata {
        protected:
            void* m_p = nullptr;

        public:
            virtual ~Userdata() { }

            inline void* getPointer() {
                return m_p;
            }
        };

        template<typename T>
        class UserdataValue : public Userdata {
        private:
            UserdataValue<T>(UserdataValue<T> const&);
            UserdataValue<T>& operator=(UserdataValue<T> const&);

            char m_storage[sizeof(T)];

            UserdataValue() {
                m_p = getObject();
            }

            inline T* getObject() {
                return reinterpret_cast<T*>(m_storage);
            }

        public:
            static T* place(lua_State* L, void* key) {
                UserdataValue<T>* const ud = new(lua_newuserdata(L, sizeof(UserdataValue<T>))) UserdataValue<T>();
                lua_rawgetp(L, LUA_REGISTRYINDEX, key);
                lua_setmetatable(L, -2);
                return (T*)ud->getPointer();
            }

            template<typename U>
            static void push(lua_State* L, void* key, U const& u) {
                new (place(L, key))  U(u);
            }
        };

        class UserdataPtr : public Userdata {
        private:
            UserdataPtr(UserdataPtr const&);
            UserdataPtr& operator= (UserdataPtr const&);

            explicit UserdataPtr(void* const p);

        public:
            static void push(lua_State* L, void* const p, void const* const key);
            static void push(lua_State* L, void* const p, const char* key);
        };
    }

    namespace callbacks {
        int ToInteger(lua_State* L, int stackPosition);
        double ToNumber(lua_State* L, int stackPosition);
        bool ToBoolean(lua_State* L, int stackPosition);
		const char* ToString(lua_State* L, int stackPosition);
    }
}