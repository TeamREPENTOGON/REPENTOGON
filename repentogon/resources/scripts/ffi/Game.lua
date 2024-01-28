ffi.cdef[[
void L_Game_ClearDonationModGreed();
]]
local repentogon = ffidll

Game = {}

function Game.ClearDonationModGreed() 
    repentogon.L_Game_ClearDonationModGreed()
end