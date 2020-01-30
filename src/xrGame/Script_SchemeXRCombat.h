#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_EvaluatorCheckCombat : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorCheckCombat(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
    {
    }

    ~Script_EvaluatorCheckCombat(void) {}

    virtual _value_type evaluate(void);

private:
    DataBase::Storage_Scheme* m_p_storage;
};

class Script_EvaluatorEnemy : public CScriptPropertyEvaluator
{
public:
    Script_EvaluatorEnemy(const xr_string& evaluator_name, DataBase::Storage_Scheme& storage)
        : CScriptPropertyEvaluator(nullptr, evaluator_name.c_str()), m_p_storage(&storage)
    {
    }
    ~Script_EvaluatorEnemy(void) {}

    virtual _value_type evaluate(void);

private:
    DataBase::Storage_Scheme* m_p_storage;
};

namespace XR_COMBAT
{
// @ PRIVATE uses, in XR_LOGIC
inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
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

    CScriptActionPlanner* p_planner = Globals::get_script_action_planner(p_client_object);

    if (!p_planner)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    p_planner->add_evaluator(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kScriptCombat,
        new Script_EvaluatorCheckCombat("script_combat", storage));
    p_planner->action(StalkerDecisionSpace::eWorldOperatorCombatPlanner)
        .add_condition(CWorldProperty(Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kScriptCombat, false));

    // Lord: доделать для игнора и зомбиед
}

inline void set_combat_checker(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name)
{
}

inline void disable_scheme(CScriptGameObject* const p_client_object, const xr_string& scheme_name) {}

inline void set_combat_type(
    CScriptGameObject* const p_npc, CScriptGameObject* const p_actor, const xr_map<std::uint32_t, CondlistData>& condlist)
{
    if (condlist.empty())
        return;

    if (!p_npc)
    {
        Msg("[Scripts/XR_COMBAT/set_combat_type(p_npc, p_actor, overrides)] WARNING: p_npc == nullptr! Return ...");
        return;
    }

    DataBase::Storage::getInstance().setStorageEnemy(p_npc->ID(), p_npc->GetBestEnemy());
    xr_string script_combat_type_name;

    script_combat_type_name = XR_LOGIC::pick_section_from_condlist(p_actor, p_npc, condlist);
    
    DataBase::Storage::getInstance().setStorageScriptCombatTypeName(p_npc->ID(), script_combat_type_name);
}

} // namespace XR_COMBAT

} // namespace Scripts
} // namespace Cordis
