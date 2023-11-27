#include "CustomImGui.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "NotificationHandler.h"

extern CustomImGui customImGui;
extern NotificationHandler notificationHandler;

extern bool menuShown;

static int CheckAndSetCallback(lua_State* L, int slot)
{
	if (!lua_isfunction(L, slot) && !lua_isnoneornil(L, slot))
		return luaL_error(L, "Argument %d is not a function and not nil", (slot - 1));
	if (!lua_isfunction(L, slot)) {
		return 0;
	}
	lua_pushvalue(L, slot); // push function on stack
	return luaL_ref(L, LUA_REGISTRYINDEX); // get unique key
}

static int EvalIDAndParent(lua_State* L, const char* id, const char* parentId)
{
	if (!customImGui.ElementExists(parentId))
		return luaL_error(L, "Parent Element with id '%s'doesnt exist.", parentId);
	if (customImGui.ElementExists(id))
		return luaL_error(L, "Element with id '%s' exists already.", id);
	return 0;
}

LUA_FUNCTION(Lua_CustomImGui)
{
	Manager* manager = g_Manager;
	CustomImGui** ud = (CustomImGui**)lua_newuserdata(L, sizeof(CustomImGui*));
	*ud = &customImGui;
	luaL_setmetatable(L, lua::metatables::ImGuiMT);
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddElement)
{
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	int type = (int)luaL_checkinteger(L, 4);
	const char* text = luaL_optstring(L, 5, "");

	EvalIDAndParent(L, id, parentId);

	customImGui.AddElement(parentId, id, text, type);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_RemoveElement)
{
	const char* elementId = luaL_checkstring(L, 2);

	customImGui.RemoveElement(elementId);

	return 0;
}

