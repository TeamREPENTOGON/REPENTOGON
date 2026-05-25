#include "ExtraRenderSteps.h"

#include "IsaacRepentance.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"
#include "Log.h"
#include <cmath>
#include <windows.h>

#undef max
#undef min

extern bool menuShown;
static bool& ImGui_Shown = menuShown;

namespace {
    struct MouseControls
    {
        private: Vector m_position = Vector(0.0, 0.0);
        private: bool m_leftPressed = false;
        private: bool m_willRender = false;
        private: bool m_forceRender = false;

        public: MouseControls(const Manager& manager, const _GLFWwindow& window)
        {
            HWND hwnd = (HWND)window.HWND;
            RECT rc;
            GetClientRect(hwnd, &rc);
            int windowWidth  = rc.right;
            int windowHeight = rc.bottom;
            float pointScaleX = g_OrthographicWidth / windowWidth;
            float pointScaleY = g_OrthographicHeight / windowHeight;

            this->m_position.x = (float)window.virtualCursorPosX * pointScaleX;
            this->m_position.y = (float)window.virtualCursorPosY * pointScaleY;
            this->m_leftPressed = window.mouseButtons[0] == 1;
            this->m_willRender = manager._optionsConfig._enableMouseControl;
        }

        public: const Vector& GetPosition() const { return this->m_position; }
        public: bool IsLeftPressed() const { return this->m_leftPressed; }
        public: bool CursorWillRender() const { return this->m_willRender; }
        public: bool GetForceRender() const { return this->m_forceRender; }

        public: void SetForceRender() { this->m_forceRender = true; }
    };

    class _ErrorDisplay
    {
        private: std::string m_mainMessage;
        private: int m_messagePriority = 0;
        private: uint32_t m_activeFrame = 0; // use activation frame, so we don't have to update it ourself
        private: bool m_active = false;

        public: void RaiseError(const std::string& message, int priority)
        {
            if (this->m_active && priority <= this->m_messagePriority)
            {
                return;
            }

            this->m_mainMessage = message;
            this->m_activeFrame = g_Manager->_framecount;
            this->m_active = true;
        }

        public: void Tick(MouseControls& mouseControls)
        {
            if (!this->m_active)
            {
                return;
            }

            // Update
            if (ImGui_Shown)
            {
                this->m_active = false;
                return;
            }

            Manager* manager = g_Manager;
            ColorMod& cursorColor = manager->_cursorSprite._color;
            Font& mainTextFont = manager->_font6_PFTempestaSevenCondensed;
            Font& clickFont = manager->_font8_LuaMiniOutlined;

            const char* mainString = this->m_mainMessage.c_str();
            const char* clickString = "Click this message for more info.";
            int mainFontWidth = mainTextFont.GetStringWidth(mainString);
            int clickFontWidth = clickFont.GetStringWidth(clickString);
            float screenCenterX = g_WIDTH / 2;

            // Click Detection
            int boxWidth = std::max(mainFontWidth, clickFontWidth);
            float boundingBoxLeftX = screenCenterX - (boxWidth / 2.0f);
            constexpr float boundingBoxLeftY = 0.0f;
            float boundingBoxRightX = screenCenterX + (boxWidth / 2.0f);
            constexpr float boundingBoxRightY = 18.0f;

            Vector mousePos = mouseControls.GetPosition();
            if ((boundingBoxLeftX < mousePos.x && boundingBoxLeftY < mousePos.y) && (mousePos.x < boundingBoxRightX && mousePos.y < boundingBoxRightY))
            {
                if (mouseControls.IsLeftPressed())
                {
                    ImGui_Shown = true;
                    this->m_active = false;
                    return; // Don't render
                }

                // render the cursor as red (but only when hovering over the message, not when pressed)
                cursorColor._tint[0] = 1.0;
                cursorColor._tint[1] = 0.5;
                cursorColor._tint[2] = 0.5;
            }

            // Render
            constexpr uint32_t PULSE_FRAMES = 483; // 8 pulsations
            uint32_t frame = manager->_framecount - this->m_activeFrame;
    
            float redSuppression; // how much the color red is suppressed (1 no red, 0 full red)
            if (frame < PULSE_FRAMES)
            {
                // generate a pulsating red suppression value
                float pulseSpeed = 200.0f / (frame + 1500.0f); // slows down with time
                float pulseValue = std::sin(frame * pulseSpeed);
                redSuppression = pulseValue / 2.0f + 0.5f; // remap value to [0, 1] range
            }
            else
            {
                redSuppression = 0.0f;
            }
            
            constexpr float ALPHA_DECAY_RATE = 0.7f / PULSE_FRAMES;
            float alpha =  1.0f - ALPHA_DECAY_RATE * frame;
            alpha = std::max(alpha, 0.3f);

            FontSettings settings;
            settings._align = 0;
            KColor color = KColor(1.0, redSuppression, redSuppression, alpha);

            mainTextFont.DrawString(mainString, Vector(screenCenterX - (mainFontWidth / 2.0f), 0), Vector(1.0, 1.0), &color, &settings);
            clickFont.DrawString(clickString, Vector(screenCenterX - (clickFontWidth / 2.0f), 7.0), Vector(1.0, 1.0), &color, &settings);

            if (!mouseControls.CursorWillRender())
            {
                float currentAlpha = cursorColor._tint[3];
                cursorColor._tint[3] = std::min(currentAlpha, alpha);
                mouseControls.SetForceRender();
            }
        }
    };
}

