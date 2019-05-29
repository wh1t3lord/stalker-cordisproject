#pragma once

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
    SDK_GizmoMove(AxisType type) : id(type) {}
    ~SDK_GizmoMove(void) = default;

    bool GetPoint(const Fvector& pos, const Fvector& dir, Fvector& pB);
    float RayPick(const Fvector& pos, const Fvector& dir);

public:
    AxisType id;
    unsigned int clr;
    Fvector origin;
    Fvector end;
};

class SDK_GizmoMovePlane
{
public:
    SDK_GizmoMovePlane(void) = default;
    SDK_GizmoMovePlane(AxisType type) : id(type) {}
    ~SDK_GizmoMovePlane(void) = default;

    bool GetPoint(const Fvector& p, const Fvector& d);
    bool RayPick(const Fvector& p, const Fvector& d);

    void SetPoints(const Fvector& p0, const Fvector& p1, const Fvector& p2, const Fvector& p3)
    {
        this->points[0] = p0;
        this->points[1] = p1;
        this->points[2] = p2;
        this->points[3] = p3;
    }

    inline const Fvector& Origin(void) { return this->points[0]; }

private:
    Fvector GetNormalOfPlane(void);

public:
    AxisType id;
    unsigned int clr_left;
    unsigned int clr_right;
    Fvector points[4];
};

extern SDK_GizmoMove GizmoMove[3];
extern SDK_GizmoMovePlane GizmoMovePlanes[3];
