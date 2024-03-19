ffi.cdef[[
	// stubs
	typedef struct {void *_;} GridEntity_Door;
	
	bool L_GridEntity_Destroy(GridEntity*, bool);
	int L_GridEntity_GetGridIndex(GridEntity*);
	RNG* L_GridEntity_GetRNG(GridEntity*);
	GridEntityDesc* L_GridEntity_GetDesc(GridEntity*);
	void L_GridEntity_SetDesc(GridEntity*, GridEntityDesc*);
	ANM2* L_GridEntity_GetSprite(GridEntity*);
	int L_GridEntity_GetType(GridEntity*);
	void L_GridEntity_SetType(GridEntity*, int);
	int L_GridEntity_GetVariant(GridEntity*);
	void L_GridEntity_SetVariant(GridEntity*, int);
	void L_GridEntity_Init(GridEntity*, int);
	void L_GridEntity_PosInit(GridEntity*);
	void L_GridEntity_Render(GridEntity*, Vector*);
	GridEntity_Door* L_GridEntity_ToDoor(GridEntity*);
	int L_GridEntity_GetCollisionClass(GridEntity*);
	void L_GridEntity_SetCollisionClass(GridEntity*, int);
	Vector* L_GridEntity_GetPosition(GridEntity*);
	int L_GridEntity_GetState(GridEntity*);
	void L_GridEntity_SetState(GridEntity*, int);
	int L_GridEntity_GetVarData(GridEntity*);
	void L_GridEntity_SetVarData(GridEntity*, int);
]]

local repentogon = ffidll
local lffi = ffi

GridEntityFuncs = {}

function GridEntityFuncs:Destroy(immediate)
	ffichecks.checkboolean(2, immediate)
	return repentogon.L_GridEntity_Destroy(self, immediate)
end

function GridEntityFuncs:GetGridIndex()
	return repentogon.L_GridEntity_GetGridIndex(self)
end

function GridEntityFuncs:GetRNG()
	return ffichecks.fixreturn(repentogon.L_GridEntity_GetRNG(self))
end

function GridEntityFuncs:GetDesc()
	return ffichecks.fixreturn(repentogon.L_GridEntity_GetDesc(self))
end

function GridEntityFuncs:GetSprite()
	return ffichecks.fixreturn(repentogon.L_GridEntity_GetSprite(self))
end

function GridEntityFuncs:GetType()
	return repentogon.L_GridEntity_GetType(self)
end

function GridEntityFuncs:GetVariant()
	return repentogon.L_GridEntity_GetVariant(self)
end

function GridEntityFuncs:Init(seed)
	ffichecks.checknumber(2, seed)
	repentogon.L_GridEntity_Init(self, seed)
end

function GridEntityFuncs:Init()
	repentogon.L_GridEntity_PostInit(self)
end

function GridEntityFuncs:Render(offset)
	ffichecks.checkcdata(2, offset, "Vector")
	repentogon.L_GridEntity_Render(self, offset)
end

function GridEntityFuncs:SetType(value)
	ffichecks.checknumber(2, value)
	repentogon.L_GridEntity_SetType(self, value)
end

function GridEntityFuncs:SetVariant(value)
	ffichecks.checknumber(2, value)
	repentogon.L_GridEntity_SetVariant(self, value)
end

function GridEntityFuncs:ToDoor()
	return ffichecks.fixreturn(repentogon.L_GridEntity_ToDoor(self))
end

local getkeys = {
    CollisionClass = repentogon.L_GridEntity_GetCollisionClass,
	Desc = ffichecks.fixreturn(repentogon.L_GridEntity_Entity_GetDesc),
	Position = ffichecks.fixreturn(repentogon.L_GridEntity_GetPosition),
	State = repentogon.L_GridEntity_GetState,
	VarData = repentogon.L_GridEntity_GetVarData,
}

local setkeys = {
	CollisionClass = repentogon.L_GridEntity_SetCollisionClass,
	Desc = function(self, cdata) ffichecks.callcdatafunc(self, cdata, "GridEntityDesc", repentogon.L_GridEntiyDesc_SetDesc) end,
	State = repentogon.L_GridEntity_SetState,
	VarData = repentogon.L_GridEntity_SetVarData,
}

GridEntity_getkeys = getkeys
GridEntity_setkeys = setkeys

GridEntityMT = lffi.metatype("GridEntity", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
        elseif GridEntityFuncs[key] ~= nil then
			return GridEntityFuncs[key]
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
