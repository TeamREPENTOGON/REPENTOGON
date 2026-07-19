---@type table<string, integer>
local s_groupNames = {}
local s_nextGroupId = 0

local ModManager = _GetModule("repentogon_extras.mod_manager")

---@param groupName string
---@return integer groupId
local function CreateGroup(groupName)
    if type(groupName) ~= "string" then
        error(string.format("bad argument #%d (string expected, got %s)", 1, type(groupName)), 2)
    end

    local existingId = s_groupNames[groupName]
    if existingId then
        if not ModManager.ModsLoaded() then
            Console.PrintWarning(string.format("[WARN] [RGON] group \"%s\" already exists", groupName))
        end

        return existingId
    end

    if ModManager.ModsLoaded() then
        error("groups can only be created during Mod Load", 2)
    end

    local groupId = s_nextGroupId

    s_nextGroupId = s_nextGroupId + 1
    if s_nextGroupId == RoomGroup.GROUP_ULTRA_SECRET then
        s_nextGroupId = s_nextGroupId + 1
    end

    s_groupNames[groupName] = groupId
    return groupId
end

---@param groupName string
---@return integer? groupId
local function GetGroupByName(groupName)
    if type(groupName) ~= "string" then
        error(string.format("bad argument #%d (string expected, got %s)", 1, type(groupName)), 2)
    end

    return s_groupNames[groupName]
end

rawset(RoomDescriptor, "CreateGroup", CreateGroup)
rawset(RoomDescriptor, "GetGroupByName", GetGroupByName)