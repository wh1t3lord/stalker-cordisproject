#include "stdafx.h"
#include "SDK_GizmoManager.h"

namespace Cordis
{
    namespace SDK
    {
    void SDK_GizmoManager::ChangeColorAxisMoveX(void) 
    {
        GizmoMove[1].m_color_value = g_gizmo_y_color;
        GizmoMove[2].m_color_value = g_gizmo_z_color;
        GizmoMovePlanes[0].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[0].m_color_right_value = g_gizmo_x_color;
        GizmoMovePlanes[1].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[1].m_color_right_value = g_gizmo_z_color;
        GizmoMovePlanes[2].m_color_left_value = g_gizmo_x_color;
        GizmoMovePlanes[2].m_color_right_value = g_gizmo_z_color;
        GizmoMove[0].m_color_value = g_gizmo_selectedcolor;
    }

    void SDK_GizmoManager::ChangeColorAxisMoveY(void)
    {
        GizmoMove[0].m_color_value = g_gizmo_x_color;
        GizmoMove[2].m_color_value = g_gizmo_z_color;
        GizmoMovePlanes[0].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[0].m_color_right_value = g_gizmo_x_color;
        GizmoMovePlanes[1].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[1].m_color_right_value = g_gizmo_z_color;
        GizmoMovePlanes[2].m_color_left_value = g_gizmo_x_color;
        GizmoMovePlanes[2].m_color_right_value = g_gizmo_z_color;
        GizmoMove[1].m_color_value = g_gizmo_selectedcolor;
    }

    void SDK_GizmoManager::ChangeColorAxisMoveZ(void) 
    {
        GizmoMove[0].m_color_value = g_gizmo_x_color;
        GizmoMove[1].m_color_value = g_gizmo_y_color;
        GizmoMovePlanes[0].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[0].m_color_right_value = g_gizmo_x_color;
        GizmoMovePlanes[1].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[1].m_color_right_value = g_gizmo_z_color;
        GizmoMovePlanes[2].m_color_left_value = g_gizmo_x_color;
        GizmoMovePlanes[2].m_color_right_value = g_gizmo_z_color;
        GizmoMove[2].m_color_value = g_gizmo_selectedcolor;
    }

    void SDK_GizmoManager::ChangeColorAxisMovePlaneXZ(void) 
    {
        GizmoMove[0].m_color_value = g_gizmo_x_color;
        GizmoMove[1].m_color_value = g_gizmo_y_color;
        GizmoMove[2].m_color_value = g_gizmo_z_color;
        GizmoMovePlanes[0].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[0].m_color_right_value = g_gizmo_x_color;
        GizmoMovePlanes[1].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[1].m_color_right_value = g_gizmo_z_color;
        GizmoMovePlanes[2].m_color_left_value = GizmoMovePlanes[2].m_color_right_value = g_gizmo_selectedcolor;
    }

    void SDK_GizmoManager::ChangeColorAxisMovePlaneYX(void)
    {
        GizmoMove[0].m_color_value = g_gizmo_x_color;
        GizmoMove[1].m_color_value = g_gizmo_y_color;
        GizmoMove[2].m_color_value = g_gizmo_z_color;
        GizmoMovePlanes[1].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[1].m_color_right_value = g_gizmo_z_color;
        GizmoMovePlanes[2].m_color_left_value = g_gizmo_x_color;
        GizmoMovePlanes[2].m_color_right_value = g_gizmo_z_color;
        GizmoMovePlanes[0].m_color_left_value = GizmoMovePlanes[0].m_color_right_value = g_gizmo_selectedcolor;
    }

    void SDK_GizmoManager::ChangeColorAxisMovePlaneZY(void) 
    {
        GizmoMove[0].m_color_value = g_gizmo_x_color;
        GizmoMove[1].m_color_value = g_gizmo_y_color;
        GizmoMove[2].m_color_value = g_gizmo_z_color;
        GizmoMovePlanes[0].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[0].m_color_right_value = g_gizmo_x_color;
        GizmoMovePlanes[2].m_color_left_value = g_gizmo_x_color;
        GizmoMovePlanes[2].m_color_right_value = g_gizmo_z_color;
        GizmoMovePlanes[1].m_color_left_value = GizmoMovePlanes[1].m_color_right_value = g_gizmo_selectedcolor;
    }