LUA_FUNCTION(Lua_ImGui_LinkWindowToElement)
{
	const char* windowId = luaL_checkstring(L, 2);
	const char* elementId = luaL_checkstring(L, 3);

	if (customImGui.GetElementById(windowId) == NULL) {
		return luaL_error(L, "No window with id '%s' exists", windowId);
	}

	bool success = customImGui.LinkWindowToElement(windowId, elementId);
	if (!success) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_CreateMenu)
{
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_checkstring(L, 3);

	if (customImGui.ElementExists(id)) {
		return luaL_error(L, "Element with id '%s' exists already.", id);
	}

	bool success = customImGui.CreateMenuElement(id, text);
	if (!success) {
		return luaL_error(L, "Error while adding new Menu '%s'", id);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_RemoveMenu)
{
	const char* menuId = luaL_checkstring(L, 2);

	customImGui.RemoveMenu(menuId);

	return 0;
}

LUA_FUNCTION(Lua_ImGui_CreateWindow)
{
	const char* id = luaL_checkstring(L, 2);
	const char* title = luaL_checkstring(L, 3);

	if (customImGui.ElementExists(id)) {
		return luaL_error(L, "Element with id '%s' exists already.", id);
	}

	bool success = customImGui.CreateWindowElement(id, title);
	if (!success) {
		return luaL_error(L, "Error while adding new Window '%s'", id);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_RemoveWindow)
{
	const char* windowId = luaL_checkstring(L, 2);

	customImGui.RemoveWindow(windowId);

	return 0;
}

LUA_FUNCTION(Lua_ImGui_AddCallback)
{
	const char* parentId = luaL_checkstring(L, 2);
	int type = (int)luaL_checkinteger(L, 3);
	if (!lua_isfunction(L, 4)) {
		return luaL_error(L, "Argument %d is not a function", 4);
	}
	int stackID = CheckAndSetCallback(L, 4);

	bool success = customImGui.AddCallback(parentId, type, stackID);
	if (!success) {
		return luaL_error(L, "No element '%s' found.", parentId);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_RemoveCallback)
{
	const char* parentId = luaL_checkstring(L, 2);
	int type = (int)luaL_checkinteger(L, 3);

	bool success = customImGui.RemoveCallback(parentId, type);
	if (!success) {
		return luaL_error(L, "No element '%s' found.", parentId);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_UpdateText)
{
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_checkstring(L, 3);

	bool success = customImGui.UpdateText(id, text);
	if (!success) {
		return luaL_error(L, "No element with id '%s' found.", id);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_UpdateData)
{
	// prechecks
	const char* id = luaL_checkstring(L, 2);
	luaL_checkinteger(L, 3);
	Element* createdElement = customImGui.GetElementById(id);
	if (createdElement == nullptr) {
		return luaL_error(L, "No element with id '%s' found.", id);
	}

	bool success = customImGui.UpdateElementData(createdElement, L);
	if (!success) {
		return luaL_error(L, "The given element does not use the provided data type.");
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddButton)
{
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	bool isSmall = lua::luaL_optboolean(L, 6, false);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::Button);
	if (isSmall)
		type = static_cast<int>(IMGUI_ELEMENT::SmallButton);

	customImGui.AddElement(parentId, id, text, type);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Clicked), stackID);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddText)
{
	const char* parentId = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	bool isWrapped = lua::luaL_optboolean(L, 4, false);
	const char* id = luaL_optstring(L, 5, "");

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::Text);
	if (isWrapped)
		type = static_cast<int>(IMGUI_ELEMENT::TextWrapped);

	customImGui.AddElement(parentId, id, text, type);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputInteger)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.setDefaultIntVal((int)luaL_optinteger(L, 6, data.defaultIntVal));
	data.step = (float)luaL_optinteger(L, 7, (int)data.step);
	data.stepFast = (float)luaL_optinteger(L, 8, (int)data.stepFast);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputInt);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputFloat)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.setDefaultFloatVal((float)luaL_optnumber(L, 6, data.defaultFloatVal));
	data.step = (float)luaL_optnumber(L, 7, data.step);
	data.stepFast = (float)luaL_optnumber(L, 8, data.stepFast);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputFloat);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddDragInteger)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.setDefaultIntVal((int)luaL_optinteger(L, 6, data.defaultIntVal));
	data.speed = (float)luaL_optnumber(L, 7, data.speed);
	data.minVal = (float)luaL_optinteger(L, 8, (int)data.minVal);
	data.maxVal = (float)luaL_optinteger(L, 9, (int)data.maxVal);
	data.formatting = luaL_optstring(L, 10, data.DefaultIntNumberFormatting);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::DragInt);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddDragFloat)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.setDefaultFloatVal((float)luaL_optnumber(L, 6, data.defaultFloatVal));
	data.speed = (float)luaL_optnumber(L, 7, data.speed);
	data.minVal = (float)luaL_optnumber(L, 8, data.minVal);
	data.maxVal = (float)luaL_optnumber(L, 9, data.maxVal);
	data.formatting = luaL_optstring(L, 10, data.DefaultFloatNumberFormatting);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::DragFloat);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddSliderInteger)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.setDefaultIntVal((int)luaL_optinteger(L, 6, data.defaultIntVal));
	data.minVal = (float)luaL_optinteger(L, 7, (int)data.minVal);
	data.maxVal = (float)luaL_optinteger(L, 8, (int)data.maxVal);
	data.formatting = luaL_optstring(L, 9, data.DefaultIntNumberFormatting);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::SliderInt);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddSliderFloat)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.setDefaultFloatVal((float)luaL_optnumber(L, 6, data.defaultFloatVal));
	data.minVal = (float)luaL_optnumber(L, 7, data.minVal);
	data.maxVal = (float)luaL_optnumber(L, 8, data.maxVal);
	data.formatting = luaL_optstring(L, 9, data.DefaultFloatNumberFormatting);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::SliderFloat);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputColor)
{
	ColorData data = ColorData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.r = (float)luaL_optnumber(L, 6, 0);
	data.g = (float)luaL_optnumber(L, 7, 0);
	data.b = (float)luaL_optnumber(L, 8, 0);
	data.useAlpha = !lua_isnoneornil(L, 9);
	data.a = (float)luaL_optnumber(L, 9, 1);

	data.init();

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::ColorEdit);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddCheckbox)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.checked = lua::luaL_optboolean(L, 6, false);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::Checkbox);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddRadioButtons)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	int stackID = CheckAndSetCallback(L, 4);
	if (!lua_istable(L, 5)) {
		return luaL_error(L, "Argument 5 needs to be a table!");
	}
	data.index = (int)luaL_optinteger(L, 6, 0);
	data.sameLine = lua::luaL_optboolean(L, 7, true);

	// get table input
	auto tableLength = lua_rawlen(L, 5);
	for (auto i = 1; i <= tableLength; ++i) {
		lua_pushinteger(L, i);
		lua_gettable(L, 5);
		if (lua_type(L, -1) == LUA_TNIL)
			break;
		data.values->push_back(luaL_checkstring(L, -1));
		lua_pop(L, 1);
	}

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::RadioButton);

	customImGui.AddElement(parentId, id, "", type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddTabBar)
{
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::TabBar);

	customImGui.AddElement(parentId, id, "", type);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddTab)
{
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_checkstring(L, 4);

	EvalIDAndParent(L, id, parentId);
	if (customImGui.GetElementById(parentId)->type != IMGUI_ELEMENT::TabBar) {
		return luaL_error(L, "The given parent element is not of type 'TabBar'!");
	}

	int type = static_cast<int>(IMGUI_ELEMENT::Tab);

	customImGui.AddElement(parentId, id, text, type);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddCombobox)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	if (!lua_istable(L, 6)) {
		return luaL_error(L, "Argument 6 needs to be a table!");
	}
	data.index = (int)luaL_optinteger(L, 7, 0);
	data.isSlider = lua::luaL_optboolean(L, 8, false);

	// get table input
	auto tableLength = lua_rawlen(L, 6);
	for (auto i = 1; i <= tableLength; ++i) {
		lua_pushinteger(L, i);
		lua_gettable(L, 6);
		if (lua_type(L, -1) == LUA_TNIL)
			break;
		data.values->push_back(luaL_checkstring(L, -1));
		lua_pop(L, 1);
	}

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::Combobox);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputText)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.inputText = luaL_optstring(L, 6, "");
	data.hintText = luaL_optstring(L, 7, "");

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputText);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputTextMultiline)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.inputText = luaL_optstring(L, 6, "");
	data.lineCount = (float)luaL_optnumber(L, 7, 6);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputTextMultiline);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_SetTooltip)
{
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_checkstring(L, 3);

	bool success = customImGui.SetTooltipText(id, text);
	if (!success) {
		return luaL_error(L, "No element with id '%s' found.", id);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_SetHelpmarker)
{
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_checkstring(L, 3);

	bool success = customImGui.SetHelpMarkerText(id, text);
	if (!success) {
		return luaL_error(L, "No element with id '%s' found.", id);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_GetMousePos)
{
	float x = -1;
	float y = -1;

	if (menuShown) {
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsMousePosValid()) {
			x = io.MousePos.x;
			y = io.MousePos.y;
		}
	}
	else {
		x = (float)*(double*)((char*)g_KAGEInputController + 0x48);
		y = (float)*(double*)((char*)g_KAGEInputController + 0x50);
	}

	lua::LuaCaller(L).pushUserdataValue(*new Vector(x, y), lua::Metatables::VECTOR);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputController)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.currentIntVal = (int)luaL_optinteger(L, 6, 0);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputController);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputKeyboard)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	int stackID = CheckAndSetCallback(L, 5);
	data.currentIntVal = (int)luaL_optinteger(L, 6, 0);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputKeyboard);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 5)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddPlotLines)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	if (!lua_istable(L, 5)) {
		return luaL_error(L, "Argument 5 needs to be a table!");
	}
	data.hintText = luaL_optstring(L, 6, "");
	data.minVal = (float)luaL_optnumber(L, 7, FLT_MIN);
	data.maxVal = (float)luaL_optnumber(L, 8, FLT_MAX);
	data.defaultFloatVal = (float)luaL_optnumber(L, 9, 40.0f);

	EvalIDAndParent(L, id, parentId);

	// get table input
	auto tableLength = lua_rawlen(L, 5);
	for (auto i = 1; i <= tableLength; ++i) {
		lua_pushinteger(L, i);
		lua_gettable(L, 5);
		if (lua_type(L, -1) == LUA_TNIL)
			break;
		data.plotValues->push_back((float)luaL_checknumber(L, -1));
		lua_pop(L, 1);
	}

	int type = static_cast<int>(IMGUI_ELEMENT::PlotLines);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddPlotHistogram)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	if (!lua_istable(L, 5)) {
		return luaL_error(L, "Argument 5 needs to be a table!");
	}
	data.hintText = luaL_optstring(L, 6, "");
	data.minVal = (float)luaL_optnumber(L, 7, FLT_MIN);
	data.maxVal = (float)luaL_optnumber(L, 8, FLT_MAX);
	data.defaultFloatVal = (float)luaL_optnumber(L, 9, 40.0f);

	EvalIDAndParent(L, id, parentId);

	// get table input
	auto tableLength = lua_rawlen(L, 5);
	for (auto i = 1; i <= tableLength; ++i) {
		lua_pushinteger(L, i);
		lua_gettable(L, 5);
		if (lua_type(L, -1) == LUA_TNIL)
			break;
		data.plotValues->push_back((float)luaL_checknumber(L, -1));
		lua_pop(L, 1);
	}

	int type = static_cast<int>(IMGUI_ELEMENT::PlotHistogram);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddProgressBar)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 2);
	const char* id = luaL_checkstring(L, 3);
	const char* text = luaL_optstring(L, 4, "");
	data.currentFloatVal = (float)luaL_optnumber(L, 5, 0.0f);
	data.hintText = luaL_optstring(L, 6, "__DEFAULT__"); // special placeholder for default behavior handling

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::ProgressBar);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_PushNotification)
{
	const char* text = luaL_checkstring(L, 2);
	int severity = (int)luaL_optinteger(L, 3, 0);
	if (severity < 0 || severity > 3)
		return luaL_error(L, "Severity needs to be a value between 0 and 3");
	int lifetime = (int)luaL_optinteger(L, 4, 5000);

	notificationHandler.AddNotification(text, severity, lifetime);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_Show)
{
	menuShown = true;

	return 0;
}

