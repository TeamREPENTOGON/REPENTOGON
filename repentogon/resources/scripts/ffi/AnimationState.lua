ffi.cdef [[
    // We don't directly expose this to the API so I'm not bothering with privates here
    struct AnimationState {
        struct Sprite* Animation;
        struct AnimationData* AnimData;
        struct AnimationFrame* LayerFrames;
        struct NullFrame* NullLayerFrames;
        float AnimFrame;
        bool IsPlaying;
        padding char[0x3];
        unsigned int CurrentlyTriggeredEvents;
        unsigned int PreviouslyTriggeredEvents;
    };

    void L_AnimationState_AdvancePosition(struct AnimationState*, int);
    struct NullFrame* L_AnimationState_GetNullFrame(struct AnimationState*, const char*);
    bool L_AnimationState_IsEventTriggered(struct AnimationState*, const char*);
    void L_AnimationState_Reset(struct AnimationState*);
    void L_AnimationState_Rewind(struct AnimationState*);
    void L_AnimationState_SetLayerFrame(struct AnimationState*, int, int);
    bool L_AnimationState_WasEventTriggered(struct AnimationState*, const char*);
]]
local ffi = ffi
local repentogon = ffidll

-- For the same reason we're not doing typechecking (by the time they get here, they're already checked)
local AnimationStateMT
AnimationStateMT = {
    __type = "AnimationState",
    AdvancePosition = function(self, frame)
        repentogon.L_AnimationState_AdvancePosition(self, frame)
    end,
    GetNullFrame = function(self, name)
        return ffichecks.fixreturn(repentogon.L_AnimationState_GetNullFrame(self, name))
    end,
    IsEventTriggered = function(self, name)
        return repentogon.L_AnimationState_IsEventTriggered(self, name)
    end,
    Play = function(self) 
        if not ffichecks.isnullptr(self.AnimData) then
            if self.AnimFrame < 0 then
                repentogon.L_AnimationState_Rewind(self)
            end
            self.IsPlaying = true
        end
    end,
    Reset = function(self)
        repentogon.L_AnimationState_Reset(self)
    end,
    SetLayerFrame = function(self, layerId, frameNum)
        repentogon.L_AnimationState_SetLayerFrame(self, layerId, frameNum)
    end,
    WasEventTriggered = function(self, name)
        return repentogon.L_AnimationState_WasEventTriggered(self, name)
    end,
}

setmetatable(AnimationStateMT, { __index = function() end })
AnimationStateMT.__index = AnimationStateMT

local AnimationStateT = ffi.metatype("struct AnimationState", AnimationStateMT)

AnimationState = setmetatable({}, {
    __class = AnimationStateMT
})