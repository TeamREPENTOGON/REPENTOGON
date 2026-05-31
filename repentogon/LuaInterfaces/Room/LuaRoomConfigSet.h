#include "LuaCore.h"
#include "../../VirtualRoomConfig/VirtualRoomSetManager.h"

namespace LuaRoomConfigSet
{
    struct Userdata
    {
        static constexpr char* MT = "RoomConfigSet";
        RoomSet* vanillaSet = nullptr; // also encodes "IsVanilla"
        VirtualRoomSet virtualSet;

        Userdata(RoomConfig_Stage& stage, int mode);
        bool IsVanilla();
    };

    Userdata* NewUserdata(lua_State* L, RoomConfig_Stage& stage, int mode);
    Userdata* GetUserdata(lua_State* L, int idx);
}

inline LuaRoomConfigSet::Userdata* LuaRoomConfigSet::NewUserdata(lua_State* L, RoomConfig_Stage& stage, int mode)
{
    void* newUserdata = lua_newuserdata(L, sizeof(Userdata));
    Userdata* userdata = new (newUserdata) Userdata(stage, mode);
    luaL_setmetatable(L, Userdata::MT);
    return userdata;
}

inline LuaRoomConfigSet::Userdata* LuaRoomConfigSet::GetUserdata(lua_State* L, int idx)
{
    return lua::GetRawUserdata<Userdata*>(L, idx, Userdata::MT);
}

inline bool LuaRoomConfigSet::Userdata::IsVanilla()
{
    return vanillaSet != nullptr;
}