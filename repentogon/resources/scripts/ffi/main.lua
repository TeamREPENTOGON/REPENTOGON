-- Aw yeah, this is happening!
ffidll = ffi.load("zhlREPENTOGON")
ffichecks = {}
local lffi = ffi

local debug_getinfo = debug.getinfo

local ctypeCache = {}
local function resolveCtype(ctype)
	if type(ctype) ~= "string" then return ctype end
	local cached = ctypeCache[ctype]
	if cached then return cached end
	local resolved
	local ok, r = pcall(lffi.typeof, ctype)
	if ok then
		resolved = r
	else
		for _, pre in ipairs({ "struct ", "union ", "enum " }) do
			local ok2, r2 = pcall(lffi.typeof, pre .. ctype)
			if ok2 then resolved = r2; break end
		end
	end
	ctypeCache[ctype] = resolved
	return resolved
end

ffichecks.gettype = function(var)
	local t = type(var)
	if t == "cdata" then t = tostring(lffi.typeof(var)) end
	return t
end

ffichecks.checktype = function(index, val, typ, level)
	local t = type(val)
	if t ~= typ then
		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", index, debug_getinfo(level or 2).name, typ, t), (level or 2)+1)
	end
end

ffichecks.istype = function(var, typ)
	return type(var) == typ
end

ffichecks.isnil = function(var) return ffichecks.istype(var, "nil") end
ffichecks.isnullptr = function(cdata) return cdata == nil end
ffichecks.isnumber = function(var) return ffichecks.istype(var, "number") end
ffichecks.isstring = function(var) return ffichecks.istype(var, "string") end
ffichecks.isboolean = function(var) return ffichecks.istype(var, "boolean") end
ffichecks.iscdata = function(var, ctype)
	if not var then return false end
	local ct = resolveCtype(ctype)
	if ct ~= nil and lffi.istype(ct, var) then
		return true
	end
	-- Also accept a pointer to the target type (reference cdata).
	if type(var) == "cdata" then
		local ptr = resolveCtype(ctype .. "*")
		return ptr ~= nil and lffi.istype(ptr, var) or false
	end
	return false
end

ffichecks.checknumber = function(index, val, level) ffichecks.checktype(index, val, "number", (level or 2)+1) end
ffichecks.checkfunction = function(index, val, level) ffichecks.checktype(index, val, "function", (level or 2)+1) end
ffichecks.checkstring = function(index, val, level) ffichecks.checktype(index, val, "string", (level or 2)+1) end
ffichecks.checkboolean = function(index, val, level) ffichecks.checktype(index, val, "boolean", (level or 2)+1) end
ffichecks.checkinteger = function(index, val, level) 
	if math.type(val) ~= "integer" then
		error(string.format("bad argument #%d to '%s' (integer expected, got %s)", index, debug_getinfo(level or 2).name, type(val)), (level or 2)+1)
	end
end

ffichecks.checkcdata = function(idx, var, ctype, allownil, level)
	if not (ffichecks.iscdata(var, ctype) or (allownil and ffichecks.isnil(var))) then
		local t = type(var)
		if t == "cdata" then t = tostring(lffi.typeof(var)) end

		error(string.format("bad argument #%d to '%s' (%s expected, got %s)", idx, debug_getinfo(level or 2).name, tostring(ctype), t), (level or 2)+1)
	end
end

ffichecks.callcdatafunc = function(this, cdata, ctype, cfunc)
	ffichecks.checkcdata(2, cdata, ctype)
	cfunc(this, cdata)
end

ffichecks.fixreturn = function(cdata)
	if not ffichecks.isnullptr(cdata) then
		return cdata
	end
	return nil
end

ffichecks.optnumber = function(var, opt)
	if ffichecks.isnumber(var) then
		return var
	end
	return opt
end
ffichecks.optboolean = function(var, opt)
	if ffichecks.isboolean(var) then
		return var
	end
	return opt
end
ffichecks.optstring = function(var, opt)
	if ffichecks.isstring(var) then
		return var
	end
	return opt
end
ffichecks.optcdata = function(var, ctype, opt)
	if ffichecks.iscdata(var, cdata) then
		return var
	end
	return opt
end


pcall(require("ffi.Vector"))
pcall(require("ffi.GridEntityDesc"))
pcall(require("ffi.Color"))
pcall(require("ffi.PosVel"))
pcall(require("ffi.BitSet128"))
pcall(require("ffi.RNG"))

ffi = nil
ffidll = nil
