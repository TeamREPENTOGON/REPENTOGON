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

static int Lua_AddElement(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);

    const char* parentId = luaL_checkstring(L, 2);
    const char* id = luaL_checkstring(L, 3);
    int type = (int)luaL_checkinteger(L, 4);
    const char* text = luaL_optstring(L, 5, "");

    bool success = imGui->AddElement(parentId, id, text, type);
    if (!success) {
        return luaL_error(L, "Parent element '%s' not found.", text, parentId);
    }

    return 1;
}

static int Lua_LinkWindowToElement(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);

    const char* windowId = luaL_checkstring(L, 2);
    const char* elementId = luaL_checkstring(L, 3);

    if (imGui->GetElementById(windowId) == NULL) {
        return luaL_error(L, "No window with id '%s' exists", windowId);
    }

    bool success = imGui->LinkWindowToElement(windowId, elementId);
    if (!success) {
        return luaL_error(L, "Element with id '%s' not found", elementId);
    }

    return 1;
}

static int Lua_CreateMenu(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);

    const char* id = luaL_checkstring(L, 2);
    const char* text = luaL_checkstring(L, 3);

    bool success = imGui->CreateMenuElement(id, text);
    if (!success) {
        return luaL_error(L, "Error while adding new Menu '%s'", id);
    }

    return 1;
}

static int Lua_RemoveMenu(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);
    const char* menuId = luaL_checkstring(L, 2);

    imGui->RemoveMenu(menuId);

    return 0;
}

static int Lua_CreateWindow(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);

    const char* id = luaL_checkstring(L, 2);
    const char* title = luaL_checkstring(L, 3);

    bool success = imGui->CreateWindowElement(id, title);
    if (!success) {
        return luaL_error(L, "Error while adding new Window '%s'", id);
    }

    return 1;
}

static int Lua_RemoveWindow(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);
    const char* windowId = luaL_checkstring(L, 2);

    imGui->RemoveWindow(windowId);

    return 0;
}

static int Lua_AddCallback(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);
    const char* parentId = luaL_checkstring(L, 2);
    int type = (int)luaL_checkinteger(L, 3);
    if (!lua_isfunction(L, 4)) {
        return luaL_error(L, "Argument 4 is not a function!");
    }
    lua_pushvalue(L, 4); // push function on stack
    int stackID = luaL_ref(L, LUA_REGISTRYINDEX); // get unique key
    bool success = imGui->AddCallback(parentId, type, stackID);
    if (!success) {
        return luaL_error(L, "No element '%s' found.", parentId);
    }

    return 1;
}

static int Lua_RemoveCallback(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);
    const char* parentId = luaL_checkstring(L, 2);
    int type = (int)luaL_checkinteger(L, 3);

    bool success = imGui->RemoveCallback(parentId, type);
    if (!success) {
        return luaL_error(L, "No element '%s' found.", parentId);
    }

    return 1;
}

extern bool menuShown;
static int Lua_ImGuiShow(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);
    menuShown = true;

    return 0;
}

static int Lua_ImGuiHide(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);
    menuShown = false;

    return 0;
}

static int Lua_ImGuiSetVisible(lua_State* L)
{
    CustomImGui* imGui = *lua::GetUserdata<CustomImGui**>(L, 1, ImGuiMT);
    const char* elementId = luaL_checkstring(L, 2);
    bool newState = lua_toboolean(L, 3);

    bool success = imGui->SetVisible(elementId, newState);

    if (!success) {
        return luaL_error(L, "Element with id '%s' not found", elementId);
    }

    return 0;
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
        { "AddCallback", Lua_AddCallback },
        { "RemoveCallback", Lua_RemoveCallback },
        { "AddElement", Lua_AddElement },
        { "CreateMenu", Lua_CreateMenu },
        { "RemoveMenu", Lua_RemoveMenu },
        { "CreateWindow", Lua_CreateWindow },
        { "RemoveWindow", Lua_RemoveWindow },
        { "LinkWindowToElement", Lua_LinkWindowToElement },
        { "SetVisible", Lua_ImGuiSetVisible },
        { "Show", Lua_ImGuiShow },
        { "Hide", Lua_ImGuiHide },
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
