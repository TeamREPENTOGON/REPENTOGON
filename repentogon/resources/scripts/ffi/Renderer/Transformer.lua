ffi.cdef [[
    struct Transformer {
        private bool Valid : 0x14;
    } : 0x18;
    typedef struct Transformer* TransformerPtr;

    bool L_Transformer_Init(struct Transformer*, struct Image*);
    void L_Transformer_Destroy(struct Transformer*);
    void L_Transformer_Render(struct Transformer*, struct Image*, struct SourceQuad*, struct DestinationQuad*,
        struct KColor*, struct KColor*, struct KColor*, struct KColor*);
    void L_Transformer_Apply(struct Transformer*);
]]

local repentogon = ffidll
local ffi = ffi

local function CheckValid(self)
    if not ffi.getprivate(self, "Valid") then
        error("No operations allowed after a transformer has been applied", 3)
    end
end

local TransformerMT
TransformerMT = {
    __type = "Transformer",
    __gc = function(self)
        repentogon.L_Transformer_Destroy(self)
    end,
    Apply = function(self)
        CheckValid(self)
        repentogon.L_Transformer_Apply(self)
    end,
    IsValid = function(self)
        return ffi.getprivate(self, "Valid")
    end,
    Render = function(self, image, sourceQuad, destQuad, color)
        CheckValid(self)
        ffichecks.checkcdata(1, image, "Image")
        ffichecks.checkcdata(2, sourceQuad, "SourceQuad")
        ffichecks.checkcdata(3, destQuad, "DestinationQuad")
        ffichecks.checkcdata(4, color, "KColor")
        repentogon.L_Transformer_Render(self, image, sourceQuad, destQuad, color, color, color, color)
    end,
    RenderEx = function(self, image, sourceQuad, destQuad, color1, color2, color3, color4)
        CheckValid(self)
        ffichecks.checkcdata(1, image, "Image")
        ffichecks.checkcdata(2, sourceQuad, "SourceQuad")
        ffichecks.checkcdata(3, destQuad, "DestinationQuad")
        ffichecks.checkcdata(4, color1, "KColor")
        ffichecks.checkcdata(5, color2, "KColor", true)
        ffichecks.checkcdata(6, color3, "KColor", true)
        ffichecks.checkcdata(7, color4, "KColor", true)
        repentogon.L_Transformer_Render(self, image, sourceQuad, destQuad, color1, color2 or color1, color3 or color1, color4 or color1)
    end,
}

setmetatable(TransformerMT, { __index = function() end })
TransformerMT.__index = TransformerMT

ffi.metatype("struct Transformer", TransformerMT)

Transformer = setmetatable({}, {
    __class = TransformerMT,
})
