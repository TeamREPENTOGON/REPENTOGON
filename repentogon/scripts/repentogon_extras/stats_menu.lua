local StatsMenuRep = {
    StatPageAngleOffsetX = -2,
    StatPageNumberOffsetX = 120,
    StatSheetSprite = Sprite(),
    StatSheetSpritePos = Vector(-39, -15),
    ArrowSprite = Sprite(),
    LeftArrowSpritePos = Vector(-93, -33),
    RightArrowSpritePos = Vector(-169, -25),
    ScheduleRefresh = true,
    CurrentlyDisplayedCategoryID = 1,
    -- data structure: {category name, {stat name (string or table of lines), EventCounter enum or custom function to calculate stat, current value, value text width/2}}
    Stats = {
        {
            "GAME STATS", {
            { "deaths",    EventCounter.DEATHS },
            { "items", function()
                local gameData = Isaac.GetPersistentGameData()
                local itemconfig = Isaac.GetItemConfig()
                local sum = 0
                local unlocked = 0
                for id = 1, CollectibleType.NUM_COLLECTIBLES - 1 do
                    local configEntry = itemconfig:GetCollectible(id)
                    if configEntry and not configEntry.Hidden then
                        sum = sum + 1
                        if gameData:IsItemInCollection(id) then
                            unlocked = unlocked + 1
                        end
                    end
                end

                return unlocked .. "/" .. sum
            end },
            { "mom kills", EventCounter.MOM_KILLS },
            { "secrets", function()
                local gameData = Isaac.GetPersistentGameData()
                local sum = 0
                local unlocked = 0
                local previousValues = {}
                for _, value in pairs(Achievement) do
                    if not previousValues[value] then
                        sum = sum + 1
                        previousValues[value] = true
                        if gameData:Unlocked(value) then
                            unlocked = unlocked + 1
                        end
                    end
                end

                return unlocked .. "/" .. sum
            end },
            { "best streak",   EventCounter.BEST_STREAK },
            { "",              function() return "" end }, -- spacer
            { "eden tokens",   EventCounter.EDEN_TOKENS },
            { "",              function() return "" end }, -- spacer
            { "dailys played", EventCounter.DAILYS_PLAYED },
            { "dailys won",    EventCounter.DAILYS_WON },
        } },
        {
            "BOSS KILLS", {
            { "mom kills",          EventCounter.MOM_KILLS },
            { "isaac kills",        EventCounter.ISAAC_KILLS },
            { "satan kills",        EventCounter.SATAN_KILLS },
            { "blue baby kills",    EventCounter.BLUE_BABY_KILLS },
            { "lamb kills",         EventCounter.LAMB_KILLS },
            { "hush kills",         EventCounter.HUSH_KILLS },
            { "delirium kills",     EventCounter.DELIRIUM_KILLS },
            { "mega satan kills",   EventCounter.MEGA_SATAN_KILLS },
            { "mother kills",       EventCounter.MOTHER_KILLS },
            { "beast kills",        EventCounter.BEAST_KILLS },
            { "baby plum kills",    EventCounter.BABY_PLUM_KILLS },
            { "boss rushs cleared", EventCounter.BOSSRUSHS_CLEARED },
        } },
        {
            "SHOPPING", {
            { "shopkeeper killed",                   EventCounter.SHOPKEEPER_KILLED },
            { "items bought",                        EventCounter.SHOP_ITEMS_BOUGHT },
            { "angel deals taken",                   EventCounter.ANGEL_DEALS_TAKEN },
            { "devil deals taken",                   EventCounter.DEVIL_DEALS_TAKEN },
            { "",                                    function() return "" end }, -- spacer
            { { "donation machine", "coins" },       EventCounter.DONATION_MACHINE_COUNTER },
            { { "greed donation", "machine coins" }, EventCounter.GREED_DONATION_MACHINE_COUNTER },
        } },
        {
            "GAMBLING", {
            { "arcades entered",                    EventCounter.ARCADES_ENTERED },
            { "shellgames played",                  EventCounter.SHELLGAMES_PLAYED },
            { { "blood donation", "machine used" }, EventCounter.BLOOD_DONATION_MACHINE_USED },
            { { "slot machines", "broken" },        EventCounter.SLOT_MACHINES_BROKEN },
            { { "golden chests", "opened" },        EventCounter.CHESTS_OPENED_WITH_KEY },
            { { "secret room", "walls broken" },    EventCounter.SECRET_ROOMS_WALLS_OPENED },
        } },
        {
            "CHAOS", {
            { "rocks destroyed",               EventCounter.ROCKS_DESTROYED },
            { { "tinted rocks", "destroyed" }, EventCounter.TINTED_ROCKS_DESTROYED },
            { { "super special", "rocks destroyed" }, EventCounter.SUPER_SPECIAL_ROCKS_DESTROYED },
            { "poop destroyed",                EventCounter.POOP_DESTROYED },
            { { "rainbow poop", "destroyed" }, EventCounter.RAINBOW_POOP_DESTROYED },
            { "pills eaten",                   EventCounter.PILLS_EATEN },
            { "cards used",                    EventCounter.CARDS_USED },
            { "batteries collected",           EventCounter.BATTERIES_COLLECTED },
            { "battery bums killed",           EventCounter.BATTERY_BUMS_KILLED },
        } },
        {
            "MISC", {
            { "beds used",                              EventCounter.BEDS_USED },
            { { "battery bum", "collectible payouts" }, EventCounter.BATTERY_BUM_COLLECTIBLE_PAYOUTS },
        } },
    }
}

