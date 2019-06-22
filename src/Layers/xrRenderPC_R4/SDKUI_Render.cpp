#include "stdafx.h"
#include "SDKUI_Render.h"
#include "SDKUI.h"
#include "r4.h"
#include "SDK_SceneManager.h"
#include "SDK_Settings.h"

namespace Cordis
{
    namespace SDK
    {
    SDKUI_Render::~SDKUI_Render(void) {}

    void SDKUI_Render::Initialize(void)
    {
        // Lord: наверное сюда же нужно поместить контант буфферы компиляцию шейдеров и прочее, хотя наверное только
        // онли рантайм загрузка Lord: БЛЯТЬ ДЖОННИ ЭТО ПИЗДЕЦ КАК ЖЕ ПЫС ЗАЕБАЛИ ХАРДКОДИТЬ
		// Сюда добавлять только то что относится к рендеру и не более!!!

        SDK_Settings::GetInstance().Initialize();
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
        Fvector a;
        a.set(0, 0, 0);

        u32 clr = D3DCOLOR_ARGB(255, 255, 255, 255); // Lord: добавить кастомизацию

        for (xr_list<SDK_CustomObject*>::value_type object : SDK_SceneManager::GetInstance().m_objects_list)
        {
            object->Render(1, false);

            if (object->m_is_selected)
            {
                // RCache.set_xform_world(object->GetTransform());
                DUImpl.DrawSelectionBoxB(object->GetBox(), &clr);
                if (SDKUI::UI().IsUsedMoveTool())
                {
                    DUImpl.DrawGizmoMove(SDK_SceneManager::GetInstance().GetPositionFromLastSelectedObject());
                }
                else if (SDKUI::UI().IsUsedScaleTool())
                {
                    DUImpl.DrawGizmoScale(SDK_SceneManager::GetInstance().GetPositionFromLastSelectedObject());
                }
            }
        }

        //     if (SDKUI::UI().IsSelected())
        //     {
        //         //    RImplementation.obj->RenderSelection(Fidentity);
        //   //      DUImpl.DrawObjectAxis(RImplementation.obj->GetTransform(), 1.0f, TRUE);
        //         DUImpl.DrawSelectionBoxB(RImplementation.obj->GetData()->GetBox(), &clr);
        //     }

#pragma region Gizmo
        //     for (size_t i = 0; i < 3; i++)
        //     {
        //         DUImpl.DrawGizmoLine(GizmoMove[i].origin, GizmoMove[i].end, GizmoMove[i].clr);
        //
        //     }
        /*Fvector new_pos;
        new_pos.set(29, 0, -0.95);
        DUImpl.DrawGizmoLine(GizmoMove[0].end, new_pos, GizmoMove[0].clr);
        new_pos.set(29, 0, 0.95);
        DUImpl.DrawGizmoLine(GizmoMove[0].end, new_pos, GizmoMove[0].clr);
        new_pos.set(29, 0.95, 0);
        DUImpl.DrawGizmoLine(GizmoMove[0].end, new_pos, GizmoMove[0].clr);
        new_pos.set(29, -0.95, 0);
        DUImpl.DrawGizmoLine(GizmoMove[0].end, new_pos, GizmoMove[0].clr);
    */
        // Lord: Debug ray by mouse (ADD it as debug??? :D)

        /*
            Fvector test_pos = SDKUI::UI().GetmPos();
            test_pos.x += 2;
            static xr_vector<Fvector> starts;
            static xr_vector<Fvector> ends;
            starts.push_back(test_pos);
            Fvector dir = SDKUI::UI().GetmDir();
            ends.push_back(SDK_GizmoManager::GetInstance().ray_end_point_plane);
            for (int i = 0, j = 0; i < starts.size(), j < ends.size(); ++i, ++j)
            {
                DUImpl.DrawGizmoLine(starts[i], ends[i], D3DCOLOR_ARGB(255, 155, 200, 0));
            }

            if (starts.size() > 100)
            {
                starts.clear();
                ends.clear();
            }*/

        //  DUImpl.DrawGizmoLine(test_pos, ray_end_point, D3DCOLOR_ARGB(255, 255, 255, 0));
#pragma region TheGrid
        if (EditorPref::GetInstance().bDrawGrid)
        {
            Fvector pos_pivot;
            pos_pivot.set(0, 0, 0);
            DUImpl.DrawPivot(pos_pivot, GridOptions::Size / 2);

            DUImpl.UpdateGrid(GridOptions::Size, GridOptions::separator);
            DUImpl.DrawGrid();
        }
#pragma endregion
        Ivector2 start, current;
        start.x = SDKUI::UI().m_start_mouseposition.x;
        start.y = SDKUI::UI().m_start_mouseposition.y;
        current.x = SDKUI::UI().m_current_mouseposition.x;
        current.y = SDKUI::UI().m_current_mouseposition.y;

        if (SDKUI::UI().b_is_used_selectionrectengular && !SDKUI::UI().b_is_hovered_coloredit)
        {
            DUImpl.DrawSelectionRect(start, current);
        }


        SDKUI::UI().End();
    }

    }
}
