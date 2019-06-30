#include "stdafx.h"
#include "SDKUI_ObjectInspector.h"
#include "SDKUI_RightWindow.h"

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
		switch (SDKUI_RightWindow::Widget().GetSection())
		{
		case kSection_SpawnElements:
		{
			break;
		}
		case kSection_WayPoints:
		{
			break;
		}
		case kSection_StaticGeometry:
		{
			
			break;
		}
		case kSection_Lights:
		{
			break;
		}
		case kSection_Sectors:
		{
			break;
		}
		case kSection_Portals:
		{
			break;
		}
		case kSection_SoundEnvironment:
		{
			break;
		}
		case kSection_SoundSource:
		{
			break;
		}
		case kSection_AIMap:
		{
			break;
		}
		case kSection_Groups:
		{
			break;
		}
		case kSection_StaticParticles:
		{
			break;
		}
		case kSection_FogVolumes:
		{
			break;
		}
		case kSection_Wallmarks:
		{
			break;
		}
		case kSection_DetailObjects:
		{
			break;
		}
		}
    }
	ImGui::End();
}
} // namespace SDK
} // namespace Cordis
