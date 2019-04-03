#pragma once

#define CHUNK_OBJECT_BODY 0x7777

struct SExportStreamItem
{
    int chunk;
    CMemoryWriter stream;

    SExportStreamItem() : chunk(0) {}
};

struct SExportStreams
{
    SExportStreamItem spawn;
    SExportStreamItem spawn_rs;
    SExportStreamItem patrolpath;
    SExportStreamItem rpoint;
    SExportStreamItem sound_static;
    SExportStreamItem sound_env_geom;
    SExportStreamItem pe_static;
    SExportStreamItem envmodif;
    SExportStreamItem fog_vol;
};

// @ Lord: всё время обновлять!
enum SceneObjectType
{
    OBJECT_CLASS_NULL,
    OBJECT_CLASS_STATIC_GEOMETRY,
    OBJECT_CLASS_SPAWN_ELEMENT,
    OBJECT_CLASS_SECTOR,
    OBJECT_CLASS_PARTICLE,
    OBJECT_CLASS_LIGHT,
    OBJECT_CLASS_SOUND_SOURCE,
    OBJECT_CLASS_SOUND_ENV,
    OBJECT_CLASS_DETAIL_OBJECTS,
    OBJECT_CLASS_GROUP,
    OBJECT_CLASS_AI_MAP,
    OBJECT_CLASS_WALLMARKS,
    OBJECT_CLASS_FOG_VOLUMES
};

class SDK_CustomObject
{
public:
    SDK_CustomObject(void) = delete;
    SDK_CustomObject(LPVOID data, LPCSTR name);
    virtual ~SDK_CustomObject(void);

    inline const Fmatrix& GetTransform(void) const { return this->mTransform; }
    inline const Fvector& GetPosition(void) const { return this->vPosition; }
    inline const Fvector& GetRotation(void) const { return this->vRotation; }
    inline const Fvector& GetScale(void) const { return this->vScale; }
    inline void SetPosition(const Fvector& pos) { this->vPosition = pos; }
    inline void SetRotation(const Fvector& rot)
    {
        this->vRotation = rot;
        VERIFY(_valid(this->vRotation));
    }
    inline void SetScale(const Fvector& scl) { this->vScale = scl; }

    inline bool IsVisible(void) const { return this->bVisible; }
    inline void Show(void)
    {
        if (!this->bVisible)
            this->bVisible = true;
    }

    inline void Hide(void)
    {
        if (this->bVisible)
            this->bVisible = false;
    }

    inline bool IsRender(void) const { return this->bRendering; }
    inline bool IsSelected(void) const { return this->bSelected; }

    void UpdateTransform(void)
    {
        this->mTransformR.setXYZi(-this->vRotation.x, -this->vRotation.y, -this->vRotation.z);
        this->mTransformS.scale(this->vScale);
        this->mTransformP.translate(this->vPosition);
        this->mTransformRP.mul(mTransformP, mTransformR);
        mTransform.mul(mTransformRP, mTransformS);
        mITransformRP.invert(mTransformRP);
        mITransform.invert(mTransform);
    }

    inline void Move(const Fvector& v)
    {
        this->vPosition.add(v);
        this->bUpdateTransform = true;
    }
    void MoveTo(const Fvector& pos, const Fvector& up);
    void RotateParent(const Fvector& axis, const float& angle);
    void RotateLocal(const Fvector& axis, const float& angle);
    void Scale(const Fvector& v);
    void NormalAlign(Fvector& rot, const Fvector& up, const Fvector& dir);
    void SnapMove(Fvector& pos, Fvector& rot, const Fmatrix& rotRP, const Fvector& amount);

    // Lord: это не используем
    //     enum
    //     {
    //         flSelected_notused = (1 << 0),
    //         flVisible_notused = (1 << 1),
    //         flLocked_notused = (1 << 2),
    //         flMotion = (1 << 3),
    //         flRenderAnyWayIfSelected = (1 << 4),
    //         flObjectInGroup = (1 << 5),
    //         flObjectInGroupUnique = (1 << 6),
    //
    //         flAutoKey = (1 << 30),
    //         flCameraView = (1 << 31),
    //     };
    //
    // private:
    //     void AnimationOnFrame(void);
    //     void AnimationDrawPath(void);
    //     void AnimationCreateKey(const float& t);
    //     inline void AnimationDeleteKey(const float& t) { this->m_Motion->DeleteKey(t); }
    //     void AnimationUpdate(const float& t);
    //     void OnMotionChange(void);

    SceneObjectType GetType(void) const { return this->ObjectType; }
    virtual void Render(const int&, const bool&) = 0;
    virtual void DrawPreferences(void) = 0;

private : bool bSelected;
    bool bVisible;
    bool bRendering;

protected:
    bool bUpdateTransform;
    SceneObjectType ObjectType;

private:
    Fvector vPosition;
    Fvector vScale;
    Fvector vRotation;

protected:
    xr_string SceneName;

private:
    Fmatrix mTransformP;
    Fmatrix mTransformR;
    Fmatrix mTransformS;
    Fmatrix mTransformRP;
    Fmatrix mTransform; // @ main

    // @ Invert
    Fmatrix mITransform;
    Fmatrix mITransformRP;
};
