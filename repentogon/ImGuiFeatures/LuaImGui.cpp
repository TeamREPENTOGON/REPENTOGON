#include "CustomImGui.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../LuaClasses.h"
#include "NotificationHandler.h"
#include "MultiViewportEnhanced.h"
#include "../REPENTOGONOptions.h"

extern CustomImGui customImGui;
extern NotificationHandler notificationHandler;

extern bool menuShown;

static bool PrepareElement(const char* id, const char* parentId) {
	if (!customImGui.ElementExists(parentId))
		return false;
	if (customImGui.ElementExists(id))
		customImGui.RemoveElement(id);
	return true;
}

static Element* CreateElement(const char* parentId, const char* id, const char* text, IMGUI_ELEMENT type) {
	customImGui.AddElement(parentId, id, text, static_cast<int>(type));
	return customImGui.GetElementById(id);
}

static void SetEditedCallback(const char* id, int callbackRef) {
	if (callbackRef != 0) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Edited), callbackRef);
	}
}

static Element* GetWindow(const char* id) {
	Element* element = customImGui.GetElementById(id);
	if (element != nullptr && element->type == IMGUI_ELEMENT::Window) {
		return element;
	}
	return nullptr;
}

// Turns a Lua function into a registry reference for the element callbacks.
LUA_FUNCTION(Lua_ImGui_Ref)
{
	luaL_checktype(L, 1, LUA_TFUNCTION);
	lua_pushvalue(L, 1);
	lua_pushinteger(L, luaL_ref(L, LUA_REGISTRYINDEX));
	return 1;
}


MOD_EXPORT bool L_ImGui_AddElement(const char* parentId, const char* id, int type, const char* text) {
	if (!PrepareElement(id, parentId))
		return false;
	customImGui.AddElement(parentId, id, text, type);
	return true;
}

MOD_EXPORT void L_ImGui_RemoveElement(const char* id) {
	customImGui.RemoveElement(id);
}

// 0: success, 1: no window, 2: no element
MOD_EXPORT int L_ImGui_LinkWindowToElement(const char* windowId, const char* elementId) {
	if (customImGui.GetElementById(windowId) == nullptr)
		return 1;
	return customImGui.LinkWindowToElement(windowId, elementId) ? 0 : 2;
}

MOD_EXPORT bool L_ImGui_CreateMenu(const char* id, const char* text) {
	if (customImGui.ElementExists(id))
		customImGui.RemoveElement(id);
	return customImGui.CreateMenuElement(id, text);
}

MOD_EXPORT bool L_ImGui_CreateWindow(const char* id, const char* title, const char* parentId) {
	if (customImGui.ElementExists(id))
		customImGui.RemoveElement(id);
	return customImGui.CreateWindowElement(id, title, parentId);
}

MOD_EXPORT bool L_ImGui_AddCallback(const char* parentId, int type, int callbackRef) {
	return customImGui.AddCallback(parentId, type, callbackRef);
}

MOD_EXPORT bool L_ImGui_RemoveCallback(const char* parentId, int type) {
	return customImGui.RemoveCallback(parentId, type);
}

MOD_EXPORT bool L_ImGui_UpdateText(const char* id, const char* text) {
	return customImGui.UpdateText(id, text);
}

MOD_EXPORT bool L_ImGui_ElementExists(const char* id) {
	return customImGui.ElementExists(id);
}

// 0: unsupported, 1: string, 2: boolean, 3: integer, 4: float
MOD_EXPORT int L_ImGui_GetValueKind(const char* id) {
	Element* element = customImGui.GetElementById(id);
	switch (element->type) {
	case IMGUI_ELEMENT::InputText:
	case IMGUI_ELEMENT::InputTextWithHint:
	case IMGUI_ELEMENT::InputTextMultiline:
		return 1;
	case IMGUI_ELEMENT::Checkbox:
		return 2;
	case IMGUI_ELEMENT::RadioButton:
	case IMGUI_ELEMENT::Combobox:
	case IMGUI_ELEMENT::InputInt:
	case IMGUI_ELEMENT::DragInt:
	case IMGUI_ELEMENT::SliderInt:
	case IMGUI_ELEMENT::InputController:
	case IMGUI_ELEMENT::InputKeyboard:
		return 3;
	case IMGUI_ELEMENT::InputFloat:
	case IMGUI_ELEMENT::DragFloat:
	case IMGUI_ELEMENT::SliderFloat:
	case IMGUI_ELEMENT::ProgressBar:
		return 4;
	default:
		return 0;
	}
}

