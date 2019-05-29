#pragma once

// @ Singleton impl
class ENGINE_API SDK_Camera : pureFrame
{
private:
    SDK_Camera(void) = default;

public:
    inline static SDK_Camera& GetInstance(void) noexcept
    {
        static SDK_Camera instance;
        return instance;
    }

    SDK_Camera(const SDK_Camera&) = delete;
    SDK_Camera& operator=(const SDK_Camera&) = delete;
    SDK_Camera(SDK_Camera&&) = delete;
    SDK_Camera& operator=(SDK_Camera&&) = delete;

    ~SDK_Camera(void) { Device.seqFrame.Remove(this); }

    void Initialize(void);
    void Rotate(int cmd, float val);
    void MoveForward(float val);
    void MoveRight(float val);
    void MoveUp(float val);
    void Update(void);
    void MouseRayFromPoint(Fvector&, const Ivector2&);
 //   inline constexpr Fvector GetPosition(void) const { return vPosition; }

    inline constexpr Fvector GetDirection(void) const { return vDirection; }

    inline constexpr Fvector GetNormal(void) const { return vNormal; }

    inline constexpr Fvector GetRight(void) const { return vRight; }

    inline constexpr float GetWorldYaw(void) const { return -yaw; }

    inline constexpr float GetWorldPitch(void) const { return pitch; }

    virtual void OnFrame()
    {
        Update();

        Device.mView.build_camera_dir(Device.vCameraPosition, Device.vCameraDirection, Device.vCameraTop);
        //     Device.vCameraPosition.set(GetPosition());
        Device.vCameraDirection.set(GetDirection());
        Device.vCameraTop.set(GetNormal());
        Device.vCameraRight.set(GetRight());
        Device.fFOV = f_fov;
        Device.fASPECT = f_aspect;
        Device.mProject.build_projection(deg2rad(SDK_Camera::GetInstance().f_fov), f_aspect, SDK_Camera::GetInstance().fNear, SDK_Camera::GetInstance().fFar);
    }

public:
    bool bClampYaw, bClampPitch, bClampRoll;
    float f_fov;
    float fNear, fFar;
    float fMoveSpeed;
    float f_aspect;
    float fSens;

private:
    float yaw, pitch, roll;
    Fvector2 lim_yaw, lim_pitch, lim_roll;
public:
    Fvector vPosition, vDirection, vNormal, vRight;
};
