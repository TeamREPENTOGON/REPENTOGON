ffi.cdef[[
typedef struct {
    void* _;
} ANM2;

const char* L_Sprite_GetAnimation(ANM2*);
const char* L_Sprite_GetDefaultAnimation(ANM2*);
const char* L_Sprite_GetFilename(ANM2*);
int L_Sprite_GetFrame(ANM2*);
int L_Sprite_GetLayerCount(ANM2*);
const char* L_Sprite_GetOverlayAnimation(ANM2*);
int L_Sprite_GetOverlayFrame(ANM2*);
Color L_Sprite_GetTexel(ANM2*, Vector, Vector, float, int);
bool L_Sprite_IsEventTriggered(ANM2*, const char*);
bool L_Sprite_IsFinished(ANM2*, const char*);
bool L_Sprite_IsLoaded(ANM2*);
bool L_Sprite_IsOverlayFinished(ANM2*, const char*);
bool L_Sprite_IsOverlayPlaying(ANM2*, const char*);
bool L_Sprite_IsPlaying(ANM2*, const char*);
void L_Sprite_Load(ANM2*, const char*, bool);
void L_Sprite_loadGraphics(ANM2*);
void L_Sprite_Play(ANM2*, const char*, bool);
void L_Sprite_PlayOverlay(ANM2*, const char*, bool);
void L_Sprite_PlayRandom(ANM2*, int);
void L_Sprite_Reload(ANM2*);
void L_Sprite_RemoveOverlay(ANM2*);
void L_Sprite_Render(ANM2*, Vector*, Vector*, Vector*);
void L_Sprite_RenderLayer(ANM2*, int, Vector*, Vector*, Vector*);
void L_Sprite_ReplaceSpritesheet(ANM2*, int, const char*);
void L_Sprite_Reset(ANM2*);
bool L_Sprite_SetAnimation(ANM2*, const char*, bool);
void L_Sprite_SetFrame(ANM2*, const char*, float);
void L_Sprite_SetFramePrecise(ANM2*, int);
void L_Sprite_SetLastFrame(ANM2*);
void L_Sprite_SetLayerFrame(ANM2*, int, int);
bool L_Sprite_SetOverlayAnimation(ANM2*, const char*, bool);
void L_Sprite_SetOverlayFrame(ANM2*, const char*, int);
void L_Sprite_SetOverlayRenderPriority(ANM2*, bool);
void L_Sprite_Stop(ANM2*);
void L_Sprite_Update(ANM2*);
bool L_Sprite_WasEventTriggered(ANM2*, const char*);

ColorMod* L_Sprite_GetColor(ANM2*);
bool L_Sprite_GetFlipX(ANM2*);
bool L_Sprite_GetFlipY(ANM2*);
Vector* L_Sprite_GetOffset(ANM2*);
float L_Sprite_GetPlaybackSpeed(ANM2*);
float L_Sprite_GetRotation(ANM2*);
Vector* L_Sprite_GetScale(ANM2*);

void L_Sprite_SetColor(ANM2*, ColorMod);
void L_Sprite_SetFlipX(ANM2*, bool);
void L_Sprite_SetFlipY(ANM2*, bool);
void L_Sprite_SetOffset(ANM2*, Vector);
void L_Sprite_SetPlaybackSpeed(ANM2*, float);
void L_Sprite_SetRotation(ANM2*, float);
void L_Sprite_SetScale(ANM2*, Vector);

ANM2* L_Sprite_GetPlayerAnim();
]]

local repentogon = ffidll
local lffi = ffi

local SpriteFuncs = {}

function SpriteFuncs:GetAnimation()
    return lffi.string(repentogon.L_Sprite_GetAnimation(self))
end

function SpriteFuncs:GetDefaultAnimation()
    return lffi.string(repentogon.L_Sprite_GetDefaultAnimation(self))
end

function SpriteFuncs:GetDefaultAnimationName()
    return lffi.string(repentogon.L_Sprite_GetDefaultAnimation(self))
end

function SpriteFuncs:GetFilename()
    return lffi.string(repentogon.L_Sprite_GetFilename(self))
end

function SpriteFuncs:GetFrame()
    return repentogon.L_Sprite_GetFrame(self)
end

function SpriteFuncs:GetLayerCount()
    return repentogon.L_Sprite_GetLayerCount(self)
