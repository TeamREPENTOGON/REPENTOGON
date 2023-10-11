#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* ProceduralItemMT = "ProceduralItem";

int Lua_PIGetItem(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    lua::luabridge::UserdataPtr::push(L, pi->item, lua::Metatables::CONST_ITEM);
    return 1;
}

int Lua_PIGetTargetItem(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    lua::luabridge::UserdataPtr::push(L, pi->targetItem, lua::Metatables::CONST_ITEM);
    return 1;
}

int Lua_PIGetID(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    lua_pushinteger(L, pi->_id);
    return 1;
}

int Lua_PIGetDamage(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    lua_pushnumber(L, pi->damage);
    return 1;
}
int Lua_PIGetFireDelay(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    lua_pushnumber(L, pi->firedelay);
    return 1;
}
int Lua_PIGetSpeed(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    lua_pushnumber(L, pi->speed);
    return 1;
}
int Lua_PIGetRange(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    lua_pushnumber(L, pi->range);
    return 1;
}
int Lua_PIGetShotSpeed(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    lua_pushnumber(L, pi->shotspeed);
    return 1;
}
int Lua_PIGetLuck(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    lua_pushnumber(L, pi->luck);
    return 1;
}

int Lua_PIGetEffectCount(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    std::vector<ProceduralEffect*> *effect = pi->GetEffects();
    lua_pushinteger(L, effect->size());
    return 1;
}

int Lua_PIGetEffect(lua_State* L){
    ProceduralItem* pi = *lua::GetUserdata<ProceduralItem**>(L, 1, ProceduralItemMT);
    std::vector<ProceduralEffect*> *effect = pi->GetEffects();
    int index = luaL_checkinteger(L, 2);
    if(index >= 0 && index < effect->size()){
        ProceduralEffect** pe = (ProceduralEffect**)lua_newuserdata(L, sizeof(void *));
        *pe = (*effect)[index];
        luaL_setmetatable(L, "ProceduralEffect");
    }else{
        lua_pushnil(L);
    }
    return 1;
}

static void RegisterProceduralItem(lua_State* L) {
	luaL_newmetatable(L, ProceduralItemMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
        { "GetItem", Lua_PIGetItem},
        { "GetTargetItem", Lua_PIGetTargetItem},
        { "GetEffectCount", Lua_PIGetEffectCount},
        { "GetEffect", Lua_PIGetEffect},
        { "GetID", Lua_PIGetID},
        { "GetDamage", Lua_PIGetDamage},
        { "GetFireDelay", Lua_PIGetFireDelay},
        { "GetSpeed", Lua_PIGetSpeed},
        { "GetRange", Lua_PIGetRange},
        { "GetShotSpeed", Lua_PIGetShotSpeed},
        { "GetLuck", Lua_PIGetLuck},
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterProceduralItem(state);
}