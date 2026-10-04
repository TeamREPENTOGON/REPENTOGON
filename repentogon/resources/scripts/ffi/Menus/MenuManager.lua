ffi.cdef [[
    int L_MenuManager_GetActiveMenu();
    struct ColorModifier* L_MenuManager_GetColorModifierLerpAmount();
    struct ColorModifier* L_MenuManager_GetCurrentColorModifier();
    unsigned int L_MenuManager_GetInputMask();
    struct Sprite* L_MenuManager_GetShadowSprite();
    struct ColorModifier* L_MenuManager_GetTargetColorModifier();
    struct Vector* L_MenuManager_GetViewPosition();
    bool L_MenuManager_IsActive();
    void L_MenuManager_SetActiveMenu(int);
    void L_MenuManager_SetColorModifier(struct ColorModifier*, bool, float);
    void L_MenuManager_SetInputMask(unsigned int);
    void L_MenuManager_SetViewPosition(struct Vector*);
]]
local ffi = ffi
local repentogon = ffidll

local cfuncs = {
    GetSeeds = __Lua_MenuManager_GetSeeds
}

ffichecks.checkmainmenu = function(className)
	if not repentogon.L_MenuManager_IsActive() then
		error(className .. " functions can only be used in the main menu", 3)
	end
end

MenuManager = {
    GetActiveMenu = function()
        ffichecks.checkmainmenu("MenuManager")
        return repentogon.L_MenuManager_GetActiveMenu();
    end,
    GetColorModifierLerpAmount = function()
        ffichecks.checkmainmenu("MenuManager")
        local p = repentogon.L_MenuManager_GetColorModifierLerpAmount();
        return ffi.new("struct ColorModifier", p[0])
    end,
    GetCurrentColorModifier = function()
        ffichecks.checkmainmenu("MenuManager")
        local p = repentogon.L_MenuManager_GetCurrentColorModifier()
        return ffi.new("struct ColorModifier", p[0])
    end,
    GetInputMask = function()
        ffichecks.checkmainmenu("MenuManager")
        return repentogon.L_MenuManager_GetInputMask();
    end,
    GetSeeds = function()
        ffichecks.checkmainmenu("MenuManager")
        return cfuncs.GetSeeds()
    end,
    GetShadowSprite = function()
        ffichecks.checkmainmenu("MenuManager")
        return repentogon.L_MenuManager_GetShadowSprite();
    end,
    GetTargetColorModifier = function()
        ffichecks.checkmainmenu("MenuManager")
        local p = repentogon.L_MenuManager_GetTargetColorModifier();
        return ffi.new("struct ColorModifier", p[0])
    end,
    GetViewPosition = function()
        ffichecks.checkmainmenu("MenuManager")
        return ffichecks.copyvector(repentogon.L_MenuManager_GetViewPosition())
    end,
    IsActive = function()
        return repentogon.L_MenuManager_IsActive();
    end,
    SetActiveMenu = function(menu)
        ffichecks.checkmainmenu("MenuManager")
        ffichecks.checkinteger(1, menu)
        repentogon.L_MenuManager_SetActiveMenu(menu);
    end,
    SetColorModifier = function(colorModifier, lerp, rate)
        ffichecks.checkmainmenu("MenuManager")
        ffichecks.checkcdata(1, colorModifier, "ColorModifier")
        lerp = ffichecks.optboolean(lerp, true)
        rate = ffichecks.optnumber(rate, 0.015)
        repentogon.L_MenuManager_SetColorModifier(colorModifier, lerp, rate);
    end,
    SetInputMask = function(inputMask)
        ffichecks.checkmainmenu("MenuManager")
        ffichecks.checkinteger(1, inputMask)
        repentogon.L_MenuManager_SetInputMask(inputMask);
    end,
    SetViewPosition = function(position)
        ffichecks.checkmainmenu("MenuManager")
        ffichecks.checkcdata(1, position, "Vector")
        repentogon.L_MenuManager_SetViewPosition(position);
    end,
}
__Lua_MenuManager_GetSeeds = nil