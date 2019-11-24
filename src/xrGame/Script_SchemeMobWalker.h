#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeMobWalker : public Script_ISchemeMonster
{
    using inherited_scheme = Script_ISchemeMonster;

public:
    Script_SchemeMobWalker(void) = delete;
    Script_SchemeMobWalker(CScriptGameObject* p_client_object, DataBase::Storage_Scheme& storage);
    ~Script_SchemeMobWalker(void);

    virtual void reset_scheme(void);
    virtual void update(const float delta);

    virtual inline bool is_arrived_to_first_waypoint(void)
    {
        // Lord: доделать!
        return false;
    }

private:
    bool m_state;
    bool m_is_crouch;
    bool m_is_running;
    std::uint16_t m_patrol_wait_time;
    std::uint32_t m_current_animation_set;
    std::uint32_t m_last_look_index;
    std::uint32_t m_last_index;
    CondlistWaypoints m_path_walk_info;
    CondlistWaypoints m_path_look_info;
    xr_string m_sheduled_sound_name;
    CPatrolPathParams* m_patrol_walk;
    CPatrolPathParams* m_patrol_look;
    
};
} // namespace Scripts
} // namespace Cordis
