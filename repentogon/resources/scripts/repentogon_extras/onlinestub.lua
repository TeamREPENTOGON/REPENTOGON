-- save copy of original enums, to make it harder for mods to mess with the repentogon features
local _ImGuiElement = _ImGuiElement
local _ImGuiCallback = ImGuiCallback
local _ModCallbacks = ModCallbacks
local _RoomTransitionAnim = RoomTransitionAnim
local _Direction = Direction

REPENTOGON.Extras.OnlineRNG=RNG()
local rng=REPENTOGON.Extras.OnlineRNG
local stubvec=Vector(-1,-1)
rng:SetSeed(1)  --placeholder seed
local isonline=false
local checkonline=true
local con=nil
if REPENTOGON.NoOnlineStub then
    return
end

function REPENTOGON.Extras.UnlinkCB() local cnt=0 for _,_ in pairs(_ModCallbacks) do cnt=cnt+1 end for i=0,cnt do local cb=Isaac.GetCallbacks(i,false) print(cb) while #(cb)~=0 do table.remove(cb,1) Isaac.SetBuiltInCallbackState(i, false) end end for _,ent in pairs(Isaac.GetRoomEntities()) do for entry,_ in pairs(ent:GetData()) do ent:GetData()[entry]=nil end end end      --temporary, used to bypass mod unload callbacks since actions done there are just way too nasty for us!


local function CheckOnlineCoop()
    if not Isaac.GetPlayer() then
        print("[onlinestub] player not initialized yet!")
        return false
    end
    for i=0, Game():GetNumPlayers() do
        if (not Isaac.GetPlayer(i):IsLocalPlayer()) then
            return true
        end
    end
    return false
end

-- if Isaac.GetPlayer()==nil or (not CheckOnlineCoop()) then
--     REPENTOGON.Extras.OnlineStubReturned=true
--     return  --preventing the thing from running in scenario when we aren't netstarting
-- end

if Isaac.GetPlayer() and CheckOnlineCoop() then
    isonline=true
    checkonline=false
else
    Isaac.DebugString("[onlinestub] Assuming no netstart for now!")
end

local stubdata={}

REPENTOGON.Extras.UnsafeFuncs={}
REPENTOGON.Extras.UnsafeFuncs.MathRandom=math.random
REPENTOGON.Extras.UnsafeFuncs.Random=Random
REPENTOGON.Extras.UnsafeFuncs.RandomVector=RandomVector

REPENTOGON.Extras.UnsafeFuncs.ActionValue=Input.GetActionValue
REPENTOGON.Extras.UnsafeFuncs.ButtonValue=Input.GetButtonValue
REPENTOGON.Extras.UnsafeFuncs.KeyPress=Input.IsButtonPressed
REPENTOGON.Extras.UnsafeFuncs.KeyTrigger=Input.IsButtonTriggered
REPENTOGON.Extras.UnsafeFuncs.ActionPress=Input.IsActionPressed
REPENTOGON.Extras.UnsafeFuncs.ActionTrigger=Input.IsActionTriggered
REPENTOGON.Extras.UnsafeFuncs.MousePos=Input.GetMousePosition
REPENTOGON.Extras.UnsafeFuncs.MouseInput=Input.IsMouseBtnPressed

REPENTOGON.Extras.UnsafeFuncs.HasData=Isaac.HasModData
REPENTOGON.Extras.UnsafeFuncs.SaveData=Isaac.SaveModData
REPENTOGON.Extras.UnsafeFuncs.LoadData=Isaac.LoadModData
REPENTOGON.Extras.UnsafeFuncs.RemoveData=Isaac.RemoveModData

function Input.GetActionValue(act,cidx)
    if (not isonline) or cidx~=0 then
        return REPENTOGON.Extras.UnsafeFuncs.ActionValue(act,cidx)
    end
    local pl=Isaac.GetPlayer()
    if pl then return REPENTOGON.Extras.UnsafeFuncs.ActionValue(act,pl.ControllerIndex) else return 0.0 end
end

function Input.GetButtonValue(act,cidx)
    if (not isonline) or cidx~=0 then
        return REPENTOGON.Extras.UnsafeFuncs.ButtonValue(act,cidx)
    end
    return 0.0
