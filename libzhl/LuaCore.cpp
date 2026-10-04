#include <map>
#include <sstream>
#include <stdexcept>

#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Log.h"
#include "HookSystem_private.h"
#include "SigScan.h"

namespace lua {
	std::map<Metatables, void*> _metatables;
	std::map<std::string, Metatables> _metatable_idx_from_name;
	bool _metatable_idx_from_name_initialized = false;

	class UnregistedMetatableException : public std::exception {
	public:
		UnregistedMetatableException(Metatables metatable) : _metatable(metatable) {
			snprintf(_err, 256, "Attempt to get unregistered metatable %d\n", (int)_metatable);
		}

		const char* what() const override {
			return _err;
		}

	private:
		Metatables _metatable;
		char _err[256];
	};
	
	void UnloadMetatables() {
		_metatables.clear();
	}

	void PushMetatable(lua_State* L, Metatables metatable) {
		auto iter = _metatables.find(metatable);

		if (iter == _metatables.end()) {
			throw UnregistedMetatableException(metatable);
		}

		lua_rawgetp(L, LUA_REGISTRYINDEX, _metatables[metatable]);
	}

	void* GetMetatableKey(Metatables metatable) {
		return _metatables[metatable];
	}

	void RegisterMetatable(Metatables metatable, void* key) {
		if (_metatables.find(metatable) != _metatables.end()) {
			return;
		}

		_metatables[metatable] = key;
	}

	void RegisterNewClass(lua_State* L, const char* name, const char* metaname, luaL_Reg* functions, lua_CFunction gc) {
		luaL_newmetatable(L, metaname);
		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);
		lua_rawset(L, -3);

		lua_pushstring(L, "__type");
		lua_pushstring(L, name);
		lua_rawset(L, -3);

		lua_pushstring(L, "__class");
		lua_pushstring(L, name);
		lua_rawset(L, -3);

