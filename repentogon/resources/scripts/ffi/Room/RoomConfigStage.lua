--[[
The custom stage support introduces some quirks for this class.
The internal RoomConfig_Stage* array is of a fixed size, so we maintain our own separate stage data under StageManager, which includes RoomConfig_Stage* objects (minus RoomSets).
When a custom stage is loaded, it overwrites vanilla RoomConfig_Stage attributes (name, spots, backdrop, music, suffix) so that many of those behaviours don't require patches.
The RoomConfig_Stage objects we provide to the lua API are the ones owned by the StageManager, not the ones in the vanilla array.
So if values are modified, we may need to "update" the values stored in the vanilla array too. That's why C functions are used even for integer fields.
Additionally, the StageManager does not hold the vanilla RoomSets, so we still need to refer back to the vanilla array for those.
]]

ffi.cdef [[
    struct RoomConfigStage {
        private unsigned int ID : 0x0;
        private struct StdString DisplayName : 0x4;
        private struct StdString PlayerSpot : 0x1c;
        private struct StdString BossSpot : 0x34;
        private struct StdString Suffix : 0x4c;
        private struct RoomSet Rooms[2] : 0x64;  // WARNING: Not present in the structs passed to lua, see above
        private int Music : 0xbc;
        private int Backdrop : 0xc0;
    } : 0xc4;
    typedef struct RoomConfigStage* RoomConfigStagePtr;

    bool L_RoomConfigStage_ValidateMusicID(int, int*);
    void L_RoomConfigStage_SetDisplayName(struct RoomConfigStage*, const char*);
    void L_RoomConfigStage_SetPlayerSpot(struct RoomConfigStage*, const char*);
    void L_RoomConfigStage_SetBossSpot(struct RoomConfigStage*, const char*);
    void L_RoomConfigStage_SetSuffix(struct RoomConfigStage*, const char*);
    void L_RoomConfigStage_SetBackdrop(struct RoomConfigStage*, int);
    void L_RoomConfigStage_SetMusic(struct RoomConfigStage*, int);
    const char* L_RoomConfigStage_GetXMLName(struct RoomConfigStage*);
    void L_RoomConfigStage_SetXMLName(struct RoomConfigStage*, const char*);
    struct RoomConfigSet* L_RoomConfigStage_LoadVanillaRoomSet(struct RoomConfigStage*, int);
    struct RoomConfigSet* L_RoomConfigStage_GetVanillaRoomSet(struct RoomConfigStage*, int);
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

local function CheckVanillaStage(stage)
    if stage:IsCustomStage() then
        ffichecks.argerror(0, "not supported for custom stages", 3)
    end
end

local RoomConfigStageMT
RoomConfigStageMT = {
    __type = "RoomConfigStage",
    IsCustomStage = function(self)
        return self:GetID() > StbType.ASCENT
    end,
    GetBackdrop = function(self)
        local result = ffi.getprivate(self, "Backdrop") return result
    end,
    GetBossSpot = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "BossSpot"))
    end,
    GetDisplayName = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "DisplayName"))
    end,
    GetID = function(self)
        local result = ffi.getprivate(self, "ID") return result
    end,
    GetMusic = function(self)
        local result = ffi.getprivate(self, "Music") return result
    end,
    GetPlayerSpot = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "PlayerSpot"))
    end,
    GetRoomSet = function(self, mode)
        mode = CheckMode(mode)
        local vanillaSet = nil
        if not self:IsCustomStage() then
            vanillaSet = repentogon.L_RoomConfigStage_LoadVanillaRoomSet(self, mode)
        end
        local result = RoomConfigSetT(vanillaSet, repentogon.L_RoomConfig_GetRoomSetID(ffi.getprivate(self, "ID"), mode)) return result
    end,
    GetSuffix = function(self)
        return ffichecks.stdstring(ffi.getprivate(self, "Suffix"))
    end,
    GetXMLName = function(self)
        CheckVanillaStage(self)
        local result = ffi.string(repentogon.L_RoomConfigStage_GetXMLName(self)) return result
    end,
    IsLoaded = function(self, mode)
        mode = CheckMode(mode)
        if self:IsCustomStage() then
            return true
        end
        local vanillaSet = repentogon.L_RoomConfigStage_GetVanillaRoomSet(self, mode)
        local result = ffi.getprivate(vanillaSet, "Loaded") return result
    end,
    SetBackdrop = function(self, backdrop)
        repentogon.L_RoomConfigStage_SetBackdrop(self, ToInteger(backdrop))
    end,
    SetBossSpot = function(self, value)
        value = ffichecks.checkstring(1, value)
        repentogon.L_RoomConfigStage_SetBossSpot(self, value)
    end,
    SetDisplayName = function(self, value)
        value = ffichecks.checkstring(1, value)
        repentogon.L_RoomConfigStage_SetDisplayName(self, value)
    end,
    SetMusic = function(self, music)
        music = ToInteger(music)
        local max = ffi.new("int[1]")
        if not repentogon.L_RoomConfigStage_ValidateMusicID(music, max) then
            ffichecks.argerror(1, string.format("invalid music ID %d. Min = 0, Max = %d", music, max[0] - 1))
        end
        repentogon.L_RoomConfigStage_SetMusic(self, music)
    end,
    SetPlayerSpot = function(self, value)
        value = ffichecks.checkstring(1, value)
        repentogon.L_RoomConfigStage_SetPlayerSpot(self, value)
    end,
    SetSuffix = function(self, value)
        value = ffichecks.checkstring(1, value)
        repentogon.L_RoomConfigStage_SetSuffix(self, value)
    end,
    SetXMLName = function(self, value)
        CheckVanillaStage(self)
        value = ffichecks.checkstring(1, value)
        repentogon.L_RoomConfigStage_SetXMLName(self, value)
    end,
}

setmetatable(RoomConfigStageMT, { __index = function() end })
RoomConfigStageMT.__index = RoomConfigStageMT

ffi.metatype("struct RoomConfigStage", RoomConfigStageMT)

RoomConfigStage = setmetatable({}, {
    __class = RoomConfigStageMT,
})
