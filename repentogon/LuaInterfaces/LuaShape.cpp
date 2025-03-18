#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EntityGetDebugShape) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool unk = lua::luaL_checkboolean(L, 2);
	Shape** ud = (Shape**)lua_newuserdata(L, sizeof(Shape*));
	*ud = g_Game->GetDebugRenderer()->Get(ent->GetIndex(), unk);
	luaL_setmetatable(L, lua::metatables::ShapeMT);

	return 1;
}

LUA_FUNCTION(Lua_ShapeGetTimeout) {
	Shape* shape = *lua::GetRawUserdata<Shape**>(L, 1, lua::metatables::ShapeMT);
	lua_pushinteger(L, *shape->GetTimeout());
	return 1;
}

LUA_FUNCTION(Lua_ShapeSetTimeout) {
	Shape* shape = *lua::GetRawUserdata<Shape**>(L, 1, lua::metatables::ShapeMT);
	*shape->GetTimeout() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_ShapeCapsule) {
	Shape* shape = *lua::GetRawUserdata<Shape**>(L, 1, lua::metatables::ShapeMT);
	Capsule* cap1 = lua::GetRawUserdata<Capsule*>(L, 2, lua::metatables::CapsuleMT);
	shape->Capsula(cap1);
	return 0;
}

LUA_FUNCTION(Lua_ShapeCircle) {
	Shape* shape = *lua::GetRawUserdata<Shape**>(L, 1, lua::metatables::ShapeMT);
	Vector* pos = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	const float size = (float)luaL_checknumber(L, 3);
	shape->Circle(pos, size);
	return 0;
}

static void RegisterShape(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY, "GetDebugShape", Lua_EntityGetDebugShape);

	luaL_Reg functions[] = {
		{ "GetTimeout", Lua_ShapeGetTimeout },
		{ "SetTimeout", Lua_ShapeSetTimeout },
		{ "Capsule", Lua_ShapeCapsule},
		{ "Circle", Lua_ShapeCircle},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::ShapeMT, lua::metatables::ShapeMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	
	lua::LuaStackProtector protector(_state);
	RegisterShape(_state);
}

