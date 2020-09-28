#pragma once

namespace Cordis
{
    namespace SDK
    {
    enum AxisType
    {
        GIZMO_X,
        GIZMO_Y,
        GIZMO_Z,
        GIZMO_PLANE_XY,
        GIZMO_PLANE_ZY,
        GIZMO_PLANE_XZ,
        GIZMO_UNKNOWN = -1
    };

    class SDK_GizmoMove
    {
    public:
        SDK_GizmoMove(void) = default;
        SDK_GizmoMove(AxisType id_) : m_axis_id(id_) {}
        ~SDK_GizmoMove(void) = default;

        bool GetPoint(const Fvector& pos, const Fvector& dir, Fvector& pB);
        float RayPick(const Fvector& pos, const Fvector& dir);

    public:
        AxisType m_axis_id;
        unsigned int m_color_value;
        Fvector m_origin_point;
        Fvector m_end_point;
    };

    class SDK_GizmoMovePlane
    {
    public:
        SDK_GizmoMovePlane(void) = default;
        SDK_GizmoMovePlane(AxisType id_) : m_axis_id(id_) {}
        ~SDK_GizmoMovePlane(void) = default;

        bool GetPoint(const Fvector& position, const Fvector& direction);
        bool RayPick(const Fvector& position, const Fvector& direction);

        void SetPoints(const Fvector& point0, const Fvector& point1, const Fvector& point2, const Fvector& point3)
        {
            this->m_points[0] = point0;
            this->m_points[1] = point1;
            this->m_points[2] = point2;
            this->m_points[3] = point3;
        }

        inline const Fvector& Origin(void) const { return this->m_points[0]; }

    private:
        Fvector GetNormalOfPlane(void);

    public:
        AxisType m_axis_id;
        unsigned int m_color_left_value;
        unsigned int m_color_right_value;
        Fvector m_points[4];
    };

    extern SDK_GizmoMove GizmoMove[3];
    extern SDK_GizmoMovePlane GizmoMovePlanes[3];

    }
}
