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
		customImGui.RemoveElement(id);
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
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	int type = (int)luaL_checkinteger(L, 3);
	const char* text = luaL_optstring(L, 4, "");

	EvalIDAndParent(L, id, parentId);

	customImGui.AddElement(parentId, id, text, type);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_RemoveElement)
{
	const char* elementId = luaL_checkstring(L, 1);

	customImGui.RemoveElement(elementId);

	return 0;
}

LUA_FUNCTION(Lua_ImGui_LinkWindowToElement)
{
	const char* windowId = luaL_checkstring(L, 1);
	const char* elementId = luaL_checkstring(L, 2);

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
	const char* id = luaL_checkstring(L, 1);
	const char* text = luaL_checkstring(L, 2);

	if (customImGui.ElementExists(id)) {
		customImGui.RemoveElement(id);
	}

	bool success = customImGui.CreateMenuElement(id, text);
	if (!success) {
		return luaL_error(L, "Error while adding new Menu '%s'", id);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_CreateWindow)
{
	const char* id = luaL_checkstring(L, 1);
	const char* title = luaL_checkstring(L, 2);
	const char* parentId = luaL_optstring(L, 3, nullptr);

	if (customImGui.ElementExists(id)) {
		customImGui.RemoveElement(id);
	}

	bool success = customImGui.CreateWindowElement(id, title, parentId);
	if (!success) {
		return luaL_error(L, "Error while adding new Window '%s'", id);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddCallback)
{
	const char* parentId = luaL_checkstring(L, 1);
	int type = (int)luaL_checkinteger(L, 2);
	if (!lua_isfunction(L, 3)) {
		return luaL_error(L, "Argument %d is not a function", 3);
	}
	int stackID = CheckAndSetCallback(L, 3);

	bool success = customImGui.AddCallback(parentId, type, stackID);
	if (!success) {
		return luaL_error(L, "No element '%s' found.", parentId);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_RemoveCallback)
{
	const char* parentId = luaL_checkstring(L, 1);
	int type = (int)luaL_checkinteger(L, 2);

	bool success = customImGui.RemoveCallback(parentId, type);
	if (!success) {
		return luaL_error(L, "No element '%s' found.", parentId);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_UpdateText)
{
	const char* id = luaL_checkstring(L, 1);
	const char* text = luaL_checkstring(L, 2);

	bool success = customImGui.UpdateText(id, text);
	if (!success) {
		return luaL_error(L, "No element with id '%s' found.", id);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_UpdateData)
{
	// prechecks
	const char* id = luaL_checkstring(L, 1);
	luaL_checkinteger(L, 2);
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
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	bool isSmall = lua::luaL_optboolean(L, 5, false);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::Button);
	if (isSmall)
		type = static_cast<int>(IMGUI_ELEMENT::SmallButton);

	customImGui.AddElement(parentId, id, text, type);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Clicked), stackID);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddText)
{
	const char* parentId = luaL_checkstring(L, 1);
	const char* text = luaL_optstring(L, 2, "");
	bool isWrapped = lua::luaL_optboolean(L, 3, false);
	const char* id = luaL_optstring(L, 4, "");

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
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.setDefaultIntVal((int)luaL_optinteger(L, 5, data.defaultIntVal));
	data.step = (float)luaL_optinteger(L, 6, (int)data.step);
	data.stepFast = (float)luaL_optinteger(L, 7, (int)data.stepFast);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputInt);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputFloat)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.setDefaultFloatVal((float)luaL_optnumber(L, 5, data.defaultFloatVal));
	data.step = (float)luaL_optnumber(L, 6, data.step);
	data.stepFast = (float)luaL_optnumber(L, 7, data.stepFast);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputFloat);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddDragInteger)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.setDefaultIntVal((int)luaL_optinteger(L, 5, data.defaultIntVal));
	data.speed = (float)luaL_optnumber(L, 6, data.speed);
	data.minVal = (float)luaL_optinteger(L, 7, (int)data.minVal);
	data.maxVal = (float)luaL_optinteger(L, 8, (int)data.maxVal);
	data.formatting = luaL_optstring(L, 9, data.DefaultIntNumberFormatting);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::DragInt);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddDragFloat)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.setDefaultFloatVal((float)luaL_optnumber(L, 5, data.defaultFloatVal));
	data.speed = (float)luaL_optnumber(L, 6, data.speed);
	data.minVal = (float)luaL_optnumber(L, 7, data.minVal);
	data.maxVal = (float)luaL_optnumber(L, 8, data.maxVal);
	data.formatting = luaL_optstring(L, 9, data.DefaultFloatNumberFormatting);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::DragFloat);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddSliderInteger)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.setDefaultIntVal((int)luaL_optinteger(L, 5, data.defaultIntVal));
	data.minVal = (float)luaL_optinteger(L, 6, (int)data.minVal);
	data.maxVal = (float)luaL_optinteger(L, 7, (int)data.maxVal);
	data.formatting = luaL_optstring(L, 8, data.DefaultIntNumberFormatting);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::SliderInt);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddSliderFloat)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.setDefaultFloatVal((float)luaL_optnumber(L, 5, data.defaultFloatVal));
	data.minVal = (float)luaL_optnumber(L, 6, data.minVal);
	data.maxVal = (float)luaL_optnumber(L, 7, data.maxVal);
	data.formatting = luaL_optstring(L, 8, data.DefaultFloatNumberFormatting);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::SliderFloat);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputColor)
{
	ColorData data = ColorData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.r = (float)luaL_optnumber(L, 5, 0);
	data.g = (float)luaL_optnumber(L, 6, 0);
	data.b = (float)luaL_optnumber(L, 7, 0);
	data.useAlpha = !lua_isnoneornil(L, 8);
	data.a = (float)luaL_optnumber(L, 8, 1);

	data.init();

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::ColorEdit);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddCheckbox)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.checked = lua::luaL_optboolean(L, 5, false);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::Checkbox);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddRadioButtons)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	int stackID = CheckAndSetCallback(L, 3);
	if (!lua_istable(L, 4)) {
		return luaL_error(L, "Argument 5 needs to be a table!");
	}
	data.index = (int)luaL_optinteger(L, 5, 0);
	data.sameLine = lua::luaL_optboolean(L, 6, true);

	// get table input
	auto tableLength = lua_rawlen(L, 4);
	for (auto i = 1; i <= tableLength; ++i) {
		lua_pushinteger(L, i);
		lua_gettable(L, 4);
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

	if (lua_isfunction(L, 3)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddTabBar)
{
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::TabBar);

	customImGui.AddElement(parentId, id, "", type);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddTab)
{
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_checkstring(L, 3);

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
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	if (!lua_istable(L, 5)) {
		return luaL_error(L, "Argument 6 needs to be a table!");
	}
	data.index = (int)luaL_optinteger(L, 6, 0);
	data.isSlider = lua::luaL_optboolean(L, 7, false);

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

	int type = static_cast<int>(IMGUI_ELEMENT::Combobox);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputText)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.inputText = luaL_optstring(L, 5, "");
	data.hintText = luaL_optstring(L, 6, "");

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputText);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputTextMultiline)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.inputText = luaL_optstring(L, 5, "");
	data.lineCount = (float)luaL_optnumber(L, 6, 6);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputTextMultiline);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_SetTooltip)
{
	const char* id = luaL_checkstring(L, 1);
	const char* text = luaL_checkstring(L, 2);

	bool success = customImGui.SetTooltipText(id, text);
	if (!success) {
		return luaL_error(L, "No element with id '%s' found.", id);
	}

	return 1;
}

