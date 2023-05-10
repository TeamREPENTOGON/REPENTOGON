local MMC = {
    COLORS = {
        WHITE = { 1, 1, 1 },
        GRAY = { 0.625, 0.625, 0.625 },
        YELLOW = { 1, 1, 0 },
        RED = { 1, 0, 0 },
    },
    LinePadding = 7,
    MaxLines = 40,
    Position = Vector(10, 10),
    TextSize = 0.5,
    IsEnabled = false,
    ToggleKey = Keyboard.KEY_GRAVE_ACCENT,
}

local console = Game():GetConsole()

local function HandleEnterGame()
    MMC.IsEnabled = false
end
Isaac.AddCallback(REPENTOGON, ModCallbacks.MC_POST_GAME_STARTED, HandleEnterGame)

local function HandleConsole()
    if Input.IsButtonTriggered(MMC.ToggleKey, 0) then
        MMC.IsEnabled = not MMC.IsEnabled
    end

    if not MMC.IsEnabled then
        return
    end

    local history = console:GetHistory()
    local histMax = math.min(#history, MMC.MaxLines)
    local isStackTrace = false

    -- eval last X entries in reverse order, to keep track of multi-line color changes
    for i = histMax, 1, -1 do
        local textColor = MMC.COLORS.WHITE
        local text = history[i]

        -- text coloring
        if string.find(text:lower(), "failed") or string.find(text:lower(), "error") then
            textColor = MMC.COLORS.RED
        elseif string.find(text:lower(), "warning") then
            textColor = MMC.COLORS.YELLOW
        elseif string.find(text:lower(), "stack traceback:") then
            textColor = MMC.COLORS.RED
            isStackTrace = true
        elseif isStackTrace then
            if string.sub(text, 1, 1) == " " then
                textColor = MMC.COLORS.RED
            else
                isStackTrace = false
            end
        elseif string.sub(text, 1, 1) == ">" then -- console input
            textColor = MMC.COLORS.GRAY
        end
        -- render text
        Isaac.RenderScaledText(text, MMC.Position.X, MMC.Position.Y + (histMax - i) * MMC.LinePadding, MMC.TextSize,
            MMC.TextSize,
            textColor[1],
            textColor[2], textColor[3], 1)
    end
end

Isaac.AddCallback(REPENTOGON, ModCallbacks.MC_MAIN_MENU_RENDER, HandleConsole)

REPENTOGON.Extras.MainMenuConsole = MMC
