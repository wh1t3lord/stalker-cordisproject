#pragma once
#include "SDK_GizmoMove.h"

class SDK_GizmoManager
{
private:
    SDK_GizmoManager(void) noexcept
        : m_is_usegizmo(false), m_current_gizmo(GIZMO_UNKNOWN), m_speed(0.2f), m_prev_point({0, 0, 0}), m_ray_end_point({0, 0, 0}),
          m_ray_end_point_plane({0,0,0})
    {
        
    }

public:
    inline static SDK_GizmoManager& GetInstance(void) noexcept
    {
        static SDK_GizmoManager instance;
        return instance;
    }
    ~SDK_GizmoManager(void) = default;

    SDK_GizmoManager(const SDK_GizmoManager&) = delete;
    SDK_GizmoManager& operator=(const SDK_GizmoManager&) = delete;
    SDK_GizmoManager(SDK_GizmoManager&&) = delete;
    SDK_GizmoManager& operator=(SDK_GizmoManager&&) = delete;

public:
    bool m_is_usegizmo;
    AxisType m_current_gizmo;
    float m_speed;
    Fvector m_prev_point;
    Fvector m_ray_end_point;
    Fvector m_ray_end_point_plane;
};
