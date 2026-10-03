ffi.cdef [[
    struct Capsule {
        private struct Vector Position : 0x0;
        private struct Vector StartPoint : 0x8;
        private struct Vector EndPoint : 0x10;
        private struct Vector Direction : 0x18;
        private float Size : 0x20;
        private float SizeDifference : 0x24;
    } : 0x28;

    typedef struct Capsule* CapsulePtr;
    
    void L_Capsule_Ctor(struct Capsule*, struct Vector*, struct Vector*, float, float);
    void L_Capsule_Ctor2(struct Capsule*, struct Vector*, struct Vector*, float);
    bool L_Capsule_Collide(struct Capsule*, struct Capsule*, struct Vector*);
]]

local ffi = ffi
local repentogon = ffidll

local CapsuleMT
CapsuleMT = {
    __type = "Capsule",
    Collide = function(self, capsule, point)
        ffichecks.checkcdata(1, capsule, "Capsule")
        ffichecks.checkcdata(2, point, "Vector")
        return repentogon.L_Capsule_Collide(self, capsule, point)
    end,
    GetDirection = function(self)
        local v = ffi.getprivate(self, "Direction")
        return Vector(v.X, v.Y)
    end,
    GetEndPoint = function(self)
        local v = ffi.getprivate(self, "EndPoint")
        return Vector(v.X, v.Y)
    end,
    GetPosition = function(self)
        local v = ffi.getprivate(self, "Position")
        return Vector(v.X, v.Y)
    end,
    GetSize = function(self)
        return ffi.getprivate(self, "Size")
    end,
    GetSizeDifference = function(self)
        return ffi.getprivate(self, "SizeDifference")
    end,
    GetStartPoint = function(self)
        local v = ffi.getprivate(self, "StartPoint")
        return Vector(v.X, v.Y)
    end,
    -- Deprecated methods
    GetF1 = function(self)
        return self:GetSize()
    end,
    GetF2 = function(self)
        return self:GetSizeDifference()
    end,
    GetVec2 = function(self)
        return self:GetStartPoint()
    end,
    GetVec3 = function(self)
        return self:GetEndPoint()
    end,
}

setmetatable(CapsuleMT, { __index = function() end })
CapsuleMT.__index = CapsuleMT

local CapsuleT = ffi.metatype("struct Capsule", CapsuleMT)

Capsule = setmetatable({}, {
    __call = function(_, position, vec2, f1, f2) 
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkcdata(2, vec2, "Vector")
        ffichecks.checknumber(3, f1)
        local capsule = CapsuleT()
        if type(f2) == "number" then
            repentogon.L_Capsule_Ctor(capsule, position, vec2, f1, f2)
        else
            repentogon.L_Capsule_Ctor2(capsule, position, vec2, f1)
        end
        return capsule
    end,
    __class = CapsuleMT,
})
