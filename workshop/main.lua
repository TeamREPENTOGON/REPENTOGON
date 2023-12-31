local RepentogonTest = RegisterMod("RepentogonTest", 1)

local function displayWarning()
    RepentogonTest:AddCallback(ModCallbacks.MC_POST_RENDER, function()
        Isaac.RenderScaledText("REPENTOGON isn't fully installed! Head to https://repentogon.com/install.html for instructions", 100, 50, 0.5, 0.5, 1, 0, 0, 1)
    end)
end

local function main()
    if not REPENTOGON then
        displayWarning()
    end
end

main()