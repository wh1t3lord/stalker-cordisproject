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
