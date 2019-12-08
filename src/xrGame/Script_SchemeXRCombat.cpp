#include "stdafx.h"
#include "Script_SchemeXRCombat.h"

namespace Cordis
{
namespace Scripts
{
// Lord : протестировать по логике кода m_object будет всегда nullptr
// Если нет то доделать данный метод
Script_EvaluatorCheckCombat::_value_type Script_EvaluatorCheckCombat::evaluate(void)
{
    if (this->m_object)
    {
        Msg("[Scripts/Script_EvaluatorCheckCombat/evaluate()] m_object not empty!");
    }

    return false;
}

// Lord: опять же проверить если m_object не всегда nullptr то доделать метод
Script_EvaluatorEnemy::_value_type Script_EvaluatorEnemy::evaluate(void)
{
    if (this->m_object)
    {
        Msg("[Scripts/Script_EvaluatorEnemy/evaluate()] m_object not empty!");
    }

    return false;
}

} // namespace Scripts
} // namespace Cordis
