local ChangeLog={
["AssetsLoaded"]=false,
["NoteOffset"]=Vector(275,190),
["PaperOffset"]=Vector(-37,-15),
["VersionOffset"]=Vector(100, 0),
["ChangelogSprite"]=Sprite(),
["NoteSprite"]=Sprite(),

["Font"]=Font(),
["VersionFont"]=Font(),
["FontColor"]=KColor(0.20,0.15,0.1,1),
["LineHeight"]=12,

["ScrollMargin"]=5,
["ScrollSpeed"]=1,
["ScrollItertia"]=0.8,

["CurrentSheet"]=1,

["Sheets"]=   --TODO: implement the ability to have multiple sheets with unique sprites and stuff
    {
    [1]={
        ["Text"]=require("repentogon_extras/changelog_official_text"),
        ["TextArray"]={},
        },
    },
}

local Cl=ChangeLog

local function TextSplit (inputstr, sep)    --shamelessly stolen from the stackoverflow
        if sep == nil then
                sep = "%s"
        end
        local t={}
        for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
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
    for i=1,#Cl.Sheets do
        local temparray=TextSplit(Cl.Sheets[i].Text,"\n")
        for k,v in pairs(temparray) do
            temparray[k]=string.gsub(v,"/newline/"," ")        --new
            temparray[k]=string.gsub(temparray[k],"/fade/","/f/")
                                                                            --line support
            temparray[k]=string.gsub(temparray[k],"https://%S+","(...)")  --replace links with three dots
        end

        local MaxWidth=200
        local finalarray={}
        for k,v in pairs(temparray) do
            local splitbywords=string.gmatch(v,"%w+")
            local wordcount=0
            for word in splitbywords do
                wordcount=wordcount+1
            end
            Isaac.DebugString(tostring(wordcount))
            local limitedarray={}
            if wordcount>0 then
                    limitedarray=SplitStringByLength(v,29)
                for _,str in pairs(limitedarray) do
                    table.insert(finalarray,str)
                end
            else table.insert(finalarray,v)
            end
        end

        Cl.Sheets[i].TextArray=finalarray
    end
end

function ChangeLog.LoadAssets()
    if #Cl.ChangelogSprite:GetDefaultAnimation()<=0 then
        Cl.ChangelogSprite:Load("../repentogon/gfx/ui/changelog.anm2",true)
        Cl.ChangelogSprite:Play("SwapOut")
        Cl.ChangelogSprite.PlaybackSpeed=0.5        --correct speed for doing Update() inside of xxx_RENDER callback

        Cl.NoteSprite:Load("../repentogon/gfx/ui/changelog_tab_paper.anm2",true)
        Cl.NoteSprite:Play("Idle")
    end
    if not Cl.Font:IsLoaded() then
        Cl.Font:Load("font/teammeatfont10.fnt")
    end

    if not Cl.VersionFont:IsLoaded() then
        Cl.VersionFont:Load("font/luamini.fnt")
    end

    if Cl.Font:IsLoaded() and Cl.VersionFont:IsLoaded() and #(Cl.ChangelogSprite:GetDefaultAnimation())>0 then
        Cl.AssetsLoaded=true
        Cl.EvaluateText()
        Isaac.RemoveCallback(REPENTOGON,ModCallbacks.MC_MAIN_MENU_RENDER,ChangeLog.LoadAssets)
    end
end

Isaac.AddCallback(REPENTOGON,ModCallbacks.MC_MAIN_MENU_RENDER,ChangeLog.LoadAssets)

local ShouldBeRendered=false                --render of the changelog isn't attempted
local CurrentState=false                    --changelog paper should be hidden
local ScrollVelocity=0
local ScrollOffset=Cl.ScrollMargin

local MaxPollCIdx=3

local UseFade=false

local function IsActionTriggeredAll(action)
    for i=0,MaxPollCIdx do
        if Input.IsActionTriggered(action,i) then
            return true
        end
    end
    return false
end

local function IsActionPressedAll(action)
    for i=0,MaxPollCIdx do
        if Input.IsActionPressed(action,i) then
            return true
        end
    end
    return false
end

local function IsGivenMenuEntry(id)              --takes MainMenu entry, returns bool, should really have a proper function instead of this smh
    local menuiddetect=(Isaac.WorldToMenuPosition(id,Vector(240,136))-Vector(Isaac.GetScreenWidth(),Isaac.GetScreenHeight())/2):LengthSquared()
    return menuiddetect<1800
end

