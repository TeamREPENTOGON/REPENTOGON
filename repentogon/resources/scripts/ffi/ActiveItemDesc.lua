ffi.cdef[[
struct ActiveItemDesc {
    int Item;
    padding char[0x4]; // Unknown field added in Repentance+
    int Charge;
    int BatteryCharge;
    int SubCharge;
    int TimedRechargeCooldown;
    int PartialCharge;
    int VarData;
};

typedef struct ActiveItemDesc* ActiveItemDescPtr;
]]

local ActiveItemDescMT
ActiveItemDescMT = {
    __type = "ActiveItemDesc",
}

ActiveItemDescMT.__index = ActiveItemDescMT

local ActiveItemDescT = ffi.metatype("struct ActiveItemDesc", ActiveItemDescMT)
