#include "stdafx.h"
#include "SDKUI_RightWindow.h"
#include "SDKUI.h"
#include "SDK_SceneManager.h"

SDKUI_RightWindow::~SDKUI_RightWindow(void) 
{ 

}

void SDKUI_RightWindow::Draw(void)
{
    if (this->bShow)
    {
        ImGui::SetNextWindowPos(ImVec2(SDKUI::UI().GetDisplayX() - 250, this->MainMenuBarY), ImGuiCond_Once);
        if (ImGui::Begin("Manager list", &this->bShow, ImVec2(250, SDKUI::UI().GetDisplayY() - this->MainMenuBarY),
                -1.0f, this->flag))
        {
            ImGui::Text("Section: ");
            static xr_string current_section = this->sections[0];
            if (ImGui::BeginCombo("", current_section.c_str()))
            {
                for (uint32_t i = 0; i < this->sections.size(); ++i)
                {
                    bool is_selected = (current_section == this->sections[i]);
                    if (ImGui::Selectable(this->sections[i].c_str(), is_selected))
                    {
                        current_section = this->sections[i];
                        this->Current_Section = i;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
            ImGui::Separator();
            if (this->Current_Section == 0)
            {
                if (ImGui::CollapsingHeader("Catalog Static Geometry"))
                {
                    for (xr_map<xr_string, CEditableObject*>::const_iterator it = SDK_Cache::GetInstance().LibStaticGeometry.cbegin();
                        it != SDK_Cache::GetInstance().LibStaticGeometry.cend(); ++it)
                    {
                        bool is_selected = (this->CurrentSelectedStaticObject == it->first);
                        
                        if (ImGui::Selectable(it->first.c_str(), is_selected))
                        {
                            this->CurrentSelectedStaticObject = it->first;
                        }
                    }
                }
            }
            ImGui::Separator();

            if (this->Current_Section == 0)
            {
                ImGui::Text("Total Static Geometry: %d", SDK_SceneManager::GetInstance().total_count_objects_static_geometry);
                ImGui::Text("Selected Object: %s", this->CurrentSelectedStaticObject.c_str());
            }
            else if (this->Current_Section == 1)
            {
                ImGui::Text("Total Lights: %d", SDK_SceneManager::GetInstance().total_count_objects_lights);
            }
            else if (this->Current_Section == 2)
            {
                ImGui::Text("Total Sound Sources: %d", SDK_SceneManager::GetInstance().total_count_objects_sound_sources);
            }
            else if (this->Current_Section == 3)
            {
                ImGui::Text("Total Sound Environments: %d", SDK_SceneManager::GetInstance().total_count_objects_sound_environments);
            }
            else if (this->Current_Section == 4)
            {
                ImGui::Text("Total Glows: %d", SDK_SceneManager::GetInstance().total_count_objects_glows);
            }
            else if (this->Current_Section == 5)
            {
                ImGui::Text("Total Shapes: %d", SDK_SceneManager::GetInstance().total_count_objects_shapes); // Lord: улучшить это посчитать как сумму shapes и shapes, которые используется в spawn elemnets
            }
            else if (this->Current_Section == 6)
            {
                ImGui::Text("Total Spawn Elements: %d", SDK_SceneManager::GetInstance().total_count_objects_spawn_elements);
            }
            else if (this->Current_Section == 7)
            {
                ImGui::Text("Total Way Points: %d", SDK_SceneManager::GetInstance().total_count_objects_waypoints);
            }
            else if (this->Current_Section == 8)
            {
          //      ImGui::Text("Total Sectors: %d"); Lord: think about it
            }
            else if (this->Current_Section == 9)
            {
           //     ImGui::Text("Total Portals"); Lord: think about it
            }
            else if (this->Current_Section == 10)
            {
                ImGui::Text("Total Groups: %d", SDK_SceneManager::GetInstance().total_count_objects_groups);
            }
            else if (this->Current_Section == 11)
            {
                ImGui::Text("Total Static Particles: %d", SDK_SceneManager::GetInstance().total_count_objects_static_particles);
            }
            else if (this->Current_Section == 12)
            {
            //    ImGui::Text("Total Detail Objects: %d", SDK_SceneManager::GetInstance().total_count_objects); Lord: think about it
            }
            else if (this->Current_Section == 13)
            {
                ImGui::Text("Total AI-nodes: %d", SDK_SceneManager::GetInstance().total_count_objects_ai_nodes);
            }
            else if (this->Current_Section == 14)
            {
                ImGui::Text("Total Wallmarks: %d", SDK_SceneManager::GetInstance().total_count_objects_wallmarks);
            }
            else if (this->Current_Section == 15)
            {
                //ImGui::Text("Total Fog Volumes"); Lord: think about it
            }
        }
        ImGui::End();
    }
}