function ChangeLog.MenuRender()
    if not Cl.AssetsLoaded then
        return
    end
    Cl.NoteSprite:Render(Isaac.WorldToMenuPosition(MainMenu.TITLE, Cl.NoteOffset))
    local versionPosition = Isaac.WorldToMenuPosition(MainMenu.TITLE, Cl.VersionOffset)
	Cl.VersionFont:DrawStringUTF8("REPENTOGON dev build", versionPosition.X + 1, versionPosition.Y + 1, KColor(0, 0, 0, 0.3), 200, true)
    Cl.VersionFont:DrawStringUTF8("REPENTOGON dev build", versionPosition.X, versionPosition.Y, KColor(67 / 255, 5 / 255, 5 / 255, 1), 200, true)
    if IsGivenMenuEntry(MainMenu.TITLE) then
        if IsActionTriggeredAll(ButtonAction.ACTION_MAP) then
            Cl.CurrentState=not Cl.CurrentState

            if Cl.CurrentState==true then
                SFXManager():Play(SoundEffect.SOUND_PAPER_IN)
                Cl.ChangelogSprite:Play("SwapIn")
            else
                SFXManager():Play(SoundEffect.SOUND_PAPER_OUT)
                Cl.ChangelogSprite:Play("SwapOut")
            end
            ShouldBeRendered=true
        end
        if ShouldBeRendered then
            local LogRenderPosition=Isaac.WorldToMenuPosition(MainMenu.TITLE,Cl.PaperOffset)
            Cl.ChangelogSprite:RenderLayer(3,LogRenderPosition)
            local TextGuideNull=Cl.ChangelogSprite:GetNullFrame("UpdateText")
            if TextGuideNull then
                local NullPos=TextGuideNull:GetPos()
                local NullScale=TextGuideNull:GetScale()
                local YOffset=0
                local BoxSize=200                                                                   --treat text as if it was drawn inside of a box
                local MinOffset=LogRenderPosition.Y + NullPos.Y + 40*NullScale.Y                    --40 is a static offset for the "Change Log" title
                local MaxOffset=MinOffset + 170*NullScale.Y                                         --170 is a total height of the paper, should get it exposed in the table
                for _,iterline in pairs(Cl.Sheets[1].TextArray) do
                    if string.find(iterline,"/f/") then
                        UseFade=true
                    elseif iterline==" " then
                        UseFade=false
                    end
                    local line=string.gsub(iterline,"/f/","")
                    local XOffset = (-BoxSize/2)*NullScale.X
                    local FullY = LogRenderPosition.Y + NullPos.Y + 40*NullScale.Y + YOffset*NullScale.Y + ScrollOffset
                    if FullY < (MinOffset + Cl.ScrollMargin*NullScale.Y) or FullY > (MaxOffset - Cl.ScrollMargin*NullScale.Y) then
                        if FullY-MinOffset < Cl.ScrollMargin*NullScale.Y then
                            Cl.FontColor.Alpha=(FullY-MinOffset)/Cl.ScrollMargin
                        end
                        if (MaxOffset-FullY) < Cl.ScrollMargin*NullScale.Y then
                            Cl.FontColor.Alpha=(MaxOffset-FullY)/Cl.ScrollMargin
                        end
                    else
                        Cl.FontColor.Alpha=1
                    end
                    if UseFade then
                        Cl.FontColor.Alpha=Cl.FontColor.Alpha*0.75
                    end
                    Cl.Font:DrawStringScaledUTF8(line,LogRenderPosition.X + NullPos.X + XOffset, FullY,NullScale.X,NullScale.Y,Cl.FontColor,0,false)
--                    Isaac.RenderText(line,LogRenderPosition.X + NullPos.X + XOffset, FullY,1,1,1,Cl.FontColor.Alpha)
                    YOffset = YOffset + Cl.LineHeight
                end
            end

            if IsActionPressedAll(ButtonAction.ACTION_MENUUP) then
                ScrollVelocity=ScrollVelocity+Cl.ScrollSpeed
            end
            if IsActionPressedAll(ButtonAction.ACTION_MENUDOWN) then
                ScrollVelocity=ScrollVelocity-Cl.ScrollSpeed
            end
            if Cl.ChangelogSprite:IsFinished("SwapOut") then
                ShouldBeRendered=false
                ScrollOffset=5
                ScrollVelocity=0
            end
            if ScrollOffset+ScrollVelocity > Cl.ScrollMargin then
                ScrollOffset   = Cl.ScrollMargin
                ScrollVelocity = 0
            end
            ScrollOffset = ScrollOffset + ScrollVelocity
            ScrollVelocity=ScrollVelocity*0.8

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

REPENTOGON.Extras.ChangeLog=ChangeLog

Isaac.AddCallback(REPENTOGON,ModCallbacks.MC_MAIN_MENU_RENDER,Cl.MenuRender)
