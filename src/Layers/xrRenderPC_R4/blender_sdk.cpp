#include "stdafx.h"
#include "blender_sdk.h"
#include "Layers/xrRender/uber_deffer.h"

blender_sdk::~blender_sdk() {}

void blender_sdk::Compile(CBlender_Compile& C)
{
    IBlender::Compile(C);
    // , TRUE, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, 1, 1
    if (this->bLeaf)
    {
        C.r_Pass("detail_still", "detail_still", TRUE, TRUE, FALSE, TRUE, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, TRUE,
            255u);
    }
    else
    {
        if (this->bIsFake)
            C.r_Pass("detail_still", "detail_still", 1, 1, false, 1, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, 1, 0);
        else
            C.r_Pass("detail_still", "detail_still", false, 1, 1, 0, D3DBLEND_ONE, D3DBLEND_ZERO, 0, 0);
    }

    // C.r_ColorWriteEnable(true,true,true,true);
    C.r_dx10Texture("s_base", C.L_textures[0]);
    C.r_dx10Sampler("smp_linear");

    if (this->bLeaf)
    {
        if (this->bCullBack)
            C.r_CullMode(D3DCULL_CCW);
        else
            C.r_CullMode(D3DCULL_NONE);
    }
    else
    {
        if (this->bCullBack)
            C.r_CullMode(D3DCULL_CCW);
        else
            C.r_CullMode(D3DCULL_NONE);
    }

    C.r_End();
}
