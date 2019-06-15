#include "stdafx.h"
#include "SDKUI_Helpers.h"
#include <chrono>
#include <ctime>
#include "../../xrEngine/SDK_Camera.h"
#include "SDK_GizmoManager.h"
namespace Cordis
{
    namespace SDK
    {
    /*
        @ UI_Log
    */

#pragma region Logger
    void SDKUI_Log::Draw(void)
    {
        if (!this->m_is_visible)
            return;

        ImGui::SetNextWindowPos(ImVec2(this->m_currentposition_x, this->m_currentposition_y), ImGuiCond_Once);
        if (ImGui::Begin("Log Window", &this->m_is_visible, ImVec2(this->m_currentsize_x, this->m_currentsize_y), 0.5f,
                this->m_flag))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImColor col = {1.0f, 1.0f, 1.0f, 1.0f};
            for (xr_string text : this->m_string_buffer)
            {
                if (strstr(text.c_str(), "|ERROR"))
                {
                    col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);

                    if (!this->m_show_errors)
                        continue;
                }
                else if (strstr(text.c_str(), "|WARNING"))
                {
                    col = ImColor(0.8f, 0.9f, 0.2f, 1.0f);

                    if (!this->m_show_warnings)
                        continue;
                }
                else if (strstr(text.c_str(), "|OLD"))
                {
                    col = ImColor(0.4f, 0.4f, 0.4f, 1.0f);

                    if (!this->m_show_unimportant)
                        continue;
                }
                else if (strstr(text.c_str(), "|SUCCESSFUL"))
                {
                    col = ImColor(0.2f * 2, 0.4f * 2, 0.1f * 2, 1.0f);

                    if (!this->m_show_good)
                        continue;
                }
                else if (strstr(text.c_str(), "|INFO"))
                {
                    col = ImColor(0.0f, 0.0f, 1.0f, 1.0f);

                    if (!this->m_show_special)
                        continue;
                }
                else
                {
                    col = {1.0f, 1.0f, 1.0f};

                    if (!this->m_show_default)
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
                        this->m_show_errors = true;
                        this->m_show_warnings = true;
                        this->m_show_default = true;
                        this->m_show_special = true;
                        this->m_show_unimportant = true;
                        this->m_show_good = true;
                    }

                    if (ImGui::MenuItem("Errors"))
                    {
                        this->m_show_errors = true;
                        this->m_show_warnings = false;
                        this->m_show_default = false;
                        this->m_show_special = false;
                        this->m_show_unimportant = false;
                        this->m_show_good = false;
                    }

                    if (ImGui::MenuItem("Warnings"))
                    {
                        this->m_show_errors = false;
                        this->m_show_warnings = true;
                        this->m_show_default = false;
                        this->m_show_special = false;
                        this->m_show_unimportant = false;
                        this->m_show_good = false;
                    }

                    if (ImGui::MenuItem("UnImportant"))
                    {
                        this->m_show_errors = false;
                        this->m_show_warnings = false;
                        this->m_show_default = false;
                        this->m_show_special = false;
                        this->m_show_unimportant = true;
                        this->m_show_good = false;
                    }

                    if (ImGui::MenuItem("Successful"))
                    {
                        this->m_show_errors = false;
                        this->m_show_warnings = false;
                        this->m_show_default = false;
                        this->m_show_special = false;
                        this->m_show_unimportant = false;
                        this->m_show_good = true;
                    }

                    if (ImGui::MenuItem("Default"))
                    {
                        this->m_show_errors = false;
                        this->m_show_warnings = false;
                        this->m_show_default = true;
                        this->m_show_special = false;
                        this->m_show_unimportant = false;
                        this->m_show_good = false;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::MenuItem("Clear"))
                {
                    this->m_string_buffer.clear();
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
#pragma endregion
    /*
        @ UI_Overlay
    */
#pragma region Overlay
    void SDKUI_Overlay::Draw(void)
    {
        if (!this->m_is_visible)
            return;

        ImGui::SetNextWindowPos(ImVec2(30, 30), ImGuiCond_Once);
        if (ImGui::Begin("Overlay", &this->m_is_visible, ImVec2(200, 500), 0.005f, this->m_flag))
        {
            if (this->m_showonly_sdkinfo)
            {
                ImGui::Text("Editor - \n"); // @ Сделать нормальный вывод и того в каком редакторе мы сейчас находимся
                ImGui::Text("FPS: %d \n");
            }

            if (this->m_showonly_projectinfo)
            {
                if (this->m_showonly_sdkinfo)
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
                            if (ImGui::ColorEdit3("Grid Color", GridOptions::col))
                            {
                                SDKUI::UI().setHoveredColorEdit(true);
                            }

                            if (ImGui::ColorEdit4("Background Scene color: ", GridOptions::col_background))
                            {
                                SDKUI::UI().setHoveredColorEdit(true);
                            }


                            static xr_string a = "100|10";
                            ImGui::SliderInt("Grid Type", &this->m_gridtype, 0, 5, a.c_str());
                            switch (this->m_gridtype)
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
                            this->m_savedposition = Device.vCameraPosition;
                            Device.vCameraPosition.set(0, 0, 0);
                        }

                        if (ImGui::Button("Previous Location"))
                        {
                            Device.vCameraPosition.set(this->m_savedposition);
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

            if (this->m_showonly_sysinfo)
            {
                if (this->m_showonly_projectinfo || this->m_showonly_sdkinfo)
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
                        this->m_showonly_sdkinfo = true;
                        this->m_showonly_projectinfo = false;
                        this->m_showonly_sysinfo = false;
                    }

                    if (ImGui::MenuItem("Project info"))
                    {
                        this->m_showonly_projectinfo = true;
                        this->m_showonly_sysinfo = false;
                        this->m_showonly_sdkinfo = false;
                    }

                    if (ImGui::MenuItem("System info"))
                    {
                        this->m_showonly_sysinfo = true;
                        this->m_showonly_sdkinfo = false;
                        this->m_showonly_projectinfo = false;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Show:"))
                {
                    if (ImGui::MenuItem("All"))
                    {
                        this->m_showonly_sysinfo = true;
                        this->m_showonly_sdkinfo = true;
                        this->m_showonly_projectinfo = true;
                    }

                    if (ImGui::MenuItem("SDK Info"))
                    {
                        this->m_showonly_sdkinfo = true;
                    }
                    if (ImGui::MenuItem("Project info"))
                    {
                        this->m_showonly_projectinfo = true;
                    }
                    if (ImGui::MenuItem("System info"))
                    {
                        this->m_showonly_sysinfo = true;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Hide:"))
                {
                    if (ImGui::MenuItem("SDK Info"))
                    {
                        this->m_showonly_sdkinfo = false;
                    }
                    if (ImGui::MenuItem("Project info"))
                    {
                        this->m_showonly_projectinfo = false;
                    }
                    if (ImGui::MenuItem("System info"))
                    {
                        this->m_showonly_sysinfo = false;
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::MenuItem("Close"))
                {
                    this->m_is_visible = false;
                }

                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }
#pragma endregion

#pragma region CameraHelper
    void SDKUI_CameraHelper::Draw(void)
    {
        if (!this->m_is_visible)
            return;

        if (ImGui::Begin("Camera Manager", &this->m_is_visible, ImGuiWindowFlags_AlwaysAutoResize))
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
#pragma endregion

#pragma region SceneOptions
    SDKUI_SceneOptions::~SDKUI_SceneOptions(void) {}

    void SDKUI_SceneOptions::Draw(void)
    {
        if (!this->m_is_visible)
            return;

        if (ImGui::Begin("Scene Preferences", &this->m_is_visible, this->m_flag))
        {
            ImGui::End();
        }
    }

#pragma endregion

    }
}
