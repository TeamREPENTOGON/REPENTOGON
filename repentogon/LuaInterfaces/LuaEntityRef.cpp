#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../LuaClasses.h"

// For any class that's FFI'd but has functions that return or consume non FFI'd classes, we define globals, set them on the class's metatable then nil them afterwards.
LUA_FUNCTION(Lua_EntityRefCtor) {
	Entity* ent = LuaEntity::GetOpt(L, 1);

	EntityRef* toLua = LuaEntityRef::Place(L);
	new (toLua) EntityRef(ent);
	return 1;
}

LUA_FUNCTION(Lua_EntityRefGetEntity) {
	EntityRef* ref = LuaEntityRef::Get(L, 1);

	LuaEntity::PushPtr(L, ref->_entity);
	return 1;
}

LUA_FUNCTION(Lua_EntityRefSetEntity) {
	EntityRef* ref = LuaEntityRef::Get(L, 1);
	Entity* ent = LuaEntity::GetOpt(L, 2);

	ref->_entity = ent;
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	lua_pushcfunction(_state, Lua_EntityRefCtor);
	lua_setglobal(_state, "__Lua_EntityRef_Ctor");
	lua_pushcfunction(_state, Lua_EntityRefGetEntity);
	lua_setglobal(_state, "__Lua_EntityRef_GetEntity");
	lua_pushcfunction(_state, Lua_EntityRefSetEntity);
	lua_setglobal(_state, "__Lua_EntityRef_SetEntity");
	
	super();
}