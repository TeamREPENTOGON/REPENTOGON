#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* DebugRendererMT = "DebugRenderer";
static constexpr const char* ShapeMT = "Shape";


static int Lua_GetDebugRenderer(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	DebugRenderer** ud = (DebugRenderer**)lua_newuserdata(L, sizeof(DebugRenderer*));
	*ud = game->GetDebugRenderer();
	luaL_setmetatable(L, DebugRendererMT);
	return 1;
}

static int Lua_DebugRendererGet(lua_State* L) {
	DebugRenderer* debugRenderer = *lua::GetUserdata<DebugRenderer**>(L, 1, DebugRendererMT);
	int index = luaL_checkinteger(L, 2);
	bool unk = lua_toboolean(L, 3);
	Shape** ud = (Shape**)lua_newuserdata(L, sizeof(Shape*));
	*ud = debugRenderer->Get(index, unk);
	luaL_setmetatable(L, ShapeMT);

	return 1;
}

static int Lua_EntityGetDebugShape(lua_State* L) {
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool unk = lua_toboolean(L, 2);
	Shape** ud = (Shape**)lua_newuserdata(L, sizeof(Shape*));
	*ud = g_Game->GetDebugRenderer()->Get(ent->GetIndex(), unk);
	luaL_setmetatable(L, ShapeMT);

	return 1;
}

static int Lua_ShapeGetTimeout(lua_State* L) {
	Shape* shape = *lua::GetUserdata<Shape**>(L, 1, ShapeMT);
	lua_pushinteger(L, *shape->GetTimeout());
	return 1;
}

static int Lua_ShapeSetTimeout(lua_State* L) {
	Shape* shape = *lua::GetUserdata<Shape**>(L, 1, ShapeMT);
	*shape->GetTimeout() = luaL_checkinteger(L, 2);
	return 0;
}

static int Lua_ShapeCapsule(lua_State* L) {
	Shape* shape = *lua::GetUserdata<Shape**>(L, 1, ShapeMT);
	Capsule* cap1 = lua::GetUserdata<Capsule*>(L, 2, lua::metatables::CapsuleMT);
	shape->Capsula(cap1);
	return 0;
}

static void RegisterDebugRenderer(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetDebugRenderer");
	lua_pushcfunction(L, Lua_GetDebugRenderer);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, DebugRendererMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "Get", Lua_DebugRendererGet },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

static void RegisterShape(lua_State* L) {

	luaL_newmetatable(L, ShapeMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetTimeout", Lua_ShapeGetTimeout },
		{ "SetTimeout", Lua_ShapeSetTimeout },
		{ "Capsule", Lua_ShapeCapsule},
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterDebugRenderer(state);
	RegisterShape(state);
	lua::Metatables mt = lua::Metatables::ENTITY;
	lua::RegisterFunction(state, mt, "GetDebugShape", Lua_EntityGetDebugShape);
}

