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

void SDKUI::DrawUITestFrame(void) 
{
    if (ImGui::Begin("TestWindow"))
    {
        ImGui::Text("Sas");
        ImGui::Text("Private");
        if (ImGui::Button("Close Program"))
        {
            Console->Execute("quit");
        }
    }   
    ImGui::End();
    ImGui::Render();
}

void SDKUI::End(void) 
{ 
    if (ImGui::GetDrawData())
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

