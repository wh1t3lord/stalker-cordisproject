// file: D3DUtils.cpp

#include "stdafx.h"
#pragma hdrstop
#ifndef XRRENDER_SDK_EXPORTS
#include "xrEngine/GameFont.h"
#else
#include "xrEngine_SDK/GameFont.h"
#endif
#include "D3DUtils.h"
#include "du_box.h"
#include "du_sphere.h"
#include "du_sphere_part.h"
#include "du_cone.h"
#include "du_cylinder.h"
#include "xrCore/_obb.h"
#include "xrCore/vector.h"

#if defined(WINDOWS)
#pragma warning(push)
#pragma warning(disable : 4995)
#include "d3dx9.h"
#pragma warning(pop)
#endif
#include "SDKUI.h"
CDrawUtilities DUImpl;
#include "SDK_GizmoScale.h"

#define LINE_DIVISION 32 // не меньше 6!!!!!
// for drawing sphere
static Fvector circledef1[LINE_DIVISION];
static Fvector circledef2[LINE_DIVISION];
static Fvector circledef3[LINE_DIVISION];
int GridOptions::Size;
int GridOptions::separator;
float GridOptions::col[3] = {0.1f, 0.1f, 0.1f};
float GridOptions::col_background[4] = {0.2f, 0.2f, 0.2f, 1.0f};
const u32 boxcolor = D3DCOLOR_RGBA(255, 255, 255, 0);
static const int boxvertcount = 48;
static Fvector boxvert[boxvertcount];
int g_size_gizmoline = 15;
float some_factor_test = 0.0f;
#ifdef _EDITOR
#define DU_DRAW_RS dxRenderDeviceRender::Instance().SetRS
#define DU_DRAW_SH_C(a, c)                                              \
    {                                                                   \
        dxRenderDeviceRender::Instance().SetShader(a);                  \
        dxRenderDeviceRender::Instance().SetRS(D3DRS_TEXTUREFACTOR, c); \
    }
#define DU_DRAW_SH(a)                                                            \
    {                                                                            \
        dxRenderDeviceRender::Instance().SetShader(a);                           \
        dxRenderDeviceRender::Instance().SetRS(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF); \
    }
#else
#define DU_DRAW_RS RCache.dbg_SetRS
#define DU_DRAW_SH_C(sh, c)                                                                   \
    {                                                                                         \
        RCache.set_Shader(sh);                                                                \
        RCache.set_c("tfactor", float(color_get_R(c)) / 255.f, float(color_get_G(c)) / 255.f, \
            float(color_get_B(c)) / 255.f, float(color_get_A(c)) / 255.f);                    \
    }
#define DU_DRAW_SH(sh)                       \
    {                                        \
        RCache.set_Shader(sh);               \
        RCache.set_c("tfactor", 1, 1, 1, 1); \
    }
#endif

#ifdef _EDITOR
#define FILL_MODE dxRenderDeviceRender::Instance().dwFillMode
#define SHADE_MODE dxRenderDeviceRender::Instance().dwShadeMode
#define SCREEN_QUALITY dxRenderDeviceRender::Instance().m_ScreenQuality
#else
#define FILL_MODE D3DFILL_SOLID
#define SHADE_MODE D3DSHADE_GOURAUD
#define SCREEN_QUALITY 1.f
#endif

// identity box
const u32 identboxcolor = D3DCOLOR_RGBA(255, 255, 255, 0);

static const int identboxwirecount = 24;
static Fvector identboxwire[identboxwirecount] = {{-0.5f, -0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, -0.5f},
    {+0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
    {-0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f}, {+0.5f, -0.5f, +0.5f},
    {-0.5f, -0.5f, +0.5f}, {-0.5f, -0.5f, +0.5f}, {-0.5f, +0.5f, +0.5f}, {-0.5f, +0.5f, -0.5f}, {-0.5f, +0.5f, +0.5f},
    {+0.5f, +0.5f, -0.5f}, {+0.5f, +0.5f, +0.5f}, {+0.5f, -0.5f, -0.5f}, {+0.5f, -0.5f, +0.5f}, {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, +0.5f}};

/*
static const int identboxindexcount = 36;
static const WORD identboxindices[identboxindexcount] = {
    0, 1, 2,   2, 3, 0,
    3, 2, 6,   6, 7, 3,
    6, 4, 5,   6, 5, 7,
    4, 1, 5,   1, 0, 5,
    3, 5, 0,   3, 7, 5,
    1, 4, 6,   1, 6, 2};
static const int identboxindexwirecount = 24;
static const WORD identboxindiceswire[identboxindexwirecount] = {
    0, 1,   1, 2,
    2, 3,   3, 0,
    4, 6,   6, 7,
    7, 5,   5, 4,
    1, 4,   2, 6,
    3, 7,   0, 5};
*/

#define SIGN(x) ((x < 0) ? -1 : 1)

using FLvertexVec = xr_vector<FVF::L>;

static FLvertexVec m_GridPoints;

u32 m_ColorAxis = 0xff000000;
u32 m_ColorGrid = 0xff909090;
u32 m_ColorGridTh = 0xffb4b4b4;
u32 m_SelectionRect = D3DCOLOR_RGBA(60, 255, 60, 50);

u32 m_ColorSafeRect = 0xffB040B0;

void SPrimitiveBuffer::CreateFromData(
    D3DPRIMITIVETYPE _pt, u32 _p_cnt, u32 FVF, LPVOID vertices, u32 _v_cnt, u16* indices, u32 _i_cnt)
{
#if defined(USE_DX11) || defined(USE_OGL)
    HRESULT handle_result;

    ID3DVertexBuffer* p_vertexbuffer = nullptr;
    D3D11_BUFFER_DESC buffer_desc = {0};
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.ByteWidth = sizeof(FVF::L) * _v_cnt;
    buffer_desc.CPUAccessFlags = NULL;
    buffer_desc.MiscFlags = NULL;

    D3D11_SUBRESOURCE_DATA buffer_data = {0};
    buffer_data.pSysMem = vertices;

    handle_result = HW.pDevice->CreateBuffer(&buffer_desc, &buffer_data, &p_vertexbuffer);
    if (FAILED(handle_result))
    {
        R_ASSERT2(false, "Can't create a vertex buffer!");
    }

    ID3DIndexBuffer* p_indexbuffer = nullptr;
    if (_i_cnt)
    {
        D3D11_BUFFER_DESC index_desc = {0};
        index_desc.Usage = D3D11_USAGE_DEFAULT;
        index_desc.ByteWidth = sizeof(unsigned int) * _i_cnt;
        index_desc.CPUAccessFlags = NULL;
        index_desc.MiscFlags = NULL;

        D3D11_SUBRESOURCE_DATA index_data = {0};
        index_data.pSysMem = indices;

        handle_result = HW.pDevice->CreateBuffer(&index_desc, &index_data, &p_indexbuffer);
        if (FAILED(handle_result))
        {
            R_ASSERT2(false, "Can't create an index buffer!");
        }
        OnRender.bind(this, &SPrimitiveBuffer::RenderDIP);
    }
    else
    {
        OnRender.bind(this, &SPrimitiveBuffer::RenderDP);
    }

    this->i_cnt = _i_cnt;
    this->p_cnt = p_cnt;
    this->p_type = _pt;
    this->v_cnt = _v_cnt;
    this->pGeom.create(FVF, p_vertexbuffer, p_indexbuffer);

#else //    USE_DX10
    ID3DVertexBuffer* pVB = nullptr;
    ID3DIndexBuffer* pIB = nullptr;
    p_cnt = _p_cnt;
    p_type = _pt;
    v_cnt = _v_cnt;
    i_cnt = _i_cnt;
    u32 stride = D3DXGetFVFVertexSize(FVF);
    R_CHK(HW.pDevice->CreateVertexBuffer(v_cnt * stride, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &pVB, nullptr));
    HW.stats_manager.increment_stats_vb(pVB);
    u8* bytes;
    R_CHK(pVB->Lock(0, 0, (LPVOID*)&bytes, 0));
    FLvertexVec verts(v_cnt);
    for (u32 k = 0; k < v_cnt; ++k)
        verts[k].set(((Fvector*)vertices)[k], 0xFFFFFFFF);
    memcpy(bytes, &*verts.begin(), v_cnt * stride);
    R_CHK(pVB->Unlock());
    if (i_cnt)
    {
        R_CHK(HW.pDevice->CreateIndexBuffer(
            i_cnt * sizeof(u16), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIB, NULL));
        HW.stats_manager.increment_stats_ib(pIB);
        R_CHK(pIB->Lock(0, 0, (LPVOID*)&bytes, 0));
        memcpy(bytes, indices, i_cnt * sizeof(u16));
        R_CHK(pIB->Unlock());
        OnRender.bind(this, &SPrimitiveBuffer::RenderDIP);
    }
    else
    {
        OnRender.bind(this, &SPrimitiveBuffer::RenderDP);
    }
    pGeom.create(FVF, pVB, pIB);
#endif //   USE_DX10
}
void SPrimitiveBuffer::Destroy()
{
#ifndef USE_OGL
    if (pGeom)
    {
        //   HW.stats_manager.decrement_stats_vb(pGeom->vb);
        //  HW.stats_manager.decrement_stats_ib(pGeom->ib);
        _RELEASE(pGeom->vb);
        _RELEASE(pGeom->ib);
        pGeom.destroy();
    }
#endif // !USE_OGL
}

void CDrawUtilities::UpdateGrid(int number_of_cell, float square_size, int subdiv)
{
    m_GridPoints.clear();
    // grid
    int m_GridSubDiv[2];
    int m_GridCounts[2];
    Fvector2 m_GridStep;

    m_GridStep.set(square_size, square_size);
    m_GridSubDiv[0] = subdiv;
    m_GridSubDiv[1] = subdiv;
    m_GridCounts[0] = number_of_cell; // iFloor(size/step)*subdiv;
    m_GridCounts[1] = number_of_cell; // iFloor(size/step)*subdiv;

    FVF::L left, right;
    left.p.y = right.p.y = 0;

    for (int thin = 0; thin < 2; thin++)
    {
        for (int i = -m_GridCounts[0]; i <= m_GridCounts[0]; i++)
        {
            if ((!!thin) != !!(i % m_GridSubDiv[0]))
            {
                left.p.z = -m_GridCounts[1] * m_GridStep.y;
                right.p.z = m_GridCounts[1] * m_GridStep.y;
                left.p.x = i * m_GridStep.x;
                right.p.x = left.p.x;
                float r = GridOptions::col[0];
                r *= 255;
                float g = GridOptions::col[1];
                g *= 255;
                float b = GridOptions::col[2];
                b *= 255;
                left.color = D3DCOLOR_RGBA((u32)r, (u32)g, (u32)b, 255);
                right.color = left.color;
                m_GridPoints.push_back(left);
                m_GridPoints.push_back(right);
            }
        }
        for (int i = -m_GridCounts[1]; i <= m_GridCounts[1]; i++)
        {
            if ((!!thin) != !!(i % m_GridSubDiv[1]))
            {
                left.p.x = -m_GridCounts[0] * m_GridStep.x;
                right.p.x = m_GridCounts[0] * m_GridStep.x;
                left.p.z = i * m_GridStep.y;
                right.p.z = left.p.z;
                float r = GridOptions::col[0];
                r *= 255;
                float g = GridOptions::col[1];
                g *= 255;
                float b = GridOptions::col[2];
                b *= 255;
                left.color = D3DCOLOR_RGBA((u32)r, (u32)g, (u32)b, 255);
                right.color = left.color;
                m_GridPoints.push_back(left);
                m_GridPoints.push_back(right);
            }
        }
    }
}

void CDrawUtilities::OnDeviceCreate()
{
    Device.seqRender.Add(this, REG_PRIORITY_LOW - 1000);

    for (int i = 0; i < LINE_DIVISION; i++)
    {
        float angle = M_PI * 2.f * (i / (float)LINE_DIVISION);
        float _sa = _sin(angle), _ca = _cos(angle);
        circledef1[i].x = _ca;
        circledef1[i].y = _sa;
        circledef1[i].z = 0;
        circledef2[i].x = 0;
        circledef2[i].y = _ca;
        circledef2[i].z = _sa;
        circledef3[i].x = _sa;
        circledef3[i].y = 0;
        circledef3[i].z = _ca;
    }
    // initialize identity box
    Fbox bb;
    bb.set(-0.505f, -0.505f, -0.505f, 0.505f, 0.505f, 0.505f);
    for (int i = 0; i < 8; i++)
    {
        Fvector S;
        Fvector p;
        bb.getpoint(i, p);
        S.set((float)SIGN(p.x), (float)SIGN(p.y), (float)SIGN(p.z));
        boxvert[i * 6 + 0].set(p);
        boxvert[i * 6 + 1].set(p.x - S.x * 0.25f, p.y, p.z);
        boxvert[i * 6 + 2].set(p);
        boxvert[i * 6 + 3].set(p.x, p.y - S.y * 0.25f, p.z);
        boxvert[i * 6 + 4].set(p);
        boxvert[i * 6 + 5].set(p.x, p.y, p.z - S.z * 0.25f);
    }
    // create render stream
    vs_L.create(FVF::F_L, RCache.Vertex.Buffer(), RCache.Index.Buffer());
    vs_TL.create(FVF::F_TL, RCache.Vertex.Buffer(), RCache.Index.Buffer());
    vs_LIT.create(FVF::F_LIT, RCache.Vertex.Buffer(), RCache.Index.Buffer());

    m_Font = new CGameFont("stat_font");
}

void CDrawUtilities::OnDeviceDestroy()
{
    Device.seqRender.Remove(this);
    xr_delete(m_Font);

    vs_L.destroy();
    vs_TL.destroy();
    vs_LIT.destroy();
}
//----------------

void CDrawUtilities::DrawSpotLight(const Fvector& p, const Fvector& d, float range, float phi, u32 clr)
{
    Fmatrix T;
    Fvector p1;
    float H, P;
    float da = PI_MUL_2 / LINE_DIVISION;
    float b = range * _cos(PI_DIV_2 - phi / 2);
    float a = range * _sin(PI_DIV_2 - phi / 2);
    d.getHP(H, P);
    T.setHPB(H, P, 0);
    T.translate_over(p);
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    FVF::L* pv = (FVF::L*)Stream->Lock(LINE_DIVISION * 2 + 2, vs_L->vb_stride, vBase);
    for (float angle = 0; angle < PI_MUL_2; angle += da)
    {
        float _sa = _sin(angle);
        float _ca = _cos(angle);
        p1.x = b * _ca;
        p1.y = b * _sa;
        p1.z = a;
        T.transform_tiny(p1);
        // fill VB
        pv->set(p, clr);
        pv++;
        pv->set(p1, clr);
        pv++;
    }
    p1.mad(p, d, range);
    pv->set(p, clr);
    pv++;
    pv->set(p1, clr);
    pv++;
    Stream->Unlock(LINE_DIVISION * 2 + 2, vs_L->vb_stride);
    // and Render it as triangle list
    DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, LINE_DIVISION + 1);
}

