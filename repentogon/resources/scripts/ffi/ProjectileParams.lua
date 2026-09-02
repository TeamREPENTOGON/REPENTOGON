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
    struct Color Color;
    uint64_t BulletFlags;
    struct Vector PositionOffset;
    struct Vector TargetPosition;
    struct Vector FireDirectionLimit;
    float DotProductLimit;
    int32_t WiggleFrameOffset;
    uint64_t ChangeFlags;
    float ChangeVelocity;
    uint32_t ChangeTimeout;
    float DepthOffset;
    uint32_t Variant;
    padding char[0x4];
    private float _Damage;
};

typedef struct ProjectileParams* ProjectileParamsPtr;
]]

local ffi = ffi

local ProjectileParamsMT
ProjectileParamsMT = {
    __type = "ProjectileParams",
}

ProjectileParamsMT.__index = function(self, key)
    if key == "Damage" then
        return ffi.getprivate(self, "_Damage")
    end

    return ProjectileParamsMT[key]
end

ProjectileParamsMT.__newindex = function(self, key, value)
    if key == "Damage" then
        if value < 0 then
            value = -1
        end
        ffi.setprivate(self, "_Damage", value)
    end
end

local ProjectileParamsT = ffi.metatype("struct ProjectileParams", ProjectileParamsMT)

local ProjectileParamsGlobalMT = {
    __call = function()
        return ProjectileParamsT({
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
            Color = Color(1, 1, 1, 1),
            BulletFlags = 0,
            PositionOffset = Vector.Zero,
            TargetPosition = Vector.Zero,
            FireDirectionLimit = Vector.Zero,
            DotProductLimit = -1000,
            WiggleFrameOffset = 0,
            ChangeFlags = 0,
            ChangeVelocity = 0,
            ChangeTimeout = 30,
            DepthOffset = 0,
            Variant = 0,
            Damage = -1,
        })
    end,

    __class = ProjectileParamsMT,
}
ProjectileParamsGlobalMT.__index = ProjectileParamsGlobalMT

ProjectileParams = setmetatable({}, ProjectileParamsGlobalMT)
