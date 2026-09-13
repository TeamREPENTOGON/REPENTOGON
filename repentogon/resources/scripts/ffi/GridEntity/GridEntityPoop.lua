ffi.cdef[[
    struct GridEntityPoop {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
        int ReviveTimer;
        padding char StdString_StateAnimation[0x18];
        bool UnderPlayer;
        private bool Destroyed;
        padding char[0x2];
    };
    typedef struct GridEntityPoop* GridEntityPoopPtr;
    
    const char* L_GridEntityPoop_GetStateAnimation(struct GridEntityPoop*);
    bool L_GridEntityPoop_Hurt(struct GridEntityPoop*, int, struct EntityRef*);
    void L_GridEntityPoop_Render(struct GridEntityPoop*, struct Vector);
    void L_GridEntityPoop_RespawnRedPoop(struct GridEntityPoop*);
    void L_GridEntityPoop_Update(struct GridEntityPoop*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityPoopMT
GridEntityPoopMT = {
    __type = "GridEntityPoop",
    Destroy = function(self)
        return repentogon.L_GridEntityPoop_Hurt(self, 4, EntityRef())
    end,
    DestroyWithSource = function(self, _, source)
        ffichecks.checkcdata(2, source, "EntityRef")
        return repentogon.L_GridEntityPoop_Hurt(self, 4, source)
    end,
    Hurt = function(self, damage)
        ffichecks.checkinteger(1, damage)
        return repentogon.L_GridEntityPoop_Hurt(self, damage, EntityRef())
    end,
    HurtWithSource = function(self, damage, source)
        ffichecks.checkinteger(1, damage)
        ffichecks.checkcdata(2, source, "EntityRef")
        return repentogon.L_GridEntityPoop_Hurt(self, damage, source)
    end,
    ReduceSpawnRate = function(self)
        self.Desc.VarData = 1
    end,
    RespawnRedPoop = function(self)
        -- Crazy that we're already at the point where we can just reimplement functions and they'll be close to C speed.
        -- Original C speed after 1,000,000 iters: {320ms}, direct C exposure {90 ms}, this reimplementation {150 ms}
        if self.Desc.Variant == 1 then
            self.Desc.State = 1000
            self.ReviveTimer = 120
            self.CollisionClass = 0

            local sprite = ffi.getprivate(self, "Sprite")
            local anim = sprite:SetAnimation("State5")
            local animState = ffi.getprivate(sprite, "AnimState")
            if anim && not ffichecks.isnullptr(animState.AnimData) then
                animState:SetPosition(ffi.getprivate(sprite:GetAnimationData("State5"), "Length"))
                if not ffichecks.isnullptr(animState.AnimData) then
                    animState.IsPlaying = false
                end
            end
        end
    end,
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityPoop_Render(self, offset)
    end,
    Update = function(self)
        repentogon.L_GridEntityPoop_Update(self)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityPoopMT.__index = function(self, key)
    if key == "StateAnimation" then
        return ffi.string(repentogon.L_GridEntityPoop_GetStateAnimation(self))
    end 
    if GridEntityPoopMT[key] ~= nil then
        return GridEntityPoopMT[key]
    end
    return baseIndex(self, key)
end

GridEntityPoopMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityPoopT = ffi.metatype("struct GridEntityPoop", GridEntityPoopMT)
GridEntityPoop = setmetatable({}, {__class = GridEntityPoopMT})
