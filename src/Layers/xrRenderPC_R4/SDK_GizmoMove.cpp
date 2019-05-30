#include "stdafx.h"
#include "SDK_GizmoMove.h"
#include "SDKUI_Helpers.h"
#include "FreeMagic/Source/Intersection3D/MgcIntr3DLinBox.h"
#include "SDK_GizmoManager.h"

SDK_GizmoMove GizmoMove[3] = {SDK_GizmoMove(GIZMO_X), SDK_GizmoMove(GIZMO_Y), SDK_GizmoMove(GIZMO_Z)};
SDK_GizmoMovePlane GizmoMovePlanes[3] = {
    SDK_GizmoMovePlane(GIZMO_PLANE_XY), SDK_GizmoMovePlane(GIZMO_PLANE_ZY), SDK_GizmoMovePlane(GIZMO_PLANE_XZ)};

bool SDK_GizmoMove::GetPoint(const Fvector& pos, const Fvector& dir, Fvector& pB)
{
    pB.set(0, 0, 0);
    Fvector dir_copy = dir;
    Fvector end_point = dir_copy.mul(SDK_Camera::GetInstance().fFar).add(pos);

    Fvector p13, p43, p21;
    float d1343, d4321, d1321, d4343, d2121;
    float numer, denom;

    p13.x = pos.x - this->m_origin_point.x;
    p13.y = pos.y - this->m_origin_point.y;
    p13.z = pos.z - this->m_origin_point.z;
    p43.x = this->m_end_point.x - this->m_origin_point.x;
    p43.y = this->m_end_point.y - this->m_origin_point.y;
    p43.z = this->m_end_point.z - this->m_origin_point.z;

    if (fis_zero(p43.x) && fis_zero(p43.y) && fis_zero(p43.z))
        return false;

    p21.x = end_point.x - pos.x;
    p21.y = end_point.y - pos.y;
    p21.z = end_point.z - pos.z;
    //  if (ABS(p21.x) < EPS && ABS(p21.y) < EPS && ABS(p21.z) < EPS)
    if (fis_zero(p21.x) && fis_zero(p21.y) && fis_zero(p21.z))
        return false;

    d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
    d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
    d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
    d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
    d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

    denom = d2121 * d4343 - d4321 * d4321;

    if (fis_zero(denom))
        return false;

    numer = d1343 * d4321 - d1321 * d4343;
    float mua = 0.0f;
    float mub = 0.0f;

    mua = numer / denom;
    mub = (d1343 + d4321 * (mua)) / d4343;
    /*
     pA.x = pos.x + mua * p21.x;
     pA.y = pos.y + mua * p21.y;
     pA.z = pos.z + mua * p21.z;*/
    pB.x = this->m_origin_point.x + mub * p43.x;
    pB.y = this->m_origin_point.y + mub * p43.y;
    pB.z = this->m_origin_point.z + mub * p43.z;

    return true;
}

float SDK_GizmoMove::RayPick(const Fvector& pos, const Fvector& dir)
{
    Fvector dir_copy = dir;
    Fvector end_point = dir_copy.mul(SDK_Camera::GetInstance().fFar);
    end_point.add(pos);
    Fvector p2_copy = this->m_end_point;
    Fvector pos_copy = pos;
    Fvector u = end_point.sub(pos);
    Fvector v = p2_copy.sub(this->m_origin_point);
    Fvector w = pos_copy.sub(this->m_origin_point);

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

bool SDK_GizmoMovePlane::GetPoint(const Fvector& p, const Fvector& d)
{
    /*
        Fvector d_ = d;
        Fvector p_ = p;
        Fvector end_point = d_.mul(SDK_Camera::GetInstance().fFar).add(p);
        Fvector u = end_point.sub(p);
        Fvector w = p_.sub(this->points[0]);

        float D = this->GetNormalOfPlane().dotproduct(u);
        float N = -this->GetNormalOfPlane().dotproduct(w);

        if (fis_zero(D))
        { // segment is parallel to plane
            if (fis_zero(N)) // segment lies in plane
                return false;
            else
                return false; // no intersection
        }
        // they are not parallel
        // compute intersect param
        float sI = N / D;
        if (sI < 0 || sI > 1)
            return false; // no intersection

      //  *I = S.P0 + sI * u; // compute segment intersect point
        SDK_GizmoManager::GetInstance().ray_end_point_plane = u.mul(sI).add(p);
        return true;*/
    Fvector normal = this->GetNormalOfPlane();
    float denom = normal.dotproduct(d);

    if (!fis_zero(denom))
    {
        Fvector p_ = this->m_points[0];
        Fvector bb = p_.sub(p);
        float t = normal.dotproduct(bb) / denom;

        if (t >= 0)
        {
            Fvector d_ = d;
            SDK_GizmoManager::GetInstance().m_ray_end_point_plane = d_.mul(t).add(p);
            return true;
        }

        return false;
    }

    return false;
}

bool SDK_GizmoMovePlane::RayPick(const Fvector& p, const Fvector& d)
{
    float tmin = (this->m_points[0].x - p.x) / d.x;
    float tmax = (this->m_points[2].x - p.x) / d.x;

    if (tmin > tmax)
        std::swap(tmin, tmax);

    float tymin = (this->m_points[0].y - p.y) / d.y;
    float tymax = (this->m_points[2].y - p.y) / d.y;

    if (tymin > tymax)
        std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (this->m_points[0].z - p.z) / d.z;
    float tzmax = (this->m_points[2].z - p.z) / d.z;

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

Fvector SDK_GizmoMovePlane::GetNormalOfPlane(void)
{
    Fvector p3 = this->m_points[3];
    Fvector p1 = this->m_points[1];
    Fvector a = p3.sub(this->m_points[0]);
    Fvector b = p1.sub(this->m_points[0]);
    Fvector c = {0,0,0};
    return c.crossproduct(b,a);
}
