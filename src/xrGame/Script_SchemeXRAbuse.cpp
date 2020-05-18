#include "stdafx.h"
#include "Script_SchemeXRAbuse.h"

namespace Cordis
{
namespace Scripts
{
Script_EvaluatorAbuse::_value_type Script_EvaluatorAbuse::evaluate(void)
{
    if (!this->m_p_storage->getXRAbuseManager())
    {
        R_ASSERT2(false, "can't be! Do you initialize and set it in add_to_binder function?");
        return false;
    }

    return this->m_p_storage->getXRAbuseManager()->update();
}

void Script_SchemeXRAbuse::execute(void) 
{ CScriptActionBase::execute(); }

void Script_SchemeXRAbuse::finalize(void) { CScriptActionBase::finalize(); }

void Script_SchemeXRAbuse::initialize(void) 
{
    CScriptActionBase::initialize();
    
    this->m_object->set_desired_position();
    this->m_object->set_desired_direction();

    Globals::set_state(this->m_object, "punch", StateManagerCallbackData(), 0, std::pair<Fvector, CScriptGameObject* const>(Fvector(), DataBase::Storage::getInstance().getActor()), StateManagerExtraData());
}

void Script_SchemeXRAbuse::add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
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

    MESSAGE("Added scheme %s", scheme_name);

    xr_map<xr_string, std::uint32_t> operators;
    xr_map<xr_string, std::uint32_t> properties;

    CScriptActionPlanner* const p_planner = Globals::get_script_action_planner(p_client_object);

    properties["abuse"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kAbuseBase;
    properties["wounded"] = Globals::XR_ACTIONS_ID::XR_EVALUATORS_ID::kSidorWoundedBase;

    operators["abuse"] = Globals::XR_ACTIONS_ID::kAbuseBase;

    p_planner->add_evaluator(properties.at("abuse"), new Script_EvaluatorAbuse("evaluator_abuse", storage));

    Script_SchemeXRAbuse* p_scheme = new Script_SchemeXRAbuse("action_abuse_hit", storage);
    p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyAlive, true));
    p_scheme->add_condition(CWorldProperty(StalkerDecisionSpace::eWorldPropertyDanger, true));
    p_scheme->add_condition(CWorldProperty(properties.at("wounded"), false));
    p_scheme->add_condition(CWorldProperty(properties.at("abuse"), false));
    p_scheme->add_effect(CWorldProperty(properties.at("abuse"), true));
    
    p_planner->add_operator(operators.at("abuse"), p_scheme);

    p_planner->action(Globals::XR_ACTIONS_ID::kAlife).add_condition(CWorldProperty(properties.at("abuse"), false));
    storage.setXRAbuseManager(new DataBase::Script_XRAbuseManager(p_client_object, storage));

}


void Script_SchemeXRAbuse::set_abuse(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    DataBase::Storage_Scheme* p_storage = XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);
}

void Script_SchemeXRAbuse::add_abuse(CScriptGameObject* const p_client_object, const float value)
{
    if (!p_client_object)
    {
        MESSAGEWR("can't add because p_client_object == nullptr!");
        return;
    }

    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
        DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
    {
        DataBase::Storage::getInstance()
            .getStorage()
            .at(p_client_object->ID())
            .getSchemes()
            .at("abuse")
            ->getXRAbuseManager()
            ->AddAbuse(value);
    }
    else
    {
        MESSAGEWR("can't add because abuse scheme doesnt exist!");
        return;
    }
}

void Script_SchemeXRAbuse::clear_abuse(CScriptGameObject* const p_client_object)
{
    if (!p_client_object)
    {
        MESSAGEWR("can't add because p_client_object == nullptr!");
        return;
    }

    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
        DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
    {
        DataBase::Storage::getInstance()
            .getStorage()
            .at(p_client_object->ID())
            .getSchemes()
            .at("abuse")
            ->getXRAbuseManager()
            ->ClearAbuse();
    }
    else
    {
        MESSAGEWR("can't add because abuse scheme doesnt exist!");
        return;
    }
}

void Script_SchemeXRAbuse::disable_abuse(CScriptGameObject* const p_client_object)
{
    if (!p_client_object)
    {
        MESSAGEWR("can't add because p_client_object == nullptr!");
        return;
    }

    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
        DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
    {
        DataBase::Storage::getInstance()
            .getStorage()
            .at(p_client_object->ID())
            .getSchemes()
            .at("abuse")
            ->getXRAbuseManager()
            ->DisableAbuse();
    }
    else
    {
        MESSAGEWR("can't add because abuse scheme doesnt exist!");
        return;
    }
}

void Script_SchemeXRAbuse::enable_abuse(CScriptGameObject* const p_client_object)
{
    if (!p_client_object)
    {
        MESSAGEWR("can't add because p_client_object == nullptr!");
        return;
    }

    if (DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().find("abuse") !=
        DataBase::Storage::getInstance().getStorage().at(p_client_object->ID()).getSchemes().end())
    {
        DataBase::Storage::getInstance()
            .getStorage()
            .at(p_client_object->ID())
            .getSchemes()
            .at("abuse")
            ->getXRAbuseManager()
            ->EnableAbuse();
    }
    else
    {
        MESSAGEWR("can't add because abuse scheme doesnt exist!");
        return;
    }
}

} // namespace Scripts
} // namespace Cordis
