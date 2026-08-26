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

GridEntityDescMT.__index = function(self, key)
	return GridEntityDescMT[key]
end 

GridEntityDesc = setmetatable({}, {__class = GridEntityDescMT})