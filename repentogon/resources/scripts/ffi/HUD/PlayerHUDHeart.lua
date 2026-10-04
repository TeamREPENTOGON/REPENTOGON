ffi.cdef [[
    struct PlayerHUDHeart {
        private bool Visible : 0x0;
        private bool EternalHeartVisible : 0x1;
        private bool GoldenHeartVisible : 0x2;
        private bool FadeHeart : 0x3;
        private int8_t FlashType : 0x4;
        private const char* SpriteAnim : 0x8;
        private const char* SpriteOverlayAnim : 0xc;
    } : 0x10;
    typedef struct PlayerHUDHeart* PlayerHUDHeartPtr;
]]

local ffi = ffi

local function OptString(ptr)
    if ptr == nil then
        return nil
    end
    return ffi.string(ptr)
end

local PlayerHUDHeartMT
PlayerHUDHeartMT = {
    __type = "PlayerHUDHeart",
    GetFlashType = function(self)
        return ffi.getprivate(self, "FlashType")
    end,
    GetHeartAnim = function(self)
        return OptString(ffi.getprivate(self, "SpriteAnim"))
    end,
    GetHeartOverlayAnim = function(self)
        return OptString(ffi.getprivate(self, "SpriteOverlayAnim"))
    end,
    IsEternalHeartOverlayVisible = function(self)
        return ffi.getprivate(self, "EternalHeartVisible")
    end,
    IsFadingHeart = function(self)
        return ffi.getprivate(self, "FadeHeart")
    end,
    IsGoldenHeartOverlayVisible = function(self)
        return ffi.getprivate(self, "GoldenHeartVisible")
    end,
    IsVisible = function(self)
        return ffi.getprivate(self, "Visible")
    end,
}

setmetatable(PlayerHUDHeartMT, { __index = function() end })
PlayerHUDHeartMT.__index = PlayerHUDHeartMT

ffi.metatype("struct PlayerHUDHeart", PlayerHUDHeartMT)

PlayerHUDHeart = setmetatable({}, {
    __class = PlayerHUDHeartMT,
})
