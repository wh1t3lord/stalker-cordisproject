#include "stdafx.h"
#include "SDKUI_ObjectInspector.h"

namespace Cordis
{
namespace SDK
{
SDKUI_ObjectInspector::~SDKUI_ObjectInspector(void) {}

void SDKUI_ObjectInspector::Draw(void)
{
    if (!this->m_is_visible || !SDK_SceneManager::GetInstance().m_objects_list.size())
        return;

    if (ImGui::Begin("Object Properties", &this->m_is_visible, this->m_flag))
    {
        // @ Lord -> Затем добавить вертикальный separator (таблицу)
        
    }
	ImGui::End();
}
} // namespace SDK
} // namespace Cordis
