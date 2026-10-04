ffi.cdef[[
struct ItemConfigCostume
{
    unsigned int ID;
    private struct StdString Anm2PathString;
    unsigned int Priority;
    bool HasOverlay;
    bool IsFlying;
    bool HasSkinAlt;
    padding char;
    int SkinColor;
    bool OverwriteColor;
};

typedef struct ItemConfigCostume* ItemConfigCostumePtr;

void L_ItemConfigCostume_SetAnm2Path(struct ItemConfigCostume*, const char*);
]]

local ffi = ffi
local repentogon = ffidll


local ItemConfigCostumeMT
ItemConfigCostumeMT = {
    __type = "ItemConfigCostume",

    __index = function(self, key)
        if key == "Anm2Path" then
            return ffichecks.stdstring(ffi.getprivate(self, "Anm2PathString"))
        end
        return ItemConfigCostumeMT[key]
    end,

    __newindex = function(self, key, value)
        if key == "Anm2Path" then
            value = ffichecks.checkstring(3, value)
            repentogon.L_ItemConfigCostume_SetAnm2Path(self, value)
            return
        end
        error("cannot set '" .. tostring(key) .. "'")
    end,
}

local ItemConfigCostumeT = ffi.metatype("struct ItemConfigCostume", ItemConfigCostumeMT)

ItemConfigCostume = setmetatable({}, {__class = ItemConfigCostumeMT})