#ifndef dxUIShader_included
#define dxUIShader_included
#pragma once

#ifndef XRRENDER_SDK_EXPORTS
#include "Include/xrRender/UIShader.h"
#else
#include "Include/xrRenderSDK/UIShader.h"
#endif


class dxUIShader : public IUIShader
{
    friend class dxUIRender;
    friend class dxDebugRender;
    friend class dxWallMarkArray;
    friend class CRender;

public:
    virtual ~dxUIShader() { ; }
    virtual void Copy(IUIShader& _in);
    virtual void create(LPCSTR sh, LPCSTR tex = nullptr);
    virtual bool inited() { return hShader; }
    virtual void destroy();

private:
    ref_shader hShader;
};

#endif //	dxUIShader_included
