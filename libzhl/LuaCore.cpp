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

	namespace ffi {
		lua_CTypeId CData[MAX_CDATA];
	}

	namespace {
		struct EntityCDataType {
			lua::ffi::CDataID id;
			lua::ffi::CDataID ptrId;
		};

		const EntityCDataType s_entityCDataTypes[] = {
			{ lua::ffi::ENTITY, lua::ffi::ENTITY_PTR },
			{ lua::ffi::ENTITY_PROJECTILE, lua::ffi::ENTITY_PROJECTILE_PTR },
			{ lua::ffi::ENTITY_TEAR, lua::ffi::ENTITY_TEAR_PTR },
			{ lua::ffi::ENTITY_BOMB, lua::ffi::ENTITY_BOMB_PTR },
			{ lua::ffi::ENTITY_KNIFE, lua::ffi::ENTITY_KNIFE_PTR },
			{ lua::ffi::ENTITY_LASER, lua::ffi::ENTITY_LASER_PTR },
			{ lua::ffi::ENTITY_EFFECT, lua::ffi::ENTITY_EFFECT_PTR },
			{ lua::ffi::ENTITY_PICKUP, lua::ffi::ENTITY_PICKUP_PTR },
			{ lua::ffi::ENTITY_FAMILIAR, lua::ffi::ENTITY_FAMILIAR_PTR },
			{ lua::ffi::ENTITY_SLOT, lua::ffi::ENTITY_SLOT_PTR },
			{ lua::ffi::ENTITY_NPC, lua::ffi::ENTITY_NPC_PTR },
			{ lua::ffi::ENTITY_DELIRIUM, lua::ffi::ENTITY_DELIRIUM_PTR },
			{ lua::ffi::ENTITY_PLAYER, lua::ffi::ENTITY_PLAYER_PTR },
		};
	}

	void* TestEntity(lua_State* L, int ud) {
		if (lua_type(L, ud) != LUA_TCDATA) {
			return NULL;
		}

		for (const EntityCDataType& type : s_entityCDataTypes) {
			if (void* pp = lua::TestCData(L, ud, lua::ffi::CData[type.ptrId])) {
				return *static_cast<void**>(pp);
			}
			if (void* pv = lua::TestCData(L, ud, lua::ffi::CData[type.id])) {
				return pv;
			}
		}
		return NULL;
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

	void RegisterGlobalClassFunction(lua_State* L, const char* className, const char* funcName, lua_CFunction func) {
		lua_getglobal(L, className);
		lua_pushstring(L, funcName);
		lua_pushcfunction(L, func);
		lua_rawset(L, -3);
		lua_pop(L, 1);
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
}