MOD_EXPORT void L_ImGui_SetValueString(const char* id, const char* value) {
	customImGui.GetElementById(id)->elementData.inputText = value;
}

MOD_EXPORT void L_ImGui_SetValueBoolean(const char* id, bool value) {
	customImGui.GetElementById(id)->elementData.checked = value;
}

MOD_EXPORT void L_ImGui_SetValueInteger(const char* id, int value) {
	Element* element = customImGui.GetElementById(id);
	if (element->type == IMGUI_ELEMENT::RadioButton || element->type == IMGUI_ELEMENT::Combobox) {
		element->elementData.index = value;
	}
	else {
		element->elementData.currentIntVal = value;
	}
}

MOD_EXPORT void L_ImGui_SetValueFloat(const char* id, float value) {
	customImGui.GetElementById(id)->elementData.currentFloatVal = value;
}

MOD_EXPORT void L_ImGui_SetLabel(const char* id, const char* label) {
	customImGui.GetElementById(id)->name = label;
}

MOD_EXPORT bool L_ImGui_SetHintText(const char* id, const char* text) {
	Element* element = customImGui.GetElementById(id);
	if (element->type != IMGUI_ELEMENT::InputText
		&& element->type != IMGUI_ELEMENT::InputTextWithHint
		&& element->type != IMGUI_ELEMENT::PlotLines
		&& element->type != IMGUI_ELEMENT::PlotHistogram
		&& element->type != IMGUI_ELEMENT::ProgressBar)
		return false;
	element->elementData.hintText = text;
	return true;
}

MOD_EXPORT bool L_ImGui_SetMinMax(const char* id, bool isMax, float value) {
	Element* element = customImGui.GetElementById(id);
	switch (element->type) {
	case IMGUI_ELEMENT::DragInt:
	case IMGUI_ELEMENT::SliderInt:
	case IMGUI_ELEMENT::DragFloat:
	case IMGUI_ELEMENT::SliderFloat:
		(isMax ? element->elementData.maxVal : element->elementData.minVal) = value;
		return true;
	default:
		return false;
	}
}

MOD_EXPORT bool L_ImGui_IsPlot(const char* id) {
	Element* element = customImGui.GetElementById(id);
	return element->type == IMGUI_ELEMENT::PlotLines || element->type == IMGUI_ELEMENT::PlotHistogram;
}

MOD_EXPORT void L_ImGui_SetListStrings(const char* id, const char** values, int count) {
	Element* element = customImGui.GetElementById(id);
	element->elementData.plotValues->clear();
	element->elementData.values->clear();
	for (int i = 0; i < count; ++i) {
		element->elementData.values->push_back(values[i]);
	}
}

MOD_EXPORT void L_ImGui_SetListNumbers(const char* id, const float* values, int count) {
	Element* element = customImGui.GetElementById(id);
	element->elementData.plotValues->clear();
	element->elementData.values->clear();
	for (int i = 0; i < count; ++i) {
		element->elementData.plotValues->push_back(values[i]);
	}
}

MOD_EXPORT bool L_ImGui_SetColorValues(const char* id, const float* values, int count) {
	Element* element = customImGui.GetElementById(id);
	if (element->type != IMGUI_ELEMENT::ColorEdit)
		return false;
	ColorData& color = element->colorData;
	color.useAlpha = count > 3;
	color.r = count > 0 ? values[0] : 0.0f;
	color.g = count > 1 ? values[1] : 0.0f;
	color.b = count > 2 ? values[2] : 0.0f;
	color.a = count > 3 ? values[3] : 1.0f;
	color.init();
	return true;
}

