#ifndef dxThunderboltRender_included
#define dxThunderboltRender_included
#pragma once

#ifndef XRRENDER_SDK_EXPORTS
#include "Include/xrRender/ThunderboltRender.h"
#else
#include "Include/xrRenderSDK/ThunderboltRender.h"
#endif


class dxThunderboltRender : public IThunderboltRender
{
public:
    dxThunderboltRender();
    virtual ~dxThunderboltRender();

    virtual void Copy(IThunderboltRender& _in);

    virtual void Render(CEffect_Thunderbolt& owner);

private:
    ref_geom hGeom_model;
    ref_geom hGeom_gradient;
};

#endif //	ThunderboltRender_included
