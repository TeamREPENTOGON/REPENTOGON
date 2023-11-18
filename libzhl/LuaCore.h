#pragma once

#include "lua.hpp"
#include "libzhl.h"

#include <string>
#include <optional>

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
        CONST_VECTOR,
        METATABLES_MAX
    };

    namespace GlobalClasses
    {
      extern LIBZHL_API const char* Input;
      extern LIBZHL_API const char* Isaac;
      extern LIBZHL_API const char* HUD;
      extern LIBZHL_API const char* Options;
    }

		namespace metatables
		{
      extern LIBZHL_API const char* AmbushMT;
			extern LIBZHL_API const char* AnimationDataMT;
			extern LIBZHL_API const char* AnimationFrameMT;
			extern LIBZHL_API const char* AnimationLayerMT;
			extern LIBZHL_API const char* AnimationStateMT;
      extern LIBZHL_API const char* BackdropMT;
      extern LIBZHL_API const char* BeamMT;
			extern LIBZHL_API const char* BestiaryMenuMT;
			extern LIBZHL_API const char* BlendModeMT;
      extern LIBZHL_API const char* CameraMT;
			extern LIBZHL_API const char* CapsuleMT;
			extern LIBZHL_API const char* ChallengeMenuMT;
      extern LIBZHL_API const char* ChallengeParamMT;
			extern LIBZHL_API const char* CharacterMenuMT;
			extern LIBZHL_API const char* CollectionMenuMT;
      extern LIBZHL_API const char* ConsoleMT;
			extern LIBZHL_API const char* ControllerSelectMenuMT;
			extern LIBZHL_API const char* ColorModifierMT;
			extern LIBZHL_API const char* CustomChallengeMenuMT;
			extern LIBZHL_API const char* CutscenesMenuMT;
			extern LIBZHL_API const char* DailyChallengeMT;
			extern LIBZHL_API const char* DailyChallengeMenuMT;
      extern LIBZHL_API const char* DebugRendererMT;
			extern LIBZHL_API const char* EntitiesSaveStateVectorMT;
			extern LIBZHL_API const char* EntitySaveStateMT;
			extern LIBZHL_API const char* EntitySlotMT;
			extern LIBZHL_API const char* FXParamsMT;
			extern LIBZHL_API const char* HistoryMT;
      extern LIBZHL_API const char* HistoryItemMT;
      extern LIBZHL_API const char* ImGuiMT;
			extern LIBZHL_API const char* ItemOverlayMT;
			extern LIBZHL_API const char* KeyConfigMenuMT;
      extern LIBZHL_API const char* LayerStateMT;
			extern LIBZHL_API const char* LevelGeneratorMT;
      extern LIBZHL_API const char* LevelGeneratorEntryMT;
			extern LIBZHL_API const char* LevelGeneratorRoomMT;
			extern LIBZHL_API const char* MainMenuMT;
			extern LIBZHL_API const char* MenuManagerMT;
			extern LIBZHL_API const char* MinimapMT;
			extern LIBZHL_API const char* ModsMenuMT;
			extern LIBZHL_API const char* MultiShotParamsMT;
			extern LIBZHL_API const char* NightmareSceneMT;
			extern LIBZHL_API const char* NullFrameMT;
			extern LIBZHL_API const char* OptionsMenuMT;
      extern LIBZHL_API const char* PersistentGameDataMT;
			extern LIBZHL_API const char* PlayerHUDMT;
			extern LIBZHL_API const char* PlayerHUDHeartMT;
      extern LIBZHL_API const char* PlayerManagerMT;
      extern LIBZHL_API const char* PocketItemMT;
      extern LIBZHL_API const char* ProceduralEffectMT;
      extern LIBZHL_API const char* ProceduralItemMT;
      extern LIBZHL_API const char* ProceduralItemManagerMT;
      extern LIBZHL_API const char* RngMT;
      extern LIBZHL_API const char* RoomMT;
      extern LIBZHL_API const char* RoomConfigHolderMT;
      extern LIBZHL_API const char* RoomDescriptorDoors;
      extern LIBZHL_API const char* RoomDescriptorDoorsConst;
      extern LIBZHL_API const char* RoomTransitionMT;
			extern LIBZHL_API const char* SaveMenuMT;
      extern LIBZHL_API const char* ScoreSheetMT;
      extern LIBZHL_API const char* ShapeMT;
			extern LIBZHL_API const char* SpecialSeedsMenuMT;
      extern LIBZHL_API const char* StageTransitionMT;
			extern LIBZHL_API const char* StatsMenuMT;
			extern LIBZHL_API const char* TitleMenuMT;
			extern LIBZHL_API const char* WeaponMT;
            extern LIBZHL_API const char* WeightedOutcomePickerMT;
            extern LIBZHL_API const char* CostumeSpriteDescMT;
		}

    LIBZHL_API void UnloadMetatables();
    LIBZHL_API void RegisterMetatable(Metatables metatable, void* key);
    LIBZHL_API void PushMetatable(lua_State* L, Metatables metatable);
    LIBZHL_API  void* GetMetatableKey(Metatables metatable);
    LIBZHL_API Metatables GetMetatableIdxFromName(std::string const& name);

    LIBZHL_API void* TestUserdata(lua_State* L, int ud, lua::Metatables mt);
    LIBZHL_API void* CheckUserdata(lua_State* L, int ud, lua::Metatables mt, std::string const& name);
    LIBZHL_API void* CheckUserdata(lua_State* L, int ud, lua::Metatables mt, lua::Metatables constMt, std::string const& name);

    LIBZHL_API void RegisterFunction(lua_State* L, lua::Metatables mt, const char* name, lua_CFunction func);
    LIBZHL_API void RegisterFunctions(lua_State* L, lua::Metatables mt, luaL_Reg* functions);

    LIBZHL_API void RegisterGlobalClassFunction(lua_State* L, const char* className, const char* funcName, lua_CFunction func);

    LIBZHL_API void RegisterVariable(lua_State* L, lua::Metatables mt, const char* variableName, lua_CFunction getFunc, lua_CFunction setFunc);
    LIBZHL_API void RegisterVariableGetter(lua_State* L, lua::Metatables mt, const char* variableName, lua_CFunction func);
    LIBZHL_API void RegisterVariableSetter(lua_State* L, lua::Metatables mt, const char* variableName, lua_CFunction func);
    LIBZHL_API void RegisterGlobalClassVariable(lua_State* L, const char* className, const char* variableName, lua_CFunction getFunc, lua_CFunction setFunc);
    LIBZHL_API void RegisterGlobalClassVariableGetter(lua_State* L, const char* className, const char* variableName, lua_CFunction func);
    LIBZHL_API void RegisterGlobalClassVariableSetter(lua_State* L, const char* className, const char* variableName, lua_CFunction func);
    LIBZHL_API void RegisterVariableToLoadedMT(lua_State* L, const char* variableName, lua_CFunction getFunc, lua_CFunction setFunc);
    LIBZHL_API void RegisterVariableGetterToLoadedMT(lua_State* L, const char* variableName, lua_CFunction func, int pop = 2);
    LIBZHL_API void RegisterVariableSetterToLoadedMT(lua_State* L, const char* variableName, lua_CFunction func, int pop = 2);

    LIBZHL_API void RegisterNewClass(lua_State* L, const char* name, const char* metaname, luaL_Reg* functions, lua_CFunction gc = nullptr);
	
    template<typename T>
    T GetUserdata(lua_State* L, int idx, lua::Metatables mt, std::string const& name) {
        void* p = CheckUserdata(L, idx, mt, name);
        return *(T*)((char*)p + 0x4);
    }

    // Use this version if you need your userdata to be either the const or non const version
    // of something. Most of the time you won't need it.
    template<typename T>
    T GetUserdata(lua_State* L, int idx, lua::Metatables mt, lua::Metatables constMt, std::string const& name) {
        void* p = CheckUserdata(L, idx, mt, constMt, name);
        return *(T*)((char*)p + 0x4);
    }

    template<typename T>
    std::optional<T> TestUserdata(lua_State* L, int idx, lua::Metatables mt) {
        void* p = TestUserdata(L, idx, mt);
        if (p) {
            return std::make_optional(*(T*)((char*)p + 0x4));
        }

        return std::nullopt;
    }

    template<typename T>
    T GetUserdata(lua_State* L, int idx, const char* mt) {
        void* ud = luaL_checkudata(L, idx, mt);
        return (T)ud;
    }

    struct lua_global_tag_t {};

    class LuaResults;

    class LIBZHL_API LuaCaller {
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
        LuaCaller& pushLuabridge(void* ptr, const char* meta); 
        LuaCaller& pushCallbackID(const char* name, const char* ns = nullptr);
        template<typename T>
        std::enable_if_t<std::is_pointer_v<T>, LuaCaller&> push(T ptr, const char* meta) {
            void** result = (void**)lua_newuserdata(_L, sizeof(void*));
            *result = ptr;
            luaL_setmetatable(_L, meta);
            ++_n;
            return *this;
        }
        template<typename T>
        LuaCaller& pushUserdataValue(T const& t, Metatables meta) {
            luabridge::UserdataValue<T>::push(_L, GetMetatableKey(meta), t);
            ++_n;
            return *this;
        }

        void* pushUd(size_t size, const char* mt);

        template<typename T, typename... Args>
        T* pushUd(const char* mt, Args&&... args) {
            void* result = lua_newuserdata(_L, sizeof(T));
            luaL_setmetatable(_L, mt);
            new (result) T(std::forward<Args>(args)...);
            ++_n;
            return (T*)result;
        }

        void pushTable(int narr = 0, int nrec = 0);
        void pushRef(int t, int ref);
        void pushGlobal(const char* global);

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


    template<typename T>
    T UserdataToData(void* ud) {
        return *(T*)((char*)ud + 4);
    }
	
    namespace luabridge {
        extern LIBZHL_API void* identityKey;
        extern LIBZHL_API lua_CFunction indexMetaMethod;
        extern LIBZHL_API lua_CFunction newIndexMetaMethod;

        inline void* getIdentityKey() {
            return identityKey;
        }

        class LIBZHL_API Userdata {
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

        class LIBZHL_API UserdataPtr : public Userdata {
        private:
            UserdataPtr(UserdataPtr const&);
            UserdataPtr& operator= (UserdataPtr const&);

            explicit UserdataPtr(void* const p);

        public:
            static void push(lua_State* L, void* const p, void const* const key);
            static void push(lua_State* L, void* const p, const char* meta);
            static void push(lua_State* L, void* const p, lua::Metatables mt);
        };
    }

    enum BoolCheckModes : uint8_t {
        BOOL_CHECK_MODE_DEFAULT,
        BOOL_CHECK_MODE_NOT_NIL,
        BOOL_CHECK_MODE_STRICT
    };

    LIBZHL_API bool luaL_optboolean(lua_State* L, int idx, bool default);
    LIBZHL_API bool luaL_checkboolean(lua_State* L, int idx, BoolCheckModes mode = BOOL_CHECK_MODE_NOT_NIL);

    template<typename T, typename... Args>
    T* place(lua_State* L, const char* mt, Args&&... args) {
        void* data = lua_newuserdata(L, sizeof(T));
        luaL_setmetatable(L, mt);
        new (data) T(std::forward<Args>(args)...);
        return (T*)data;
    }

    struct LIBZHL_API LuaStackRef {
        LuaStackRef(int ref) : _ref(ref) { }
        operator int() {
            return _ref;
        }

        int _ref;
    };

    /* The following four functions assume the destination table is at the top of the stack.
     * If it's not the case you need to use the overload that takes two LuaStackRefs as 
     * parameters. To do so, push the value you want to set in the table, then give both the
     * index of this value and of the table as parameters to TableAssoc.
     */
    LIBZHL_API void TableAssoc(lua_State* L, std::string const& name, int value);
    LIBZHL_API void TableAssoc(lua_State* L, std::string const& name, float value);
    LIBZHL_API void TableAssoc(lua_State* L, std::string const& name, lua_CFunction fn);
    LIBZHL_API void TableAssoc(lua_State* L, std::string const& name, void* ptr);
    /* Store in the table at index dstTable the value at index srcObj.
     * The value at index srcObj is subsequently poped from the stack.
     */
    LIBZHL_API void TableAssoc(lua_State* L, std::string const& name, LuaStackRef dstTable, LuaStackRef srcObj);

    /* The following four functions assume the destination table is at the top of the stack.
     * If it's not the case you need to use the overload that takes two LuaStackRefs as
     * parameters. To do so, push the value you want to set in the table, then give both the
     * index of this value and of the table as parameters to TableAssoc.
     */
    LIBZHL_API void TableAssoc(lua_State* L, int key, int value);
    LIBZHL_API void TableAssoc(lua_State* L, int key, float value);
    LIBZHL_API void TableAssoc(lua_State* L, int key, lua_CFunction fn);
    LIBZHL_API void TableAssoc(lua_State* L, int key, void* ptr);
    /* Store in the table at index dstTable the value at index srcObj.
     * The value at index srcObj is subsequently poped from the stack.
     */
    LIBZHL_API void TableAssoc(lua_State* L, int key, LuaStackRef dstTable, LuaStackRef srcObj);

    namespace callbacks {
        LIBZHL_API int ToInteger(lua_State* L, int stackPosition);
        LIBZHL_API double ToNumber(lua_State* L, int stackPosition);
        LIBZHL_API bool ToBoolean(lua_State* L, int stackPosition);
        LIBZHL_API const char* ToString(lua_State* L, int stackPosition);
    }

    void LIBZHL_API PushCallbackID(lua_State* L, const char* name, const char* ns = nullptr);
    void LIBZHL_API PushCallbackRegistryKey(lua_State* L = nullptr);
    int LIBZHL_API LuaCheckMainMenuExists(lua_State* L, const char* className);
}

#define LUA_FUNCTION(name) static int name(lua_State* L)