end

function SpriteFuncs:GetOverlayAnimation()
    return lffi.string(repentogon.L_Sprite_GetOverlayAnimation(self))
end

function SpriteFuncs:GetOverlayFrame()
    return repentogon.L_Sprite_GetOverlayFrame(self)
end

function SpriteFuncs:GetTexel(samplePos, renderPos, alphaThreshold, layerId)
    layerId = ffichecks.optnumber(layerId, 0)

    ffichecks.checkcdata(2, samplePos, "Vector")
    ffichecks.checkcdata(3, renderPos, "Vector")
    ffichecks.checknumber(4, alphaThreshold)
    ffichecks.checknumber(5, layerId)

    return repentogon.L_Sprite_GetTexel(self, samplePos, renderPos, alphaThreshold, layerId)
end

function SpriteFuncs:IsEventTriggered(eventName)
    ffichecks.checkstring(2, eventName)

    return repentogon.L_Sprite_IsEventTriggered(self, eventName)
end

function SpriteFuncs:IsFinished(animationName)
    animationName = animationName or ""
    ffichecks.checkstring(2, animationName)
    
    return repentogon.L_Sprite_IsFinished(self, animationName)
end

function SpriteFuncs:IsLoaded()
    return repentogon.L_Sprite_IsLoaded(self)
end

function SpriteFuncs:IsOverlayFinished(animationName)
    animationName = animationName or ""
    ffichecks.checkstring(2, animationName)
    
    return repentogon.L_Sprite_IsOverlayFinished(self, animationName)
end

function SpriteFuncs:IsOverlayPlaying(animationName)
    animationName = animationName or ""
    ffichecks.checkstring(2, animationName)
    
    return repentogon.L_Sprite_IsOverlayPlaying(self, animationName)
end

function SpriteFuncs:IsPlaying(animationName)
    animationName = animationName or ""
    ffichecks.checkstring(2, animationName)
    
    return repentogon.L_Sprite_IsPlaying(self, animationName)
end

function SpriteFuncs:Load(anm2Path, loadGraphics)
    ffichecks.checkstring(2, anm2Path)
    loadGraphics = ffichecks.optboolean(3, loadGraphics, false)

    repentogon.L_Sprite_Load(self, anm2Path, loadGraphics)
end

function SpriteFuncs:LoadGraphics()
    repentogon.L_Sprite_LoadGraphics(self)
end

function SpriteFuncs:Play(animationName, force)
    animationName = animationName or ""
    ffichecks.checkstring(2, animationName)

    repentogon.L_Sprite_Play(self, animationName, force)
end

function SpriteFuncs:PlayOverlay(animationName, force)
    animationName = animationName or ""
    ffichecks.checkstring(2, animationName)

    repentogon.L_Sprite_PlayOverlay(self, animationName, force)
end

function SpriteFuncs:PlayRandom(seed)
    ffichecks.checknumber(2, seed)

    repentogon.L_Sprite_PlayRandom(self, seed)
end

function SpriteFuncs:Reload()
    repentogon.L_Sprite_Reload(self)
end

function SpriteFuncs:RemoveOverlay()
    repentogon.L_Sprite_RemoveOverlay(self)
end

function SpriteFuncs:Render(position, topLeftClamp, bottomRightClamp)
    topLeftClamp = topLeftClamp or Vector.Zero
    bottomRightClamp = bottomRightClamp or Vector.Zero

    ffichecks.checkcdata(2, position, "Vector")
    ffichecks.checkcdata(3, topLeftClamp, "Vector")
    ffichecks.checkcdata(4, bottomRightClamp, "Vector")

    repentogon.L_Sprite_Render(self, position, topLeftClamp, bottomRightClamp)
end

function SpriteFuncs:RenderLayer(layerId, position, topLeftClamp, bottomRightClamp)
    topLeftClamp = topLeftClamp or Vector.Zero
    bottomRightClamp = bottomRightClamp or Vector.Zero

    ffichecks.checknumber(2, layerId)
    ffichecks.checkcdata(3, position, "Vector")
    ffichecks.checkcdata(4, topLeftClamp, "Vector")
    ffichecks.checkcdata(5, bottomRightClamp, "Vector")

    repentogon.L_Sprite_RenderLayer(self, layerId, position, topLeftClamp, bottomRightClamp)
end

