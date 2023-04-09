#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int Lua_NPCUpdateDirtColor(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	bool unk = lua_toboolean(L, 2);

	npc->UpdateDirtColor(unk);
	return 0;
}

int Lua_NPCGetDirtColor(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");

	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *npc->GetDirtColor();

	return 1;
}


HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ENTITY_NPC;
	lua::RegisterFunction(state, mt, "UpdateDirtColor", Lua_NPCUpdateDirtColor);
	lua::RegisterFunction(state, mt, "GetDirtColor", Lua_NPCGetDirtColor);
}