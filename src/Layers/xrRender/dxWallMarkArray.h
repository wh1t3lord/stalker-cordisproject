#ifndef dxWallMarkArray_included
#define dxWallMarkArray_included
#pragma once

#ifndef XRRENDER_SDK_EXPORTS
#include "Include/xrRender/WallMarkArray.h"
#else
#include "Include/xrRenderSDK/WallMarkArray.h"
#endif // XRRENDER_SDK_EXPORTS

class dxWallMarkArray : public IWallMarkArray
{
public:
    virtual ~dxWallMarkArray();
    virtual void Copy(IWallMarkArray& _in);

    virtual void AppendMark(LPCSTR s_textures);
    virtual void clear();
    virtual bool empty();
    virtual wm_shader GenerateWallmark();

    ref_shader* dxGenerateWallmark();

private:
    using ShaderVec = xr_vector<ref_shader>;

    ShaderVec m_CollideMarks;
};

#endif //	WallMarkArray_included