MOD_EXPORT bool L_ImGui_AddButton(const char* parentId, const char* id, const char* text, int callbackRef, bool isSmall) {
	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, isSmall ? IMGUI_ELEMENT::SmallButton : IMGUI_ELEMENT::Button);
	if (callbackRef != 0) {
		customImGui.AddCallback(id, static_cast<int>(IMGUI_CALLBACK::Clicked), callbackRef);
	}
	return true;
}

MOD_EXPORT bool L_ImGui_AddText(const char* parentId, const char* text, bool isWrapped, const char* id) {
	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, isWrapped ? IMGUI_ELEMENT::TextWrapped : IMGUI_ELEMENT::Text);
	return true;
}

MOD_EXPORT bool L_ImGui_AddInputInteger(const char* parentId, const char* id, const char* text, int callbackRef, int defaultVal, int step, int stepFast) {
	ElementData data = ElementData();
	data.setDefaultIntVal(defaultVal);
	data.step = (float)step;
	data.stepFast = (float)stepFast;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::InputInt)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddInputFloat(const char* parentId, const char* id, const char* text, int callbackRef, float defaultVal, float step, float stepFast) {
	ElementData data = ElementData();
	data.setDefaultFloatVal(defaultVal);
	data.step = step;
	data.stepFast = stepFast;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::InputFloat)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddDragInteger(const char* parentId, const char* id, const char* text, int callbackRef, int defaultVal, float speed, int minVal, int maxVal, const char* formatting) {
	ElementData data = ElementData();
	data.setDefaultIntVal(defaultVal);
	data.speed = speed;
	data.minVal = (float)minVal;
	data.maxVal = (float)maxVal;
	data.formatting = formatting;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::DragInt)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddDragFloat(const char* parentId, const char* id, const char* text, int callbackRef, float defaultVal, float speed, float minVal, float maxVal, const char* formatting) {
	ElementData data = ElementData();
	data.setDefaultFloatVal(defaultVal);
	data.speed = speed;
	data.minVal = minVal;
	data.maxVal = maxVal;
	data.formatting = formatting;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::DragFloat)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddSliderInteger(const char* parentId, const char* id, const char* text, int callbackRef, int defaultVal, int minVal, int maxVal, const char* formatting) {
	ElementData data = ElementData();
	data.setDefaultIntVal(defaultVal);
	data.minVal = (float)minVal;
	data.maxVal = (float)maxVal;
	data.formatting = formatting;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::SliderInt)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddSliderFloat(const char* parentId, const char* id, const char* text, int callbackRef, float defaultVal, float minVal, float maxVal, const char* formatting) {
	ElementData data = ElementData();
	data.setDefaultFloatVal(defaultVal);
	data.minVal = minVal;
	data.maxVal = maxVal;
	data.formatting = formatting;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::SliderFloat)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddInputColor(const char* parentId, const char* id, const char* text, int callbackRef, float r, float g, float b, bool useAlpha, float a) {
	ColorData data = ColorData();
	data.r = r;
	data.g = g;
	data.b = b;
	data.useAlpha = useAlpha;
	data.a = a;
	data.init();

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::ColorEdit)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddCheckbox(const char* parentId, const char* id, const char* text, int callbackRef, bool checked) {
	ElementData data = ElementData();
	data.checked = checked;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::Checkbox)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddRadioButtons(const char* parentId, const char* id, int callbackRef, const char** values, int count, int index, bool sameLine) {
	ElementData data = ElementData();
	data.index = index;
	data.sameLine = sameLine;
	for (int i = 0; i < count; ++i) {
		data.values->push_back(values[i]);
	}

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, "", IMGUI_ELEMENT::RadioButton)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddTabBar(const char* parentId, const char* id) {
	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, "", IMGUI_ELEMENT::TabBar);
	return true;
}

// 0: success, 1: no parent, 2: parent isn't a TabBar
MOD_EXPORT int L_ImGui_AddTab(const char* parentId, const char* id, const char* text) {
	if (!PrepareElement(id, parentId))
		return 1;
	if (customImGui.GetElementById(parentId)->type != IMGUI_ELEMENT::TabBar)
		return 2;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::Tab);
	return 0;
}

