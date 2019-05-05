#include "stdafx.h"
#include "SDK_GizmoMove.h"
#include "SDKUI_Helpers.h"
#include "FreeMagic/Source/Intersection3D/MgcIntr3DLinBox.h"

SDK_GizmoMove GizmoMove[3] = {SDK_GizmoMove(GIZMO_X), SDK_GizmoMove(GIZMO_Y), SDK_GizmoMove(GIZMO_Z)};
SDK_GizmoMovePlane GizmoMovePlanes[3] = {
    SDK_GizmoMovePlane(GIZMO_PLANE_XY), SDK_GizmoMovePlane(GIZMO_PLANE_ZY), SDK_GizmoMovePlane(GIZMO_PLANE_XZ)};

float SDK_GizmoMove::RayPick(const float& distance, const Fvector& pos, const Fvector& dir)
{
    Fvector dir_copy = dir;
    Fvector end_point = dir_copy.mul(distance);
    end_point.add(pos);
    Fvector p2_copy = this->end;
    Fvector pos_copy = pos;
    Fvector u = end_point.sub(pos);
    Fvector v = p2_copy.sub(this->origin);
    Fvector w = pos_copy.sub(this->origin);

    float a = u.dotproduct(u);
    float b = u.dotproduct(v);
    float c = v.dotproduct(v);
    float d = u.dotproduct(w);
    float e = v.dotproduct(w);
    float D = a * c - b * b;
    float sc, sN, sD = D; // sc = sN / sD, default sD = D >= 0
    float tc, tN, tD = D; // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
    if (fis_zero(D))
    { // the lines are almost parallel
        sN = 0.0; // force using point P0 on segment S1
        sD = 1.0; // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else
    { // get the closest points on the infinite lines
        sN = (b * e - c * d);
        tN = (a * e - b * d);
        if (sN < 0.0)
        { // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD)
        { // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0)
    { // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else
        {
            sN = -d;
            sD = a;
        }
    }
    else if (tN > tD)
    { // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else
        {
            sN = (-d + b);
            sD = a;
        }
    }

    if (fis_zero(sN))
        sc = 0.0f;
    else
        sc = sN / sD;

    if (fis_zero(tN))
        tc = 0.0f;
    else
        tc = tN / tD;

    u.mul(sc);
    v.mul(tc);
    u.sub(v);
    w.add(u);

    return sqrt(w.x * w.x + w.y * w.y + w.z * w.z);
}

bool SDK_GizmoMovePlane::RayPick(const Fvector& p, const Fvector& d)
{
    float tmin = (this->points[0].x - p.x) / d.x;
    float tmax = (this->points[2].x - p.x) / d.x;

    if (tmin > tmax)
        std::swap(tmin, tmax);

    float tymin = (this->points[0].y - p.y) / d.y;
    float tymax = (this->points[2].y - p.y) / d.y;

    if (tymin > tymax)
        std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (this->points[0].z - p.z) / d.z;
    float tzmax = (this->points[2].z - p.z) / d.z;

    if (tzmin > tzmax)
        std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return true;
}
