#pragma once

#include "lua.hpp"
#include "libzhl.h"

#include <string>
#include <optional>

struct lua_State;

namespace lua {
    namespace GlobalClasses
    {
      extern LIBZHL_API const char* Isaac;
    }

    LIBZHL_API void* TestCData(lua_State* L, int ud, lua_CTypeId ctypeid);
    LIBZHL_API void* TestEntity(lua_State* L, int ud);
    LIBZHL_API void* CheckCData(lua_State* L, int ud, lua_CTypeId ctypeid, std::string const& name);

    LIBZHL_API void RegisterGlobalClassFunction(lua_State* L, const char* className, const char* funcName, lua_CFunction func);

    LIBZHL_API void RegisterNewClass(lua_State* L, const char* name, const char* metaname, luaL_Reg* functions, lua_CFunction gc = nullptr);
	
    template<typename T>
    T GetRawUserdata(lua_State* L, int idx, const char* mt) {
        void* ud = luaL_checkudata(L, idx, mt);
        return (T)ud;
    }

    template<typename T>
    T GetCData(lua_State* L, int idx, lua_CTypeId ctypeid, std::string const& name) {
        void* p = CheckCData(L, idx, ctypeid, name);

        if constexpr (std::is_pointer_v<T>) {
            if (lua_tocdataid(L, idx) != ctypeid) {
                return *reinterpret_cast<T*>(p);
            }
            return static_cast<T>(p);
        } else {
            return *(T*)p;
        }
    }

    class LuaResults;

    // used for dynamic pushes in lua caller
    struct LuaClassInterface
    {
        void (*Push)(lua_State*, const void*);
        void (*PushPtr)(lua_State*, void*);
    };

    class LIBZHL_API LuaCaller {
    public:
        LuaCaller(lua_State* L);

        LuaCaller& push(bool x);
        LuaCaller& push(lua_CFunction fn, int n = 0);
        LuaCaller& pushfstring(const char* fmt, ...);

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
        LuaCaller& pushluaref(int t, int ref);
        LuaCaller& pushluaref(int ref);
        LuaCaller& push(const char* fmt, va_list va);
        LuaCaller& pushCallbackID(const char* name, const char* ns = nullptr);
        template<typename LuaClass, typename T>
        LuaCaller& pushClass(const T& value)
        {
            LuaClass::Push(_L, value);
            ++_n;
            return *this;
        }
        template<typename LuaClass, class... Args>
        LuaCaller& pushClassPtr(Args&&... args)
        {
            LuaClass::PushPtr(_L, std::forward<Args>(args)...);
            ++_n;
            return *this;
        }
        LuaCaller& pushClass(const LuaClassInterface& classInterface, const void* value)
        {
            classInterface.Push(_L, value);
            ++_n;
            return *this;
        };
        LuaCaller& pushClassPtr(const LuaClassInterface& classInterface, void* value)
        {
            classInterface.PushPtr(_L, value);
            ++_n;
            return *this;
        };
        void pushTable(int narr = 0, int nrec = 0);

        LuaResults call(int nresults);

    private:
        lua_State* _L;
        int _n = 0;
    };

    class LIBZHL_API LuaStackProtector {
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

    class LIBZHL_API LuaResults {
    public:
        friend LuaResults LuaCaller::call(int);

        ~LuaResults();

        LuaResults& operator=(LuaResults const&) = delete;
        LuaResults(LuaResults const&) = delete;

        LuaResults(LuaResults&&);
        LuaResults& operator=(LuaResults&&);

        /* Return true if the call triggered an error, false if everything went 
         * well. This is for backward compatibility with the C API where LUA_OK is 
         * defined as 0. Therefore, a call to lua_(x)(p)call is a success if it returns
         * 0, checked as `if (!lua_call(...))`.
         */
        operator bool() const;

        int getResultCode() const { return _resultCode; }

    private:
        LuaResults(lua_State* L, int n, int resultCode);

        lua_State* _L;
        int _n;
        int _resultCode;
    };


    enum BoolCheckModes : uint8_t {
        BOOL_CHECK_MODE_DEFAULT,
        BOOL_CHECK_MODE_NOT_NIL,
        BOOL_CHECK_MODE_STRICT
    };

    LIBZHL_API bool luaL_optboolean(lua_State* L, int idx, bool default);
    LIBZHL_API bool luaL_checkboolean(lua_State* L, int idx, BoolCheckModes mode = BOOL_CHECK_MODE_NOT_NIL);

    namespace ffi {
        template<typename T>
        void pushCdata(lua_State* L, lua_CTypeId ctypeid, T const& value) {
            lua_pushcdata(L, ctypeid, &value, sizeof(T));
        }

        template<typename T>
        T* placeCdata(lua_State* L, lua_CTypeId ctypeid) {
            lua_pushcdata(L, ctypeid, nullptr, sizeof(T));
            return (T*)lua_tocdata(L, -1);
        }