void CDrawUtilities::DrawDirectionalLight(const Fvector& p, const Fvector& d, float radius, float range, u32 c)
{
    float r = radius * 0.71f;
    Fvector R, N, D;
    D.normalize(d);
    Fmatrix rot;

    N.set(0, 1, 0);
    if (_abs(D.y) > 0.99f)
        N.set(1, 0, 0);
    R.crossproduct(N, D);
    R.normalize();
    N.crossproduct(D, R);
    N.normalize();
    rot.set(R, N, D, p);
    float sz = radius + range;

    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    FVF::L* pv = (FVF::L*)Stream->Lock(6, vs_L->vb_stride, vBase);
    pv->set(0, 0, r, c);
    rot.transform_tiny(pv->p);
    pv++;
    pv->set(0, 0, sz, c);
    rot.transform_tiny(pv->p);
    pv++;
    pv->set(-r, 0, r, c);
    rot.transform_tiny(pv->p);
    pv++;
    pv->set(-r, 0, sz, c);
    rot.transform_tiny(pv->p);
    pv++;
    pv->set(r, 0, r, c);
    rot.transform_tiny(pv->p);
    pv++;
    pv->set(r, 0, sz, c);
    rot.transform_tiny(pv->p);
    pv++;
    Stream->Unlock(6, vs_L->vb_stride);

    // and Render it as triangle list
    DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, 3);

    Fbox b;
    b.vMin.set(-r, -r, -r);
    b.vMax.set(r, r, r);

    DrawLineSphere(p, radius, c, true);
}

void CDrawUtilities::DrawPointLight(const Fvector& p, float radius, u32 c)
{
    RCache.set_xform_world(Fidentity);
    DrawCross(p, radius, radius, radius, radius, radius, radius, c, true);
}

void CDrawUtilities::DrawEntity(u32 clr, ref_shader s)
{
    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    {
        FVF::L* pv = (FVF::L*)Stream->Lock(5, vs_L->vb_stride, vBase);
        pv->set(0.f, 0.f, 0.f, clr);
        pv++;
        pv->set(0.f, 1.f, 0.f, clr);
        pv++;
        pv->set(0.f, 1.f, .5f, clr);
        pv++;
        pv->set(0.f, .5f, .5f, clr);
        pv++;
        pv->set(0.f, .5f, 0.f, clr);
        pv++;
    }
    Stream->Unlock(5, vs_L->vb_stride);
    // render flagshtok
    DU_DRAW_SH(RImplementation.m_WireShader);
    DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, vBase, 4);

    if (s)
        DU_DRAW_SH(s);
    {
        // fill VB
        FVF::LIT* pv = (FVF::LIT*)Stream->Lock(6, vs_LIT->vb_stride, vBase);
        pv->set(0.f, 1.f, 0.f, clr, 0.f, 0.f);
        pv++;
        pv->set(0.f, 1.f, .5f, clr, 1.f, 0.f);
        pv++;
        pv->set(0.f, .5f, .5f, clr, 1.f, 1.f);
        pv++;
        pv->set(0.f, .5f, 0.f, clr, 0.f, 1.f);
        pv++;
        pv->set(0.f, .5f, .5f, clr, 1.f, 1.f);
        pv++;
        pv->set(0.f, 1.f, .5f, clr, 1.f, 0.f);
        pv++;
        Stream->Unlock(6, vs_LIT->vb_stride);
        // and Render it as line list
        DU_DRAW_DP(D3DPT_TRIANGLEFAN, vs_LIT, vBase, 4);
    }
}

void CDrawUtilities::DrawFlag(
    const Fvector& p, float heading, float height, float sz, float sz_fl, u32 clr, BOOL bDrawEntity)
{
    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    {
        FVF::L* pv = (FVF::L*)Stream->Lock(2, vs_L->vb_stride, vBase);
        pv->set(p, clr);
        pv++;
        pv->set(p.x, p.y + height, p.z, clr);
        pv++;
    }
    Stream->Unlock(2, vs_L->vb_stride);
    // and Render it as triangle list
    DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, 1);

    if (bDrawEntity)
    {
        // fill VB
        float rx = _sin(heading);
        float rz = _cos(heading);
        FVF::L* pv = (FVF::L*)Stream->Lock(6, vs_L->vb_stride, vBase);
        sz *= 0.8f;
        pv->set(p.x, p.y + height, p.z, clr);
        pv++;
        pv->set(p.x + rx * sz, p.y + height, p.z + rz * sz, clr);
        pv++;
        sz *= 0.5f;
        pv->set(p.x, p.y + height * (1.f - sz_fl * .5f), p.z, clr);
        pv++;
        pv->set(p.x + rx * sz * 0.6f, p.y + height * (1.f - sz_fl * .5f), p.z + rz * sz * 0.75f, clr);
        pv++;
        pv->set(p.x, p.y + height * (1.f - sz_fl), p.z, clr);
        pv++;
        pv->set(p.x + rx * sz, p.y + height * (1.f - sz_fl), p.z + rz * sz, clr);
        pv++;
        Stream->Unlock(6, vs_L->vb_stride);
        // and Render it as line list
        DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, 3);
    }
    else
    {
        // fill VB
        FVF::L* pv = (FVF::L*)Stream->Lock(6, vs_L->vb_stride, vBase);
        pv->set(p.x, p.y + height * (1.f - sz_fl), p.z, clr);
        pv++;
        pv->set(p.x, p.y + height, p.z, clr);
        pv++;
        pv->set(p.x + _sin(heading) * sz, ((pv - 2)->p.y + (pv - 1)->p.y) / 2, p.z + _cos(heading) * sz, clr);
        pv++;
        pv->set(*(pv - 3));
        pv++;
        pv->set(*(pv - 2));
        pv++;
        pv->set(*(pv - 4));
        pv++;
        Stream->Unlock(6, vs_L->vb_stride);
        // and Render it as triangle list
        DU_DRAW_DP(D3DPT_TRIANGLELIST, vs_L, vBase, 2);
    }
}

//------------------------------------------------------------------------------

