#include "stdafx.h"
#include "SDKUI_ObjectList.h"

 

SDKUI_ObjectList::~SDKUI_ObjectList(void) 
{

}


void SDKUI_ObjectList::Draw(void)
{
    if (!this->bShow)
        return;

    if (ImGui::Begin("Object List", &this->bShow, this->flag))
    {





        ImGui::End();
    }
}
