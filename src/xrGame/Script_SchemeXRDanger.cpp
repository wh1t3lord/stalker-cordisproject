#include "stdafx.h"
#include "Script_SchemeXRDanger.h"

namespace Cordis
{
namespace Scripts
{
Script_EvaluatorDanger::_value_type Script_EvaluatorDanger::evaluate(void)
{
    if (!this->m_p_manager)
    {
        this->m_p_manager = Globals::get_script_action_planner(this->m_object);
    }

    if (this->m_p_storage->getDangerTime() && this->m_object->GetBestDanger() &&
        (Globals::get_time_global() - this->m_p_storage->getDangerTime() < XR_DANGER::kDangerInertionTime))
    {
        DataBase::Storage::getInstance().setStorageDangerFlag(this->m_object->ID(), true);
        return true;
    }

    if (!XR_DANGER::is_danger(this->m_object))
    {
        DataBase::Storage::getInstance().setStorageDangerFlag(this->m_object->ID(), false);
        return false;
    }

    if (this->m_p_manager->initialized() &&
        this->m_p_manager->current_action_id() == StalkerDecisionSpace::eWorldOperatorDangerPlanner)
    {
        this->m_p_storage->setDangerTime(XR_DANGER::get_danger_time(this->m_object->GetBestDanger()));
    }

    DataBase::Storage::getInstance().setStorageDangerFlag(this->m_object->ID(), true);

    CSE_ALifeDynamicObject* p_server_object = ai().alife().objects().object(this->m_object->ID());
    if (p_server_object)
    {
        CSE_ALifeMonsterAbstract* p_server_creature = p_server_object->cast_monster_abstract();
        if (p_server_creature && p_server_creature->m_smart_terrain_id != Globals::kUnsignedInt32Undefined)
        {
            ai().alife()
                .objects()
                .object(p_server_creature->m_smart_terrain_id)
                ->cast_script_se_smartterrain()
                ->set_alarm();
        }
    }

    return true;
}

} // namespace Scripts
} // namespace Cordis
