#include "stdafx.h"
#include "SDKUI.h"
#include "../../xrCore/Imgui/imgui.h"
#include "../../xrCore/Imgui/imgui_impl_sdl.h"
#include "../../xrCore/Imgui/imgui_impl_dx11.h"
#include "xrEngine/XR_IOConsole.h"

void SDKUI::Begin(void)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL2_NewFrame(Device.m_sdlWnd);
    ImGui::NewFrame();
}

void SDKUI::KeyBoardMessages(void)
{
    // @ Хендлим шорткаты
    if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_F12))
    {
        Console->Execute("screenshot 1");
        SDKUI_Log::Widget().AddText("Screenshot was made!", SDKErrorType::special);
        return;
    }

    if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_LCTRL) || ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_RCTRL))
    {
        if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_L))
        {
            if (SDKUI_Log::Widget().GetVisible() == false)
            {
                SDKUI_Log::Widget().Show();
                SDKUI_Log::Widget().SetColor(unimportant);
                SDKUI_Log::Widget().AddText("The logger show");
                return;
            }
            else
            {
                SDKUI_Log::Widget().Hide();
                SDKUI_Log::Widget().SetColor(unimportant);
                SDKUI_Log::Widget().AddText("The logger hide");
                return;
            }
        }
    }
}

void SDKUI::DrawMainMenuBar(void)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Quit"))
            {
                Console->Execute("quit");
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::BeginMenu("Others"))
            {
                if (ImGui::MenuItem("Show Log", "CTRL + L"))
                {
                    if (SDKUI_Log::Widget().GetVisible() == false)
                    {
                        SDKUI_Log::Widget().Show();
                        SDKUI_Log::Widget().SetColor(unimportant);
                        SDKUI_Log::Widget().AddText("The logger show");
                    }
                }

                if (ImGui::MenuItem("ScreenShot", "F12"))
                {
                    Console->Execute("screenshot 1");
                    SDKUI_Log::Widget().SetColor(special);
                    SDKUI_Log::Widget().AddText("Screenshot was made!");
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void SDKUI::Draw(void)
{
    //   ImGui::ShowDemoWindow();
    KeyBoardMessages();
    DrawMainMenuBar();
    SDKUI_Log::Widget().Draw();
    ImGui::Render();
}

void SDKUI::End(void)
{
    if (ImGui::GetDrawData())
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

/*
    PRIVATE SECTION
*/
