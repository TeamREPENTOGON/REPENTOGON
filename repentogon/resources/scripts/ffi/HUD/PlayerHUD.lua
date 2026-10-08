ffi.cdef [[
    struct PlayerHUD {
        private struct EntityPlayer* Player : 0x0;
        private struct HUD* HUD : 0x4;
        private int16_t Index : 0x8;
        private int8_t RedHeartFlashCountdown : 0xc;
        private int8_t SoulHeartFlashCountdown : 0xd;
        private int8_t GoldHeartFlashCountdown : 0xe;
        private struct PlayerHUDHeart Hearts[24] : 0x10;
    } : 0x6e0;
    typedef struct PlayerHUD* PlayerHUDPtr;

    int L_PlayerHUD_GetLayout(struct PlayerHUD*);
    void L_PlayerHUD_RenderActiveItem(struct PlayerHUD*, unsigned int, struct Vector*, float, float);
]]

local repentogon = ffidll
local ffi = ffi

local PlayerHUDHeartT = ffi.typeof("struct PlayerHUDHeart")

local function SetFlashCountdown(self, field, duration)
    duration = ffichecks.optnumber(duration, 4)
    ffi.setprivate(self, field, math.max(0, math.min(127, duration)))
end

local PlayerHUDMT
PlayerHUDMT = {
    __type = "PlayerHUD",
    GetFlashGoldHearts = function(self)
        local result = ffi.getprivate(self, "GoldHeartFlashCountdown") return result
    end,
    GetFlashRedHearts = function(self)
        local result = ffi.getprivate(self, "RedHeartFlashCountdown") return result
    end,
    GetFlashSoulHearts = function(self)
        local result = ffi.getprivate(self, "SoulHeartFlashCountdown") return result
    end,
    GetHUD = function(self)
        local result = ffi.getprivate(self, "HUD") return result
    end,
    GetHeartByIndex = function(self, index)
        index = ffichecks.checkinteger(1, index)
        if index < 0 or index > 23 then
            ffichecks.argerror(1, string.format("invalid index: %d", index))
        end
        local result = PlayerHUDHeartT(ffi.getprivate(self, "Hearts")[index]) return result
    end,
    GetHearts = function(self)
        local hearts = ffi.getprivate(self, "Hearts")
        local result = {}
        for i = 0, 23 do
            result[i + 1] = PlayerHUDHeartT(hearts[i])
        end
        return result
    end,
    GetIndex = function(self)
        local result = ffi.getprivate(self, "Index") return result
    end,
    GetLayout = function(self)
        local result = repentogon.L_PlayerHUD_GetLayout(self) return result
    end,
    GetPlayer = function(self)
        local result = ffi.getprivate(self, "Player") return result
    end,
    RenderActiveItem = function(self, activeSlot, position, alpha, size)
        activeSlot = ffichecks.checkinteger(1, activeSlot)
        ffichecks.checkcdata(2, position, "Vector")
        repentogon.L_PlayerHUD_RenderActiveItem(self, activeSlot, position, ffichecks.optnumber(alpha, 1), ffichecks.optnumber(size, 1))
    end,
    SetFlashGoldHearts = function(self, duration)
        SetFlashCountdown(self, "GoldHeartFlashCountdown", duration)
    end,
    SetFlashRedHearts = function(self, duration)
        SetFlashCountdown(self, "RedHeartFlashCountdown", duration)
    end,
    SetFlashSoulHearts = function(self, duration)
        SetFlashCountdown(self, "SoulHeartFlashCountdown", duration)
    end,
}

setmetatable(PlayerHUDMT, { __index = function() end })
PlayerHUDMT.__index = PlayerHUDMT

ffi.metatype("struct PlayerHUD", PlayerHUDMT)

PlayerHUD = setmetatable({}, {
    __class = PlayerHUDMT,
})

