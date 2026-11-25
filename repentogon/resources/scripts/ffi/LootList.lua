ffi.cdef [[
    struct LootList {
        private uint32_t Storage[5];
    } : 0x14;
    typedef struct LootList* LootListPtr;

    void L_LootList_Init(struct LootList*);
    void L_LootList_Destroy(struct LootList*);
    unsigned int L_LootList_GetSize(struct LootList*);
    struct LootListEntry* L_LootList_GetEntry(struct LootList*, unsigned int);
    void L_LootList_PushEntry(struct LootList*, uint32_t, uint32_t, uint32_t, uint32_t, struct RNG*);
]]

local repentogon = ffidll
local ffi = ffi

local LootListMT
LootListMT = {
    __type = "LootList",
    __gc = function(self)
        repentogon.L_LootList_Destroy(self)
    end,
    GetEntries = function(self)
        local entries = {}
        for i = 0, repentogon.L_LootList_GetSize(self) - 1 do
            entries[i + 1] = repentogon.L_LootList_GetEntry(self, i)
        end
        return entries
    end,
    PushEntry = function(self, entType, variant, subType, seed, rng)
        entType = ffichecks.checkinteger(1, entType)
        variant = ffichecks.checkinteger(2, variant)
        subType = ffichecks.checkinteger(3, subType)
        seed = ffichecks.optnumber(seed, Random())
        ffichecks.checkcdata(5, rng, "RNG", true)
        repentogon.L_LootList_PushEntry(self, entType, variant, subType, seed, rng)
    end,
}

setmetatable(LootListMT, { __index = function() end })
LootListMT.__index = LootListMT

local LootListT = ffi.metatype("struct LootList", LootListMT)

LootList = setmetatable({}, {
    __call = function(_)
        local list = LootListT()
        repentogon.L_LootList_Init(list)
        return list
    end,
    __class = LootListMT,
})
