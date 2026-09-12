ffi.cdef [[
    struct AnimationLayer {
        private const unsigned int LayerID;
        private const struct AnimationFrame* AnimFrames;
        private const unsigned int FrameCount;
        private const bool Visible;
        padding char[0x3];
    };
]]	

local repentogon = ffidll
local ffi = ffi

local function FindAnimationFrame(layer, frame)
    local frames = ffi.getprivate(layer, "AnimFrames")
    local count = ffi.getprivate(layer, "FrameCount")
    for i = 0, count - 1 do
        local animationFrame = frames[i]
        if frame >= animationFrame:GetStartFrame() and frame < animationFrame:GetEndFrame() then
            return animationFrame
        end
    end
    -- Return the last frame.
    return frames[count - 1]
end


local AnimationLayerMT
AnimationLayerMT = {
    __type = "AnimationLayer",
    GetFrame = function(self, frame)
        if ffi.getprivate(self, "FrameCount") == 0 then return nil end

        ffichecks.checkinteger(1, frame)
        if frame < 0 then frame = 0 end

        local animationFrame = FindAnimationFrame(self, frame)

        if not animationFrame then
            -- We clamp the requested frame if needed, so reaching this error
            -- probably indicates a bug or something I didn't expect.
            error("Failed to read an AnimationFrame while looking for frame " .. frame);
        end

        return animationFrame
    end,
    GetLayerID = function(self)
        return ffi.getprivate(self, "LayerID")
    end,
    IsVisible = function(self)
        return ffi.getprivate(self, "Visible")
    end,
}

setmetatable(AnimationLayerMT, { __index = function() end })
AnimationLayerMT.__index = AnimationLayerMT

local AnimationLayerT = ffi.metatype("struct AnimationLayer", AnimationLayerMT)

AnimationLayer = setmetatable({}, {
    __class = AnimationLayerMT
})