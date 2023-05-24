#include "imgui.h"
#include <IsaacRepentance.h>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "LuaCore.h"

namespace Repentogon {
enum class ELEMENT_TYPE {
    Menu,
    MenuItem,
    CollapsingHeader,
    TreeNode,
    Text,
    SeparatorText,
    BulletText,
    Separator,
    SameLine,
    Button,
};

enum class CALLBACK_TYPE {
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

}

struct Element {
    std::string name;
    Repentogon::ELEMENT_TYPE type;
    std::list<Element>* children;
    bool isActive = false;
    bool isEnabled = true;
    std::map<Repentogon::CALLBACK_TYPE, int> callbacks; // type, StackID
    int clickCounter = 0;

    Element(const char* elemName, int elemType)
    {
        name = std::string(elemName); // ugly copy into std::string, because i dont know how to copy const char* into another const char pointer :(
        type = static_cast<Repentogon::ELEMENT_TYPE>(elemType);
        children = new std::list<Element>();
    }

    void AddChild(Element element)
    {
        children->push_back(element);
    }

    void AddCallback(int type, int callbackID)
    {
        callbacks.insert(std::pair<Repentogon::CALLBACK_TYPE, int>(static_cast<Repentogon::CALLBACK_TYPE>(type), callbackID));
    }
};

struct Window {
    std::string name;
    Element* parentMenuItem;
    std::list<Element>* elements;

    Window(Element* menuItemParent, const char* windowName)
    {
        name = std::string(windowName);
        parentMenuItem = menuItemParent;
        elements = new std::list<Element>();
    }

    bool IsEnabled()
    {
        return parentMenuItem->isActive;
    }
};

struct CustomImGui {
    bool enabled;
    std::list<Element>* menuElements;
    std::list<Window>* windows;

    CustomImGui()
    {
        enabled = false;
        menuElements = new std::list<Element>();
        windows = new std::list<Window>();
    }

    bool AddElementToWindow(const char* windowName, const char* name, int type, const char* parentName) IM_FMTARGS(2)
    {
        for (auto window = windows->begin(); window != windows->end(); ++window) {
            if (strcmp(window->name.c_str(), windowName) != 0) {
                continue;
            }
            if ((parentName != NULL) && (parentName[0] == '\0')) { // no parent given / string empty
                window->elements->push_back(Element(name, type));
            } else {
                Element* parent = GetElement(parentName, window->elements);
                if (parent != NULL) {
                    parent->AddChild(Element(name, type));
                }
            }
            return true;
        }
        return false;
    }

    bool AddElementToMenu(const char* name, int type, const char* parentName) IM_FMTARGS(2)
    {
        if ((parentName != NULL) && (parentName[0] == '\0')) { // no parent given / string empty
            menuElements->push_back(Element(name, type));
            return true;
        } else {
            Element* parent = GetElement(parentName, menuElements);
            if (parent != NULL) {
                parent->AddChild(Element(name, type));
                return true;
            }
        }
        return false;
    }

    bool AddWindow(const char* parentName, const char* name) IM_FMTARGS(2)
    {
        Element* parent = GetElement(parentName, menuElements);
        if (parent != NULL) {
            windows->push_back(Window(parent, name));
            return true;
        }
        return false;
    }

    void RemoveWindow(const char* windowName) IM_FMTARGS(2)
    {
        for (auto window = windows->begin(); window != windows->end(); ++window) {
            if (strcmp(window->name.c_str(), windowName) == 0) {
                windows->erase(window);
            }
        }
    }

    bool AddCallbackToMenuElement(const char* parentName, int type, int callbackID) IM_FMTARGS(2)
    {
        Element* parent = GetElement(parentName, menuElements);
        if (parent != NULL) {
            parent->AddCallback(type, callbackID);
            return true;
        }
        return false;
    }

    bool AddCallbackToWindowElement(const char* windowName, const char* parentName, int type, int callbackID) IM_FMTARGS(2)
    {
        for (auto window = windows->begin(); window != windows->end(); ++window) {
            if (strcmp(window->name.c_str(), windowName) != 0) {
                continue;
            }
            Element* parent = GetElement(parentName, window->elements);
            if (parent != NULL) {
                parent->AddCallback(type, callbackID);
                return true;
            }
        }
        return false;
    }

