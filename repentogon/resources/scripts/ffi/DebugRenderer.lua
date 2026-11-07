ffi.cdef [[
    struct Shape* L_DebugRenderer_Get(int, bool);
]]

local repentogon = ffidll

DebugRenderer = {
    Get = function(index, unk)
        index = ffichecks.optnumber(index, -1)
        unk = ffichecks.optboolean(unk, false)
        local result = repentogon.L_DebugRenderer_Get(index, unk) return result
    end,
}