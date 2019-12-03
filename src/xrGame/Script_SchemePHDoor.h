#pragma once

class cphysics_joint_scripted;

namespace Cordis
{
namespace Scripts
{
    // Lord: реализовать try_to_open_door и try_to_close_door когда будет xr_motivator
class Script_SchemePHDoor : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemePHDoor(void) = delete;
    Script_SchemePHDoor(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
    ~Script_SchemePHDoor(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);
    virtual void use_callback(CScriptGameObject* const p_client_object, CScriptGameObject* const p_client_who);
    virtual void hit_callback(CScriptGameObject* const p_client_object, const float amount,
        const Fvector& local_direction, CScriptGameObject* const p_client_who, const std::int16_t bone_index);

    bool open_fastcall(void);
    bool fastcall(void);
private:
    void close_door(const bool is_disable_sound);
    void open_door(const bool is_disable_sound);
    void close_action(void);
    inline bool try_switch(void) 
    {
        if (!this->m_storage->getPHDoorOnUseCondlist().empty())
        {
            if (XR_LOGIC::switch_to_section(this->m_npc, this->m_storage->getIni(),
                    XR_LOGIC::pick_section_from_condlist(DataBase::Storage::getInstance().getActor(), this->m_npc,
                        this->m_storage->getPHDoorOnUseCondlist())))
            {
                return true;
            }
        }

        return false;
    }
    inline bool is_closed(void) noexcept
    {
        if (!this->m_p_joint)
        {
            Msg("[Scripts/Script_SchemePHDoor/is_closed()] WARNING: this->m_p_joint == nullptr! Return ...");
            return false;
        }

        float angle;
        if (this->m_storage->IsPHDoorSlider())
        {
            angle = -this->m_p_joint->GetAxisAngle(0);
        }
        else
        {
            angle = this->m_p_joint->GetAxisAngle(90);
        }

        if (angle <= this->m_low_limits + 0.02f)
            return true;

        return false;
    }

    inline bool is_open(void) noexcept
    {
        if (!this->m_p_joint)
        {
            Msg("[Scripts/Script_SchemePHDoor/is_open()] WARNING: this->m_p_joint == nullptr! Return ...");
            return false;
        }

        float angle;
        if (this->m_storage->IsPHDoorSlider())
        {
            angle = -this->m_p_joint->GetAxisAngle(0);
        }
        else
        {
            angle = this->m_p_joint->GetAxisAngle(90);
        }

        if (angle >= this->m_high_limits - 0.02f)
            return true;

        return false;
    }

private:
    bool m_is_initialized;
    bool m_is_block;
    bool m_is_soundless_block;
    bool m_is_show_tips;
    float m_low_limits;
    float m_high_limits;
    cphysics_joint_scripted* m_p_joint;
};
} // namespace Scripts
} // namespace Cordis