end

function Input.IsButtonPressed(act,cidx)
    if (not isonline) or cidx~=0 then
        return REPENTOGON.Extras.UnsafeFuncs.KeyPress(act,cidx)
    end
    return false
end

function Input.IsButtonTriggered(act,cidx)
    if (not isonline) or cidx~=0 then
        return REPENTOGON.Extras.UnsafeFuncs.KeyTrigger(act,cidx)
    end
    return false
end

function Input.IsActionPressed(act,cidx)
    if (not isonline) or cidx~=0 then
        return REPENTOGON.Extras.UnsafeFuncs.ActionPress(act,cidx)
    end
    local pl=Isaac.GetPlayer()
    if pl then return REPENTOGON.Extras.UnsafeFuncs.ActionPress(act,pl.ControllerIndex) else return false end
end

function Input.IsActionTriggered(act,cidx)
    if (not isonline) or cidx~=0 then
        return REPENTOGON.Extras.UnsafeFuncs.ActionTrigger(act,cidx)
    end
    local pl=Isaac.GetPlayer()
    if pl then return REPENTOGON.Extras.UnsafeFuncs.ActionTriggerda(act,pl.ControllerIndex) else return false end
end


function Isaac.HasModData(modref)
    if not isonline then
        return REPENTOGON.Extras.UnsafeFuncs.HasData(modref)
    end
    if stubdata[modref] then
        return true
    end
    return false
end

function Isaac.LoadModData(modref)
    if not isonline then
        return REPENTOGON.Extras.UnsafeFuncs.LoadData(modref)
    end
    if stubdata[modref]~=nil then
        return stubdata[modref]
    end
    return ""    --stub
end

function Isaac.SaveModData(modref,data)
    if not isonline then
        return REPENTOGON.Extras.UnsafeFuncs.SaveData(modref,data)
    end
    stubdata[modref]=data
    return false    --stub
end

function Isaac.RemoveModData(modref)
    if not isonline then
        return REPENTOGON.Extras.UnsafeFuncs.RemoveData(modref)
    end --stub
    stubdata[modref]=nil
end

function Input.GetMousePosition(gamecoords)
    if not isonline then
        return REPENTOGON.Extras.UnsafeFuncs.MousePos(gamecoords)
    end
    if gamecoords then
        return Isaac.ScreenToWorld(stubvec)
    end
    return stubvec  --stub
end


function Input.IsMouseBtnPressed(button)
    if not isonline then
        return REPENTOGON.Extras.UnsafeFuncs.MouseInput(button)
    end
    return 0    --stub
end

function math.random(...)
    local args={...}
    local min=args[1]
    local max=args[2]
    if not isonline then
        return REPENTOGON.Extras.UnsafeFuncs.MathRandom(...)
    end
    if min==nil and max==nil then
        return rng:RandomFloat()
    end
    if min~=nil and max==nil then
        return rng:RandomInt(min)+1
    end
    return rng:RandomInt(max-min+1)+min
end

function Random()
    if not isonline then
        return REPENTOGON.Extras.UnsafeFuncs.Random()
    end
    return rng:RandomInt(2^32 - 1)  --might not be 100% accurate but eh, good enough
end

function RandomVector()
    if not isonline then
        return REPENTOGON.Extras.UnsafeFuncs.RandomVector()
    end
    return Vector.FromAngle(math.random()*360)
end



function REPENTOGON.Extras.IsOnlineStub()
    return isonline
end

local function ScheduleLuaReset()
    if Game():GetFrameCount()>30 then
        Isaac.RemoveCallback(REPENTOGON, _ModCallbacks.MC_POST_UPDATE,ScheduleLuaReset)
        print('Please, run the "luareset" debug command :)')
        if not REPENTOGON.Extras.NoImGui then
            local imgui = Isaac.GetImGui()
            imgui:CreateWindow("luareset-warning","A rather important message!")
            imgui:AddElement("luareset-warning","luareset-text",_ImGuiElement.Text,'Hey, folks, can you please have one person run the "luareset" command for me?')
            imgui:AddElement("luareset-warning","luareset-okay",_ImGuiElement.Button,"Okay, I will!")
            imgui:AddCallback("luareset-okay", _ImGuiCallback.Clicked, function() imgui:RemoveWindow("luareset-warning") imgui:Hide() end)
            imgui:SetVisible("luareset-warning",true)
            imgui:Show()
        end