void CDrawUtilities::DrawRomboid(const Fvector& p, float r, u32 c)
{
    static const WORD IL[24] = {0, 2, 2, 5, 0, 5, 3, 5, 3, 0, 4, 3, 4, 0, 4, 2, 1, 2, 1, 5, 1, 3, 1, 4};
    static const WORD IT[24] = {2, 4, 0, 4, 3, 0, 3, 5, 0, 5, 2, 0, 4, 2, 1, 2, 5, 1, 5, 3, 1, 3, 4, 1};
    u32 vBase, iBase;

    Fcolor C;
    C.set(c);
    C.mul_rgb(0.75);
    u32 c1 = C.get();

    int k;
    FVF::L* pv;
    WORD* i;
    _VertexStream* Stream = &RCache.Vertex;
    _IndexStream* StreamI = &RCache.Index;

    // fill VB
    pv = (FVF::L*)Stream->Lock(6, vs_L->vb_stride, vBase);
    pv->set(p.x, p.y + r, p.z, c1);
    pv++;
    pv->set(p.x, p.y - r, p.z, c1);
    pv++;
    pv->set(p.x, p.y, p.z - r, c1);
    pv++;
    pv->set(p.x, p.y, p.z + r, c1);
    pv++;
    pv->set(p.x - r, p.y, p.z, c1);
    pv++;
    pv->set(p.x + r, p.y, p.z, c1);
    pv++;
    Stream->Unlock(6, vs_L->vb_stride);

    i = StreamI->Lock(24, iBase);
    for (k = 0; k < 24; k++, i++)
        *i = IT[k];
    StreamI->Unlock(24);

    // and Render it as triangle list
    DU_DRAW_DIP(D3DPT_TRIANGLELIST, vs_L, vBase, 0, 6, iBase, 12);

    // draw lines
    pv = (FVF::L*)Stream->Lock(6, vs_L->vb_stride, vBase);
    pv->set(p.x, p.y + r, p.z, c);
    pv++;
    pv->set(p.x, p.y - r, p.z, c);
    pv++;
    pv->set(p.x, p.y, p.z - r, c);
    pv++;
    pv->set(p.x, p.y, p.z + r, c);
    pv++;
    pv->set(p.x - r, p.y, p.z, c);
    pv++;
    pv->set(p.x + r, p.y, p.z, c);
    pv++;
    Stream->Unlock(6, vs_L->vb_stride);

    i = StreamI->Lock(24, iBase);
    for (k = 0; k < 24; k++, i++)
        *i = IL[k];
    StreamI->Unlock(24);

    DU_DRAW_DIP(D3DPT_LINELIST, vs_L, vBase, 0, 6, iBase, 12);
}
//------------------------------------------------------------------------------

void CDrawUtilities::DrawSound(const Fvector& p, float r, u32 c) { DrawCross(p, r, r, r, r, r, r, c, true); }
//------------------------------------------------------------------------------
void CDrawUtilities::DrawIdentCone(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w) { VERIFY("Lord реализовать это!"); }

void CDrawUtilities::DrawIdentSphere(const Fmatrix& transform, bool selected)
{
#pragma region GeometryWire
    {
        _VertexStream* stream_vertex = &RCache.Vertex;
        _IndexStream* stream_index = &RCache.Index;
        unsigned int v_base;
        unsigned int i_base;

        FVF::L* pv = (FVF::L*)stream_vertex->Lock(DU_SPHERE_NUMVERTEXL, vs_L->vb_stride, v_base);
        for (int k = 0; k < DU_SPHERE_NUMVERTEXL; ++k, pv++)
        {
            pv->color = Cordis::SDK::ShapesColors::kEdgeColor;
            pv->p = du_sphere_verticesl[k];
        }
        stream_vertex->Unlock(DU_SPHERE_NUMVERTEXL, vs_L->vb_stride);

        WORD* index = stream_index->Lock(DU_SPHERE_NUMLINES * 2, i_base);

        for (int k = 0; k < DU_SPHERE_NUMLINES * 2; ++k, index++)
            *index = du_sphere_lines[k];

        stream_index->Unlock(DU_SPHERE_NUMLINES * 2);
        RCache.set_xform_world(transform);
        RCache.set_Shader(RImplementation.m_WireShader);
        DU_DRAW_DIP(D3DPT_LINELIST, vs_L, v_base, 0, DU_SPHERE_NUMVERTEXL, i_base, DU_SPHERE_NUMLINES);
    }
#pragma endregion

#pragma region GeometrySolid
    {
        _VertexStream* stream_vertex = &RCache.Vertex;
        _IndexStream* stream_index = &RCache.Index;
        unsigned int v_base;
        unsigned int i_base;

        FVF::L* buffer_vertex = (FVF::L*)stream_vertex->Lock(DU_SPHERE_NUMVERTEX, vs_L->vb_stride, v_base);

        for (int k = 0; k < DU_SPHERE_NUMVERTEX; ++k, buffer_vertex++)
        {
            buffer_vertex->color = Cordis::SDK::ShapesColors::kTransparentColor;
            buffer_vertex->p = du_sphere_vertices[k];
        }

        stream_vertex->Unlock(DU_SPHERE_NUMVERTEX, vs_L->vb_stride);

        WORD* buffer_index = stream_index->Lock(DU_SPHERE_NUMFACES * 3, i_base);

        for (int k = 0; k < DU_SPHERE_NUMFACES * 3; ++k, buffer_index++)
            *buffer_index = du_sphere_faces[k];

        stream_index->Unlock(DU_SPHERE_NUMFACES * 3);

        RCache.set_Shader(RImplementation.m_SelectionShader);

        if (selected)
            RCache.set_c("tfactor", 1, 1, 1, 3);

        DU_DRAW_DIP(D3DPT_TRIANGLELIST, vs_L, v_base, 0, DU_SPHERE_NUMVERTEX, i_base, DU_SPHERE_NUMFACES);
    }
#pragma endregion
}

void CDrawUtilities::DrawIdentSpherePart(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w) { VERIFY("Lord реализовать!"); }

void CDrawUtilities::DrawIdentCylinder(BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w) { VERIFY("Lord реализовать!"); }

void CDrawUtilities::DrawIdentBox(const Fmatrix& transform, bool selected)
{
#pragma region GeometryWire
    {
        _VertexStream* stream = &RCache.Vertex;
        _IndexStream* stream_index = &RCache.Index;
        unsigned int v_base;
        unsigned int i_base;

        FVF::L* pv = (FVF::L*)stream->Lock(DU_BOX_NUMVERTEX, vs_L->vb_stride, v_base);

        for (int k = 0; k < DU_BOX_NUMVERTEX; ++k, pv++)
            *pv = du_box_vertices[k];

        stream->Unlock(DU_BOX_NUMVERTEX, vs_L->vb_stride);

        WORD* i = stream_index->Lock(DU_BOX_NUMLINES * 2, i_base);

        for (int k = 0; k < DU_BOX_NUMLINES * 2; ++k, i++)
            *i = du_box_lines[k];

        stream_index->Unlock(DU_BOX_NUMLINES * 2);

        RCache.set_xform_world(transform);
        RCache.set_Shader(RImplementation.m_WireShader);
        DU_DRAW_DIP(D3DPT_LINELIST, vs_L, v_base, 0, DU_BOX_NUMVERTEX, i_base, DU_BOX_NUMLINES);
    }

#pragma endregion

#pragma region GeometrySolid
    {
        _VertexStream* stream_vertex = &RCache.Vertex;
        _IndexStream* stream_index = &RCache.Index;
        unsigned int v_base;
        unsigned int i_base;

        FVF::L* pv = (FVF::L*)stream_vertex->Lock(DU_BOX_NUMVERTEX, vs_L->vb_stride, v_base);

        for (int k = 0; k < DU_BOX_NUMVERTEX; ++k, pv++)
        {
            pv->color = Cordis::SDK::ShapesColors::kTransparentColor;
            pv->p = du_box_vertices[k].p;
        }

        stream_vertex->Unlock(DU_BOX_NUMVERTEX, vs_L->vb_stride);

        WORD* index = stream_index->Lock(36, i_base);

        for (int k = 0; k < 36; ++k, index++)
            *index = du_box_faces[k];

        stream_index->Unlock(36);
        RCache.set_Shader(RImplementation.m_SelectionShader);

        if (selected)
            RCache.set_c("tfactor", 1, 1, 1, 3);

        DU_DRAW_DIP(D3DPT_TRIANGLELIST, vs_L, v_base, 0, 8, i_base, 12);
    }
#pragma endregion
}

void CDrawUtilities::DrawLineSphere(const Fvector& p, float radius, u32 c, BOOL bCross)
{
    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    int i;
    FVF::L* pv;
    // seg 0
    pv = (FVF::L*)Stream->Lock(LINE_DIVISION + 1, vs_L->vb_stride, vBase);
    for (i = 0; i < LINE_DIVISION; i++, pv++)
    {
        pv->p.mad(p, circledef1[i], radius);
        pv->color = c;
    }
    pv->set(*(pv - LINE_DIVISION));
    Stream->Unlock(LINE_DIVISION + 1, vs_L->vb_stride);
    DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, vBase, LINE_DIVISION);
    // seg 1
    pv = (FVF::L*)Stream->Lock(LINE_DIVISION + 1, vs_L->vb_stride, vBase);
    for (i = 0; i < LINE_DIVISION; i++)
    {
        pv->p.mad(p, circledef2[i], radius);
        pv->color = c;
        pv++;
    }
    pv->set(*(pv - LINE_DIVISION));
    pv++;
    Stream->Unlock(LINE_DIVISION + 1, vs_L->vb_stride);
    DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, vBase, LINE_DIVISION);
    // seg 2
    pv = (FVF::L*)Stream->Lock(LINE_DIVISION + 1, vs_L->vb_stride, vBase);
    for (i = 0; i < LINE_DIVISION; i++)
    {
        pv->p.mad(p, circledef3[i], radius);
        pv->color = c;
        pv++;
    }
    pv->set(*(pv - LINE_DIVISION));
    pv++;
    Stream->Unlock(LINE_DIVISION + 1, vs_L->vb_stride);
    DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, vBase, LINE_DIVISION);

    if (bCross)
        DrawCross(p, radius, radius, radius, radius, radius, radius, c);
}

//----------------------------------------------------
#ifdef _EDITOR
IC float _x2real(float x) { return (x + 1) * Device.m_RenderWidth_2; }
IC float _y2real(float y) { return (y + 1) * Device.m_RenderHeight_2; }
#else
IC float _x2real(float x) { return (x + 1) * Device.dwWidth * 0.5f; }
IC float _y2real(float y) { return (y + 1) * Device.dwHeight * 0.5f; }
#endif

void CDrawUtilities::dbgDrawPlacement(const Fvector& p, int sz, u32 clr, LPCSTR caption, u32 clr_font)
{
    VERIFY(Device.b_is_Ready);
    Fvector c;
    float w = p.x * Device.mFullTransform._14 + p.y * Device.mFullTransform._24 + p.z * Device.mFullTransform._34 +
        Device.mFullTransform._44;
    if (w < 0)
        return; // culling

    float s = (float)sz;
    Device.mFullTransform.transform(c, p);
    c.x = (float)iFloor(_x2real(c.x));
    c.y = (float)iFloor(_y2real(-c.y));

    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    FVF::TL* pv = (FVF::TL*)Stream->Lock(5, vs_TL->vb_stride, vBase);
    pv->p.set(c.x - s, c.y - s, 0, 1);
    pv->color = clr;
    pv++;
    pv->p.set(c.x + s, c.y - s, 0, 1);
    pv->color = clr;
    pv++;
    pv->p.set(c.x + s, c.y + s, 0, 1);
    pv->color = clr;
    pv++;
    pv->p.set(c.x - s, c.y + s, 0, 1);
    pv->color = clr;
    pv++;
    pv->p.set(c.x - s, c.y - s, 0, 1);
    pv->color = clr;
    pv++;
    Stream->Unlock(5, vs_TL->vb_stride);

    // Render it as line strip
    // DU_DRAW_SH(RImplementation.m_WireShader);
    DU_DRAW_DP(D3DPT_LINESTRIP, vs_TL, vBase, 4);
    if (caption)
    {
        m_Font->SetColor(clr_font);
        m_Font->Out(c.x, c.y + s, "%s", caption);
    }
}

