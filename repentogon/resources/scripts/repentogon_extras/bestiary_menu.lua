-- save copy of original enums, to make it harder for mods to mess with the repentogon features
local _ModCallbacks = ModCallbacks
local _MainMenuType = MainMenuType
local _ButtonAction = ButtonAction
local _SoundEffect = SoundEffect 

local BestiaryMenuRep = {
    BestiarySheetSprite = Sprite(),
    BestiarySheetSpritePos = Vector(5, 28),
    ArrowSprite = Sprite(),
    PageWidgetPos = Vector(84, 238),
    PageTextPos = Vector(84, 231)
}

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
local function LoadAssets()
    if #BestiaryMenuRep.BestiarySheetSprite:GetDefaultAnimation() <= 0 then
        BestiaryMenuRep.BestiarySheetSprite:Load(REPENTOGON.RESOURCEPATH .. "/gfx/ui/bestiary_tabs.anm2", true)
        BestiaryMenuRep.BestiarySheetSprite:SetFrame("Tabs", 0)
        
        BestiaryMenuRep.ArrowSprite:Load("gfx/ui/leaderboardmenu.anm2", true)
        BestiaryMenuRep.ArrowSprite:SetFrame("Idle", 0)
    end
    if not font:IsLoaded() then
        font:Load("font/teammeatfont10.fnt")
    end
    if font:IsLoaded() and #(BestiaryMenuRep.BestiarySheetSprite:GetDefaultAnimation()) > 0 then
        Isaac.RemoveCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, LoadAssets)
    end
end

Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, LoadAssets)

local function RenderBestiaryMenu()
    local selectedPage = BestiaryMenu:GetSelectedPage()
    local lastEnemyPage = BestiaryMenu:GetNumMonsterPages()
    local totalPages = BestiaryMenu:GetNumPages()
    local isEnemyPage = selectedPage <= lastEnemyPage
    if MenuManager:GetActiveMenu() == _MainMenuType.BESTIARY then
        if IsActionTriggeredAll(_ButtonAction.ACTION_MENUTAB) then
            SFXManager():Play(_SoundEffect.SOUND_PAPER_IN)
			if isEnemyPage then
				BestiaryMenu:SetSelectedPage(lastEnemyPage+1)
			else
				BestiaryMenu:SetSelectedPage(0)
			end
            BestiaryMenu:SetSelectedElement(0)
        elseif IsActionTriggeredAll(_ButtonAction.ACTION_BOMB) then
            if selectedPage < BestiaryMenu:GetNumPages() then -- last page reached
                SFXManager():Play(_SoundEffect.SOUND_PAPER_IN)
                BestiaryMenu:SetSelectedPage(selectedPage + 1)
                BestiaryMenu:SetSelectedElement(0)
            end
        elseif IsActionTriggeredAll(_ButtonAction.ACTION_PILLCARD) then
            if selectedPage > 0 then -- first page
                SFXManager():Play(_SoundEffect.SOUND_PAPER_IN)
                BestiaryMenu:SetSelectedPage(selectedPage - 1)
                BestiaryMenu:SetSelectedElement(0)
            end
        end
    end

    -- render stat page over original stat page, to allow for custom content
    BestiaryMenuRep.BestiarySheetSprite:SetFrame("Tabs", isEnemyPage and 0 or 1)
    local posSheet = Isaac.WorldToMenuPosition(_MainMenuType.BESTIARY, BestiaryMenuRep.BestiarySheetSpritePos)
    BestiaryMenuRep.BestiarySheetSprite:Render(posSheet)

    BestiaryMenuRep.BestiarySheetSprite:SetFrame("PageWidget", 0)
    local posPageSheet = Isaac.WorldToMenuPosition(_MainMenuType.BESTIARY, BestiaryMenuRep.PageWidgetPos)
    BestiaryMenuRep.BestiarySheetSprite:Render(posPageSheet)

    local pos = Isaac.WorldToMenuPosition(_MainMenuType.BESTIARY, BestiaryMenuRep.PageTextPos)
    local text = "Page: "..tostring(selectedPage + 1).."/"..tostring(totalPages + 1)
    local width = font:GetStringWidthUTF8(text)
    font:DrawStringUTF8(text, pos.X-width/2, pos.Y, fontcolor, 0, false)
end

REPENTOGON.Extras.BestiaryMenu = BestiaryMenuRep

Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, RenderBestiaryMenu)
