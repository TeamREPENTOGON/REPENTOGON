-- save copy of original enums, to make it harder for mods to mess with the repentogon features
local _ModCallbacks = ModCallbacks
local _MainMenuType = MainMenuType
local _ButtonAction = ButtonAction
local _Keyboard = Keyboard

local MaxPollCIdx = 3
local function IsActionTriggeredAll(action)
    for i = 0, MaxPollCIdx do
        if Input.IsActionTriggered(action, i) then
            return true
        end
    end
    return false
end

local function IsButtonTriggeredAll(action)
    for i = 0, MaxPollCIdx do
        if Input.IsButtonTriggered(action, i) then
            return true
        end
    end
    return false
end

local function TweakModsMenu()
    -- The values are automatically clamped so there's not much to worry about here
    if MenuManager.GetActiveMenu() == _MainMenuType.MODS then
        if IsActionTriggeredAll(_ButtonAction.ACTION_MENULEFT) then
		 	ModsMenu.SetSelectedElement(ModsMenu.GetSelectedElement() - 3)
        elseif IsActionTriggeredAll(_ButtonAction.ACTION_MENURIGHT) then
            ModsMenu.SetSelectedElement(ModsMenu.GetSelectedElement() + 3)
		end
		
		if IsButtonTriggeredAll(_Keyboard.KEY_PAGE_UP) or IsButtonTriggeredAll(_ButtonAction.ACTION_MENULT) then
			ModsMenu.SetSelectedElement(ModsMenu.GetSelectedElement() - 10)
		elseif IsButtonTriggeredAll(_Keyboard.KEY_PAGE_DOWN) or IsActionTriggeredAll(_ButtonAction.ACTION_MENURT) then
			ModsMenu.SetSelectedElement(ModsMenu.GetSelectedElement() + 10)
		end
	end
end

Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, TweakModsMenu)