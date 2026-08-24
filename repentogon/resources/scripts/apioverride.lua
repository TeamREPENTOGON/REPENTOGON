if not APIOverride then
    APIOverride = {
        OverriddenClasses = {}
    }
end

function APIOverride.GetClass(class)
    if type(class) == "function" then
        return getmetatable(class()).__class
    else
        return getmetatable(class).__class
    end
end

function APIOverride.OverrideClass(class)
    local class_mt = APIOverride.GetClass(class)

    local classDat = APIOverride.OverriddenClasses[class_mt.__type]
    if not classDat then
        classDat = {Original = class_mt, New = {}}

        local oldIndex = class_mt.__index

        rawset(class_mt, "__index", function(self, k)
            return classDat.New[k] or oldIndex(self, k)
        end)

        APIOverride.OverriddenClasses[class_mt.__type] = classDat
    end

    return classDat
end

function APIOverride.GetCurrentClassFunction(class, funcKey)
    local class_mt = APIOverride.GetClass(class)
    return class_mt[funcKey]
end

function APIOverride.OverrideClassFunction(class, funcKey, fn)
    local classDat = APIOverride.OverrideClass(class)
    classDat.New[funcKey] = fn
end