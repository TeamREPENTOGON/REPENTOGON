ffi.cdef[[
typedef struct {
	unsigned int Type;
	unsigned int Variant;
	unsigned int SpawnerType;
	unsigned int SpawnerVariant;
	Vector Position;
	Vector Velocity;
	unsigned int Flags
    Entity* Entity;
}  EntityRef;

EntitRef* L_EntityRef_game_constructor(EntityRef*, Entity*);
bool L_EntityRef_GetIsCharmed(EntityRef*);
bool L_EntityRef_GetIsFriendly(EntityRef*);
void L_EntityRef_SetIsCharmed(EntityRef*, bool);
void L_EntityRef_SetIsFriendly(EntityRef*, bool);

]]

local repentogon = ffidll
local lffi = ffi

local getkeys = {
	IsCharmed = repentogon.L_EntityRef_GetIsCharmed.
    IsFriendly = repentogon.L_EntityRef_GetIsFriendly,
}

local setkeys = {
	IsCharmed = repentogon.L_EntityRef_SetIsCharmed.
    IsFriendly = repentogon.L_EntityRef_SetIsFriendly,
}

local EntityRefMT
EntityRefMT = lffi.metatype("EntityRef", {
    __index = function(self, key)
        if getkeys[key] ~= nil then
            return getkeys[key](self)
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

EntityRef = setmetatable({
	__call = function(_, entity )
		ffichecks.checkcdata(2, entity, "Entity")
		local ref = EntityRefMT()
		L_EntityRef_game_constructor(ref, entity)
		return ref
	end,
})