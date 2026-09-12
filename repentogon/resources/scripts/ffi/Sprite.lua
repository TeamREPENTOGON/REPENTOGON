ffi.cdef[[
struct Sprite {
    padding char[0x18]; // std::string _filename
    padding char[0x18]; // std::string _animDefaultName
    private struct AnimationState AnimState;
    private struct AnimationState OverlayAnimState;
    private bool OverlayRenderFirst;
    padding char[0x3];
    private struct AnimationData* AnimationData;
    private int AnimCount;
    private struct LayerState* LayerState;
    private unsigned int LayerCount;
    padding char[0x4]; // std::string* _nullLayerNames
    private unsigned int NullLayerCount;
    padding char[0x4]; // std::string* _eventNames
    private unsigned int EventCount;
    struct Vector Offset;
    struct Vector Scale;
    float Rotation;
    struct Color Color;
    private struct Color ChampionColor;
    bool FlipX;
    bool FlipY;
    padding char[0x2];
    float PlaybackSpeed;
    private bool unk;
    private bool Loaded;
    padding char[0x2];
    private int ShadowLayer;
    private int RenderFlags;
};
typedef struct Sprite* SpritePtr;

struct Sprite* L_Sprite_New();
void L_Sprite_Delete(struct Sprite*);
void L_Sprite_ClearCustomShader(struct Sprite*, bool);
const char* L_Sprite_GetAnimation(struct Sprite*);
struct AnimationData* L_Sprite_GetAnimationData(struct Sprite*, const char*);
const char* L_Sprite_GetDefaultAnimation(struct Sprite*);
const char* L_Sprite_GetFilename(struct Sprite*);
struct LayerState* L_Sprite_GetLayerById(struct Sprite*, int);
struct LayerState* L_Sprite_GetLayerByName(struct Sprite*, const char*);
const char* L_Sprite_GetOverlayAnimation(struct Sprite*);
struct KColor* L_Sprite_GetTexel(struct Sprite*, struct KColor*, struct Vector, struct Vector, float, int);
bool L_Sprite_HasCustomShader(struct Sprite*, bool);
bool L_Sprite_HasCustomShaderWithPath(struct Sprite*, const char*, bool);
bool L_Sprite_IsFinished(struct Sprite*, const char*);
bool L_Sprite_IsOverlayFinished(struct Sprite*, const char*);
void L_Sprite_Load(struct Sprite*, const char*, bool);
void L_Sprite_LoadGraphics(struct Sprite*);
void L_Sprite_Play(struct Sprite*, const char*, bool);
void L_Sprite_PlayOverlay(struct Sprite*, const char*, bool);
void L_Sprite_PlayRandom(struct Sprite*, unsigned int);
void L_Sprite_Reload(struct Sprite*);
void L_Sprite_Render(struct Sprite*, struct Vector*, struct Vector*, struct Vector*);
void L_Sprite_RenderLayer(struct Sprite*, int, struct Vector*, struct Vector*, struct Vector*);
bool L_Sprite_ReplaceSpritesheet(struct Sprite*, int, const char*);
void L_Sprite_Reset(struct Sprite*);
bool L_Sprite_SetAnimation(struct Sprite*, const char*, bool);
bool L_Sprite_SetCustomShader(struct Sprite*, const char*, bool);
void L_Sprite_SetFrame(struct Sprite*, int);
void L_Sprite_SetFrameWithAnim(struct Sprite*, const char*, int);
bool L_Sprite_SetOverlayAnimation(struct Sprite*, const char*, bool);
void L_Sprite_SetOverlayFrame(struct Sprite*, int);
void L_Sprite_SetOverlayFrameWithAnim(struct Sprite*, const char*, int);
void L_Sprite_Update(struct Sprite*);
]]
local ffi = ffi
local repentogon = ffidll

local function GetLayerFrameDataInternal(animData, layerId)
        local animData = ffichecks.isnullptr(animState) and nil or animState.animData
        if not animdata then
            return nil
        end
        local layerIndex = animData:GetLayerOrder(layerId)
        local animLayer = animData:GetLayerById(layerId)
        if layerIndex < 0 or not animLayer then
            return nil
        end
        local animFrameIndex = math.max(0, math.min(ffi.getprivate(animLayer, "FrameCount") - 1, animState.LayerFrames[layerIndex]))
        local animFrame = animLayer->GetFrame(animFrameindex)

        return animFrame;
end