LUA_FUNCTION(Lua_ImGui_SetHelpmarker)
{
	const char* id = luaL_checkstring(L, 1);
	const char* text = luaL_checkstring(L, 2);

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

	lua::LuaCaller(L).pushUserdataValue(Vector(x, y), lua::Metatables::VECTOR);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputController)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.currentIntVal = (int)luaL_optinteger(L, 5, 0);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputController);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddInputKeyboard)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	int stackID = CheckAndSetCallback(L, 4);
	data.currentIntVal = (int)luaL_optinteger(L, 5, 0);

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::InputKeyboard);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	if (lua_isfunction(L, 4)) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), stackID);
	}
	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddPlotLines)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	if (!lua_istable(L, 4)) {
		return luaL_error(L, "Argument 5 needs to be a table!");
	}
	data.hintText = luaL_optstring(L, 5, "");
	data.minVal = (float)luaL_optnumber(L, 6, FLT_MIN);
	data.maxVal = (float)luaL_optnumber(L, 7, FLT_MAX);
	float height = (float)luaL_optnumber(L, 8, 40.0f);

	EvalIDAndParent(L, id, parentId);

	// get table input
	auto tableLength = lua_rawlen(L, 4);
	for (auto i = 1; i <= tableLength; ++i) {
		lua_pushinteger(L, i);
		lua_gettable(L, 4);
		if (lua_type(L, -1) == LUA_TNIL)
			break;
		data.plotValues->push_back((float)luaL_checknumber(L, -1));
		lua_pop(L, 1);
	}

	int type = static_cast<int>(IMGUI_ELEMENT::PlotLines);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);
	createdElement->data.size.y = height;

	createdElement->AddData(data);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddPlotHistogram)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	if (!lua_istable(L, 4)) {
		return luaL_error(L, "Argument 5 needs to be a table!");
	}
	data.hintText = luaL_optstring(L, 5, "");
	data.minVal = (float)luaL_optnumber(L, 6, FLT_MIN);
	data.maxVal = (float)luaL_optnumber(L, 7, FLT_MAX);
	float height = (float)luaL_optnumber(L, 8, 40.0f);

	EvalIDAndParent(L, id, parentId);

	// get table input
	auto tableLength = lua_rawlen(L, 4);
	for (auto i = 1; i <= tableLength; ++i) {
		lua_pushinteger(L, i);
		lua_gettable(L, 4);
		if (lua_type(L, -1) == LUA_TNIL)
			break;
		data.plotValues->push_back((float)luaL_checknumber(L, -1));
		lua_pop(L, 1);
	}

	int type = static_cast<int>(IMGUI_ELEMENT::PlotHistogram);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);
	createdElement->data.size.y = height;

	createdElement->AddData(data);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_AddProgressBar)
{
	ElementData data = ElementData();
	const char* parentId = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);
	const char* text = luaL_optstring(L, 3, "");
	data.currentFloatVal = (float)luaL_optnumber(L, 4, 0.0f);
	data.hintText = luaL_optstring(L, 5, "__DEFAULT__"); // special placeholder for default behavior handling

	EvalIDAndParent(L, id, parentId);

	int type = static_cast<int>(IMGUI_ELEMENT::ProgressBar);

	customImGui.AddElement(parentId, id, text, type);
	Element* createdElement = customImGui.GetElementById(id);

	createdElement->AddData(data);

	return 1;
}

