#include "stdafx.h"
#include "SDKUI.h"
#include "../../xrCore/Imgui/imgui.h"
#include "../../xrCore/Imgui/imgui_impl_sdl.h"
#include "../../xrCore/Imgui/imgui_impl_dx11.h"
#include "xrEngine/XR_IOConsole.h"
#include "SDK_SceneManager.h"
#include "SDKUI_ToolBar.h"
#include "SDKUI_ObjectInspector.h"
#include "SDKUI_ObjectList.h"
#include "SDKUI_Settings.h"
// New commit a 
namespace Cordis
{
namespace SDK
{
void SDKUI::Begin(void)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplSDL2_NewFrame(Device.m_sdlWnd);
    ImGui::NewFrame();
}

bool PickGrid(Fvector& hitpoint, const Fvector& start, const Fvector& dir, /*int bSnap,*/ Fvector* hitnormal)
{
    Fvector normal;
    normal.set(0, 1, 0);
    float clcheck = dir.dotproduct(normal);

    if (fis_zero(clcheck))
        return false;

    float alpha = -start.dotproduct(normal) / clcheck;

    if (alpha <= 0)
        return false;

    hitpoint.x = start.x + dir.x * alpha;
    hitpoint.y = start.y + dir.y * alpha;
    hitpoint.z = start.z + dir.z * alpha;

    //     if (Tools->GetSettings(etfGSnap) && bSnap) Lord: реализовать это
    //     {
    //         hitpoint.x = snapto(hitpoint.x, LTools->m_MoveSnap);
    //         hitpoint.z = snapto(hitpoint.z, LTools->m_MoveSnap);
    //         hitpoint.y = 0.f;
    //     }

    if (hitnormal)
        hitnormal->set(0, 1, 0);

    return true;
}

bool SDKUI::PickGround(Fvector& hitpoint, const Fvector& start, const Fvector& dir, /*int bSnap,*/ Fvector* hitnormal)
{
    // pick object geometry Lord: реализовать
    //     if ((bSnap == -1) || (Tools->GetSettings(etfOSnap) && (bSnap == 1)))
    //     {
    //         bool b = PickObjectGeometry(est, hitpoint, start, direction, bSnap, hitnormal);
    //         if (b)
    //             return true;
    //     }
    return PickGrid(hitpoint, start, dir, hitnormal);
}

void SDKUI::KeyBoardMessages(void)
{
    // @ Хендлим шорткаты

    // @ Глобальные клики можно обрабатывать динамические пупапы если надо
    if (ImGui::IsMouseHoveringAnyWindow())
    {
		// Lord: Исправить функционал при переносе какого-либо окна, появляется выделение которого вообще не должно быть
        this->bCanUseLeftButton = false;

        if (!this->b_is_dragged_selection_rectangle)
            this->b_is_used_selectionrectengular = false;

        this->b_is_hovered_coloredit = false;
    }
    else
    {
        this->bCanUseLeftButton = true;
    }

    if (this->bCanUseLeftButton) // Lord: добавить описание если используем сам инструмент перемещения
    {
        Ivector2 p;
        p.x = ImGui::GetIO().MousePos.x;
        p.y = ImGui::GetIO().MousePos.y;
        this->mPos.set(Device.vCameraPosition);

        if (!this->bClickedRightButton)
            SDK_Camera::GetInstance().MouseRayFromPoint(this->mDir, p);

#pragma region GIZMO
        if (this->bMoveTool)
        {
            if (SDK_GizmoManager::GetInstance().m_current_gizmo != GIZMO_UNKNOWN &&
                SDK_GizmoManager::GetInstance().m_current_gizmo < 3)
            {
                SDK_GizmoManager::GetInstance().m_prev_point = SDK_GizmoManager::GetInstance().m_ray_end_point;
                GizmoMove[SDK_GizmoManager::GetInstance().m_current_gizmo].GetPoint(
                    SDKUI::UI().getMousePosition(), SDKUI::UI().getMouseDirection(), SDK_GizmoManager::GetInstance().m_ray_end_point);
            }

            if (SDK_GizmoManager::GetInstance().m_current_gizmo != GIZMO_UNKNOWN &&
                SDK_GizmoManager::GetInstance().m_current_gizmo > 2)
            {
                SDK_GizmoManager::GetInstance().m_prev_point = SDK_GizmoManager::GetInstance().m_ray_end_point_plane;
                GizmoMovePlanes[SDK_GizmoManager::GetInstance().m_current_gizmo - 3].GetPoint(
                    SDKUI::UI().getMousePosition(), SDKUI::UI().getMouseDirection());
            }
        }

        if (this->bScaleTool)
        {
            if (SDK_GizmoManager::GetInstance().m_current_scale_gizmo != GIZMO_UNKNOWN &&
                SDK_GizmoManager::GetInstance().m_current_scale_gizmo < 3)
            {
                SDK_GizmoManager::GetInstance().m_prev_point = SDK_GizmoManager::GetInstance().m_ray_end_point;
                g_gizmo_lines_scale[SDK_GizmoManager::GetInstance().m_current_scale_gizmo].getPoint(
                    SDKUI::UI().getMousePosition(), SDKUI::UI().getMouseDirection(), SDK_GizmoManager::GetInstance().m_ray_end_point);
            }

            if (SDK_GizmoManager::GetInstance().m_current_scale_gizmo != GIZMO_UNKNOWN &&
                SDK_GizmoManager::GetInstance().m_current_scale_gizmo > 2)
            {
                SDK_GizmoManager::GetInstance().m_prev_point = SDK_GizmoManager::GetInstance().m_ray_end_point_plane;
                g_gizmo_planes_scale[SDK_GizmoManager::GetInstance().m_current_scale_gizmo - 3].getPoint(
                    SDKUI::UI().getMousePosition(), SDKUI::UI().getMouseDirection());
            }

            if (SDK_GizmoManager::GetInstance().m_current_scale_gizmo == GIZMO_SCALE_BOX)
            {
                SDK_GizmoManager::GetInstance().m_prev_point = SDK_GizmoManager::GetInstance().m_ray_end_point;
                g_gizmo_lines_scale[1].getPoint(SDKUI::UI().getMousePosition(), SDKUI::UI().getMouseDirection(), SDK_GizmoManager::GetInstance().m_ray_end_point);
            }
        }
#pragma endregion
    }

    this->GizmoHandler();
    this->SelectionHandler();

    /*
                 if (ImGui::IsMouseDragging(0))
                 {
                   //  SDKUI_Log::Widget().AddText("%f %f", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
                     if (ImGui::GetMousePos().x == 0)
                     {
                         SetCursorPos(this->GetDisplayX()-4, ImGui::GetIO().MousePos.y);
                     }
        




                     if (ImGui::GetMousePos().x == this->GetDisplayX()-1)
                     {
                         SetCursorPos(4, ImGui::GetIO().MousePos.y);
                     }
        





        




        




                 }*/

    if (ImGui::IsMouseReleased(0)) // Lord: обработка если мы в драге моде и инструмент перемешение
    {
        if (SDK_GizmoManager::GetInstance().m_is_usegizmo)
            SDK_GizmoManager::GetInstance().m_is_usegizmo = false;

        this->b_is_left_mouse_clicked_still_using = false;

        if (this->b_is_used_selectionrectengular)
        {
            // @ Frustrum selection
            this->b_is_used_selectionrectengular = false;
            this->b_is_dragged_selection_rectangle = false;
            CFrustum frustum;

            if (this->PrepareSelectionFrustum(frustum))
            {
                const unsigned char* keyboard_state = SDL_GetKeyboardState(NULL);

                if ((keyboard_state[SDL_SCANCODE_LCTRL] || keyboard_state[SDL_SCANCODE_RCTRL]) &&
                    (!keyboard_state[SDL_SCANCODE_RALT] && !keyboard_state[SDL_SCANCODE_LALT]))
                {
                    SDK_SceneManager::GetInstance().Selection(frustum);
                }
                else if ((keyboard_state[SDL_SCANCODE_LALT] || keyboard_state[SDL_SCANCODE_RALT]) &&
                    (!keyboard_state[SDL_SCANCODE_RCTRL] && !keyboard_state[SDL_SCANCODE_LCTRL]))
                {
                    SDK_SceneManager::GetInstance().DeSelection(frustum);
                }
                else
                {
                    SDK_SceneManager::GetInstance().Selection(frustum, true);
                }
            }
        }
    }

    if (ImGui::IsMouseReleased(1))
    {
        this->bCanUseLeftButton = true;
        this->bClickedRightButton = false;
    }

    if (ImGui::IsMouseClicked(1)) // @ Right button
    {
        this->bCanUseLeftButton = false;
        this->bClickedRightButton = true;
		this->m_start_mouseposition.x = this->m_current_mouseposition.x;
		this->m_start_mouseposition.y = this->m_current_mouseposition.y;
        if (this->bMoveTool) // @ Lord дописывать и для остальных инструментов
        {
            SDK_GizmoManager::GetInstance().ChangeColorAxisMoveDefault();
        }

		if (this->bScaleTool)
		{
			SDK_GizmoManager::GetInstance().ChangeColorAxisScaleDefault();
		}


    }

    if (ImGui::IsMouseClicked(2)) // @ Wheel
    {
    }

#pragma region SHORTCUTS
    if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_A) && this->bCanUseLeftButton && !this->bClickedRightButton)
    {
        if (this->bAddTool)
        {
            SDKUI_Log::Widget().AddText("AddTool: OFF");
            this->bAddTool = false;
            this->b_is_used_any_tool = false;
        }
        else
        {
            SDKUI_Log::Widget().AddText("AddTool: ON");
            this->b_is_used_any_tool = true;
            this->bAddTool = true;
            this->bMoveTool = false;
            this->bRotateTool = false;
            this->bScaleTool = false;
        }
    }

    // @ Exit
    if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_LALT) || ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_RALT))
    {
        if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_F4) && !bCloseOnce)
        {
            SDKUI_Log::Widget().SetColor(good);
            SDKUI_Log::Widget().AddText(SDK_Names::getInstance().getName("st_text_closing_application").c_str());
            Console->Execute("quit");
            bCloseOnce = true;
            return;
        }
    }

    // @ Screenshot
    if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_F12))
    {
        Console->Execute("screenshot 1");
        SDKUI_Log::Widget().AddText(SDK_Names::getInstance().getName("st_text_making_screenshot").c_str(), SDKErrorType::special);
        return;
    }

    // @ Log
    if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_LCTRL) || ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_RCTRL))
    {
        if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_L))
        {
            if (!SDKUI_Log::Widget().GetVisible())
            {
                SDKUI_Log::Widget().SetColor(unimportant);
                SDKUI_Log::Widget().AddText("Log window is shown");
                SDKUI_Log::Widget().Show();
            }
        }
    }

    // @ Overlay
    if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_LCTRL) || ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_RCTRL))
    {
        if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_LALT) || ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_RALT))
        {
            if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_S))
            {
                if (!SDKUI_Overlay::Widget().GetVisible())
                {
                    SDKUI_Overlay::Widget().Show();
                    SDKUI_Log::Widget().SetColor(unimportant);
                    SDKUI_Log::Widget().AddText("Overlay window is shown");
                }
            }
        }
    }

    // @ Camera manager
    if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_LCTRL) || ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_RCTRL))
    {
        if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_SPACE))
        {
            if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_C))
            {
                if (!SDKUI_CameraHelper::Widget().GetVisible())
                {
                    SDKUI_CameraHelper::Widget().Show();
                    SDKUI_Log::Widget().SetColor(unimportant);
                    SDKUI_Log::Widget().AddText("Camera manager is shown");
                }
            }
        }
    }

	if (!SDK_SceneManager::GetInstance().m_selectedobjects_list.empty())
	{
		if (ImGui::IsKeyPressed(SDL_SCANCODE_M) && !this->bClickedRightButton)
		{
			if (!this->bMoveTool)
			{
				this->b_is_used_any_tool = true;
				this->bMoveTool = true;
				this->bScaleTool = false;
				SDKUI_Log::Widget().SetColor(unimportant);
				SDKUI_Log::Widget().AddText("Move Tool: ON");
			}
			else
			{
				this->b_is_used_any_tool = false;
				this->bMoveTool = false;
				SDKUI_Log::Widget().SetColor(unimportant);
				SDKUI_Log::Widget().AddText("Move Tool: OFF");
			}
		}

		if (ImGui::IsKeyPressed(SDL_SCANCODE_S) && !this->bClickedRightButton)
		{
			if (!this->bScaleTool)
			{
				this->b_is_used_any_tool = true;
				this->bMoveTool = false;
				this->bScaleTool = true;
				SDKUI_Log::Widget().SetColor(unimportant);
				SDKUI_Log::Widget().AddText("Scale Tool: ON");
			}
			else
			{
				this->bScaleTool = false;
				this->b_is_used_any_tool = false;
				SDKUI_Log::Widget().SetColor(unimportant);
				SDKUI_Log::Widget().AddText("Scale Tool: OFF");
			}
		}
	}


