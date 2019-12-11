#include "stdafx.h"
#include "Script_SchemeXRRemark.h"

namespace Cordis
{
namespace Scripts
{
Script_EvaluatorNeedRemark::_value_type Script_EvaluatorNeedRemark::evaluate(void)
{
    return XR_LOGIC::is_active(this->m_object, *this->m_p_storage);
}

void Script_SchemeXRRemark::initialize(void)
{
    CScriptActionBase::initialize();
    this->m_object->set_desired_position();
    this->m_object->set_desired_direction();
}

void Script_SchemeXRRemark::execute(void) { CScriptActionBase::execute(); }

void Script_SchemeXRRemark::finalize(void) { CScriptActionBase::finalize(); }

void Script_SchemeXRRemark::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object) {}

void Script_SchemeXRRemark::update(const float delta)
{
    Msg("[Scripts/Script_SchemeXRRemark/update(delta)] %s state->%d", this->m_object->Name(), this->m_state);

    if (this->m_state == XR_REMARK::kStateInitial)
    {
        
    }
}

} // namespace Scripts
} // namespace Cordis
