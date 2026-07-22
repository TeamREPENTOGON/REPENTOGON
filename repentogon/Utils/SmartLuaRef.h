#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"

class SmartLuaRef {
  public:
	SmartLuaRef() = default;
	explicit SmartLuaRef(int r) : ref_(r) {}

	~SmartLuaRef() {
		Free();
	}

	// Disable copying
	SmartLuaRef(const SmartLuaRef&) = delete;
	SmartLuaRef& operator=(const SmartLuaRef&) = delete;

	// Move constructor
	SmartLuaRef(SmartLuaRef&& other) : ref_(other.ref_) {
		other.ref_ = LUA_NOREF;
	}

	// Move assignment
	SmartLuaRef& operator=(SmartLuaRef&& other) {
		if (this != &other) {
			Free();
			ref_ = other.ref_;
			other.ref_ = LUA_NOREF;
		}
		return *this;
	}

	int Get() {
		return ref_;
	}

	void Set(int r) {
		Free();
		ref_ = r;
	}

	void Free(lua_State* L) {
		if (ref_ != LUA_NOREF && L) {
			luaL_unref(L, LUA_REGISTRYINDEX, ref_);
			ref_ = LUA_NOREF;
		}
	}

	void Free() {
		if (g_LuaEngine) {
			Free(g_LuaEngine->_state);
		}
	}

	operator int() const { return ref_; }

  private:
	  int ref_ = LUA_NOREF;
};
