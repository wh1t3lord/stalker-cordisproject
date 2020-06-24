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
    Script_SchemeMobWalker(CScriptGameObject* p_client_object, DataBase::Script_ComponentScheme_MobWalker* storage);
    ~Script_SchemeMobWalker(void);

    virtual void reset_scheme(const bool, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);
    virtual void waypoint_callback(
        CScriptGameObject* p_client_object, const std::uint32_t action_movement_type, const std::uint32_t index);
    virtual void update_movement_state(void);
    virtual void update_standing_state(void);
    virtual void deactivate(CScriptGameObject* const p_client_object);
    virtual void look_at_waypoint(const std::uint32_t point_index);

        // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
            const xr_string& scheme_name, const xr_string& section_name, void* storage)
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

       MESSAGEI("added scheme to binder, name=%s scheme=%s section=%s", p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        Script_ISchemeEntity* action = new Script_SchemeMobWalker(p_client_object, reinterpret_cast<DataBase::Script_ComponentScheme_MobWalker*>(storage));
        DataBase::Storage::getInstance().setStorageSchemesActions(
            p_client_object->ID(), action->getSchemeName(), action);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

private:
    bool m_state;
    bool m_is_crouch;
    bool m_is_running;
    std::uint32_t m_point_wait_time;
    std::uint32_t m_current_animation_set;
    std::uint32_t m_last_look_index;
    std::uint32_t m_last_index;
    DataBase::Script_ComponentScheme_MobWalker* m_p_storage;
	CPatrolPathParams* m_patrol_walk;
	CPatrolPathParams* m_patrol_look;
    CondlistWaypoints m_path_walk_info;
    CondlistWaypoints m_path_look_info;
    xr_string m_sheduled_sound_name;
};





} // namespace Scripts
} // namespace Cordis
