RemoveTest = RegisterMod("TestRemoveCallback", 1)

local itemPool = Game():GetItemPool()

local get1 = 0
local get3 = false

local FunctionToString = {}

local function fullPrint(message)
    print(message)
    Isaac.DebugString(message)
end

function RemoveTest.PrintGetCallbacks()
    fullPrint("Get Callbacks Begin")

    local callbacks = Isaac.GetCallbacks(ModCallbacks.MC_PRE_GET_COLLECTIBLE)
    for i, callback in ipairs(callbacks) do
        fullPrint(i .. ": " .. (FunctionToString[callback.Function] or "unknown"))
    end

    fullPrint("Get Callbacks End")
end

local instance = 0

local function fun4() -- Forward Declaration
end 

local function InstanceBegin()
    instance = instance + 1
    Isaac.DebugString("Instance " .. instance .. " Begin")
end

local function fun1()
    Isaac.DebugString("Executed 1")
    if instance == 3 then
        RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun1)
        RemoveTest.PrintGetCallbacks()
        itemPool:GetCollectible(ItemPoolType.POOL_TREASURE, false, 9514637)
    end
end

local function fun2()
    Isaac.DebugString("Executed 2")
end

local function fun3()
    Isaac.DebugString("Executed 3")
    RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun4)
    RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun3)
    RemoveTest.PrintGetCallbacks()
    itemPool:GetCollectible(ItemPoolType.POOL_TREASURE, false, 9514637)
end

fun4 = function()
    Isaac.DebugString("Executed 4")
end

local first5 = true

local function fun5()
    Isaac.DebugString("Executed 5")
    RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun2)
    RemoveTest.PrintGetCallbacks()
    if first5 then
        first5 = false
        itemPool:GetCollectible(ItemPoolType.POOL_TREASURE, false, 9514637)
    end
end

local function InstanceEnd()
    Isaac.DebugString("Instance " .. instance .. " End")
    instance = instance - 1
end

FunctionToString[InstanceBegin] = "Instance Begin"
FunctionToString[fun1] = "function 1"
FunctionToString[fun2] = "function 2"
FunctionToString[fun3] = "function 3"
FunctionToString[fun4] = "function 4"
FunctionToString[fun5] = "function 5"
FunctionToString[InstanceEnd] = "Instance End"

function RemoveTest.ClearTestCallbacks()
    RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, InstanceBegin)
    RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun1)
    RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun2)
    RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun3)
    RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun4)
    RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun5)
    RemoveTest:RemoveCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, InstanceEnd)
end

function RemoveTest.StartTest()
    RemoveTest.ClearTestCallbacks()

    RemoveTest:AddCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, InstanceBegin)
    RemoveTest:AddCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun1)
    RemoveTest:AddCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun2)
    RemoveTest:AddCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun3)
    RemoveTest:AddCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun4)
    first5 = true
    RemoveTest:AddCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, fun5)
    RemoveTest:AddCallback(ModCallbacks.MC_PRE_GET_COLLECTIBLE, InstanceEnd)

    itemPool:GetCollectible(ItemPoolType.POOL_TREASURE, false, 9514637)
end