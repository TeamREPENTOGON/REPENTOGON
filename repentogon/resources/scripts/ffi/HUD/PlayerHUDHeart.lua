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
    local result = ffi.string(ptr) return result
end

local PlayerHUDHeartMT
PlayerHUDHeartMT = {
    __type = "PlayerHUDHeart",
    GetFlashType = function(self)
        local result = ffi.getprivate(self, "FlashType") return result
    end,
    GetHeartAnim = function(self)
        return OptString(ffi.getprivate(self, "SpriteAnim"))
    end,
    GetHeartOverlayAnim = function(self)
        return OptString(ffi.getprivate(self, "SpriteOverlayAnim"))
    end,
    IsEternalHeartOverlayVisible = function(self)
        local result = ffi.getprivate(self, "EternalHeartVisible") return result
    end,
    IsFadingHeart = function(self)
        local result = ffi.getprivate(self, "FadeHeart") return result
    end,
    IsGoldenHeartOverlayVisible = function(self)
        local result = ffi.getprivate(self, "GoldenHeartVisible") return result
    end,
    IsVisible = function(self)
        local result = ffi.getprivate(self, "Visible") return result
    end,
}

setmetatable(PlayerHUDHeartMT, { __index = function() end })
PlayerHUDHeartMT.__index = PlayerHUDHeartMT

ffi.metatype("struct PlayerHUDHeart", PlayerHUDHeartMT)

PlayerHUDHeart = setmetatable({}, {
    __class = PlayerHUDHeartMT,
})
