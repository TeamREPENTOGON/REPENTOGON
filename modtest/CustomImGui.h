#include "imgui.h"
#include <IsaacRepentance.h>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "LuaCore.h"

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
};

static const char* IGNORE_ID = "IGNORE_THIS_ELEMENT";

struct Element {
    std::string id;
    std::string name;
    IMGUI_ELEMENT type;
    std::list<Element>* children;
    Element* parent;

    Element* triggerElement = NULL; // element that when clicked, activates this element (Window)
    Element* triggerPopup = NULL; // popup that gets triggered when clicking this element
    std::map<IMGUI_CALLBACK, int> callbacks; // type, StackID

    bool useroverride_isVisible = false;

    bool evaluatedVisibleState = false; // used for imgui to use
    bool lastEvalVisibleState = false; // used for imgui to use

    bool isActive = false; // active = clicked
    int clickCounter = 0;

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

    void RunCallbacks(Element* element) IM_FMTARGS(2)
    {
        for (auto callback = element->callbacks.begin(); callback != element->callbacks.end(); ++callback) {
            switch (callback->first) {
            case IMGUI_CALLBACK::Clicked:
                if (ImGui::IsItemClicked()) {
                    element->clickCounter++;
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

        lua::LuaResults result = lua::LuaCaller(L)
                                     .push(element->clickCounter)
                                     .call(1);
    }

    Element* GetElementById(const char* parentId) IM_FMTARGS(2)
    {
        Element* parent = GetElementByList(parentId, menuElements);
        if (parent != NULL) {
            return parent;
        }
        parent = GetElementByList(parentId, windows);
        if (parent != NULL) {
            return parent;
        }
        return false;
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

    void DrawMenu()
    {
        DrawMenuElements(menuElements);
    }

    void DrawWindows()
    {
        for (auto window = windows->begin(); window != windows->end(); ++window) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 100));
            window->EvaluateVisible();
            if (window->evaluatedVisibleState && ImGui::Begin(window->name.c_str(), &window->evaluatedVisibleState)) {
                DrawElements(window->children);
                ImGui::End();
            }
            ImGui::PopStyleVar();
        }
    }

    void DrawMenuElements(std::list<Element>* elements)
    {
        for (auto element = elements->begin(); element != elements->end(); ++element) {
            const char* name = element->name.c_str();

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
        }
    }

    void DrawElements(std::list<Element>* elements)
    {
        for (auto element = elements->begin(); element != elements->end(); ++element) {
            const char* name = element->name.c_str();
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
            default:
                break;
            }
        }
    }
};

extern CustomImGui customImGui;