LUA_FUNCTION(Lua_ImGui_Hide)
{
	menuShown = false;

	return 0;
}

LUA_FUNCTION(Lua_ImGui_Reset)
{
	customImGui.Reset();

	return 0;
}

LUA_FUNCTION(Lua_ImGui_ElementExists)
{
	const char* id = luaL_checkstring(L, 2);

	lua_pushboolean(L, customImGui.ElementExists(id));

	return 1;
}

LUA_FUNCTION(Lua_ImGui_GetVisible)
{
	const char* elementId = luaL_checkstring(L, 2);

	if (!customImGui.ElementExists(elementId)) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	lua_pushboolean(L, customImGui.GetVisible(elementId));
	return 1;
}


LUA_FUNCTION(Lua_ImGui_SetVisible)
{
	const char* elementId = luaL_checkstring(L, 2);
	bool newState = lua::luaL_checkboolean(L, 3);

	bool success = customImGui.SetVisible(elementId, newState);

	if (!success) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_SetColor)
{
	const char* elementId = luaL_checkstring(L, 2);
	int type = (int)luaL_checkinteger(L, 3);
	float r = (float)luaL_checknumber(L, 4);
	float g = (float)luaL_checknumber(L, 5);
	float b = (float)luaL_checknumber(L, 6);
	float a = (float)luaL_optnumber(L, 7, 1.0f);

	bool success = customImGui.SetColor(elementId, static_cast<ImGuiCol_>(type), ImVec4(r, g, b, a));

	if (!success) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_RemoveColor)
{
	const char* elementId = luaL_checkstring(L, 2);
	int type = (int)luaL_checkinteger(L, 3);

	bool success = customImGui.RemoveColor(elementId, static_cast<ImGuiCol_>(type));

	if (!success) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_SetTextColor)
{
	const char* elementId = luaL_checkstring(L, 2);
	float r = (float)luaL_checknumber(L, 3);
	float g = (float)luaL_checknumber(L, 4);
	float b = (float)luaL_checknumber(L, 5);
	float a = (float)luaL_optnumber(L, 6, 1.0f);

	bool success = customImGui.SetColor(elementId, ImGuiCol_Text, ImVec4(r, g, b, a));

	if (!success) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_GetWindowPinned)
{
	const char* elementId = luaL_checkstring(L, 2);

	Element* element = customImGui.GetElementById(elementId);
	if (element != NULL && element->type == IMGUI_ELEMENT::Window) {
		lua_pushboolean(L, element->data.windowPinned);
		return 1;
	}
	else {
		return luaL_error(L, "Window Element with id '%s' not found", elementId);
	}
}

