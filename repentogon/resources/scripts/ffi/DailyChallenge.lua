ffi.cdef [[
    struct ChallengeParam* L_DailyChallenge_GetChallengeParams();
]]

local repentogon = ffidll

DailyChallenge = {
    GetChallengeParams = function()
        local result = repentogon.L_DailyChallenge_GetChallengeParams() return result
    end,
}