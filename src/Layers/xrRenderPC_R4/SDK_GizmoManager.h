#pragma once
#include "SDK_GizmoMove.h"

class SDK_GizmoManager
{
private:
    SDK_GizmoManager(void) noexcept
        : bUseGizmo(false), current_gizmo(GIZMO_UNKNOWN), fSpeed(0.2f), prev_point({0, 0, 0}), ray_end_point({0, 0, 0}),
          ray_end_point_plane({0,0,0})
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
    bool bUseGizmo;
    AxisType current_gizmo;
    float fSpeed;
    Fvector prev_point;
    Fvector ray_end_point;
    Fvector ray_end_point_plane;
};