function SpriteFuncs:ReplaceSpritesheet(layerId, pngFilename)
    ffichecks.checknumber(2, layerId)
    ffichecks.checkstring(3, pngFilename)

    repentogon.L_Sprite_ReplaceSpritesheet(self, layerId, pngFilename)
end

function SpriteFuncs:Reset()
    repentogon.L_Sprite_Reset(self)
end

function SpriteFuncs:SetAnimation(animationName, reset)
    ffichecks.checkstring(2, animationName)
    reset = ffichecks.optboolean(3, reset, true)

    return repentogon.L_Sprite_SetAnimation(self, animationName, reset)
end

function SpriteFuncs:SetFrame(animationName, frame)
    if frame == nil then
        ffichecks.checknumber(2, animationName)
        repentogon.L_Sprite_SetFramePrecise(self, animationName)
    else 
        ffichecks.checkstring(2, animationName)
        ffichecks.checknumber(3, frame)
        repentogon.L_Sprite_SetFrame(self, animationName, frame)
    end
end

function SpriteFuncs:SetLastFrame()
    repentogon.L_Sprite_SetLastFrame(self)
end

function SpriteFuncs:SetLayerFrame(layerId, frame)
    ffichecks.checknumber(2, layerId)
    ffichecks.checknumber(3, frame)

    repentogon.L_Sprite_SetLayerFrame(self, layerId, frame)
end

function SpriteFuncs:SetOverlayAnimation(animationName, reset)
    ffichecks.checkstring(2, animationName)
    reset = ffichecks.optboolean(3, reset, true)

    return repentogon.L_Sprite_SetOverlayAnimation(self, animationName, reset)
end

function SpriteFuncs:SetOverlayFrame(animationName, frame)
    ffichecks.checkstring(2, animationName)
    frame = ffichecks.checknumber(3, frame)

    repentogon.L_Sprite_SetOverlayFrame(self, animationName, frame)
end

function SpriteFuncs:SetOverlayRenderPriority(renderFirst)
    ffichecks.checkboolean(2, renderFirst)

    repentogon.L_Sprite_SetOverlayRenderPriority(self, renderFirst)
end

function SpriteFuncs:Stop()
    repentogon.L_Sprite_Stop(self)
end

function SpriteFuncs:Update()
    repentogon.L_Sprite_Update(self)
end

function SpriteFuncs:WasEventTriggered(event)
    ffichecks.checkstring(2, event)

    return repentogon.L_Sprite_WasEventTriggered(self, event)
end

local getkeys = {
    Color = repentogon.L_Sprite_GetColor,
    FlipX = repentogon.L_Sprite_GetFlipX,
    FlipY = repentogon.L_Sprite_GetFlipY,
    Offset = repentogon.L_Sprite_GetOffset,
    PlaybackSpeed = repentogon.L_Sprite_GetPlaybackSpeed,
    Rotation = repentogon.L_Sprite_GetRotation,
    Scale = repentogon.L_Sprite_GetScale
}

local setkeys = {
    Color = function(self, color)
       ffichecks.checkcdata(2, color, "ColorMod") 
       repentogon.L_Sprite_SetColor(self, color)
    end,
    FlipX = repentogon.L_Sprite_SetFlipX,
    FlipY = repentogon.L_Sprite_SetFlipY,
    Offset = function(self, offset)
        ffichecks.checkcdata(2, offset, "Vector") 
        repentogon.L_Sprite_SetOffset(self, offset)
    end,
    PlaybackSpeed = repentogon.L_Sprite_SetPlaybackSpeed,
    Rotation = repentogon.L_Sprite_SetRotation,
    Scale = function(self, scale)
        ffichecks.checkcdata(2, scale, "Vector") 
        repentogon.L_Sprite_SetScale(self, scale)
    end,
}

local SpriteMT = lffi.metatype("ANM2", {
    __tostring = function(self)
        return "Sprite(" .. repentogon.L_Sprite_GetFilename(self) .. ")"
    end,
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        end
		return SpriteFuncs;
    end,
    __newindex = function(self, key, value)
        if setkeys[key] ~= nil then
            return setkeys[key](self, value)
        else
            error(string.format("no writable variable '%s'", key))
        end
    end
})

Sprite = setmetatable({}, {
    __call = function() 
        return repentogon.L_Sprite_GetPlayerAnim() 
    end
})