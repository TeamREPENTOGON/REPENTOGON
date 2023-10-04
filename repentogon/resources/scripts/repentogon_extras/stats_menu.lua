-- save copy of original enums, to make it harder for mods to mess with the repentogon features
local _Achievement = Achievement
local _EventCounter = EventCounter
local _ModCallbacks = ModCallbacks
local _CollectibleType = CollectibleType
local _MainMenuType = MainMenuType
local _ButtonAction = ButtonAction
local _SoundEffect = SoundEffect 

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
            { "deaths", _EventCounter.DEATHS },
            { "items", function()
                local gameData = Isaac.GetPersistentGameData()
                local itemconfig = Isaac.GetItemConfig()
                local sum = 0
                local unlocked = 0
                for id = 1, _CollectibleType.NUM_COLLECTIBLES - 1 do
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
            { "mom kills", _EventCounter.MOM_KILLS },
            { "secrets", function()
                local gameData = Isaac.GetPersistentGameData()
                local sum = 0
                local unlocked = 0
                local previousValues = {}
                for _, value in pairs(_Achievement) do
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
            { "best streak",   _EventCounter.BEST_STREAK },
            { "",              function() return "" end }, -- spacer
            { "eden tokens",   _EventCounter.EDEN_TOKENS },
            { "",              function() return "" end }, -- spacer
            { "dailys played", _EventCounter.DAILYS_PLAYED },
            { "dailys won",    _EventCounter.DAILYS_WON },
        } },
        {
            "BOSS KILLS", {
            { "mom kills",          _EventCounter.MOM_KILLS },
            { "isaac kills",        _EventCounter.ISAAC_KILLS },
            { "satan kills",        _EventCounter.SATAN_KILLS },
            { "blue baby kills",    _EventCounter.BLUE_BABY_KILLS },
            { "lamb kills",         _EventCounter.LAMB_KILLS },
            { "hush kills",         _EventCounter.HUSH_KILLS },
            { "delirium kills",     _EventCounter.DELIRIUM_KILLS },
            { "mega satan kills",   _EventCounter.MEGA_SATAN_KILLS },
            { "mother kills",       _EventCounter.MOTHER_KILLS },
            { "beast kills",        _EventCounter.BEAST_KILLS },
            { "baby plum kills",    _EventCounter.BABY_PLUM_KILLS },
            { "boss rushs cleared", _EventCounter.BOSSRUSHS_CLEARED },
        } },
        {
            "SHOPPING", {
            { "shopkeeper killed",                   _EventCounter.SHOPKEEPER_KILLED },
            { "items bought",                        _EventCounter.SHOP_ITEMS_BOUGHT },
            { "angel deals taken",                   _EventCounter.ANGEL_DEALS_TAKEN },
            { "devil deals taken",                   _EventCounter.DEVIL_DEALS_TAKEN },
            { "",                                    function() return "" end }, -- spacer
            { { "donation machine", "coins" },       _EventCounter.DONATION_MACHINE_COUNTER },
            { { "greed donation", "machine coins" }, _EventCounter.GREED_DONATION_MACHINE_COUNTER },
        } },
        {
            "GAMBLING", {
            { "arcades entered",                    _EventCounter.ARCADES_ENTERED },
            { "shellgames played",                  _EventCounter.SHELLGAMES_PLAYED },
            { { "blood donation", "machine used" }, _EventCounter.BLOOD_DONATION_MACHINE_USED },
            { { "slot machines", "broken" },        _EventCounter.SLOT_MACHINES_BROKEN },
            { { "golden chests", "opened" },        _EventCounter.CHESTS_OPENED_WITH_KEY },
            { { "secret room", "walls broken" },    _EventCounter.SECRET_ROOMS_WALLS_OPENED },
        } },
        {
            "CHAOS", {
            { "rocks destroyed",               _EventCounter.ROCKS_DESTROYED },
            { { "tinted rocks", "destroyed" }, _EventCounter.TINTED_ROCKS_DESTROYED },
            { { "super special", "rocks destroyed" }, _EventCounter.SUPER_SPECIAL_ROCKS_DESTROYED },
            { "poop destroyed",                _EventCounter.POOP_DESTROYED },
            { { "rainbow poop", "destroyed" }, _EventCounter.RAINBOW_POOP_DESTROYED },
            { "pills eaten",                   _EventCounter.PILLS_EATEN },
            { "cards used",                    _EventCounter.CARDS_USED },
            { "batteries collected",           _EventCounter.BATTERIES_COLLECTED },
            { "battery bums killed",           _EventCounter.BATTERY_BUMS_KILLED },
        } },
        {
            "MISC", {
            { "beds used",                              _EventCounter.BEDS_USED },
            { { "battery bum", "collectible payouts" }, _EventCounter.BATTERY_BUM_COLLECTIBLE_PAYOUTS },
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
        Isaac.RemoveCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, LoadAssets)
    end
end

Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, LoadAssets)


local function RenderExtraStats()
    if StatsMenu:IsSecretsMenuVisible() then
        StatsMenu:GetStatsMenuSprite():GetLayer("Paper1"):SetVisible(true)
        return
    end
    if MenuManager:GetActiveMenu() == _MainMenuType.SAVES then StatsMenuRep.ScheduleRefresh = true end
    if MenuManager:GetActiveMenu() == _MainMenuType.STATS then
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

        if IsActionTriggeredAll(_ButtonAction.ACTION_MENULEFT) then
            SFXManager():Play(_SoundEffect.SOUND_PAPER_IN)
            StatsMenuRep.CurrentlyDisplayedCategoryID = (StatsMenuRep.CurrentlyDisplayedCategoryID - 1) % MaxCategoryID
            if StatsMenuRep.CurrentlyDisplayedCategoryID == 0 then
                StatsMenuRep.CurrentlyDisplayedCategoryID = MaxCategoryID - 1
            end
        elseif IsActionTriggeredAll(_ButtonAction.ACTION_MENURIGHT) then
            SFXManager():Play(_SoundEffect.SOUND_PAPER_IN)
            StatsMenuRep.CurrentlyDisplayedCategoryID = math.max(
                (StatsMenuRep.CurrentlyDisplayedCategoryID + 1) % MaxCategoryID, 1)
        end
    end
    StatsMenu:GetStatsMenuSprite():GetLayer("Paper1"):SetVisible(false)

    -- render stat page over original stat page, to allow for custom content
    local posSheet = Isaac.WorldToMenuPosition(_MainMenuType.STATS, StatsMenuRep.StatSheetSpritePos)
    StatsMenuRep.StatSheetSprite:RenderLayer(1, posSheet)
    local posLeftArrow = Isaac.WorldToMenuPosition(_MainMenuType.STATS, StatsMenuRep.LeftArrowSpritePos)
    StatsMenuRep.ArrowSprite:RenderLayer(6, posLeftArrow)
    local posRightArrow = Isaac.WorldToMenuPosition(_MainMenuType.STATS, StatsMenuRep.RightArrowSpritePos)
    StatsMenuRep.ArrowSprite:RenderLayer(7, posRightArrow)

    local nextLineOffsetX = 0
    local nextLineOffsetY = 0

    local pos = Isaac.WorldToMenuPosition(_MainMenuType.STATS, StatsMenuPos)

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

Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_PRE_GAME_EXIT, RefreshExtraStats)
Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, RenderExtraStats)
