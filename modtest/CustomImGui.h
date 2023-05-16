#include "imgui.h"
#include <iostream>
#include <list>
#include <sstream>
#include <string>

namespace Repentogon {
enum class ELEMENT_TYPE {
    CollapsingHeader,
    TreeNode,
    Text,
    SeparatorText,
    BulletText,
    Separator,
    SameLine,
    Button,
};
}

struct Element {
    std::string name;
    Repentogon::ELEMENT_TYPE type;
    std::list<Element>* children;

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
};

struct CustomImGui {
    bool enabled;
    std::list<Element>* elements;

    CustomImGui()
    {
        enabled = false;
        elements = new std::list<Element>();
    }

    void AddElement(const char* name, int type, const char* parentName) IM_FMTARGS(2)
    {
        if ((parentName != NULL) && (parentName[0] == '\0')) { // no parent given / string empty
            elements->push_back(Element(name, type));
        } else {
            Element* parent = GetElement(parentName, elements);
            if (parent != NULL) {
                parent->AddChild(Element(name, type));
            }
        }
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

    void Draw()
    {
        if (ImGui::Begin("Custom ImGui Menu", &enabled)) {
            if (ImGui::CollapsingHeader("Some initial Category")) {
                ImGui::Text("Initial text is coolis cool because:");
                ImGui::BulletText("Testing is easier");
                ImGui::BulletText("We see stuff");
            }
            DrawElements(elements);

            ImGui::End();
        }
    }

    void DrawElements(std::list<Element>* elements)
    {
        for (auto element = elements->begin(); element != elements->end(); ++element) {
            const char* name = element->name.c_str();
            switch (element->type) {
            case Repentogon::ELEMENT_TYPE::CollapsingHeader:
                if (ImGui::CollapsingHeader(name)) {
                    DrawElements(element->children);
                }
                break;
            case Repentogon::ELEMENT_TYPE::TreeNode:
                if (ImGui::TreeNode(name)) {
                    DrawElements(element->children);
                }
                break;
            case Repentogon::ELEMENT_TYPE::Text:
                ImGui::Text(name);
                break;
            case Repentogon::ELEMENT_TYPE::SeparatorText:
                ImGui::SeparatorText(name);
                break;
            case Repentogon::ELEMENT_TYPE::BulletText:
                ImGui::BulletText(name);
                break;
            case Repentogon::ELEMENT_TYPE::Separator:
                ImGui::Separator();
                break;
            case Repentogon::ELEMENT_TYPE::SameLine:
                ImGui::SameLine();
                break;
            case Repentogon::ELEMENT_TYPE::Button:
                ImGui::Button(name);
                break;
            default:
                break;
            }
        }
    }
};
extern CustomImGui customImGui;
