#include "StdAfx.h"
#include "SDKUI_ObjectInspector.h"

namespace Cordis
{
namespace SDK
{
SDKUI_ObjectInspector::~SDKUI_ObjectInspector(void) {}

void SDKUI_ObjectInspector::Draw(void)
{
    if (!this->m_is_visible && !SDK_SceneManager::GetInstance().getObjectsListSelected().size())
        return;

    if (ImGui::Begin(
            SDK_Names::getInstance().getName("st_object_inspector_name").c_str(), &this->m_is_visible, this->m_flag))
    {
        switch (SDKUI_RightWindow::Widget().GetSection())
        {
        case kSection_StaticGeometry:
        {
            const xr_list<SDK_CustomObject*>& selectedobjects = SDK_SceneManager::GetInstance().getObjectsListSelected();
            if (selectedobjects.size() == 1)
            {
                SDK_CustomObject* object = selectedobjects.back();
                static char buffer[128];
                strcpy_s(buffer, object->getSceneName().c_str());
                if (ImGui::InputText(SDK_Names::getInstance().getName("st_object_inspector_sg_name").c_str(), buffer,
                        IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    object->setSceneName(buffer);
                    SDKUI_Log::Widget().AddText(object->getSceneName().c_str());
                }

                if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_object_inspector_sg_transform").c_str()))
                {
                    Fvector& object_position = object->getPosition();
                    Fvector& object_rotation = object->getRotation();
                    Fvector& object_scale = object->getScale();
                    if (ImGui::TreeNode(
                            SDK_Names::getInstance().getName("st_object_inspector_sg_transform_position").c_str()))
                    {
                        if (ImGui::DragFloat(
                                SDK_Names::getInstance().getName("st_object_inspector_sg_transform_position_x").c_str(),
                                &object_position.x, 0.001f))
                        {
                            object->UpdateTransform();
                        }

                        if (ImGui::DragFloat(
                                SDK_Names::getInstance().getName("st_object_inspector_sg_transform_position_y").c_str(),
                                &object_position.y, 0.001f))
                        {
                            object->UpdateTransform();
                        }

                        if (ImGui::DragFloat(
                                SDK_Names::getInstance().getName("st_object_inspector_sg_transform_position_z").c_str(),
                                &object_position.z, 0.001f))
                        {
                            object->UpdateTransform();
                        }

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode(
                            SDK_Names::getInstance().getName("st_object_inspector_sg_transform_direction").c_str()))
                    {
                        if (ImGui::DragFloat(SDK_Names::getInstance()
                                                 .getName("st_object_inspector_sg_transform_direction_x")
                                                 .c_str(),
                                &object_rotation.x, 0.001f))
                        {
                            object->UpdateTransform();
                        }

                        if (ImGui::DragFloat(SDK_Names::getInstance()
                                                 .getName("st_object_inspector_sg_transform_direction_y")
                                                 .c_str(),
                                &object_rotation.y, 0.001f))
                        {
                            object->UpdateTransform();
                        }

                        if (ImGui::DragFloat(SDK_Names::getInstance()
                                                 .getName("st_object_inspector_sg_transform_direction_z")
                                                 .c_str(),
                                &object_rotation.z, 0.001f))
                        {
                            object->UpdateTransform();
                        }

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode(
                            SDK_Names::getInstance().getName("st_object_inspector_sg_transform_scale").c_str()))
                    {
                        if (ImGui::DragFloat(
                                SDK_Names::getInstance().getName("st_object_inspector_sg_transform_scale_x").c_str(),
                                &object_scale.x, 0.001f))
                        {
                            object->UpdateTransform();
                        }

                        if (ImGui::DragFloat(
                                SDK_Names::getInstance().getName("st_object_inspector_sg_transform_scale_y").c_str(),
                                &object_scale.y, 0.001f))
                        {
                            object->UpdateTransform();
                        }

                        if (ImGui::DragFloat(
                                SDK_Names::getInstance().getName("st_object_inspector_sg_transform_scale_z").c_str(),
                                &object_scale.z, 0.001f))
                        {
                            object->UpdateTransform();
                        }
                        ImGui::TreePop();
                    }

                    ImGui::TreePop();
                }
            }
            else
            {
            }
            break;
        }
        }
    }
    ImGui::End();
}
} // namespace SDK
} // namespace Cordis
