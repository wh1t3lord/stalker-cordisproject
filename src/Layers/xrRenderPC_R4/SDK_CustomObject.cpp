#include "stdafx.h"
#include "SDK_CustomObject.h"
#include "SDKUI_Helpers.h"
#include "EditorPref.h"
#include "xrCore/Animation/Envelope.hpp"


namespace Cordis
{
    namespace SDK
    {
#define CUSTOMOBJECT_CHUNK_PARAMS 0xF900
#define CUSTOMOBJECT_CHUNK_LOCK 0xF902
#define CUSTOMOBJECT_CHUNK_TRANSFORM 0xF903
#define CUSTOMOBJECT_CHUNK_GROUP 0xF904
#define CUSTOMOBJECT_CHUNK_MOTION 0xF905
#define CUSTOMOBJECT_CHUNK_FLAGS 0xF906
#define CUSTOMOBJECT_CHUNK_NAME 0xF907
#define CUSTOMOBJECT_CHUNK_MOTION_PARAM 0xF908


    SDK_CustomObject::SDK_CustomObject(LPCSTR name)
        : m_is_selected(false), m_is_rendering(false), m_is_visible(false), m_position({0, 0, 0}),
          m_rotation({0, 0, 0}), m_scale({1, 1, 1}), m_id_objecttype(kSection_NULL), m_is_updatetransform(false),
          m_scene_name(name), m_reference_name("")
    {
        this->m_transform.identity();
        this->m_transform_rotation_position.identity();
        this->m_invert_transform.identity();
        this->m_invert_transform_rotation_position.identity();
    }

    SDK_CustomObject::~SDK_CustomObject(void) {}

    // void SDK_CustomObject::Move(const Fvector& v)
    // {
    //     this->vPosition.add(v);
    // }

    void SDK_CustomObject::MoveTo(const Fvector& position, const Fvector& up) // @ Lord: метод не реализован!
    {
        //    ASSERT(TEXT("It's implement now!"));
        this->m_position.set(position);
        this->m_is_updatetransform = true;
        // @ Lord: Реализовать поддержку etfNormalAlign
    }

    void SDK_CustomObject::RotateParent(const Fvector& v, const float& angle) { this->m_rotation.mad(v, angle); }

    void SDK_CustomObject::RotateLocal(const Fvector& v, const float& angle)
    {
        Fmatrix m;
        Fvector r;
        m.rotation(v, angle);
        this->m_transform_rotation_position.mulB_43(m);
        this->m_transform_rotation_position.getXYZ(r);
        this->m_rotation = r;
        this->m_is_updatetransform = true;
    }

    void SDK_CustomObject::Scale(const Fvector& v)
    {
        this->m_scale.add(v);

        if (this->m_scale.x < EPS)
            this->m_scale.x = EPS;

        if (this->m_scale.y < EPS)
            this->m_scale.y = EPS;

        if (this->m_scale.z < EPS)
            this->m_scale.z = EPS;

        this->m_is_updatetransform = true;
    }

    void SDK_CustomObject::NormalAlign(Fvector& rot, const Fvector& up, const Fvector& dir)
    {
        Fmatrix mR;
        Fvector vR, vD, vN;
        vN.set(up);
        //   vD.set(0,0,1);
        vD.set(dir);
        if (fabsf(vN.z) > 0.99f)
            vD.set(1, 0, 0);
        vR.crossproduct(vN, vD);
        vR.normalize();
        vD.crossproduct(vR, vN);
        vD.normalize();
        mR.set(vR, vN, vD, vR);
        mR.getXYZ(rot);

        this->m_is_updatetransform = true;
    }

    void SDK_CustomObject::SnapMove(Fvector& pos, Fvector& rot, const Fmatrix& rotRP, const Fvector& amount)
    {
        SRayPickInfo pinf;
        Fvector up, dn = {0, -1, 0};
        rotRP.transform_dir(dn);
        up.invert(dn);
        Fvector s2, s1 = pos;
        s1.add(amount);
        s2.mad(s1, up, EditorPref::GetInstance().snap_moveto);

        pinf.inf.range = EditorPref::GetInstance().snap_moveto;
        // Lord: дореализовать
        /*
            if (Scene->RayPickObject(pinf.inf.range, s1, dn, OBJCLASS_SCENEOBJECT, &pinf, Scene->GetSnapList(false)) ||
                Scene->RayPickObject(pinf.inf.range, s2, dn, OBJCLASS_SCENEOBJECT, &pinf, Scene->GetSnapList(false)))
            {
                pos.set(pinf.pt);
                if (Tools->GetSettings(etfNormalAlign))
                {
                    Fvector verts[3];
                    pinf.e_obj->GetFaceWorld(pinf.s_obj->_Transform(), pinf.e_mesh, pinf.inf.id, verts);
                    Fvector vR, vD, vN;
                    vN.mknormal(verts[0], verts[1], verts[2]);

                    vD.set(rotRP.k);
                    vR.crossproduct(vN, vD);
                    vR.normalize();
                    vD.crossproduct(vR, vN);
                    vD.normalize();

                    Fmatrix M;
                    M.set(vR, vN, vD, vR);
                    M.getXYZ(rot);
                }
            }
            else
                pos.add(amount);*/

        this->m_is_updatetransform = true;
    }

    }
}