        inline void pushCdataPtr(lua_State* L, void const* p, lua_CTypeId ctypeid) {
            if (p == nullptr)
                lua_pushnil(L);
            else
                lua_pushcdata(L, ctypeid, &p, sizeof(p));
        }
        enum CDataID : lua_CTypeId {
            VECTOR,
            VECTOR_PTR,
            GRID_ENTITY_DESC,
            GRID_ENTITY_DESC_PTR,
            COLOR,
            CONST_COLOR,
            COLOR_PTR,
            BITSET_128,
            BITSET_128_PTR,
            POS_VEL,
            POS_VEL_PTR,
            COSTUME,
            COSTUME_PTR,
            ITEM,
            ITEM_PTR,
            KCOLOR,
            KCOLOR_PTR,
            TEAR_PARAMS,
            TEAR_PARAMS_PTR,
            PROJECTILE_PARAMS,
            PROJECTILE_PARAMS_PTR,
            ACTIVE_ITEM_DESC,
            ACTIVE_ITEM_DESC_PTR,
            QUEUE_ITEM_DATA,
            QUEUE_ITEM_DATA_PTR,
            TEMPORARY_EFFECTS,
            TEMPORARY_EFFECTS_PTR,
            SHADER,
            SHADER_PTR,
            IMAGE,
            IMAGE_PTR,
            DESTINATION_QUAD,
            DESTINATION_QUAD_PTR,
            SOURCE_QUAD,
            SOURCE_QUAD_PTR,
            SPRITE,
            SPRITE_PTR,
            BLEND_MODE,
            BLEND_MODE_PTR,
            ENTITY_REF,
            ENTITY_REF_PTR,
            GRID_ENTITY,
            GRID_ENTITY_PTR,
            GRID_ENTITY_ROCK,
            GRID_ENTITY_ROCK_PTR,
            GRID_ENTITY_PIT,
            GRID_ENTITY_PIT_PTR,
            GRID_ENTITY_SPIKES,
            GRID_ENTITY_SPIKES_PTR,
            GRID_ENTITY_TNT,
            GRID_ENTITY_TNT_PTR,
            GRID_ENTITY_POOP,
            GRID_ENTITY_POOP_PTR,
            GRID_ENTITY_DOOR,
            GRID_ENTITY_DOOR_PTR,
            GRID_ENTITY_PRESSURE_PLATE,
            GRID_ENTITY_PRESSURE_PLATE_PTR,
            GRID_ENTITY_DECORATION,
            GRID_ENTITY_DECORATION_PTR,
            GRID_ENTITY_WEB,
            GRID_ENTITY_WEB_PTR,
            GRID_ENTITY_LOCK,
            GRID_ENTITY_LOCK_PTR,
            GRID_ENTITY_FIRE,
            GRID_ENTITY_FIRE_PTR,
            GRID_ENTITY_WALL,
            GRID_ENTITY_WALL_PTR,
            GRID_ENTITY_TRAP_DOOR,
            GRID_ENTITY_TRAP_DOOR_PTR,
            GRID_ENTITY_STAIRS,
            GRID_ENTITY_STAIRS_PTR,
            GRID_ENTITY_GRAVITY,
            GRID_ENTITY_GRAVITY_PTR,
            GRID_ENTITY_STATUE,
            GRID_ENTITY_STATUE_PTR,
            GRID_ENTITY_TELEPORTER,
            GRID_ENTITY_TELEPORTER_PTR,
            ROOM_CONFIG_ROOM,
            ROOM_CONFIG_ROOM_PTR,
            COLOR_MODIFIER,
            COLOR_MODIFIER_PTR,
            ROOM,
            ROOM_PTR,
            ROOM_DESCRIPTOR,
            ROOM_DESCRIPTOR_PTR,
            ROOM_DESCRIPTOR_LIST,
            ROOM_DESCRIPTOR_LIST_PTR,
            RNG,
            RNG_PTR,
            VECTOR_LIST,
            VECTOR_LIST_PTR,
            ENTITY_DESC,
            ENTITY_DESC_PTR,
            CHALLENGE_PARAM,
            CHALLENGE_PARAM_PTR,
            CAPSULE,
            CAPSULE_PTR,
            SHAPE,
            SHAPE_PTR,
            COLOR_PARAMS,
            COLOR_PARAMS_PTR,
            LOOT_LIST,
            LOOT_LIST_PTR,
            MULTI_SHOT_PARAMS,
            MULTI_SHOT_PARAMS_PTR,
            WEIGHTED_OUTCOME_PICKER,
            WEIGHTED_OUTCOME_PICKER_PTR,
            HISTORY,
            HISTORY_PTR,
            HISTORY_ITEM,
            HISTORY_ITEM_PTR,
            POCKET_ITEM,
            POCKET_ITEM_PTR,
            LEVEL_GENERATOR,
            LEVEL_GENERATOR_PTR,
            LEVEL_GENERATOR_ROOM,
            LEVEL_GENERATOR_ROOM_PTR,
            LEVEL_GENERATOR_ENTRY,
            LEVEL_GENERATOR_ENTRY_PTR,
            BOSS_POOL,
            BOSS_POOL_PTR,
            COSTUME_SPRITE_DESC,
            COSTUME_SPRITE_DESC_PTR,
            GENERIC_PROMPT,
            GENERIC_PROMPT_PTR,
            HUD,
            HUD_PTR,
            PLAYER_HUD,
            PLAYER_HUD_PTR,
            HISTORY_HUD,
            HISTORY_HUD_PTR,
            WEAPON,
            WEAPON_PTR,
            ENTITY_SAVE_STATE,
            ENTITY_SAVE_STATE_PTR,
            ENTITIES_SAVE_STATE_VECTOR,
            ENTITIES_SAVE_STATE_VECTOR_PTR,
            GRID_ENTITIES_SAVE_STATE_VECTOR,
            GRID_ENTITIES_SAVE_STATE_VECTOR_PTR,
            ENTITY_CONFIG_ENTITY,
            ENTITY_CONFIG_ENTITY_PTR,
            ENTITY_CONFIG_PLAYER,
            ENTITY_CONFIG_PLAYER_PTR,
            ENTITY_CONFIG_BABY,
            ENTITY_CONFIG_BABY_PTR,
            SEEDS,
            SEEDS_PTR,
            FONT,
            FONT_PTR,
            FONT_RENDER_SETTINGS,
            FONT_RENDER_SETTINGS_PTR,
            ITEM_CONFIG,
            ITEM_CONFIG_PTR,
            ITEM_CONFIG_CARD,
            ITEM_CONFIG_CARD_PTR,
            ITEM_CONFIG_PILL_EFFECT,
            ITEM_CONFIG_PILL_EFFECT_PTR,
            ITEM_POOL,
            ITEM_POOL_PTR,
            LEVEL,
            LEVEL_PTR,
            GAME,
            GAME_PTR,
            ENTITY,
            ENTITY_PTR,
            ENTITY_PROJECTILE,
            ENTITY_PROJECTILE_PTR,
            ENTITY_TEAR,
            ENTITY_TEAR_PTR,
            ENTITY_BOMB,
            ENTITY_BOMB_PTR,
            ENTITY_KNIFE,
            ENTITY_KNIFE_PTR,
            ENTITY_LASER,
            ENTITY_LASER_PTR,
            ENTITY_EFFECT,
            ENTITY_EFFECT_PTR,
            ENTITY_PICKUP,
            ENTITY_PICKUP_PTR,
            ENTITY_FAMILIAR,
            ENTITY_FAMILIAR_PTR,
            ENTITY_SLOT,
            ENTITY_SLOT_PTR,
            PATHFINDER,
            PATHFINDER_PTR,
            ENTITY_NPC,
            ENTITY_NPC_PTR,
            ENTITY_DELIRIUM,
            ENTITY_DELIRIUM_PTR,
            ENTITY_PLAYER,
            ENTITY_PLAYER_PTR,
            MAX_CDATA
        };