local StatsMenuPos = Vector(35, 34)
local MaxCategoryID = 1

local font = Font()
font:Load("font/teammeatfont10.fnt")
local fontcolor = KColor(0.20, 0.15, 0.1, 1)

local MaxPollCIdx = 3
local function IsActionTriggeredAll(action)
    for i = 0, MaxPollCIdx do
        if Input.IsActionTriggered(action, i) then
            return true
        end
    end
    return false
end

local function RefreshExtraStats()
    StatsMenuRep.ScheduleRefresh = true
end

local function LoadAssets()
    if #StatsMenuRep.StatSheetSprite:GetDefaultAnimation() <= 0 then
        StatsMenuRep.StatSheetSprite:Load("gfx/ui/main menu/statsmenu.anm2", true)
        StatsMenuRep.StatSheetSprite:SetFrame("Idle", 0)

        StatsMenuRep.ArrowSprite:Load("gfx/ui/leaderboardmenu.anm2", true)
        StatsMenuRep.ArrowSprite:SetFrame("Idle", 0)
    end
    if not font:IsLoaded() then
        font:Load("font/teammeatfont10.fnt")
    end
    if font:IsLoaded() and #(StatsMenuRep.StatSheetSprite:GetDefaultAnimation()) > 0 then
        Isaac.RemoveCallback(REPENTOGON, ModCallbacks.MC_MAIN_MENU_RENDER, LoadAssets)
    end
end

Isaac.AddCallback(REPENTOGON, ModCallbacks.MC_MAIN_MENU_RENDER, LoadAssets)


