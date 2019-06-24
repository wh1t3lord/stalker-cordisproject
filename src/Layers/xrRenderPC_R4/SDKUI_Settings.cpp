#include "stdafx.h"
#include "SDKUI_Settings.h"


namespace Cordis
{
	namespace SDK
	{
		SDKUI_Settings::~SDKUI_Settings(void)
		{
		}

		void SDKUI_Settings::Draw(void)
		{
			if (!this->m_is_visible)
				return;

			if (ImGui::Begin(SDK_Names::getInstance().getName("st_label_settings").c_str(), &this->m_is_visible, this->m_flag))
			{
				
			}
			ImGui::End();
		}
	}
}


