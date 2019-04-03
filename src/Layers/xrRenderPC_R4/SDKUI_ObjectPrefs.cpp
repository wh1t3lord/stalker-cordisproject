#include "stdafx.h"
#include "SDKUI_ObjectPrefs.h"
#include "SDK_ObjectStaticGeometry.h"

void SDKUI_StaticGeometryPref::Draw(SDK_ObjectStaticGeometry* data)
{
    if (this->bShow)
    {
        if (data)
        {
            if (ImGui::Begin("Object Preferences", &this->bShow))
            {
                
            }

            ImGui::End();
        }
    }
}