#pragma region Data

static _ErrorDisplay s_errorDisplay;
static ColorMod s_preCursorRenderColor;

#pragma endregion // Data

// Force Cursor to be initialized, this doesn't cause problems just makes the resource usable.
static void Patch_ManagerInit_PreInitCursor()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Manager_Init_PreInitCursor);
    ZHL::Log("[REPENTOGON] Patching Manager::Init for ExtraRenderSteps at %p\n", addr);

    ASMPatch patch;
    patch.AddBytes("\x90\x90\x90\x90\x90\x90"); // NOP the jump for _enableMouseControl option
    sASMPatcher.FlatPatch((void*)addr, &patch);
}

static void __stdcall asm_pre_cursor_render()
{
    auto* manager = g_Manager;
    auto& graphics = g_KAGE_Graphics_Manager;
    
    auto* window = graphics._window;
    ANM2& cursorSprite = manager->_cursorSprite;
    s_preCursorRenderColor = cursorSprite._color;

    MouseControls mouseControls = MouseControls(*manager, *window);

    s_errorDisplay.Tick(mouseControls);

    // Attempt force render
    if (!mouseControls.CursorWillRender() && mouseControls.GetForceRender())
    {
        Vector position = mouseControls.GetPosition();
        Vector vectorZero = Vector(0, 0);
        cursorSprite.Render(&position, &vectorZero, &vectorZero);
        cursorSprite._color = s_preCursorRenderColor;
    }
}

static void Patch_ManagerRender_PreCursorRender()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Manager_Render_PreCursorRender);
    ZHL::Log("[REPENTOGON] Patching Manager::Render for ExtraRenderSteps at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 7;
    constexpr size_t RESTORED_BYTES = 7;

    patch.PreserveRegisters(savedRegisters)
        .AddInternalCall(asm_pre_cursor_render)
        .RestoreRegisters(savedRegisters)
        .AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

static void __stdcall asm_post_cursor_render()
{
    ANM2& cursorSprite = g_Manager->_cursorSprite;
    cursorSprite._color = s_preCursorRenderColor;
}

static void Patch_ManagerRender_CursorRender()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Manager_Render_OnCursorRender);
    ZHL::Log("[REPENTOGON] Patching Manager::Render for ExtraRenderSteps at %p\n", addr);

    ASMPatch patch;

    intptr_t callAddr = addr;
    intptr_t resumeAddr = addr + 5;
    int32_t call_rel32 = *(int32_t*)(callAddr + 1);
    intptr_t calleeAddress = callAddr + 5 + call_rel32;

    patch.AddInternalCall((void*)calleeAddress) // restore call to assign
        .AddInternalCall((void*)asm_post_cursor_render)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

#pragma region API

void ExtraRenderSteps::ErrorDisplay::RaiseError(const std::string &message, int priority)
{
    s_errorDisplay.RaiseError(message, priority);
}

#pragma endregion // API

void ExtraRenderSteps::detail::ApplyPatches()
{
    Patch_ManagerInit_PreInitCursor();
    Patch_ManagerRender_PreCursorRender();
    Patch_ManagerRender_CursorRender();
}