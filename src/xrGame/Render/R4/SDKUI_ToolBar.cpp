#include "StdAfx.h"
#include "SDKUI_ToolBar.h"
#include "SDK_IconManager.h"


namespace Cordis
{
    namespace SDK
    {
    /* @Lord дополнять!
        0 - MoveTool
        1 - RotateTool
        2 - ScaleTool
        3 - Brush Tool
        4 - ...
    */

    SDKUI_ToolBar::~SDKUI_ToolBar(void) {}

    void SDKUI_ToolBar::Draw(void)
    {
        unsigned int count_tools_icons = SDK_IconManager::GetInstance().GetCountToolsIcons();
        if (!this->m_is_visible || !count_tools_icons)
            return;

        if (ImGui::Begin("Tool Bar", &this->m_is_visible, this->m_flag))
        {
            for (unsigned int i = 0; i < count_tools_icons; ++i)
            {
                ImGui::Image(SDK_IconManager::GetInstance().GetImageFromToolsIcons(i), ImVec2(32, 32));

                if (i < count_tools_icons - 1)
                {
                    ImGui::Separator();
                }
            }
        }
		ImGui::End();
    }

    }
}
