#include <Windows.h>

#include <regex>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Signature.h"

namespace Debug {
	LUA_FUNCTION(GetSignature) {
		lua_Integer addr = luaL_checkinteger(L, 1);
		if (addr < 0) {
			return luaL_error(L, "Invalid address %lld\n", addr);
		}

		uintptr_t asUptr = (uintptr_t)addr;
		if (asUptr != addr) {
			return luaL_error(L, "GetSignature: address %lld falls outside the virtual address space\n", addr);
		}

		MEMORY_BASIC_INFORMATION info;
		SIZE_T queryResult = VirtualQuery((void*)asUptr, &info, sizeof(info));
		if (!queryResult) {
			DWORD error = GetLastError();
			return luaL_error(L, "GetSignature: VirtualQuery failed with error code %d\n", error);
		}

		Signature sig((void*)addr, NULL, NULL);
		std::optional<ByteBuffer> bytes = sig.Make();

		lua_pushinteger(L, (int)sig.GetStatus());
		if (bytes) {
			lua_pushstring(L, bytes->ToString().c_str());
			return 2;
		}
		else {
			return 1;
		}
	}

	LUA_FUNCTION(ListLoadedFiles) {
		int i = 1;
		lua_settop(L, 0);
		lua_newtable(L); // Index 1
		lua_getfield(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE); // Index 2
		lua_pushnil(L);
		while (lua_next(L, 2) != 0) {
			lua_pushinteger(L, i++); // Key in new table
			lua_pushvalue(L, -3); // Value (key in the LOADED table)
			lua_rawset(L, 1); // Set in new table. 
			lua_pop(L, 1); // Pop the value in the LOADED table (only the key in LOADED remains)
		}
		lua_pop(L, 1); // Pop LOADED
		return 1;
	}

	LUA_FUNCTION(ForceUnload) {
		const char* base = luaL_checkstring(L, 1);
		std::vector<std::string> modules;
		std::regex r("/");

		// Retrieve all module names, replacing the forward slashes in them with dots
		int loadedIndex = 2, resultIndex = 1;
		int i = 1;
		lua_settop(L, 0);
		lua_newtable(L);
		lua_getfield(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE); // Index 2


		lua_pushnil(L);
		while (lua_next(L, loadedIndex) != 0) {
			const char* name = lua_tostring(L, -2);
			modules.push_back(std::regex_replace(name, r, std::string(".")));
			lua_pop(L, 1);
		}

		// Locate all modules that match the input parameter
		auto start = base;
		size_t len = strlen(base);
		auto end = base + len;
		for (std::string& s : modules) {
			if (std::equal(start, end, s.begin())) {
				// lua_getfield(L, 1, s.c_str());
				lua_pushnil(L);
				lua_setfield(L, loadedIndex, s.c_str());
				lua_pushinteger(L, i++);
				lua_pushstring(L, s.c_str());
				lua_rawset(L, resultIndex);
			}
		}

		lua_pop(L, 1);
		return 1;
	}
}

static void RegisterDebugMethods(lua_State* L) {
	lua_newtable(L);
	lua::TableAssoc(L, "GetSignature", Debug::GetSignature);
	lua::TableAssoc(L, "ListLoadedFiles", Debug::ListLoadedFiles);
	lua::TableAssoc(L, "ForceUnload", Debug::ForceUnload);
	lua_setglobal(L, "Debug");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterDebugMethods(_state);
}