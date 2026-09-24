ffi.cdef[[
    struct GridEntityRock {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
        padding char StdString_Anim[0x18];
        padding char StdString_RubbleAnim[0x18];
        padding char[0xc];
        int FrameCnt;
        padding char[0x4];
    };
    typedef struct GridEntityRock* GridEntityRockPtr;
    
    bool L_GridEntityRock_Destroy(struct GridEntityRock*, int, struct EntityRef*);
    int L_GridEntityRock_GetAltRockType(int);
    const char* L_GridEntityRock_GetAnim(struct GridEntityRock*);
    const char* L_GridEntityRock_GetRubbleAnim(struct GridEntityRock*);
    void L_GridEntityRock_PlayBreakSound(struct GridEntityRock*, int, int);
    void L_GridEntityRock_PostInit(struct GridEntityRock*);
    void L_GridEntityRock_RegisterRocksDestroyed(struct GridEntityRock*, int);
    void L_GridEntityRock_Render(struct GridEntityRock*, struct Vector);
    void L_GridEntityRock_RenderTop(struct GridEntityRock*, struct Vector);
    void L_GridEntityRock_SpawnDrops(struct Vector, int, int, unsigned int, bool, int);
    void L_GridEntityRock_Update(struct GridEntityRock*);
    void L_GridEntityRock_UpdateCollision(struct GridEntityRock*);  
    void L_GridEntityRock_UpdateNeighbors(struct GridEntityRock*);
]]
local ffi = ffi
local repentogon = ffidll

local GridEntityRockMT
GridEntityRockMT = {
    __type = "GridEntityRock",
    Destroy = function(self, immediate)
        ffichecks.checkboolean(1, immediate)
        return repentogon.L_GridEntityRock_Destroy(self, immediate, EntityRef())
    end,
    DestroyWithSource = function(self, immediate, source)
        ffichecks.checkboolean(1, immediate)
        ffichecks.checkcdata(2, source, "EntityRef")
        return repentogon.L_GridEntityRock_Destroy(self, immediate, source)
    end,
    GetAltRockType = function(_, backdrop)
        backdrop = backdrop or 0
        ffichecks.checkinteger(1, backdrop)
        return repentogon.L_GridEntityRock_GetAltRockType(backdrop)
    end,
    GetBigRockFrame = function(self)
        if self.Desc.Variant < 1000 then
            return -1
        end
        return self.Desc.Variant - 1000
    end,
    GetRubbleAnim = function(self)
        return ffi.string(repentogon.L_GridEntityRock_GetRubbleAnim(self))
    end,
    PlayBreakSound = function(self, gridType, backdrop)
        ffichecks.checkinteger(1, gridType)
        ffichecks.checkinteger(2, backdrop)
        repentogon.L_GridEntityRock_PlayBreakSound(self, gridType, backdrop)
    end,
    PostInit = function(self)
        repentogon.L_GridEntityRock_PostInit(self)
    end,
    RegisterRocksDestroyed = function(self, gridType)
        repentogon.ffichecks.checkinteger(1, gridType)
        repentogon.L_GridEntityRock_RegisterRocksDestroyed(self, gridType)
    end,
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityRock_Render(self, offset)
    end,
    RenderTop = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityRock_RenderTop(self, offset)
    end,
    TrySpawnLadder = function(self)
        repentogon.L_GridEntityRock_TrySpawnLadder(self)
    end,
    TrySpawnWorms = function(self)
        repentogon.L_GridEntityRock_TrySpawnWorms(self)
    end,
    Update = function(self)
        repentogon.L_GridEntityRock_Update(self)
    end,
    UpdateAnimFrame = function(self)
        if self.Desc.Variant < 1000 then
            return
        end

        local sprite = ffi.getprivate(self, "Sprite")
        local animState = ffi.getprivate(sprite, "AnimState")
        local name = ffi.string(repentogon.L_GridEntityRock_GetAnim(self))

        if sprite:SetAnimation(name, true) and not ffichecks.isnullptr(animState.AnimData) then
            animState:SetPosition(self.Desc.Variant - 1000)
            if not ffichecks.isnullptr(animState.AnimData) then
                animState.IsPlaying = false
            end
        end
    end,

    UpdateCollision = function(self)
        repentogon.L_GridEntityRock_UpdateCollision(self)
    end,
    UpdateNeighbors = function(self)
        repentogon.L_GridEntityRock_UpdateNeighbors(self)
    end,

}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityRockMT.__index = function(self, key)
    if key == "Anim" then
        return ffi.string(repentogon.L_GridEntityRock_GetAnim(self));
    end
    if key == "RubbleAnim" then
        return ffi.string(repentogon.L_GridEntityRock_GetRubbleAnim(self));
    end
    if GridEntityRockMT[key] ~= nil then
        return GridEntityRockMT[key]
    end
    return baseIndex(self, key)
end

GridEntityRockMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityRockT = ffi.metatype("struct GridEntityRock", GridEntityRockMT)
GridEntityRock = setmetatable({
    SpawnDrops = function(position, gridType, gridVariant, seed, unk, backdrop)
        ffichecks.checkcdata(1, position, "Vector")
        ffichecks.checkinteger(2, gridType)
        ffichecks.checkinteger(3, gridVariant)
        ffichecks.checkinteger(4, seed)
        ffichecks.checkboolean(5, unk)
        repentogon.L_GridEntityRock_SpawnDrops(position, gridType, gridVariant, seed, unk, backdrop)
    end,
    GetAltRockType = function(backdrop)
        backdrop = backdrop or 0
        ffichecks.checkinteger(1, backdrop)
        return repentogon.L_GridEntityRock_GetAltRockType(backdrop)
    end,
}, {__class = GridEntityRockMT})