#pragma endregion
}

void SDKUI::DrawAllHelpers(void)
{
    SDKUI_Log::Widget().Draw();
    SDKUI_Overlay::Widget().Draw();
    SDKUI_CameraHelper::Widget().Draw();
    SDKUI_RightWindow::Widget().Draw();
    SDKUI_ToolBar::Widget().Draw();
    SDKUI_ObjectInspector::Widget().Draw();
    SDKUI_ObjectList::Widget().Draw();
    SDKUI_SceneOptions::Widget().Draw();
	SDKUI_Settings::Widget().Draw();

}

void SDKUI::DrawMainMenuBar(void)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Quit", "ALT+F4"))
            {
                SDKUI_Log::Widget().SetColor(good);
                SDKUI_Log::Widget().AddText("Application is closing...");
                Console->Execute("quit");
                bCloseOnce = true;
            }
            ImGui::EndMenu();
        } // @ File

        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::BeginMenu("Others"))
            {
                if (ImGui::MenuItem("Show Log", "CTRL+L"))
                {
                    if (SDKUI_Log::Widget().GetVisible() == false)
                    {
                        SDKUI_Log::Widget().Show();
                        SDKUI_Log::Widget().SetColor(unimportant);
                        SDKUI_Log::Widget().AddText("The logger show");
                    }
                }

                if (ImGui::MenuItem("Show Overlay", "CTRL+ALT+S"))
                {
                    SDKUI_Overlay::Widget().Show();
                    SDKUI_Log::Widget().SetColor(unimportant);
                    SDKUI_Log::Widget().AddText("Overlay window is shown");
                }

                if (ImGui::MenuItem("Show Camera Manager", "CTRL+SPACE+C"))
                {
                    SDKUI_CameraHelper::Widget().Show();
                    SDKUI_Log::Widget().SetColor(unimportant);
                    SDKUI_Log::Widget().AddText("Camera manager is shown");
                }

                if (ImGui::MenuItem("ScreenShot", "F12"))
                {
                    Console->Execute("screenshot 1");
                    SDKUI_Log::Widget().SetColor(special);
                    SDKUI_Log::Widget().AddText("Screenshot was made!");
                }

                if (ImGui::MenuItem("Show tool bar", "SHORTCUT NOT IMPLEMENTED"))
                {
                    SDKUI_ToolBar::Widget().Show();
                    SDKUI_Log::Widget().SetColor(unimportant);
                    SDKUI_Log::Widget().AddText("Show tool bar!");
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        } // @ Tools

		if (ImGui::BeginMenu(SDK_Names::getInstance().getName("st_label_settings").c_str()))
		{
			if (ImGui::BeginMenu(SDK_Names::getInstance().getName("st_label_languages").c_str()))
			{
				bool _current_language = false;
				for (auto language : SDK_Names::getInstance().m_languages)
				{

					if (language == SDK_Names::getInstance().getCurrentLanguage())
						_current_language = true;
					else
						_current_language = false;

					if (ImGui::MenuItem(language.c_str(), NULL, _current_language))
					{
						SDK_Names::getInstance().setCurrentLanguage(language);
					}
				}
				

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}


        ImGui::EndMainMenuBar();
    }
}

