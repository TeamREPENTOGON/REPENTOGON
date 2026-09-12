ffi.cdef [[
    struct AnimationData {
        padding char[0x18]; // std::string _name
        private const struct AnimationLayer* AnimationLayers;
        private const unsigned int LayerCount;
        padding char[0x4]; // NullLayer* _nullLayers
        private const unsigned int NullLayerCount;
        padding char[0x4]; // unk
        padding char[0x4]; // unk
        private const int Length;
        private const bool Loop;
        padding char[0x3];
        private int LayerMap[64];
        padding char[0x4]; // unk
    };

    const char* L_AnimationData_GetName(struct AnimationData*);
    bool L_AnimationData_IsEventTriggered(struct AnimationData*, const char*);
]]	

local repentogon = ffidll
local ffi = ffi

local AnimationDataMT
AnimationDataMT = {
    __type = "AnimationData",
    GetAllLayers = function(self)
        local ret = {}
        for i = 0, ffi.getprivate(self, "LayerCount") - 1 do
            table.insert(ret, self:GetLayerByOrder(i))
        end
        return ret
    end,
    GetLayer = function(self, layerId)
        ffichecks.checkinteger(1, layerId)

        if layerId < 0 then return nil end

        local animationLayer = self:GetLayerById(layerId)

        if ffichecks.isnullptr(animationLayer) then
            -- No layer with this ID found. It IS possible for an anm2 file/animation to just not have a layer with a given ID even if it has layers with surrounding IDs.
            return nil
        end

        return animationLayer
    end,
    GetLayerById = function(self, layerId)
        ffichecks.checkinteger(1, layerId)
        return self:GetLayerByOrder(self:GetLayerOrder(layerId))
    end,
    GetLayerByOrder = function(self, layer)
        ffichecks.checkinteger(1, layer)
        local layers = ffi.getprivate(self, "AnimationLayers")
        if not ffichecks.isnullptr(layers) and layer >= 0 and layer < ffi.getprivate(self, "LayerCount") then
            return ffi.cast("const struct AnimationLayer*", layers + layer)
        end
        return nil
    end,
    GetLayerOrder = function(self, layerId)
        ffichecks.checkinteger(1, layerId)
        local map = ffi.getprivate(self, "LayerMap")
        if layerId >= 0 and layerId < 64 then
            return map[layerId]
        end
        return -1
    end,
    GetLength = function(self)
        return ffi.getprivate(self, "Length")
    end,
    GetName = function(self)
        return ffi.string(repentogon.L_AnimationData_GetName(self))
    end,
    IsLoopingAnimation = function(self)
        return ffi.getprivate(self, "Loop")
    end,
    IsEventTriggered = function(self, name)
        ffichecks.checkstring(1, name)
        return repentogon.L_AnimationData_IsEventTriggered(self, name)
    end,
}

setmetatable(AnimationDataMT, { __index = function() end })
AnimationDataMT.__index = AnimationDataMT

local AnimationDataT = ffi.metatype("struct AnimationData", AnimationDataMT)

AnimationData = setmetatable({}, {
    __class = AnimationDataMT
})