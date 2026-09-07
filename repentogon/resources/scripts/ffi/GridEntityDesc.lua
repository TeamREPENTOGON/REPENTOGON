ffi.cdef[[

    struct GridEntityDesc
    {
        int Type;
        int Variant;
        int State;
        int SpawnCount;
        unsigned int SpawnSeed;
        int VarData;
        bool Initialized;
        unsigned int VariableSeed;
    };  
    typedef struct GridEntityDesc* GridEntityDescPtr;
]]

local GridEntityDescMT
GridEntityDescMT = {
    __type = "GridEntityDesc",
}

local GridEntityDescT = ffi.metatype("struct GridEntityDesc", GridEntityDescMT)

setmetatable(GridEntityDescMT, { __index = function() end })
GridEntityDescMT.__index = GridEntityDescMT

GridEntityDesc = setmetatable({}, {__class = GridEntityDescMT})