void SDKUI::Draw(void)
{
    ImGui::ShowDemoWindow();
    KeyBoardMessages();
    DrawMainMenuBar();
    DrawAllHelpers();

    ImGui::Render();
}

void SDKUI::End(void)
{
    if (ImGui::GetDrawData())
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

/*
    PRIVATE SECTION
*/

void SDKUI::GizmoHandler(void)
{
    if (!this->bMoveTool && !this->bScaleTool)
        return;

    if (this->bCanUseLeftButton && !this->bClickedRightButton && !SDK_GizmoManager::GetInstance().m_is_usegizmo &&
        this->bMoveTool) // Lord: сюда добавить поддержку того что если используем инструмент перемещение
    {
        SDK_GizmoManager::GetInstance().m_current_gizmo = SDK_SceneManager::GetInstance().SelectionAxisMove();
        switch (SDK_GizmoManager::GetInstance().m_current_gizmo) // Lord: цвета переместить в constexpr!
        {
        case GIZMO_X:
        {
            SDK_GizmoManager::GetInstance().ChangeColorAxisMoveX();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }

            break;
        }
        case GIZMO_Y:
        {
            SDK_GizmoManager::GetInstance().ChangeColorAxisMoveY();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }

            break;
        }
        case GIZMO_Z:
        {
            SDK_GizmoManager::GetInstance().ChangeColorAxisMoveZ();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }

            break;
        }
        case GIZMO_PLANE_XY:
        {
            SDK_GizmoManager::GetInstance().ChangeColorAxisMovePlaneYX();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }

            break;
        }
        case GIZMO_PLANE_ZY:
        {
            SDK_GizmoManager::GetInstance().ChangeColorAxisMovePlaneZY();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }

            break;
        }
        case GIZMO_PLANE_XZ:
        {
            SDK_GizmoManager::GetInstance().ChangeColorAxisMovePlaneXZ();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }

            break;
        }

        case GIZMO_UNKNOWN:
        {
            SDK_GizmoManager::GetInstance().ChangeColorAxisMoveDefault();
            break;
        }
        }
    }

    if (this->bMoveTool)
    {
        if (ImGui::IsMouseDragging(0))
        {
            switch (SDK_GizmoManager::GetInstance().m_current_gizmo)
            {
            case GIZMO_X:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.y = 0.0f;
                delta.z = 0.0f;

                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }

            case GIZMO_Y:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.x = 0.0f;
                delta.z = 0.0f;
                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }
            case GIZMO_Z:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.x = 0.0f;
                delta.y = 0.0f;
                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }

            case GIZMO_PLANE_XY:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point_plane;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.z = 0.0f;
                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }
            case GIZMO_PLANE_ZY:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point_plane;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.x = 0.0f;

                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }

            case GIZMO_PLANE_XZ:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point_plane;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.y = 0.0f;
                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }
            }
        }
    }
    else if (this->bScaleTool)
    {
        if (!SDK_GizmoManager::GetInstance().m_is_usegizmo)
            SDK_GizmoManager::GetInstance().m_current_scale_gizmo =
                SDK_SceneManager::GetInstance().SelectionAxisScale();

        AxisScaleType& _id = SDK_GizmoManager::GetInstance().m_current_scale_gizmo;
        switch (_id)
        {
        case GIZMO_SCALE_X:
        {
            SDKUI_Log::Widget().AddText("Intersection: X");
            SDK_GizmoManager::GetInstance().ChangeColorAxisScaleX();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }
            break;
        }
        case GIZMO_SCALE_Y:
        {
            SDKUI_Log::Widget().AddText("Intersection: Y");
            SDK_GizmoManager::GetInstance().ChangeColorAxisScaleY();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }
            break;
        }
        case GIZMO_SCALE_Z:
        {
            SDKUI_Log::Widget().AddText("Intersection: Z");
            SDK_GizmoManager::GetInstance().ChangeColorAxisScaleZ();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }
            break;
        }
        case GIZMO_SCALE_PLANE_XY:
        {
            SDKUI_Log::Widget().AddText("Intersection: XY");
            SDK_GizmoManager::GetInstance().ChangeColorAxisScalePlaneYX();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }
            break;
        }
        case GIZMO_SCALE_PLANE_XZ:
        {
            SDKUI_Log::Widget().AddText("Intersection: XZ");
            SDK_GizmoManager::GetInstance().ChangeColorAxisScalePlaneXZ();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }
            break;
        }
        case GIZMO_SCALE_PLANE_ZY:
        {
            SDKUI_Log::Widget().AddText("Intersection: ZY");
            SDK_GizmoManager::GetInstance().ChangeColorAxisScalePlaneZY();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }
            break;
        }
        case GIZMO_SCALE_BOX:
        {
            SDKUI_Log::Widget().AddText("Intersection: BOX");
            SDK_GizmoManager::GetInstance().ChangeColorAxisScaleBOX();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }
            break;
        }
        case GIZMO_SCALE_UNKNOWN:
        {
            SDK_GizmoManager::GetInstance().ChangeColorAxisScaleDefault();
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().m_is_usegizmo = true;
            }
            break;
        }
        }

        if (ImGui::IsMouseDragging(0))
        {
            switch (SDK_GizmoManager::GetInstance().m_current_scale_gizmo)
            {
            case GIZMO_SCALE_X:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.y = 0.0f;
                delta.z = 0.0f;

                SDK_SceneManager::GetInstance().Scale(delta);
                break;
            }
            case GIZMO_SCALE_Y:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.x = 0.0f;
                delta.z = 0.0f;

                SDK_SceneManager::GetInstance().Scale(delta);
                break;
            }
            case GIZMO_SCALE_Z:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.y = 0.0f;
                delta.x = 0.0f;
                SDK_SceneManager::GetInstance().Scale(delta);
                break;
            }

            case GIZMO_SCALE_PLANE_XY:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point_plane;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.z = 0.0f;
                SDK_SceneManager::GetInstance().Scale(delta);
                break;
            }

            case GIZMO_SCALE_PLANE_ZY:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point_plane;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.x = 0.0f;
                SDK_SceneManager::GetInstance().Scale(delta);
                break;
            }
            case GIZMO_SCALE_PLANE_XZ:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point_plane;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);
                delta.y = 0.0f;

                SDK_SceneManager::GetInstance().Scale(delta);
                break;
            }
            case GIZMO_SCALE_BOX:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().m_ray_end_point;
                delta.sub(SDK_GizmoManager::GetInstance().m_prev_point);

                delta.x = delta.z = delta.y;
                SDK_SceneManager::GetInstance().Scale(delta);
                break;
            }
            }
        }
    }
}

