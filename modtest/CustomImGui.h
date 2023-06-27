#include "imgui.h"
#include <IsaacRepentance.h>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "LuaCore.h"

extern int handleWindowFlags(int flags);
extern ImGuiKey AddChangeKeyButton(bool isController, bool& wasPressed);
extern void AddWindowContextMenu(bool* pinned);
extern void HelpMarker(const char* desc);

enum class IMGUI_ELEMENT {
    Window,
    Menu,
    MenuItem,
    Popup,
    CollapsingHeader,
    TreeNode,
    Separator,
    SeparatorText,
    Text,
    BulletText,
    SameLine,
    Button,
    SmallButton,
    InputInt,
    InputFloat,
    DragInt,
    DragFloat,
    SliderInt,
    SliderFloat,
    ColorEdit,
    TabBar,
    Tab,
    Checkbox,
    RadioButton,
    Combobox,
    InputText,
    InputTextWithHint,
    InputTextMultiline,
    InputController,
    InputKeyboard
};

enum class IMGUI_CALLBACK {
    Clicked,
    Hovered,
    Active,
    Focused,
    Visible,
    Edited,
    Activated,
    Deactivated,
    DeactivatedAfterEdit,
    ToggledOpen,
    Render
};

enum class IMGUI_DATA {
    Label,
    Value,
    ListValues,
    Min,
    Max,
    HintText,
    ColorValues,
};