--        Isaac.ExecuteCommand("luareset")
    end
end

local function ScheduleRestart()
    if Game():GetFrameCount()>1 then
        Isaac.ExecuteCommand("seed "..Game():GetSeeds():GetStartSeedString())
        Isaac.RemoveCallback(REPENTOGON, _ModCallbacks.MC_POST_UPDATE,ScheduleRestart)
        Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_POST_UPDATE,ScheduleLuaReset)
    end
end

local function SetOnlineRNG()
    if not checkonline then
        return
    end
    checkonline=false
    stubdata={}
    isonline=CheckOnlineCoop()  --will be set to false on local only runs
    rng:SetSeed(Game():GetSeeds():GetStartSeed(),35)
    if isonline then
        REPENTOGON.Extras.UnlinkCB()
        Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_POST_UPDATE,ScheduleRestart)
    end
end

local function ResetStubData()
    stubdata={}
    checkonline=true
end

function REPENTOGON.Extras.SetOnlineStub(val)
    isonline=val
    checkonline=false
end

local function render()
    if isonline then
        Isaac.RenderScaledText("Online stubber enabled!",5,Isaac.GetScreenHeight()-8,0.5,0.5,1,1,1,0.5)
    end
end

local function TeleportToBeast()
    Isaac.ExecuteCommand("goto x.itemdungeon.666")
    Game():StartRoomTransition(-3, _Direction.NO_DIRECTION, _RoomTransitionAnim.FADE,nil,-1)
    Isaac.RemoveCallback(REPENTOGON, _ModCallbacks.MC_POST_UPDATE,TeleportToBeast)
end

local function DelayDogmaTeleport(_,npc)
    if npc:GetSprite():GetFrame()>=110 then
        Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_POST_UPDATE,TeleportToBeast)
        Isaac.RemoveCallback(REPENTOGON, _ModCallbacks.MC_POST_NPC_RENDER,DelayDogmaTeleport)
    end
end

local function DogmaStub(_,npc)
    if isonline and npc.Variant==2 and Game():GetLevel():GetStage()==13 and Game():GetLevel():GetStageType()>0 and Game():GetLevel():GetCurrentRoomIndex()==109 then
        Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_POST_NPC_RENDER,DelayDogmaTeleport)
    end
end

local function DesyncDetect()
    if isonline and not REPENTOGON.Extras.NoImGui then
        if not con then con=Game():GetConsole() end
        local conhist=con:GetHistory()
        local imgui = Isaac.GetImGui()
        for i=1,5 do
            if string.match(conhist[i],"Checksum mismatch") then
                print("\n\n\n\n\n")
                imgui:CreateWindow("desync-warning","Fuck.")
                imgui:AddElement("desync-warning","desync-text", _ImGuiElement.Text,'Shit just happened, want to try a "rewind"? Maybe a "restart"?')
                imgui:AddElement("desync-warning","desync-rewind", _ImGuiElement.Button,"Please rewind!")
                imgui:AddElement("desync-warning","desync-restart", _ImGuiElement.Button,"I choose a restart")
                imgui:AddCallback("desync-rewind", _ImGuiCallback.Clicked,function() imgui:RemoveWindow("desync-warning") imgui:Hide() Isaac.ExecuteCommand("rewind") end)
                imgui:AddCallback("desync-restart", _ImGuiCallback.Clicked,function() imgui:RemoveWindow("desync-warning") imgui:Hide() Isaac.ExecuteCommand("restart") end)
                imgui:SetVisible("desync-warning",true)
                imgui:Show()
                break
            end
        end
    end
end

function REPENTOGON.Extras.StubCallbacks()
    Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_POST_UPDATE, DesyncDetect)
    Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_POST_ENTITY_KILL, DogmaStub,950)
    Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_POST_RENDER, render)
    Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_PRE_GAME_EXIT, ResetStubData)
    Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_POST_NEW_ROOM, SetOnlineRNG) --because new room triggers before anything else :skull:
end
REPENTOGON.Extras.StubCallbacks()
