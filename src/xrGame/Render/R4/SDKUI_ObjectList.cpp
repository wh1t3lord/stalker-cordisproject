#include "StdAfx.h"
#include "SDKUI_ObjectList.h"
#include "SDK_SceneManager.h"

namespace Cordis
{
namespace SDK
{
SDKUI_ObjectList::~SDKUI_ObjectList(void) {}
// @ Lord: подумать всё таки как можно было оптимизировать
void SDKUI_ObjectList::Draw(void)
{
    if (!this->m_is_visible)
        return;

    this->m_sections = {SDK_Names::getInstance().getName("st_log_all"),
        SDK_Names::getInstance().getName("st_name_staticgeometry"), SDK_Names::getInstance().getName("st_name_light"),
        SDK_Names::getInstance().getName("st_name_sound_sources"),
        SDK_Names::getInstance().getName("st_name_sound_environments"),
        SDK_Names::getInstance().getName("st_name_shapes"), SDK_Names::getInstance().getName("st_name_spawnelements"),
        SDK_Names::getInstance().getName("st_name_waypoints"), SDK_Names::getInstance().getName("st_name_sectors"),
        SDK_Names::getInstance().getName("st_name_portals"), SDK_Names::getInstance().getName("st_name_groups"),
        SDK_Names::getInstance().getName("st_name_staticparticles"),
        SDK_Names::getInstance().getName("st_name_detail_objects"), SDK_Names::getInstance().getName("st_name_aimap"),
        SDK_Names::getInstance().getName("st_name_wallmarks"), SDK_Names::getInstance().getName("st_name_fogvolumes")};

    xr_string currentsection_name = this->m_sections[this->m_index_currentsection];
    if (ImGui::Begin(
            SDK_Names::getInstance().getName("st_label_object_list").c_str(), &this->m_is_visible, this->m_flag))
    {
        ImGui::Text(SDK_Names::getInstance().getName("st_text_section").c_str());

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

        // @ Static Geometry
        if (this->m_index_currentsection == 1 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_staticgeometry").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_StaticGeometry)
                    {
                        // @ Lord: доработать все Selectable в данном файле
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }

                ImGui::TreePop();
            }
        }

        // @ Lights
        if (this->m_index_currentsection == 2 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_light").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_Lights)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }

                ImGui::TreePop();
            }
        }

        // @ Sound Sources
        if (this->m_index_currentsection == 3 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_sound_sources").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_SoundSource)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }

                ImGui::TreePop();
            }
        }

        // @ Sound Environment
        if (this->m_index_currentsection == 4 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_sound_environments").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_SoundEnvironment)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }
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

        // @ Shapes
        if (this->m_index_currentsection == 5 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_shapes").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_Shapes)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }

                ImGui::TreePop();
            }
        }

        // @ Spawn Elements
        if (this->m_index_currentsection == 6 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_spawnelements").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_SpawnElements)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }
                ImGui::TreePop();
            }
        }

        // @ Waypoints
        if (this->m_index_currentsection == 7 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_waypoints").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_WayPoints)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }
                ImGui::TreePop();
            }
        }

        // @ Sectors
        if (this->m_index_currentsection == 8 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_sectors").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_Sectors)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }
                ImGui::TreePop();
            }
        }

        // @ Portals
        if (this->m_index_currentsection == 9 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_portals").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_Portals)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }
                ImGui::TreePop();
            }
        }

        // @ Groups
        if (this->m_index_currentsection == 10 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_groups").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_Groups)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }
                ImGui::TreePop();
            }
        }

        // @ Static Particles
        if (this->m_index_currentsection == 11 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_staticparticles").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_StaticParticles)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }
                ImGui::TreePop();
            }
        }

        // @ Detail Objects
        if (this->m_index_currentsection == 12 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_detail_objects").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_DetailObjects)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }

                ImGui::TreePop();
            }
        }

        // @ AI-Map
        if (this->m_index_currentsection == 13 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_aimap").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_AIMap)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }

                ImGui::TreePop();
            }
        }

        // @ Wallmarks
        if (this->m_index_currentsection == 14 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_wallmarks").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_Wallmarks)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }

                ImGui::TreePop();
            }
        }

        // @ Fog Volumes
        if (this->m_index_currentsection == 15 || this->m_index_currentsection == 0)
        {
            if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_name_fogvolumes").c_str()))
            {
                for (SDK_CustomObject* object : SDK_SceneManager::GetInstance().getObjectsList())
                {
                    if (object->getType() == kSection_FogVolumes)
                    {
                        if (ImGui::Selectable(object->getSceneName().c_str(), false))
                        {
                        }
                    }
                }

                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
}
} // namespace SDK
} // namespace Cordis
