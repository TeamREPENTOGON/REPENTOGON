local lpeg = require("lpeg")

local useIdaFormat = false

local loc = lpeg.locale()
local sp = (
	loc.space +
	(lpeg.P("//") * (1 - lpeg.P("\n"))^0) +
	(lpeg.P("/*") * (1 - lpeg.P("*/"))^0 * lpeg.P("*/"))
)^0
local enum = lpeg.P("enum") * (1 - lpeg.P("};"))^0 * lpeg.P("};")
local garbage = enum + ((lpeg.P("#pragma") + lpeg.P("typedef")) * (1 - lpeg.P("\n"))^0)
local lN0 = loc.alpha + "_"
local lN = loc.alnum + "_"

local sig = lpeg.R

local meta = {}
local meta0 = {__index = meta}

function meta:contains(c)
	local cn = c:cname()
	return self:cname() == cn or c:cname(self) ~= cn
end

function meta:isPointer()
	return self.ptr and #self.ptr > 0
end

function meta:cname(parent)
	local t = {}
	if self.parent then
		local cname = self.parent:cname()
		if not parent or parent:cname() ~= cname then
			t[#t+1] = self.parent:cname(parent)
			t[#t+1] = "::"
		end
	end
	t[#t+1] = self.class
	if self.template then
		t[#t+1] = "<"
		for i,tpl in ipairs(self.template) do
			if i>1 then t[#t+1] = ", " end
			t[#t+1] = tpl:toString(true, parent)
		end
		t[#t+1] = ">"
	end
	
	return table.concat(t)
end

function meta:toString(isNamespace, parent)
	local str = ""
	
	if self.declspec and self.declspec:find("align") then
		str = str.."__declspec"..self.declspec.." "
	end
	
	if self.const then
		str = str.."const "
	end
	
	if self.unsigned then
		str = str.."unsigned "
	end
	
	str = str..self:cname(parent)
	
	if not isNamespace and self.name then
		str = str.." "
	end
	if #self.ptr > 0 then
		str = str..table.concat(self.ptr)
	end
	
	return str
end

function meta:toStringFull(parent)
	local str = self:toString(false, parent)
	str = str..self.name
	
	if self.array then
		str = str.."["
		if self.array > 0 then
			str = str..self.array
		end
		str = str.."]"
	end
	
	return str
end

local initdef

local function idaFormat(t)
	local i0, i1, n = t.class:find("_(%d+)")
	local j0, j1 = t.class:find("__")
	if j0 == 1 then j0 = nil end
	
	if i0 and (not j0 or i0 < j0) then
		-- template
		n = tonumber(n)
		local tstr = t.class:sub(i1+1, i1+n)
		t.template = {}
		
		for cl in string.gmatch(tstr.."___", "(.-)___") do
			t.template[#t.template+1] = initdef({class=cl})
		end
		
		t.class = t.class:sub(1, i0-1)..t.class:sub(i1+n+1)
		j0, j1 = t.class:find("__")
		if j0 == 1 then j0 = nil end
	end
	
	if j0 then
		-- namespace
		local c = {class = t.class:sub(j1+1), ptr = {}, fields = {}}
		t.class = t.class:sub(1, j0-1)
		
		t.name, c.name = c.name, t.name
		t.ptr, c.ptr = c.ptr, t.ptr
		t.array, c.array = c.array, t.array
		t.template, c.template = c.template, t.template
		t.fields, c.fields = c.fields, t.fields
		t.args, c.args = c.args, t.args
		t.varparent, c.varparent = c.varparent, t.varparent
		t.reg, c.reg = c.reg, t.reg
		t.constfunc, c.constfunc = c.constfunc, t.constfunc
		t.generic_code, c.generic_code = c.generic_code, t.generic_code
		t.default, c.default = c.default, t.default
		
		t.child = initdef(c)
	end
	
	-- pointer
	local a, b = t.class:match("^(.-)Z(%d+)$")
	if a then
		t.class = a
		for i=1,tonumber(b) do
			t.ptr[#t.ptr+1] = "*"
		end
	end
	
	-- ignore underscores at the end of the name
	t.class = t.class:gsub("_$", "")
end

local function fixNamespace(t)
	local c = t.child
	
	if t.parentnames then
		local c = t
		local first = true
		for i=#t.parentnames,1,-1 do
			if first then
				c.varparent = setmetatable({class=t.parentnames[i]}, meta0)
				c = c.varparent
				first = false
			else
				c.parent = setmetatable({class=t.parentnames[i]}, meta0)
				c = c.parent
			end
		end
		t.parentnames = nil
	end
	
	if c then
		local p = c
		while p.parent do p = p.parent end
		
		t.name, c.name = c.name, t.name
		t.class, c.class = c.class, t.class
		t.ptr, c.ptr = c.ptr, t.ptr
		t.array, c.array = c.array, t.array
		t.template, c.template = c.template, t.template
		t.fields, c.fields = c.fields, t.fields
		t.args, c.args = c.args, t.args
		t.varparent, c.varparent = c.varparent, t.varparent
		t.reg, c.reg = c.reg, t.reg
		t.constfunc, c.constfunc = c.constfunc, t.constfunc
		t.generic_code, c.generic_code = c.generic_code, t.generic_code
		t.default, c.default = c.default, t.default
		
		if p ~= c then
			t.parent = c.parent
			p.parent = c
			c.parent = nil
		else
			t.parent = c
		end
		t.child = nil
	end
end

local function tonumberorzero(s)
	return tonumber(s) or 0
end

function initdef(t)
	t.fields = t.fields or {}
	t.ptr = t.ptr or {}
	
	if useIdaFormat then
		idaFormat(t)
	end
	fixNamespace(t)
	return setmetatable(t, meta0)
end

local callingConventions = lpeg.P("__stdcall") + lpeg.P("__fastcall") + lpeg.P("__vectorcall") + lpeg.P("__thiscall") + lpeg.P("__cdecl") + lpeg.P("__regparm3") + lpeg.P("__regparm2") + lpeg.P("__regparm1") + lpeg.P("__amd64")

local funcdef = lpeg.P
{
	"S";
	S = sp * lpeg.V("class");
	id = lN0 * lN^0;
	id_or_operator = ("operator" * sp * ("()" + lpeg.S("+-*/<>=!&|^")^1)) + (lN0 * lN^0);
	template = "<" * sp * lpeg.Cg(lpeg.Ct(
		(lpeg.V("class") * ("," * sp * lpeg.V("class"))^0)^-1
	), "template") * ">" * sp;
	depends = "depends" * sp * "(" * sp * lpeg.Cg(lpeg.Ct(
		(lpeg.V("class") * ("," * sp * lpeg.V("class"))^0)^-1
	), "depends") * ")" * sp;
	fields = "{" * sp * lpeg.Cg(lpeg.Ct(
		(lpeg.V("class") * ";" * sp)^0
	), "fields") * "}" * sp;
	generic_code = (lpeg.P("{{") * lpeg.Cg((1 - lpeg.P("}}"))^0, "generic_code") * lpeg.P("}}"));
	sig = "\"" * lpeg.Cg((1 - lpeg.S("\""))^1, "sig") * "\"" * sp * ":" * sp;
	args = "(" * sp * lpeg.Cg(lpeg.Ct(
		(lpeg.V("class") * ("," * sp * lpeg.V("class"))^0)^-1
	), "args") * ")" * sp;
	reg = "<" * sp * lpeg.Cg(lpeg.V("id"), "reg") * sp * ">" * sp;
	default = "=" * sp * lpeg.Cg((lN + lpeg.S(".+-'\"*/"))^1, "default") * sp;
	bp = lpeg.P{"(" * ((1 - lpeg.S"()") + lpeg.V(1))^0 * ")"};
	class = lpeg.Ct(
		lpeg.V("sig")^-1 *
        (lpeg.P("forceDetour") * sp * lpeg.Cg(lpeg.Cc(true), "forceDetour"))^-1 *
		(lpeg.P("noHook") * sp * lpeg.Cg(lpeg.Cc(true), "noHook"))^-1 *
		(lpeg.P("virtual") * sp * lpeg.Cg(lpeg.Cc(true), "virtual"))^-1 *
		(lpeg.P("static") * sp * lpeg.Cg(lpeg.Cc(true), "static"))^-1 *
		(lpeg.P("reference") * sp * lpeg.Cg(lpeg.Cc(true), "reference"))^-1 *
		(lpeg.P("instruction") * sp * lpeg.Cg(lpeg.Cc(true), "instruction"))^-1 *
        (lpeg.P("offsetVariable") * sp * lpeg.Cg(lpeg.Cc(true), "isOffsetVariable"))^-1 *
		(lpeg.P("noop") * sp * lpeg.Cg(lpeg.Cc(true), "noop"))^-1 *
		(lpeg.P("cleanup") * sp * lpeg.Cg(lpeg.Cc(true), "cleanup"))^-1 *
		(lpeg.P("const") * sp * lpeg.Cg(lpeg.Cc(true), "const"))^-1 *
		(lpeg.P("unsigned") * sp * lpeg.Cg(lpeg.Cc(true), "unsigned"))^-1 *
		(lpeg.P("struct") * sp * lpeg.Cg(lpeg.Cc(true), "struct"))^-1 *
		(lpeg.P("union") * sp * lpeg.Cg(lpeg.Cc(true), "union"))^-1 *
		(lpeg.P("namespace") * sp * lpeg.Cg(lpeg.Cc(true), "namespace"))^-1 *
		(lpeg.P(lpeg.Cg(callingConventions, "callingConvention") * sp))^-1 *
		(lpeg.P("__declspec") * sp * lpeg.Cg(lpeg.V("bp"), "declspec") * sp)^-1 *
		lpeg.Cg(lpeg.V("id"), "class") * sp *
		lpeg.V("template")^-1 *
		("::" * sp * lpeg.Cg(lpeg.V("class"), "child"))^-1 *
		lpeg.V("depends")^-1 *
		lpeg.V("generic_code")^-1 *
		lpeg.V("fields")^-1 *
		(lpeg.P("__attribute__") * sp * lpeg.Cg(lpeg.V("bp"), "attribute") * sp)^-1 *
		lpeg.Cg(lpeg.Ct((lpeg.C(lpeg.S("*&")) * sp)^0), "ptr") * sp *
		(
			(
				lpeg.P("(") * sp * lpeg.Cg(lpeg.V("id"), "convention") * sp *
				lpeg.Cg(lpeg.Ct((lpeg.C(lpeg.S("*&")) * sp)^0), "fptr") *
				lpeg.Cg(lpeg.V("id"), "name") * sp * ")" * sp
			)	+
			(
				lpeg.Cg(lpeg.Ct((lpeg.C(lpeg.V("id")) * sp * "::" * sp)^0), "parentnames") *
				lpeg.Cg(lpeg.V("id_or_operator"), "name")
			)
		)^-1 * sp *
		(lpeg.P("[") * sp * lpeg.Cg((loc.alnum^0)/tonumberorzero, "array") * sp * "]" * sp)^-1 *
		lpeg.V("reg")^-1 *
		lpeg.V("args")^-1 *
		(lpeg.P("const") * sp * lpeg.Cg(lpeg.Cc(true), "constfunc") * sp)^-1 *
		lpeg.V("default")^-1
	) / initdef;
}

local cparser = {}

function cparser.UseIdaFormat(b)
	useIdaFormat = b
end

function cparser.ParseDefinition(str)
	return lpeg.match(funcdef * ";", str)
end

function cparser.ParseDefinitions(str)
	return lpeg.match(lpeg.Ct(((sp * garbage)^0 * funcdef * ";")^0), str)
end

function cparser.ParseFunctions(str)
	return lpeg.match(lpeg.Ct((funcdef * ";")^0), str)
end

return cparser