LUA_FUNCTION(Lua_ImGui_SetWindowPinned)
{
	const char* elementId = luaL_checkstring(L, 2);
	bool newState = lua::luaL_checkboolean(L, 3);

	bool success = customImGui.SetPinned(elementId, newState);

	if (!success) {
		return luaL_error(L, "Window Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_SetWindowPosition)
{
	const char* elementId = luaL_checkstring(L, 2);
	float x = (float)luaL_checknumber(L, 3);
	float y = (float)luaL_checknumber(L, 4);

	bool success = customImGui.SetWindowPosition(elementId, x, y);

	if (!success) {
		return luaL_error(L, "Window Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_SetWindowSize)
{
	const char* elementId = luaL_checkstring(L, 2);
	float sizeX = (float)luaL_checknumber(L, 3);
	float sizeY = (float)luaL_checknumber(L, 4);

	bool success = customImGui.SetWindowSize(elementId, sizeX, sizeY);

	if (!success) {
		return luaL_error(L, "Window Element with id '%s' not found", elementId);
	}

	return 0;
}

static void RegisterCustomImGui(lua_State* L)
{
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "GetImGui", Lua_CustomImGui);

	luaL_Reg functions[] = {
			{ "AddCallback", Lua_ImGui_AddCallback },
			{ "RemoveCallback", Lua_ImGui_RemoveCallback },
			{ "AddElement", Lua_ImGui_AddElement },
			{ "RemoveElement", Lua_ImGui_RemoveElement },
			{ "CreateMenu", Lua_ImGui_CreateMenu },
			{ "RemoveMenu", Lua_ImGui_RemoveMenu },
			{ "CreateWindow", Lua_ImGui_CreateWindow },
			{ "RemoveWindow", Lua_ImGui_RemoveWindow },
			{ "LinkWindowToElement", Lua_ImGui_LinkWindowToElement },
			{ "ElementExists", Lua_ImGui_ElementExists },
			{ "UpdateText", Lua_ImGui_UpdateText },
			{ "UpdateData", Lua_ImGui_UpdateData },
			{ "AddButton", Lua_ImGui_AddButton },
			{ "AddText", Lua_ImGui_AddText },
			{ "AddInputInteger", Lua_ImGui_AddInputInteger },
			{ "AddInputFloat", Lua_ImGui_AddInputFloat },
			{ "AddDragInteger", Lua_ImGui_AddDragInteger },
			{ "AddDragFloat", Lua_ImGui_AddDragFloat },
			{ "AddSliderInteger", Lua_ImGui_AddSliderInteger },
			{ "AddSliderFloat", Lua_ImGui_AddSliderFloat },
			{ "AddInputColor", Lua_ImGui_AddInputColor },
			{ "AddCheckbox", Lua_ImGui_AddCheckbox },
			{ "AddRadioButtons", Lua_ImGui_AddRadioButtons },
			{ "AddCombobox", Lua_ImGui_AddCombobox },
			{ "AddInputText", Lua_ImGui_AddInputText },
			{ "AddInputTextMultiline", Lua_ImGui_AddInputTextMultiline },
			{ "AddTabBar", Lua_ImGui_AddTabBar },
			{ "AddTab", Lua_ImGui_AddTab },
			{ "AddInputController", Lua_ImGui_AddInputController },
			{ "AddInputKeyboard", Lua_ImGui_AddInputKeyboard },
			{ "AddPlotLines", Lua_ImGui_AddPlotLines },
			{ "AddPlotHistogram", Lua_ImGui_AddPlotHistogram },
			{ "AddProgressBar", Lua_ImGui_AddProgressBar },
			{ "SetHelpmarker", Lua_ImGui_SetHelpmarker },
			{ "SetColor", Lua_ImGui_SetColor },
			{ "SetTextColor", Lua_ImGui_SetTextColor },
			{ "SetVisible", Lua_ImGui_SetVisible },
			{ "SetWindowPinned", Lua_ImGui_SetWindowPinned },
			{ "SetWindowPosition", Lua_ImGui_SetWindowPosition },
			{ "SetWindowSize", Lua_ImGui_SetWindowSize },
			{ "SetTooltip", Lua_ImGui_SetTooltip },
			{ "RemoveColor", Lua_ImGui_RemoveColor },
			{ "GetMousePosition", Lua_ImGui_GetMousePos },
			{ "GetVisible", Lua_ImGui_GetVisible },
			{ "GetWindowPinned", Lua_ImGui_GetWindowPinned },
			{ "PushNotification", Lua_ImGui_PushNotification },
			{ "Reset", Lua_ImGui_Reset },
			{ "Show", Lua_ImGui_Show },
			{ "Hide", Lua_ImGui_Hide },
			{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::ImGuiMT, lua::metatables::ImGuiMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, ()->void)
{
	super();

	lua::LuaStackProtector protector(_state);

	RegisterCustomImGui(_state);
}
