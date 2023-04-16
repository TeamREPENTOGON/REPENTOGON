#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_SpriteGetNullFrame(lua_State* L)
{
	ANM2* sprite = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const char* nullLayerName = luaL_checkstring(L, 2);
	NullFrame* toLua = sprite->GetNullFrame(nullLayerName);
	if (toLua == nullptr) {
		lua_pushnil(L);
		return 1;
	}
	NullFrame** luaNullFrame = (NullFrame**)lua_newuserdata(L, sizeof(NullFrame*));
	*luaNullFrame = toLua;
	luaL_setmetatable(L, lua::metatables::NullFrameMT);
	return 1;
}

static int Lua_NullFrameGetScale(lua_State* L)
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
	lua::PushMetatable(L, lua::Metatables::SPRITE);
	lua_pushstring(L, "GetNullFrame");
	lua_pushcfunction(L, Lua_SpriteGetNullFrame);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, lua::metatables::NullFrameMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {
		{ "GetPos", Lua_NullFrameGetPos},
		{ "GetScale", Lua_NullFrameGetScale},
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