#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int Lua_NPCUpdateDirtColor(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");
	bool unk = lua_toboolean(L, 2);

	npc->UpdateDirtColor(unk);
	return 1;
}

static void RegisterNPCUpdateDirtColor(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY_NPC);
	lua_pushstring(L, "UpdateDirtColor");
	lua_pushcfunction(L, Lua_NPCUpdateDirtColor);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_NPCGetDirtColor(lua_State* L)
{
	Entity_NPC* npc = lua::GetUserdata<Entity_NPC*>(L, 1, lua::Metatables::ENTITY_NPC, "EntityNPC");

	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *npc->GetDirtColor();

	return 1;
}

static void RegisterNPCGetDirtColor(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY_NPC);
	lua_pushstring(L, "GetDirtColor");
	lua_pushcfunction(L, Lua_NPCGetDirtColor);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterNPCUpdateDirtColor(state);
	RegisterNPCGetDirtColor(state);
}