static const char* IGNORE_ID = "IGNORE_THIS_ELEMENT";
static const std::map<ImGuiKey, int> imGuiToIsaacKey {
    { ImGuiKey_Tab, 258 }, { ImGuiKey_LeftArrow, 263 }, { ImGuiKey_RightArrow, 262 }, { ImGuiKey_UpArrow, 265 }, { ImGuiKey_DownArrow, 264 },
    { ImGuiKey_PageUp, 266 }, { ImGuiKey_PageDown, 267 }, { ImGuiKey_Home, 268 }, { ImGuiKey_End, 269 }, { ImGuiKey_Insert, 260 },
    { ImGuiKey_Delete, 261 }, { ImGuiKey_Backspace, 259 }, { ImGuiKey_Space, 32 }, { ImGuiKey_Enter, 257 }, { ImGuiKey_Escape, 256 },
    { ImGuiKey_LeftCtrl, 341 }, { ImGuiKey_LeftShift, 340 }, { ImGuiKey_LeftAlt, 342 }, { ImGuiKey_LeftSuper, 343 }, { ImGuiKey_RightCtrl, 345 },
    { ImGuiKey_RightShift, 344 }, { ImGuiKey_RightAlt, 346 }, { ImGuiKey_RightSuper, 347 }, { ImGuiKey_Menu, 348 }, { ImGuiKey_0, 48 },
    { ImGuiKey_1, 49 }, { ImGuiKey_2, 50 }, { ImGuiKey_3, 51 }, { ImGuiKey_4, 52 }, { ImGuiKey_5, 53 }, { ImGuiKey_6, 54 }, { ImGuiKey_7, 55 },
    { ImGuiKey_8, 56 }, { ImGuiKey_9, 57 }, { ImGuiKey_A, 65 }, { ImGuiKey_B, 66 }, { ImGuiKey_C, 67 }, { ImGuiKey_D, 68 }, { ImGuiKey_E, 69 }, { ImGuiKey_F, 70 },
    { ImGuiKey_G, 71 }, { ImGuiKey_H, 72 }, { ImGuiKey_I, 73 }, { ImGuiKey_J, 74 }, { ImGuiKey_K, 75 }, { ImGuiKey_L, 76 }, { ImGuiKey_M, 77 }, { ImGuiKey_N, 78 },
    { ImGuiKey_O, 79 }, { ImGuiKey_P, 80 }, { ImGuiKey_Q, 81 }, { ImGuiKey_R, 82 }, { ImGuiKey_S, 83 }, { ImGuiKey_T, 84 }, { ImGuiKey_U, 85 }, { ImGuiKey_V, 86 },
    { ImGuiKey_W, 87 }, { ImGuiKey_X, 88 }, { ImGuiKey_Y, 89 }, { ImGuiKey_Z, 90 }, { ImGuiKey_F1, 290 }, { ImGuiKey_F2, 291 }, { ImGuiKey_F3, 292 }, { ImGuiKey_F4, 293 },
    { ImGuiKey_F5, 294 }, { ImGuiKey_F6, 295 }, { ImGuiKey_F7, 296 }, { ImGuiKey_F8, 297 }, { ImGuiKey_F9, 298 }, { ImGuiKey_F10, 299 }, { ImGuiKey_F11, 300 },
    { ImGuiKey_F12, 301 }, { ImGuiKey_Apostrophe, 39 }, { ImGuiKey_Comma, 44 }, { ImGuiKey_Minus, 45 }, { ImGuiKey_Period, 46 }, { ImGuiKey_Slash, 47 },
    { ImGuiKey_Semicolon, 59 }, { ImGuiKey_Equal, 61 }, { ImGuiKey_LeftBracket, 91 }, { ImGuiKey_Backslash, 92 }, { ImGuiKey_RightBracket, 93 },
    { ImGuiKey_GraveAccent, 96 }, { ImGuiKey_CapsLock, 280 }, { ImGuiKey_ScrollLock, 281 }, { ImGuiKey_NumLock, 282 }, { ImGuiKey_PrintScreen, 283 },
    { ImGuiKey_Pause, 284 }, { ImGuiKey_Keypad0, 320 }, { ImGuiKey_Keypad1, 321 }, { ImGuiKey_Keypad2, 322 }, { ImGuiKey_Keypad3, 323 }, { ImGuiKey_Keypad4, 324 },
    { ImGuiKey_Keypad5, 325 }, { ImGuiKey_Keypad6, 326 }, { ImGuiKey_Keypad7, 327 }, { ImGuiKey_Keypad8, 328 }, { ImGuiKey_Keypad9, 329 },
    { ImGuiKey_KeypadDecimal, 330 }, { ImGuiKey_KeypadDivide, 331 }, { ImGuiKey_KeypadMultiply, 332 }, { ImGuiKey_KeypadSubtract, 333 }, { ImGuiKey_KeypadAdd, 334 },
    { ImGuiKey_KeypadEnter, 335 }, { ImGuiKey_KeypadEqual, 336 },
    // Controller
    { ImGuiKey_GamepadLStickLeft, 0 }, { ImGuiKey_GamepadDpadLeft, 0 }, // ACTION_LEFT
    { ImGuiKey_GamepadLStickRight, 1 }, { ImGuiKey_GamepadDpadRight, 1 }, // ACTION_RIGHT
    { ImGuiKey_GamepadLStickUp, 2 }, { ImGuiKey_GamepadDpadUp, 2 }, // ACTION_UP
    { ImGuiKey_GamepadLStickDown, 3 }, { ImGuiKey_GamepadDpadDown, 3 }, // ACTION_DOWN
    { ImGuiKey_GamepadRStickLeft, 4 }, { ImGuiKey_GamepadFaceLeft, 4 }, // ACTION_SHOOTLEFT
    { ImGuiKey_GamepadRStickRight, 5 }, { ImGuiKey_GamepadFaceRight, 5 }, // ACTION_SHOOTRIGHT
    { ImGuiKey_GamepadRStickUp, 6 }, { ImGuiKey_GamepadFaceUp, 6 }, // ACTION_SHOOTUP
    { ImGuiKey_GamepadRStickDown, 7 }, { ImGuiKey_GamepadFaceDown, 7 }, // ACTION_SHOOTDOWN
    { ImGuiKey_GamepadL1, 8 }, // ACTION_BOMB
    { ImGuiKey_GamepadL2, 9 }, // ACTION_ITEM
    { ImGuiKey_GamepadR1, 10 }, // ACTION_PILLCARD
    { ImGuiKey_GamepadR2, 11 }, // ACTION_DROP
    { ImGuiKey_GamepadStart, 12 }, // ACTION_PAUSE
    { ImGuiKey_GamepadBack, 13 }, // ACTION_MAP
};

struct Data {
    int clickCounter = 0;
    std::string tooltipText = "";
    std::string helpmarkerText = "";
    bool windowPinned = false;
};

struct MiscData : Data {
    bool checked = false; // used for Checkbox
    int index = 0; // used for RadioButtons
    bool sameLine = true; // used for RadioButtons
    bool isSlider = false; // used for switch from Combobox to Enum Slider
    std::list<std::string>* values = new std::list<std::string>(); // used for RadioButtons and Combobox
    std::string inputText = ""; // Used for Text input
    std::string hintText = ""; // Used for Text input
    int defaultKeyboardKey = 0;
    float lineCount = 6;

    const char* getCurrentValue()
    {
        auto curValue = values->begin();
        std::advance(curValue, index); // get element at index
        return (index >= 0 && index < (int)values->size()) ? (*curValue).c_str() : "Unknown";
    }
};

struct IntData : Data {
    int defaultVal = 0;
    int currentVal = 0;
    int minVal = INT_MIN;
    int maxVal = INT_MAX;
    int step = 1;
    int stepFast = 100;
    float speed = 1;
    const char* formatting = "%d%";

    void setDefaultVal(int val)
    {
        defaultVal = val;
        currentVal = val;
    }
};

