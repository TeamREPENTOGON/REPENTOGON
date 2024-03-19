ffi.cdef[[
	// stubs
	bool L_GridEntityDesc_GetInitialized(GridEntityDesc*);
	void L_GridEntityDesc_SetInitialized(GridEntityDesc*, bool);
	int L_GridEntityDesc_GetSpawnCount(GridEntityDesc*);
	void L_GridEntityDesc_SetSpawnCount(GridEntityDesc*, int);
	int L_GridEntityDesc_GetSpawnSeed(GridEntityDesc*);
	void L_GridEntityDesc_SetSpawnSeed(GridEntityDesc*, int);
	int L_GridEntityDesc_GetState(GridEntityDesc*);
	void L_GridEntityDesc_SetState(GridEntityDesc*, int);
	int L_GridEntityDesc_GetType(GridEntityDesc*);
	void L_GridEntityDesc_SetType(GridEntityDesc*, int);
	int L_GridEntityDesc_GetVarData(GridEntityDesc*);
	void L_GridEntityDesc_SetVarData(GridEntityDesc*, int);
	int L_GridEntityDesc_GetVariant(GridEntityDesc*);
	void L_GridEntityDesc_SetVariant(GridEntityDesc*, int);
	int L_GridEntityDesc_GetVariableSeed(GridEntityDesc*);
	void L_GridEntityDesc_SetVariableSeed(GridEntityDesc*, int);
]]

local repentogon = ffidll
local lffi = ffi

local getkeys = {
    Initialized = repentogon.L_GridEntityDesc_GetInitialized,
	SpawnCount = repentogon.L_GridEntityDesc_GetSpawnCount,
	SpawnSeed = repentogon.L_GridEntityDesc_GetSpawnSeed,
	State = repentogon.L_GridEntityDesc_GetState,
	Type = repentogon.L_GridEntityDesc_GetType,
	VarData = repentogon.L_GridEntityDesc_GetVarData,
	VariableSeed = repentogon.L_GridEntityDesc_GetVariableSeed,
	Variant = repentogon.L_GridEntityDesc_GetVariant
}

local setkeys = {
    Initialized = repentogon.L_GridEntityDesc_SetInitialized,
	SpawnCount = repentogon.L_GridEntityDesc_SetSpawnCount,
	SpawnSeed = repentogon.L_GridEntityDesc_SetSpawnSeed,
	State = repentogon.L_GridEntityDesc_SetState,
	Type = repentogon.L_GridEntityDesc_SetType,
	VarData = repentogon.L_GridEntityDesc_SetVarData,
	VariableSeed = repentogon.L_GridEntityDesc_SetVariableSeed,
	Variant = repentogon.L_GridEntityDesc_SetVariant
}

GridEntityDescMT = lffi.metatype("", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
		else
			error(string.format("attempted to access nil value '%s'", key))
		end
    end,
    __newindex = function(self, key, value)
        if setkeys[key] ~= nil then
            return setkeys[key](self, value)
        else
            error(string.format("no writable variable '%s'", key))
        end
    end
})
