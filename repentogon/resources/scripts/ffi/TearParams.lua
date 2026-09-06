ffi.cdef[[
struct TearParams {
    int TearVariant;
    int BombVariant;
    struct Color TearColor;
    padding char[0x04]; // Unused field internally.
    struct BitSet128 TearFlags;
    float TearDamage;
    float TearScale;
    float TearHeight;
    float MassMultiplier;
    float KnockbackMultiplier;
    float SpeedMultiplier;
    private int _TearDisplacement;
};

typedef struct TearParams* TearParamsPtr;
]]

local TearParamsMT
TearParamsMT = {
    __type = "TearParams",
}

TearParamsMT.__index = function(self, key)
    if key == "TearDisplacement" then
        return ffi.getprivate(self, "_TearDisplacement")
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

        ffi.setprivate(self, "_TearDisplacement", value)
    end
end

local TearParamsT = ffi.metatype("struct TearParams", TearParamsMT)
