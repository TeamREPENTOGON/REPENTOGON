#pragma once
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
extern bool WindowBeginEx(const char* name, bool* p_open, ImGuiWindowFlags flags);
extern bool menuShown;

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
    TextWrapped,
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
    InputKeyboard,
    PlotLines,
    PlotHistogram,
    ProgressBar,
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

static const char* IMGUI_CALLBACK_TO_STRING[11] = {
    "Clicked",
    "Hovered",
    "Active",
    "Focused",
    "Visible",
    "Edited",
    "Activated",
    "Deactivated",
    "DeactivatedAfterEdit",
    "ToggledOpen",
    "Render"
};

enum class IMGUI_DATA {
    Label,
    Value,
    ListValues,
    Min,
    Max,
    HintText,
    ColorValues
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

struct ColorHandler {
    ImGuiCol_ type;
    ImVec4 color;

    ColorHandler(ImGuiCol_ t, ImVec4 c)
    {
        type = t;
        color = c;
    }
};

struct Data {
    int clickCounter = 0;
    std::string tooltipText = "";
    std::string helpmarkerText = "";
    bool windowPinned = false;
    bool newPositionRequested = false;
    ImVec2 newPosition = ImVec2(0, 0);
    bool newSizeRequested = false;
    ImVec2 size = ImVec2(0, 0); // 0,0 initializes elements with dynamic size
    ImGuiWindowFlags windowFlags = 0;
    ImGuiChildFlags childFlags = ImGuiChildFlags_Border;
};

struct ElementData : Data {
    bool checked = false; // used for Checkbox
    int index = 0; // used for RadioButtons
    bool sameLine = true; // used for RadioButtons
    bool isSlider = false; // used for switch from Combobox to Enum Slider
    std::list<std::string>* values = new std::list<std::string>(); // used for RadioButtons and Combobox
    std::list<float>* plotValues = new std::list<float>(); // used for Plots
    std::string inputText = ""; // Used for Text input
    std::string hintText = ""; // Used for Text input
    float lineCount = 6;
    float minVal = FLT_MIN;
    float maxVal = FLT_MAX;
    float defaultFloatVal = 0; // used to store default float input value
    float currentFloatVal = 0; // used by Float Inputs
    int defaultIntVal = 0;
    int currentIntVal = 0; // used by Int Inputs and Keyboard/controller inputs
    float step = 1;
    float stepFast = 100;
    float speed = 1;
    std::string formatting = "%.3f";
    std::list<ColorHandler>* colors = new std::list<ColorHandler>();

    const char* DefaultFloatNumberFormatting = "%.3f";
    const char* DefaultIntNumberFormatting = "%d%";

    void setDefaultIntVal(int val)
    {
        defaultIntVal = val;
        currentIntVal = val;
    }
    void setDefaultFloatVal(float val)
    {
        defaultFloatVal = val;
        currentFloatVal = val;
    }

