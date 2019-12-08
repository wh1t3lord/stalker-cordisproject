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
    // @ Можете доделать!
}

} // namespace Scripts
} // namespace Cordis
