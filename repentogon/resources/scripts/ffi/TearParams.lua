ffi.cdef[[
struct TearParams {
    int TearVariant;
    int BombVariant;
    private struct Color TearColor;
    padding char[0x04]; // Unused field internally.
    private struct BitSet128 TearFlags;
    float TearDamage;
    float TearScale;
    float TearHeight;
    float MassMultiplier;
    float KnockbackMultiplier;
    float SpeedMultiplier;
    private int TearDisplacement;
};

typedef struct TearParams* TearParamsPtr;
]]

local ffi = ffi

local function CopyStruct(ctype, reference)
    local copy = ffi.new(ctype)
    ffi.copy(copy, reference, ffi.sizeof(ctype))
    return copy
end

local TearParamsMT
TearParamsMT = {
    __type = "TearParams",
}

TearParamsMT.__index = function(self, key)
    if key == "TearDisplacement" then
        local result = ffi.getprivate(self, "TearDisplacement") return result
    end
    if key == "TearColor" then
        return CopyStruct("struct Color", ffi.getprivate(self, "TearColor"))
    end
    if key == "TearFlags" then
        return CopyStruct("struct BitSet128", ffi.getprivate(self, "TearFlags"))
    end

    return TearParamsMT[key]
end

TearParamsMT.__newindex = function(self, key, value)
    if key == "TearDisplacement" then
        -- 0 is technically "valid" in that the game can sometimes allow it via GetTearHitParams.
        -- I dunno what the implication of having it be zero is though, it doesn't seem entirely intentional.
        -- Not treating 0 as an error, but also not going to mention it in the error message.
        if value ~= -1 and value ~= 0 and value ~= 1 then
            error("TearDisplacement may only be set to -1 or 1")
        end

        ffi.setprivate(self, "TearDisplacement", value)
        return
    end
    if key == "TearColor" then
        ffichecks.checkcdata(1, value, "Color")
        ffi.setprivate(self, "TearColor", value)
        return
    end
    if key == "TearFlags" then
        ffichecks.checkcdata(1, value, "BitSet128")
        ffi.setprivate(self, "TearFlags", value)
        return
    end
end

local TearParamsT = ffi.metatype("struct TearParams", TearParamsMT)

TearParams = setmetatable({}, { __class = TearParamsMT })
