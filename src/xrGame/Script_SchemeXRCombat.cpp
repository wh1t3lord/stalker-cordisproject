#include "stdafx.h"
#include "Script_SchemeXRCombat.h"

namespace Cordis
{
namespace Scripts
{
Script_EvaluatorCheckCombat::_value_type Script_EvaluatorCheckCombat::evaluate(void)
{
    if (this->m_p_storage->IsEnabled() && this->m_object->GetBestEnemy())
    {
        if (!DataBase::Storage::getInstance().getActor())
            return false;

        return !this->m_p_storage->getXRCombatScriptCombatTypeName().empty();
    }

    return false;
}

Script_EvaluatorEnemy::_value_type Script_EvaluatorEnemy::evaluate(void) { return !!(this->m_object->GetBestEnemy()); }

} // namespace Scripts
} // namespace Cordis

void Cordis::Scripts::XR_COMBAT::add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, DataBase::Storage_Scheme& storage)
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

	MESSAGE("added Script_SchemeMobWalker scheme to binder, name=%s scheme=%s section=%s",
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

	Script_SchemeXRCombatZombied::add_to_binder(p_client_object, p_ini, section_name, scheme_name, storage);
	Script_SchemeXRCombatCamper::add_to_binder(p_client_object, p_ini, section_name, scheme_name, storage);
}

void Cordis::Scripts::XR_COMBAT::set_combat_checker(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini, const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
	if (p_client_object == nullptr)
	{
		MESSAGEWR("invalid object!");
		return;
	}

	if (p_ini == nullptr)
	{
		MESSAGEWR("invalid ini!");
		return;
	}

	bool is_zombied = (Globals::character_community(p_client_object) == "zombied");

	if ((section_name.empty() == false) || is_zombied)
	{
		DataBase::Storage_Scheme* const p_storage = XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

		p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
		p_storage->setEnabled(true);

		p_storage->setXRCombatCombatTypeCondlist(XR_LOGIC::cfg_get_condlist(p_ini, section_name, "combat_type", p_client_object).getCondlist());

		xr_string result_name = XR_LOGIC::pick_section_from_condlist(nullptr, p_client_object, p_storage->getXRCombatCombatTypeCondlist());

		if (result_name == "monolith")
		{
			p_storage->setXRCombatCombatTypeCondlist({});
		}

		if (p_storage->getXRCombatCombatTypeCondlist().empty() && is_zombied)
		{
			p_storage->setXRCombatCombatTypeCondlist(XR_LOGIC::parse_condlist_by_script_object(section_name, "", "zombied"));
		}

		if (p_storage->getXRCombatCombatTypeCondlist().empty() == false)
		{
			set_combat_type(p_client_object, DataBase::Storage::getInstance().getActor(), p_storage->getXRCombatCombatTypeCondlist());
		}
	}
}
