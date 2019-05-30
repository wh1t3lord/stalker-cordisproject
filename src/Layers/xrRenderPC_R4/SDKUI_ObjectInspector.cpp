#include "stdafx.h"
#include "SDKUI_ObjectInspector.h"

 
SDKUI_ObjectInspector::~SDKUI_ObjectInspector(void)
{

}


void SDKUI_ObjectInspector::Draw(void)
{
    if (!this->m_is_visible)
        return;
    
    if (ImGui::Begin("Object Properties", &this->m_is_visible, this->m_flag))
    {   
        ImGui::Columns(2);
        



        ImGui::NextColumn();
 





        ImGui::End();
    }

}
