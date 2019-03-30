#include "stdafx.h"
#include "SDKUI_Render.h"
#include "SDKUI.h"
SDKUI_Render::~SDKUI_Render() {}
 
void SDKUI_Render::Initialize(void)
{
    // Lord: наверное сюда же нужно поместить контант буфферы компиляцию шейдеров и прочее, хотя наверное только онли
    // рантайм загрузка Lord: БЛЯТЬ ДЖОННИ ЭТО ПИЗДЕЦ КАК ЖЕ ПЫС ЗАЕБАЛИ ХАРДКОДИТЬ
    GridOptions::Size = 100;
    GridOptions::separator = 10;
 
}

void SDKUI_Render::MainRender(void)
{
    RImplementation.SetFullOM();

    /*
        Fmatrix wv;
    wv.mul_43(Device.mView, Fidentity);
    Fmatrix vp;
    vp.mul(Device.mProject, Device.mView);
    Fmatrix mwvp;
    mwvp.mul(Device.mProject, wv);
      RCache.set_c("m_W", Fidentity);
    RCache.set_c("m_V", Device.mView);
    RCache.set_c("m_P", Device.mProject);
    RCache.set_c("m_WV", wv);
    RCache.set_c("m_VP", vp);
    */

    //  RCache.set_c("m_WVP", mwvp);

    SDKUI::UI().Begin();
    SDKUI::UI().Draw();

    RImplementation.ClearTarget();
    HW.pContext->ClearDepthStencilView(RCache.get_ZB(), D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0);



    RImplementation.obj->Render(Fidentity, 1, false);
    u32 clr = D3DCOLOR_ARGB(255, 255, 255, 255);

    if (SDKUI::UI().IsSelected())
    {

        DUImpl.DrawSelectionBoxB(RImplementation.obj->GetBox(), &clr);
    }
 
#pragma region TheGrid
    Fvector pos_pivot;
    pos_pivot.set(0, 1, 0);
    DUImpl.DrawPivot(pos_pivot, GridOptions::Size/2);

    DUImpl.UpdateGrid(GridOptions::Size, GridOptions::separator);
    DUImpl.DrawGrid();
#pragma endregion

    SDKUI::UI().End();
}
