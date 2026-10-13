ffi.cdef[[
struct ProjectileParams {
    bool GridCollision;
    padding char[0x3];
    float HeightModifier;
    float FallingSpeedModifier;
    float FallingAccelModifier;
    float VelocityMulti;
    float Scale;
    float CircleAngle;
    float HomingStrength;
    float CurvingStrength;
    float Acceleration;
    float Spread;
    private struct Color ColorValue;
    uint64_t BulletFlags;
    private struct Vector PositionOffsetValue;
    private struct Vector TargetPositionValue;
    private struct Vector FireDirectionLimitValue;
    float DotProductLimit;
    int32_t WiggleFrameOffset;
    uint64_t ChangeFlags;
    float ChangeVelocity;
    uint32_t ChangeTimeout;
    float DepthOffset;
    uint32_t Variant;
    padding char[0x4];
    private float _Damage;
    padding char[0x8];
};

typedef struct ProjectileParams* ProjectileParamsPtr;
]]

local ffi = ffi

local function CopyStruct(ctype, reference)
    local copy = ffi.new(ctype)
    ffi.copy(copy, reference, ffi.sizeof(ctype))
    return copy
end

local ProjectileParamsMT
ProjectileParamsMT = {
    __type = "ProjectileParams",
}

ProjectileParamsMT.__index = function(self, key)
    if key == "Damage" then
        local result = ffi.getprivate(self, "_Damage") return result
    end
    if key == "Color" then
        return CopyStruct("struct Color", ffi.getprivate(self, "ColorValue"))
    end
    if key == "PositionOffset" then
        return ffichecks.copyvector(ffi.getprivate(self, "PositionOffsetValue"))
    end
    if key == "TargetPosition" then
        return ffichecks.copyvector(ffi.getprivate(self, "TargetPositionValue"))
    end
    if key == "FireDirectionLimit" then
        return ffichecks.copyvector(ffi.getprivate(self, "FireDirectionLimitValue"))
    end

    return ProjectileParamsMT[key]
end

ProjectileParamsMT.__newindex = function(self, key, value)
    if key == "Damage" then
        if value < 0 then
            value = -1
        end
        ffi.setprivate(self, "_Damage", value)
        return
    end
    if key == "Color" then
        ffichecks.checkcdata(1, value, "Color")
        ffi.setprivate(self, "ColorValue", value)
        return
    end
    if key == "PositionOffset" then
        ffichecks.checkcdata(1, value, "Vector")
        ffi.setprivate(self, "PositionOffsetValue", value)
        return
    end
    if key == "TargetPosition" then
        ffichecks.checkcdata(1, value, "Vector")
        ffi.setprivate(self, "TargetPositionValue", value)
        return
    end
    if key == "FireDirectionLimit" then
        ffichecks.checkcdata(1, value, "Vector")
        ffi.setprivate(self, "FireDirectionLimitValue", value)
        return
    end
end

local ProjectileParamsT = ffi.metatype("struct ProjectileParams", ProjectileParamsMT)

local ProjectileParamsGlobalMT = {
    __call = function()
        local result = ProjectileParamsT({
            GridCollision = true,
            HeightModifier = 0,
            FallingSpeedModifier = 0,
            FallingAccelModifier = 0,
            VelocityMulti = 1,
            Scale = 1,
            CircleAngle = 1.6815145015717,
            HomingStrength = 1,
            CurvingStrength = 0.0049999998882413,
            Acceleration = 1.0099999904633,
            Spread = 1,
            ColorValue = Color(1, 1, 1, 1),
            BulletFlags = 0,
            PositionOffsetValue = Vector.Zero,
            TargetPositionValue = Vector.Zero,
            FireDirectionLimitValue = Vector.Zero,
            DotProductLimit = -1000,
            WiggleFrameOffset = 0,
            ChangeFlags = 0,
            ChangeVelocity = 0,
            ChangeTimeout = 30,
            DepthOffset = 0,
            Variant = 0,
            _Damage = -1,
        }) return result
    end,

    __class = ProjectileParamsMT,
}
ProjectileParamsGlobalMT.__index = ProjectileParamsGlobalMT

ProjectileParams = setmetatable({}, ProjectileParamsGlobalMT)
