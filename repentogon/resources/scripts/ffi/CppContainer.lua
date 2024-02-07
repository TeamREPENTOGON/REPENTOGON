local repentogon = ffidll
local lffi = ffi

CppContainer = {
    ArrayProxyMT = function(typename)
        local funcs = {}

        function funcs:Get(key)
            ffichecks.checknumber(1, key)

            if key >= 0 and key < self.Size then return self._items[key]
            else error(string.format("invalid index '%d'", key)) end
        end

        -- todo: wrap it in a table so that __index will work
        return lffi.metatype(typename .. "_List", {
            __tostring = function(self) return string.format("%s[%d]", typename, self.Size) end,
            __len = function(self) return self.Size end,
            __index = function(self, key)
                if ffichecks.isnumber(key) then
                    return self:Get(key)
                else return funcs[key]
                end
            end
        })
    end
}