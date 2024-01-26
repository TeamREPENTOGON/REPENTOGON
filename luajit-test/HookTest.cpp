#include <LuaJIT/src/lua.hpp>

#include "HookSystem.h"
#include "IsaacRepentance.h"

#include "LuaJITState.h"

HOOK_METHOD(Level, GetName, () -> std::string) {
    lua_getglobal(L, "getName");
    lua_pcall(L, 0, 1, 0);

    const char* str = luaL_checkstring(L, -1);
    return std::string(str);
}

//HOOK_METHOD(Weapon, Fire, (const Vector& dir, bool isShooting, bool isInterpolated)-> void) {
//	super(dir, isShooting, isInterpolated);
//    lua_getglobal(L, "weaponFire");
//    lua_pushlightuserdata(L, (void*)&dir);
//    lua_pcall(L, 1, 0, 0);
//}

HOOK_METHOD(Entity_Laser, DoDamage, (Entity* entity, float damage) -> void) {
    super(entity, damage);

    lua_getglobal(L, "laserCollision");
    lua_pushlightuserdata(L, (void*)entity->GetPosition());
    lua_pushnumber(L, damage);
    if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
        g_Game->GetConsole()->PrintError(luaL_checkstring(L, -1));
    };
}