local function RenderExtraStats()
    if StatsMenu:IsAchievementsScreenVisible() then
        StatsMenu:GetStatsMenuSprite():GetLayerByName("Paper1"):SetVisible(true)
        return
    end
    if MenuManager:GetActiveMenu() == MainMenuType.SAVES then StatsMenuRep.ScheduleRefresh = true end
    if MenuManager:GetActiveMenu() == MainMenuType.STATS then
        if StatsMenuRep.ScheduleRefresh then
            local gameData = Isaac.GetPersistentGameData()
            MaxCategoryID = #StatsMenuRep.Stats + 1
            for i, categoryData in ipairs(StatsMenuRep.Stats) do
                for j, stat in ipairs(categoryData[2]) do
                    if type(stat[2]) == "function" then
                        StatsMenuRep.Stats[i][2][j][3] = stat[2]()
                    else
                        StatsMenuRep.Stats[i][2][j][3] = gameData:GetEventCounter(stat[2])
                    end
                    StatsMenuRep.Stats[i][2][j][4] = font:GetStringWidthUTF8(tostring(StatsMenuRep.Stats[i][2][j][3])) / 2
                end
            end
            StatsMenuRep.ScheduleRefresh = false
        end

        if IsActionTriggeredAll(ButtonAction.ACTION_MENULEFT) then
            SFXManager():Play(SoundEffect.SOUND_PAPER_IN)
            StatsMenuRep.CurrentlyDisplayedCategoryID = (StatsMenuRep.CurrentlyDisplayedCategoryID - 1) % MaxCategoryID
            if StatsMenuRep.CurrentlyDisplayedCategoryID == 0 then
                StatsMenuRep.CurrentlyDisplayedCategoryID = MaxCategoryID - 1
            end
        elseif IsActionTriggeredAll(ButtonAction.ACTION_MENURIGHT) then
            SFXManager():Play(SoundEffect.SOUND_PAPER_IN)
            StatsMenuRep.CurrentlyDisplayedCategoryID = math.max(
                (StatsMenuRep.CurrentlyDisplayedCategoryID + 1) % MaxCategoryID, 1)
        end
    end
    StatsMenu:GetStatsMenuSprite():GetLayerByName("Paper1"):SetVisible(false)

    -- render stat page over original stat page, to allow for custom content
    local posSheet = Isaac.WorldToMenuPosition(MainMenuType.STATS, StatsMenuRep.StatSheetSpritePos)
    StatsMenuRep.StatSheetSprite:RenderLayer(1, posSheet)
    local posLeftArrow = Isaac.WorldToMenuPosition(MainMenuType.STATS, StatsMenuRep.LeftArrowSpritePos)
    StatsMenuRep.ArrowSprite:RenderLayer(6, posLeftArrow)
    local posRightArrow = Isaac.WorldToMenuPosition(MainMenuType.STATS, StatsMenuRep.RightArrowSpritePos)
    StatsMenuRep.ArrowSprite:RenderLayer(7, posRightArrow)

    local nextLineOffsetX = 0
    local nextLineOffsetY = 0

    local pos = Isaac.WorldToMenuPosition(MainMenuType.STATS, StatsMenuPos)

    local categoryData = StatsMenuRep.Stats[StatsMenuRep.CurrentlyDisplayedCategoryID]
    font:DrawStringUTF8(categoryData[1], pos.X, pos.Y + nextLineOffsetY, fontcolor, 0, false)
    nextLineOffsetY = nextLineOffsetY + 12
    for _, stat in ipairs(categoryData[2]) do
        -- text
        local statValue = tostring(stat[3] or 0)
        local statValueWidth = stat[4] or 0
        if type(stat[1]) == "table" then
            nextLineOffsetY = nextLineOffsetY + 2 -- some padding aboth
            for i, text in ipairs(stat[1]) do
                font:DrawStringUTF8(text, pos.X + nextLineOffsetX, pos.Y + nextLineOffsetY, fontcolor, 0, false)
                if i ~= #stat[1] then
                    nextLineOffsetY = nextLineOffsetY + 12
                    nextLineOffsetX = nextLineOffsetX + StatsMenuRep.StatPageAngleOffsetX
                end
            end
            -- value
            font:DrawStringUTF8(statValue, pos.X + nextLineOffsetX + StatsMenuRep.StatPageNumberOffsetX - statValueWidth,
                pos.Y + nextLineOffsetY - 5, fontcolor, 0, false)

            nextLineOffsetY = nextLineOffsetY + 2 -- some padding below
        else
            font:DrawStringUTF8(stat[1], pos.X + nextLineOffsetX, pos.Y + nextLineOffsetY, fontcolor, 0, false)
            -- value
            font:DrawStringUTF8(statValue, pos.X + nextLineOffsetX + StatsMenuRep.StatPageNumberOffsetX - statValueWidth,
                pos.Y + nextLineOffsetY + 2, fontcolor, 0, false)
        end

        nextLineOffsetY = nextLineOffsetY + 12
        nextLineOffsetX = nextLineOffsetX + StatsMenuRep.StatPageAngleOffsetX
    end
end

REPENTOGON.Extras.StatsMenu = StatsMenuRep

Isaac.AddCallback(REPENTOGON, ModCallbacks.MC_PRE_GAME_EXIT, RefreshExtraStats)
Isaac.AddCallback(REPENTOGON, ModCallbacks.MC_MAIN_MENU_RENDER, RenderExtraStats)
