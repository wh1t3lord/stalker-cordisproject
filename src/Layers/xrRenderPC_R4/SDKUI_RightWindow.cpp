#include "stdafx.h"
#include "SDKUI_RightWindow.h"
#include "SDKUI.h"
#include "SDK_SceneManager.h"
#include "SDK_IconManager.h"
#include "SDKUI_ObjectList.h"

SDKUI_RightWindow::~SDKUI_RightWindow(void) {}

void SDKUI_RightWindow::Draw(void)
{
    if (!this->m_is_visible)
        return;


        ImGui::SetNextWindowPos(ImVec2(SDKUI::UI().GetDisplayX() - 250, this->m_size_mainmenubar_y), ImGuiCond_Once);
        if (ImGui::Begin("Manager list", &this->m_is_visible, ImVec2(250, SDKUI::UI().GetDisplayY() - this->m_size_mainmenubar_y),
                -1.0f, this->m_flag))
        {
#pragma region SECTIONS
            ImGui::Text("Section: ");
            static xr_string current_section_name = this->m_sections[0];
            if (ImGui::BeginCombo("", current_section_name.c_str()))
            {
                for (uint32_t i = 0; i < this->m_sections.size(); ++i)
                {
                    bool is_selected = (current_section_name == this->m_sections[i]);
                    if (ImGui::Selectable(this->m_sections[i].c_str(), is_selected))
                    {
                        current_section_name = this->m_sections[i];
                        this->m_id_currentsection = i;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
#pragma endregion
            if (ImGui::Button("Show Object List"))
            {
                if (!SDKUI_ObjectList::Widget().IsVisible())
                {
                    SDKUI_ObjectList::Widget().Show();
                }
                else
                {
                    SDKUI_ObjectList::Widget().Hide();
                }
            }
            ImGui::Separator();
            if (this->m_id_currentsection == 0)
            {
                if (ImGui::CollapsingHeader("Catalog Static Geometry"))
                {
                    for (xr_map<xr_string, CEditableObject*>::const_iterator it =
                             SDK_Cache::GetInstance().m_staticgeometry.cbegin();
                         it != SDK_Cache::GetInstance().m_staticgeometry.cend(); ++it)
                    {
                        bool is_selected = (this->m_currentselected_staticobject_name == it->first);
                        //    ImGui::Image(SDK_IconManager::GetInstance().GetImageFromToolsIcons(0), ImVec2(164, 164));
                        if (ImGui::Selectable(it->first.c_str(), is_selected))
                        {
                            this->m_currentselected_staticobject_name = it->first;
                        }
                    }
                }
            }
            ImGui::Separator();

            if (this->m_id_currentsection == 0)
            {
                ImGui::Text(
                    "Total Static Geometry: %d", SDK_SceneManager::GetInstance().m_total_count_objects_static_geometry);
                ImGui::Text("Selected Object: %s", this->m_currentselected_staticobject_name.c_str());
            }
            else if (this->m_id_currentsection == 1)
            {
                ImGui::Text("Total Lights: %d", SDK_SceneManager::GetInstance().m_total_count_objects_lights);
            }
            else if (this->m_id_currentsection == 2)
            {
                ImGui::Text(
                    "Total Sound Sources: %d", SDK_SceneManager::GetInstance().m_total_count_objects_sound_sources);
            }
            else if (this->m_id_currentsection == 3)
            {
                ImGui::Text("Total Sound Environments: %d",
                    SDK_SceneManager::GetInstance().m_total_count_objects_sound_environments);
            }
            else if (this->m_id_currentsection == 4)
            {
                ImGui::Text("Total Glows: %d", SDK_SceneManager::GetInstance().m_total_count_objects_glows);
            }
            else if (this->m_id_currentsection == 5)
            {
                ImGui::Text("Total Shapes: %d",
                    SDK_SceneManager::GetInstance()
                        .m_total_count_objects_shapes); // Lord: улучшить это посчитать как сумму shapes и shapes, которые
                                                      // используется в spawn elemnets
            }
            else if (this->m_id_currentsection == 6)
            {
                ImGui::Text(
                    "Total Spawn Elements: %d", SDK_SceneManager::GetInstance().m_total_count_objects_spawn_elements);
            }
            else if (this->m_id_currentsection == 7)
            {
                ImGui::Text("Total Way Points: %d", SDK_SceneManager::GetInstance().m_total_count_objects_waypoints);
            }
            else if (this->m_id_currentsection == 8)
            {
                //      ImGui::Text("Total Sectors: %d"); Lord: think about it
            }
            else if (this->m_id_currentsection == 9)
            {
                //     ImGui::Text("Total Portals"); Lord: think about it
            }
            else if (this->m_id_currentsection == 10)
            {
                ImGui::Text("Total Groups: %d", SDK_SceneManager::GetInstance().m_total_count_objects_groups);
            }
            else if (this->m_id_currentsection == 11)
            {
                ImGui::Text(
                    "Total Static Particles: %d", SDK_SceneManager::GetInstance().m_total_count_objects_static_particles);
            }
            else if (this->m_id_currentsection == 12)
            {
                //    ImGui::Text("Total Detail Objects: %d", SDK_SceneManager::GetInstance().m_total_count_objects);
                //    Lord: think about it
            }
            else if (this->m_id_currentsection == 13)
            {
                ImGui::Text("Total AI-nodes: %d", SDK_SceneManager::GetInstance().m_total_count_objects_ai_nodes);
            }
            else if (this->m_id_currentsection == 14)
            {
                ImGui::Text("Total Wallmarks: %d", SDK_SceneManager::GetInstance().m_total_count_objects_wallmarks);
            }
            else if (this->m_id_currentsection == 15)
            {
                // ImGui::Text("Total Fog Volumes"); Lord: think about it
            }
        }
        ImGui::End();
    
}