    const char* getCurrentListValue()
    {
        auto curValue = values->begin();
        std::advance(curValue, index); // get element at index
        return (index >= 0 && index < (int)values->size()) ? (*curValue).c_str() : "Unknown";
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
    Element* parent = NULL;

    Element* triggerElement = NULL; // element that when clicked, activates this element (Window)
    Element* triggerPopup = NULL; // popup that gets triggered when clicking this element
    std::map<IMGUI_CALLBACK, int> callbacks; // type, StackID

    Data data {}; // i want to only use this in the future for all data types, but im to stupid to get the cast to work correctly :(
    ColorData colorData;
    ElementData elementData {};

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
        element.parent = this;
        children->push_back(element);
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
    void AddData(ColorData dataObj)
    {
        colorData = dataObj;
    }
    void AddData(ElementData dataObj)
    {
        elementData = dataObj;
    }

    ColorData* GetColorData()
    {
        return &colorData;
    }
    ElementData* GetElementData()
    {
        return &elementData;
    }

    void PropagateCallbackData(lua::LuaCaller* caller)
    {
        switch (type) {
        case IMGUI_ELEMENT::InputInt:
        case IMGUI_ELEMENT::DragInt:
        case IMGUI_ELEMENT::SliderInt:
            caller->push(elementData.currentIntVal);
            break;
        case IMGUI_ELEMENT::InputFloat:
        case IMGUI_ELEMENT::DragFloat:
        case IMGUI_ELEMENT::SliderFloat:
            caller->push(elementData.currentFloatVal);
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
            caller->push(elementData.checked);
            break;
        case IMGUI_ELEMENT::RadioButton:
        case IMGUI_ELEMENT::Combobox:
            caller->push(elementData.index);
            caller->push(elementData.getCurrentListValue());
            break;
        case IMGUI_ELEMENT::InputText:
        case IMGUI_ELEMENT::InputTextWithHint:
        case IMGUI_ELEMENT::InputTextMultiline:
            caller->push(elementData.inputText.c_str());
            break;
        case IMGUI_ELEMENT::InputController:
        case IMGUI_ELEMENT::InputKeyboard:
            caller->push(imGuiToIsaacKey.at(static_cast<ImGuiKey>(elementData.currentIntVal)));
            caller->push(ImGui::GetKeyName(static_cast<ImGuiKey>(elementData.currentIntVal)));
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
        if (parent == NULL)
            return false;

        parent->AddChild(Element(id, text, type));
        return true;
    }

    bool RemoveElement(const char* elementId) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
            if (element == NULL) {
                g_Game->GetConsole()->PrintError("Couldn't find the element to remove! (" + std::string(elementId) + ") \n");
                return false;
            }
        if (element->type == IMGUI_ELEMENT::Menu) {
            RemoveMenu(elementId);
            return true;
        }
        if (element->type == IMGUI_ELEMENT::Window && element->parent == NULL) {
            RemoveWindow(elementId);
            return true;
        }

        if (element->parent != NULL) {
            Element* daddy = element->parent;
            if ((daddy != NULL) && (daddy->children != NULL) && (daddy->children->size() > 0)) {
                std::list<Element>* siblings = daddy->children;
                for (auto elem = siblings->begin(); elem != siblings->end(); ++elem) {
                    if (strcmp(elem->id.c_str(), elementId) == 0) {
                        siblings->erase(elem);
                        return true;
                    }
                }
            }
        }
        g_Game->GetConsole()->PrintError("Couldnt find the element to remove! (" + std::string(elementId) + ") \n");
        return false;
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

    bool CreateWindowElement(const char* id, const char* name, const char* parentId = nullptr) IM_FMTARGS(2)
    {
        if (parentId != nullptr) {
            Element* parent = GetElementById(parentId);
            if (parent == NULL) {
                return false;
            }
            else {
              parent->AddChild(Element(id, name, static_cast<int>(IMGUI_ELEMENT::Window)));
              return true;
            }
        }
        windows->push_back(Element(id, name, static_cast<int>(IMGUI_ELEMENT::Window)));
        return true;
    }

    void RemoveWindow(const char* windowId) IM_FMTARGS(2)
    {
        for (auto window = windows->begin(); window != windows->end(); ++window) {
            if (strcmp(window->id.c_str(), windowId) == 0) {
                windows->erase(window);
                return;
            }
        }
    }

    bool AddCallback(const char* parentId, int type, int callbackID) IM_FMTARGS(2)
    {
        Element* parent = GetElementById(parentId);
        if (parent == NULL)
            return false;

        parent->AddCallback(type, callbackID);
        return true;
    }

    bool RemoveCallback(const char* parentId, int type) IM_FMTARGS(2)
    {
        Element* parent = GetElementById(parentId);
        if (parent == NULL)
            return false;

        parent->RemoveCallback(type);
        return true;
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
        if (element == NULL)
            return false;

        std::string newTextStr(newText);
        if (element->name != newTextStr)
            element->name = newTextStr;
        return true;
    }

    bool SetTooltipText(const char* elementId, const char* newText)
    {
        Element* element = GetElementById(elementId);
        if (element == NULL)
            return false;

        std::string newTextStr(newText);
        if (element->data.tooltipText != newTextStr)
            element->data.tooltipText = newTextStr;
        return true;
    }

    bool SetHelpMarkerText(const char* elementId, const char* newText)
    {
        Element* element = GetElementById(elementId);
        if (element == NULL)
            return false;

        std::string newTextStr(newText);
        if (element->data.helpmarkerText != newTextStr)
            element->data.helpmarkerText = newTextStr;
        return true;
    }

    bool ElementExists(const char* elementId)
    {
        return GetElementById(elementId) != NULL;
    }

    bool SetVisible(const char* elementId, bool newState) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
        if (element == NULL)
            return false;

        element->SetVisible(newState);
        return true;
    }

    bool GetVisible(const char* elementId) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
        if (element == NULL)
            return false;

        return (menuShown || !menuShown && element->data.windowPinned) && element->evaluatedVisibleState;
    }

