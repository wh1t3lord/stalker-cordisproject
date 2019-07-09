#pragma once

enum AxisType;

namespace Cordis
{
namespace SDK
{
enum AxisScaleType
{
    GIZMO_SCALE_X,
    GIZMO_SCALE_Y,
    GIZMO_SCALE_Z,
    GIZMO_SCALE_PLANE_XY,
    GIZMO_SCALE_PLANE_XZ,
    GIZMO_SCALE_PLANE_ZY,
    GIZMO_SCALE_BOX,
    GIZMO_SCALE_UNKNOWN = -1
};

class SDK_GizmoScale
{
public:
    SDK_GizmoScale(void) = default;
    SDK_GizmoScale(const AxisScaleType& _id) : m_axis_id(_id), m_origin_point({0, 0, 0}), m_end_point({0, 0, 0}) {}

    ~SDK_GizmoScale(void);

    bool getPoint(const Fvector& pos, const Fvector& dir, Fvector& pB);
    float RayPick(const Fvector& pos, const Fvector& dir);

public:
    AxisScaleType m_axis_id;
    unsigned int m_color;
    Fvector m_origin_point;
    Fvector m_end_point;
};

class SDK_GizmoScalePlane
{
public:
    SDK_GizmoScalePlane(void) = default;
    SDK_GizmoScalePlane(const AxisScaleType& _id) : m_axis_id(_id) {}
    ~SDK_GizmoScalePlane(void);

    bool getPoint(const Fvector& position, const Fvector& direction);
    bool RayPick(const Fvector& position, const Fvector& direction);

    inline void setPoints(const Fvector& point0, const Fvector& point1, const Fvector& point2, const Fvector& point3)
    {
        this->m_points[0] = point0;
        this->m_points[1] = point1;
        this->m_points[2] = point2;
        this->m_points[3] = point3;
    }

    inline const Fvector& getOrigin(void) const { return this->m_points[0]; }

private:
    Fvector GetNormalOfPlane(void);

public:
    AxisScaleType m_axis_id;
    unsigned int m_color;
    Fvector m_points[4];
};

extern SDK_GizmoScale g_gizmo_lines_scale[3];
extern SDK_GizmoScalePlane g_gizmo_planes_scale[9];
} // namespace SDK
} // namespace Cordis
