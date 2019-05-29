#include "stdafx.h"
#include "SDKUI.h"
#include "../../xrCore/Imgui/imgui.h"
#include "../../xrCore/Imgui/imgui_impl_sdl.h"
#include "../../xrCore/Imgui/imgui_impl_dx11.h"
#include "xrEngine/XR_IOConsole.h"
#include "SDK_SceneManager.h"
#include "SDKUI_ToolBar.h"
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

void SDKUI::PickObject(void)
{
    /*
        this->dis_to_current_obj = SDK_Camera::GetInstance().fFar;

        Fbox a = RImplementation.obj->GetData()->GetBox();
        a.xform(RImplementation.obj->GetTransform()); // LORD: УЧИТЫВАЕМ ПРОСЧЁТ BBOX!!!!!!!!
        if (RImplementation.occ_visible(a))
        {
            Fmatrix lol = RImplementation.obj->GetTransform();
            lol.invert();
            //    SDKUI_Log::Widget().AddText("Rendering!");
            if (RImplementation.obj->GetData()->RayPick(this->dis_to_current_obj, this->mPos, this->mDir, lol))
            {
                this->bSelected = true;
                //    SDKUI_Log::Widget().AddText("Selected!");
            }

            else
                this->bSelected = false;
        }*/
}

void SDKUI::KeyBoardMessages(void)
{
    // @ Хендлим шорткаты

    // @ Глобальные клики можно обрабатывать динамические пупапы если надо
    if (ImGui::IsMouseHoveringAnyWindow())
    {
        this->bCanUseLeftButton = false;
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
        SDK_Camera::GetInstance().MouseRayFromPoint(this->mDir, p);

        #pragma region GIZMO 
        if (SDK_GizmoManager::GetInstance().current_gizmo != GIZMO_UNKNOWN &&
            SDK_GizmoManager::GetInstance().current_gizmo < 3)
        {
            SDK_GizmoManager::GetInstance().prev_point = SDK_GizmoManager::GetInstance().ray_end_point;
            GizmoMove[SDK_GizmoManager::GetInstance().current_gizmo].GetPoint(
                SDKUI::UI().GetmPos(), SDKUI::UI().GetmDir(), SDK_GizmoManager::GetInstance().ray_end_point);
        }

        if (SDK_GizmoManager::GetInstance().current_gizmo != GIZMO_UNKNOWN &&
            SDK_GizmoManager::GetInstance().current_gizmo > 2)
        {
            SDK_GizmoManager::GetInstance().prev_point = SDK_GizmoManager::GetInstance().ray_end_point_plane;
            GizmoMovePlanes[SDK_GizmoManager::GetInstance().current_gizmo - 3].GetPoint(
                SDKUI::UI().GetmPos(), SDKUI::UI().GetmDir());
        }
        #pragma endregion

    }


    this->GizmoHandler();
    this->SelectionHandler();


    if (ImGui::IsMouseReleased(0) &&
        SDK_GizmoManager::GetInstance().bUseGizmo) // Lord: обработка если мы в драге моде и инструмент перемешение
    {
        SDK_GizmoManager::GetInstance().bUseGizmo = false;
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
        }
        else
        {
            SDKUI_Log::Widget().AddText("AddTool: ON");
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
            SDKUI_Log::Widget().AddText("Application is closing...");
            Console->Execute("quit");
            bCloseOnce = true;
            return;
        }
    }

    // @ Screenshot
    if (ImGui::IsKeyDown(SDL_Scancode::SDL_SCANCODE_F12))
    {
        Console->Execute("screenshot 1");
        SDKUI_Log::Widget().AddText("Screenshot was made!", SDKErrorType::special);
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

    if (ImGui::IsKeyPressed(SDL_SCANCODE_M) && !this->bClickedRightButton)
    {
        if (!this->bMoveTool)
        {
            this->bMoveTool = true;
            SDKUI_Log::Widget().SetColor(unimportant);
            SDKUI_Log::Widget().AddText("Move Tool: ON");
        }
        else
        {
            this->bMoveTool = false;
            SDKUI_Log::Widget().SetColor(unimportant);
            SDKUI_Log::Widget().AddText("Move Tool: OFF");
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
    if (!this->bMoveTool) 
        return;

    if (this->bCanUseLeftButton && !this->bClickedRightButton && !SDK_GizmoManager::GetInstance().bUseGizmo &&
        this->bMoveTool) // Lord: сюда добавить поддержку того что если используем инструмент перемещение
    {
        SDK_GizmoManager::GetInstance().current_gizmo = SDK_SceneManager::GetInstance().SelectionAxisMove();
        switch (SDK_GizmoManager::GetInstance().current_gizmo) // Lord: цвета переместить в constexpr!
        {
        case GIZMO_X:
        {
            GizmoMove[1].clr = GizmoYColor;
            GizmoMove[2].clr = GizmoZColor;
            GizmoMovePlanes[0].clr_left = GizmoYColor;
            GizmoMovePlanes[0].clr_right = GizmoXColor;
            GizmoMovePlanes[1].clr_left = GizmoYColor;
            GizmoMovePlanes[1].clr_right = GizmoZColor;
            GizmoMovePlanes[2].clr_left = GizmoXColor;
            GizmoMovePlanes[2].clr_right = GizmoZColor;
            GizmoMove[0].clr = GizmoSelectedColor;

            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().bUseGizmo = true;
            }

            break;
        }
        case GIZMO_Y:
        {
            GizmoMove[0].clr = GizmoXColor;
            GizmoMove[2].clr = GizmoZColor;
            GizmoMovePlanes[0].clr_left = GizmoYColor;
            GizmoMovePlanes[0].clr_right = GizmoXColor;
            GizmoMovePlanes[1].clr_left = GizmoYColor;
            GizmoMovePlanes[1].clr_right = GizmoZColor;
            GizmoMovePlanes[2].clr_left = GizmoXColor;
            GizmoMovePlanes[2].clr_right = GizmoZColor;
            GizmoMove[1].clr = GizmoSelectedColor;

            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().bUseGizmo = true;
            }

            break;
        }
        case GIZMO_Z:
        {
            GizmoMove[0].clr = GizmoXColor;
            GizmoMove[1].clr = GizmoYColor;
            GizmoMovePlanes[0].clr_left = GizmoYColor;
            GizmoMovePlanes[0].clr_right = GizmoXColor;
            GizmoMovePlanes[1].clr_left = GizmoYColor;
            GizmoMovePlanes[1].clr_right = GizmoZColor;
            GizmoMovePlanes[2].clr_left = GizmoXColor;
            GizmoMovePlanes[2].clr_right = GizmoZColor;
            GizmoMove[2].clr = GizmoSelectedColor;
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().bUseGizmo = true;
            }

            break;
        }
        case GIZMO_PLANE_XY:
        {
            GizmoMove[0].clr = GizmoXColor;
            GizmoMove[1].clr = GizmoYColor;
            GizmoMove[2].clr = GizmoZColor;
            GizmoMovePlanes[1].clr_left = GizmoYColor;
            GizmoMovePlanes[1].clr_right = GizmoZColor;
            GizmoMovePlanes[2].clr_left = GizmoXColor;
            GizmoMovePlanes[2].clr_right = GizmoZColor;
            GizmoMovePlanes[0].clr_left = GizmoMovePlanes[0].clr_right = GizmoSelectedColor;
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().bUseGizmo = true;
            }

            break;
        }
        case GIZMO_PLANE_ZY:
        {
            GizmoMove[0].clr = GizmoXColor;
            GizmoMove[1].clr = GizmoYColor;
            GizmoMove[2].clr = GizmoZColor;
            GizmoMovePlanes[0].clr_left = GizmoYColor;
            GizmoMovePlanes[0].clr_right = GizmoXColor;
            GizmoMovePlanes[2].clr_left = GizmoXColor;
            GizmoMovePlanes[2].clr_right = GizmoZColor;
            GizmoMovePlanes[1].clr_left = GizmoMovePlanes[1].clr_right = GizmoSelectedColor;
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().bUseGizmo = true;
            }

            break;
        }
        case GIZMO_PLANE_XZ:
        {
            GizmoMove[0].clr = GizmoXColor;
            GizmoMove[1].clr = GizmoYColor;
            GizmoMove[2].clr = GizmoZColor;
            GizmoMovePlanes[0].clr_left = GizmoYColor;
            GizmoMovePlanes[0].clr_right = GizmoXColor;
            GizmoMovePlanes[1].clr_left = GizmoYColor;
            GizmoMovePlanes[1].clr_right = GizmoZColor;
            GizmoMovePlanes[2].clr_left = GizmoMovePlanes[2].clr_right = GizmoSelectedColor;
            if (ImGui::IsMouseClicked(0))
            {
                SDK_GizmoManager::GetInstance().bUseGizmo = true;
            }

            break;
        }

        case GIZMO_UNKNOWN:
        {
            GizmoMove[0].clr = GizmoXColor;
            GizmoMove[1].clr = GizmoYColor;
            GizmoMove[2].clr = GizmoZColor;
            GizmoMovePlanes[0].clr_left = GizmoYColor;
            GizmoMovePlanes[0].clr_right = GizmoXColor;
            GizmoMovePlanes[1].clr_left = GizmoYColor;
            GizmoMovePlanes[1].clr_right = GizmoZColor;
            GizmoMovePlanes[2].clr_left = GizmoXColor;
            GizmoMovePlanes[2].clr_right = GizmoZColor;
            break;
        }
        }
    }

    if (this->bMoveTool)
    {
        if (ImGui::IsMouseDragging(0))
        {
            switch (SDK_GizmoManager::GetInstance().current_gizmo)
            {
            case GIZMO_X:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().ray_end_point;
                delta.sub(SDK_GizmoManager::GetInstance().prev_point);
                delta.y = 0.0f;
                delta.z = 0.0f;

                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }

            case GIZMO_Y:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().ray_end_point;
                delta.sub(SDK_GizmoManager::GetInstance().prev_point);
                delta.x = 0.0f;
                delta.z = 0.0f;
                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }
            case GIZMO_Z:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().ray_end_point;
                delta.sub(SDK_GizmoManager::GetInstance().prev_point);
                delta.x = 0.0f;
                delta.y = 0.0f;
                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }

            case GIZMO_PLANE_XY:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().ray_end_point_plane;
                delta.sub(SDK_GizmoManager::GetInstance().prev_point);
                delta.z = 0.0f;
                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }
            case GIZMO_PLANE_ZY:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().ray_end_point_plane;
                delta.sub(SDK_GizmoManager::GetInstance().prev_point);
                delta.x = 0.0f;

                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }

            case GIZMO_PLANE_XZ:
            {
                Fvector delta = SDK_GizmoManager::GetInstance().ray_end_point_plane;
                delta.sub(SDK_GizmoManager::GetInstance().prev_point);
                delta.y = 0.0f;
                SDK_SceneManager::GetInstance().Move(delta);

                break;
            }
            }
        }
    }
}

void SDKUI::SelectionHandler(void)
{
    if (ImGui::IsMouseClicked(0) && this->bCanUseLeftButton && !this->bClickedRightButton && !SDK_GizmoManager::GetInstance().bUseGizmo) // @ Left button
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

            if ((keyboard_state[SDL_SCANCODE_LCTRL] ||
                keyboard_state[SDL_SCANCODE_RCTRL]) &&
                    (!keyboard_state[SDL_SCANCODE_LALT] && !keyboard_state[SDL_SCANCODE_RALT]))
            {
                SDK_SceneManager::GetInstance().Selection(this->mPos, this->mDir);
            }
            else if ((keyboard_state[SDL_SCANCODE_LALT] || keyboard_state[SDL_SCANCODE_RALT]) &&
                    (!keyboard_state[SDL_SCANCODE_RCTRL] && !keyboard_state[SDL_SCANCODE_LCTRL]))
            {
                SDK_SceneManager::GetInstance().DeSelection(this->mPos, this->mDir);
            }
            else
            {
                if (!this->bMoveTool)
                    SDK_SceneManager::GetInstance().Selection(this->mPos, this->mDir, true);
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
