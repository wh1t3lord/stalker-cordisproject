#include "stdafx.h"
#include "SDKUI_Helpers.h"

void SDKUI_Log::Draw(void)
{
    if (bShow)
    {
        ImGui::SetNextWindowPos(ImVec2(CurrentPosX, CurrentPosY), ImGuiCond_Once);
        if (ImGui::Begin("Log Window", &bShow, ImVec2(CurrentSizeX, CurrentSizeY), 0.5f, wndFlags))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImColor col = {1.0f, 1.0f, 1.0f, 1.0f};
            for (xr_string text : buff)
            {
                if (strstr(text.c_str(), "|ERROR"))
                {
                    col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
                }
                else if (strstr(text.c_str(), "|WARNING"))
                {
                    col = ImColor(0.8f, 0.9f, 0.2f, 1.0f);
                }
                else if (strstr(text.c_str(), "|OLD"))
                {
                    col = ImColor(0.4f, 0.4f, 0.4f, 1.0f);
                }
                else if (strstr(text.c_str(), "|SUCCESSFUL"))
                {
                    col = ImColor(0.2f * 2, 0.4f * 2, 0.1f * 2, 1.0f);
                }
                else if (strstr(text.c_str(), "|INFO"))
                {
                    col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);
                }
                else
                    col = {1.0f, 1.0f, 1.0f};

                    ImGui::TextColored(col, text.c_str());
            }
            ImGui::PopStyleVar();

            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Clear"))
                {
                    buff.clear();
                }

                if (ImGui::MenuItem("Close"))
                {
                    Hide();
                }

                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }
}
