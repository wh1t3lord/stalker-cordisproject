#ifndef dxThunderboltDescRender_included
#define dxThunderboltDescRender_included
#pragma once

#ifndef XRRENDER_SDK_EXPORTS
#include "Include/xrRender/ThunderboltDescRender.h"
#else
#include "Include/xrRenderSDK/ThunderboltDescRender.h"
#endif


class IRender_DetailModel;

class dxThunderboltDescRender : public IThunderboltDescRender
{
public:
    virtual void Copy(IThunderboltDescRender& _in);

    virtual void CreateModel(LPCSTR m_name);
    virtual void DestroyModel();
    // private:
public:
    IRender_DetailModel* l_model;
};

#endif //	dxThunderboltDescRender_included
