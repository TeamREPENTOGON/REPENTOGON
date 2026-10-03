ffi.cdef [[
    struct ChallengeParam* L_DailyChallenge_GetChallengeParams();
]]

local repentogon = ffidll

DailyChallenge = {
    GetChallengeParams = function()
        return repentogon.L_DailyChallenge_GetChallengeParams()
    end,
}