    void SDK_GizmoManager::ChangeColorAxisMoveDefault(void)
    {
        GizmoMove[0].m_color_value = g_gizmo_x_color;
        GizmoMove[1].m_color_value = g_gizmo_y_color;
        GizmoMove[2].m_color_value = g_gizmo_z_color;
        GizmoMovePlanes[0].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[0].m_color_right_value = g_gizmo_x_color;
        GizmoMovePlanes[1].m_color_left_value = g_gizmo_y_color;
        GizmoMovePlanes[1].m_color_right_value = g_gizmo_z_color;
        GizmoMovePlanes[2].m_color_left_value = g_gizmo_x_color;
        GizmoMovePlanes[2].m_color_right_value = g_gizmo_z_color;
    }

    void SDK_GizmoManager::ChangeColorAxisScaleX(void) 
    {
        g_gizmo_lines_scale[0].m_color = g_gizmo_selectedcolor;
        g_gizmo_lines_scale[1].m_color = g_gizmo_y_color;
        g_gizmo_lines_scale[2].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_planes_scale[1].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[2].m_color = g_gizmo_y_color;
        g_gizmo_planes_scale[3].m_color = g_gizmo_box_color;
    }

    void SDK_GizmoManager::ChangeColorAxisScaleY(void) 
    {
        g_gizmo_lines_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_lines_scale[1].m_color = g_gizmo_selectedcolor;
        g_gizmo_lines_scale[2].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_planes_scale[1].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[2].m_color = g_gizmo_y_color;
        g_gizmo_planes_scale[3].m_color = g_gizmo_box_color;
    }

    void SDK_GizmoManager::ChangeColorAxisScaleZ(void) 
    {
        g_gizmo_lines_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_lines_scale[1].m_color = g_gizmo_y_color;
        g_gizmo_lines_scale[2].m_color = g_gizmo_selectedcolor;
        g_gizmo_planes_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_planes_scale[1].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[2].m_color = g_gizmo_y_color;
        g_gizmo_planes_scale[3].m_color = g_gizmo_box_color;
    }

    void SDK_GizmoManager::ChangeColorAxisScalePlaneXZ(void) 
    {
        g_gizmo_lines_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_lines_scale[1].m_color = g_gizmo_y_color;
        g_gizmo_lines_scale[2].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_planes_scale[1].m_color = g_gizmo_selectedcolor;
        g_gizmo_planes_scale[2].m_color = g_gizmo_y_color;
        g_gizmo_planes_scale[3].m_color = g_gizmo_box_color;
    }

    void SDK_GizmoManager::ChangeColorAxisScalePlaneYX(void)
    {
        g_gizmo_lines_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_lines_scale[1].m_color = g_gizmo_y_color;
        g_gizmo_lines_scale[2].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[0].m_color = g_gizmo_selectedcolor;
        g_gizmo_planes_scale[1].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[2].m_color = g_gizmo_y_color;
        g_gizmo_planes_scale[3].m_color = g_gizmo_box_color;
    }

    void SDK_GizmoManager::ChangeColorAxisScalePlaneZY(void) 
    {
        g_gizmo_lines_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_lines_scale[1].m_color = g_gizmo_y_color;
        g_gizmo_lines_scale[2].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_planes_scale[1].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[2].m_color = g_gizmo_selectedcolor;
        g_gizmo_planes_scale[3].m_color = g_gizmo_box_color;
    }

    void SDK_GizmoManager::ChangeColorAxisScaleBOX(void) 
    {
        g_gizmo_lines_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_lines_scale[1].m_color = g_gizmo_y_color;
        g_gizmo_lines_scale[2].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_planes_scale[1].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[2].m_color = g_gizmo_y_color;
        g_gizmo_planes_scale[3].m_color = g_gizmo_selectedcolor;
    }

    void SDK_GizmoManager::ChangeColorAxisScaleDefault(void) 
    {
        
        g_gizmo_lines_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_lines_scale[1].m_color = g_gizmo_y_color;
        g_gizmo_lines_scale[2].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[0].m_color = g_gizmo_x_color;
        g_gizmo_planes_scale[1].m_color = g_gizmo_z_color;
        g_gizmo_planes_scale[2].m_color = g_gizmo_y_color;
        g_gizmo_planes_scale[3].m_color = g_gizmo_box_color;
    }

    } // namespace SDK
}
