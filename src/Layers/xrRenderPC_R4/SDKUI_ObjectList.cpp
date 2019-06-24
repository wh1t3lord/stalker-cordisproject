#include "stdafx.h"
#include "SDKUI_ObjectList.h"

namespace Cordis
{
    namespace SDK
    {
    SDKUI_ObjectList::~SDKUI_ObjectList(void) {}

    void SDKUI_ObjectList::Draw(void)
    {
        if (!this->m_is_visible)
            return;

		this->m_sections = { SDK_Names::getInstance().getName("st_log_all"), SDK_Names::getInstance().getName("st_name_staticgeometry"), SDK_Names::getInstance().getName("st_name_light"), SDK_Names::getInstance().getName("st_name_sound_sources"),
				  SDK_Names::getInstance().getName("st_name_sound_environments"), SDK_Names::getInstance().getName("st_name_shapes"), SDK_Names::getInstance().getName("st_name_spawnelements"),
				  SDK_Names::getInstance().getName("st_name_waypoints"), SDK_Names::getInstance().getName("st_name_sectors"), SDK_Names::getInstance().getName("st_name_portals"), SDK_Names::getInstance().getName("st_name_groups"),
				  SDK_Names::getInstance().getName("st_name_staticparticles"), SDK_Names::getInstance().getName("st_name_detail_objects"), SDK_Names::getInstance().getName("st_name_aimap"), SDK_Names::getInstance().getName("st_name_wallmarks"),
				  SDK_Names::getInstance().getName("st_name_fogvolumes") };
		 
		xr_string currentsection_name = this->m_sections[this->m_index_currentsection];
        if (ImGui::Begin(SDK_Names::getInstance().getName("st_label_object_list").c_str(), &this->m_is_visible, this->m_flag))
        {
            ImGui::Text(SDK_Names::getInstance().getName("st_section").c_str());
            
            if (ImGui::BeginCombo("", currentsection_name.c_str()))
            {
                for (uint32_t i = 0; i < this->m_sections.size(); ++i)
                {
                    bool is_selected = (currentsection_name == this->m_sections[i]);
                    if (ImGui::Selectable(this->m_sections[i].c_str(), is_selected))
                    {
                        currentsection_name = this->m_sections[i];
                        this->m_index_currentsection = i;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
            ImGui::Separator();

            if (this->m_index_currentsection == 1 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_staticgeometry").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 2 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_light").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 3 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_sound_sources").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 4 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_sound_environments").c_str()))
                {
                    ImGui::TreePop();
                }
            }
			// Lord: удалить это потому что Glow не используется рендером игры толком!
//             if (this->m_index_currentsection == 5 || this->m_index_currentsection == 0)
//             {
//                 if (ImGui::TreeNode("Glows"))
//                 {
//                     ImGui::TreePop();
//                 }
//             }

            if (this->m_index_currentsection == 5 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_shapes").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 6 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_spawnelements").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 7 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_waypoints").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 8 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_sectors").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 9 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_portals").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 10 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_groups").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 11 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_staticparticles").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 12 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_detail_objects").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 13 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_aimap").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 14 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_wallmarks").c_str()))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 15 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_fogvolumes").c_str()))
                {
                    ImGui::TreePop();
                }
            }
        }
		ImGui::End();
    }
    }
}

