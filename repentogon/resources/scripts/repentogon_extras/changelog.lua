-- save copy of original enums, to make it harder for mods to mess with the repentogon features
local _ModCallbacks = ModCallbacks
local _MainMenuType = MainMenuType
local _ButtonAction = ButtonAction
local _Keyboard = Keyboard
local _SoundEffect = SoundEffect

local ChangeLog = {
    ["AssetsLoaded"] = false,
    ["NoteOffset"] = Vector(275, 190),
    ["PaperOffset"] = Vector(-37, -15),
    ["VersionOffset"] = Vector(238, 240),
    ["ChangelogSprite"] = Sprite(),
    ["NoteSprite"] = Sprite(),

    ["Font"] = Font(),
    ["VersionFont"] = Font(),
    ["FontColor"] = KColor(0.20, 0.15, 0.1, 1),
    ["LineHeight"] = 12,

    ["ScrollMargin"] = 5,
    ["ScrollSpeed"] = 1,
    ["ScrollSpeedFast"] = 2.5,
    ["ScrollItertia"] = 0.8,

    ["CurrentSheet"] = 1,

    ["Sheets"] = --TODO: implement the ability to have multiple sheets with unique sprites and stuff
    {},
}

local Cl = ChangeLog

local function TextSplit(inputstr, sep)  --shamelessly stolen from the stackoverflow
    if sep == nil then
        sep = "%s"
    end
    local t = {}
    for str in string.gmatch(inputstr, "([^" .. sep .. "]+)") do
        table.insert(t, str)
    end
    return t
end

function SplitStringByLength(str, max_length)
    local result = {}
    local line = ""
    for word in string.gmatch(str, "%S+") do
        if #line + #word + 1 > max_length then
            table.insert(result, line)
            line = word
        else
            if line == "" then
                line = word
            else
                line = line .. " " .. word
            end
        end
    end
    if line ~= "" then
        table.insert(result, line)
    end
    return result
end

function ChangeLog.EvaluateText()
    for i = 1, #Cl.Sheets do
        local temparray = TextSplit(Cl.Sheets[i].Text, "\n")
        for k, v in pairs(temparray) do
            temparray[k] = string.gsub(v, "/newline/", " ") --new
            temparray[k] = string.gsub(temparray[k], "/fade/", "/f/")
            --line support
            temparray[k] = string.gsub(temparray[k], "https://%S+", "(...)") --replace links with three dots
        end

        local finalarray = {}
        for _, v in pairs(temparray) do
            local splitbywords = string.gmatch(v, "%w+")
            local wordcount = 0
            for _ in splitbywords do
                wordcount = wordcount + 1
            end
            local limitedarray = {}
            if wordcount > 0 then
                limitedarray = SplitStringByLength(v, 29)
                for _, str in pairs(limitedarray) do
                    table.insert(finalarray, str)
                end
            else
                table.insert(finalarray, v)
            end
        end

        Cl.Sheets[i].TextArray = finalarray
    end
end


local ShouldBeRendered = false --render of the changelog isn't attempted
local ScrollVelocity = 0
local CurrentPage = 1
local ScrollOffset = Cl.ScrollMargin

local MaxPollCIdx = 3

local UseFade = false

local function UpdateMaxPollCIdx()
    local str=Input.GetDeviceNameByIdx(0)
    local i=0
    while (str~=nil) do
        i=i+1
        str=Input.GetDeviceNameByIdx(i)
    end
    MaxPollCIdx=i
end

local function IsActionTriggeredAll(action)
    for i = 0, MaxPollCIdx do
        if Input.IsActionTriggered(action, i) then
            return true
        end
    end
    return false
end

local function IsActionPressedAll(action)
    for i = 0, MaxPollCIdx do
        if Input.IsActionPressed(action, i) then
            return true
        end
    end
    return false
end

local function IsKeyboardPressedAll(input)
    for i = 0, MaxPollCIdx do
        if Input.IsButtonPressed(input, i) then
            return true
        end
    end
    return false
end

