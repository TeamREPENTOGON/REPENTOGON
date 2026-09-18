ffi.cdef[[
    struct GridEntityDoor {
        padding void* vtable;
        struct GridEntityDesc Desc;
        private int GridIndex;
        private int SpawnFrame;
        private struct RNG RNG;
        int CollisionClass;
        private struct Sprite Sprite;
        padding char[0x4];
        int Slot;
        int CurrentRoomType;
        int TargetRoomType;
        int Direction;
        struct Sprite ExtraSprite;
        bool ExtraVisible;
        bool Busted;
        private bool CanOpenChallengeRoom;
        padding char;
        int TargetRoomIndex;
        int PreviousState;
        int PreviousVariant;
        private int BehaviorFlags;
        padding char StdString_OpenAnimation[0x18];
        padding char StdString_CloseAnimation[0x18];
        padding char StdString_LockedAnimation[0x18];
        padding char StdString_OpenLockedAnimation[0x18];
    };
    typedef struct GridEntityDoor* GridEntityDoorPtr;

    void L_GridEntityDoor_Bar(struct GridEntityDoor*);
    bool L_GridEntityDoor_CanBlowOpen(struct GridEntityDoor*);
    void L_GridEntityDoor_Close(struct GridEntityDoor*, bool);
    const char* L_GridEntityDoor_GetCloseAnimation(struct GridEntityDoor*);
    const char* L_GridEntityDoor_GetLockedAnimation(struct GridEntityDoor*);
    const char* L_GridEntityDoor_GetOpenAnimation(struct GridEntityDoor*);
    const char* L_GridEntityDoor_GetOpenLockedAnimation(struct GridEntityDoor*);
    bool L_GridEntityDoor_IsLocked(struct GridEntityDoor*);
    bool L_GridEntityDoor_IsTargetRoomArcade(struct GridEntityDoor*);
    void L_GridEntityDoor_Open(struct GridEntityDoor*);
    void L_GridEntityDoor_PlayAnimation(struct GridEntityDoor*);
    void L_GridEntityDoor_Render(struct GridEntityDoor*, struct Vector);
    void L_GridEntityDoor_SetLocked(struct GridEntityDoor*, bool);
    void L_GridEntityDoor_SetRoomTypes(struct GridEntityDoor*, int, int);

]]
    
local ffi = ffi
local repentogon = ffidll

local cfuncs = {
    TryBlowOpen = __Lua_GridEntityDoor_TryBlowOpen,
    TryUnlock = __Lua_GridEntityDoor_TryUnlock
}

local GridEntityDoorMT
GridEntityDoorMT = {
    __type = "GridEntityDoor",
    Bar = function(self)
        repentogon.L_GridEntityDoor_Bar(self)
    end,
    CanBlowOpen = function(self)
        return repentogon.L_GridEntityDoor_CanBlowOpen(self)
    end,
    Close = function(self, force) 
        force = ffichecks.optboolean(force, false)
        repentogon.L_GridEntityDoor_Close(self, force)
    end,
    GetExtraSprite = function(self)
        return self.ExtraSprite
    end,
    GetSpriteOffset = function(self)
        return ffi.getprivate(self, "Sprite").Offset
    end,
    IsBusted = function(self)
        return self.Busted
    end,
    IsKeyFamiliarTarget = function(self)
        return self.Desc.Variant == 5
    end,
    IsLocked = function(self)
        return repentogon.L_GridEntityDoor_IsLocked(self)
    end,
    IsOpen = function(self)
        return self.Desc.State == 2
    end,
    IsRoomType = function(self, roomType)
        ffichecks.checkinteger(1, roomType)
        if self.CurrentRoomType ~= roomType and self.TargetRoomType ~= roomType then
            return false
        end
        return true
    end,
    IsTargetRoomArcade = function(self)
        return repentogon.L_GridEntityDoor_IsTargetRoomArcade(self)
    end,
    Open = function(self)
        repentogon.L_GridEntityDoor_Open(self)
    end,
    PlayAnimation = function(self)
        repentogon.L_GridEntityDoor_PlayAnimation(self)
    end,
    Render = function(self, offset)
        ffichecks.checkcdata(1, offset, "Vector")
        repentogon.L_GridEntityDoor_Render(self, offset)
    end,
    SetLocked = function(self, locked)
        ffichecks.checkboolean(1, locked)
        repentogon.L_GridEntityDoor_SetLocked(self, locked)
    end,
    SetRoomTypes = function(self, currentRoomType, targetRoomType)
        ffichecks.checkinteger(1, currentRoomType)
        ffichecks.checkinteger(2, targetRoomType)
        repentogon.L_GridEntityDoor_SetRoomTypes(self, currentRoomType, targetRoomType)
    end,
    Update = function(self)
        repentogon.L_GridEntityDoor_Update(self)
    end,
    TryBlowOpen = function(self, fromExplosion, source)
        return cfuncs.TryBlowOpen(self, fromExplosion, source)
    end,
    TryUnlock = function(self, player, force)
        return cfuncs.TryUnlock(self, player, force)
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityDoorMT.__index = function(self, key)
    if key == "CloseAnimation" then
        return ffi.string(repentogon.L_GridEntityDoor_GetCloseAnimation(self))
    end 
    if key == "LockedAnimation" then
        return ffi.string(repentogon.L_GridEntityDoor_GetLockedAnimation(self))
    end
    if key == "OpenAnimation" then
        return ffi.string(repentogon.L_GridEntityDoor_GetOpenAnimation(self))
    end
    if key == "OpenLockedAnimation" then
        return ffi.string(repentogon.L_GridEntityDoor_GetOpenLockedAnimation(self))
    end
    if GridEntityDoorMT[key] ~= nil then
        return GridEntityDoorMT[key]
    end
    return baseIndex(self, key)
end

GridEntityDoorMT.__newindex = function(self, key, value)
    baseNewindex(self, key, value)
end

local GridEntityDoorT = ffi.metatype("struct GridEntityDoor", GridEntityDoorMT)
GridEntityDoor = setmetatable({}, {__class = GridEntityDoorMT})

__Lua_GridEntityDoor_TryBlowOpen = nil
__Lua_GridEntityDoor_TryUnlock = nil