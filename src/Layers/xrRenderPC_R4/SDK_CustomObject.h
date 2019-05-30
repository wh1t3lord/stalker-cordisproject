#pragma once

#define CHUNK_OBJECT_BODY 0x7777

struct SExportStreamItem
{
    int chunk;
    CMemoryWriter stream;

    SExportStreamItem(void) : chunk(0) {}
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
    SDK_CustomObject(LPCSTR name);
    virtual ~SDK_CustomObject(void);

    inline const Fmatrix& GetTransform(void) const noexcept { return this->m_transform; }
    inline const Fvector& GetPosition(void) const noexcept { return this->m_position; }
    inline const Fvector& GetRotation(void) const noexcept { return this->m_rotation; }
    inline const Fvector& GetScale(void) const noexcept { return this->m_scale; }
    inline void SetPosition(const Fvector& pos) { this->m_position = pos; }
    inline void SetRotation(const Fvector& rot)
    { 
        if (_valid(rot))
        {
            this->m_rotation = rot;
        }
        else
        {
            this->m_rotation = {0, 0, 0};
        }
    }
    inline void SetScale(const Fvector& scl) { this->m_scale = scl; }

    //   inline bool IsVisible(void) const noexcept { return this->bVisible; }
    inline void Show(void) noexcept
    {
        if (!this->m_is_visible)
            this->m_is_visible = true;
    }

    inline void Hide(void) noexcept
    {
        if (this->m_is_visible)
            this->m_is_visible = false;
    }

    // inline bool IsRender(void) const noexcept { return this->bRendering; }
    // inline bool IsSelected(void) const noexcept { return this->bSelected; }

    void UpdateTransform(void)
    {
        this->m_transform_rotation.setXYZi(-this->m_rotation.x, -this->m_rotation.y, -this->m_rotation.z);
        this->m_transform_scaling.scale(this->m_scale);
        this->m_transform_position.translate(this->m_position);
        this->m_transform_rotation_position.mul(this->m_transform_position, this->m_transform_rotation);
        this->m_transform.mul(this->m_transform_rotation_position, this->m_transform_scaling);
        this->m_invert_transform_rotation_position.invert(this->m_transform_rotation_position);
        this->m_invert_transform.invert(this->m_transform);
    }

    inline void Move(const Fvector& v)
    {
        this->m_position.add(v);
        this->m_is_updatetransform = true;
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

    SceneObjectType GetType(void) const noexcept { return this->m_id_objecttype; }
    xr_string GetSceneName(void) const noexcept { return this->m_scene_name; }
    inline const Fbox& GetBox(void) const noexcept { return this->m_box; }

    virtual void Render(const int&, const bool&) = 0;
    virtual void DrawPreferences(void) = 0;
    virtual bool RayPick(float&, const Fvector&, const Fvector&) = 0;

public:
    bool m_is_selected;
    bool m_is_visible;
    bool m_is_rendering;

protected:
    bool m_is_updatetransform;
    SceneObjectType m_id_objecttype;

private:
    Fvector m_position;
    Fvector m_scale;
    Fvector m_rotation;

protected:
    Fbox m_box;
    xr_string m_scene_name;

private:
    Fmatrix m_transform_position;
    Fmatrix m_transform_rotation;
    Fmatrix m_transform_scaling;
    Fmatrix m_transform_rotation_position;
    Fmatrix m_transform; // @ main

    // @ Invert
    Fmatrix m_invert_transform;
    Fmatrix m_invert_transform_rotation_position;
};
