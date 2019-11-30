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

    virtual void reset_scheme(const bool);
    virtual void update(const float delta);
    virtual void waypoint_callback(
        CScriptGameObject* p_client_object, const std::uint32_t action_movement_type, const std::uint32_t index);
    virtual void update_movement_state(void);
    virtual void update_standing_state(void);
    virtual void deactivate(void);
    virtual void look_at_waypoint(const std::uint32_t point_index);

    // Lord: если не будет работать при const CScriptIniFile то убрать его, но у нас это в любом случае поинтер!
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





} // namespace Scripts
} // namespace Cordis
