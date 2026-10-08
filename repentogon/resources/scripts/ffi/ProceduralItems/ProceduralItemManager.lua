ffi.cdef [[
    struct ProceduralItemManager {
        private struct ProceduralItem** ItemsFirst : 0x0;
        private struct ProceduralItem** ItemsLast : 0x4;
    } : 0x18;

    struct ProceduralItemManager* L_ProceduralItemManager_Get();
    int L_ProceduralItemManager_CreateProceduralItem(unsigned int, unsigned int);
]]

local repentogon = ffidll
local ffi = ffi

local function GetProceduralItemCount()
    local manager = repentogon.L_ProceduralItemManager_Get()
    return ffichecks.vectorsize(ffi.getprivate(manager, "ItemsFirst"), ffi.getprivate(manager, "ItemsLast"), ffi.sizeof("void*"))
end

ProceduralItemManager = {
    CreateProceduralItem = function(seed, unk)
        seed = ffichecks.checkinteger(1, seed)
        unk = ffichecks.checkinteger(2, unk)
        return repentogon.L_ProceduralItemManager_CreateProceduralItem(seed, unk)
    end,
    GetProceduralItem = function(index)
        index = ffichecks.checkinteger(1, index)
        if index >= 0 and index < GetProceduralItemCount() then
            return ffi.getprivate(repentogon.L_ProceduralItemManager_Get(), "ItemsFirst")[index]
        end
        return nil
    end,
    GetProceduralItemCount = GetProceduralItemCount,
}
