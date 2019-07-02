#include "stdafx.h"
#include "SDKUI_ObjectInspector.h"

namespace Cordis
{
	namespace SDK
	{
		SDKUI_ObjectInspector::~SDKUI_ObjectInspector(void)
		{

		}

		void SDKUI_ObjectInspector::Draw(void)
		{
			if (!this->m_is_visible && !SDK_SceneManager::GetInstance().getObjectsListSelected().size())
				return;

			if (ImGui::Begin(SDK_Names::getInstance().getName("st_object_inspector_name").c_str(), &this->m_is_visible, this->m_flag))
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
						if (ImGui::InputText(SDK_Names::getInstance().getName("st_object_inspector_sg_name").c_str(), buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							object->setSceneName(buffer);
							SDKUI_Log::Widget().AddText(object->getSceneName().c_str());
						}

						if (ImGui::TreeNode(SDK_Names::getInstance().getName("st_object_inspector_sg_transform").c_str()))
						{


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
	}
}