void CDrawUtilities::dbgDrawVert(const Fvector& p0, u32 clr, LPCSTR caption)
{
    dbgDrawPlacement(p0, 1, clr, caption);
    DrawCross(p0, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, clr, false);
}

void CDrawUtilities::dbgDrawEdge(const Fvector& p0, const Fvector& p1, u32 clr, LPCSTR caption)
{
    dbgDrawPlacement(p0, 1, clr, caption);
    DrawCross(p0, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, clr, false);
    DrawCross(p1, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, clr, false);
    DrawLine(p0, p1, clr);
}

void CDrawUtilities::dbgDrawFace(const Fvector& p0, const Fvector& p1, const Fvector& p2, u32 clr, LPCSTR caption)
{
    dbgDrawPlacement(p0, 1, clr, caption);
    DrawCross(p0, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, clr, false);
    DrawCross(p1, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, clr, false);
    DrawCross(p2, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, clr, false);
    DrawLine(p0, p1, clr);
    DrawLine(p1, p2, clr);
    DrawLine(p2, p0, clr);
}
//----------------------------------------------------

void CDrawUtilities::DrawLine(const Fvector& p0, const Fvector& p1, u32 c)
{
    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    FVF::L* pv = (FVF::L*)Stream->Lock(2, vs_L->vb_stride, vBase);
    pv->set(p0, c);
    pv++;
    pv->set(p1, c);
    pv++;
    Stream->Unlock(2, vs_L->vb_stride);
    // and Render it as triangle list
    DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, 2);
}

//----------------------------------------------------
void CDrawUtilities::DrawSelectionBox(const Fvector& C, const Fvector& S, u32* c)
{
    u32 cc = (c) ? *c : boxcolor;

    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    FVF::L* pv = (FVF::L*)Stream->Lock(boxvertcount, vs_L->vb_stride, vBase);
    for (int i = 0; i < boxvertcount; i++, pv++)
    {
        pv->p.mul(boxvert[i], S);
        pv->p.add(C);
        pv->color = cc;
    }
    Stream->Unlock(boxvertcount, vs_L->vb_stride);

    // and Render it as triangle list
    // DU_DRAW_RS(D3DRS_FILLMODE, D3DFILL_SOLID);
    DU_DRAW_SH(RImplementation.m_WireShader)
    DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, boxvertcount / 2);
    //    DU_DRAW_RS(D3DRS_FILLMODE, FILL_MODE);
}

void CDrawUtilities::DrawBox(const Fvector& offs, const Fvector& Size, BOOL bSolid, BOOL bWire, u32 clr_s, u32 clr_w)
{
    _VertexStream* Stream = &RCache.Vertex;
    if (bWire)
    {
        u32 vBase;
        FVF::L* pv = (FVF::L*)Stream->Lock(identboxwirecount, vs_L->vb_stride, vBase);
        for (int i = 0; i < identboxwirecount; i++, pv++)
        {
            pv->p.mul(identboxwire[i], Size);
            pv->p.mul(2);
            pv->p.add(offs);
            pv->color = clr_w;
        }
        Stream->Unlock(identboxwirecount, vs_L->vb_stride);

        DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, identboxwirecount / 2);
    }
    if (bSolid)
    {
        u32 vBase;
        FVF::L* pv = (FVF::L*)Stream->Lock(DU_BOX_NUMVERTEX2, vs_L->vb_stride, vBase);
        for (int i = 0; i < DU_BOX_NUMVERTEX2; i++, pv++)
        {
            pv->p.mul(du_box_vertices2[i], Size);
            pv->p.mul(2);
            pv->p.add(offs);
            pv->color = clr_s;
        }
        Stream->Unlock(DU_BOX_NUMVERTEX2, vs_L->vb_stride);

        DU_DRAW_DP(D3DPT_TRIANGLELIST, vs_L, vBase, DU_BOX_NUMFACES);
    }
}
//----------------------------------------------------

void CDrawUtilities::DrawOBB(const Fmatrix& parent, const Fobb& box, u32 clr_s, u32 clr_w)
{
    Fmatrix R, S, X;
    box.xform_get(R);
    S.scale(box.m_halfsize.x * 2.f, box.m_halfsize.y * 2.f, box.m_halfsize.z * 2.f);
    X.mul_43(R, S);
    R.mul_43(parent, X);
    RCache.set_xform_world(R);
    DrawIdentBox(R);
}
//----------------------------------------------------

void CDrawUtilities::DrawAABB(
    const Fmatrix& parent, const Fvector& center, const Fvector& size, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
    Fmatrix R, S;
    S.scale(size.x * 2.f, size.y * 2.f, size.z * 2.f);
    S.translate_over(center);
    R.mul_43(parent, S);
    RCache.set_xform_world(R);
    DrawIdentBox(R);
}

void CDrawUtilities::DrawAABB(const Fvector& p0, const Fvector& p1, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
    Fmatrix R;
    Fvector C;
    C.set((p1.x + p0.x) * 0.5f, (p1.y + p0.y) * 0.5f, (p1.z + p0.z) * 0.5f);
    R.scale(_abs(p1.x - p0.x), _abs(p1.y - p0.y), _abs(p1.z - p0.z));
    R.translate_over(C);
    RCache.set_xform_world(R);
    DrawIdentBox(R);
}

void CDrawUtilities::DrawSphere(
    const Fmatrix& parent, const Fvector& center, float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
    Fmatrix B;
    B.scale(radius, radius, radius);
    B.translate_over(center);
    B.mulA_43(parent);
    // RCache.set_xform_world(B);
    DrawIdentSphere(B);
}
//----------------------------------------------------

void CDrawUtilities::DrawFace(
    const Fvector& p0, const Fvector& p1, const Fvector& p2, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
    _VertexStream* Stream = &RCache.Vertex;

    u32 vBase;
    if (bSolid)
    {
        FVF::L* pv = (FVF::L*)Stream->Lock(3, vs_L->vb_stride, vBase);
        pv->set(p0, clr_s);
        pv++;
        pv->set(p1, clr_s);
        pv++;
        pv->set(p2, clr_s);
        pv++;
        Stream->Unlock(3, vs_L->vb_stride);
        DU_DRAW_DP(D3DPT_TRIANGLELIST, vs_L, vBase, 1);
    }
    if (bWire)
    {
        FVF::L* pv = (FVF::L*)Stream->Lock(4, vs_L->vb_stride, vBase);
        pv->set(p0, clr_w);
        pv++;
        pv->set(p1, clr_w);
        pv++;
        pv->set(p2, clr_w);
        pv++;
        pv->set(p0, clr_w);
        pv++;
        Stream->Unlock(4, vs_L->vb_stride);
        DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, vBase, 3);
    }
}
//----------------------------------------------------

static const u32 MAX_VERT_COUNT = 0xFFFF;
void CDrawUtilities::DD_DrawFace_begin(BOOL bWire)
{
    VERIFY(m_DD_pv_start == nullptr);
    m_DD_wire = bWire;
    m_DD_pv_start = (FVF::L*)RCache.Vertex.Lock(MAX_VERT_COUNT, vs_L->vb_stride, m_DD_base);
    m_DD_pv = m_DD_pv_start;
}
void CDrawUtilities::DD_DrawFace_flush(BOOL try_again)
{
    RCache.Vertex.Unlock((u32)(m_DD_pv - m_DD_pv_start), vs_L->vb_stride);
    if (m_DD_wire)
        DU_DRAW_RS(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    DU_DRAW_DP(D3DPT_TRIANGLELIST, vs_L, m_DD_base, u32(m_DD_pv - m_DD_pv_start) / 3);
    if (m_DD_wire)
        DU_DRAW_RS(D3DRS_FILLMODE, FILL_MODE);
    if (try_again)
    {
        m_DD_pv_start = (FVF::L*)RCache.Vertex.Lock(MAX_VERT_COUNT, vs_L->vb_stride, m_DD_base);
        m_DD_pv = m_DD_pv_start;
    }
}
void CDrawUtilities::DD_DrawFace_push(const Fvector& p0, const Fvector& p1, const Fvector& p2, u32 clr)
{
    m_DD_pv->set(p0, clr);
    m_DD_pv++;
    m_DD_pv->set(p1, clr);
    m_DD_pv++;
    m_DD_pv->set(p2, clr);
    m_DD_pv++;
    if (m_DD_pv - m_DD_pv_start == MAX_VERT_COUNT)
        DD_DrawFace_flush(TRUE);
}
void CDrawUtilities::DD_DrawFace_end()
{
    DD_DrawFace_flush(FALSE);
    m_DD_pv_start = nullptr;
}
//----------------------------------------------------

void CDrawUtilities::DrawCylinder(const Fmatrix& parent, const Fvector& center, const Fvector& dir, float height,
    float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
    Fmatrix mScale;
    mScale.scale(2.f * radius, 2.f * radius, height);

    // build final rotation / translation
    Fvector L_dir, L_up, L_right;
    L_dir.set(dir);
    L_dir.normalize();
    L_up.set(0, 1, 0);
    if (_abs(L_up.dotproduct(L_dir)) > .99f)
        L_up.set(0, 0, 1);
    L_right.crossproduct(L_up, L_dir);
    L_right.normalize();
    L_up.crossproduct(L_dir, L_right);
    L_up.normalize();

    Fmatrix mR;
    mR.i = L_right;
    mR._14 = 0;
    mR.j = L_up;
    mR._24 = 0;
    mR.k = L_dir;
    mR._34 = 0;
    mR.c = center;
    mR._44 = 1;

    // final xform
    Fmatrix xf;
    xf.mul(mR, mScale);
    xf.mulA_43(parent);
    RCache.set_xform_world(xf);
    DrawIdentCylinder(bSolid, bWire, clr_s, clr_w);
}
//----------------------------------------------------

void CDrawUtilities::DrawCone(const Fmatrix& parent, const Fvector& apex, const Fvector& dir, float height,
    float radius, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
    Fmatrix mScale;
    mScale.scale(2.f * radius, 2.f * radius, height);

    // build final rotation / translation
    Fvector L_dir, L_up, L_right;
    L_dir.set(dir);
    L_dir.normalize();
    L_up.set(0, 1, 0);
    if (_abs(L_up.dotproduct(L_dir)) > .99f)
        L_up.set(0, 0, 1);
    L_right.crossproduct(L_up, L_dir);
    L_right.normalize();
    L_up.crossproduct(L_dir, L_right);
    L_up.normalize();

    Fmatrix mR;
    mR.i = L_right;
    mR._14 = 0;
    mR.j = L_up;
    mR._24 = 0;
    mR.k = L_dir;
    mR._34 = 0;
    mR.c = apex;
    mR._44 = 1;

    // final xform
    Fmatrix xf;
    xf.mul(mR, mScale);
    xf.mulA_43(parent);
    RCache.set_xform_world(xf);
    DrawIdentCone(bSolid, bWire, clr_s, clr_w);
}
//----------------------------------------------------

void CDrawUtilities::DrawPlane(const Fvector& p, const Fvector& n, const Fvector2& scale, u32 clr_s, u32 clr_w,
    BOOL bCull, BOOL bSolid, BOOL bWire)
{
    if (n.square_magnitude() < EPS_S)
        return;
    // build final rotation / translation
    Fvector L_dir, L_up = n, L_right;
    L_dir.set(0, 0, 1);
    if (_abs(L_up.dotproduct(L_dir)) > .99f)
        L_dir.set(1, 0, 0);
    L_right.crossproduct(L_up, L_dir);
    L_right.normalize();
    L_dir.crossproduct(L_right, L_up);
    L_dir.normalize();

    Fmatrix mR;
    mR.i = L_right;
    mR._14 = 0;
    mR.j = L_up;
    mR._24 = 0;
    mR.k = L_dir;
    mR._34 = 0;
    mR.c = p;
    mR._44 = 1;

    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;

    if (bSolid)
    {
        DU_DRAW_SH(RImplementation.m_SelectionShader);
        FVF::L* pv = (FVF::L*)Stream->Lock(5, vs_L->vb_stride, vBase);
        pv->set(-scale.x, 0, -scale.y, clr_s);
        mR.transform_tiny(pv->p);
        pv++;
        pv->set(-scale.x, 0, +scale.y, clr_s);
        mR.transform_tiny(pv->p);
        pv++;
        pv->set(+scale.x, 0, +scale.y, clr_s);
        mR.transform_tiny(pv->p);
        pv++;
        pv->set(+scale.x, 0, -scale.y, clr_s);
        mR.transform_tiny(pv->p);
        pv++;
        pv->set(*(pv - 4));
        Stream->Unlock(5, vs_L->vb_stride);
        if (!bCull)
            DU_DRAW_RS(D3DRS_CULLMODE, D3DCULL_NONE);
        DU_DRAW_DP(D3DPT_TRIANGLEFAN, vs_L, vBase, 2);
        if (!bCull)
            DU_DRAW_RS(D3DRS_CULLMODE, D3DCULL_CCW);
    }

    if (bWire)
    {
        DU_DRAW_SH(RImplementation.m_WireShader);
        FVF::L* pv = (FVF::L*)Stream->Lock(5, vs_L->vb_stride, vBase);
        pv->set(-scale.x, 0, -scale.y, clr_w);
        mR.transform_tiny(pv->p);
        pv++;
        pv->set(+scale.x, 0, -scale.y, clr_w);
        mR.transform_tiny(pv->p);
        pv++;
        pv->set(+scale.x, 0, +scale.y, clr_w);
        mR.transform_tiny(pv->p);
        pv++;
        pv->set(-scale.x, 0, +scale.y, clr_w);
        mR.transform_tiny(pv->p);
        pv++;
        pv->set(*(pv - 4));
        Stream->Unlock(5, vs_L->vb_stride);
        DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, vBase, 4);
    }
}
//----------------------------------------------------