local SpriteMT
SpriteMT = {
    __type = "Sprite",
    ClearCustomChampionShader = function(self)
        repentogon.L_Sprite_ClearCustomShader(self, true)
    end,
    ClearCustomShader = function(self)
        repentogon.L_Sprite_ClearCustomShader(self, false)
    end,
    Continue = function(self, continueOverlay) 
        continueOverlay = ffichecks.optboolean(continueOverlay, true)
        ffi.getprivate(self, "AnimState"):Play()
        if continueOverlay then
            ffi.getprivate(self, "OverlayAnimState"):Play()
        end
    end,
    ContinueOverlay = function(self) 
        ffi.getprivate(self, "OverlayAnimState"):Play()
    end,
    GetAllAnimationData = function(self)
        local ret = {}
        for i = 0, ffi.getprivate(self, "AnimCount") - 1 do
            table.insert(ret, ffi.getprivate(self, "AnimationData")[i])
        end
        return ret
    end,
    GetAllLayers = function(self)
        local ret = {}
        for i = 0, ffi.getprivate(self, "LayerCount") - 1 do
            table.insert(ret, ffi.getprivate(self, "LayerState")[i])
        end
        return ret
    end,
    GetAnimation = function(self)
        return ffi.string(repentogon.L_Sprite_GetAnimation(self))
    end,
    GetAnimationData = function(self, animationName)
        ffichecks.checkstring(1, animationName)
        return ffichecks.fixreturn(repentogon.L_Sprite_GetAnimationData(self, animationName))
    end,
    GetCurrentAnimationData = function(self)
        return ffichecks.fixreturn(ffi.getprivate(self, "AnimState").AnimData)
    end,
    GetDefaultAnimation = function(self)
        return ffi.string(repentogon.L_Sprite_GetDefaultAnimation(self))
    end,
    GetDefaultAnimationName = function(self)
        return ffi.string(repentogon.L_Sprite_GetDefaultAnimation(self))
    end,
    GetFilename = function(self)
        return ffi.string(repentogon.L_Sprite_GetFilename(self))
    end,
    GetFrame = function(self)
        local state = ffi.getprivate(self, "AnimState")
        if ffichecks.isnullptr(state.AnimData) then
            return -1
        end
        return state.AnimFrame
    end,
    GetLayer = function(self, param)
        local layerState = nil
        if ffichecks.isstring(param) then
            layerState = repentogon.L_Sprite_GetLayerByName(self, param)
        elseif param >= 0 and param < ffi.getprivate(self, "LayerCount") then
            layerState = repentogon.L_Sprite_GetLayerById(self, param)
        end
        return ffichecks.fixreturn(layerState)
    end,
    GetLayerCount = function(self)
        return ffi.getprivate(self, "LayerCount")
    end,
    GetLayerFrameData = function(self, layerId)
        ffichecks.checkinteger(1, layerId)
        return GetLayerFrameDataInternal(ffi.getprivate(self, "AnimState"), layerId)
    end,
	GetNullFrame = function(self, name)
        ffichecks.checkstring(1, name)
        return ffi.getprivate(self, "AnimState"):GetNullFrame(name)
    end,
    GetOverlayNullFrame = function(self, name)
        ffichecks.checkstring(1, name)
        return ffi.getprivate(self, "OverlayAnimState"):GetNullFrame(name)
    end,
    GetOverlayAnimation = function(self)
       local animData = ffi.getprivate(self, "OverlayAnimState").AnimData
       if ffichecks.isnullptr(animData) then return "" end
       return animData:GetName()
    end,
    GetOverlayAnimationData = function(self)
       return ffi.getprivate(self, "OverlayAnimState").AnimData
    end,
    GetOverlayFrame = function(self)
       return ffi.getprivate(self, "OverlayAnimState").AnimFrame
    end,
    GetOverlayLayerFrameData = function(self, layerId)
        ffichecks.checkinteger(1, layerId)
        return GetLayerFrameDataInternal(ffi.getprivate(self, "OverlayAnimState"), layerId)
    end,
    GetRenderFlags = function(self)
        return ffi.getprivate(self, "RenderFlags")
    end,
    GetSpritesheet = function(self, layer)
        ffichecks.checkinteger(1, layer)
        layer = self:GetLayer(layer)
        if not layer then 
            return nil 
        end
        return ffi.getprivate(layer, "Spritesheet")
    end,
    GetTexel = function(self, samplePos, renderPos, alphaThreshold, layerId)
        ffichecks.checkcdata(1, samplePos, "Vector")
        ffichecks.checkcdata(2, renderPos, "Vector")
        ffichecks.checknumber(3, alphaThreshold)
        ffichecks.checknumber(4, layerId)
        local result = ffi.new("struct KColor")
        if repentogon.L_Sprite_GetTexel(self, result, samplePos, renderPos, alphaThreshold, layerId) == nil then
            return nil
        end
        return result
    end,
    HasCustomShader = function(self, path)
        path = ffichecks.optstring(path, "")
        if path == "" then
            return repentogon.L_Sprite_HasCustomShader(self, true)
        end
        return repentogon.L_Sprite_HasCustomShaderWithPath(self, path, true)
    end,
    HasCustomChampionShader = function(self, path)
        path = ffichecks.optstring(path, "")
        if path == "" then
            return repentogon.L_Sprite_HasCustomShader(self, true)
        end
        return repentogon.L_Sprite_HasCustomShaderWithPath(self, path, true)
    end,
    IsEventTriggered = function(self, name)
        ffichecks.checkstring(1, name)
        return ffi.getprivate(self, "AnimState"):IsEventTriggered(name)
    end,
    IsFinished = function(self, name) 
        name = ffichecks.optstring(name, "")
        return repentogon.L_Sprite_IsFinished(self, name)
    end,
    IsLoaded = function(self) 
        return ffi.getprivate(self, "Loaded")
    end,
    IsOverlayEventTriggered = function(self, name)
        ffichecks.checkstring(1, name)
        return ffi.getprivate(self, "OverlayAnimState"):IsEventTriggered(name)
    end,
    IsOverlayFinished = function(self, name) 
        name = ffichecks.optstring(name, "")
        return repentogon.L_Sprite_IsOverlayFinished(self, name)
    end,
    IsOverlayPlaying = function(self, name)
        name = ffichecks.optstring(name, "")
        local state = ffi.getprivate(self, "OverlayAnimState")
        if not state.IsPlaying then return false end
        if name == "" then return true end
        return self:GetOverlayAnimation() == name
    end,
    IsPlaying = function(self, name)
        name = ffichecks.optstring(name, "")
        local state = ffi.getprivate(self, "AnimState")
        if not state.IsPlaying then return false end
        if name == "" then return true end
        return self:GetAnimation() == name
    end,
    Load = function(self, path, loadGraphics) 
        ffichecks.checkstring(1, path)
        loadGraphics = ffichecks.optboolean(loadGraphics, true)
        repentogon.L_Sprite_Load(self, path, loadGraphics)
    end,
    LoadGraphics = function(self)
        repentogon.L_Sprite_LoadGraphics(self)
    end,
    Play = function(self, animationName, force)
        ffichecks.checkstring(1, animationName)
        force = ffichecks.optboolean(force, false)
        repentogon.L_Sprite_Play(self, animationName, force)
    end,
    PlayOverlay = function(self, animationName, force)
        ffichecks.checkstring(1, animationName)
        force = ffichecks.optboolean(force, false)
        repentogon.L_Sprite_PlayOverlay(self, animationName, force)
    end,
    PlayRandom = function(self, seed)
        ffichecks.checkinteger(1, seed)
        repentogon.L_Sprite_PlayRandom(self, seed)
    end,
    Reload = function(self)
        repentogon.L_Sprite_Reload(self)
    end,
    RemoveOverlay = function(self)
        ffi.getprivate(self, "OverlayAnimState"):Reset()
    end,
    Render = function(self, position, topLeftClamp, bottomRightClamp)
        ffichecks.checkcdata(1, position, "Vector")
        topLeftClamp = ffichecks.optcdata(topLeftClamp, "Vector", Vector.Zero)
        bottomRightClamp = ffichecks.optcdata(bottomRightClamp, "Vector", Vector.Zero)
        repentogon.L_Sprite_Render(self, position, topLeftClamp, bottomRightClamp)
    end,
    RenderLayer = function(self, layerId, position, topLeftClamp, bottomRightClamp)
        ffichecks.checkinteger(1, layerId)
        ffichecks.checkcdata(2, position, "Vector")
        topLeftClamp = ffichecks.optcdata(topLeftClamp, "Vector", Vector.Zero)
        bottomRightClamp = ffichecks.optcdata(bottomRightClamp, "Vector", Vector.Zero)
        repentogon.L_Sprite_RenderLayer(self, layerId, position, topLeftClamp, bottomRightClamp)
    end,
    ReplaceSpritesheet = function(self, layerId, filename, loadGraphics) 
        ffichecks.checkinteger(1, layerId)
        ffichecks.checkstring(2, filename)
        loadGraphics = ffichecks.optboolean(loadGraphics, false)
        repentogon.L_Sprite_ReplaceSpritesheet(self, layerId, filename)
        if loadGraphics then
            repentogon.L_Sprite_LoadGraphics(self)
        end
    end,
    Reset = function(self) 
        repentogon.L_Sprite_Reset(self)
    end,
    SetAnimation = function(self, name, reset)
        ffichecks.checkstring(1, name)
        reset = ffichecks.optboolean(reset, true)
        return repentogon.L_Sprite_SetAnimation(self, name, reset)
    end,
    SetCustomChampionShader = function(self, path)
        ffichecks.checkstring(1, path)
        if not repentogon.L_Sprite_SetCustomShader(self, path, true) then 
            error("Failed to load shader: " .. path)
        end
    end,
    SetChampionShader = function(self, path)
        ffichecks.checkstring(1, path)
        if not repentogon.L_Sprite_SetCustomShader(self, path, true) then 
            error("Failed to load shader: " .. path)
        end
    end,
    SetFrame = function(self, param, param2)
        if ffichecks.isstring(param) then
            ffichecks.checkinteger(2, param2)
            repentogon.L_Sprite_SetFrameWithAnim(self, param, param2)
        else
            ffichecks.checkinteger(1, param)
            repentogon.L_Sprite_SetFrame(self, param)
        end
    end,
    SetLastFrame = function(self)
        animState = ffi.getprivate(self, "AnimState")
        if animState.AnimData == nil then return end
        animState:AdvancePosition(animState.AnimData:GetLength() - 1)
    end,
    SetLayerFrame = function(self, layerId, frameNum)
        ffichecks.checknumber(1, layerId)
        ffichecks.checknumber(2, frameNum)
        animState = ffi.getprivate(self, "AnimState")
        if animState.AnimData == nil then return end
        animState:SetLayerFrame(layerId, frameNum)
    end,
    SetOverlayAnimation = function(self, name, reset)
        ffichecks.checkstring(1, name)
        reset = ffichecks.optboolean(reset, true)
        return L_Sprite_SetOverlayAnimation(self, name, reset);
    end,
    SetOverlayFrame = function(self, param, param2)
        if ffichecks.isstring(param) then
            ffichecks.checkinteger(2, param2)
            repentogon.L_Sprite_SetOverlayFrameWithAnim(self, param, param2)
        else
            ffichecks.checkinteger(1, param)
            repentogon.L_Sprite_SetOverlayFrame(self, param)
        end
    end,
    SetOverlayLayerFrame = function(self, layerId, frameNum)
        ffichecks.checknumber(1, layerId)
        ffichecks.checknumber(2, frameNum)
        animState = ffi.getprivate(self, "OverlayAnimState")
        if animState.AnimData == nil then return end
        animState:SetLayerFrame(layerId, frameNum)
    end,
    SetOverlayRenderPriority = function(self, renderFirst)
        ffichecks.checkboolean(1, renderFirst)
        ffi.setprivate(self, "OverlayRenderFirst", renderFirst)
    end,
    SetRenderFlags = function(self, renderFlags)
        ffichecks.checkinteger(1, renderFlags)
        return ffi.setprivate(self, "RenderFlags", renderFlags)
    end,
    SetSpritesheet = function(self, layer, spritesheet)
        ffichecks.checkinteger(1, layer)
        ffichecks.checkcdata(2, spritesheet, "Image")
        layer = self:GetLayer(layer)
        if not layer then 
            return false
        end
        ffi.setprivate(layer, "Spritesheet", spritesheet)
        return true
    end,
    Stop = function(self)
        ffi.getprivate(self, "AnimState").IsPlaying = false
    end,
    StopOverlay = function(self)
        ffi.getprivate(self, "OverlayAnimState").IsPlaying = false
    end,
    Update = function(self)
        repentogon.L_Sprite_Update(self)
    end,
    WasEventTriggered = function(self, name)
        ffichecks.checkstring(1, name)
        return ffi.getprivate(self, "AnimState"):WasEventTriggered(name)
    end,
    WasOverlayEventTriggered = function(self, name)
        ffichecks.checkstring(1, name)
        return ffi.getprivate(self, "OverlayAnimState"):WasEventTriggered(name)
    end,
}

setmetatable(SpriteMT, {
    __index = function() end,
})
SpriteMT.__index = SpriteMT

local SpriteT = ffi.metatype("struct Sprite", SpriteMT)

Sprite = setmetatable({}, {
    __class = SpriteMT,
    __call = function(_, anm2Path, loadGraphics)
        anm2Path = ffichecks.optstring(anm2Path, "")
        loadGraphics = ffichecks.optboolean(loadGraphics, true)
        local isLoaded = false

        local sprite = ffi.gc(repentogon.L_Sprite_New(), repentogon.L_Sprite_Delete)

        if anm2Path ~= "" then
            sprite:Load(anm2Path, loadGraphics)
        end
        isLoaded = sprite:GetLayerCount() > 0
        return sprite, isLoaded
    end,
})