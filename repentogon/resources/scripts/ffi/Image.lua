ffi.cdef[[
struct Image {
    padding char[0x8]; // We just need the right shape for GC purposes.
};

typedef struct Image* ImagePtr;

void L_Image_DecrRef(struct Image*);
int L_Image_GetHeight(struct Image*);
const char* L_Image_GetName(struct Image*);
int L_Image_GetPaddedWidth(struct Image*);
int  L_Image_GetPaddedHeight(struct Image*);    
void L_Image_GetTexelRegion(struct Image*, int, int, uint32_t, uint32_t, void*);
int L_Image_GetWidth(struct Image*);
void L_Image_Render(struct Image*, struct SourceQuad*, struct DestinationQuad*, struct KColor*, struct Color*);
void L_Image_RenderWithShader(struct Image*, struct SourceQuad*, struct DestinationQuad*, struct KColor*, struct Shader*);
float* L_Image_SubmitQuadForShader(struct Image*, struct SourceQuad*, struct DestinationQuad*, struct KColor*, struct Shader*);
]]

local ffi = ffi
local repentogon = ffidll

local FMT_SIZE = { [1]=1, [2]=2, [3]=3, [4]=4, [5]=3, [6]=4, [7]=2, [8]=1 }

local function fill_shader_vertices(vb, shader, params)
    local attrs = shader._vertexAttributes
    local n = shader._numVertexAttributes
    local errs

    local vertexSize = 0
    for i = 0, n - 1 do
        vertexSize = vertexSize + (FMT_SIZE[attrs[i].format] or 0)
    end

    local off = 0
    for i = 0, n - 1 do
        local a, fmt = attrs[i], attrs[i].format

        if fmt == 1 then
            local name = ffi.string(a.name)
            local v = params[name]
            if type(v) ~= "number" then
                errs = errs or {}; errs[#errs + 1] = name
            else
                for q = 0, 3 do vb[off + q * vertexSize] = v end
            end
            off = off + 1

        elseif fmt == 2 or fmt == 3 or fmt == 4 then
            local name = ffi.string(a.name)
            local t = params[name]
            local ok = type(t) == "table"
            if ok then
                for k = 1, fmt do if type(t[k]) ~= "number" then ok = false break end end
            end
            if not ok then
                errs = errs or {}; errs[#errs + 1] = name
            else
                for k = 0, fmt - 1 do
                    local e = t[k + 1]
                    for q = 0, 3 do vb[off + k + q * vertexSize] = e end
                end
            end
            off = off + fmt

        else
            off = off + (FMT_SIZE[fmt] or 0)
        end
    end

    return errs
end

local ImageMT
ImageMT = {
    __type  = "Image",
    __gc = function(self)
        repentogon.L_Image_DecrRef(self)
    end,

    GetWidth = function(self) 
        return repentogon.L_Image_GetWidth(self) 
    end,
    GetHeight = function(self) 
        return repentogon.L_Image_GetHeight(self) 
    end,
    GetPaddedWidth  = function(self) 
        return repentogon.L_Image_GetPaddedWidth(self) 
    end,
    GetPaddedHeight = function(self) 
        return repentogon.L_Image_GetPaddedHeight(self) 
    end,
    GetName = function(self)
        local p = repentogon.L_Image_GetName(self)
        if p == nil then return nil end
        return ffi.string(p)
    end,

    GetTexelRegion = function(self, x, y, w, h)
        ffichecks.checkinteger(1, x)
        ffichecks.checkinteger(2, y)
        ffichecks.checkinteger(3, w)
        ffichecks.checkinteger(4, h)

        local n = w * h * 4
        local buf = ffi.new("uint8_t[?]", n)
        repentogon.L_Image_GetTexelRegion(self, x, y, w, h, buf)
        return ffi.string(buf, n)
    end,

    Render = function(self, sourceQuad, destQuad, color, colorMod)
        ffichecks.checkcdata(1, sourceQuad, "SourceQuad")
        ffichecks.checkcdata(2, destQuad, "DestinationQuad")
        ffichecks.checkcdata(3, color, "KColor")
        ffichecks.checkcdata(4, colorMod, "Color")
        repentogon.L_Image_Render(self, sourceQuad, destQuad, color, colorMod)
    end,

     RenderWithShader = function(self, sourceQuad, destQuad, color, shader, params)
        ffichecks.checkcdata(1, sourceQuad, "SourceQuad")
        ffichecks.checkcdata(2, destQuad, "DestinationQuad")
        ffichecks.checkcdata(3, color, "KColor")
        ffichecks.checkcdata(4, colorMod, "Color")
        ffichecks.checkcdata(5, shader, "Shader")
        if not shader.Initialized then
            error("bad argument #4 to 'RenderWithShader' (invalid shader used)")
        end
        ffichecks.checktable(5, params)

        local vb = repentogon.L_Image_SubmitQuadForShader(self, sourceQuad, destQuad, color, shader)
        if vb == nil then return end

        -- no reserve, since the error path should be a rare and non desirable occurrence.
        local errs = fill_shader_vertices(vb, shader, params)
        if errs then
            -- field not being setup correctly is purely the caller's fault, so even tho we can safely continue it's better to error.
            error("bad argument #5 to 'RenderWithShader' (some fields were not properly set : "
                  .. table.concat(errs, ", ") .. ")", 2)
        end
    end,
}

setmetatable(ImageMT, {
    __index = function() end,
})
ImageMT.__index = ImageMT

local ImageT = ffi.metatype("struct Image", ImageMT)