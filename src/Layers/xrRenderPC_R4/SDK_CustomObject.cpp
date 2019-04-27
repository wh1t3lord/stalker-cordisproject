#include "stdafx.h"
#include "SDK_CustomObject.h"
#include "SDKUI_Helpers.h"
#include "EditorPref.h"
#include "xrCore/Animation/Envelope.hpp"

#define CUSTOMOBJECT_CHUNK_PARAMS 0xF900
#define CUSTOMOBJECT_CHUNK_LOCK 0xF902
#define CUSTOMOBJECT_CHUNK_TRANSFORM 0xF903
#define CUSTOMOBJECT_CHUNK_GROUP 0xF904
#define CUSTOMOBJECT_CHUNK_MOTION 0xF905
#define CUSTOMOBJECT_CHUNK_FLAGS 0xF906
#define CUSTOMOBJECT_CHUNK_NAME 0xF907
#define CUSTOMOBJECT_CHUNK_MOTION_PARAM 0xF908

SDK_CustomObject::SDK_CustomObject(LPCSTR name)
{
    this->bSelected = false;
    this->bRendering = false;
    this->bVisible = false;
    this->vPosition.set(0, 0, 0);
    this->vRotation.set(0, 0, 0);
    this->vScale.set(1, 1, 1);
    this->ObjectType = OBJECT_CLASS_NULL;
    this->bUpdateTransform = false;
    this->mTransform.identity();
    this->mTransformRP.identity();
    this->mITransform.identity();
    this->mITransformRP.identity();
    this->SceneName = name;
}

SDK_CustomObject::~SDK_CustomObject(void) {}

// void SDK_CustomObject::Move(const Fvector& v)
// {
//     this->vPosition.add(v);
// }

void SDK_CustomObject::MoveTo(const Fvector& pos, const Fvector& up) // @ Lord: метод не реализован!
{
//    ASSERT(TEXT("It's implement now!"));
    this->vPosition.set(pos);
    this->bUpdateTransform = true;
    // @ Lord: Реализовать поддержку etfNormalAlign
}

void SDK_CustomObject::RotateParent(const Fvector& v, const float& angle) { this->vRotation.mad(v, angle); }

void SDK_CustomObject::RotateLocal(const Fvector& v, const float& angle)
{
    Fmatrix m;
    Fvector r;
    m.rotation(v, angle);
    this->mTransformRP.mulB_43(m);
    this->mTransformRP.getXYZ(r);
    this->vRotation = r;
    this->bUpdateTransform = true;
}

void SDK_CustomObject::Scale(const Fvector& v)
{
    this->vScale.add(v);

    if (this->vScale.x < EPS)
        this->vScale.x = EPS;

    if (this->vScale.y < EPS)
        this->vScale.y = EPS;

    if (this->vScale.z < EPS)
        this->vScale.z = EPS;

    this->bUpdateTransform = true;
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

    this->bUpdateTransform = true;
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

    this->bUpdateTransform = true;
}