function ChangeLog.MenuRender()
    if not Cl.AssetsLoaded then
        return
    end
    Cl.NoteSprite:Render(Isaac.WorldToMenuPosition(_MainMenuType.TITLE, Cl.NoteOffset))
    local versionPosition = Isaac.WorldToMenuPosition(_MainMenuType.TITLE, Cl.VersionOffset)
    local versionString = "REPENTOGON " .. REPENTOGON.Version

    Cl.VersionFont:DrawStringUTF8(versionString, versionPosition.X + 1, versionPosition.Y + 1,
        KColor(0, 0, 0, 0.3), 200) -- shadow
    Cl.VersionFont:DrawStringUTF8(versionString, versionPosition.X, versionPosition.Y,
        KColor(67 / 255, 5 / 255, 5 / 255, 1), 200) -- text
    local ActiveMenu=MenuManager:GetActiveMenu()

    if ActiveMenu == _MainMenuType.TITLE or ActiveMenu==0 then    --reserve menu id 0 for changelog to prevent inputs, mdoders: please do not use that specific id, you already have almost 2^32 of them!...
        UpdateMaxPollCIdx()
        if IsActionTriggeredAll(_ButtonAction.ACTION_MAP) then
            Cl.CurrentState = not Cl.CurrentState

            if Cl.CurrentState == true then
                SFXManager():Play(_SoundEffect.SOUND_PAPER_IN)
                Cl.ChangelogSprite:Play("SwapIn")
                MenuManager.SetActiveMenu(0)    --block inputs
            else
                SFXManager():Play(_SoundEffect.SOUND_PAPER_OUT)
                Cl.ChangelogSprite:Play("SwapOut")
                MenuManager.SetActiveMenu(1)    --unblock inputs
            end
            ShouldBeRendered = true
        elseif IsActionTriggeredAll(_ButtonAction.ACTION_MENUBACK) and Cl.CurrentState==true then
            Cl.CurrentState=false
            SFXManager():Play(_SoundEffect.SOUND_PAPER_OUT)
            Cl.ChangelogSprite:Play("SwapOut")
            MenuManager.SetActiveMenu(1)    --unblock inputs
        elseif IsActionTriggeredAll(_ButtonAction.ACTION_MENURIGHT) and Cl.CurrentState then
            CurrentPage = CurrentPage + 1
            if CurrentPage < #ChangeLog.Sheets then
                SFXManager():Play(_SoundEffect.SOUND_PAPER_IN)
                ScrollOffset = 5
            else
                CurrentPage = #ChangeLog.Sheets
            end
        elseif IsActionTriggeredAll(_ButtonAction.ACTION_MENULEFT) and Cl.CurrentState then
            CurrentPage = CurrentPage - 1
            if CurrentPage > 0 then
                SFXManager():Play(_SoundEffect.SOUND_PAPER_IN)
                ScrollOffset = 5
            else
                CurrentPage = 1
            end
        end
        if ShouldBeRendered then
            local LogRenderPosition = Isaac.WorldToMenuPosition(_MainMenuType.TITLE, Cl.PaperOffset)
            Cl.ChangelogSprite:RenderLayer(3, LogRenderPosition)
            local TextGuideNull = Cl.ChangelogSprite:GetNullFrame("UpdateText")
            local Height = 0
            local BoxSize = 200
            if TextGuideNull then
                local NullPos = TextGuideNull:GetPos()
                local NullScale = TextGuideNull:GetScale()
                local YOffset = 0
                local MinOffset = LogRenderPosition.Y + NullPos.Y + 40 * NullScale.Y  --40 is a static offset for the "Change Log" title
                local MaxOffset = MinOffset + 170 * NullScale.Y                       --170 is a total height of the paper, should get it exposed in the table
                local TextStretchFactor = 0
                for _, iterline in pairs(Cl.Sheets[CurrentPage].TextArray) do
                    if string.find(iterline, "/f/") then
                        UseFade = true
                    elseif iterline == " " then
                        UseFade = false
                    end
                    local line = string.gsub(iterline, "/f/", "")
                    local XOffset = (-BoxSize / 2) * NullScale.X
                    local FullY = LogRenderPosition.Y + NullPos.Y + 40 * NullScale.Y + YOffset * NullScale.Y + ScrollOffset