void SDKUI::SelectionHandler(void)
{
    if (!this->b_is_hovered_coloredit && !this->b_is_used_any_tool && this->bCanUseLeftButton)
    {
        if (ImGui::IsMouseDragging(0) && !SDK_GizmoManager::GetInstance().m_is_usegizmo &&
            !this->b_is_left_mouse_clicked_still_using)
        {
            this->b_is_used_selectionrectengular = true;
            this->b_is_dragged_selection_rectangle = true;
            this->m_current_mouseposition = ImGui::GetIO().MousePos;
        }
    }

    if (ImGui::IsMouseClicked(0) && this->bCanUseLeftButton && !this->bClickedRightButton &&
        !SDK_GizmoManager::GetInstance().m_is_usegizmo) // @ Left button
    {
        if (this->bAddTool)
        {
            Fvector p, n;
            if (this->PickGround(p, this->mPos, this->mDir, &n))
            {
                SDK_SceneManager::GetInstance().AddObject(p, n);
                SDK_SceneManager::GetInstance().UnSelectAll();
            }
        }
        else
        {
            const unsigned char* keyboard_state = SDL_GetKeyboardState(NULL);
            this->m_start_mouseposition = ImGui::GetIO().MousePos;
            if ((keyboard_state[SDL_SCANCODE_LCTRL] || keyboard_state[SDL_SCANCODE_RCTRL]) &&
                (!keyboard_state[SDL_SCANCODE_LALT] && !keyboard_state[SDL_SCANCODE_RALT]))
            {
                if (SDK_SceneManager::GetInstance().Selection(this->mPos, this->mDir))
                {
                    this->b_is_left_mouse_clicked_still_using = true;
                }
            }
            else if ((keyboard_state[SDL_SCANCODE_LALT] || keyboard_state[SDL_SCANCODE_RALT]) &&
                (!keyboard_state[SDL_SCANCODE_RCTRL] && !keyboard_state[SDL_SCANCODE_LCTRL]))
            {
                SDK_SceneManager::GetInstance().DeSelection(this->mPos, this->mDir);
            }
            else
            {
                if (!this->bMoveTool)
                {
                    if (SDK_SceneManager::GetInstance().Selection(this->mPos, this->mDir, true))
                        this->b_is_left_mouse_clicked_still_using = true;
                }
            }
            // Lord: перемещение по лучу можно и реальзовать
            // Device.vCameraPosition.set(this->mDir.mul(SDK_Camera::GetInstance().fFar).add(this->mPos));
            /*
                        for (size_t i = 0; i < 3; i++)
                        {
                            // @ Have intersection
                            if (GizmoMove[i].RayPick(SDK_Camera::GetInstance().fFar, this->mPos, this->mDir) < 1)
                            {
                                GizmoMove[i].clr = D3DCOLOR_ARGB(255, 255, 255, 0);
                            }
                            else
                            {
                                switch (GizmoMove[i].id)
                                {
                                case GIZMO_X:
                                {
                                    GizmoMove[i].clr = D3DCOLOR_ARGB(255, 255, 0, 0);
                                    break;
                                }
                                case GIZMO_Y:
                                {
                                    GizmoMove[i].clr = D3DCOLOR_ARGB(255, 0, 255, 0);
                                    break;
                                }
                                case GIZMO_Z:
                                {
                                    GizmoMove[i].clr = D3DCOLOR_ARGB(255, 0, 0, 255);
                                    break;
                                }
                                }
                            }

                            //   SDKUI_Log::Widget().AddText("%d %s",
                            //   i,std::to_string(GizmoMove[i].RayPick(SDK_Camera::GetInstance().fFar, this->mPos,
                            //   this->mDir)).c_str());
                        }*/
        }
    }
}

