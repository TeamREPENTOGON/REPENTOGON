ffi.cdef [[
    struct Shape* L_DebugRenderer_Get(int, bool);
]]

local repentogon = ffidll

DebugRenderer = {
    Get = function(index, unk)
        index = ffichecks.optinteger(index, -1)
        unk = ffichecks.optboolean(unk, false)
        return repentogon.L_DebugRenderer_Get(index, unk)
    end,
}