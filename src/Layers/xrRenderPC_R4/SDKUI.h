/*
    [Author]: Lord
    [Date]: 16.02.2019
*/
#pragma once

#include "SDKUI_Render.h"
#include "SDKUI_Helpers.h"
#include "../../xrEngine/MonitorManager.hpp"
#include "SDKUI_RightWindow.h"
#include "SDK_FileSystemState.h"
#include "SDK_IconManager.h"

namespace Cordis
{
    namespace SDK
    {
    class SDK_SceneManager;

    class SDKUI
    {
        friend class SDKUI_Render;
    private:
        SDKUI(void)
            : bShowMainMenuBar(true), bCloseOnce(false), bSelected(false), bCanUseLeftButton(true),
              DisplayX(g_monitors.GetMaximalResolution().first), DisplayY(g_monitors.GetMaximalResolution().second),
              bClickedRightButton(false), bAddTool(false), bRotateTool(false), bScaleTool(false), bMoveTool(false),
              b_is_used_selectionrectengular(false), m_start_mouseposition(0, 0), m_current_mouseposition(0, 0),
              b_is_hovered_coloredit(false), b_is_dragged_selection_rectangle(false),
              b_is_left_mouse_clicked_still_using(false), mouse_drag_delta(0.0f)
        {
            FS.CheckSDKMainFolder();
			SDK_FileSystemState::MakeInfoAboutRawDataFolder();
			SDK_Names::getInstance().Initialize();
			SDK_Cache::GetInstance().MakeCache();
            // @ Init All stuff
            SDKUI_Log::Widget().Init(this->getPosBottomLeft(), this->getDisplayX() / 3.84f, this->getDisplayY() / 7.2f, ImGuiWindowFlags_NoResize);
            SDKUI_Log::Widget().SetColor(good);
            SDKUI_Log::Widget().AddText("SDK was initialized!"); // Lord: изменить здесь потом текст

			this->m_handle_xrgame = LoadLibrary("xrGame.dll");
			CHECK_OR_EXIT(this->m_handle_xrgame, "Can't LoadLibrary -> xrGame.dll");

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
        inline static SDKUI& UI(void) noexcept
        {
            static SDKUI instance;
            return instance;
        }

		~SDKUI(void)
		{
			FreeLibrary(this->m_handle_xrgame);
		}

        inline void ShowMainMenuBar(void) { this->bShowMainMenuBar = true; }
        inline void HideMainMenuBar(void) { this->bShowMainMenuBar = false; }
        inline bool IsSelected(void) const { return this->bSelected; }
        inline float getDisplayX(void) const { return this->DisplayX; }
        inline float getDisplayY(void) const { return this->DisplayY; }
        inline Fvector getMousePosition(void) const noexcept { return this->mPos; }
        inline Fvector getMouseDirection(void) const noexcept { return this->mDir; }
        inline bool IsUsedAddTool(void) noexcept { return this->bAddTool; }
        inline bool IsUsedMoveTool(void) noexcept { return this->bMoveTool; }
        inline bool IsUsedRotateTool(void) noexcept { return this->bRotateTool; }
        inline bool IsUsedScaleTool(void) noexcept { return this->bScaleTool; }
        inline void setAddTool(bool value) noexcept { this->bAddTool = value; }
        inline void setMoveTool(bool value) noexcept { this->bMoveTool = value; }
        inline void setRotateTool(bool value) noexcept { this->bRotateTool = value; }
        inline void setScaleTool(bool value) noexcept { this->bScaleTool = value; }
        inline void setHoveredColorEdit(bool value) noexcept { this->b_is_hovered_coloredit = value; }

        void Begin(void);
        void Draw(void);
        void End(void);
        bool PickGround(Fvector& hitpoint, const Fvector& start, const Fvector& dir, /*int bSnap,*/ Fvector* hitnormal);
    private:
        inline int getCenteredX(void) { return DisplayX / 2; }
        inline int getCenteredY(void) { return DisplayY / 2; }
        inline ImVec2 getPosCentered(void) { return ImVec2(this->getCenteredX(), this->getCenteredY()); }
        inline ImVec2 getPosTopLeft(void) { return ImVec2(0.0f, 0.0f); }
        inline ImVec2 getPosTopRight(void) { return ImVec2(DisplayX, 0.0f); }
        inline ImVec2 getPosBottomLeft(void) { return ImVec2(0.0f, DisplayY); }
        inline ImVec2 getPosBottomRight(void) { return ImVec2(DisplayX, DisplayY); }

        void DrawMainMenuBar(void);
        void DrawAllHelpers(void);
        void KeyBoardMessages(void);
        void GizmoHandler(void);
        void SelectionHandler(void);
        bool PrepareSelectionFrustum(CFrustum& frustum);

    private:
        bool bShowMainMenuBar;
        bool bCloseOnce;
        bool bSelected;
        bool bCanUseLeftButton;
        bool bClickedRightButton;
        bool b_is_used_selectionrectengular;
        // @ or something else like this window
        bool b_is_hovered_coloredit; 
        // @ in current time of the moment
        bool b_is_used_any_tool; 
        // @ have to states: IN if dragged in current time (true) and OUT: when we released the left button of mouse
        bool b_is_dragged_selection_rectangle; 
        bool b_is_left_mouse_clicked_still_using;
#pragma region Tools
        bool bAddTool; // Lord: подумать стоит ли это здесь так оставлять?
        bool bMoveTool;
        bool bRotateTool;
        bool bScaleTool;
#pragma endregion
        float DisplayX;
        float DisplayY;
		float dis_to_current_obj = SDK_Camera::GetInstance().fFar;
		float mouse_drag_delta;
		HMODULE m_handle_xrgame;
        ImVec2 m_start_mouseposition;
        ImVec2 m_current_mouseposition;

        // @ Mouse-Ray origin
        Fvector mPos;
        // @ Mouse-Ray
        Fvector mDir;
    };

    }
}
