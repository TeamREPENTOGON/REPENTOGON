ffi.cdef[[
struct BitSet128 { uint64_t L; uint64_t H; };
typedef struct BitSet128* BitSet12Ptr;
]]
local repentogon = ffidll
local lffi = ffi

local function bsNorm(op, idx)
	if type(op) == "number" then
		return lffi.new("uint64_t", op), lffi.new("uint64_t", 0)
	end
	ffichecks.checkcdata(idx, op, "BitSet128")
	return op.L, op.H
end

local BitSet128MT
BitSet128MT = {
    __type = "BitSet128",

    __tostring = function(self) 
        return string.format("BitSet128: %016x%016x", tonumber(self.H), tonumber(self.L))
    end,
__bnot = function(self)
        local a = ~self.L
        local b = ~self.H
        return BitSet128(a, b)
    end,
	__bor = function(self, second)
        local sl, sh = bsNorm(self, 1)
        local ol, oh = bsNorm(second, 2)
        return BitSet128(sl | ol, sh | oh)
    end,
	__band = function(self, second)
        local sl, sh = bsNorm(self, 1)
        local ol, oh = bsNorm(second, 2)
        return BitSet128(sl & ol, sh & oh)
    end,
    __bxor = function(self, second)
        local sl, sh = bsNorm(self, 1)
        local ol, oh = bsNorm(second, 2)
        return BitSet128(sl ~ ol, sh ~ oh)
    end,
    __shl = function(self, shift)
        ffichecks.checknumber(2, shift)
        local a = self.L << shift
        local b = self.H << shift
        return BitSet128(a, b)
    end,
    __shr = function(self, shift)
        ffichecks.checknumber(2, shift)
        local a = self.L >> shift
        local b = self.H >> shift
        return BitSet128(a, b)
    end,
    __eq = function(self, second)
        local sl, sh = bsNorm(self, 1)
        local ol, oh = bsNorm(second, 2)
        return (sl == ol and sh == oh)
    end,
    __lt = function(self, second)
        local sl, sh = bsNorm(self, 1)
        local ol, oh = bsNorm(second, 2)
        if (sh ~= oh) then
            return (sh < oh)
        else
            return (sl < ol)
        end
    end,
    __le = function(self, second)
        local sl, sh = bsNorm(self, 1)
        local ol, oh = bsNorm(second, 2)
        if (sh ~= oh) then
            return (sh <= oh)
        else
            return (sl <= ol)
        end
    end,
    Get = function(self, pos)
        ffichecks.checknumber(2, pos)
        local p = pos < 64 and self.L or self.H
        local bit_pos = pos % 64
        return ((p >> bit_pos) & 1) ~= 0
    end,
    Set = function(self, pos, state)
        ffichecks.checknumber(2, pos)
        ffichecks.checkboolean(3, state)
        local shift = pos % 64
        if pos < 64 then
            if state then
                self.L = self.L | (1 << shift)
            else
                self.L = self.L & ~(1 << shift)
            end
        else
            if state then
                self.H = self.H | (1 << shift)
            else
                self.H = self.H & ~(1 << shift)
            end
        end
    end
}

BitSet128MT.__index = function(self, key)
	return BitSet128MT[key]
end 

local BitSet128T = ffi.metatype("struct BitSet128", BitSet128MT)

BitSet128 = setmetatable({}, {
    __call = function(_, L, H)
        return BitSet128T(L or 0, H or 0)
    end,
    __class = BitSet128MT,
})