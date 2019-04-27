/*
    [Author]: Lord
    [Date]: 16.02.2019
*/

#pragma once

#include "SDKUI_Render.h"
#include "SDKUI_Helpers.h"
#include "../../xrEngine/MonitorManager.hpp"
#include "SDKUI_RightWindow.h"

class SDKUI
{
private:
    SDKUI(void)
    {
        this->DisplayX = g_monitors.GetMaximalResolution().first;
        this->DisplayY = g_monitors.GetMaximalResolution().second;
        this->bShowMainMenuBar = true;
        // @ Init All stuff
        SDKUI_Log::Widget().Init(GetPosBottomLeft(), 500, 150, ImGuiWindowFlags_NoResize);
        SDKUI_Log::Widget().SetColor(good);
        SDKUI_Log::Widget().AddText("SDK was initialized! %s", "Test"); // Lord: изменить здесь потом текст
 
        // Lord: удалить потом реализовать в EThumbnail
        //         D3D11_TEXTURE2D_DESC desc = {0};
        //         desc.Width = 128;
        //         desc.Height = 128;
        //         desc.MipLevels = desc.ArraySize = 1;
        //         desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        //         desc.SampleDesc.Count = 1;
        //         desc.Usage = D3D11_USAGE_DYNAMIC;
        //         desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        //         desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        //         desc.MiscFlags = 0;
        //         D3D11_SUBRESOURCE_DATA a = {0};
        //         a.pSysMem = RImplementation.image_test.Pixels();
        //         a.SysMemPitch = 128 * 4;
        //         a.SysMemSlicePitch = 0;
        //
        //         HRESULT hr = HW.pDevice->CreateTexture2D(&desc, &a, &image_tex);
        //         if (FAILED(hr))
        //         {
        //             ASSERT(TEXT("aaaaaa"));
        //         }
        //         D3D11_SHADER_RESOURCE_VIEW_DESC b;
        //         b.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        //         b.Texture2D.MipLevels = 1;
        //         b.Texture2D.MostDetailedMip = 0;
        //         b.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        //         hr = HW.pDevice->CreateShaderResourceView(image_tex, &b, &image_test_);
    }

public:
    inline static SDKUI& UI(void)
    {
        static SDKUI instance;
        return instance;
    }

    ~SDKUI(void) = default;

    void Begin(void);
    void Draw(void);
    void End(void);

    inline void ShowMainMenuBar(void) { this->bShowMainMenuBar = true; }

    inline void HideMainMenuBar(void) { this->bShowMainMenuBar = false; }

    inline bool IsSelected(void) const { return this->bSelected; }

    inline int GetDisplayX(void) const { return this->DisplayX; }
    inline int GetDisplayY(void) const { return this->DisplayY; }

    bool PickGround(Fvector& hitpoint, const Fvector& start, const Fvector& dir, /*int bSnap,*/ Fvector* hitnormal);

private:
    void DrawMainMenuBar(void);
    void DrawAllHelpers(void);
    void KeyBoardMessages(void);

    inline int GetCenteredX(void) { return DisplayX / 2; }
    inline int GetCenteredY(void) { return DisplayY / 2; }
    inline ImVec2 GetPosCentered(void) { return ImVec2(GetCenteredX(), GetCenteredY()); }
    inline ImVec2 GetPosTopLeft(void) { return ImVec2(0, 0); }
    inline ImVec2 GetPosTopRight(void) { return ImVec2(DisplayX, 0); }
    inline ImVec2 GetPosBottomLeft(void) { return ImVec2(0, DisplayY); }
    inline ImVec2 GetPosBottomRight(void) { return ImVec2(DisplayX, DisplayY); }

    void PickObject(void); // Lord: наверное лучше это переместить в класс сцены и здесь это удалить

private:
    bool bShowMainMenuBar;
    bool bCloseOnce = false;
    bool bSelected = false;
    bool bCanUseLeftButton = true;
    bool bClickedRightButton = false;
    bool bAddTool = false; // Lord: подумать стоит ли это здесь так оставлять?
    int DisplayX;
    int DisplayY;
    float dis_to_current_obj = SDK_Camera::GetInstance().fFar;
    Fvector mPos;
    Fvector mDir;
};