void CDrawUtilities::DrawPlane(const Fvector& center, const Fvector2& scale, const Fvector& rotate, u32 clr_s,
    u32 clr_w, BOOL bCull, BOOL bSolid, BOOL bWire)
{
    Fmatrix M;
    M.setHPB(rotate.y, rotate.x, rotate.z);
    M.translate_over(center);
    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;

    if (bSolid)
    {
        DU_DRAW_SH(RImplementation.m_SelectionShader);
        FVF::L* pv = (FVF::L*)Stream->Lock(5, vs_L->vb_stride, vBase);
        pv->set(-scale.x, 0, -scale.y, clr_s);
        M.transform_tiny(pv->p);
        pv++;
        pv->set(-scale.x, 0, +scale.y, clr_s);
        M.transform_tiny(pv->p);
        pv++;
        pv->set(+scale.x, 0, +scale.y, clr_s);
        M.transform_tiny(pv->p);
        pv++;
        pv->set(+scale.x, 0, -scale.y, clr_s);
        M.transform_tiny(pv->p);
        pv++;
        pv->set(*(pv - 4));
        Stream->Unlock(5, vs_L->vb_stride);
        if (!bCull)
            DU_DRAW_RS(D3DRS_CULLMODE, D3DCULL_NONE);
        DU_DRAW_DP(D3DPT_TRIANGLEFAN, vs_L, vBase, 2);
        if (!bCull)
            DU_DRAW_RS(D3DRS_CULLMODE, D3DCULL_CCW);
    }

    if (bWire)
    {
        DU_DRAW_SH(RImplementation.m_WireShader);
        FVF::L* pv = (FVF::L*)Stream->Lock(5, vs_L->vb_stride, vBase);
        pv->set(-scale.x, 0, -scale.y, clr_w);
        M.transform_tiny(pv->p);
        pv++;
        pv->set(+scale.x, 0, -scale.y, clr_w);
        M.transform_tiny(pv->p);
        pv++;
        pv->set(+scale.x, 0, +scale.y, clr_w);
        M.transform_tiny(pv->p);
        pv++;
        pv->set(-scale.x, 0, +scale.y, clr_w);
        M.transform_tiny(pv->p);
        pv++;
        pv->set(*(pv - 4));
        Stream->Unlock(5, vs_L->vb_stride);
        DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, vBase, 4);
    }
}
//----------------------------------------------------

void CDrawUtilities::DrawRectangle(
    const Fvector& o, const Fvector& u, const Fvector& v, u32 clr_s, u32 clr_w, BOOL bSolid, BOOL bWire)
{
    _VertexStream* Stream = &RCache.Vertex;

    u32 vBase;
    if (bSolid)
    {
        DU_DRAW_SH(RImplementation.m_SelectionShader);
        FVF::L* pv = (FVF::L*)Stream->Lock(6, vs_L->vb_stride, vBase);
        pv->set(o.x, o.y, o.z, clr_s);
        pv++;
        pv->set(o.x + u.x + v.x, o.y + u.y + v.y, o.z + u.z + v.z, clr_s);
        pv++;
        pv->set(o.x + v.x, o.y + v.y, o.z + v.z, clr_s);
        pv++;
        pv->set(o.x, o.y, o.z, clr_s);
        pv++;
        pv->set(o.x + u.x, o.y + u.y, o.z + u.z, clr_s);
        pv++;
        pv->set(o.x + u.x + v.x, o.y + u.y + v.y, o.z + u.z + v.z, clr_s);
        pv++;
        Stream->Unlock(6, vs_L->vb_stride);
        DU_DRAW_DP(D3DPT_TRIANGLELIST, vs_L, vBase, 2);
    }
    if (bWire)
    {
        DU_DRAW_SH(RImplementation.m_WireShader);
        FVF::L* pv = (FVF::L*)Stream->Lock(5, vs_L->vb_stride, vBase);
        pv->set(o.x, o.y, o.z, clr_w);
        pv++;
        pv->set(o.x + u.x, o.y + u.y, o.z + u.z, clr_w);
        pv++;
        pv->set(o.x + u.x + v.x, o.y + u.y + v.y, o.z + u.z + v.z, clr_w);
        pv++;
        pv->set(o.x + v.x, o.y + v.y, o.z + v.z, clr_w);
        pv++;
        pv->set(o.x, o.y, o.z, clr_w);
        pv++;
        Stream->Unlock(5, vs_L->vb_stride);
        DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, vBase, 4);
    }
}
//----------------------------------------------------

void CDrawUtilities::DrawCross(
    const Fvector& p, float szx1, float szy1, float szz1, float szx2, float szy2, float szz2, u32 clr, BOOL bRot45)
{
    _VertexStream* Stream = &RCache.Vertex;
    // actual rendering
    u32 vBase;
    FVF::L* pv = (FVF::L*)Stream->Lock(bRot45 ? 12 : 6, vs_L->vb_stride, vBase);
    pv->set(p.x + szx2, p.y, p.z, clr);
    pv++;
    pv->set(p.x - szx1, p.y, p.z, clr);
    pv++;
    pv->set(p.x, p.y + szy2, p.z, clr);
    pv++;
    pv->set(p.x, p.y - szy1, p.z, clr);
    pv++;
    pv->set(p.x, p.y, p.z + szz2, clr);
    pv++;
    pv->set(p.x, p.y, p.z - szz1, clr);
    pv++;
    if (bRot45)
    {
        Fmatrix M;
        M.setHPB(PI_DIV_4, PI_DIV_4, PI_DIV_4);
        for (int i = 0; i < 6; i++, pv++)
        {
            pv->p.sub((pv - 6)->p, p);
            M.transform_dir(pv->p);
            pv->p.add(p);
            pv->color = clr;
        }
    }
    // unlock VB and Render it as triangle list
    Stream->Unlock(bRot45 ? 12 : 6, vs_L->vb_stride);
    RCache.set_xform_world(Fidentity);
    DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, bRot45 ? 6 : 3);
}

void CDrawUtilities::DrawPivot(const Fvector& pos, float sz)
{
    DU_DRAW_SH(RImplementation.m_WireShader);
    DrawCross(pos, sz, sz, sz, sz, sz, sz, 0xFF7FFF7F);
}

void CDrawUtilities::DrawAxis(const Fmatrix& T)
{
    _VertexStream* Stream = &RCache.Vertex;
    Fvector p[6];
    u32 c[6];

    // colors
    c[0] = c[2] = c[4] = 0x00222222;
    c[1] = 0x00FF0000;
    c[3] = 0x0000FF00;
    c[5] = 0x000000FF;

    // position
    p[0].mad(T.c, T.k, 0.25f);
    p[1].set(p[0]);
    p[1].x += .015f;
    p[2].set(p[0]);
    p[3].set(p[0]);
    p[3].y += .015f;
    p[4].set(p[0]);
    p[5].set(p[0]);
    p[5].z += .015f;

    u32 vBase;
    FVF::TL* pv = (FVF::TL*)Stream->Lock(6, vs_TL->vb_stride, vBase);
    // transform to screen
    float dx = -float(Device.dwWidth) / 2.2f;
    float dy = float(Device.dwHeight) / 2.25f;

    for (int i = 0; i < 6; i++, pv++)
    {
        pv->color = c[i];
        pv->transform(p[i], Device.mFullTransform);
        pv->p.set((float)iFloor(_x2real(pv->p.x) + dx), (float)iFloor(_y2real(pv->p.y) + dy), 0, 1);
        p[i].set(pv->p.x, pv->p.y, 0);
    }

    // unlock VB and Render it as triangle list
    Stream->Unlock(6, vs_TL->vb_stride);
    //   DU_DRAW_RS(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    DU_DRAW_SH(RImplementation.m_WireShader);
    DU_DRAW_DP(D3DPT_LINELIST, vs_TL, vBase, 3);
    //  DU_DRAW_RS(D3DRS_SHADEMODE, SHADE_MODE);

    m_Font->SetColor(0xFF909090);
    m_Font->Out(p[1].x, p[1].y, "x");
    m_Font->Out(p[3].x, p[3].y, "y");
    m_Font->Out(p[5].x, p[5].y, "z");
    m_Font->SetColor(0xFF000000);
    m_Font->Out(p[1].x - 1, p[1].y - 1, "x");
    m_Font->Out(p[3].x - 1, p[3].y - 1, "y");
    m_Font->Out(p[5].x - 1, p[5].y - 1, "z");
}

