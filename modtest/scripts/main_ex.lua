REPENTOGON = true

local defaultCallbackMeta = {
	__matchParams = function(a, b)
		return not a or not b or a == -1 or b == -1 or a == b
	end
}

local typecheckFunctions = {}

typecheckFunctions[tostring(ModCallbacks.MC_PRE_ADD_COLLECTIBLE)] = function(ret) --TODO might be better as a table of paths to take depending on ret type? might help performance a bit, less if/else statements
	if type(ret) == "table" then
		local tablesize = #ret
		if tablesize ~= 5 then
			return "Bad return table length to MC_PRE_ADD_COLLECTIBLE (5 expected, got " .. tostring(tablesize) .. ")"
		end
	elseif type(ret) == "number" and math.type(ret) == "integer" then -- valid
	else
		return "Bad return type to MC_PRE_ADD_COLLECTIBLE (table or whole number expected, got " .. tostring(type(ret)) .. ")"
	end
end



function Isaac.RunCallbackWithParam(callbackID, Param, ...)
    local callbacks = Isaac.GetCallbacks(callbackID)
	if callbacks then
		for _, callback in ipairs(callbacks) do
			local matchFunc = getmetatable(callbacks).__matchParams or defaultCallbackMeta.__matchParams
			if matchFunc(Param, callback.Param) then
			local ret = callback.Function(callback.Mod, ...)
				if ret ~= nil then
					if typecheckFunctions[tostring(callbackID)] then
						local err = typecheckFunctions[tostring(callbackID)](ret)
						if err then
							error({callback = callback, ret = ret, msg = err})
						end
					end
					return ret
				end
			end
		end
	end
end

function Isaac.RunCallback(callbackID, ...)
	return Isaac.RunCallbackWithParam(callbackID, nil, ...)
end