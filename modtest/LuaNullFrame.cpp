#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_NullFrameGetSize(lua_State* L)
{
	NullFrame* nullFrame = *lua::GetUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *nullFrame->GetScale();

	return 1;
}

static int Lua_NullFrameGetPos(lua_State* L)
{
	NullFrame* nullFrame = *lua::GetUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *nullFrame->GetPos();

	return 1;
}

static void RegisterNullFrame(lua_State* L)
{
	luaL_newmetatable(L, lua::metatables::NullFrameMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {
		{ "GetPos", Lua_NullFrameGetPos},
		{ "GetSize", Lua_NullFrameGetSize},
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterNullFrame(state);
}