bool SDKUI::PrepareSelectionFrustum(CFrustum& frustum)
{
    float x1, x2, y1, y2, depth;
    Ivector2 points_2d[4];
    Fvector start, direction, points_3d[4];

    depth = SDK_Camera::GetInstance().fFar;

    x1 = this->m_start_mouseposition.x;
    y1 = this->m_start_mouseposition.y;
    x2 = this->m_current_mouseposition.x;
    y2 = this->m_current_mouseposition.y;

	// Lord: Buggy shit
    if (x1 == x2 || y2 == y1)
        return false;

    points_2d[0].set(_min(x1, x2), _min(y1, y2));
    points_2d[1].set(_max(x1, x2), _min(y1, y2));
    points_2d[2].set(_max(x1, x2), _max(y1, y2));
    points_2d[3].set(_min(x1, x2), _max(y1, y2));

    for (int i = 0; i < 4; ++i)
    {
        SDK_Camera::GetInstance().MouseRayFromPoint(direction, points_2d[i]);
        points_3d[i].mad(Device.vCameraPosition, direction, depth);
    }

    frustum.CreateFromPoints(points_3d, 4, Device.vCameraPosition);

    Fplane plane;
    plane.build(points_3d[0], points_3d[1], points_3d[2]);

    if (plane.classify(Device.vCameraPosition) > 0)
        plane.build(points_3d[2], points_3d[1], points_3d[0]);

    frustum._add(plane);

    return true;
}

} // namespace SDK
} // namespace Cordis
