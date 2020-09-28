#pragma once
namespace Cordis
{
namespace Scripts
{
class Script_StateAnimation
{
public:
    Script_StateAnimation(void) = delete;
    Script_StateAnimation(CScriptGameObject* const p_client_object, Script_StateManager& state_manager, const bool is_animlist_access);
    ~Script_StateAnimation(void);

    void set_state(const xr_string& new_state_name = "", const bool is_fast_set = true);
    void set_control(void);
    void update_anim(void);
    std::pair<xr_vector<StateManagerAnimationData::AnimationData>, StateManagerAnimationData> select_animation(
        void);
    int weapon_slot(void);
    void animation_callback(const bool is_skip_multianim_check);
    void process_special_action(StateManagerAnimationData::AnimationData& animation_data);
    void add_animation(const xr_vector<StateManagerAnimationData::AnimationData>& animations, const StateManagerAnimationData& state);
    xr_vector<StateManagerAnimationData::AnimationData> anim_for_slot(
        int& weapon_slot_id, const xr_map<std::uint32_t, xr_vector<StateManagerAnimationData::AnimationData>>& state);
    StateManagerAnimationData::AnimationData select_rnd(
        const xr_map<std::uint32_t, xr_vector<StateManagerAnimationData::AnimationData>>& state,
        int& weapon_slot_id, const bool is_must_play);

    inline bool isUsingAnimationList(void) const noexcept { return this->m_is_using_animation_list; }
    inline CScriptGameObject* const getClientObject(void) const noexcept { return this->m_p_client_object; }
    inline const StateManagerAnimationStates& getStates(void) const noexcept { return this->m_states; }
    inline void setStatesAnimationMarker(const std::uint8_t value) { this->m_states.setAnimationMarker(value); }

private:
    // @ false -> animstate | true -> animation
    bool m_is_using_animation_list;
    bool m_is_control_called;
    CScriptGameObject* m_p_client_object;
    StateManagerAnimationStates m_states;
    Script_StateManager* m_p_state_manager;
    xr_map<xr_string, StateManagerAnimationData> m_current_data_list; // if is_animlist_access is true => Using Script_GlobalHelper::getStateManagerAnimationList else Script_GlobalHelper::getStateManagerAnimStateList
};
} // namespace Scripts
} // namespace Cordis
