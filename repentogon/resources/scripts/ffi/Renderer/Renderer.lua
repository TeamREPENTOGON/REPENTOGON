ffi.cdef [[
    bool L_Renderer_LoadImage(const char*, struct Image*);
    bool L_Renderer_CreateImage(uint32_t, uint32_t, const char*, struct Image*);
    bool L_Renderer_IsProceduralImage(struct Image*);
    void L_Renderer_BeginRenderToImage(struct Image*, struct BlendMode*);
    void L_Renderer_EndRenderToImage(struct BlendMode*);
    struct Shader* L_Renderer_GetShaderByType(int);
    struct Shader* L_Renderer_LoadShader(const char*, const char**, const int*, int, const char**);
    float L_Renderer_GetPixelationAmount();
    void L_Renderer_GetClipPaneNormal(struct Vector*);
    float L_Renderer_GetClipPaneThreshold();
]]

local repentogon = ffidll
local ffi = ffi

local ImageT = ffi.typeof("struct Image")
local BlendModeT = ffi.typeof("struct BlendMode")
local TransformerT = ffi.typeof("struct Transformer")
local SurfaceRenderControllerT = ffi.typeof("struct SurfaceRenderController")

local SHADER_MAX = 20

local function CheckProcedural(image)
    if not repentogon.L_Renderer_IsProceduralImage(image) then
        error("Cannot use a non procedural image as the render target", 3)
    end
end

local function RenderErrorHandler(err)
    if type(err) ~= "string" and type(err) ~= "number" then
        return nil
    end
    local traceback = debug.traceback(tostring(err), 2)
    local cut = traceback:find("\n%s*%[C%]: in function 'xpcall'")
    if cut then
        traceback = traceback:sub(1, cut - 1)
    end
    return traceback
end

Renderer = {
    CreateImage = function(width, height, name)
        width = ffichecks.checkinteger(1, width)
        height = ffichecks.checkinteger(2, height)
        name = ffichecks.checkstring(3, name)
        local image = ImageT()
        if not repentogon.L_Renderer_CreateImage(width, height, name, image) then
            error("Unable to create Image", 2)
        end
        return image
    end,
    GetClipPaneNormal = function()
        local normal = Vector(0, 0)
        repentogon.L_Renderer_GetClipPaneNormal(normal)
        return normal
    end,
    GetClipPaneThreshold = function()
        local result = repentogon.L_Renderer_GetClipPaneThreshold() return result
    end,
    GetPixelationAmount = function()
        local result = repentogon.L_Renderer_GetPixelationAmount() return result
    end,
    GetShaderByType = function(shaderType)
        shaderType = ffichecks.checkinteger(1, shaderType)
        if shaderType < 0 or shaderType >= SHADER_MAX then
            ffichecks.argerror(1, "invalid shader type")
        end
        local result = repentogon.L_Renderer_GetShaderByType(shaderType) return result
    end,
    LoadImage = function(path)
        path = ffichecks.checkstring(1, path)
        local image = ImageT()
        if not repentogon.L_Renderer_LoadImage(path, image) then
            ffichecks.argerror(1, string.format("Image %s does not exist", path))
        end
        return image
    end,
    LoadShader = function(path, descriptor)
        path = ffichecks.checkstring(1, path)
        ffichecks.checktable(2, descriptor)

        local count = #descriptor
        local names = {}
        local formats = ffi.new("int[?]", count + 1)
        for i = 1, count do
            local attribute = rawget(descriptor, i)
            if type(attribute) ~= "table" then
                error(string.format("Invalid vertex attribute %d (table expected)", i), 2)
            end
            local name = rawget(attribute, 1)
            if type(name) ~= "string" and type(name) ~= "number" then
                error(string.format("Invalid name for vertex attribute %d (string expected)", i), 2)
            end
            name = tostring(name)
            local format = rawget(attribute, 2)
            if math.type(format) ~= "integer" then
                error(string.format("Invalid format for vertex attribute \"%s\" (integer expected)", name), 2)
            end
            if format < 1 or format > 8 then
                error(string.format("Invalid format for vertex attribute \"%s\"", name), 2)
            end
            names[i] = name
            formats[i - 1] = format
        end

        local err = ffi.new("const char*[1]")
        local shader = repentogon.L_Renderer_LoadShader(path, ffi.new("const char*[?]", count + 1, names), formats, count, err)
        if shader == nil then
            error(string.format("Unable to load shader \"%s\": %s", path, ffi.string(err[0])), 2)
        end
        return shader
    end,
    RenderToImage = function(image, renderFunction)
        ffichecks.checkcdata(1, image, "Image")
        CheckProcedural(image)
        ffichecks.checkfunction(2, renderFunction)

        local controller = SurfaceRenderControllerT()
        ffi.setprivate(controller, "Valid", true)
        local previousBlendMode = BlendModeT()

        repentogon.L_Renderer_BeginRenderToImage(image, previousBlendMode)
        local ok, err = xpcall(renderFunction, RenderErrorHandler, controller)
        ffi.setprivate(controller, "Valid", false)
        repentogon.L_Renderer_EndRenderToImage(previousBlendMode)

        if not ok then
            if err ~= nil then
                error(string.format("An error occurred while Rendering to Surface \"%s\": (%s)", image:GetName() or "", err), 2)
            end
            error(string.format("An error occurred while Rendering to Surface \"%s\"", image:GetName() or ""), 2)
        end
    end,
    StartTransformation = function(image)
        ffichecks.checkcdata(1, image, "Image")
        local transformer = TransformerT()
        if not repentogon.L_Transformer_Init(transformer, image) then
            error("Cannot use a non procedural image as the render target", 2)
        end
        return transformer
    end,

    GLSLType = {
        Float = 0,
        Vec2 = 1,
        Vec3 = 2,
        Vec4 = 3,
    },
    ShaderType = {
        SHADER_COLOR_OFFSET = 0,
        SHADER_PIXELATION = 1,
        SHADER_BLOOM = 2,
        SHADER_COLOR_CORRECTION = 3,
        SHADER_HQ4X = 4,
        SHADER_SHOCKWAVE = 5,
        SHADER_OLDTV = 6,
        SHADER_WATER = 7,
        SHADER_HALLUCINATION = 8,
        SHADER_COLOR_MOD = 9,
        SHADER_COLOR_OFFSET_CHAMPION = 10,
        SHADER_WATER_V2 = 11,
        SHADER_BACKGROUND = 12,
        SHADER_WATER_OVERLAY = 13,
        SHADER_UNK = 14,
        SHADER_COLOR_OFFSET_DOGMA = 15,
        SHADER_COLOR_OFFSET_GOLD = 16,
        SHADER_DIZZY = 17,
        SHADER_HEAT_WAVE = 18,
        SHADER_MIRROR = 19,
    },
    VertexAttributeFormat = {
        FLOAT = 1,
        VEC2 = 2,
        VEC3 = 3,
        VEC4 = 4,
        POSITION = 5,
        COLOR = 6,
        TEX_COORD = 7,
    },
}