struct FloatData : Data {
    float defaultVal = 0;
    float currentVal = 0;
    float minVal = (float)INT_MIN;
    float maxVal = (float)INT_MAX;
    float step = 1;
    float stepFast = 100;
    float speed = 1;
    const char* formatting = "%.3f";

    void setDefaultVal(float val)
    {
        defaultVal = val;
        currentVal = val;
    }
};

struct ColorData : Data {
    float r = 0;
    float g = 0;
    float b = 0;
    float a = 1;
    float useAlpha = false;
    float rgba[4] = { 0, 0, 0, 1 };

    void init()
    {
        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = a;
    }
};

struct Element {
    std::string id;
    std::string name;
    IMGUI_ELEMENT type;
    std::list<Element>* children;
    Element* parent;

    Element* triggerElement = NULL; // element that when clicked, activates this element (Window)
    Element* triggerPopup = NULL; // popup that gets triggered when clicking this element
    std::map<IMGUI_CALLBACK, int> callbacks; // type, StackID

    Data data; // i want to only use this in the future for all data types, but im to stupid to get the cast to work correctly :(
    IntData intData;
    FloatData floatData;
    ColorData colorData;
    MiscData miscData;

    bool useroverride_isVisible = false;

    bool evaluatedVisibleState = false; // used for imgui to use
    bool lastEvalVisibleState = false; // used for imgui to use

    bool isActive = false; // active = clicked

    Element(const char* elemId, const char* elemName, int elemType)
    {
        if (elemId[0] == '\0') {
            id = std::string(IGNORE_ID); // Elements without ID get ignored in searches
        } else {
            id = std::string(elemId);
        }
        name = std::string(elemName);
        type = static_cast<IMGUI_ELEMENT>(elemType);
        children = new std::list<Element>();
    }

    void AddChild(Element element)
    {
        children->push_back(element);
        element.parent = this;
    }

    void AddCallback(int type, int callbackID)
    {
        RemoveCallback(type);
        callbacks.insert(std::pair<IMGUI_CALLBACK, int>(static_cast<IMGUI_CALLBACK>(type), callbackID));
    }

    void RemoveCallback(int type)
    {
        if (callbacks.count(static_cast<IMGUI_CALLBACK>(type))) {
            callbacks.erase(static_cast<IMGUI_CALLBACK>(type));
        }
    }

    void AddData(Data dataObj)
    {
        data = dataObj;
    }
    void AddData(IntData dataObj)
    {
        intData = dataObj;
    }
    void AddData(FloatData dataObj)
    {
        floatData = dataObj;
    }
    void AddData(ColorData dataObj)
    {
        colorData = dataObj;
    }
    void AddData(MiscData dataObj)
    {
        miscData = dataObj;
    }

    IntData* GetIntData()
    {
        return &intData;
    }
    FloatData* GetFloatData()
    {
        return &floatData;
    }
    ColorData* GetColorData()
    {
        return &colorData;
    }
    MiscData* GetMiscData()
    {
        return &miscData;
    }

    void PropagateCallbackData(lua::LuaCaller* caller)
    {
        switch (type) {
        case IMGUI_ELEMENT::InputInt:
        case IMGUI_ELEMENT::DragInt:
        case IMGUI_ELEMENT::SliderInt:
            caller->push(intData.currentVal);
            break;
        case IMGUI_ELEMENT::InputFloat:
        case IMGUI_ELEMENT::DragFloat:
        case IMGUI_ELEMENT::SliderFloat:
            caller->push(floatData.currentVal);
            break;
        case IMGUI_ELEMENT::ColorEdit:
            caller->push(colorData.rgba[0]);
            caller->push(colorData.rgba[1]);
            caller->push(colorData.rgba[2]);
            if (colorData.useAlpha) {
                caller->push(colorData.rgba[3]);
            }
            break;
        case IMGUI_ELEMENT::Checkbox:
            caller->push(miscData.checked);
            break;
        case IMGUI_ELEMENT::RadioButton:
        case IMGUI_ELEMENT::Combobox:
            caller->push(miscData.index);
            caller->push(miscData.getCurrentValue());
            break;
        case IMGUI_ELEMENT::InputText:
        case IMGUI_ELEMENT::InputTextWithHint:
        case IMGUI_ELEMENT::InputTextMultiline:
            caller->push(miscData.inputText.c_str());
            break;
        case IMGUI_ELEMENT::InputController:
        case IMGUI_ELEMENT::InputKeyboard:
            caller->push(imGuiToIsaacKey.at(static_cast<ImGuiKey>(miscData.defaultKeyboardKey)));
            caller->push(ImGui::GetKeyName(static_cast<ImGuiKey>(miscData.defaultKeyboardKey)));
            break;
        default:
            caller->push((float)data.clickCounter);
            break;
        }
    }