LUA_FUNCTION(Lua_ImGui_PushNotification)
{
	const char* text = luaL_checkstring(L, 1);
	int severity = (int)luaL_optinteger(L, 2, 0);
	if (severity < 0 || severity > 3)
		return luaL_error(L, "Severity needs to be a value between 0 and 3");
	int lifetime = (int)luaL_optinteger(L, 3, 5000);

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
	const char* id = luaL_checkstring(L, 1);

	lua_pushboolean(L, customImGui.ElementExists(id));

	return 1;
}

LUA_FUNCTION(Lua_ImGui_GetVisible)
{
	const char* elementId = luaL_checkstring(L, 1);

	if (!customImGui.ElementExists(elementId)) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	lua_pushboolean(L, customImGui.GetVisible(elementId));
	return 1;
}


LUA_FUNCTION(Lua_ImGui_SetVisible)
{
	const char* elementId = luaL_checkstring(L, 1);
	bool newState = lua::luaL_checkboolean(L, 2);

	bool success = customImGui.SetVisible(elementId, newState);

	if (!success) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_SetColor)
{
	const char* elementId = luaL_checkstring(L, 1);
	int type = (int)luaL_checkinteger(L, 2);
	float r = (float)luaL_checknumber(L, 3);
	float g = (float)luaL_checknumber(L, 4);
	float b = (float)luaL_checknumber(L, 5);
	float a = (float)luaL_optnumber(L, 6, 1.0f);

	bool success = customImGui.SetColor(elementId, static_cast<ImGuiCol_>(type), ImVec4(r, g, b, a));

	if (!success) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_RemoveColor)
{
	const char* elementId = luaL_checkstring(L, 1);
	int type = (int)luaL_checkinteger(L, 2);

	bool success = customImGui.RemoveColor(elementId, static_cast<ImGuiCol_>(type));

	if (!success) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_SetTextColor)
{
	const char* elementId = luaL_checkstring(L, 1);
	float r = (float)luaL_checknumber(L, 2);
	float g = (float)luaL_checknumber(L, 3);
	float b = (float)luaL_checknumber(L, 4);
	float a = (float)luaL_optnumber(L, 5, 1.0f);

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
	const char* elementId = luaL_checkstring(L, 1);
	bool newState = lua::luaL_checkboolean(L, 2);

	bool success = customImGui.SetPinned(elementId, newState);

	if (!success) {
		return luaL_error(L, "Window Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_GetWindowFlags)
{
	const char* elementId = luaL_checkstring(L, 1);

	Element* element = customImGui.GetElementById(elementId);
	if (element != NULL && element->type == IMGUI_ELEMENT::Window) {
		lua_pushinteger(L, element->data.windowFlags);
		return 1;
	}
	else {
		return luaL_error(L, "Window Element with id '%s' not found", elementId);
	}
}

LUA_FUNCTION(Lua_ImGui_SetWindowFlags)
{
	const char* elementId = luaL_checkstring(L, 1);
	ImGuiWindowFlags newFlags = (ImGuiWindowFlags)luaL_checkinteger(L, 2);


	bool success = customImGui.SetWindowFlags(elementId, newFlags);

	if (!success) {
		return luaL_error(L, "Window Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_GetWindowChildFlags)
{
	const char* elementId = luaL_checkstring(L, 1);

	Element* element = customImGui.GetElementById(elementId);
	if (element != NULL && element->type == IMGUI_ELEMENT::Window) {
		lua_pushinteger(L, element->data.childFlags);
		return 1;
	}
	else {
		return luaL_error(L, "Window Element with id '%s' not found", elementId);
	}
}

LUA_FUNCTION(Lua_ImGui_SetWindowChildFlags)
{
	const char* elementId = luaL_checkstring(L, 1);
	ImGuiChildFlags newFlags = (ImGuiChildFlags)luaL_checkinteger(L, 2);


	bool success = customImGui.SetWindowChildFlags(elementId, newFlags);

	if (!success) {
		return luaL_error(L, "Window Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_SetWindowPosition)
{
	const char* elementId = luaL_checkstring(L, 1);
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);

	bool success = customImGui.SetWindowPosition(elementId, x, y);

	if (!success) {
		return luaL_error(L, "Window Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_SetSize)
{
	const char* elementId = luaL_checkstring(L, 1);
	float sizeX = (float)luaL_checknumber(L, 2);
	float sizeY = (float)luaL_checknumber(L, 3);

	bool success = customImGui.SetElementSize(elementId, sizeX, sizeY);

	if (!success) {
		return luaL_error(L, "Element with id '%s' not found", elementId);
	}

	return 0;
}

LUA_FUNCTION(Lua_ImGui_IsVisible)
{
	lua_pushboolean(L, menuShown);
	return 1;
}

static void RegisterCustomImGui(lua_State* L)
{
	//lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "GetImGui", Lua_CustomImGui);

	//luaL_Reg functions[] = {
	lua_newtable(L);
			lua::TableAssoc(L, "AddCallback", Lua_ImGui_AddCallback );
			lua::TableAssoc(L, "RemoveCallback", Lua_ImGui_RemoveCallback );
			lua::TableAssoc(L, "AddElement", Lua_ImGui_AddElement );
			lua::TableAssoc(L, "RemoveElement", Lua_ImGui_RemoveElement );
			lua::TableAssoc(L, "CreateMenu", Lua_ImGui_CreateMenu );
			lua::TableAssoc(L, "RemoveMenu", Lua_ImGui_RemoveElement); // deprecated. now its an alias of RemoveElement
			lua::TableAssoc(L, "CreateWindow", Lua_ImGui_CreateWindow );
			lua::TableAssoc(L, "RemoveWindow", Lua_ImGui_RemoveElement); // deprecated. now its an alias of RemoveElement
			lua::TableAssoc(L, "LinkWindowToElement", Lua_ImGui_LinkWindowToElement );
			lua::TableAssoc(L, "ElementExists", Lua_ImGui_ElementExists );
			lua::TableAssoc(L, "UpdateText", Lua_ImGui_UpdateText );
			lua::TableAssoc(L, "UpdateData", Lua_ImGui_UpdateData );
			lua::TableAssoc(L, "AddButton", Lua_ImGui_AddButton );
			lua::TableAssoc(L, "AddText", Lua_ImGui_AddText );
			lua::TableAssoc(L, "AddInputInteger", Lua_ImGui_AddInputInteger );
			lua::TableAssoc(L, "AddInputFloat", Lua_ImGui_AddInputFloat );
			lua::TableAssoc(L, "AddDragInteger", Lua_ImGui_AddDragInteger );
			lua::TableAssoc(L, "AddDragFloat", Lua_ImGui_AddDragFloat );
			lua::TableAssoc(L, "AddSliderInteger", Lua_ImGui_AddSliderInteger );
			lua::TableAssoc(L, "AddSliderFloat", Lua_ImGui_AddSliderFloat );
			lua::TableAssoc(L, "AddInputColor", Lua_ImGui_AddInputColor );
			lua::TableAssoc(L, "AddCheckbox", Lua_ImGui_AddCheckbox );
			lua::TableAssoc(L, "AddRadioButtons", Lua_ImGui_AddRadioButtons );
			lua::TableAssoc(L, "AddCombobox", Lua_ImGui_AddCombobox );
			lua::TableAssoc(L, "AddInputText", Lua_ImGui_AddInputText );
			lua::TableAssoc(L, "AddInputTextMultiline", Lua_ImGui_AddInputTextMultiline );
			lua::TableAssoc(L, "AddTabBar", Lua_ImGui_AddTabBar );
			lua::TableAssoc(L, "AddTab", Lua_ImGui_AddTab );
			lua::TableAssoc(L, "AddInputController", Lua_ImGui_AddInputController );
			lua::TableAssoc(L, "AddInputKeyboard", Lua_ImGui_AddInputKeyboard );
			lua::TableAssoc(L, "AddPlotLines", Lua_ImGui_AddPlotLines );
			lua::TableAssoc(L, "AddPlotHistogram", Lua_ImGui_AddPlotHistogram );
			lua::TableAssoc(L, "AddProgressBar", Lua_ImGui_AddProgressBar );
			lua::TableAssoc(L, "SetHelpmarker", Lua_ImGui_SetHelpmarker );
			lua::TableAssoc(L, "SetColor", Lua_ImGui_SetColor );
			lua::TableAssoc(L, "SetSize", Lua_ImGui_SetSize );
			lua::TableAssoc(L, "SetTextColor", Lua_ImGui_SetTextColor );
			lua::TableAssoc(L, "SetVisible", Lua_ImGui_SetVisible );
			lua::TableAssoc(L, "SetWindowPinned", Lua_ImGui_SetWindowPinned );
			lua::TableAssoc(L, "SetWindowFlags", Lua_ImGui_SetWindowFlags);
			lua::TableAssoc(L, "SetWindowChildFlags", Lua_ImGui_SetWindowChildFlags);
			lua::TableAssoc(L, "SetWindowPosition", Lua_ImGui_SetWindowPosition );
			lua::TableAssoc(L, "SetWindowSize", Lua_ImGui_SetSize ); // deprecated. now its an alias of SetSize
			lua::TableAssoc(L, "SetTooltip", Lua_ImGui_SetTooltip );
			lua::TableAssoc(L, "RemoveColor", Lua_ImGui_RemoveColor );
			lua::TableAssoc(L, "GetMousePosition", Lua_ImGui_GetMousePos );
			lua::TableAssoc(L, "GetVisible", Lua_ImGui_GetVisible );
			lua::TableAssoc(L, "GetWindowPinned", Lua_ImGui_GetWindowPinned );
			lua::TableAssoc(L, "GetWindowFlags", Lua_ImGui_GetWindowFlags);
			lua::TableAssoc(L, "GetWindowChildFlags", Lua_ImGui_GetWindowChildFlags);
			lua::TableAssoc(L, "PushNotification", Lua_ImGui_PushNotification );
			lua::TableAssoc(L, "Reset", Lua_ImGui_Reset );
			lua::TableAssoc(L, "Show", Lua_ImGui_Show );
			lua::TableAssoc(L, "Hide", Lua_ImGui_Hide );
			lua::TableAssoc(L, "IsVisible", Lua_ImGui_IsVisible);
			//{ NULL, NULL }
	//};
	lua_setglobal(L, "ImGui");
	//lua::RegisterNewClass(L, lua::metatables::ImGuiMT, lua::metatables::ImGuiMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, ()->void)
{
	super();

	lua::LuaStackProtector protector(_state);

	RegisterCustomImGui(_state);
}