MOD_EXPORT bool L_ImGui_AddCombobox(const char* parentId, const char* id, const char* text, int callbackRef, const char** values, int count, int index, bool isSlider) {
	ElementData data = ElementData();
	data.index = index;
	data.isSlider = isSlider;
	for (int i = 0; i < count; ++i) {
		data.values->push_back(values[i]);
	}

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::Combobox)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddInputText(const char* parentId, const char* id, const char* text, int callbackRef, const char* inputText, const char* hintText) {
	ElementData data = ElementData();
	data.inputText = inputText;
	data.hintText = hintText;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::InputText)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddInputTextMultiline(const char* parentId, const char* id, const char* text, int callbackRef, const char* inputText, float lineCount) {
	ElementData data = ElementData();
	data.inputText = inputText;
	data.lineCount = lineCount;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::InputTextMultiline)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

static bool AddInputBinding(const char* parentId, const char* id, const char* text, int callbackRef, int defaultVal, IMGUI_ELEMENT type) {
	ElementData data = ElementData();
	data.currentIntVal = defaultVal;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, type)->AddData(data);
	SetEditedCallback(id, callbackRef);
	return true;
}

MOD_EXPORT bool L_ImGui_AddInputController(const char* parentId, const char* id, const char* text, int callbackRef, int defaultVal) {
	return AddInputBinding(parentId, id, text, callbackRef, defaultVal, IMGUI_ELEMENT::InputController);
}

MOD_EXPORT bool L_ImGui_AddInputKeyboard(const char* parentId, const char* id, const char* text, int callbackRef, int defaultVal) {
	return AddInputBinding(parentId, id, text, callbackRef, defaultVal, IMGUI_ELEMENT::InputKeyboard);
}

static bool AddPlot(const char* parentId, const char* id, const char* text, const float* values, int count, const char* hintText, float minVal, float maxVal, float height, IMGUI_ELEMENT type) {
	ElementData data = ElementData();
	data.hintText = hintText;
	data.minVal = minVal;
	data.maxVal = maxVal;

	if (!PrepareElement(id, parentId))
		return false;
	for (int i = 0; i < count; ++i) {
		data.plotValues->push_back(values[i]);
	}
	Element* createdElement = CreateElement(parentId, id, text, type);
	createdElement->data.size.y = height;
	createdElement->AddData(data);
	return true;
}

MOD_EXPORT bool L_ImGui_AddPlotLines(const char* parentId, const char* id, const char* text, const float* values, int count, const char* hintText, float minVal, float maxVal, float height) {
	return AddPlot(parentId, id, text, values, count, hintText, minVal, maxVal, height, IMGUI_ELEMENT::PlotLines);
}

MOD_EXPORT bool L_ImGui_AddPlotHistogram(const char* parentId, const char* id, const char* text, const float* values, int count, const char* hintText, float minVal, float maxVal, float height) {
	return AddPlot(parentId, id, text, values, count, hintText, minVal, maxVal, height, IMGUI_ELEMENT::PlotHistogram);
}

MOD_EXPORT bool L_ImGui_AddProgressBar(const char* parentId, const char* id, const char* text, float value, const char* hintText) {
	ElementData data = ElementData();
	data.currentFloatVal = value;
	data.hintText = hintText;

	if (!PrepareElement(id, parentId))
		return false;
	CreateElement(parentId, id, text, IMGUI_ELEMENT::ProgressBar)->AddData(data);
	return true;
}

MOD_EXPORT bool L_ImGui_SetTooltip(const char* id, const char* text) {
	return customImGui.SetTooltipText(id, text);
}

MOD_EXPORT bool L_ImGui_SetHelpmarker(const char* id, const char* text) {
	return customImGui.SetHelpMarkerText(id, text);
}

MOD_EXPORT bool L_ImGui_GetVisible(const char* id) {
	return customImGui.GetVisible(id);
}

MOD_EXPORT bool L_ImGui_SetVisible(const char* id, bool visible) {
	return customImGui.SetVisible(id, visible);
}

MOD_EXPORT bool L_ImGui_SetColor(const char* id, int type, float r, float g, float b, float a) {
	return customImGui.SetColor(id, static_cast<ImGuiCol_>(type), ImVec4(r, g, b, a));
}

