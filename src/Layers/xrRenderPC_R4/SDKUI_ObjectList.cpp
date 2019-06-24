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

        if (ImGui::Begin("Object List", &this->m_is_visible, this->m_flag))
        {
            ImGui::Text("Section: ");
            static xr_string currentsection_name = this->m_sections[0];
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
                if (ImGui::TreeNode(SECTION_LIGHTS_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 3 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_SOUNDSOURCE_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 4 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_SOUNDENVIRONMENT_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 5 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_GLOWS_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 6 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_SHAPES_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 7 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_SPAWNELEMENTS_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 8 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_WAYPOINTS_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 9 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_SECTORS_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 10 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_PORTALS_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 11 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_GROUPS_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 12 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_STATICPARTICLES_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 13 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_DETAILOBJECTS_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 14 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_AIMAP_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 15 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_WALLMARKS_NAME))
                {
                    ImGui::TreePop();
                }
            }

            if (this->m_index_currentsection == 16 || this->m_index_currentsection == 0)
            {
                if (ImGui::TreeNode(SECTION_FOGVOLUMES_NAME))
                {
                    ImGui::TreePop();
                }
            }
        }
		ImGui::End();
    }
    }
}

