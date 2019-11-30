#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeHelicopterMove : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemeHelicopterMove(void) = delete;
    Script_SchemeHelicopterMove(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
    ~Script_SchemeHelicopterMove(void);

    virtual void reset_scheme(const bool is_loading);
    virtual void save(void);
    virtual void update(const float delta);
    virtual void waypoint_callback(
        CScriptGameObject* p_client_object, const std::uint32_t action_movement_type, const std::uint32_t index);

    // Lord: доделать и доделать сет_шим тоже
    // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage)
    {
        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        if (!p_ini)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        Msg("[Scripts/add_to_binder(p_client_object, p_ini, scheme_name, section_name, storage)] added "
            "Script_SchemeMobWalker scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        // Lord: доделать
        // Lord: проследить за корректным удалением схем!!!
        Script_ISchemeEntity* action = new Script_SchemeMobWalker(p_client_object, storage);
        DataBase::Storage::getInstance().setStorageSchemesActions(
            p_client_object->ID(), action->getSchemeName(), action);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static inline void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const DataBase::Storage_Scheme& storage)
    {
    }

private:
    inline void update_look_state(void)
    {
        this->m_p_manager_fly->set_block_flook(true);
        this->m_p_manager_fly->look_at_position();
    }

    inline void update_movement_state(void)
    {
        this->m_state = 0;

        if (this->m_p_patrol_move)
        {
            if (!this->m_last_index)
            {
                this->m_last_index = 0;
                this->m_next_index = 1;
            }
            else
            {
                this->m_next_index = this->m_last_index + 1;

                if (this->m_next_index >= this->m_p_patrol_move->count())
                {
                    this->m_next_index = 0;
                }
            }
        }

        if (!this->m_is_by_stop_fire_fly)
        {
            if (this->m_p_patrol_move->count() > 2)
            {
                this->m_flag_to_wp_callback = this->m_p_manager_fly->fly_on_point_with_vector(
                    this->m_p_patrol_move->point(this->m_last_index), this->m_p_patrol_move->point(this->m_next_index),
                    this->m_max_velocity, this->m_flag_to_wp_callback, false);
            }
            else
            {
                if (this->m_p_patrol_move->count() > 1)
                {
                    this->m_flag_to_wp_callback = this->m_p_manager_fly->fly_on_point_with_vector(
                        this->m_p_patrol_move->point(this->m_last_index),
                        this->m_p_patrol_move->point(this->m_next_index), this->m_max_velocity, true, true);
                }
                else
                {
                    this->m_flag_to_wp_callback = this->m_p_manager_fly->fly_on_point_with_vector(
                        this->m_p_patrol_move->point(this->m_last_index),
                        this->m_p_patrol_move->point(this->m_next_index), this->m_max_velocity, true, true);
                }
            }
        }
        else
        {
            this->m_flag_to_wp_callback = this->m_p_manager_fly->fly_on_point_with_vector(
                this->m_stop_point, this->m_stop_point, this->m_max_velocity, true, true);
            this->m_flag_to_wp_callback = true;
        }
    }

private:
    bool m_is_callback;
    bool m_is_by_stop_fire_fly;
    bool m_flag_to_wp_callback;
    std::uint8_t m_state;
    std::uint32_t m_last_index;
    std::uint32_t m_next_index;
    float m_max_velocity;
    CPatrolPathParams* m_p_patrol_move;
    CPatrolPathParams* m_p_patrol_look;
    CHelicopter* m_p_helicopter;
    Script_HelicopterFire* m_p_manager_fire;
    Script_HelicopterLook* m_p_manager_look;
    Script_HelicopterFly* m_p_manager_fly;
    Fvector m_stop_point;
    CondlistWaypoints m_patrol_move_info;
};
} // namespace Scripts
} // namespace Cordis