void CDrawUtilities::DrawObjectAxis(const Fmatrix& T, float sz, BOOL sel)
{
    VERIFY(Device.b_is_Ready);
    _VertexStream* Stream = &RCache.Vertex;
    Fvector c, r, n, d;
    float w = T.c.x * Device.mFullTransform._14 + T.c.y * Device.mFullTransform._24 +
        T.c.z * Device.mFullTransform._34 + Device.mFullTransform._44;
    if (w < 0)
        return; // culling

    float s = w * sz;
    Device.mFullTransform.transform(c, T.c);
    r.mul(T.i, s);
    r.add(T.c);
    Device.mFullTransform.transform(r);
    n.mul(T.j, s);
    n.add(T.c);
    Device.mFullTransform.transform(n);
    d.mul(T.k, s);
    d.add(T.c);
    Device.mFullTransform.transform(d);
    c.x = (float)iFloor(_x2real(c.x));
    c.y = (float)iFloor(_y2real(-c.y));
    r.x = (float)iFloor(_x2real(r.x));
    r.y = (float)iFloor(_y2real(-r.y));
    n.x = (float)iFloor(_x2real(n.x));
    n.y = (float)iFloor(_y2real(-n.y));
    d.x = (float)iFloor(_x2real(d.x));
    d.y = (float)iFloor(_y2real(-d.y));

    u32 vBase;
    FVF::L* pv = (FVF::L*)Stream->Lock(6, vs_L->vb_stride, vBase);
    pv->set(c.x, c.y, 0, 0xFF222222);
    pv++;
    pv->set(d.x, d.y, 0, sel ? 0xFF0000FF : 0xFF000080);
    pv++;
    pv->set(c.x, c.y, 0, 0xFF222222);
    pv++;
    pv->set(r.x, r.y, 0, sel ? 0xFFFF0000 : 0xFF800000);
    pv++;
    pv->set(c.x, c.y, 0, 0xFF222222);
    pv++;
    pv->set(n.x, n.y, 0, sel ? 0xFF00FF00 : 0xFF008000);
    Stream->Unlock(6, vs_L->vb_stride);

    // Render it as line list
    //    DU_DRAW_RS(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    DU_DRAW_SH(RImplementation.m_WireShader);
    DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, 3);
    //   DU_DRAW_RS(D3DRS_SHADEMODE, SHADE_MODE);

    m_Font->SetColor(sel ? 0xFF000000 : 0xFF909090);
    m_Font->Out(r.x, r.y, "x");
    m_Font->Out(n.x, n.y, "y");
    m_Font->Out(d.x, d.y, "z");
    m_Font->SetColor(sel ? 0xFFFFFFFF : 0xFF000000);
    m_Font->Out(r.x - 1, r.y - 1, "x");
    m_Font->Out(n.x - 1, n.y - 1, "y");
    m_Font->Out(d.x - 1, d.y - 1, "z");
}

void CDrawUtilities::DrawGrid()
{
    VERIFY(Device.b_is_Ready);
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    // fill VB
    FVF::L* pv = (FVF::L*)Stream->Lock(m_GridPoints.size(), vs_L->vb_stride, vBase);
    for (auto v_it = m_GridPoints.begin(); v_it != m_GridPoints.end(); v_it++, pv++)
        pv->set(*v_it);
    Stream->Unlock(m_GridPoints.size(), vs_L->vb_stride);
    // Render it as triangle list
    Fmatrix ddd;
    ddd.identity();
    RCache.set_xform_world(ddd);
    DU_DRAW_SH(RImplementation.m_WireShader);
    DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, m_GridPoints.size() / 2);
}

void CDrawUtilities::DrawSelectionRect(const Ivector2& m_SelStart, const Ivector2& m_SelEnd)
{
    VERIFY(Device.b_is_Ready);
    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    FVF::L* pv = (FVF::L*)Stream->Lock(6, vs_L->vb_stride, vBase);
    //     pv->set(m_SelStart.x * SCREEN_QUALITY, m_SelStart.y * SCREEN_QUALITY, 0, m_SelectionRect);
    //     pv++;
    //     pv->set(m_SelStart.x * SCREEN_QUALITY, m_SelEnd.y * SCREEN_QUALITY, 0, m_SelectionRect);
    //     pv++;
    //     pv->set(m_SelEnd.x * SCREEN_QUALITY, m_SelEnd.y * SCREEN_QUALITY, 0, m_SelectionRect);
    //     pv++;
    //     pv->set(m_SelEnd.x * SCREEN_QUALITY, m_SelStart.y * SCREEN_QUALITY, 0, m_SelectionRect);
    //     pv++;
    pv->set(m_SelStart.x, m_SelEnd.y, 0.001f, m_SelectionRect); // v0
    pv++;
    pv->set(m_SelStart.x, m_SelStart.y, 0.001f, m_SelectionRect); // v1
    pv++;
    pv->set(m_SelEnd.x, m_SelEnd.y, 0.001f, m_SelectionRect); // v2
    pv++;
    pv->set(m_SelStart.x, m_SelStart.y, 0.001f, m_SelectionRect); // v1
    pv++;
    pv->set(m_SelEnd.x, m_SelStart.y, 0.001f, m_SelectionRect); // v3
    pv++;
    pv->set(m_SelEnd.x, m_SelEnd.y, 0.001f, m_SelectionRect); // v2
    pv++;
    Stream->Unlock(6, vs_L->vb_stride);
    // Render it as triangle list
    Fmatrix world = Fidentity;
    world.build_projection_orthoOffCentrLH(
        Cordis::SDK::SDKUI::UI().GetDisplayX(), Cordis::SDK::SDKUI::UI().GetDisplayY(), 0, 1);
    //  world.transpose();
    RCache.set_xform_project(Fidentity);
    RCache.set_xform_view(Fidentity);
    RCache.set_xform_world(world);
    RCache.set_Shader(RImplementation.m_Shader2D);
    DU_DRAW_DP(D3DPT_TRIANGLELIST, vs_L, vBase, 2);
}

