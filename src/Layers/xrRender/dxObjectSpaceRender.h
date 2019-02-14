#ifndef dxObjectSpaceRender_included
#define dxObjectSpaceRender_included
#pragma once

#ifdef DEBUG


#ifndef XRRENDER_SDK_EXPORTS
#include "Include/xrRender/ObjectSpaceRender.h"
#include "xrEngine/xr_collide_form.h"
#else
#include "Include/xrRenderSDK/ObjectSpaceRender.h"
#include "xrEngine_SDK/xr_collide_form.h"
#endif

class dxObjectSpaceRender : public IObjectSpaceRender
{
public:
    dxObjectSpaceRender();
    virtual ~dxObjectSpaceRender();
    virtual void Copy(IObjectSpaceRender& _in);

    virtual void dbgRender();
    virtual void dbgAddSphere(const Fsphere& sphere, u32 colour);
    virtual void SetShader();

private:
    ref_shader m_shDebug;
    clQueryCollision q_debug; // MT: dangerous
    xr_vector<std::pair<Fsphere, u32>> dbg_S; // MT: dangerous
};

#endif // DEBUG

#endif //	ObjectSpaceRender_included