MOD_EXPORT bool L_ImGui_RemoveColor(const char* id, int type) {
	return customImGui.RemoveColor(id, static_cast<ImGuiCol_>(type));
}

MOD_EXPORT bool L_ImGui_SetTextColor(const char* id, float r, float g, float b, float a) {
	return customImGui.SetColor(id, ImGuiCol_Text, ImVec4(r, g, b, a));
}

MOD_EXPORT bool L_ImGui_SetSize(const char* id, float x, float y) {
	return customImGui.SetElementSize(id, x, y);
}

// -1: not a window
MOD_EXPORT int L_ImGui_GetWindowPinned(const char* id) {
	Element* window = GetWindow(id);
	return window ? (int)window->data.windowPinned : -1;
}

MOD_EXPORT bool L_ImGui_SetWindowPinned(const char* id, bool pinned) {
	return customImGui.SetPinned(id, pinned);
}

MOD_EXPORT bool L_ImGui_GetWindowFlags(const char* id, int* flags) {
	Element* window = GetWindow(id);
	if (!window)
		return false;
	*flags = window->data.windowFlags;
	return true;
}

MOD_EXPORT bool L_ImGui_SetWindowFlags(const char* id, int flags) {
	return customImGui.SetWindowFlags(id, (ImGuiWindowFlags)flags);
}

MOD_EXPORT bool L_ImGui_GetWindowChildFlags(const char* id, int* flags) {
	Element* window = GetWindow(id);
	if (!window)
		return false;
	*flags = window->data.childFlags;
	return true;
}

MOD_EXPORT bool L_ImGui_SetWindowChildFlags(const char* id, int flags) {
	return customImGui.SetWindowChildFlags(id, (ImGuiChildFlags)flags);
}

MOD_EXPORT bool L_ImGui_SetWindowPosition(const char* id, float x, float y) {
	RECT rect = { 0,0,0,0 };
	if ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) && GetWindowRect(rgonImGuiMultiViewportConfig.mainGameWindowForCreateImGuiWindow, &rect)) {
		// when viewports enabled, the position is relative to monitor, so we fix it
		// when viewports disabled, the position is relative to the game window, that's fine
		x += rect.left;
		y += rect.top;
	}

	return customImGui.SetWindowPosition(id, x, y);
}

MOD_EXPORT void L_ImGui_GetMousePosition(Vector* out) {
	float x = -1;
	float y = -1;

	if (menuShown) {
		const ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsMousePosValid()) {
			x = io.MousePos.x;
			y = io.MousePos.y;

			RECT rect = { 0,0,0,0 };
			if (GetWindowRect(rgonImGuiMultiViewportConfig.mainGameWindowForCreateImGuiWindow, &rect)) {
				x -= rect.left;
				y -= rect.top;
			}
		}
	}
	else {
		x = (float)*(double*)(g_KAGEInputController + 0x48);
		y = (float)*(double*)(g_KAGEInputController + 0x50);
	}

	*out = Vector(x, y);
}

MOD_EXPORT void L_ImGui_GetGameWindowRect(Vector* position, Vector* size) {
	RECT rect = { 0,0,0,0 };
	GetWindowRect(rgonImGuiMultiViewportConfig.mainGameWindowForCreateImGuiWindow, &rect);
	*position = Vector((float)rect.left, (float)rect.top);
	*size = Vector((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
}

MOD_EXPORT void L_ImGui_PushNotification(const char* text, int severity, int lifetime) {
	notificationHandler.AddNotification(text, severity, lifetime);
}

MOD_EXPORT void L_ImGui_Show() {
	menuShown = true;
}

MOD_EXPORT void L_ImGui_Hide() {
	menuShown = false;
}

MOD_EXPORT bool L_ImGui_IsVisible() {
	return menuShown;
}

MOD_EXPORT void L_ImGui_Reset() {
	customImGui.Reset();
}

HOOK_METHOD(LuaEngine, RegisterClasses, ()->void)
{
	lua_register(_state, "__Lua_ImGui_Ref", Lua_ImGui_Ref);

	super();
}
