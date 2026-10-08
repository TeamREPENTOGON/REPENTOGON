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
        private struct Sprite _ExtraSprite;
        bool ExtraVisible;
        bool Busted;
        private bool CanOpenChallengeRoom;
        padding char;
        int TargetRoomIndex;
        int PreviousState;
        int PreviousVariant;
        private int BehaviorFlags;
        private struct StdString OpenAnimationString;
        private struct StdString CloseAnimationString;
        private struct StdString LockedAnimationString;
        private struct StdString OpenLockedAnimationString;
    };
    typedef struct GridEntityDoor* GridEntityDoorPtr;

    void L_GridEntityDoor_Bar(struct GridEntityDoor*);
    bool L_GridEntityDoor_CanBlowOpen(struct GridEntityDoor*);
    void L_GridEntityDoor_Close(struct GridEntityDoor*, bool);
    bool L_GridEntityDoor_IsLocked(struct GridEntityDoor*);
    bool L_GridEntityDoor_IsTargetRoomArcade(struct GridEntityDoor*);
    void L_GridEntityDoor_Open(struct GridEntityDoor*);
    void L_GridEntityDoor_SpawnDust(struct GridEntityDoor*);
    void L_GridEntityDoor_PlayAnimation(struct GridEntityDoor*);
    void L_GridEntityDoor_Render(struct GridEntityDoor*, struct Vector);
    void L_GridEntityDoor_SetExtraSprite(struct GridEntityDoor*, struct Sprite*);
    void L_GridEntityDoor_SetLocked(struct GridEntityDoor*, bool);
    void L_GridEntityDoor_SetRoomTypes(struct GridEntityDoor*, int, int);
    void L_GridEntityDoor_Update(struct GridEntityDoor*);
    bool L_GridEntityDoor_TryBlowOpen(struct GridEntityDoor*, bool, void*);
    bool L_GridEntityDoor_TryUnlock(struct GridEntityDoor*, struct EntityPlayer*, bool);
]]
    
local ffi = ffi
local repentogon = ffidll


local GridEntityDoorMT
GridEntityDoorMT = {
    __type = "GridEntityDoor",
    Bar = function(self)
        repentogon.L_GridEntityDoor_Bar(self)
    end,
    CanBlowOpen = function(self)
        local result = repentogon.L_GridEntityDoor_CanBlowOpen(self) return result
    end,
    Close = function(self, force) 
        force = ffichecks.optboolean(force, false)
        repentogon.L_GridEntityDoor_Close(self, force)
    end,
    GetExtraSprite = function(self)
        local result = ffi.getprivate(self, "_ExtraSprite") return result
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
        local result = repentogon.L_GridEntityDoor_IsLocked(self) return result
    end,
    IsOpen = function(self)
        return self.Desc.State == 2
    end,
    IsRoomType = function(self, roomType)
        roomType = ffichecks.checkinteger(1, roomType)
        if self.CurrentRoomType ~= roomType and self.TargetRoomType ~= roomType then
            return false
        end
        return true
    end,
    IsTargetRoomArcade = function(self)
        local result = repentogon.L_GridEntityDoor_IsTargetRoomArcade(self) return result
    end,
    SpawnDust = function(self)
        repentogon.L_GridEntityDoor_SpawnDust(self)
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
        locked = ffichecks.checkboolean(1, locked)
        repentogon.L_GridEntityDoor_SetLocked(self, locked)
    end,
    SetRoomTypes = function(self, currentRoomType, targetRoomType)
        currentRoomType = ffichecks.checkinteger(1, currentRoomType)
        targetRoomType = ffichecks.checkinteger(2, targetRoomType)
        repentogon.L_GridEntityDoor_SetRoomTypes(self, currentRoomType, targetRoomType)
    end,
    Update = function(self)
        repentogon.L_GridEntityDoor_Update(self)
    end,
    TryBlowOpen = function(self, fromExplosion, source)
        local result = repentogon.L_GridEntityDoor_TryBlowOpen(self, ffichecks.checkboolean(1, fromExplosion), ffichecks.entitytopointer(source)) return result
    end,
    TryUnlock = function(self, player, force)
        ffichecks.checkcdata(1, player, "EntityPlayer")
        local result = repentogon.L_GridEntityDoor_TryUnlock(self, player, ffichecks.checkboolean(2, force)) return result
    end,
}

local baseIndex = getmetatable(GridEntity).__class.__index
local baseNewindex = getmetatable(GridEntity).__class.__newindex

GridEntityDoorMT.__index = function(self, key)
    if key == "ExtraSprite" then
        local result = ffi.getprivate(self, "_ExtraSprite") return result
    end
    if key == "CloseAnimation" then
        return ffichecks.stdstring(ffi.getprivate(self, "CloseAnimationString"))
    end 
    if key == "LockedAnimation" then
        return ffichecks.stdstring(ffi.getprivate(self, "LockedAnimationString"))
    end
    if key == "OpenAnimation" then
        return ffichecks.stdstring(ffi.getprivate(self, "OpenAnimationString"))
    end
    if key == "OpenLockedAnimation" then
        return ffichecks.stdstring(ffi.getprivate(self, "OpenLockedAnimationString"))
    end
    if GridEntityDoorMT[key] ~= nil then
        return GridEntityDoorMT[key]
    end
    return baseIndex(self, key)
end

GridEntityDoorMT.__newindex = function(self, key, value)
    if key == "ExtraSprite" then
        ffichecks.checkcdata(1, value, "Sprite")
        repentogon.L_GridEntityDoor_SetExtraSprite(self, value)
        return
    end
    baseNewindex(self, key, value)
end

local GridEntityDoorT = ffi.metatype("struct GridEntityDoor", GridEntityDoorMT)
GridEntityDoor = setmetatable({}, {__class = GridEntityDoorMT})

