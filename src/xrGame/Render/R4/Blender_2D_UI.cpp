#include "StdAfx.h"
#include "Blender_2D_UI.h"

 

Blender_2D_UI::~Blender_2D_UI(void) 
{

}

void Blender_2D_UI::Compile(CBlender_Compile& C)
{
    IBlender::Compile(C);

    C.r_Pass("editor", "simple_color", false, 1, FALSE, TRUE, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);
    C.r_CullMode(D3DCULL_NONE);
    C.r_End();
}
