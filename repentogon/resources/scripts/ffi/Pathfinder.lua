ffi.cdef [[
    struct PathFinder {
        private int GridIndexValue : 0x20;
        private bool HasDirectPathValue : 0x72c;
        private bool CanCrushRocksValue : 0x738;
        private int EvadeMovementCountdownValue : 0x73c;
    } : 0x750;
    typedef struct PathFinder* PathFinderPtr;

    void L_PathFinder_Reset(struct PathFinder*);
    bool L_PathFinder_MoveRandomly(struct PathFinder*, bool);
    void L_PathFinder_MoveRandomlyBoss(struct PathFinder*, bool);
    void L_PathFinder_MoveRandomlyAxisAligned(struct PathFinder*, float, bool);
    void L_PathFinder_FindGridPath(struct PathFinder*, struct Vector*, float, int, bool);
    bool L_PathFinder_HasPathToPos(struct PathFinder*, struct Vector*, bool);
    void L_PathFinder_EvadeTarget(struct PathFinder*, struct Vector*, bool);
    void L_PathFinder_ResetMovementTarget(struct PathFinder*);
    void L_PathFinder_UpdateGridIndex(struct PathFinder*);
    void L_PathFinder_SimulatePlayerMovement(struct PathFinder*, struct Vector*, float);
]]

local ffi = ffi
local repentogon = ffidll

local function VoidMethod(export)
    return function(self)
        export(self)
    end
end

local function Getter(field)
    return function(self)
        return ffi.getprivate(self, field)
    end
end

local PathFinderMT = {
    __type = "PathFinder",
    EvadeTarget = function(self, position, ignoreStatusEffects)
        ffichecks.checkcdata(1, position, "Vector")
        repentogon.L_PathFinder_EvadeTarget(self, position, ffichecks.optboolean(ignoreStatusEffects, false))
    end,
    FindGridPath = function(self, position, speed, pathMarker, useDirectPath)
        ffichecks.checkcdata(1, position, "Vector")
        speed = ffichecks.checknumber(2, speed)
        pathMarker = ffichecks.checkinteger(3, pathMarker)
        repentogon.L_PathFinder_FindGridPath(self, position, speed, pathMarker, ffichecks.optboolean(useDirectPath, false))
    end,
    GetEvadeMovementCountdown = Getter("EvadeMovementCountdownValue"),
    GetGridIndex = Getter("GridIndexValue"),
    HasDirectPath = Getter("HasDirectPathValue"),
    HasPathToPos = function(self, position, ignorePoop)
        ffichecks.checkcdata(1, position, "Vector")
        return repentogon.L_PathFinder_HasPathToPos(self, position, ffichecks.optboolean(ignorePoop, false))
    end,
    MoveRandomly = function(self, ignoreStatusEffects)
        return repentogon.L_PathFinder_MoveRandomly(self, not not ignoreStatusEffects)
    end,
    MoveRandomlyAxisAligned = function(self, speed, ignoreStatusEffects)
        speed = ffichecks.checknumber(1, speed)
        repentogon.L_PathFinder_MoveRandomlyAxisAligned(self, speed, not not ignoreStatusEffects)
    end,
    MoveRandomlyBoss = function(self, ignoreStatusEffects)
        repentogon.L_PathFinder_MoveRandomlyBoss(self, not not ignoreStatusEffects)
    end,
    Reset = VoidMethod(repentogon.L_PathFinder_Reset),
    ResetMovementTarget = VoidMethod(repentogon.L_PathFinder_ResetMovementTarget),
    SetCanCrushRocks = function(self, canCrushRocks)
        ffi.setprivate(self, "CanCrushRocksValue", not not canCrushRocks)
    end,
    SimulatePlayerMovement = function(self, movement, speed)
        ffichecks.checkcdata(1, movement, "Vector")
        speed = ffichecks.checknumber(2, speed)
        repentogon.L_PathFinder_SimulatePlayerMovement(self, movement, speed)
    end,
    UpdateGridIndex = VoidMethod(repentogon.L_PathFinder_UpdateGridIndex),
}

PathFinderMT.__index = PathFinderMT

ffi.metatype("struct PathFinder", PathFinderMT)

PathFinder = setmetatable({}, { __class = PathFinderMT })
