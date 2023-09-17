#include <Windows.h>

#include <lua.hpp>

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
}

static void RegisterDebugMethods(lua_State* L) {
	lua_newtable(L);
	lua_pushstring(L, "GetSignature");
	lua_pushcfunction(L, Debug::GetSignature);
	lua_rawset(L, -3);
	lua_setglobal(L, "Debug");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterDebugMethods(_state);
}