void CDrawUtilities::DrawPrimitiveL(
    D3DPRIMITIVETYPE pt, u32 pc, Fvector* vertices, int vc, u32 color, BOOL bCull, BOOL bCycle)
{
    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase, dwNeed = (bCycle) ? vc + 1 : vc;
    FVF::L* pv = (FVF::L*)Stream->Lock(dwNeed, vs_L->vb_stride, vBase);
    for (int k = 0; k < vc; k++, pv++)
        pv->set(vertices[k], color);
    if (bCycle)
        pv->set(*(pv - vc));
    Stream->Unlock(dwNeed, vs_L->vb_stride);

    if (!bCull)
        DU_DRAW_RS(D3DRS_CULLMODE, D3DCULL_NONE);
    DU_DRAW_DP(pt, vs_L, vBase, pc);
    if (!bCull)
        DU_DRAW_RS(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDrawUtilities::DrawPrimitiveTL(D3DPRIMITIVETYPE pt, u32 pc, FVF::TL* vertices, int vc, BOOL bCull, BOOL bCycle)
{
    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase, dwNeed = (bCycle) ? vc + 1 : vc;
    FVF::TL* pv = (FVF::TL*)Stream->Lock(dwNeed, vs_TL->vb_stride, vBase);
    for (int k = 0; k < vc; k++, pv++)
        pv->set(vertices[k]);
    if (bCycle)
        pv->set(*(pv - vc));
    Stream->Unlock(dwNeed, vs_TL->vb_stride);

    if (!bCull)
        DU_DRAW_RS(D3DRS_CULLMODE, D3DCULL_NONE);
    DU_DRAW_DP(pt, vs_TL, vBase, pc);
    if (!bCull)
        DU_DRAW_RS(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDrawUtilities::DrawPrimitiveLIT(D3DPRIMITIVETYPE pt, u32 pc, FVF::LIT* vertices, int vc, BOOL bCull, BOOL bCycle)
{
    // fill VB
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase, dwNeed = (bCycle) ? vc + 1 : vc;
    FVF::LIT* pv = (FVF::LIT*)Stream->Lock(dwNeed, vs_LIT->vb_stride, vBase);
    for (int k = 0; k < vc; k++, pv++)
        pv->set(vertices[k]);
    if (bCycle)
        pv->set(*(pv - vc));
    Stream->Unlock(dwNeed, vs_LIT->vb_stride);

    if (!bCull)
        DU_DRAW_RS(D3DRS_CULLMODE, D3DCULL_NONE);
    DU_DRAW_DP(pt, vs_LIT, vBase, pc);
    if (!bCull)
        DU_DRAW_RS(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDrawUtilities::DrawLink(const Fvector& p0, const Fvector& p1, float sz, u32 clr)
{
    DrawLine(p1, p0, clr);
    Fvector pp[2], D, R, N = {0, 1, 0};
    D.sub(p1, p0);
    D.normalize();
    R.crossproduct(N, D);
    R.mul(0.5f);
    D.mul(2.0f);
    N.mul(0.5f);
    // LR
    pp[0].add(R, D);
    pp[0].mul(sz * -0.5f);
    pp[0].add(p1);
    R.invert();
    pp[1].add(R, D);
    pp[1].mul(sz * -0.5f);
    pp[1].add(p1);
    DrawLine(p1, pp[0], clr);
    DrawLine(p1, pp[1], clr);
    // UB
    pp[0].add(N, D);
    pp[0].mul(sz * -0.5f);
    pp[0].add(p1);
    N.invert();
    pp[1].add(N, D);
    pp[1].mul(sz * -0.5f);
    pp[1].add(p1);
    DrawLine(p1, pp[0], clr);
    DrawLine(p1, pp[1], clr);
}

void CDrawUtilities::DrawJoint(const Fvector& p, float radius, u32 clr) { DrawLineSphere(p, radius, clr, false); }
void CDrawUtilities::OnRender() { m_Font->OnRender(); }

void CDrawUtilities::DrawGizmoLine(const Fvector& start, const Fvector& end, u32 clr)
{
    R_ASSERT(Device.b_is_Ready);
    _VertexStream* Stream = &RCache.Vertex;
    u32 vBase;
    // fill VB
    FVF::L* pv = (FVF::L*)Stream->Lock(2, vs_L->vb_stride, vBase);
    pv->set(start, clr);
    pv++;
    pv->set(end, clr);
    pv++;
    Stream->Unlock(2, vs_L->vb_stride);
    // Render it as triangle list
    Fmatrix ddd;
    ddd.identity();
    RCache.set_xform_world(ddd);
    DU_DRAW_SH(RImplementation.m_GizmoShader);
    DU_DRAW_DP(D3DPT_LINELIST, vs_L, vBase, 1);
}

void CDrawUtilities::DrawGizmoPlane(const Fvector* points, u32 clr_left, u32 clr_right)
{
    /*    static const WORD Indecies[] = {0, 1, 2, 0, 2, 3};*/
    u32 vBase /*, iBase*/;

    int k;
    FVF::L* pv;
    WORD* i;
    _VertexStream* Stream = &RCache.Vertex;
    /*    _IndexStream* StreamI = &RCache.Index;*/

    // fill VB
    pv = (FVF::L*)Stream->Lock(5, vs_L->vb_stride, vBase);
    pv->set(points[0].x, points[0].y, points[0].z, clr_left);
    pv++;
    pv->set(points[1].x, points[1].y, points[1].z, clr_left);
    pv++;
    pv->set(points[2].x, points[2].y, points[2].z, clr_right);
    pv++;
    pv->set(points[3].x, points[3].y, points[3].z, clr_right);
    pv++;
    pv->set(points[0].x, points[0].y, points[0].z, clr_left);
    pv++;
    Stream->Unlock(5, vs_L->vb_stride);

    /*
        i = StreamI->Lock(6, iBase);
        for (k = 0; k < 6; k++, i++)
            *i = Indecies[k];
        StreamI->Unlock(6);*/

    DU_DRAW_SH(RImplementation.m_GizmoShader);
    DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, vBase, 4);
    //  DU_DRAW_DIP(D3DPT_LINELIST, vs_L, vBase, 0, 6, iBase, 2);
}

void CDrawUtilities::DrawGizmoMove(const Fvector& pos)
{
    int size_currentgizmoline = g_size_gizmoline;

    unsigned int size_gizmoplane = size_currentgizmoline / 3;
    for (unsigned int i = 0; i < 3; ++i)
    {
        for (unsigned int j = 0; j < 4; ++j)
        {
            Cordis::SDK::GizmoMovePlanes[i].m_points[j] = pos;
        }
    }

    // @ X
    {
        Cordis::SDK::GizmoMove[0].m_origin_point = pos;
        Cordis::SDK::GizmoMove[0].m_origin_point.x += size_gizmoplane;
        Cordis::SDK::GizmoMove[0].m_end_point = pos;
        Cordis::SDK::GizmoMove[0].m_end_point.x += size_currentgizmoline;
        DrawGizmoLine(Cordis::SDK::GizmoMove[0].m_origin_point, Cordis::SDK::GizmoMove[0].m_end_point,
            Cordis::SDK::GizmoMove[0].m_color_value);
        this->OutText(Cordis::SDK::GizmoMove[0].m_end_point, "X", D3DCOLOR_ARGB(255, 255, 255, 255));

        // @ Arrows
        Fvector new_pos = Cordis::SDK::GizmoMove[0].m_end_point;
        new_pos.x -= 1;

        new_pos.z -= 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[0].m_end_point, new_pos, Cordis::SDK::GizmoMove[0].m_color_value);

        new_pos.z += 0.95;
        new_pos.z += 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[0].m_end_point, new_pos, Cordis::SDK::GizmoMove[0].m_color_value);

        new_pos.z -= 0.95;
        new_pos.y += 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[0].m_end_point, new_pos, Cordis::SDK::GizmoMove[0].m_color_value);

        new_pos.y -= 0.95;
        new_pos.y -= 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[0].m_end_point, new_pos, Cordis::SDK::GizmoMove[0].m_color_value);
    }

    // @ Y
    {
        Cordis::SDK::GizmoMove[1].m_origin_point = pos;
        Cordis::SDK::GizmoMove[1].m_origin_point.y += size_gizmoplane;
        Cordis::SDK::GizmoMove[1].m_end_point = pos;
        Cordis::SDK::GizmoMove[1].m_end_point.y += size_currentgizmoline;

        DrawGizmoLine(Cordis::SDK::GizmoMove[1].m_origin_point, Cordis::SDK::GizmoMove[1].m_end_point,
            Cordis::SDK::GizmoMove[1].m_color_value);
        this->OutText(Cordis::SDK::GizmoMove[1].m_end_point, "Y", D3DCOLOR_ARGB(255, 255, 255, 255));
        // @ Arrows
        Fvector new_pos = Cordis::SDK::GizmoMove[1].m_end_point;
        new_pos.y -= 1;

        new_pos.z -= 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[1].m_end_point, new_pos, Cordis::SDK::GizmoMove[1].m_color_value);

        new_pos.z += 0.95;
        new_pos.z += 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[1].m_end_point, new_pos, Cordis::SDK::GizmoMove[1].m_color_value);

        new_pos.z -= 0.95;
        new_pos.x += 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[1].m_end_point, new_pos, Cordis::SDK::GizmoMove[1].m_color_value);

        new_pos.x -= 0.95;
        new_pos.x -= 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[1].m_end_point, new_pos, Cordis::SDK::GizmoMove[1].m_color_value);
    }

    // @ Z
    {
        Cordis::SDK::GizmoMove[2].m_origin_point = pos;
        Cordis::SDK::GizmoMove[2].m_origin_point.z -= size_gizmoplane;
        Cordis::SDK::GizmoMove[2].m_end_point = pos;
        Cordis::SDK::GizmoMove[2].m_end_point.z -= size_currentgizmoline;

        DrawGizmoLine(Cordis::SDK::GizmoMove[2].m_origin_point, Cordis::SDK::GizmoMove[2].m_end_point,
            Cordis::SDK::GizmoMove[2].m_color_value);
        this->OutText(Cordis::SDK::GizmoMove[2].m_end_point, "Z", D3DCOLOR_ARGB(255, 255, 255, 255));
        // @ Arrows
        Fvector new_pos = Cordis::SDK::GizmoMove[2].m_end_point;
        new_pos.z += 1;

        new_pos.x -= 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[2].m_end_point, new_pos, Cordis::SDK::GizmoMove[2].m_color_value);

        new_pos.x += 0.95;
        new_pos.x += 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[2].m_end_point, new_pos, Cordis::SDK::GizmoMove[2].m_color_value);

        new_pos.x -= 0.95;
        new_pos.y += 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[2].m_end_point, new_pos, Cordis::SDK::GizmoMove[2].m_color_value);

        new_pos.y -= 0.95;
        new_pos.y -= 0.95;
        DrawGizmoLine(Cordis::SDK::GizmoMove[2].m_end_point, new_pos, Cordis::SDK::GizmoMove[2].m_color_value);
    }

    // @ XY
    {
        Cordis::SDK::GizmoMovePlanes[0].m_points[1].y += size_gizmoplane;
        Cordis::SDK::GizmoMovePlanes[0].m_points[2].y += size_gizmoplane;
        Cordis::SDK::GizmoMovePlanes[0].m_points[2].x += size_gizmoplane;
        Cordis::SDK::GizmoMovePlanes[0].m_points[3].x += size_gizmoplane;

        this->DrawGizmoPlane(Cordis::SDK::GizmoMovePlanes[0].m_points,
            Cordis::SDK::GizmoMovePlanes[0].m_color_left_value, Cordis::SDK::GizmoMovePlanes[0].m_color_right_value);
    }

    // @ ZY
    {
        Cordis::SDK::GizmoMovePlanes[1].m_points[1].y += size_gizmoplane;
        Cordis::SDK::GizmoMovePlanes[1].m_points[2].y += size_gizmoplane;
        Cordis::SDK::GizmoMovePlanes[1].m_points[2].z -= size_gizmoplane;
        Cordis::SDK::GizmoMovePlanes[1].m_points[3].z -= size_gizmoplane;

        this->DrawGizmoPlane(Cordis::SDK::GizmoMovePlanes[1].m_points,
            Cordis::SDK::GizmoMovePlanes[1].m_color_left_value, Cordis::SDK::GizmoMovePlanes[1].m_color_right_value);
    }

    // @ XZ
    {
        Cordis::SDK::GizmoMovePlanes[2].m_points[1].x += size_gizmoplane;
        Cordis::SDK::GizmoMovePlanes[2].m_points[2].x += size_gizmoplane;
        Cordis::SDK::GizmoMovePlanes[2].m_points[2].z -= size_gizmoplane;
        Cordis::SDK::GizmoMovePlanes[2].m_points[3].z -= size_gizmoplane;

        this->DrawGizmoPlane(Cordis::SDK::GizmoMovePlanes[2].m_points,
            Cordis::SDK::GizmoMovePlanes[2].m_color_left_value, Cordis::SDK::GizmoMovePlanes[2].m_color_right_value);
    }
}

