#include <lua.hpp>

#include "CustomImGui.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

extern CustomImGui customImGui;

static constexpr const char* ImGuiMT = "ImGui";

static int Lua_CustomImGui(lua_State* L)
{
    Manager* manager = g_Manager;
    CustomImGui** ud = (CustomImGui**)lua_newuserdata(L, sizeof(CustomImGui*));
    *ud = &customImGui;
    luaL_setmetatable(L, ImGuiMT);
    return 1;
}

static int Lua_AddElementToWindow(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);

    const char* windowName = luaL_checkstring(L, 2);
    const char* text = luaL_checkstring(L, 3);
    int type = (int)luaL_checkinteger(L, 4);
    const char* parentName = luaL_optstring(L, 5, "");

    bool success = imGui->AddElementToWindow(windowName, text, type, parentName);
    if (!success) {
        return luaL_error(L, "Error while adding element '%s' to window with name '%s'", text, windowName);
    }

    return 1;
}

static int Lua_AddElementToMenu(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);

    const char* text = luaL_checkstring(L, 2);
    int type = (int)luaL_checkinteger(L, 3);
    const char* parentName = luaL_optstring(L, 4, "");

    bool success = imGui->AddElementToMenu(text, type, parentName);
    if (!success) {
        return luaL_error(L, "Error while adding element '%s' to menu '%s'", text, parentName);
    }

    return 1;
}

static int Lua_AddWindow(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);

    const char* parentName = luaL_checkstring(L, 2);
    const char* text = luaL_checkstring(L, 3);

    bool success = imGui->AddWindow(parentName, text);
    if (!success) {
        return luaL_error(L, "Error while adding new Window '%s' to menu entry '%s'", text, parentName);
    }

    return 1;
}

static int Lua_AddCallbackToMenuElement(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);
    const char* parentName = luaL_checkstring(L, 2);
    int type = (int)luaL_checkinteger(L, 3);
    if (!lua_isfunction(L, 4)) {
        return luaL_error(L, "Argument 4 is not a function!");
    }
    lua_pushvalue(L, 4); // push function on stack
    int stackID = luaL_ref(L, LUA_REGISTRYINDEX); // get unique key
    bool success = imGui->AddCallbackToMenuElement(parentName, type, stackID);
    if (!success) {
        return luaL_error(L, "Error adding callback to element '%s'", parentName);
    }

    return 1;
}

static int Lua_AddCallbackToWindowElement(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);
    const char* windowName = luaL_checkstring(L, 2);
    const char* parentName = luaL_checkstring(L, 3);
    int type = (int)luaL_checkinteger(L, 4);
    if (!lua_isfunction(L, 5)) {
        return luaL_error(L, "Argument 5 is not a function!");
    }
    lua_pushvalue(L, 5); // push function on stack
    int stackID = luaL_ref(L, LUA_REGISTRYINDEX); // get unique key
    bool success = imGui->AddCallbackToWindowElement(windowName, parentName, type, stackID);
    if (!success) {
        return luaL_error(L, "Error adding callback to element '%s'", parentName);
    }

    return 1;
}

static int Lua_RemoveWindow(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);
    const char* windowName = luaL_checkstring(L, 2);

    imGui->RemoveWindow(windowName);

    return 1;
}

static void RegisterCustomImGui(lua_State* L)
{
    lua_getglobal(L, "Isaac");
    lua_pushstring(L, "GetImGui");
    lua_pushcfunction(L, Lua_CustomImGui);
    lua_rawset(L, -3);
    lua_pop(L, 1);

    luaL_newmetatable(L, ImGuiMT);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_Reg functions[] = {
        { "AddCallbackToMenuElement", Lua_AddCallbackToMenuElement },
        { "AddCallbackToWindowElement", Lua_AddCallbackToWindowElement },
        { "AddElementToWindow", Lua_AddElementToWindow },
        { "AddElementToMenu", Lua_AddElementToMenu },
        { "AddWindow", Lua_AddWindow },
        { "RemoveWindow", Lua_RemoveWindow },
        { NULL, NULL }
    };

    luaL_setfuncs(L, functions, 0);
    lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, ()->void)
{
    super();
    lua_State* state = g_LuaEngine->_state;
    lua::LuaStackProtector protector(state);
    RegisterCustomImGui(state);
}
