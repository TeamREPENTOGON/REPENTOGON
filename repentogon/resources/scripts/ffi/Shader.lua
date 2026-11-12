ffi.cdef [[
    struct Shader {
        const bool Initialized : 0x4;
        private const struct VertexDescriptor* VertexAttributes : 0x8;
        private unsigned int NumVertexAttributes : 0xc;
    } : 0x28;

    struct VertexDescriptor {
        const char* Name : 0x0;
        unsigned int Format : 0x4;
    };

    typedef struct Shader* ShaderPtr;
]]

local ShaderMT
ShaderMT = {
    __type = "Shader",
}

setmetatable(ShaderMT, { __index = function() end })
ShaderMT.__index = ShaderMT

local ShaderT = ffi.metatype("struct Shader", ShaderMT)

Shader = setmetatable({}, {
    __class = ShaderMT,
})
