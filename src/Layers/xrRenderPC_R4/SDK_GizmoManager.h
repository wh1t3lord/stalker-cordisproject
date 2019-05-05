#pragma once
#include "SDK_GizmoMove.h"

class SDK_GizmoManager
{
public:
    inline static SDK_GizmoManager& GetInstance(void) noexcept 
    { 
        static SDK_GizmoManager instance;
        return instance;
    }
    SDK_GizmoManager(void) = default;
    ~SDK_GizmoManager(void) = default;

    SDK_GizmoManager(const SDK_GizmoManager&) = delete;
    SDK_GizmoManager& operator=(const SDK_GizmoManager&) = delete;
    SDK_GizmoManager(SDK_GizmoManager&&) = delete;
    SDK_GizmoManager& operator=(SDK_GizmoManager&&) = delete;

public:
    bool bUseGizmo = false;
    AxisType current_gizmo = GIZMO_UNKNOWN;
    float fSpeed = 0.2f;
};
