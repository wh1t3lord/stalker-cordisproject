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
