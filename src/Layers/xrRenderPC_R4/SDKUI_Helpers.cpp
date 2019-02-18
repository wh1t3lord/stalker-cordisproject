#include "stdafx.h"
#include "SDKUI_Helpers.h"
#include <chrono>
#include <ctime>
/*
    @ UI_Log
*/
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

                    if (!bShowErrors)
                        continue;
                }
                else if (strstr(text.c_str(), "|WARNING"))
                {
                    col = ImColor(0.8f, 0.9f, 0.2f, 1.0f);

                    if (!bShowWarnings)
                        continue;
                }
                else if (strstr(text.c_str(), "|OLD"))
                {
                    col = ImColor(0.4f, 0.4f, 0.4f, 1.0f);

                    if (!bShowUn)
                        continue;
                }
                else if (strstr(text.c_str(), "|SUCCESSFUL"))
                {
                    col = ImColor(0.2f * 2, 0.4f * 2, 0.1f * 2, 1.0f);

                    if (!bShowGood)
                        continue;
                }
                else if (strstr(text.c_str(), "|INFO"))
                {
                    col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);

                    if (!bShowSpecial)
                        continue;
                }
                else
                {
                    col = {1.0f, 1.0f, 1.0f};

                    if (!bShowDefault)
                        continue;
                }

                ImGui::TextColored(col, text.c_str());
            }
            ImGui::PopStyleVar();

            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::BeginMenu("Show (Only):"))
                {
                    if (ImGui::MenuItem("All"))
                    {
                        bShowErrors = true;
                        bShowWarnings = true;
                        bShowDefault = true;
                        bShowSpecial = true;
                        bShowUn = true;
                        bShowGood = true;
                    }

                    if (ImGui::MenuItem("Errors"))
                    {
                        bShowErrors = true;
                        bShowWarnings = false;
                        bShowDefault = false;
                        bShowSpecial = false;
                        bShowUn = false;  
                        bShowGood = false;
                    }

                    if (ImGui::MenuItem("Warnings"))
                    {
                        bShowErrors = false;
                        bShowWarnings = true;
                        bShowDefault = false;
                        bShowSpecial = false;
                        bShowUn = false;
                        bShowGood = false;
                    }

                    if (ImGui::MenuItem("UnImportant"))
                    {
                        bShowErrors = false;
                        bShowWarnings = false;
                        bShowDefault = false;
                        bShowSpecial = false;
                        bShowUn = true;
                        bShowGood = false;
                    }

                    if (ImGui::MenuItem("Successful"))
                    {
                        bShowErrors = false;
                        bShowWarnings = false;
                        bShowDefault = false;
                        bShowSpecial = false;
                        bShowUn = false;
                        bShowGood = true;
                    }

                    if (ImGui::MenuItem("Default"))
                    {
                        bShowErrors = false;
                        bShowWarnings = false;
                        bShowDefault = true;
                        bShowSpecial = false;
                        bShowUn = false;
                        bShowGood = false;
                    }

                    ImGui::EndMenu();
                }

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

/*
    @ UI_Overlay
*/

void SDKUI_Overlay::Draw(void)
{
    if (bShow)
    {
        ImGuiWindowFlags flag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground;
        ImGui::SetNextWindowPos(ImVec2(30, 30), ImGuiCond_Once);
        if (ImGui::Begin("Overlay", &bShow, ImVec2(200, 500), 0.005f, flag))
        {
            if (bShowOnlySDKInfo)
            {
                ImGui::Text("Level Editor: \n"); // @ Сделать нормальный вывод и того в каком редакторе мы сейчас находимся
                ImGui::Text("FPS: %d \n");
                ImGui::Text("Total tris: \n");
                ImGui::Text("Total vertecies: \n");
                ImGui::Text("Total something: \n");
            }

            if (bShowOnlyProjectInfo)
            {
                if (bShowOnlySDKInfo)
                    ImGui::Separator();

                ImGui::Text("Project info: \n");
                ImGui::Text("Scene name: \n");
                ImGui::Text("Total:\n");
                ImGui::Text("   objects   \n");
                ImGui::Text("   spawn_elements   \n");
                ImGui::Text("   way_points   \n");
                ImGui::Text(" ");
            }

            if (bShowOnlySysInfo)
            {
                if (bShowOnlyProjectInfo || bShowOnlySDKInfo)
                    ImGui::Separator();

                ImGui::Text("System info: \n");
                std::chrono::system_clock::time_point x = std::chrono::system_clock::now();
                std::time_t t = std::chrono::system_clock::to_time_t(x);
                xr_string result = std::ctime(&t);
                xr_string r2 = result.substr(result.rfind(' ') - 8);
                r2.erase(r2.rfind(' '));
                xr_string& r1 = result.erase(result.rfind(' '));
                r1.erase(r1.rfind(' '));

                ImGui::Text("Current Date: %s", result.c_str());
                ImGui::Text("Current time: %s", r2.c_str());
            }


            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::BeginMenu("Show only:"))
                {
                    if (ImGui::MenuItem("SDK info"))
                    {
                        bShowOnlySDKInfo = true;
                        bShowOnlyProjectInfo = false;
                        bShowOnlySysInfo = false;
                    }

                    if (ImGui::MenuItem("Project info"))
                    {
                        bShowOnlyProjectInfo = true;
                        bShowOnlySysInfo = false;
                        bShowOnlySDKInfo = false;
                    }

                    if (ImGui::MenuItem("System info"))
                    {
                        bShowOnlySysInfo = true;
                        bShowOnlySDKInfo = false;
                        bShowOnlyProjectInfo = false;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Show:"))
                {
                    if (ImGui::MenuItem("All"))
                    {
                        bShowOnlySysInfo = true;
                        bShowOnlySDKInfo = true;
                        bShowOnlyProjectInfo = true;
                    }

                    if (ImGui::MenuItem("SDK Info"))
                    {
                        bShowOnlySDKInfo = true;
                    }
                    if (ImGui::MenuItem("Project info"))
                    {
                        bShowOnlyProjectInfo = true;
                    }
                    if (ImGui::MenuItem("System info"))
                    {
                        bShowOnlySysInfo = true;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Hide:"))
                {
                    if (ImGui::MenuItem("SDK Info"))
                    {
                        bShowOnlySDKInfo = false;
                    }
                    if (ImGui::MenuItem("Project info"))
                    {
                        bShowOnlyProjectInfo = false;
                    }
                    if (ImGui::MenuItem("System info"))
                    {
                        bShowOnlySysInfo = false;
                    }
                    ImGui::EndMenu();
                }



                if (ImGui::MenuItem("Close"))
                {
                    bShow = false;
                }

                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }
}