--                    if FullY<500 then
--                    print(line,"fully-minoffset",FullY - MinOffset,",minoffset:",(MinOffset + Cl.ScrollMargin * NullScale.Y),",maxoffset:",(MaxOffset - Cl.ScrollMargin * NullScale.Y)) end
--                    print("checking",FullY-MinOffset)
                    if ((MaxOffset - FullY) < 0.0) or ((FullY - MinOffset) < 0.0) then
                        goto skiptonextline
                    end
                    if FullY < (MinOffset + Cl.ScrollMargin * NullScale.Y) or FullY > (MaxOffset - Cl.ScrollMargin * NullScale.Y) then
                        if FullY - MinOffset < Cl.ScrollMargin * NullScale.Y then
                            Cl.FontColor.Alpha = (FullY - MinOffset) / Cl.ScrollMargin
                        end
                        if (MaxOffset - FullY) < Cl.ScrollMargin * NullScale.Y then
                            Cl.FontColor.Alpha = (MaxOffset - FullY) / Cl.ScrollMargin
                        end
                    else
                        Cl.FontColor.Alpha = 1
                    end
                    if UseFade then
                        Cl.FontColor.Alpha = Cl.FontColor.Alpha * 0.75
                    end
                    TextStretchFactor=math.min(1.0, BoxSize/Cl.Font:GetStringWidthUTF8(line) )    --this way the long lines of text get squished horizontally
                    Cl.Font:DrawStringScaledUTF8(line, LogRenderPosition.X + NullPos.X + XOffset, FullY, NullScale.X*TextStretchFactor,
                        NullScale.Y, Cl.FontColor, 0, false)
                    ::skiptonextline::
                    YOffset = YOffset + Cl.LineHeight
                    Height = YOffset
                end
            end

            -- Scroll faster when holding SHIFT or DROP
            local speed = (IsActionPressedAll(_ButtonAction.ACTION_DROP) or IsKeyboardPressedAll(_Keyboard.KEY_LEFT_SHIFT))
                and Cl.ScrollSpeedFast or Cl.ScrollSpeed

            if IsActionPressedAll(_ButtonAction.ACTION_MENUUP) then
                ScrollVelocity = ScrollVelocity + speed
            end
            if IsActionPressedAll(_ButtonAction.ACTION_MENUDOWN) and -ScrollOffset < Height - (BoxSize / 2) then
                ScrollVelocity = ScrollVelocity - speed
            end

            if Cl.ChangelogSprite:IsFinished("SwapOut") then
                ShouldBeRendered = false
                ScrollOffset = 5
                ScrollVelocity = 0
            end
            if ScrollOffset + ScrollVelocity > Cl.ScrollMargin then
                ScrollOffset   = Cl.ScrollMargin
                ScrollVelocity = 0
            end
            ScrollOffset = ScrollOffset + ScrollVelocity
            ScrollVelocity = ScrollVelocity * 0.8

            Cl.ChangelogSprite:Update()
        end
    elseif ShouldBeRendered then
        ShouldBeRendered = false
        Cl.CurrentState  = false
        ScrollOffset     = Cl.ScrollMargin
        ScrollVelocity   = 0
        Cl.ChangelogSprite:Play("SwapOut")
    end
end

REPENTOGON.Extras.ChangeLog = ChangeLog

local curattempt=0
function ChangeLog.LoadAssets()
    if #Cl.ChangelogSprite:GetDefaultAnimation() <= 0 then
        Cl.ChangelogSprite:Load("gfx/ui/changelog.anm2", true)
        Cl.ChangelogSprite:Play("SwapOut")
        Cl.ChangelogSprite.PlaybackSpeed = 0.5 --correct speed for doing Update() inside of xxx_RENDER callback

        Cl.NoteSprite:Load("gfx/ui/changelog_tab_paper.anm2", true)
        Cl.NoteSprite:Play("Idle")
        curattempt=curattempt+1
        if curattempt>5 then
		Isaac.RemoveCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, ChangeLog.LoadAssets)
		Isaac.RemoveCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, Cl.MenuRender)
            Isaac.AddCallback(REPENTOGON,_ModCallbacks.MC_MAIN_MENU_RENDER,REPENTOGON.Extras.Misc.NoRPTGNFldrErr)
        end
    end
    if not Cl.Font:IsLoaded() then
        Cl.Font:Load("font/teammeatex/teammeatex10.fnt")
    end

    if not Cl.VersionFont:IsLoaded() then
        Cl.VersionFont:Load("font/luamini.fnt")
    end

    if Cl.Font:IsLoaded() and Cl.VersionFont:IsLoaded() and #(Cl.ChangelogSprite:GetDefaultAnimation()) > 0 then

        -- Separate sheets by version
        -- Also, Lua doesn't have a way to separate strings by words. Only with patterns.
        -- This makes me very sad.
        local rawChangelog = Isaac.RGON_GetChangelog()
        local entireChangelog = TextSplit(rawChangelog, "\n")
        local chunk = ""
        for _, word in ipairs(entireChangelog) do
            if word == "/versionseparator/" then
                table.insert(Cl.Sheets, {
                    ["Text"] = chunk,
                    ["TextArray"] = {},
                })
                chunk = ""
            else
                chunk = chunk .. word .. "\n"
            end
        end

        -- Just a failsafe if the log somehow doesn't have any version separators.
        -- Mainly just for dev environments.
        if #Cl.Sheets == 0 then
            table.insert(Cl.Sheets, {
                ["Text"] = rawChangelog,
                ["TextArray"] = {}
            })
        end

        Cl.EvaluateText()
        Isaac.RemoveCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, ChangeLog.LoadAssets)

        Cl.AssetsLoaded = true
    end
end

Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, ChangeLog.LoadAssets)
Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, Cl.MenuRender)