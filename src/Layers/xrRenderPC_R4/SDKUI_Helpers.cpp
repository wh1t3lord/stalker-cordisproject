#include "stdafx.h"
#include "SDKUI_Helpers.h"
#include <chrono>
#include <ctime>
#include "../../xrEngine/SDK_Camera.h"
#include "SDK_GizmoManager.h"
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
                ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, col.Value);
                ImGui::TextWrapped(text.c_str());
                ImGui::PopStyleColor();
                
              //  ImGui::TextColored(col, text.c_str());
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
        ImGui::SetScrollHereY();
        ImGui::End();
    }
}

/*
    @ UI_Overlay
*/

void SDKUI_Overlay::Draw(void)
{
    if (this->bShow)
    {
        ImGui::SetNextWindowPos(ImVec2(30, 30), ImGuiCond_Once);
        if (ImGui::Begin("Overlay", &this->bShow, ImVec2(200, 500), 0.005f, flag))
        {
            if (bShowOnlySDKInfo)
            {
                ImGui::Text("Editor - \n"); // @ Сделать нормальный вывод и того в каком редакторе мы сейчас находимся
                ImGui::Text("FPS: %d \n");
            }

            if (bShowOnlyProjectInfo)
            {
                if (bShowOnlySDKInfo)
                    ImGui::Separator();
                if (ImGui::TreeNode("Project info:"))
                {
                    ImGui::Text("Scene name: \n");
                    ImGui::Checkbox("Draw Grid", &EditorPref::GetInstance().bDrawGrid);
                    if (ImGui::TreeNode("Total:"))
                    {
                        ImGui::Text("objects - ");
                        ImGui::Text("tris - ");
                        ImGui::Text("vertices - ");

                        ImGui::TreePop();
                    }

                    if (EditorPref::GetInstance().bDrawGrid)
                    {
                        if (ImGui::TreeNode("Grid:"))
                        {
                            ImGui::ColorEdit3("Grid Color", GridOptions::col);
                            ImGui::ColorEdit4("Background Scene color: ", GridOptions::col_background);
                            static xr_string a = "100|10";
                            ImGui::SliderInt("Grid Type", &this->iGridType, 0, 5, a.c_str());
                            switch (this->iGridType)
                            {
                            case 0:
                            {
                                GridOptions::Size = 100;
                                GridOptions::separator = 10;
                                a = "100|10";
                                break;
                            }
                            case 1:
                            {
                                GridOptions::Size = 100;
                                GridOptions::separator = 20;
                                a = "100|20";
                                break;
                            }
                            case 2:
                            {
                                GridOptions::Size = 100;
                                GridOptions::separator = 50;
                                a = "100|50";
                                break;
                            }
                            case 3:
                            {
                                GridOptions::Size = 100;
                                GridOptions::separator = 100;
                                a = "100|100";
                                break;
                            }
                            case 4:
                            {
                                GridOptions::Size = 10;
                                GridOptions::separator = 1;
                                a = "10|1";
                                break;
                            }
                            case 5:
                            {
                                GridOptions::Size = 10000;
                                GridOptions::separator = 10;
                                a = "10000|10";
                                break;
                            }
                            }
                            ImGui::TreePop();
                        }
                    }

                    if (ImGui::TreeNode("Camera: "))
                    {
                        if (ImGui::Button("Start Location"))
                        {
                            this->vSavedPosition = Device.vCameraPosition;
                            Device.vCameraPosition.set(0, 0, 0);
                        }

                        if (ImGui::Button("Previous Location"))
                        {
                            Device.vCameraPosition.set(this->vSavedPosition);
                        }

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Gizmo: "))
                    {
                        ImGui::DragInt("Gizmo Size", &g_size_gizmoline, 1.0f, 5, 20);
                        ImGui::DragFloat("Gizmo Sens", &SDK_GizmoManager::GetInstance().m_speed, 0.001f, 0.001f, 1.0f);
                        ImGui::TreePop();
                    }


                    ImGui::TreePop();
                }
               
            }

            if (bShowOnlySysInfo)
            {
                if (bShowOnlyProjectInfo || bShowOnlySDKInfo)
                    ImGui::Separator();

                std::chrono::system_clock::time_point x = std::chrono::system_clock::now();
                std::time_t t = std::chrono::system_clock::to_time_t(x);
                xr_string result = std::ctime(&t);
                xr_string r2 = result.substr(result.rfind(' ') - 8);
                r2.erase(r2.rfind(' '));
                xr_string& r1 = result.erase(result.rfind(' '));
                r1.erase(r1.rfind(' '));

                ImGui::Text("%s | %s", result.c_str(), r2.c_str());
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

void SDKUI_CameraHelper::Draw(void)
{
    //   ImGuiWindowFlags flag =
    if (this->bShow)
    {
        if (ImGui::Begin("Camera Manager", &this->bShow, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Position:");
            ImGui::DragFloat("PosX", &Device.vCameraPosition.x, 1.0f);
            ImGui::SameLine();
            ImGui::DragFloat("PosY", &Device.vCameraPosition.y, 1.0f);
            ImGui::SameLine();
            ImGui::DragFloat("PosZ", &Device.vCameraPosition.z, 1.0f);
            ImGui::Separator();
            ImGui::Text("Direction: (read-only)");
            ImGui::DragFloat("DirX", &Device.vCameraDirection.x, 0.0f);
            ImGui::SameLine();
            ImGui::DragFloat("DirY", &Device.vCameraDirection.y, 0.0f);
            ImGui::SameLine();
            ImGui::DragFloat("DirZ", &Device.vCameraDirection.z, 0.0f);
            ImGui::Separator();
            ImGui::Text("Normal: (read-only)");
            ImGui::DragFloat("NorX", &Device.vCameraTop.x, 0.0f);
            ImGui::SameLine();
            ImGui::DragFloat("NorY", &Device.vCameraTop.y, 0.0f);
            ImGui::SameLine();
            ImGui::DragFloat("NorZ", &Device.vCameraTop.z, 0.0f);
            ImGui::Separator();
            ImGui::Text("Sens: ");
            ImGui::DragFloat("koef", &SDK_Camera::GetInstance().fSens, 0.0001f, 0.0f, 1.0f);
            ImGui::Text("System attributes: ");
            ImGui::DragFloat("FOV", &SDK_Camera::GetInstance().f_fov, 1.0f, 1.0f, 90.0f);
            ImGui::DragFloat("ASPECT", &SDK_Camera::GetInstance().f_aspect, 0.1f, 0.1f, 1.0f);
            ImGui::DragFloat("SPEED", &SDK_Camera::GetInstance().fMoveSpeed, 1.0f, 1.0f, 10000.0f);
            ImGui::DragFloat("NearPlane (read-only)", &SDK_Camera::GetInstance().fNear, 0.0f);
            ImGui::DragFloat("FarPlane (read-only)", &SDK_Camera::GetInstance().fFar, 1.0f, 1.0f, 3000.0f);
        }

        ImGui::End();
    }
}
