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
    virtual void waypoint_callback(
        CScriptGameObject* p_client_object, const std::uint32_t action_movement_type, const std::uint32_t index);
    virtual void update_movement_state(void);
    virtual void update_standing_state(void);
    virtual void deactivate(void);
    virtual void look_at_waypoint(const std::uint32_t point_index);

private:
    bool m_state;
    bool m_is_crouch;
    bool m_is_running;
    std::uint32_t m_point_wait_time;
    std::uint32_t m_current_animation_set;
    std::uint32_t m_last_look_index;
    std::uint32_t m_last_index;
    CondlistWaypoints m_path_walk_info;
    CondlistWaypoints m_path_look_info;
    xr_string m_sheduled_sound_name;
    CPatrolPathParams* m_patrol_walk;
    CPatrolPathParams* m_patrol_look;
    
};

// Lord: если не будет работать при const CScriptIniFile то убрать его, но у нас это в любом случае поинтер!
inline void add_to_binder(CScriptGameObject* p_client_object, const CScriptIniFile* ini, const xr_string& scheme_name,
    const xr_string& section_name, const DataBase::Storage_Scheme& storage)
{
    // Lord: доделать 
    // Lord: проследить за корректным удалением схем!!!
    Script_ISchemeEntity* action = new Script_SchemeMobWalker(p_client_object, storage);
    
}

} // namespace Scripts
} // namespace Cordis
