ffi.cdef [[
    struct SurfaceRenderController {
        private bool Valid;
    };
    typedef struct SurfaceRenderController* SurfaceRenderControllerPtr;

    void L_SurfaceRenderController_Clear();
    void L_SurfaceRenderController_SetBlendMode(struct BlendMode*);
]]

local repentogon = ffidll
local ffi = ffi

local function CheckValid(self)
    if not ffi.getprivate(self, "Valid") then
        error("This surface render controller has already been applied and cannot be used again", 3)
    end
end

local SurfaceRenderControllerMT
SurfaceRenderControllerMT = {
    __type = "SurfaceRenderController",
    Clear = function(self)
        CheckValid(self)
        repentogon.L_SurfaceRenderController_Clear()
    end,
    SetBlendMode = function(self, blendMode)
        CheckValid(self)
        ffichecks.checkcdata(1, blendMode, "BlendMode")
        repentogon.L_SurfaceRenderController_SetBlendMode(blendMode)
    end,
}

setmetatable(SurfaceRenderControllerMT, { __index = function() end })
SurfaceRenderControllerMT.__index = SurfaceRenderControllerMT

ffi.metatype("struct SurfaceRenderController", SurfaceRenderControllerMT)

SurfaceRenderController = setmetatable({}, {
    __class = SurfaceRenderControllerMT,
})
