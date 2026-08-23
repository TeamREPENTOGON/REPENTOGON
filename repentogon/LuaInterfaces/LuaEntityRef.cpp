#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EntityRef_GetPosition) {
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 1, lua::Metatables::ENTITY_REF, "EntityRef");

	lua::ffi::pushCdata(L, lua::ffi::CData[lua::ffi::CDataID::VECTOR], ref->_position);
	return 1;
}

LUA_FUNCTION(Lua_EntityRef_SetPosition) {
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 1, lua::Metatables::ENTITY_REF, "EntityRef");
	Vector* position = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");

	ref->_position = *position;
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	lua::RegisterVariable(_state, lua::Metatables::ENTITY_REF, "Position", Lua_EntityRef_GetPosition, Lua_EntityRef_SetPosition);
}