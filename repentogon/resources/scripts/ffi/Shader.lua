ffi.cdef [[
    struct Shader {
        padding char[0x4];
        const bool Initialized;
        padding char[0x27];
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
