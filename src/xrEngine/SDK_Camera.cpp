#include "stdafx.h"
#include "SDK_Camera.h"
#include "../xrCore/_quaternion.h"
void SDK_Camera::Initialize(void)
{
    bClampRoll = false;
    bClampYaw = false;
    bClampPitch = false;
    f_fov = 55;
    f_aspect = 0.5f;
    lim_yaw.set(0, 0);
    lim_pitch.set(-1.5, 1.5);
    yaw, pitch, roll = 0;
    //   vPosition.set(0, 0, 0);
    Device.vCameraPosition.set(0, 5.0f, 0);
    vDirection.set(0, 0, 1);
    vNormal.set(0, 1, 0);
    fMoveSpeed = 10.0f;
    fNear = 0.2f;
    fFar = 1000.f;
    vRight = {0, 0, 0};
    fSens = 0.002f;

 

    Device.seqFrame.Add(this, REG_PRIORITY_HIGH);
}

void SDK_Camera::Rotate(int cmd, float val)
{
    if (true)
    {
        while (pitch < lim_pitch[0])
            pitch += PI_MUL_2;
        while (pitch > lim_pitch[1])
            pitch -= PI_MUL_2;
    };

    switch (cmd)
    {
    case 3: pitch -= val; break;
    case 2: pitch += val; break;
    case 0: yaw -= val; break;
    case 1: yaw += val; break;
    }

    clamp(pitch, lim_pitch[0], lim_pitch[1]);
}

void SDK_Camera::MoveForward(float val)
{
    if (val == 0)
        return;
    Fvector vPos = Device.vCameraPosition;
    Device.vCameraPosition.mul(vDirection, val * fMoveSpeed);
    Device.vCameraPosition.add(vPos);
}

void SDK_Camera::MoveRight(float val)
{
    if (val == 0)
        return;
    Fvector vPos = Device.vCameraPosition;
    Fvector vDir;
    vDir.crossproduct(vNormal, vDirection);
    Device.vCameraPosition.mul(vDir, val * fMoveSpeed);
    Device.vCameraPosition.add(vPos);
}

void SDK_Camera::MoveUp(float val)
{
    if (val == 0)
        return;
 
    Device.vCameraPosition.y += val;

}

void SDK_Camera::Update(void)
{
 
    Fmatrix mR;
    mR.setHPB(-yaw, -pitch, -roll);

    vDirection.set(mR.k);
    vNormal.set(mR.j);

}