    void RunCallbacks(Element* element) IM_FMTARGS(2)
    {
        for (auto callback = element->callbacks.begin(); callback != element->callbacks.end(); ++callback) {
            switch (callback->first) {
            case Repentogon::CALLBACK_TYPE::Clicked:
                if (ImGui::IsItemClicked()) {
                    element->clickCounter++;
                    RunCallback(element, callback->second);
                }
                break;
            case Repentogon::CALLBACK_TYPE::Hovered:
                if (ImGui::IsItemHovered()) {
                    RunCallback(element, callback->second);
                }
                break;
            case Repentogon::CALLBACK_TYPE::Active:
                if (ImGui::IsItemActive()) {
                    RunCallback(element, callback->second);
                }
                break;
            case Repentogon::CALLBACK_TYPE::Focused:
                if (ImGui::IsItemFocused()) {
                    RunCallback(element, callback->second);
                }
                break;
            case Repentogon::CALLBACK_TYPE::Visible:
                if (ImGui::IsItemVisible()) {
                    RunCallback(element, callback->second);
                }
                break;
            case Repentogon::CALLBACK_TYPE::Edited:
                if (ImGui::IsItemEdited()) {
                    RunCallback(element, callback->second);
                }
                break;
            case Repentogon::CALLBACK_TYPE::Activated:
                if (ImGui::IsItemActivated()) {
                    RunCallback(element, callback->second);
                }
                break;
            case Repentogon::CALLBACK_TYPE::Deactivated:
                if (ImGui::IsItemDeactivated()) {
                    RunCallback(element, callback->second);
                }
                break;
            case Repentogon::CALLBACK_TYPE::DeactivatedAfterEdit:
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    RunCallback(element, callback->second);
                }
                break;
            case Repentogon::CALLBACK_TYPE::ToggledOpen:
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

    Element* GetElement(const char* name, std::list<Element>* list)
    {
        for (auto element = list->begin(); element != list->end(); ++element) {
            if (strcmp(element->name.c_str(), name) == 0) {
                return &(*element);
            }
            Element* childResult = GetElement(name, element->children);
            if (childResult != NULL) {
                return childResult;
            }
        }
        return NULL;
    }

    void DrawMenu()
    {
        // if (ImGui::MenuItem("Custom ImGui", NULL, &enabled)) { }
        DrawMenuElements(menuElements);
    }

    void DrawWindows()
    {
        if (enabled && ImGui::Begin("Custom ImGui Menu", &enabled)) {
            if (ImGui::CollapsingHeader("Some initial Category")) {
                ImGui::Text("Initial text is coolis cool because:");
                ImGui::BulletText("Testing is easier");
                ImGui::BulletText("We see stuff");
            }
            ImGui::End();
        }

        for (auto window = windows->begin(); window != windows->end(); ++window) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 100));
            if (window->IsEnabled() && ImGui::Begin(window->name.c_str(), &window->parentMenuItem->isActive)) {
                DrawElements(window->elements);
                ImGui::End();
            }
        }
    }

    void DrawMenuElements(std::list<Element>* elements)
    {
        for (auto element = elements->begin(); element != elements->end(); ++element) {
            const char* name = element->name.c_str();

            switch (element->type) {
            case Repentogon::ELEMENT_TYPE::Menu:
                if (ImGui::BeginMenu(name)) {
                    RunCallbacks(&(*element));
                    DrawMenuElements(element->children);
                    ImGui::EndMenu();
                }
                break;
            case Repentogon::ELEMENT_TYPE::MenuItem:
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
            case Repentogon::ELEMENT_TYPE::CollapsingHeader:
                if (ImGui::CollapsingHeader(name)) {
                    RunCallbacks(&(*element));
                    DrawElements(element->children);
                }
                break;
            case Repentogon::ELEMENT_TYPE::TreeNode:
                if (ImGui::TreeNode(name)) {
                    RunCallbacks(&(*element));
                    DrawElements(element->children);
                }
                break;
            case Repentogon::ELEMENT_TYPE::Text:
                ImGui::Text(name);
                RunCallbacks(&(*element));
                break;
            case Repentogon::ELEMENT_TYPE::SeparatorText:
                ImGui::SeparatorText(name);
                RunCallbacks(&(*element));
                break;
            case Repentogon::ELEMENT_TYPE::BulletText:
                ImGui::BulletText(name);
                RunCallbacks(&(*element));
                break;
            case Repentogon::ELEMENT_TYPE::Separator:
                ImGui::Separator();
                RunCallbacks(&(*element));
                break;
            case Repentogon::ELEMENT_TYPE::SameLine:
                ImGui::SameLine();
                RunCallbacks(&(*element));
                break;
            case Repentogon::ELEMENT_TYPE::Button:
                ImGui::Button(name);
                RunCallbacks(&(*element));
                break;
            default:
                break;
            }
        }
    }
};
extern CustomImGui customImGui;
