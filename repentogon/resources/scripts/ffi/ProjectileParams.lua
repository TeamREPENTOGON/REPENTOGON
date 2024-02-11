ffi.cdef[[
typedef struct {
    bool GridCollision;
	char[3] _pad1;
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
	ColorMod Color;
	unsigned long long BulletFlags;
	Vector PositionOffset;
	Vector TargetPosition;
	Vector FireDirectionLimit;
	float DotProductLimit;
	int WiggleFrameOffset;
	unsigned long long ChangeFlags;
	float ChangeVelocity;
	unsigned int ChangeTimeout;
	float DepthOffset;
	unsigned int Variant;
	bool _unkFallSpeedAccelMod;
	char[3] _pad2;
	float Damage;
	void* _interpFunc;
	void* _unk
} ProjectileParams;
	
	ProjectileParams* L_ProjectileParams_constructor(ProjectileParams*);
]]

local repentogon = ffidll
local lffi = ffi

local ProjectileParamsMT = lffi.metatype("ProjectileParams", {})

ProjectileParams = setmetatable({}, {
	__call = function()
		local params = ProjectileParamsMT()
		L_ProjectileParams_constructor(params)
		return params
	end,
})