ffi.cdef [[
    struct ItemConfigPillEffect {
        int ID : 0x0;
        int AchievementID : 0x4;
        private struct StdString NameString : 0x8;
        bool GreedModeAllowed : 0x38;
        int AnnouncerVoice : 0x3c;
        int AnnouncerVoiceSuper : 0x40;
        int AnnouncerDelay : 0x44;
        int MimicCharge : 0x48;
        uint8_t EffectClass : 0x4c;
        uint8_t EffectSubClass : 0x4d;
    } : 0x50;
    typedef struct ItemConfigPillEffect* ItemConfigPillEffectPtr;

    bool L_ItemConfigPillEffect_IsAvailable(struct ItemConfigPillEffect*);
]]

local ffi = ffi
local repentogon = ffidll

local ItemConfigPillEffectMT
ItemConfigPillEffectMT = {
    __type = "ItemConfigPillEffect",

    __index = function(self, key)
        if key == "Name" then
            return ffichecks.stdstring(ffi.getprivate(self, "NameString"))
        end
        return ItemConfigPillEffectMT[key]
    end,

    __newindex = function(self, key, value)
        if key == "Name" then
            value = ffichecks.checkstring(3, value)
            repentogon.L_StdString_Assign(ffi.getprivate(self, "NameString"), value)
            return
        end
        error(string.format("cannot set '%s'", tostring(key)))
    end,

    IsAvailable = function(self)
        return repentogon.L_ItemConfigPillEffect_IsAvailable(self)
    end,
}

ffi.metatype("struct ItemConfigPillEffect", ItemConfigPillEffectMT)

ItemConfigPillEffect = setmetatable({}, {__class = ItemConfigPillEffectMT})
