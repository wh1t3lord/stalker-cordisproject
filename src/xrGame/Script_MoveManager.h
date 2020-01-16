#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_MoveManager
{
public:
    Script_MoveManager(void) = delete;
    Script_MoveManager(CScriptGameObject* const p_client_object);
    ~Script_MoveManager(void);

    void validate_paths(void);
    void extrapolate_callback(CScriptGameObject* const p_client_object);
    bool standing_on_terminal_waypoint(std::uint32_t& return_index);
    void Continue(void);
    void setup_movement_by_patrol_path(void);
    bool is_arrived_to_first_waypoint(void);
    bool is_synchronization_succesful(void);
    void update(void);
    void finalize(CScriptGameObject* const p_client_object);
    void update_movement_state(void);
    void update_standing_state(const Fvector& look_position);
    void time_callback(void);
    void scheme_set_signal(const xr_string& signal_name);
    void turn_end_callback(void);
    void waypoint_callback(
        CScriptGameObject* p_client_object, const std::uint32_t action_type_movement, const std::uint32_t point_index);
    void reset(const xr_string& path_walk_name, const CondlistWaypoints& path_walk_info,
        const xr_string& path_look_name, const CondlistWaypoints& path_look_info, const xr_string& team_name,
        const xr_map<xr_string, xr_string>& suggested_state,
        std::function<bool(std::uint32_t, std::uint32_t)>& move_callback_info, const bool is_no_validation);
    
    inline bool is_at_terminal_waypoint(void) const noexcept { return this->m_is_at_terminal_waypoint; }

private:
    bool m_is_at_terminal_waypoint;
    bool m_is_no_validation;
    bool m_is_can_use_get_current_point_index;
    std::uint32_t m_point_wait_time;
    std::uint32_t m_current_point_initialization_time;
    std::uint32_t m_current_point_index;
    std::uint32_t m_state;
    std::uint32_t m_last_index;
    std::uint32_t m_synchronization_signal_set_tm;
    std::uint32_t m_keep_state_until;
    std::uint32_t m_walk_until;
    std::uint32_t m_run_until;
    std::uint32_t m_pt_wait_time;
    std::uint32_t m_last_look_index;
    std::uint32_t m_retval_after_rotation;
    CPatrolPathParams* m_p_patrol_walk;
    CPatrolPathParams* m_p_patrol_look;
    CScriptGameObject* m_p_client_object;
    xr_map<std::uint32_t, CondlistData> m_default_state_standing_condlist;
    xr_map<std::uint32_t, CondlistData> m_default_state_moving1_condlist;
    xr_map<std::uint32_t, CondlistData> m_default_state_moving2_condlist;
    xr_map<std::uint32_t, CondlistData> m_default_state_moving3_condlist;
    xr_map<xr_string, xr_string> m_suggested_state;
    xr_string m_path_walk_name;
    xr_string m_path_look_name;
    xr_string m_team_name;
    xr_string m_synchronization_signal_name;
    xr_string m_current_state_moving_name;
    xr_string m_current_state_standing_name;
    std::function<bool(std::uint32_t, std::uint32_t)> m_move_callback_info;
    CondlistWaypoints m_path_walk_info;
    CondlistWaypoints m_path_look_info;
};
} // namespace Scripts
} // namespace Cordis
