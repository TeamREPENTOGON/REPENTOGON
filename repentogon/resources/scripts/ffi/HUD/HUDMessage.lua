ffi.cdef [[
    struct HUDMessage {
        private bool Showing : 0x8;
        private struct Sprite Sprite : 0x20;
    } : 0x1f4;
    typedef struct HUDMessage* HUDMessagePtr;

    void L_HUDMessage_Show(struct HUDMessage*, const char*, const char*, bool, bool);
    const char* L_HUDMessage_GetMainText(struct HUDMessage*);
    void L_HUDMessage_SetMainText(struct HUDMessage*, const char*);
    const char* L_HUDMessage_GetSubText(struct HUDMessage*);
    void L_HUDMessage_SetSubText(struct HUDMessage*, const char*);
]]

local repentogon = ffidll
local ffi = ffi

local HUDMessageMT
HUDMessageMT = {
    __type = "HUDMessage",
    GetMainText = function(self)
        return ffi.string(repentogon.L_HUDMessage_GetMainText(self))
    end,
    GetSprite = function(self)
        return ffi.getprivate(self, "Sprite")
    end,
    GetSubText = function(self)
        return ffi.string(repentogon.L_HUDMessage_GetSubText(self))
    end,
    Hide = function(self)
        ffi.setprivate(self, "Showing", false)
    end,
    IsShowing = function(self)
        return ffi.getprivate(self, "Showing")
    end,
    SetMainText = function(self, text)
        text = ffichecks.checkstring(1, text)
        repentogon.L_HUDMessage_SetMainText(self, text)
    end,
    SetSubText = function(self, text)
        text = ffichecks.checkstring(1, text)
        repentogon.L_HUDMessage_SetSubText(self, text)
    end,
    Show = function(self, text, subtext, sticky, curseDisplay)
        text = ffichecks.checkstring(1, text)
        repentogon.L_HUDMessage_Show(self, text, ffichecks.optstring(subtext, ""),
            ffichecks.optboolean(sticky, false), ffichecks.optboolean(curseDisplay, false))
    end,
}

setmetatable(HUDMessageMT, { __index = function() end })
HUDMessageMT.__index = HUDMessageMT

ffi.metatype("struct HUDMessage", HUDMessageMT)

HUDMessage = setmetatable({}, {
    __class = HUDMessageMT,
})