    std::size_t GetHash()
    {
        std::size_t h1 = std::hash<std::string> {}(name);
        std::size_t h2 = std::hash<std::string> {}(id);
        std::size_t h3 = std::hash<IMGUI_ELEMENT> {}(type);
        return h1 ^ (h2 << 1) ^ (h3 << 1);
    }

    void EvaluateVisible()
    {
        bool newEval = triggerElement != NULL && triggerElement->isActive || useroverride_isVisible;
        // Workaround: There are 3 ways to change visibility of a window - the trigger element gets clicked, the SetVisible() function is called
        //   or the user clicks on the (X) to close the window. that action does not have a callback and can only be detected when the visibility
        //   boolean variable was changed unexpected.
        if (lastEvalVisibleState == newEval && newEval != evaluatedVisibleState) {
            // user closed the window via imgui (X) button
            SetVisible(evaluatedVisibleState);
            newEval = evaluatedVisibleState;
        }
        evaluatedVisibleState = newEval;
        lastEvalVisibleState = newEval;
    }

    void SetVisible(bool newState)
    {
        if (triggerElement != NULL) {
            triggerElement->isActive = newState;
        } else {
            useroverride_isVisible = newState;
        }
    }
};

struct CustomImGui {
    bool enabled;
    std::list<Element>* menuElements;
    std::list<Element>* windows;

    CustomImGui()
    {
        enabled = false;
        menuElements = new std::list<Element>();
        windows = new std::list<Element>();
    }

    void Reset()
    {
        enabled = false;
        menuElements->clear();
        windows->clear();
    }

    bool AddElement(const char* parentId, const char* id, const char* text, int type) IM_FMTARGS(2)
    {
        Element* parent = GetElementById(parentId);
        if (parent != NULL) {
            parent->AddChild(Element(id, text, type));
            return true;
        }
        return false;
    }

    void RemoveElement(const char* elementId) IM_FMTARGS(2)
    {
        Element* element = GetElementByList(elementId, menuElements);
        if (element == NULL) {
            element = GetElementByList(elementId, windows);
            if (element == NULL) {
                return;
            }
        }
        for (auto elem = element->parent->children->begin(); elem != element->parent->children->end(); ++elem) {
            if (strcmp(elem->id.c_str(), elementId) == 0) {
                element->parent->children->erase(elem);
                return;
            }
        }
    }

    bool CreateMenuElement(const char* id, const char* text) IM_FMTARGS(2)
    {
        menuElements->push_back(Element(id, text, static_cast<int>(IMGUI_ELEMENT::Menu)));
        return true;
    }

    void RemoveMenu(const char* menuId) IM_FMTARGS(2)
    {
        for (auto menu = menuElements->begin(); menu != menuElements->end(); ++menu) {
            if (strcmp(menu->id.c_str(), menuId) == 0) {
                menuElements->erase(menu);
            }
        }
    }

    Element* CreateWindowElement(const char* id, const char* name) IM_FMTARGS(2)
    {
        windows->push_back(Element(id, name, static_cast<int>(IMGUI_ELEMENT::Window)));
        return &windows->back();
    }

    void RemoveWindow(const char* windowId) IM_FMTARGS(2)
    {
        for (auto window = windows->begin(); window != windows->end(); ++window) {
            if (strcmp(window->id.c_str(), windowId) == 0) {
                windows->erase(window);
            }
        }
    }

    bool AddCallback(const char* parentId, int type, int callbackID) IM_FMTARGS(2)
    {
        Element* parent = GetElementById(parentId);
        if (parent != NULL) {
            parent->AddCallback(type, callbackID);
            return true;
        }
        return false;
    }

    bool RemoveCallback(const char* parentId, int type) IM_FMTARGS(2)
    {
        Element* parent = GetElementById(parentId);
        if (parent != NULL) {
            parent->RemoveCallback(type);
            return true;
        }
        return false;
    }

