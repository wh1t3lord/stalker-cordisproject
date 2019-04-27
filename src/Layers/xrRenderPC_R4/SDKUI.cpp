#include "stdafx.h"
#include "SDKUI.h"
#include "../../xrCore/Imgui/imgui.h"
#include "../../xrCore/Imgui/imgui_impl_sdl.h"
#include "../../xrCore/Imgui/imgui_impl_dx11.h"
#include "xrEngine/XR_IOConsole.h"
#include "SDK_SceneManager.h"
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
    }
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

    if (ImGui::IsMouseClicked(0) && this->bCanUseLeftButton && !this->bClickedRightButton) // @ Left button
    {
        Ivector2 p;
        p.x = ImGui::GetIO().MousePos.x;
        p.y = ImGui::GetIO().MousePos.y;
        this->mPos.set(Device.vCameraPosition);
        SDK_Camera::GetInstance().MouseRayFromPoint(this->mDir, p);
        // Lord: написать функционал для Ctrl и Alt
        if (this->bAddTool)
        {

            Fvector p, n;
            if (this->PickGround(p, this->mPos, this->mDir, &n))
            {
                SDK_SceneManager::GetInstance().AddObject(p,n);
            }
                 
        }
        else
        {
            
         //   this->PickObject();

            if (SDK_SceneManager::GetInstance().SingleSelection(this->mPos, this->mDir))
            {

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
    }

    if (ImGui::IsMouseClicked(2)) // @ Wheel
    {
    }

    if (ImGui::IsKeyPressed(SDL_Scancode::SDL_SCANCODE_A) && this->bCanUseLeftButton)
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
}

void SDKUI::DrawAllHelpers(void)
{
    SDKUI_Log::Widget().Draw();
    SDKUI_Overlay::Widget().Draw();
    SDKUI_CameraHelper::Widget().Draw();
    SDKUI_RightWindow::Widget().Draw();
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
        }

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
