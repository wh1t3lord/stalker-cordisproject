#include "stdafx.h"
#include "SDK_ObjectShape.h"
#include "SDK_SceneManager.h"
#include "../xrRender/du_box.h"

namespace Cordis
{
namespace SDK
{
SDK_ObjectShape::SDK_ObjectShape(const xr_string& name)
    : inherited(name.c_str()), m_shape_type(NULL), m_shape_geometry_type(kShapeType_NonSelected)
{
    this->m_id_objecttype = kSection_Shapes;
    this->m_box.invalidate();
}

SDK_ObjectShape::~SDK_ObjectShape(void) {}

void SDK_ObjectShape::ComputeBounds(void)
{
    this->m_box.invalidate();

 
        switch (this->m_shape_geometry_type)
        {
        case kShapeType_Sphere:
        {
            Fsphere& transform = this->m_sphere_data;
            Fvector point;
            point.set(transform.P);
            point.sub(transform.R);
            this->m_box.modify(point);
            point.set(transform.P);
            point.add(transform.R);
            this->m_box.modify(point);
            break;
        }
        case kShapeType_Box:
        {
            Fvector point;
            Fmatrix& transform = this->m_box_data;

            for (int i = 0; i < DU_BOX_NUMVERTEX; i++)
            {
                transform.transform_tiny(point, du_box_vertices[i].p);
                this->m_box.modify(point);
            }
            break;
        }

        //   this->m_box.getsphere(this->m_sphere.P, this->m_sphere.R);
    }
}

void SDK_ObjectShape::Render(const int&, const bool&)
{
    switch (this->m_shape_geometry_type)
    {
    case kShapeType_Box:
    {
        if (this->m_is_updatetransform)
        {
            this->UpdateTransform();
            this->ComputeBounds();
        }

        DUImpl.DrawIdentBox(this->GetTransform());
        break;
    }
    case kShapeType_Sphere:
    {
        if (this->m_is_updatetransform)
        {
            this->UpdateTransform();
            this->ComputeBounds();
        }
        DUImpl.DrawIdentSphere(this->GetTransform());
        break;
    }
    }

    this->m_is_updatetransform = false;
}

void SDK_ObjectShape::Scale(const Fvector& value) {}

bool SDK_ObjectShape::RayPick(float& distance, const Fvector& start, const Fvector& direction)
{ 
        float dist = distance;

 
    switch (this->m_shape_geometry_type)
    {
        case kShapeType_Sphere:
        {
            Fvector S, D;
            Fmatrix M;
            M.invert(this->m_transform_rotation);
            M.transform_dir(D, direction);
            this->m_invert_transform.transform_tiny(S, start);
            Fsphere& T = this->m_sphere_data;
            float bk_r = T.R;
            //            T.R					= FScale.x;
            T.intersect(S, D, dist);
            if (dist <= 0.f)
                dist = distance;

            T.R = bk_r;
        }
        break;
        case kShapeType_Box:
        {
            Fbox box;
            box.identity();
            Fmatrix BI;
            BI.invert(this->m_box_data);
            Fvector S, D, S1, D1, P;
            this->m_invert_transform.transform_tiny(S, start);
            this->m_invert_transform.transform_dir(D, direction);
            BI.transform_tiny(S1, S);
            BI.transform_dir(D1, D);
            Fbox::ERP_Result rp_res = box.Pick2(S1, D1, P);
            if (rp_res == Fbox::rpOriginOutside)
            {
                this->m_box_data.transform_tiny(P);
                this->m_transform.transform_tiny(P);
                P.sub(start);
                dist = P.magnitude();
            }
        }
        break;
    }
    
    if (dist < distance)
    {
        distance = dist;
        return true;
    }
    return false;
}

bool SDK_ObjectShape::FrustumPick(const CFrustum& frustum)
{
    switch (this->m_shape_geometry_type)
    {
    case kShapeType_Box:
    { 
        Fbox box;
        box.identity();
        Fmatrix data = this->m_box_data;
        data.mulA_43(this->GetTransform());
        box.xform(data);
        unsigned int mask = 0xff;

        if (frustum.testAABB(box.data(), mask))
            return true;

        break;
    }
    case kShapeType_Sphere:
    { 
        Fvector center;
        this->GetTransform().transform_tiny(center, this->m_sphere_data.P);

        if (frustum.testSphere_dirty(center, this->m_sphere_data.R * this->GetScale().x))
            return true;

        break;
    }
    }

    return false;
}

} // namespace SDK
} // namespace Cordis
