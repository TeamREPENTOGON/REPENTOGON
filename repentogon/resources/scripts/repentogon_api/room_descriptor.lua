---@type table<string, integer>
local s_groupNames = {}
local s_nextGroupId = 0

---@param groupName string
---@return integer groupId
local function CreateGroup(groupName)
    if type(groupName) ~= "string" then
        error(string.format("bad argument #%d (string expected, got %s)", 1, type(groupName)), 2)
    end

    local exists = s_groupNames[groupName] ~= nil
    if exists then
        error(string.format("group \"%s\" already exists", groupName), 2)
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