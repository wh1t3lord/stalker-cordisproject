#ifndef dxFontRender_included
#define dxFontRender_included
#pragma once

#ifndef XRRENDER_SDK_EXPORTS
#include "Include/xrRender/FontRender.h"
#else
#include "Include/xrRenderSDK/FontRender.h"
#endif


class dxFontRender : public IFontRender
{
public:
    dxFontRender();
    virtual ~dxFontRender();

    virtual void Initialize(LPCSTR cShader, LPCSTR cTexture);
    virtual void OnRender(CGameFont& owner);

private:
    ref_shader pShader;
    ref_geom pGeom;
};

#endif //	FontRender_included
