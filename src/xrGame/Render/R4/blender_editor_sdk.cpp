#include "StdAfx.h"
#include "blender_editor_sdk.h"
blender_editor_sdk::~blender_editor_sdk(void) 
{
}

void blender_editor_sdk::Compile(CBlender_Compile& C)
{
    IBlender::Compile(C);

    if (this->bSelection)
     C.r_Pass("editor_sdk", "editor_sdk", true, TRUE, FALSE, TRUE, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);
    else
     C.r_Pass("editor_sdk", "editor_sdk", false, TRUE, TRUE);

    C.r_End();
}
