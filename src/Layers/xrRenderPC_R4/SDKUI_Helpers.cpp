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

            /*
            
            ImGuiListClipper clip;
            clip.Begin(LineOffSet.Size);

            while (clip.Step())
            {
                for (int line_no = clip.DisplayStart; line_no < clip.DisplayEnd; ++line_no)
                {
                    const char* line_start = buf.begin() + LineOffSet[line_no];
                    if (strstr(line_start, "ERROR"))
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 0, 0, 255));
                    else if (strstr(line_start, "WARNING"))
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(219, 234, 56, 255));
                    else if (strstr(line_start, "INFO"))
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(26, 62, 98, 255));
                    else if (strstr(line_start, "OLD"))
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(113, 113, 113, 255));
                    else
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 255, 255));

                    const char* line_end = (line_no + 1 < LineOffSet.Size) ? (buf.begin() + LineOffSet[line_no + 1] - 1)
            : buf.end();
                    
                    ImGui::PopStyleColor();
                }
            }
            clip.End();
            */
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }
}
