#include "stdafx.h"
#include "SDKUI_Render.h"
#include "SDKUI.h"
SDKUI_Render::~SDKUI_Render() {}

void SDKUI_Render::Initialize(void)
{
    // Lord: наверное сюда же нужно поместить контант буфферы компиляцию шейдеров и прочее, хотя наверное только онли рантайм загрузка
    // Lord: БЛЯТЬ ДЖОННИ ЭТО ПИЗДЕЦ КАК ЖЕ ПЫС ЗАЕБАЛИ ХАРДКОДИТЬ
}

void SDKUI_Render::MainRender(void)
{
    RImplementation.SDK_Init(); // Lord: переименовать данный метод 

    SDKUI::UI().Begin();
    SDKUI::UI().Draw();

    RImplementation.ClearTarget();
    HW.pContext->ClearDepthStencilView(RCache.get_ZB(), D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0);
    DUImpl.UpdateGrid(1000, 10);
    DUImpl.DrawGrid();


    SDKUI::UI().End();


}