        extern LIBZHL_API lua_CTypeId CData[MAX_CDATA];
    };

    template<typename T, typename... Args>
    T* place(lua_State* L, const char* mt, Args&&... args) {
        void* data = lua_newuserdata(L, sizeof(T));
        luaL_setmetatable(L, mt);
        new (data) T(std::forward<Args>(args)...);
        return (T*)data;
    }

    // These assume the destination table is at the top of the stack.
    LIBZHL_API void TableAssoc(lua_State* L, std::string const& name, int value);
    LIBZHL_API void TableAssoc(lua_State* L, std::string const& name, float value);
    LIBZHL_API void TableAssoc(lua_State* L, std::string const& name, lua_CFunction fn);
    LIBZHL_API void TableAssoc(lua_State* L, std::string const& name, void* ptr);
    LIBZHL_API void TableAssoc(lua_State* L, int key, int value);
    LIBZHL_API void TableAssoc(lua_State* L, int key, float value);
    LIBZHL_API void TableAssoc(lua_State* L, int key, lua_CFunction fn);
    LIBZHL_API void TableAssoc(lua_State* L, int key, void* ptr);

    namespace callbacks {
        LIBZHL_API bool CheckInteger(lua_State* L, int stackPosition);
        LIBZHL_API int ToInteger(lua_State* L, int stackPosition);
        LIBZHL_API double ToNumber(lua_State* L, int stackPosition);
        LIBZHL_API bool ToBoolean(lua_State* L, int stackPosition);
        LIBZHL_API const char* ToString(lua_State* L, int stackPosition);
    }

    void LIBZHL_API PushCallbackID(lua_State* L, const char* name, const char* ns = nullptr);
    void LIBZHL_API PushCallbackRegistryKey(lua_State* L = nullptr);
}

#define LUA_FUNCTION(name) static int name(lua_State* L)
