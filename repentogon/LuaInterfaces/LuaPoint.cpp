#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PointConstructor) {
	Point point;
	point._pos = *lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	point._spritesheetCoordinate = (float)luaL_checknumber(L, 2);
	point._width = (float)luaL_optnumber(L, 3, 1.0f);

	Point* toLua = lua::place<Point>(L, lua::metatables::PointMT);
	*toLua = point;
	luaL_setmetatable(L, lua::metatables::PointMT);

	return 1;
}

LUA_FUNCTION(Lua_PointGetSpritesheetCoordinate) {
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	lua_pushnumber(L, point->_spritesheetCoordinate);
	return 1;
}

LUA_FUNCTION(Lua_PointSetSpritesheetCoordinate) {
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	point->_spritesheetCoordinate = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PointGetWidth) {
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	lua_pushnumber(L, point->_width);
	return 1;
}

LUA_FUNCTION(Lua_PointSetWidth) {
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	point->_width = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PointGetPosition) {
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = point->_pos;
	return 1;
}

LUA_FUNCTION(Lua_PointSetPosition) {
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	point->_lastPos = point->_pos;
	point->_pos = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(Lua_PointGetFixed) {
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	lua_pushboolean(L, point->_fixed);
	return 1;
}

LUA_FUNCTION(Lua_PointSetFixed) {
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	point->_fixed = lua::luaL_checkboolean(L, 2);
	if (point->_fixed)
		point->_lastPos = point->_pos;
	return 0;
}

/*////////////////////
//// Deque time
*/////////////////////

LUA_FUNCTION(Lua_PointDequePushBack) {
	PointDeque* d = lua::GetUserdata<PointDeque*>(L, 1, lua::metatables::PointDequeMT);
	Point* point = lua::GetUserdata<Point*>(L, 2, lua::metatables::PointMT);
	d->deque.push_back(*point);

	return 0;
}

LUA_FUNCTION(Lua_PointDequePushFront) {
	PointDeque* d = lua::GetUserdata<PointDeque*>(L, 1, lua::metatables::PointDequeMT);
	Point* point = lua::GetUserdata<Point*>(L, 2, lua::metatables::PointMT);
	d->deque.push_front(*point);

	return 0;
}

LUA_FUNCTION(Lua_PointDequePopBack) {
	PointDeque* d = lua::GetUserdata<PointDeque*>(L, 1, lua::metatables::PointDequeMT);
	d->deque.pop_back();

	return 0;
}

LUA_FUNCTION(Lua_PointDequePopFront) {
	PointDeque* d = lua::GetUserdata<PointDeque*>(L, 1, lua::metatables::PointDequeMT);
	d->deque.pop_front();

	return 0;
}

LUA_FUNCTION(Lua_PointDeque__gc) {
	PointDeque* d = lua::GetUserdata<PointDeque*>(L, 1, lua::metatables::PointDequeMT);
	d->~PointDeque();
	return 0;
}

static void RegisterPoint(lua_State* L) {
	luaL_Reg pointFunctions[] = {
		{ "GetSpritesheetCoordinate", Lua_PointGetSpritesheetCoordinate},
		{ "SetSpritesheetCoordinate", Lua_PointSetSpritesheetCoordinate},
		{ "GetHeight", Lua_PointGetSpritesheetCoordinate}, // deprecated
		{ "SetHeight", Lua_PointSetSpritesheetCoordinate}, // deprecated
		{ "GetWidth", Lua_PointGetWidth},
		{ "SetWidth", Lua_PointSetWidth},
		{ "GetPosition", Lua_PointGetPosition},
		{ "SetPosition", Lua_PointSetPosition},
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::PointMT, lua::metatables::PointMT, pointFunctions);
	lua_register(L, lua::metatables::PointMT, Lua_PointConstructor);

	luaL_Reg dequeFunctions[] = {
		{ "PushBack", Lua_PointDequePushBack},
		{ "PushFront", Lua_PointDequePushFront },
		{ "PopBack", Lua_PointDequePopBack },
		{ "PopFront", Lua_PointDequePopFront },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::PointDequeMT, lua::metatables::PointDequeMT, dequeFunctions, Lua_PointDeque__gc);
	//lua_register(L, lua::metatables::PointDequeMT, Lua_PointConstructor);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterPoint(_state);
}