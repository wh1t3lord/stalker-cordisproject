#ifndef dxConsoleRender_included
#define dxConsoleRender_included
#pragma once

#ifndef XRRENDER_SDK_EXPORTS
#include "Include/xrRender/ConsoleRender.h"
#else
#include "Include/xrRenderSDK/ConsoleRender.h"
#endif

class dxConsoleRender : public IConsoleRender
{
public:
    dxConsoleRender();

    virtual void Copy(IConsoleRender& _in);
    virtual void OnRender(bool bGame);

private:
#ifndef USE_DX9
    ref_shader m_Shader;
    ref_geom m_Geom;
#endif
};

#endif //	ConsoleRender_included