void CDrawUtilities::DrawGizmoScale(const Fvector& position_object)
{
    float size_gizmoplane = g_size_gizmoline / 5;
    for (unsigned int i = 0; i < 9; ++i)
    {
        for (unsigned int j = 0; j < 4; ++j)
        {
            Cordis::SDK::g_gizmo_planes_scale[i].m_points[j] = position_object;
        }
    }

#pragma region Lines
    // @ X
    {
        Cordis::SDK::SDK_GizmoScale& line_x = Cordis::SDK::g_gizmo_lines_scale[0];
        line_x.m_origin_point = position_object;
        line_x.m_end_point = position_object;
        line_x.m_origin_point.x += size_gizmoplane;
        line_x.m_end_point.x += g_size_gizmoline;
        line_x.m_origin_point.z -= size_gizmoplane / 2;
        line_x.m_origin_point.y += size_gizmoplane / 2;
        line_x.m_end_point.y += size_gizmoplane / 2;
        line_x.m_end_point.z -= size_gizmoplane / 2;

        this->DrawGizmoLine(line_x.m_origin_point, line_x.m_end_point, line_x.m_color);
        this->OutText(line_x.m_end_point, "X", D3DCOLOR_RGBA(255, 255, 255, 255));
    }

    // @ Y
    {
        Cordis::SDK::SDK_GizmoScale& line_y = Cordis::SDK::g_gizmo_lines_scale[1];
        line_y.m_origin_point = position_object;
        line_y.m_end_point = position_object;
        line_y.m_origin_point.y += size_gizmoplane;
        line_y.m_origin_point.x += size_gizmoplane / 2;
        line_y.m_origin_point.z -= size_gizmoplane / 2;
        line_y.m_end_point.y += g_size_gizmoline;
        line_y.m_end_point.x += size_gizmoplane / 2;
        line_y.m_end_point.z -= size_gizmoplane / 2;
        this->DrawGizmoLine(line_y.m_origin_point, line_y.m_end_point, line_y.m_color);
        this->OutText(line_y.m_end_point, "Y", D3DCOLOR_RGBA(255,255,255,255));
    }

    // @ Z
    {
        Cordis::SDK::SDK_GizmoScale& line_z = Cordis::SDK::g_gizmo_lines_scale[2];
        line_z.m_origin_point = position_object;
        line_z.m_end_point = position_object;
        line_z.m_origin_point.z -= size_gizmoplane;
        line_z.m_end_point.z -= g_size_gizmoline;
        line_z.m_origin_point.x += size_gizmoplane / 2;
        line_z.m_origin_point.y += size_gizmoplane / 2;
        line_z.m_end_point.y += size_gizmoplane / 2;
        line_z.m_end_point.x += size_gizmoplane / 2;
        this->DrawGizmoLine(line_z.m_origin_point, line_z.m_end_point, line_z.m_color);
        this->OutText(line_z.m_end_point, "Z", D3DCOLOR_RGBA(255, 255, 255, 255));
    }
#pragma endregion

#pragma region AxisPlanes
    {
        Cordis::SDK::SDK_GizmoScalePlane& plane_x = Cordis::SDK::g_gizmo_planes_scale[0];
        plane_x.m_points[0].x += size_gizmoplane + (size_gizmoplane);
        plane_x.m_points[0].y += size_gizmoplane + (size_gizmoplane);
        plane_x.m_points[1].x += size_gizmoplane + (size_gizmoplane);
        plane_x.m_points[1].y += size_gizmoplane + size_gizmoplane + (size_gizmoplane);
        plane_x.m_points[2] = plane_x.m_points[1];
        plane_x.m_points[2].x += size_gizmoplane;
        plane_x.m_points[3] = plane_x.m_points[0];
        plane_x.m_points[3].x += size_gizmoplane;

        _VertexStream* stream_vertex = &RCache.Vertex;
        unsigned int base_vertex;

        FVF::L* buffer_vertex = (FVF::L*)stream_vertex->Lock(5, vs_L->vb_stride, base_vertex);

        for (unsigned int i = 0; i < 4; ++i, ++buffer_vertex)
            buffer_vertex->set(plane_x.m_points[i], plane_x.m_color);

        buffer_vertex->set(plane_x.m_points[0], plane_x.m_color);
        buffer_vertex++;
        stream_vertex->Unlock(5, vs_L->vb_stride);

        RCache.set_Shader(RImplementation.m_GizmoShader);
        DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, base_vertex, 4);
    }

    {
        Cordis::SDK::SDK_GizmoScalePlane& plane_y = Cordis::SDK::g_gizmo_planes_scale[2];
        plane_y.m_points[0].z -= size_gizmoplane + (size_gizmoplane);
        plane_y.m_points[0].y += size_gizmoplane + (size_gizmoplane);
        plane_y.m_points[1].z -= size_gizmoplane + (size_gizmoplane);
        plane_y.m_points[1].y += size_gizmoplane + size_gizmoplane + (size_gizmoplane);
        plane_y.m_points[2] = plane_y.m_points[1];
        plane_y.m_points[2].z -= size_gizmoplane;
        plane_y.m_points[3] = plane_y.m_points[0];
        plane_y.m_points[3].z -= size_gizmoplane;

        _VertexStream* stream_vertex = &RCache.Vertex;
        unsigned int base_vertex;

        FVF::L* buffer_vertex = (FVF::L*)stream_vertex->Lock(5, vs_L->vb_stride, base_vertex);

        for (unsigned int i = 0; i < 4; ++i, ++buffer_vertex)
            buffer_vertex->set(plane_y.m_points[i], plane_y.m_color);

        buffer_vertex->set(plane_y.m_points[0], plane_y.m_color);
        buffer_vertex++;
        stream_vertex->Unlock(5, vs_L->vb_stride);

        RCache.set_Shader(RImplementation.m_GizmoShader);
        DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, base_vertex, 4);
    }

    {
        Cordis::SDK::SDK_GizmoScalePlane& plane_z = Cordis::SDK::g_gizmo_planes_scale[1];
        plane_z.m_points[0].z -= size_gizmoplane + (size_gizmoplane);
        plane_z.m_points[0].x += size_gizmoplane + (size_gizmoplane);
        plane_z.m_points[1].z -= size_gizmoplane + (size_gizmoplane);
        plane_z.m_points[1].x += size_gizmoplane + size_gizmoplane + (size_gizmoplane);
        plane_z.m_points[2] = plane_z.m_points[1];
        plane_z.m_points[2].z -= size_gizmoplane;
        plane_z.m_points[3] = plane_z.m_points[0];
        plane_z.m_points[3].z -= size_gizmoplane;

        _VertexStream* stream_vertex = &RCache.Vertex;
        unsigned int base_vertex;

        FVF::L* buffer_vertex = (FVF::L*)stream_vertex->Lock(5, vs_L->vb_stride, base_vertex);

        for (unsigned int i = 0; i < 4; ++i, ++buffer_vertex)
            buffer_vertex->set(plane_z.m_points[i], plane_z.m_color);

        buffer_vertex->set(plane_z.m_points[0], plane_z.m_color);
        buffer_vertex++;
        stream_vertex->Unlock(5, vs_L->vb_stride);

        RCache.set_Shader(RImplementation.m_GizmoShader);
        DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, base_vertex, 4);
    }
#pragma endregion

#pragma region BOX
    {
        Cordis::SDK::SDK_GizmoScalePlane& first_plane = Cordis::SDK::g_gizmo_planes_scale[3];
        first_plane.m_points[0].x += size_gizmoplane;
        first_plane.m_points[1].x += size_gizmoplane;
        first_plane.m_points[1].y += size_gizmoplane;
        first_plane.m_points[2].x += size_gizmoplane;
        first_plane.m_points[2].y += size_gizmoplane;
        first_plane.m_points[2].z -= size_gizmoplane;
        first_plane.m_points[3].x += size_gizmoplane;
        first_plane.m_points[3].z -= size_gizmoplane;

        Cordis::SDK::SDK_GizmoScalePlane& second_plane = Cordis::SDK::g_gizmo_planes_scale[4];
        second_plane.m_points[0].x += size_gizmoplane;
        second_plane.m_points[0].z -= size_gizmoplane;
        second_plane.m_points[1].z -= size_gizmoplane;
        second_plane.m_points[2].z -= size_gizmoplane;
        second_plane.m_points[2].y += size_gizmoplane;
        second_plane.m_points[3].x += size_gizmoplane;
        second_plane.m_points[3].y += size_gizmoplane;
        second_plane.m_points[3].z -= size_gizmoplane;

        Cordis::SDK::SDK_GizmoScalePlane& third_plane = Cordis::SDK::g_gizmo_planes_scale[5];
        third_plane.m_points[1].z -= size_gizmoplane;
        third_plane.m_points[2].z -= size_gizmoplane;
        third_plane.m_points[2].y += size_gizmoplane;
        third_plane.m_points[3].y += size_gizmoplane;

        Cordis::SDK::SDK_GizmoScalePlane& fourth_plane = Cordis::SDK::g_gizmo_planes_scale[6];
        fourth_plane.m_points[1].x += size_gizmoplane;
        fourth_plane.m_points[2].x += size_gizmoplane;
        fourth_plane.m_points[2].y += size_gizmoplane;
        fourth_plane.m_points[3].y += size_gizmoplane;

        Cordis::SDK::SDK_GizmoScalePlane& fifth_plane = Cordis::SDK::g_gizmo_planes_scale[7];
        fifth_plane.m_points[1].x += size_gizmoplane;
        fifth_plane.m_points[2].x += size_gizmoplane;
        fifth_plane.m_points[2].z -= size_gizmoplane;
        fifth_plane.m_points[3].z -= size_gizmoplane;

        Cordis::SDK::SDK_GizmoScalePlane& sixth_plane = Cordis::SDK::g_gizmo_planes_scale[8];
        sixth_plane.m_points[0].y += size_gizmoplane;
        sixth_plane.m_points[1].x += size_gizmoplane;
        sixth_plane.m_points[1].y += size_gizmoplane;
        sixth_plane.m_points[2].x += size_gizmoplane;
        sixth_plane.m_points[2].y += size_gizmoplane;
        sixth_plane.m_points[2].z -= size_gizmoplane;
        sixth_plane.m_points[3].z -= size_gizmoplane;
        sixth_plane.m_points[3].y += size_gizmoplane;

        _VertexStream* stream_vertex = &RCache.Vertex;
        unsigned int vertex_base;
        FVF::L* buffer_vertex = (FVF::L*)stream_vertex->Lock(30, vs_L->vb_stride, vertex_base);

        for (unsigned int i = 0; i < 4; ++i, ++buffer_vertex)
            buffer_vertex->set(first_plane.m_points[i], first_plane.m_color);
        

        buffer_vertex->set(first_plane.m_points[0], first_plane.m_color);
        buffer_vertex++;
// //         buffer_vertex->set(first_plane.m_points[2], first_plane.m_color);
// //         buffer_vertex++;
// //         buffer_vertex->set(first_plane.m_points[3], first_plane.m_color);
// //         buffer_vertex++;
// 
        for (unsigned int i = 0; i < 4; ++i, ++buffer_vertex)
            buffer_vertex->set(second_plane.m_points[i], first_plane.m_color);

        buffer_vertex->set(second_plane.m_points[0], first_plane.m_color);
        buffer_vertex++;
// //         buffer_vertex->set(second_plane.m_points[2], first_plane.m_color);
// //         buffer_vertex++;
// //         buffer_vertex->set(second_plane.m_points[3], first_plane.m_color);
// //         buffer_vertex++;
// 
        for (unsigned int i = 0; i < 4; ++i, ++buffer_vertex)
            buffer_vertex->set(third_plane.m_points[i], first_plane.m_color);

        buffer_vertex->set(third_plane.m_points[0], first_plane.m_color);
        buffer_vertex++;
// //         buffer_vertex->set(third_plane.m_points[2], first_plane.m_color);
// //         buffer_vertex++;
// //         buffer_vertex->set(third_plane.m_points[3], first_plane.m_color);
// //         buffer_vertex++;
// 
        for (unsigned int i = 0; i < 4; ++i, ++buffer_vertex)
            buffer_vertex->set(fourth_plane.m_points[i], first_plane.m_color);

        buffer_vertex->set(fourth_plane.m_points[0], first_plane.m_color);
        buffer_vertex++;
// //         buffer_vertex->set(fourth_plane.m_points[2], first_plane.m_color);
// //         buffer_vertex++;
// //         buffer_vertex->set(fourth_plane.m_points[3], first_plane.m_color);
// //         buffer_vertex++;
// 
         for (unsigned int i = 0; i < 4; ++i, ++buffer_vertex)
             buffer_vertex->set(fifth_plane.m_points[i], first_plane.m_color);
 
         buffer_vertex->set(fifth_plane.m_points[0], first_plane.m_color);
         buffer_vertex++;
//         buffer_vertex->set(fifth_plane.m_points[2], first_plane.m_color);
//         buffer_vertex++;
//         buffer_vertex->set(fifth_plane.m_points[3], first_plane.m_color);
//         buffer_vertex++;

        for (unsigned int i = 0; i < 4; ++i, ++buffer_vertex)
            buffer_vertex->set(sixth_plane.m_points[i], first_plane.m_color);

         buffer_vertex->set(sixth_plane.m_points[0], first_plane.m_color);
         buffer_vertex++;
//         buffer_vertex->set(sixth_plane.m_points[2], first_plane.m_color);
//         buffer_vertex++;
//         buffer_vertex->set(sixth_plane.m_points[3], first_plane.m_color);
//         buffer_vertex++;

        stream_vertex->Unlock(30, vs_L->vb_stride);

        //  RCache.set_xform_world(Fidentity);
        RCache.set_Shader(RImplementation.m_GizmoShader);

        DU_DRAW_DP(D3DPT_LINESTRIP, vs_L, vertex_base, 29);
    }
#pragma endregion
}

void CDrawUtilities::OutText(const Fvector& pos, LPCSTR text, u32 color, u32 shadow_color)
{
    Fvector p;
    float w = pos.x * Device.mFullTransform._14 + pos.y * Device.mFullTransform._24 +
        pos.z * Device.mFullTransform._34 + Device.mFullTransform._44;

    if (w >= 0)
    {
        Device.mFullTransform.transform(p, pos);
        p.x = (float)iFloor(_x2real(p.x));
        p.y = (float)iFloor(_y2real(-p.y));

        m_Font->SetColor(shadow_color);
        m_Font->Out(p.x, p.y, (LPSTR)text);
        m_Font->SetColor(color);
        m_Font->Out(p.x, p.y, (LPSTR)text);
    }
}