		if (gc) {
			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, gc);
			lua_rawset(L, -3);
		}

		luaL_setfuncs(L, functions, 0);
		lua_pop(L, 1);
	}

	static void InitMetatableIdxFromName() {
		_metatable_idx_from_name["PillEffect"] = Metatables::PILL_EFFECT;
		_metatable_idx_from_name["EntityTear"] = Metatables::ENTITY_TEAR;
		_metatable_idx_from_name["Seeds"] = Metatables::SEEDS;
		_metatable_idx_from_name["ActiveItemDesc"] = Metatables::ACTIVE_ITEM_DESC;
		_metatable_idx_from_name["Entity"] = Metatables::ENTITY;
		_metatable_idx_from_name["EntityBomb"] = Metatables::ENTITY_BOMB;
		_metatable_idx_from_name["Config"] = Metatables::CONFIG;
		_metatable_idx_from_name["ItemConfigList"] = Metatables::ITEM_CONFIG_LIST;
		_metatable_idx_from_name["EntityKnife"] = Metatables::ENTITY_KNIFE;
		_metatable_idx_from_name["Level"] = Metatables::LEVEL;
		_metatable_idx_from_name["PillEffect"] = Metatables::PILL_EFFECT;
		_metatable_idx_from_name["PillConfigList"] = Metatables::PILL_CONFIG_LIST;
		_metatable_idx_from_name["EntityEffect"] = Metatables::ENTITY_EFFECT;
		_metatable_idx_from_name["EntityPlayer"] = Metatables::ENTITY_PLAYER;
		_metatable_idx_from_name["Font"] = Metatables::FONT;
		_metatable_idx_from_name["FontRenderSettings"] = Metatables::FONTRENDERSETTINGS;
		_metatable_idx_from_name["EntityPickup"] = Metatables::ENTITY_PICKUP;
		_metatable_idx_from_name["EntityList"] = Metatables::ENTITY_LIST;
		_metatable_idx_from_name["Game"] = Metatables::GAME;
		_metatable_idx_from_name["EntityNPC"] = Metatables::ENTITY_NPC;
		_metatable_idx_from_name["EntityProjectile"] = Metatables::ENTITY_PROJECTILE;
		_metatable_idx_from_name["EntityFamiliar"] = Metatables::ENTITY_FAMILIAR;
		_metatable_idx_from_name["intValues"] = Metatables::INT_VALUES;
		_metatable_idx_from_name["ItemPool"] = Metatables::ITEM_POOL;
		_metatable_idx_from_name["EntityPtr"] = Metatables::ENTITY_PTR;
		_metatable_idx_from_name["PathFinder"] = Metatables::PATHFINDER;
		_metatable_idx_from_name["Card"] = Metatables::CARD;
		_metatable_idx_from_name["EntityLaser"] = Metatables::ENTITY_LASER;
		_metatable_idx_from_name["CardConfigList"] = Metatables::CARD_CONFIG_LIST;

		_metatable_idx_from_name["const PillEffect"] = Metatables::CONST_PILL_EFFECT;
		_metatable_idx_from_name["const EntityTear"] = Metatables::CONST_ENTITY_TEAR;
		_metatable_idx_from_name["const Seeds"] = Metatables::CONST_SEEDS;
		_metatable_idx_from_name["const ActiveItemDesc"] = Metatables::CONST_ACTIVE_ITEM_DESC;
		_metatable_idx_from_name["const Entity"] = Metatables::CONST_ENTITY;
		_metatable_idx_from_name["const EntityBomb"] = Metatables::CONST_ENTITY_BOMB;
		_metatable_idx_from_name["const Config"] = Metatables::CONST_CONFIG;
		_metatable_idx_from_name["const ItemConfigList"] = Metatables::CONST_ITEM_CONFIG_LIST;
		_metatable_idx_from_name["const EntityKnife"] = Metatables::CONST_ENTITY_KNIFE;
		_metatable_idx_from_name["const Level"] = Metatables::CONST_LEVEL;
		_metatable_idx_from_name["const PillConfigList"] = Metatables::CONST_PILL_CONFIG_LIST;
		_metatable_idx_from_name["const EntityEffect"] = Metatables::CONST_ENTITY_EFFECT;
		_metatable_idx_from_name["const EntityPlayer"] = Metatables::CONST_ENTITY_PLAYER;
		_metatable_idx_from_name["const Font"] = Metatables::CONST_FONT;
		_metatable_idx_from_name["const FontRenderSettings"] = Metatables::CONST_FONTRENDERSETTINGS;
		_metatable_idx_from_name["const EntityPickup"] = Metatables::CONST_ENTITY_PICKUP;
		_metatable_idx_from_name["const EntityList"] = Metatables::CONST_ENTITY_LIST;
		_metatable_idx_from_name["const Game"] = Metatables::CONST_GAME;
		_metatable_idx_from_name["const EntityNPC"] = Metatables::CONST_ENTITY_NPC;
		_metatable_idx_from_name["const EntityProjectile"] = Metatables::CONST_ENTITY_PROJECTILE;
		_metatable_idx_from_name["const EntityFamiliar"] = Metatables::CONST_ENTITY_FAMILIAR;
		_metatable_idx_from_name["const intValues"] = Metatables::CONST_INT_VALUES;
		_metatable_idx_from_name["const ItemPool"] = Metatables::CONST_ITEM_POOL;
		_metatable_idx_from_name["const EntityPtr"] = Metatables::CONST_ENTITY_PTR;
		_metatable_idx_from_name["const PathFinder"] = Metatables::CONST_PATHFINDER;
		_metatable_idx_from_name["const Card"] = Metatables::CONST_CARD;
		_metatable_idx_from_name["const EntityLaser"] = Metatables::CONST_ENTITY_LASER;
		_metatable_idx_from_name["const CardConfigList"] = Metatables::CONST_CARD_CONFIG_LIST;
	}

	Metatables GetMetatableIdxFromName(std::string const& name) {
		if (!_metatable_idx_from_name_initialized) {
			InitMetatableIdxFromName();
			_metatable_idx_from_name_initialized = true;
		}

		auto iter = _metatable_idx_from_name.find(name);
		if (iter == _metatable_idx_from_name.end()) {
			return Metatables::METATABLES_MAX;
		}

		return _metatable_idx_from_name[name];
	}

	void* TestUserdata(lua_State* L, int ud, lua::Metatables mt) {
		// s = ... userdata ...
		void* p = lua_touserdata(L, ud);
		if (p != NULL) {
			lua::PushMetatable(L, mt); // ... userdata ... meta
			if (lua_getmetatable(L, ud)) { // ... userdata ... meta meta
				while (true) {
					if (!lua_rawequal(L, -1, -2)) {
						// Check parent metatable
						lua_pushstring(L, "__parent"); // ... userdata ... meta meta __parent
						int type = lua_rawget(L, -2); // ... userdata ... meta meta ?parent
						if (type != LUA_TTABLE) {
							// Pop the metatable we compare against, the metatable of the userdata
							// and its attempted parent
							lua_pop(L, 3);
							return NULL;
						}

						lua_remove(L, -2); // ... userdata ... meta parentmeta
					}
					else {
						lua_pop(L, 2);
						return p;
					}
				}
				// Proof that the while finishes in normal working conditions :
				// The else case of the if is trivial. Proof that the if iteself finishes
				// The if can not finish only if lua_rawget(L, -2) never produces something that is not a table.
				// This means there is an infinite chain of __parent, i.e. a loop.
				// Then this means the application is bugged and we are no longer in normal work conditions.
				// Otherwise, the chain of __parent is bounded and the inner if will eventually be entered, therefore the while finishes.

				// Pop the metatable we compare against and the metatable of the userdata
				lua_pop(L, 2);
				return p;
			}
			else {
				// Pop the metatable we compare against
				lua_pop(L, 1); // ... userdata ...
				return NULL;
			}
		}
		return NULL;
	}

	void* CheckUserdata(lua_State* L, int ud, lua::Metatables mt, std::string const& name) {
		void* p = TestUserdata(L, ud, mt);
		if (!p) {
			lua_getmetatable(L, ud);
			lua::PushMetatable(L, mt);
			std::string type = lua_typename(L, lua_type(L, ud));
			std::string err = name + " expected, got " + type;
			luaL_argerror(L, ud, err.c_str());
		}
		return p;
	}

	void* CheckUserdata(lua_State* L, int ud, lua::Metatables mt, lua::Metatables constMt, std::string const& name) {
		void* p = TestUserdata(L, ud, mt);
		if (!p) {
			p = TestUserdata(L, ud, constMt);
			if (!p) {
				lua_getmetatable(L, ud);
				lua::PushMetatable(L, mt);
				std::string type = lua_typename(L, lua_type(L, ud));
				std::string err = name + " expected, got " + type;
				luaL_argerror(L, ud, err.c_str());
			}
		}
		return p;
	}

	void* TestCData(lua_State* L, int ud, lua_CTypeId ctypeid) {
		if (lua_cdata_matches(L, ud, ctypeid))
			return lua_tocdata(L, ud);
		return NULL;
	}

	void* CheckCData(lua_State* L, int ud, lua_CTypeId ctypeid, std::string const& name) {
		void* p = TestCData(L, ud, ctypeid);
		if (!p) {
			std::string type = lua_typename(L, lua_type(L, ud));
			std::string err = name + " expected, got " + type;
			luaL_argerror(L, ud, err.c_str());
		}
		return p;
	}

	void RegisterFunction(lua_State* L, lua::Metatables mt, const char* name, lua_CFunction func) {
		lua::PushMetatable(L, mt);
		lua_pushstring(L, name);
		lua_pushcfunction(L, func);
		lua_rawset(L, -3);
		lua_pop(L, 1);
	}

	void RegisterFunctions(lua_State* L, lua::Metatables mt, luaL_Reg* functions) {
		luaL_Reg* ptr = functions;
		lua::PushMetatable(L, mt);
		while (const char* name = ptr->name) {
			lua_pushstring(L, name);
			lua_pushcfunction(L, ptr->func);
			lua_rawset(L, -3);

			++ptr;
		}
		lua_pop(L, 1);
	}

	void RegisterGlobalClassFunction(lua_State* L, const char* className, const char* funcName, lua_CFunction func) {
		lua_getglobal(L, className);
		lua_pushstring(L, funcName);
		lua_pushcfunction(L, func);
		lua_rawset(L, -3);
		lua_pop(L, 1);
	}

	void RegisterVariable(lua_State* L, lua::Metatables mt, const char* variableName, lua_CFunction getFunc, lua_CFunction setFunc) {
		lua::PushMetatable(L, mt);
		RegisterVariableToLoadedMT(L, variableName, getFunc, setFunc);
	}

	void RegisterVariableGetter(lua_State* L, lua::Metatables mt, const char* variableName, lua_CFunction func) {
		lua::PushMetatable(L, mt);
		RegisterVariableGetterToLoadedMT(L, variableName, func);
	}

	void RegisterVariableSetter(lua_State* L, lua::Metatables mt, const char* variableName, lua_CFunction func) {
		lua::PushMetatable(L, mt);
		RegisterVariableSetterToLoadedMT(L, variableName, func);
	}

	void RegisterGlobalClassVariable(lua_State* L, const char* className, const char* variableName, lua_CFunction getFunc, lua_CFunction setFunc) {
		lua_getglobal(L, className);
		RegisterVariableToLoadedMT(L, variableName, getFunc, setFunc);
	}

	void RegisterGlobalClassVariableGetter(lua_State* L, const char* className, const char* variableName, lua_CFunction func) {
		lua_getglobal(L, className);
		RegisterVariableGetterToLoadedMT(L, variableName, func);
	}

	void RegisterGlobalClassVariableSetter(lua_State* L, const char* className, const char* variableName, lua_CFunction func) {
		lua_getglobal(L, className);
		RegisterVariableSetterToLoadedMT(L, variableName, func);
	}

	void RegisterVariableToLoadedMT(lua_State* L, const char* variableName, lua_CFunction getFunc, lua_CFunction setFunc) {
		if (getFunc) {
			RegisterVariableGetterToLoadedMT(L, variableName, getFunc, 1);
		}

		if (setFunc) {
			RegisterVariableSetterToLoadedMT(L, variableName, setFunc, 1);
		}

		lua_pop(L, 1);
	}

	void RegisterVariableGetterToLoadedMT(lua_State* L, const char* variableName, lua_CFunction func, int pop) {
		lua_pushstring(L, "__propget"); // table, key
		lua_rawget(L, -2); // table, value

		lua_pushstring(L, variableName); // table, value, string
		lua_pushcfunction(L, func); // table, value, string, function
		lua_rawset(L, -3); // table, value
		lua_pop(L, pop);
	}

	void RegisterVariableSetterToLoadedMT(lua_State* L, const char* variableName, lua_CFunction func, int pop) {
		lua_pushstring(L, "__propset");
		lua_rawget(L, -2);

		lua_pushstring(L, variableName);
		lua_pushcfunction(L, func);
		lua_rawset(L, -3);
		lua_pop(L, pop);
	}

	void TracebackTillFunction(lua_State* L, const char* msg, int level, lua_CFunction function)
	{
		luaL_Buffer b;
		lua_Debug ar;
		luaL_buffinit(L, &b);
		if (msg) {
			luaL_addstring(&b, msg);
			luaL_addchar(&b, '\n');
		}

		luaL_addstring(&b, "Begin Stack Traceback:\n");
		while (lua_getstack(L, level++, &ar))
		{
			lua_getinfo(L, "f", &ar);

			if (lua_iscfunction(L, -1))
			{
				lua_CFunction fn = lua_tocfunction(L, -1);
				if (fn == function) break;
			}

			lua_getinfo(L, "Sln", &ar);
			if (*ar.what == 'C')
			{
				lua_pushfstring(L, "  %s: in method %s\n", ar.short_src, ar.name ? ar.name : "?");
			}
			else
			{
				if (ar.name == '\0')
				{
					lua_pushfstring(L, "  %s:%d: in function at line %d\n", ar.short_src, ar.currentline, ar.linedefined);
				}
				else
				{
					lua_pushfstring(L, "  %s:%d: in function '%s'\n", ar.short_src, ar.currentline, ar.name);
				}
			}
			luaL_addvalue(&b);
		}
		luaL_addstring(&b, "End Stack Traceback");
		luaL_pushresult(&b);
	}

	namespace luabridge {
		UserdataPtr::UserdataPtr(void* const p) {
			m_p = p;
		}

		const void* UserdataPtr::GetVTable() {
			UserdataPtr ud(nullptr);
			return *reinterpret_cast<void* const*>(&ud);
		}

		static void* sUserdataCacheToken = nullptr;

		void UserdataPtr::push(lua_State* L, void* const p, void const* const key) {
			if (p) {
				lua_rawgetp(L, LUA_REGISTRYINDEX, &sUserdataCacheToken);
				if (lua_isnil(L, -1)) {
					lua_pop(L, 1);
					lua_newtable(L);
					lua_rawsetp(L, LUA_REGISTRYINDEX, &sUserdataCacheToken);
					lua_rawgetp(L, LUA_REGISTRYINDEX, &sUserdataCacheToken);
				}
				const int cacheIdx = lua_gettop(L);

				lua_rawgetp(L, cacheIdx, key);
				if (lua_isnil(L, -1)) {
					lua_pop(L, 1);
					lua_newtable(L);
					lua_newtable(L);
					lua_pushliteral(L, "v");
					lua_setfield(L, -2, "__mode");
					lua_setmetatable(L, -2);
					lua_pushvalue(L, -1);
					lua_rawsetp(L, cacheIdx, key);
				}
				const int innerIdx = lua_gettop(L);

				lua_pushlightuserdata(L, p);
				lua_rawget(L, innerIdx);
				if (lua_type(L, -1) != LUA_TNIL) {
					lua_remove(L, innerIdx);
					lua_remove(L, cacheIdx);
					return;
				}

				lua_pop(L, 1);
				new (lua_newuserdata(L, sizeof(UserdataPtr))) UserdataPtr(p);
				lua_rawgetp(L, LUA_REGISTRYINDEX, key);
				lua_setmetatable(L, -2);
				lua_pushlightuserdata(L, p);
				lua_pushvalue(L, -2);
				lua_rawset(L, innerIdx);
				lua_remove(L, innerIdx);
				lua_remove(L, cacheIdx);
			}
			else {
				lua_pushnil(L);
			}
		}

		void UserdataPtr::push(lua_State* L, void* const p, const char* meta) {
			if (p) {
				new (lua_newuserdata(L, sizeof(UserdataPtr))) UserdataPtr(p);
				luaL_setmetatable(L, meta);
			}
			else {
				lua_pushnil(L);
			}
		}

		void UserdataPtr::push(lua_State* L, void* const p, lua::Metatables mt) {
			void* key = lua::GetMetatableKey(mt);
			push(L, p, key);
		}

		void* identityKey;
		static VariableDefinition identityKeyDef("luabridge::IdentityKey", "5357FF15????????68(????????)", &identityKey);

		lua_CFunction indexMetaMethod;

		namespace index {
			static const HookSystem::ArgData* argdata = nullptr;
			static FunctionDefinition indexMetaMethodDef("luabrige::Namespace::ClassBase::indexMetaMethod", "", typeid(lua_CFunction),
				"558bec83ec0c53568b7508576a0156ff15????????6a0256ff15", argdata, 1, 0, (void**)&indexMetaMethod, true);
		}

		lua_CFunction newIndexMetaMethod;
		namespace newIndex {
			static const HookSystem::ArgData* argdata = nullptr;
			static FunctionDefinition newIndexMetaMethodDef("luabridge::Namespace::ClassBase::newIndexMetaMethod", "", typeid(lua_CFunction),
				"558bec53568b7508576a0156ff15????????8b1d", argdata, 1, 0, (void**)&newIndexMetaMethod, true);
		}
	}

	namespace callbacks {
		bool CheckInteger(lua_State* L, int stackPosition) {
			lua_pushinteger(L, stackPosition);
			lua_gettable(L, -2);
			bool res = (int)lua_isinteger(L, -1);
			lua_pop(L, 1);
			return res;
		}

		int ToInteger(lua_State* L, int stackPosition) {
			lua_pushinteger(L, stackPosition);
			lua_gettable(L, -2);
			int res = (int)luaL_checkinteger(L, -1);
			lua_pop(L, 1);
			return res;
		}

		double ToNumber(lua_State* L, int stackPosition) {
			lua_pushinteger(L, stackPosition);
			lua_gettable(L, -2);
			double res = luaL_checknumber(L, -1);
			lua_pop(L, 1);
			return res;
		}

		bool ToBoolean(lua_State* L, int stackPosition) {
			lua_pushinteger(L, stackPosition);
			lua_gettable(L, -2);
			bool res = luaL_checkboolean(L, -1);
			lua_pop(L, 1);
			return res;
		}

		const char* ToString(lua_State* L, int stackPosition) {
			lua_pushinteger(L, stackPosition);
			lua_gettable(L, -2);
			const char* res = luaL_checkstring(L, -1);
			lua_pop(L, 1);
			return res;
		}
	}

	bool luaL_optboolean(lua_State* L, int idx, bool default) {
		if (lua_gettop(L) < idx) {
			return default;
		}
		else {
			return lua_toboolean(L, idx);
		}
	}

	bool luaL_checkboolean(lua_State* L, int idx, BoolCheckModes mode) {
		switch (mode) {
		case BOOL_CHECK_MODE_NOT_NIL:
			if (lua_isnil(L, idx)) {
				return luaL_error(L, "Expected non nil boolean as parameter #%d\n", idx);
			}
			return lua_toboolean(L, idx);

		case BOOL_CHECK_MODE_STRICT:
			if (lua_type(L, idx) != LUA_TBOOLEAN) {
				return luaL_error(L, "Stricly expected boolean as parameter #%d\n", idx);
			}
			return lua_toboolean(L, idx);
		default:
			return lua_toboolean(L, idx);
		}
	}

	uint64_t luaL_checkuint64(lua_State* L, int idx) {
		const int t = lua_type(L, idx);
		if (t == LUA_TNUMBER) {
			return (uint64_t)lua_tonumber(L, idx);
		}
		if (t == LUA_TCDATA) {
			void* payload = lua_tocdata(L, idx);
			if (payload) {
				return *(uint64_t*)payload;
			}
		}
		luaL_argerror(L, idx, "number or int64 expected");
		return 0;
	}

	LuaCaller::LuaCaller(lua_State* L) : _L(L) { }

	LuaCaller& LuaCaller::push(bool x) {
		lua_pushboolean(_L, x);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::push(lua_CFunction f, int n) {
		if (n != 0) {
			lua_pushcclosure(_L, f, n);
		}
		else {
			lua_pushcfunction(_L, f);
		}

		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::pushfstring(const char* fmt, ...) {
		va_list va;
		va_start(va, fmt);
		return push(fmt, va);
	}

	LuaCaller& LuaCaller::push(lua_global_tag_t) {
		lua_pushglobaltable(_L);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::push(void* p) {
		lua_pushlightuserdata(_L, p);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::push(const char* s, size_t len) {
		if (len == 0) {
			lua_pushstring(_L, s);
		}
		else {
			lua_pushlstring(_L, s, len);
		}

		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::pushnil() {
		lua_pushnil(_L);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::pushvalue(int idx) {
		lua_pushvalue(_L, idx);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::pushluaref(int idx, int ref) {
		lua_rawgeti(_L, idx, ref);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::pushluaref(int ref) {
		return pushluaref(LUA_REGISTRYINDEX, ref);
	}

	LuaCaller& LuaCaller::push(const char* fmt, va_list va) {
		lua_pushvfstring(_L, fmt, va);
		++_n;
		return *this;
	}

	LuaCaller& LuaCaller::pushCallbackID(const char* name, const char* ns) {
		PushCallbackID(_L, name, ns);
		++_n;
		return *this;
	}

	void LuaCaller::pushTable(int narr, int nrec) {
		lua_createtable(_L, narr, nrec);
		++_n;
	}

	LuaResults LuaCaller::call(int nresults) {
		int n = lua_gettop(_L) - _n - 1; // Expected amount after poping everything (number of params + function)
		int result = lua_pcall(_L, _n, nresults, 0);
		int results = lua_gettop(_L) - n; // How many results

		return LuaResults(_L, results, result);
	}

	LuaStackProtector::LuaStackProtector(lua_State* L, int n) : _L(L), _orig(lua_gettop(L)), _n(n) {

	}

	LuaStackProtector::~LuaStackProtector() {
		if (_L) {
			int n = lua_gettop(_L);
			if (n != _orig + _n) {
				ZHL::Logger logger(true);
				logger.Log("Inconsistent Lua stack, expected %d elements, got %d\n", _orig + _n, n);
				logger.DumpLuaStack(_L);
				abort();
			}
		}
	}

	LuaStackProtector::LuaStackProtector(LuaStackProtector&& other) : _L(other._L), _n(other._n) {
		other._L = nullptr;
	}

	LuaStackProtector& LuaStackProtector::operator=(LuaStackProtector&& other) {
		_L = other._L;
		other._L = nullptr;
		return *this;
	}

	LuaResults::LuaResults(lua_State* L, int n, int resultCode) : _L(L), _n(n), _resultCode(resultCode) {

	}

	LuaResults::LuaResults(LuaResults&& other) : _L(other._L), _n(other._n), _resultCode(other._resultCode) {
		other._L = nullptr;
	}

	LuaResults& LuaResults::operator=(LuaResults&& other) {
		_L = other._L;
		_n = other._n;
		_resultCode = other._resultCode;
		other._L = nullptr;
		return *this;
	}

	LuaResults::~LuaResults() {
		// For consistency with lua_pcall returning 0 when everything is okay
		if (_L) {
			if (_resultCode == LUA_OK) {
				lua_pop(_L, _n);
			}
			else {
				// If an error occured during lua_pcall, there is an error object at the top
				// of the stack. Pop it now.
				lua_pop(_L, 1);
			}
		}
	}

	LuaResults::operator bool() const {
		return _resultCode;
	}

	namespace GlobalClasses
	{
		const char* Isaac = "Isaac";
		const char* HUD = "HUD";
		const char* Options = "Options";
	}

	namespace metatables
	{;
		const char* EntityConfigMT = "EntityConfig";
		const char* EntityConfigEntityMT = "EntityConfigEntity";
		const char* EntityConfigPlayerMT = "EntityConfigPlayer";
		const char* EntityConfigBabyMT = "EntityConfigBaby";
		const char* EntitySlotMT = "EntitySlot";
		const char* DeliriumMetatable = "DeliriumMT";
		const char* ImGuiMT = "ImGui";
		const char* RoomDescriptorDoors = "RoomDescriptorDoors";
		const char* RoomDescriptorDoorsConst = "RoomDescriptorDoorsConst";
	}

	void TableAssoc(lua_State* L, std::string const& name, int value) {
		lua_pushstring(L, name.c_str());
		lua_pushinteger(L, value);
		lua_rawset(L, -3);
	}

	void TableAssoc(lua_State* L, std::string const& name, float f) {
		lua_pushstring(L, name.c_str());
		lua_pushnumber(L, f);
		lua_rawset(L, -3);
	}

	void TableAssoc(lua_State* L, std::string const& name, lua_CFunction fn) {
		lua_pushstring(L, name.c_str());
		lua_pushcfunction(L, fn);
		lua_rawset(L, -3);
	}

	void TableAssoc(lua_State* L, std::string const& name, void* ptr) {
		lua_pushstring(L, name.c_str());
		lua_pushlightuserdata(L, ptr);
		lua_rawset(L, -3);
	}

	void TableAssoc(lua_State* L, std::string const& name, LuaStackRef dstTable, LuaStackRef srcObj) {
		int src = lua_absindex(L, srcObj);
		int dst = lua_absindex(L, dstTable);

		lua_pushstring(L, name.c_str());
		lua_pushvalue(L, src);
		lua_rawset(L, dst);

		lua_remove(L, src);
	}

	void TableAssoc(lua_State* L, int key, int value) {
		lua_pushinteger(L, key);
		lua_pushinteger(L, value);
		lua_rawset(L, -3);
	}

	void TableAssoc(lua_State* L, int key, float f) {
		lua_pushinteger(L, key);
		lua_pushnumber(L, f);
		lua_rawset(L, -3);
	}

	void TableAssoc(lua_State* L, int key, lua_CFunction fn) {
		lua_pushinteger(L, key);
		lua_pushcfunction(L, fn);
		lua_rawset(L, -3);
	}

	void TableAssoc(lua_State* L, int key, void* ptr) {
		lua_pushinteger(L, key);
		lua_pushlightuserdata(L, ptr);
		lua_rawset(L, -3);
	}

	void TableAssoc(lua_State* L, int key, LuaStackRef dstTable, LuaStackRef srcObj) {
		int src = lua_absindex(L, srcObj);
		int dst = lua_absindex(L, dstTable);

		lua_pushinteger(L, key);
		lua_pushvalue(L, src);
		lua_rawset(L, dst);

		lua_remove(L, src);
	}

	void PushCallbackID(lua_State* L, const char* name, const char* ns) {
		if (ns && strcmp(ns, "")) {
			int type = lua_getglobal(L, ns);
			if (type == LUA_TNIL) {
				std::ostringstream err;
				err << "Request for callback named " << name << " in non existent namespace " << ns << std::endl;
				throw std::runtime_error(err.str());
			}
			else if (type != LUA_TTABLE) {
				std::ostringstream err;
				err << "Request for callback named " << name << " in namespace " << ns << " that is not a table" << std::endl;
				throw std::runtime_error(err.str());
			}
		}
		else {
			lua_getglobal(L, "ModCallbacks");
		}

		lua_pushstring(L, name);
		int type = lua_rawget(L, -2);

		if (type == LUA_TNIL) {
			std::ostringstream err;
			err << "Request for non existent callback named " << name << " in namespace " << ns << std::endl;
			throw std::runtime_error(err.str());
		}

		lua_remove(L, -2); // Pop the table from the stack (rawget only pops the key)
	}

	void PushCallbackRegistryKey(lua_State* L) {
		if (!L) {
			L = g_LuaEngine->_state;
		}

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);
	}

	int LuaCheckMainMenuExists(lua_State* L, const char* className) {
		if (g_MenuManager == NULL) { return luaL_error(L, "%s functions can only be used in the main menu", className); }
		return 0;
	}
}
