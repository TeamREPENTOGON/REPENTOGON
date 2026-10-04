ffi.cdef [[
    union EntitySaveStateI1 {
        uint32_t Int;
        float Float;
    };

    struct EntitySaveStateData {
        int Type : 0x0;
        int Variant : 0x4;
        int SubType : 0x8;
        union EntitySaveStateI1 I1 : 0xc;
        uint32_t I2 : 0x10;
        uint32_t I3 : 0x14;
        int I4 : 0x18;
        int I5 : 0x1c;
        bool B1 : 0x22;
        bool B2 : 0x23;
        uint32_t I6 : 0x24;
        struct Vector TargetPosition : 0x28;
        uint32_t InitSeed : 0x34;
        uint32_t DropSeed : 0x38;
        int SpawnerType : 0x3c;
        int SpawnerVariant : 0x40;
        float F1 : 0x50;
        float F2 : 0x54;
        uint32_t I8 : 0x5c;
        int8_t U1 : 0x60;
    } : 0x78;

    struct EntitySaveState {
        private struct EntitiesSaveStateVector* Vector;
        private int Index;
    };
    typedef struct EntitySaveState* EntitySaveStatePtr;

    uint32_t* L_EntitySaveState_GetI7(struct EntitySaveStateData*);
    int16_t* L_EntitySaveState_GetGridSpawnIdx(struct EntitySaveStateData*);
]]

local repentogon = ffidll
local ffi = ffi

local function GetData(self)
    return ffi.getprivate(ffi.getprivate(self, "Vector"), "First")[ffi.getprivate(self, "Index")]
end

local function HasFloatI1(data)
    local type = data.Type
    return type == 4 or type == 33 or type == 292
end

local EntitySaveStateMT
EntitySaveStateMT = {
    __type = "EntitySaveState",
    GetB1 = function(self)
        return GetData(self).B1
    end,
    GetB2 = function(self)
        return GetData(self).B2
    end,
    GetDropSeed = function(self)
        return GetData(self).DropSeed
    end,
    GetF1 = function(self)
        return GetData(self).F1
    end,
    GetF2 = function(self)
        return GetData(self).F2
    end,
    GetGridSpawnIdx = function(self)
        return repentogon.L_EntitySaveState_GetGridSpawnIdx(GetData(self))[0]
    end,
    GetI1 = function(self)
        local data = GetData(self)
        if HasFloatI1(data) then
            return data.I1.Float
        end
        return data.I1.Int
    end,
    GetI2 = function(self)
        return GetData(self).I2
    end,
    GetI3 = function(self)
        return GetData(self).I3
    end,
    GetI4 = function(self)
        return GetData(self).I4
    end,
    GetI5 = function(self)
        return GetData(self).I5
    end,
    GetI6 = function(self)
        return GetData(self).I6
    end,
    GetI7 = function(self)
        return repentogon.L_EntitySaveState_GetI7(GetData(self))[0]
    end,
    GetI8 = function(self)
        return GetData(self).I8
    end,
    GetInitSeed = function(self)
        return GetData(self).InitSeed
    end,
    GetPos = function(self)
        local position = GetData(self).TargetPosition
        return Vector(position.X, position.Y)
    end,
    GetSpawnerType = function(self)
        return GetData(self).SpawnerType
    end,
    GetSpawnerVariant = function(self)
        return GetData(self).SpawnerVariant
    end,
    GetSubType = function(self)
        return GetData(self).SubType
    end,
    GetType = function(self)
        return GetData(self).Type
    end,
    GetU1 = function(self)
        local data = GetData(self)
        if data.Type == 5 then
            return data.U1 ~= 0
        end
        return data.U1
    end,
    GetVariant = function(self)
        return GetData(self).Variant
    end,
    SetB1 = function(self, value)
        ffichecks.checkboolean(1, value)
        GetData(self).B1 = value
    end,
    SetB2 = function(self, value)
        ffichecks.checkboolean(1, value)
        GetData(self).B2 = value
    end,
    SetF1 = function(self, value)
        ffichecks.checknumber(1, value)
        GetData(self).F1 = value
    end,
    SetF2 = function(self, value)
        ffichecks.checknumber(1, value)
        GetData(self).F2 = value
    end,
    SetI1 = function(self, value)
        local data = GetData(self)
        if HasFloatI1(data) then
            ffichecks.checknumber(1, value)
            data.I1.Float = value
        else
            ffichecks.checkinteger(1, value)
            data.I1.Int = value
        end
    end,
    SetI2 = function(self, value)
        ffichecks.checkinteger(1, value)
        GetData(self).I2 = value
    end,
    SetI3 = function(self, value)
        ffichecks.checkinteger(1, value)
        GetData(self).I3 = value
    end,
    SetI4 = function(self, value)
        ffichecks.checkinteger(1, value)
        GetData(self).I4 = value
    end,
    SetI5 = function(self, value)
        ffichecks.checkinteger(1, value)
        GetData(self).I5 = value
    end,
    SetI6 = function(self, value)
        ffichecks.checkinteger(1, value)
        GetData(self).I6 = value
    end,
    SetI7 = function(self, value)
        ffichecks.checkinteger(1, value)
        repentogon.L_EntitySaveState_GetI7(GetData(self))[0] = value
    end,
    SetI8 = function(self, value)
        ffichecks.checkinteger(1, value)
        GetData(self).I8 = value
    end,
    SetPos = function(self, position)
        ffichecks.checkcdata(1, position, "Vector")
        local target = GetData(self).TargetPosition
        target.X = position.X
        target.Y = position.Y
    end,
    SetSubType = function(self, value)
        ffichecks.checkinteger(1, value)
        GetData(self).SubType = value
    end,
    SetType = function(self, value)
        ffichecks.checkinteger(1, value)
        GetData(self).Type = value
    end,
    SetU1 = function(self, value)
        local data = GetData(self)
        if data.Type == 5 then
            ffichecks.checkboolean(1, value)
            data.U1 = value and 1 or 0
        else
            ffichecks.checkinteger(1, value)
            data.U1 = value
        end
    end,
    SetVariant = function(self, value)
        ffichecks.checkinteger(1, value)
        GetData(self).Variant = value
    end,
}

setmetatable(EntitySaveStateMT, { __index = function() end })
EntitySaveStateMT.__index = EntitySaveStateMT

ffi.metatype("struct EntitySaveState", EntitySaveStateMT)

EntitySaveState = setmetatable({}, {
    __class = EntitySaveStateMT,
})
