#pragma once

namespace Cordis
{
namespace Scripts
{

class Script_SchemeMobCamp : public Script_ISchemeMonster
{
    using inherited_scheme = Script_ISchemeMonster;

public:
    Script_SchemeMobCamp(void) = delete;
    Script_SchemeMobCamp(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage);
    ~Script_SchemeMobCamp(void);

    virtual void reset_scheme(const bool, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);
    virtual void deactivate(CScriptGameObject* const p_client_object);
    virtual void net_destroy(CScriptGameObject* const p_client_object);

    enum : std::uint32_t
    {
        STATE_CAMP = 1,
        STATE_ALIFE, 
        STATE_MOVE_HOME
    };

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

        Script_ISchemeEntity* action = new Script_SchemeMobWalker(p_client_object, storage);
        DataBase::Storage::getInstance().setStorageSchemesActions(
            p_client_object->ID(), action->getSchemeName(), action);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

private:
    void execute_state(void);
    void select_state(void);
    void select_current_home_point(const bool is_first_call);

private:
    bool m_is_previous_enemy;
    std::uint32_t m_current_point_index;
    std::uint32_t m_time_point_changed;
    std::uint32_t m_camp_node; // @ level_vertex_id of this->m_npc
    std::uint32_t m_state_current;
    std::uint32_t m_state_previous;
    Fvector m_camp_position;
    CPatrolPathParams* m_path_look;
    CPatrolPathParams* m_path_home;
};

}
} // namespace Cordis

