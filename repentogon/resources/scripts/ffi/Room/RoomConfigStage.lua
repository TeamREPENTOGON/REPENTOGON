ffi.cdef [[
    struct RoomConfigStage {
        private unsigned int ID : 0x0;
        private struct RoomSet Rooms[2] : 0x64;
        private int Music : 0xbc;
        private int Backdrop : 0xc0;
    } : 0xc4;
    typedef struct RoomConfigStage* RoomConfigStagePtr;

    bool L_RoomConfigStage_ValidateMusicID(int, int*);
    const char* L_RoomConfigStage_GetDisplayName(struct RoomConfigStage*);
    void L_RoomConfigStage_SetDisplayName(struct RoomConfigStage*, const char*);
    const char* L_RoomConfigStage_GetPlayerSpot(struct RoomConfigStage*);
    void L_RoomConfigStage_SetPlayerSpot(struct RoomConfigStage*, const char*);
    const char* L_RoomConfigStage_GetBossSpot(struct RoomConfigStage*);
    void L_RoomConfigStage_SetBossSpot(struct RoomConfigStage*, const char*);
    const char* L_RoomConfigStage_GetSuffix(struct RoomConfigStage*);
    void L_RoomConfigStage_SetSuffix(struct RoomConfigStage*, const char*);
    const char* L_RoomConfigStage_GetXMLName(struct RoomConfigStage*);
    void L_RoomConfigStage_SetXMLName(struct RoomConfigStage*, const char*);
    void L_RoomConfigStage_LoadRoomSet(struct RoomConfigStage*, int);
]]

local repentogon = ffidll
local ffi = ffi

local RoomConfigSetT = ffi.typeof("struct RoomConfigSet")

local function ToInteger(value)
    local number = tonumber(value)
    if not number then
        return 0
    end
    return number < 0 and math.ceil(number) or math.floor(number)
end

local function CheckMode(mode)
    mode = ffichecks.optnumber(mode, 0)
    if mode < 0 or mode > 1 then
        ffichecks.argerror(1, string.format("invalid RoomSet mode %d", mode), 3)
    end
    return mode
end

local RoomConfigStageMT
RoomConfigStageMT = {
    __type = "RoomConfigStage",
    GetBackdrop = function(self)
        return ffi.getprivate(self, "Backdrop")
    end,
    GetBossSpot = function(self)
        return ffi.string(repentogon.L_RoomConfigStage_GetBossSpot(self))
    end,
    GetDisplayName = function(self)
        return ffi.string(repentogon.L_RoomConfigStage_GetDisplayName(self))
    end,
    GetID = function(self)
        return ffi.getprivate(self, "ID")
    end,
    GetMusic = function(self)
        return ffi.getprivate(self, "Music")
    end,
    GetPlayerSpot = function(self)
        return ffi.string(repentogon.L_RoomConfigStage_GetPlayerSpot(self))
    end,
    GetRoomSet = function(self, mode)
        mode = CheckMode(mode)
        repentogon.L_RoomConfigStage_LoadRoomSet(self, mode)
        return RoomConfigSetT(ffi.getprivate(self, "Rooms") + mode, repentogon.L_RoomConfig_GetVanillaSetID(ffi.getprivate(self, "ID"), mode))
    end,
    GetSuffix = function(self)
        return ffi.string(repentogon.L_RoomConfigStage_GetSuffix(self))
    end,
    GetXMLName = function(self)
        return ffi.string(repentogon.L_RoomConfigStage_GetXMLName(self))
    end,
    IsLoaded = function(self, mode)
        mode = CheckMode(mode)
        return ffi.getprivate(ffi.getprivate(self, "Rooms")[mode], "Loaded")
    end,
    SetBackdrop = function(self, backdrop)
        ffi.setprivate(self, "Backdrop", ToInteger(backdrop))
    end,
    SetBossSpot = function(self, value)
        ffichecks.checkstring(1, value)
        repentogon.L_RoomConfigStage_SetBossSpot(self, value)
    end,
    SetDisplayName = function(self, value)
        ffichecks.checkstring(1, value)
        repentogon.L_RoomConfigStage_SetDisplayName(self, value)
    end,
    SetMusic = function(self, music)
        music = ToInteger(music)
        local max = ffi.new("int[1]")
        if not repentogon.L_RoomConfigStage_ValidateMusicID(music, max) then
            ffichecks.argerror(1, string.format("invalid music ID %d. Min = 0, Max = %d", music, max[0] - 1))
        end
        ffi.setprivate(self, "Music", music)
    end,
    SetPlayerSpot = function(self, value)
        ffichecks.checkstring(1, value)
        repentogon.L_RoomConfigStage_SetPlayerSpot(self, value)
    end,
    SetSuffix = function(self, value)
        ffichecks.checkstring(1, value)
        repentogon.L_RoomConfigStage_SetSuffix(self, value)
    end,
    SetXMLName = function(self, value)
        ffichecks.checkstring(1, value)
        repentogon.L_RoomConfigStage_SetXMLName(self, value)
    end,
}

setmetatable(RoomConfigStageMT, { __index = function() end })
RoomConfigStageMT.__index = RoomConfigStageMT

ffi.metatype("struct RoomConfigStage", RoomConfigStageMT)

RoomConfigStage = setmetatable({}, {
    __class = RoomConfigStageMT,
})
