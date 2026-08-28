ffi.cdef[[
struct ItemConfigCostume
{
    unsigned int ID;
    char _pad_anm2_path[0x18];
    unsigned int Priority;
    bool HasOverlay;
    bool IsFlying;
    bool HasSkinAlt;
    char _pad;
    int SkinColor;
    bool OverwriteColor;
};

typedef struct ItemConfigCostume* ItemConfigCostumePtr;

const char* L_ItemConfigCostume_GetAnm2Path(struct ItemConfigCostume*);
void L_ItemConfigCostume_SetAnm2Path(struct ItemConfigCostume*, const char*);
]]

local lffi = ffi
local repentogon = ffidll


local ItemConfigCostumeMT
ItemConfigCostumeMT = {
    __type = "ItemConfigCostume",

    __index = function(self, key)
        if key == "Anm2Path" then
            return lffi.string(repentogon.L_ItemConfigCostume_GetAnm2Path(self))
        end
        return ItemConfigCostumeMT[key]
    end,

    __newindex = function(self, key, value)
        if key == "Anm2Path" then
            ffichecks.checkstring(3, value)
            repentogon.L_ItemConfigCostume_SetAnm2Path(self, value)
            return
        end
        error("cannot set '" .. tostring(key) .. "'")
    end,
}

local ItemConfigCostumeT = ffi.metatype("struct ItemConfigCostume", ItemConfigCostumeMT)

ItemConfigCostume = setmetatable({}, {__class = ItemConfigCostumeMT})