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

local repentogon = ffidll
local lffi = ffi

local GridEntityDescMT
GridEntityDescMT = {
    __type = "GridEntityDesc",
}

GridEntityDescMT.__index = function(self, key)
	return GridEntityDescMT[key]
end 

local GridEntityDescT = ffi.metatype("struct GridEntityDesc", GridEntityDescMT)

GridEntityDesc = setmetatable({}, {__class = GridEntityDescMT})