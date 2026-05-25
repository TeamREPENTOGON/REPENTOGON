#include <type_traits>
#include "IsaacRepentance.h"
#include "LuaCore.h"

class LuaBlendMode
{
private:
    enum class StorageType
    {
        Pointer,
        Value
    };

    struct Userdata
    {
        StorageType type;

        union
        {
            BlendMode* ptr;
            BlendMode value;
        };
    };

public:
    static BlendMode* NewPointer(lua_State* L, BlendMode* blendMode)
    {
        assert(blendMode != nullptr);
        Userdata* userdata = (Userdata*)lua_newuserdata(L, sizeof(Userdata));
        luaL_setmetatable(L, lua::metatables::BlendModeMT);
    
        userdata->type = StorageType::Pointer;
        userdata->ptr = blendMode;

        return blendMode;
    }

    template <typename... Args>
    static BlendMode* NewValue(lua_State* L, Args&&... args)
    {
        Userdata* userdata = (Userdata*)lua_newuserdata(L, sizeof(Userdata));
        luaL_setmetatable(L, lua::metatables::BlendModeMT);

        userdata->type = StorageType::Value;
        new (&userdata->value) BlendMode(std::forward<Args>(args)...);
    
        return &userdata->value;
    }

    static BlendMode* Get(lua_State* L, int idx)
    {
        Userdata* userdata = lua::GetRawUserdata<Userdata*>(L, idx, lua::metatables::BlendModeMT);
        return userdata->type == StorageType::Pointer ? userdata->ptr : &userdata->value;
    }

    static void GC(lua_State* L, int idx)
    {
        if constexpr (!std::is_trivially_destructible<BlendMode>::value)
        {
            Userdata* userdata = lua::GetRawUserdata<Userdata*>(L, idx, lua::metatables::BlendModeMT);
            if (userdata->type == StorageType::Value)
            {
                userdata->value.~BlendMode();
            }
        }
    }
};