    bool LinkWindowToElement(const char* windowId, const char* elementId) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
        Element* window = GetElementById(windowId);
        if (window != NULL && element != NULL) {
            if (window->type == IMGUI_ELEMENT::Popup) {
                element->triggerPopup = window;
            } else {
                window->triggerElement = element;
            }
            if (element->type != IMGUI_ELEMENT::MenuItem) {
                element->AddCallback(static_cast<int>(IMGUI_CALLBACK::Clicked), 0);
            }
            return true;
        }
        return false;
    }

    bool UpdateText(const char* elementId, const char* newText)
    {
        Element* element = GetElementById(elementId);
        if (element != NULL) {
            element->name = std::string(newText);
            return true;
        }

        return false;
    }

    bool SetTooltipText(const char* elementId, const char* newText)
    {
        Element* element = GetElementById(elementId);
        if (element != NULL) {
            element->data.tooltipText = std::string(newText);
            return true;
        }

        return false;
    }

    bool SetHelpMarkerText(const char* elementId, const char* newText)
    {
        Element* element = GetElementById(elementId);
        if (element != NULL) {
            element->data.helpmarkerText = std::string(newText);
            return true;
        }

        return false;
    }

    bool ElementExists(const char* elementId)
    {
        return GetElementById(elementId) != NULL;
    }

    bool SetVisible(const char* elementId, bool newState) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
        if (element != NULL) {
            element->SetVisible(newState);
            return true;
        }
        return false;
    }

    void OpenPopup(const char* popupId)
    {
        ImGui::OpenPopup(popupId);
    }

    void RunPreRenderCallbacks(Element* element) IM_FMTARGS(2)
    {
        for (auto callback = element->callbacks.begin(); callback != element->callbacks.end(); ++callback) {
            if (callback->first == IMGUI_CALLBACK::Render) {
                RunCallback(element, callback->second);
            }
        }
    }

    void RunCallbacks(Element* element) IM_FMTARGS(2)
    {
        for (auto callback = element->callbacks.begin(); callback != element->callbacks.end(); ++callback) {
            switch (callback->first) {
            case IMGUI_CALLBACK::Clicked:
                if (ImGui::IsItemClicked()) {
                    element->data.clickCounter++;
                    element->isActive = !element->isActive;
                    if (element->triggerPopup != NULL) {
                        OpenPopup(element->triggerPopup->id.c_str());
                    }
                    RunCallback(element, callback->second);
                }
                break;
            case IMGUI_CALLBACK::Hovered:
                if (ImGui::IsItemHovered()) {
                    RunCallback(element, callback->second);
                }
                break;
            case IMGUI_CALLBACK::Active:
                if (ImGui::IsItemActive()) {
                    RunCallback(element, callback->second);
                }
                break;
            case IMGUI_CALLBACK::Focused:
                if (ImGui::IsItemFocused()) {
                    RunCallback(element, callback->second);
                }
                break;
            case IMGUI_CALLBACK::Visible:
                if (ImGui::IsItemVisible()) {
                    RunCallback(element, callback->second);
                }
                break;
            case IMGUI_CALLBACK::Edited:
                if (ImGui::IsItemEdited()) {
                    RunCallback(element, callback->second);
                }
                break;
            case IMGUI_CALLBACK::Activated:
                if (ImGui::IsItemActivated()) {
                    element->isActive = true;
                    RunCallback(element, callback->second);
                }
                break;
            case IMGUI_CALLBACK::Deactivated:
                if (ImGui::IsItemDeactivated()) {
                    element->isActive = false;
                    RunCallback(element, callback->second);
                }
                break;
            case IMGUI_CALLBACK::DeactivatedAfterEdit:
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    RunCallback(element, callback->second);
                }
                break;
            case IMGUI_CALLBACK::ToggledOpen:
                if (ImGui::IsItemToggledOpen()) {
                    RunCallback(element, callback->second);
                }
                break;
            default:
                break;
            }
        }
    }

    void RunCallback(Element* element, int callbackID) IM_FMTARGS(2)
    {
        if (callbackID == 0) {
            return;
        }
        lua_State* L = g_LuaEngine->_state;
        lua::LuaStackProtector protector(L);

        lua_rawgeti(L, LUA_REGISTRYINDEX, callbackID);

        lua::LuaCaller caller = lua::LuaCaller(L);
        element->PropagateCallbackData(&caller);
        caller.call(1);
    }

    bool UpdateElementValue(Element* element, lua_State* L)
    {
        switch (element->type) {
        case IMGUI_ELEMENT::InputText:
        case IMGUI_ELEMENT::InputTextWithHint:
        case IMGUI_ELEMENT::InputTextMultiline:
            element->miscData.inputText = luaL_checkstring(L, 4);
            return true;

        case IMGUI_ELEMENT::Checkbox:
            element->miscData.checked = lua_toboolean(L, 4);
            return true;

        case IMGUI_ELEMENT::RadioButton:
        case IMGUI_ELEMENT::Combobox:
            element->miscData.index = (int)luaL_checkinteger(L, 4);
            return true;

        case IMGUI_ELEMENT::InputInt:
        case IMGUI_ELEMENT::DragInt:
        case IMGUI_ELEMENT::SliderInt:
            element->intData.currentVal = (int)luaL_checkinteger(L, 4);
            return true;

        case IMGUI_ELEMENT::InputFloat:
        case IMGUI_ELEMENT::DragFloat:
        case IMGUI_ELEMENT::SliderFloat:
            element->floatData.currentVal = (float)luaL_checknumber(L, 4);
            return true;

        case IMGUI_ELEMENT::InputController:
        case IMGUI_ELEMENT::InputKeyboard:
            element->miscData.defaultKeyboardKey = (int)luaL_checkinteger(L, 4);
            return true;

        default:
            return false;
        }
    }

    bool UpdateElementData(Element* element, lua_State* L)
    {
        IMGUI_DATA dataType = static_cast<IMGUI_DATA>(luaL_checkinteger(L, 3));
        std::list<float>* newColorValues = new std::list<float>();

        switch (dataType) {
        case IMGUI_DATA::Label:
            element->name = luaL_checkstring(L, 4);
            return true;

        case IMGUI_DATA::Value:
            return UpdateElementValue(element, L);

        case IMGUI_DATA::ListValues:
            if (!lua_istable(L, 4))
                return luaL_error(L, "Argument 4 needs to be a table!");

            element->miscData.values->clear();
            for (auto i = 1; i <= lua_rawlen(L, 4); ++i) {
                lua_pushinteger(L, i);
                lua_gettable(L, 4);
                if (lua_type(L, -1) == LUA_TNIL)
                    break;
                element->miscData.values->push_back(luaL_checkstring(L, -1));
                lua_pop(L, 1);
            }
            return true;

        case IMGUI_DATA::Min:
            switch (element->type) {
            case IMGUI_ELEMENT::DragInt:
            case IMGUI_ELEMENT::SliderInt:
                element->intData.minVal = (int)luaL_checkinteger(L, 4);
                return true;
            case IMGUI_ELEMENT::DragFloat:
            case IMGUI_ELEMENT::SliderFloat:
                element->floatData.minVal = (float)luaL_checknumber(L, 4);
                return true;
            default:
                return false;
            }

        case IMGUI_DATA::Max:
            switch (element->type) {
            case IMGUI_ELEMENT::DragInt:
            case IMGUI_ELEMENT::SliderInt:
                element->intData.maxVal = (int)luaL_checkinteger(L, 4);
                return true;
            case IMGUI_ELEMENT::DragFloat:
            case IMGUI_ELEMENT::SliderFloat:
                element->floatData.maxVal = (float)luaL_checknumber(L, 4);
                return true;
            default:
                return false;
            }

        case IMGUI_DATA::HintText:
            if (element->type != IMGUI_ELEMENT::InputText && element->type != IMGUI_ELEMENT::InputTextWithHint)
                return false;
            element->miscData.hintText = luaL_checkstring(L, 4);
            return true;

        case IMGUI_DATA::ColorValues:
            if (element->type != IMGUI_ELEMENT::ColorEdit)
                return false;
            if (!lua_istable(L, 4))
                return luaL_error(L, "Argument 4 needs to be a table!");

            // get table input
            for (auto i = 1; i <= lua_rawlen(L, 4); ++i) {
                lua_pushinteger(L, i);
                lua_gettable(L, 4);
                if (lua_type(L, -1) == LUA_TNIL)
                    break;
                newColorValues->push_back((float)luaL_checknumber(L, -1));
                lua_pop(L, 1);
            }
            element->colorData.useAlpha = newColorValues->size() > 3;
            element->colorData.r = newColorValues->front();
            newColorValues->pop_front();
            element->colorData.g = newColorValues->front();
            newColorValues->pop_front();
            element->colorData.b = newColorValues->front();
            newColorValues->pop_front();
            element->colorData.a = newColorValues->front();
            element->colorData.init();
            return true;

        default:
            return false;
        }
    }

    Element* GetElementById(const char* elementID) IM_FMTARGS(2)
    {
        Element* element = GetElementByList(elementID, menuElements);
        if (element != NULL) {
            return element;
        }
        element = GetElementByList(elementID, windows);
        if (element != NULL) {
            return element;
        }
        return nullptr;
    }

    Element* GetElementByList(const char* id, std::list<Element>* list)
    {
        for (auto element = list->begin(); element != list->end(); ++element) {
            if (strcmp(element->id.c_str(), id) == 0 && !(strcmp(element->id.c_str(), IGNORE_ID) == 0)) {
                return &(*element);
            }
            Element* childResult = GetElementByList(id, element->children);
            if (childResult != NULL) {
                return childResult;
            }
        }
        return NULL;
    }

    /* From https://github.com/pthom/imgui/blob/DemoCodeDocking/misc/cpp/imgui_stdlib.cpp */
    struct InputTextCallback_UserData {
        std::string* Str;
        ImGuiInputTextCallback ChainCallback;
        void* ChainCallbackUserData;
    };

    static int InputTextCallback(ImGuiInputTextCallbackData* data)
    {
        InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
            // Resize string callback
            // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
            std::string* str = user_data->Str;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char*)str->c_str();
        } else if (user_data->ChainCallback) {
            // Forward to user callback, if any
            data->UserData = user_data->ChainCallbackUserData;
            return user_data->ChainCallback(data);
        }
        return 0;
    }

    bool StringInputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

    bool StringInputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

    bool StringInputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
    }
    // End Custom Text input implementaiton with String as Input

    void DrawMenu()
    {
        DrawMenuElements(menuElements);
    }

    void DrawWindows(bool isImGuiActive)
    {
        for (auto window = windows->begin(); window != windows->end(); ++window) {
            ImGui::PushID(window->GetHash());
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 100));
            window->EvaluateVisible();
            if ((isImGuiActive || !isImGuiActive && window->data.windowPinned) && window->evaluatedVisibleState) {
                if (ImGui::Begin(window->name.c_str(), &window->evaluatedVisibleState, handleWindowFlags(0))) {
                    AddWindowContextMenu(&window->data.windowPinned);
                    DrawElements(window->children);
                    ImGui::End();
                }
            }
            ImGui::PopStyleVar();
            ImGui::PopID();
        }
    }

    void HandleElementExtras(Element* element)
    {
        if (!element->data.tooltipText.empty()) {
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip(element->data.tooltipText.c_str());
        }
        if (!element->data.helpmarkerText.empty()) {
            ImGui::SameLine();
            HelpMarker(element->data.helpmarkerText.c_str());
        }
    }

    void DrawMenuElements(std::list<Element>* elements)
    {
        for (auto element = elements->begin(); element != elements->end(); ++element) {
            const char* name = element->name.c_str();

            ImGui::PushID(element->GetHash());
            switch (element->type) {
            case IMGUI_ELEMENT::Menu:
                if (ImGui::BeginMenu(name)) {
                    RunCallbacks(&(*element));
                    DrawMenuElements(element->children);
                    ImGui::EndMenu();
                }
                break;
            case IMGUI_ELEMENT::MenuItem:
                ImGui::MenuItem(name, NULL, &element->isActive);
                RunCallbacks(&(*element));
                break;
            default:
                break;
            }
            ImGui::PopID();
        }
    }

    void DrawElements(std::list<Element>* elements)
    {
        for (auto element = elements->begin(); element != elements->end(); ++element) {
            const char* name = element->name.c_str();
            RunPreRenderCallbacks(&(*element));

            ImGui::PushID(element->GetHash());
            switch (element->type) {
            case IMGUI_ELEMENT::CollapsingHeader:
                if (ImGui::CollapsingHeader(name)) {
                    RunCallbacks(&(*element));
                    DrawElements(element->children);
                }
                break;
            case IMGUI_ELEMENT::TreeNode:
                if (ImGui::TreeNode(name)) {
                    RunCallbacks(&(*element));
                    DrawElements(element->children);
                }
                break;
            case IMGUI_ELEMENT::Popup:
                if (element->useroverride_isVisible) {
                    // make popup visible when user changes visibility to true
                    OpenPopup(element->id.c_str());
                    element->useroverride_isVisible = false;
                }
                if (ImGui::BeginPopup(element->id.c_str())) {
                    DrawElements(element->children);
                    ImGui::EndPopup();
                }
                break;
            case IMGUI_ELEMENT::TabBar:
                if (ImGui::BeginTabBar(element->id.c_str(), ImGuiTabBarFlags_None)) {
                    RunCallbacks(&(*element));
                    DrawElements(element->children);
                    ImGui::EndTabBar();
                }
                break;
            case IMGUI_ELEMENT::Tab:
                if (ImGui::BeginTabItem(name)) {
                    RunCallbacks(&(*element));
                    DrawElements(element->children);
                    ImGui::EndTabItem();
                }
                break;
            case IMGUI_ELEMENT::Text:
                ImGui::Text(name);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::SeparatorText:
                ImGui::SeparatorText(name);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::BulletText:
                ImGui::BulletText(name);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::Separator:
                ImGui::Separator();
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::SameLine:
                ImGui::SameLine();
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::Button:
                ImGui::Button(name);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::SmallButton:
                ImGui::SmallButton(name);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::InputInt:
                if (element->GetIntData() != nullptr) {
                    IntData* data = element->GetIntData();
                    ImGui::InputInt(name, &data->currentVal, data->step, data->stepFast);
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::InputFloat:
                if (element->GetFloatData() != nullptr) {
                    FloatData* data = element->GetFloatData();
                    ImGui::InputFloat(name, &data->currentVal, data->step, data->stepFast);
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::DragInt:
                if (element->GetIntData() != nullptr) {
                    IntData* data = element->GetIntData();
                    ImGui::DragInt(name, &data->currentVal, data->speed, data->minVal, data->maxVal, data->formatting);
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::DragFloat:
                if (element->GetFloatData() != nullptr) {
                    FloatData* data = element->GetFloatData();
                    ImGui::DragFloat(name, &data->currentVal, data->speed, data->minVal, data->maxVal, data->formatting);
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::SliderInt:
                if (element->GetIntData() != nullptr) {
                    IntData* data = element->GetIntData();
                    ImGui::SliderInt(name, &data->currentVal, data->minVal, data->maxVal, data->formatting);
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::SliderFloat:
                if (element->GetFloatData() != nullptr) {
                    FloatData* data = element->GetFloatData();
                    ImGui::SliderFloat(name, &data->currentVal, data->minVal, data->maxVal, data->formatting);
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::ColorEdit:
                if (element->GetColorData() != nullptr) {
                    ColorData* data = element->GetColorData();
                    if (data->useAlpha) {
                        ImGui::ColorEdit4(name, data->rgba);
                    } else {
                        ImGui::ColorEdit3(name, data->rgba);
                    }
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::Checkbox:
                if (element->GetMiscData() != nullptr) {
                    MiscData* data = element->GetMiscData();
                    ImGui::Checkbox(name, &data->checked);
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::RadioButton:
                if (element->GetMiscData() != nullptr) {
                    MiscData* data = element->GetMiscData();
                    int counter = 0;
                    for (auto elemName = data->values->begin(); elemName != data->values->end(); ++elemName) {
                        ImGui::RadioButton(elemName->c_str(), &data->index, counter);
                        RunCallbacks(&(*element));
                        if (data->sameLine && counter < (int)data->values->size() - 1) {
                            ImGui::SameLine();
                        }
                        ++counter;
                    }
                }
                break;
            case IMGUI_ELEMENT::Combobox:
                if (element->GetMiscData() != nullptr) {
                    MiscData* data = element->GetMiscData();
                    if (data->isSlider) {
                        ImGui::SliderInt(name, &data->index, 0, data->values->size() - 1, data->getCurrentValue());
                        RunCallbacks(&(*element));
                    } else {
                        if (ImGui::BeginCombo(name, data->getCurrentValue())) {
                            int counter = 0;
                            for (auto elemName = data->values->begin(); elemName != data->values->end(); ++elemName) {
                                const bool is_selected = (data->index == counter);
                                if (ImGui::Selectable(elemName->c_str(), is_selected)) {
                                    data->index = counter;
                                    RunCallbacks(&(*element));
                                }
                                if (is_selected) {
                                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                                    ImGui::SetItemDefaultFocus();
                                }
                                ++counter;
                            }
                            ImGui::EndCombo();
                        }
                    }
                }
                break;
            case IMGUI_ELEMENT::InputText:
            case IMGUI_ELEMENT::InputTextWithHint:
                if (element->GetMiscData() != nullptr) {
                    MiscData* data = element->GetMiscData();
                    if (data->hintText.empty()) {
                        StringInputText(name, &data->inputText, NULL, NULL, NULL);
                    } else {
                        StringInputTextWithHint(name, data->hintText.c_str(), &data->inputText, NULL, NULL, NULL);
                    }
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::InputTextMultiline:
                if (element->GetMiscData() != nullptr) {
                    MiscData* data = element->GetMiscData();
                    StringInputTextMultiline(name, &data->inputText, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * data->lineCount), NULL, NULL, NULL);
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::InputController:
            case IMGUI_ELEMENT::InputKeyboard:
                if (element->GetMiscData() != nullptr) {
                    MiscData* data = element->GetMiscData();
                    if (data->inputText.empty())
                        data->inputText = std::string(ImGui::GetKeyName(static_cast<ImGuiKey>(data->defaultKeyboardKey)));

                    ImGuiKey newButton = AddChangeKeyButton(element->type == IMGUI_ELEMENT::InputController, element->isActive);
                    if (newButton != ImGuiKey_None) {
                        data->inputText = ImGui::GetKeyName(newButton);
                        data->defaultKeyboardKey = static_cast<int>(newButton);
                        // workaround: manually trigger Edited events
                        for (auto callback = element->callbacks.begin(); callback != element->callbacks.end(); ++callback) {
                            if (callback->first == IMGUI_CALLBACK::Edited) {
                                RunCallback(&(*element), callback->second);
                            }
                        }
                    }
                    ImGui::SameLine();
                    StringInputText(name, &data->inputText, ImGuiInputTextFlags_ReadOnly, NULL, NULL);
                }
                break;
            default:
                break;
            }
            HandleElementExtras(&(*element));
            ImGui::PopID();
        }
    }
};

extern CustomImGui customImGui;
