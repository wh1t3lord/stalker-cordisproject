#ifndef dxRainRender_included
#define dxRainRender_included
#pragma once

#ifndef XRRENDER_SDK_EXPORTS
#include "Include/xrRender/RainRender.h"
#else
#include "Include/xrRenderSDK/RainRender.h"
#endif


class dxRainRender : public IRainRender
{
public:
    dxRainRender();
    virtual ~dxRainRender();
    virtual void Copy(IRainRender& _in);

    virtual void Render(CEffect_Rain& owner);

    virtual const Fsphere& GetDropBounds() const;

private:
    // Visualization	(rain)
    ref_shader SH_Rain;
    ref_geom hGeom_Rain;

    // Visualization	(drops)
    IRender_DetailModel* DM_Drop;
    ref_geom hGeom_Drops;
};

#endif //	RainRender_included