    bool SetPinned(const char* elementId, bool newState) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
        if (element != NULL && element->type == IMGUI_ELEMENT::Window) {
            if (element->data.windowPinned != newState)
                element->data.windowPinned = newState;
            return true;
        }
        return false;
    }

    bool SetWindowFlags(const char* elementId, ImGuiWindowFlags newFlags) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
        if (element != NULL && element->type == IMGUI_ELEMENT::Window) {
            newFlags = newFlags & (int)strtol("0000000011111111111111111111111", NULL, 2); // filter internal/deprecated flags just incase
            element->data.windowFlags = newFlags;
            return true;
        }
        return false;
    } 

    bool SetWindowChildFlags(const char* elementId, ImGuiChildFlags newFlags) IM_FMTARGS(2)
    {
      Element* element = GetElementById(elementId);
      if (element != NULL && element->type == IMGUI_ELEMENT::Window) {
        element->data.childFlags = newFlags;
        return true;
      }
      return false;
    }

    bool SetWindowPosition(const char* elementId, float x, float y) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
        if (element != NULL && element->type == IMGUI_ELEMENT::Window) {
            element->data.newPosition = ImVec2(x, y);
            element->data.newPositionRequested = true;
            return true;
        }
        return false;
    }

    bool SetElementSize(const char* elementId, float sizeX, float sizeY) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
        if (element != NULL) {
            element->data.size = ImVec2(sizeX, sizeY);
            element->data.newSizeRequested = true;
            return true;
        }
        return false;
    }

    bool SetColor(const char* elementId, ImGuiCol_ type, ImVec4 newColor) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
        if (element != NULL) {
            RemoveColor(elementId, type);
            element->elementData.colors->push_back(ColorHandler(type, newColor));
            return true;
        }
        return false;
    }

    bool RemoveColor(const char* elementId, ImGuiCol_ type) IM_FMTARGS(2)
    {
        Element* element = GetElementById(elementId);
        if (element != NULL) {
            for (auto color = element->elementData.colors->begin(); color != element->elementData.colors->end(); ++color) {
                if (color->type == type) {
                    element->elementData.colors->erase(color);
                }
            }
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
                RunCallback(element, callback->second, callback->first);
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
                    RunCallback(element, callback->second, callback->first);
                }
                break;
            case IMGUI_CALLBACK::Hovered:
                if (ImGui::IsItemHovered()) {
                    RunCallback(element, callback->second, callback->first);
                }
                break;
            case IMGUI_CALLBACK::Active:
                if (ImGui::IsItemActive()) {
                    RunCallback(element, callback->second, callback->first);
                }
                break;
            case IMGUI_CALLBACK::Focused:
                if (ImGui::IsItemFocused()) {
                    RunCallback(element, callback->second, callback->first);
                }
                break;
            case IMGUI_CALLBACK::Visible:
                if (ImGui::IsItemVisible()) {
                    RunCallback(element, callback->second, callback->first);
                }
                break;
            case IMGUI_CALLBACK::Edited:
                if (ImGui::IsItemEdited()) {
                    RunCallback(element, callback->second, callback->first);
                }
                break;
            case IMGUI_CALLBACK::Activated:
                if (ImGui::IsItemActivated()) {
                    element->isActive = true;
                    RunCallback(element, callback->second, callback->first);
                }
                break;
            case IMGUI_CALLBACK::Deactivated:
                if (ImGui::IsItemDeactivated()) {
                    element->isActive = false;
                    RunCallback(element, callback->second, callback->first);
                }
                break;
            case IMGUI_CALLBACK::DeactivatedAfterEdit:
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    RunCallback(element, callback->second, callback->first);
                }
                break;
            case IMGUI_CALLBACK::ToggledOpen:
                if (ImGui::IsItemToggledOpen()) {
                    RunCallback(element, callback->second, callback->first);
                }
                break;
            default:
                break;
            }
        }
    }

    void RunCallback(Element* element, const int callbackID, const IMGUI_CALLBACK callbackType = IMGUI_CALLBACK::Clicked) IM_FMTARGS(2)
    {
        if (callbackID == 0) {
            return;
        }
        lua_State* L = g_LuaEngine->_state;
        lua::LuaStackProtector protector(L);

        lua_rawgeti(L, LUA_REGISTRYINDEX, callbackID);

        lua::LuaCaller caller = lua::LuaCaller(L);
        element->PropagateCallbackData(&caller);
        if (caller.call(1)) {
            // Lua encountered an error while executing the callback function
            // printing the error needs to happen in a seperate lua call, as to keep the lua-stack and imgui-stack protected and stable
            const char* errorMsg = lua_tostring(L, lua_gettop(L));
            int callbackTypeID = static_cast<int>(callbackType);
            g_Game->GetConsole()->PrintError(
                "ImGui encountered an error in the '" + std::string(IMGUI_CALLBACK_TO_STRING[callbackTypeID])
                + "' callback of element '" + std::string(element->id) + "': " + std::string(errorMsg) + "\n");
        }
    }

    bool UpdateElementValue(Element* element, lua_State* L)
    {
        int intVal = 0;
        bool checkVal = false;
        float floatVal = 0.0;
        switch (element->type) {
        case IMGUI_ELEMENT::InputText:
        case IMGUI_ELEMENT::InputTextWithHint:
        case IMGUI_ELEMENT::InputTextMultiline:
            if (element->elementData.inputText != luaL_checkstring(L, 3))
                element->elementData.inputText = luaL_checkstring(L, 3);
            return true;

        case IMGUI_ELEMENT::Checkbox:
            checkVal = lua::luaL_checkboolean(L, 3);
            if (element->elementData.checked != checkVal)
                element->elementData.checked = checkVal;
            return true;

        case IMGUI_ELEMENT::RadioButton:
        case IMGUI_ELEMENT::Combobox:
            intVal = (int)luaL_checkinteger(L, 3);
            if (element->elementData.index != intVal)
                element->elementData.index = intVal;
            return true;

        case IMGUI_ELEMENT::InputInt:
        case IMGUI_ELEMENT::DragInt:
        case IMGUI_ELEMENT::SliderInt:
        case IMGUI_ELEMENT::InputController:
        case IMGUI_ELEMENT::InputKeyboard:
            intVal = (int)luaL_checkinteger(L, 3);
            if (element->elementData.currentIntVal != intVal)
                element->elementData.currentIntVal = intVal;
            return true;

        case IMGUI_ELEMENT::InputFloat:
        case IMGUI_ELEMENT::DragFloat:
        case IMGUI_ELEMENT::SliderFloat:
        case IMGUI_ELEMENT::ProgressBar:
            floatVal = (float)luaL_checknumber(L, 3);
            if (element->elementData.currentFloatVal != floatVal)
                element->elementData.currentFloatVal = floatVal;
            return true;

        default:
            return false;
        }
    }

    bool UpdateElementData(Element* element, lua_State* L)
    {
        IMGUI_DATA dataType = static_cast<IMGUI_DATA>(luaL_checkinteger(L, 2));
        std::list<float>* newColorValues = new std::list<float>();

        float floatVal = 0.0;
        switch (dataType) {
        case IMGUI_DATA::Label:
            if (element->name != luaL_checkstring(L, 3))
                element->name = luaL_checkstring(L, 3);
            return true;

        case IMGUI_DATA::Value:
            return UpdateElementValue(element, L);

        case IMGUI_DATA::ListValues:
            if (!lua_istable(L, 3))
                return luaL_error(L, "Argument 4 needs to be a table!");

            element->elementData.plotValues->clear();
            element->elementData.values->clear();
            for (auto i = 1; i <= lua_rawlen(L, 3); ++i) {
                lua_pushinteger(L, i);
                lua_gettable(L, 3);
                if (lua_type(L, -1) == LUA_TNIL)
                    break;
                if (element->type == IMGUI_ELEMENT::PlotLines || element->type == IMGUI_ELEMENT::PlotHistogram) {
                    element->elementData.plotValues->push_back((float)luaL_checknumber(L, -1));
                } else {
                    element->elementData.values->push_back(luaL_checkstring(L, -1));
                }
                lua_pop(L, 1);
            }
            return true;

        case IMGUI_DATA::Min:
            switch (element->type) {
            case IMGUI_ELEMENT::DragInt:
            case IMGUI_ELEMENT::SliderInt:
            case IMGUI_ELEMENT::DragFloat:
            case IMGUI_ELEMENT::SliderFloat:
                floatVal = (float)luaL_checknumber(L, 3);
                if (element->elementData.minVal != floatVal)
                    element->elementData.minVal = floatVal;
                return true;
            default:
                return false;
            }

        case IMGUI_DATA::Max:
            switch (element->type) {
            case IMGUI_ELEMENT::DragInt:
            case IMGUI_ELEMENT::SliderInt:
            case IMGUI_ELEMENT::DragFloat:
            case IMGUI_ELEMENT::SliderFloat:
                floatVal = (float)luaL_checknumber(L, 3);
                if (element->elementData.maxVal != floatVal)
                    element->elementData.maxVal = floatVal;
                return true;
            default:
                return false;
            }

        case IMGUI_DATA::HintText:
            if (element->type != IMGUI_ELEMENT::InputText
                && element->type != IMGUI_ELEMENT::InputTextWithHint
                && element->type != IMGUI_ELEMENT::PlotLines
                && element->type != IMGUI_ELEMENT::PlotHistogram
                && element->type != IMGUI_ELEMENT::ProgressBar)
                return false;
            if (element->elementData.hintText != luaL_checkstring(L, 3))
                element->elementData.hintText = luaL_checkstring(L, 3);
            return true;

        case IMGUI_DATA::ColorValues:
            if (element->type != IMGUI_ELEMENT::ColorEdit)
                return false;
            if (!lua_istable(L, 3))
                return luaL_error(L, "Argument 4 needs to be a table!");

            // get table input
            for (auto i = 1; i <= lua_rawlen(L, 3); ++i) {
                lua_pushinteger(L, i);
                lua_gettable(L, 3);
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
            ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_FirstUseEver);
            HandleElementColors(window->GetElementData(), true);
            window->EvaluateVisible();

            if ((isImGuiActive || !isImGuiActive && window->data.windowPinned) && window->evaluatedVisibleState) {
                RunPreRenderCallbacks(&(*window));
                if (WindowBeginEx(window->name.c_str(), &window->evaluatedVisibleState, handleWindowFlags(window->data.windowFlags))) {
                    if (window->data.newPositionRequested) {
                        ImGui::SetWindowPos(window->data.newPosition);
                        window->data.newPositionRequested = false;
                    }
                    if (window->data.newSizeRequested) {
                        ImGui::SetWindowSize(window->data.size);
                        window->data.newSizeRequested = false;
                    }
                    AddWindowContextMenu(&window->data.windowPinned);
                    DrawElements(window->children);
                }
                ImGui::End(); // close window element
            }
            HandleElementColors(window->GetElementData(), false);
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

    void HandleElementColors(ElementData* data, bool isPush)
    {
        if (isPush)
            for (auto color = data->colors->begin(); color != data->colors->end(); ++color)
                ImGui::PushStyleColor(color->type, color->color);
        else
            ImGui::PopStyleColor(data->colors->size());
    }

    void HandleElementSize(Element* element, bool isPush)
    {
        switch (element->type) {
        // ignore elements with special size handling
        case IMGUI_ELEMENT::Window:
        case IMGUI_ELEMENT::Button:
        case IMGUI_ELEMENT::PlotLines:
        case IMGUI_ELEMENT::PlotHistogram:
            return;
        default:
            if (isPush) {
                ImGui::PushItemWidth(element->data.size.x);
            } else {
                ImGui::PopItemWidth();
            }
            return;
        }
    }

    void DrawMenuElements(std::list<Element>* elements)
    {
        for (auto element = elements->begin(); element != elements->end(); ++element) {
            const char* name = element->name.c_str();
            RunPreRenderCallbacks(&(*element));

            ImGui::PushID(element->GetHash());
            HandleElementSize(&(*element), true);
            HandleElementColors(element->GetElementData(), true);
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
            HandleElementColors(element->GetElementData(), false);
            HandleElementSize(&(*element), false);
            ImGui::PopID();
        }
    }

    void DrawElements(std::list<Element>* elements)
    {
        for (auto element = elements->begin(); element != elements->end(); ++element) {
            const char* name = element->name.c_str();
            RunPreRenderCallbacks(&(*element));

            ImGui::PushID(element->GetHash());
            ElementData* data = element->GetElementData();
            HandleElementSize(&(*element), true);
            HandleElementColors(data, true);

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
                    ImGui::TreePop();
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
            case IMGUI_ELEMENT::Window: {
                if (ImGui::BeginChild(element->name.c_str(), element->data.size, element->data.childFlags, element->data.windowFlags)) {
                    RunCallbacks(&(*element));
                    DrawElements(element->children);
                    ImGui::EndChild();
                }
                break;
            }
            case IMGUI_ELEMENT::Text:
                ImGui::Text(name);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::TextWrapped:
                ImGui::TextWrapped(name);
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
                ImGui::Button(name, element->data.size);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::SmallButton:
                ImGui::SmallButton(name);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::InputInt:
                ImGui::InputInt(name, &(int&)data->currentIntVal, (int)data->step, (int)data->stepFast);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::InputFloat:
                ImGui::InputFloat(name, &data->currentFloatVal, data->step, data->stepFast);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::DragInt:
                ImGui::DragInt(name, &data->currentIntVal, data->speed, (int)data->minVal, (int)data->maxVal, data->formatting.c_str());
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::DragFloat:
                ImGui::DragFloat(name, &data->currentFloatVal, data->speed, data->minVal, data->maxVal, data->formatting.c_str());
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::SliderInt:
                ImGui::SliderInt(name, &(int&)data->currentIntVal, (int)data->minVal, (int)data->maxVal, data->formatting.c_str());
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::SliderFloat:
                ImGui::SliderFloat(name, &data->currentFloatVal, data->minVal, data->maxVal, data->formatting.c_str());
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::ColorEdit:
                if (element->GetColorData() != nullptr) {
                    ColorData* colorData = element->GetColorData();
                    if (colorData->useAlpha) {
                        ImGui::ColorEdit4(name, colorData->rgba);
                    } else {
                        ImGui::ColorEdit3(name, colorData->rgba);
                    }
                    RunCallbacks(&(*element));
                }
                break;
            case IMGUI_ELEMENT::Checkbox:
                ImGui::Checkbox(name, &data->checked);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::RadioButton: {
                int counter = 0;
                for (auto elemName = data->values->begin(); elemName != data->values->end(); ++elemName) {
                    ImGui::RadioButton(elemName->c_str(), &data->index, counter);
                    RunCallbacks(&(*element));
                    if (data->sameLine && counter < (int)data->values->size() - 1) {
                        ImGui::SameLine();
                    }
                    ++counter;
                }
            } break;
            case IMGUI_ELEMENT::Combobox: {
                if (data->isSlider) {
                    ImGui::SliderInt(name, &data->index, 0, data->values->size() - 1, data->getCurrentListValue());
                    RunCallbacks(&(*element));
                } else {
                    if (ImGui::BeginCombo(name, data->getCurrentListValue())) {
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
            } break;
            case IMGUI_ELEMENT::InputText:
            case IMGUI_ELEMENT::InputTextWithHint:
                if (data->hintText.empty()) {
                    StringInputText(name, &data->inputText, NULL, NULL, NULL);
                } else {
                    StringInputTextWithHint(name, data->hintText.c_str(), &data->inputText, NULL, NULL, NULL);
                }
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::InputTextMultiline:
                StringInputTextMultiline(name, &data->inputText, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * data->lineCount), NULL, NULL, NULL);
                RunCallbacks(&(*element));
                break;
            case IMGUI_ELEMENT::InputController:
            case IMGUI_ELEMENT::InputKeyboard: {
                if (data->inputText.empty())
                    data->inputText = std::string(ImGui::GetKeyName(static_cast<ImGuiKey>(data->currentIntVal)));

                ImGuiKey newButton = AddChangeKeyButton(element->type == IMGUI_ELEMENT::InputController, element->isActive);
                if (newButton != ImGuiKey_None) {
                    data->inputText = ImGui::GetKeyName(newButton);
                    data->currentIntVal = static_cast<int>(newButton);
                    // workaround: manually trigger Edited events
                    for (auto callback = element->callbacks.begin(); callback != element->callbacks.end(); ++callback) {
                        if (callback->first == IMGUI_CALLBACK::Edited) {
                            RunCallback(&(*element), callback->second, callback->first);
                        }
                    }
                }
                ImGui::SameLine();
                StringInputText(name, &data->inputText, ImGuiInputTextFlags_ReadOnly, NULL, NULL);
            } break;
            case IMGUI_ELEMENT::PlotLines: {
                std::vector<float> values(data->plotValues->begin(), data->plotValues->end());
                ImGui::PlotLines(name, &values[0], data->plotValues->size(), NULL, data->hintText.c_str(), data->minVal, data->maxVal, element->data.size);
                RunCallbacks(&(*element));
            } break;
            case IMGUI_ELEMENT::PlotHistogram: {
                std::vector<float> values(data->plotValues->begin(), data->plotValues->end());

                ImGui::PlotHistogram(name, &values[0], data->plotValues->size(), NULL, data->hintText.c_str(), data->minVal, data->maxVal, element->data.size);
                RunCallbacks(&(*element));
            } break;
            case IMGUI_ELEMENT::ProgressBar: {
                const char* overlayText = data->hintText == "__DEFAULT__" ? NULL : data->hintText.c_str();
                ImGui::ProgressBar(data->currentFloatVal, element->data.size, overlayText);
                if (!element->name.empty()) {
                    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                    ImGui::Text(name);
                }
                RunCallbacks(&(*element));
            } break;
            default:
                break;
            }

            HandleElementColors(data, false);
            HandleElementExtras(&(*element));
            HandleElementSize(&(*element), false);
            ImGui::PopID();
        }
    }
};

extern CustomImGui customImGui;
