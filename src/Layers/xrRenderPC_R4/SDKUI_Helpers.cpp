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
        if (ImGui::Begin(SDK_Names::getInstance().getName("st_label_manager_log").c_str(), &this->m_is_visible, ImVec2(this->m_currentsize_x, this->m_currentsize_y), 0.5f,
                this->m_flag))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImColor col = {1.0f, 1.0f, 1.0f, 1.0f};
            for (xr_string text : this->m_string_buffer)
            {
                if (strstr(text.c_str(), u8"|ERROR"))
                {
                    col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);

                    if (!this->m_show_errors)
                        continue;
                }
                else if (strstr(text.c_str(), u8"|WARNING"))
                {
                    col = ImColor(0.8f, 0.9f, 0.2f, 1.0f);

                    if (!this->m_show_warnings)
                        continue;
                }
                else if (strstr(text.c_str(), u8"|OLD"))
                {
                    col = ImColor(0.4f, 0.4f, 0.4f, 1.0f);

                    if (!this->m_show_unimportant)
                        continue;
                }
                else if (strstr(text.c_str(), u8"|SUCCESSFUL"))
                {
                    col = ImColor(0.2f * 2, 0.4f * 2, 0.1f * 2, 1.0f);

                    if (!this->m_show_good)
                        continue;
                }
                else if (strstr(text.c_str(), u8"|INFO"))
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
                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_log_all").c_str()))
                    {
                        this->m_show_errors = true;
                        this->m_show_warnings = true;
                        this->m_show_default = true;
                        this->m_show_special = true;
                        this->m_show_unimportant = true;
                        this->m_show_good = true;
                    }

                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_log_errors").c_str()))
                    {
                        this->m_show_errors = true;
                        this->m_show_warnings = false;
                        this->m_show_default = false;
                        this->m_show_special = false;
                        this->m_show_unimportant = false;
                        this->m_show_good = false;
                    }

                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_log_warnings").c_str()))
                    {
                        this->m_show_errors = false;
                        this->m_show_warnings = true;
                        this->m_show_default = false;
                        this->m_show_special = false;
                        this->m_show_unimportant = false;
                        this->m_show_good = false;
                    }

                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_log_unimportant").c_str()))
                    {
                        this->m_show_errors = false;
                        this->m_show_warnings = false;
                        this->m_show_default = false;
                        this->m_show_special = false;
                        this->m_show_unimportant = true;
                        this->m_show_good = false;
                    }

                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_log_successful").c_str()))
                    {
                        this->m_show_errors = false;
                        this->m_show_warnings = false;
                        this->m_show_default = false;
                        this->m_show_special = false;
                        this->m_show_unimportant = false;
                        this->m_show_good = true;
                    }

                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_log_default").c_str()))
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

                if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_log_clear").c_str()))
                {
                    this->m_string_buffer.clear();
                }

                if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_log_close").c_str()))
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
        if (ImGui::Begin(SDK_Names::getInstance().getName("st_overlay_overlay").c_str(), &this->m_is_visible, ImVec2(200, 500), 0.005f, this->m_flag))
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

                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_overlay_projectinfo").c_str()))
                {
                    ImGui::Text("Scene name: \n");
                    ImGui::Checkbox(SDK_Names::getInstance().getName("st_overlay_drawgrid").c_str(), &EditorPref::GetInstance().bDrawGrid);
                    if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_overlay_total").c_str()))
                    {
                        ImGui::Text("objects - ");
                        ImGui::Text("tris - ");
                        ImGui::Text("vertices - ");

                        ImGui::TreePop();
                    }

                    if (EditorPref::GetInstance().bDrawGrid)
                    {
                        if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_overlay_grid").c_str()))
                        {
                            if (ImGui::ColorEdit3(SDK_Names::getInstance().getName("st_overlay_grid_color").c_str(), GridOptions::col))
                            {
                                SDKUI::UI().setHoveredColorEdit(true);
                            }

                            if (ImGui::ColorEdit4(SDK_Names::getInstance().getName("st_overlay_background_color").c_str(), GridOptions::col_background))
                            {
                                SDKUI::UI().setHoveredColorEdit(true);
                            }


                            static xr_string a = "100|10";
                            ImGui::SliderInt(SDK_Names::getInstance().getName("st_overlay_grid_type").c_str(), &this->m_gridtype, 0, 5, a.c_str());
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

                    if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_overlay_camera").c_str()))
                    {
                        if (ImGui::Button(SDK_Names::getInstance().getName("st_overlay_sloc").c_str()))
                        {
                            this->m_savedposition = Device.vCameraPosition;
                            Device.vCameraPosition.set(0, 0, 0);
                        }

                        if (ImGui::Button(SDK_Names::getInstance().getName("st_overlay_prevpos").c_str()))
                        {
                            Device.vCameraPosition.set(this->m_savedposition);
                        }

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_overlay_gizmo").c_str()))
                    {
                        ImGui::DragInt(SDK_Names::getInstance().getName("st_overlay_gizmosize").c_str(), &g_size_gizmoline, 1.0f, 5, 20);
                        ImGui::DragFloat(SDK_Names::getInstance().getName("st_overlay_gizmosens").c_str(), &SDK_GizmoManager::GetInstance().m_speed, 0.001f, 0.001f, 1.0f);
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
                if (ImGui::BeginMenu(SDK_Names::getInstance().getName("st_overlay_popup_showonly").c_str()))
                {
                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_sdkinfo").c_str()))
                    {
                        this->m_showonly_sdkinfo = true;
                        this->m_showonly_projectinfo = false;
                        this->m_showonly_sysinfo = false;
                    }

                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_projectinfo").c_str()))
                    {
                        this->m_showonly_projectinfo = true;
                        this->m_showonly_sysinfo = false;
                        this->m_showonly_sdkinfo = false;
                    }

                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_systeminfo").c_str()))
                    {
                        this->m_showonly_sysinfo = true;
                        this->m_showonly_sdkinfo = false;
                        this->m_showonly_projectinfo = false;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu(SDK_Names::getInstance().getName("st_overlay_popup_show").c_str()))
                {
                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_all").c_str()))
                    {
                        this->m_showonly_sysinfo = true;
                        this->m_showonly_sdkinfo = true;
                        this->m_showonly_projectinfo = true;
                    }

                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_sdkinfo").c_str()))
                    {
                        this->m_showonly_sdkinfo = true;
                    }
                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_projectinfo").c_str()))
                    {
                        this->m_showonly_projectinfo = true;
                    }
                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_systeminfo").c_str()))
                    {
                        this->m_showonly_sysinfo = true;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu(SDK_Names::getInstance().getName("st_overlay_popup_hide").c_str()))
                {
                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_sdkinfo").c_str()))
                    {
                        this->m_showonly_sdkinfo = false;
                    }
                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_projectinfo").c_str()))
                    {
                        this->m_showonly_projectinfo = false;
                    }
                    if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_systeminfo").c_str()))
                    {
                        this->m_showonly_sysinfo = false;
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::MenuItem(SDK_Names::getInstance().getName("st_overlay_popup_close").c_str()))
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

        if (ImGui::Begin(SDK_Names::getInstance().getName("st_manager_camera").c_str(), &this->m_is_visible, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text(SDK_Names::getInstance().getName("st_text_position_camera_manager").c_str());
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_posX_camera_manager").c_str(), &Device.vCameraPosition.x, 1.0f);
            ImGui::SameLine();
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_posY_camera_manager").c_str(), &Device.vCameraPosition.y, 1.0f);
            ImGui::SameLine();
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_posZ_camera_manager").c_str(), &Device.vCameraPosition.z, 1.0f);
            ImGui::Separator();
            ImGui::Text(SDK_Names::getInstance().getName("st_text_direction_ro_camera_manager").c_str());
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_dirX_camera_manager").c_str(), &Device.vCameraDirection.x, 0.0f);
            ImGui::SameLine();
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_dirY_camera_manager").c_str(), &Device.vCameraDirection.y, 0.0f);
            ImGui::SameLine();
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_dirZ_camera_manager").c_str(), &Device.vCameraDirection.z, 0.0f);
            ImGui::Separator();
            ImGui::Text(SDK_Names::getInstance().getName("st_text_normal_ro_camera_manager").c_str());
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_norX_camera_manager").c_str(), &Device.vCameraTop.x, 0.0f);
            ImGui::SameLine();
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_norY_camera_manager").c_str(), &Device.vCameraTop.y, 0.0f);
            ImGui::SameLine();
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_norZ_camera_manager").c_str(), &Device.vCameraTop.z, 0.0f);
            ImGui::Separator();
            ImGui::Text(SDK_Names::getInstance().getName("st_text_sensivity_camera_manager").c_str());
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_sensvalue_camera_manager").c_str(), &SDK_Camera::GetInstance().fSens, 0.0001f, 0.0f, 1.0f);
            ImGui::Text(SDK_Names::getInstance().getName("st_text_camera_sysatt").c_str());
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_camera_FOV").c_str(), &SDK_Camera::GetInstance().f_fov, 1.0f, 1.0f, 90.0f);
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_camera_aspect").c_str(), &SDK_Camera::GetInstance().f_aspect, 0.1f, 0.1f, 1.0f);
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_speed_camera_manager").c_str(), &SDK_Camera::GetInstance().fMoveSpeed, 1.0f, 1.0f, 10000.0f);
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_camera_near_plane").c_str(), &SDK_Camera::GetInstance().fNear, 0.0f);
            ImGui::DragFloat(SDK_Names::getInstance().getName("st_camera_far_plane").c_str(), &SDK_Camera::GetInstance().fFar, 1.0f, 1.0f, 3000.0f);
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
			ImGui::InputText("Level prefix", this->m_level_prefix, IM_ARRAYSIZE(this->m_level_prefix));	

			if (ImGui::TreeNode("Build options"))
			{
				xr_string jitter_result = "";

				switch (this->m_jitter_samples)
				{
				case 0:
				{
					jitter_result.append("Low");
					break;
				}
				case 1:
				{
					jitter_result.append("Medium");
					break;
				}
				case 2: 
				{
					jitter_result.append("High");
					break;
				}
				}

				 
				ImGui::SliderInt("Hemisphere quality", &this->m_hemisphere_quality, 0, 3);
				ImGui::SliderInt("Sun shadow quality", &this->m_sun_shadow_quality, 0, 3);
				ImGui::DragFloat("Pixel per meter", &this->m_pixel_per_meter, 1.0f, 0.0f, 100.0f);
				ImGui::DragInt("Light map collapsing", &this->m_light_map_collapsing, 1.0f, 0, 100);
				ImGui::DragInt("Light map error", &this->m_light_map_zero, 1.0f, 0, 100);
				ImGui::SliderInt("Jitter samples", &this->m_jitter_samples, 0, 2, jitter_result.c_str());
				


				ImGui::TreePop();
			}
        }
		ImGui::End();
    }

#pragma endregion

    }
}
