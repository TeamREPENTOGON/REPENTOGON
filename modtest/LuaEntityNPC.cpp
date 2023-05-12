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

int Lua_NPCGetControllerId(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");

	lua_pushnumber(L,*npc->GetControllerId());

	return 1;
}

int Lua_NPCSetControllerId(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	int unk = lua_tonumber(L, 2);

	npc->SetControllerId(unk);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ENTITY_NPC;
	lua::RegisterFunction(state, mt, "UpdateDirtColor", Lua_NPCUpdateDirtColor);
	lua::RegisterFunction(state, mt, "GetDirtColor", Lua_NPCGetDirtColor);
	lua::RegisterFunction(state, mt, "GetControllerId", Lua_NPCGetControllerId);
	lua::RegisterFunction(state, mt, "SetControllerId", Lua_NPCSetControllerId);
}