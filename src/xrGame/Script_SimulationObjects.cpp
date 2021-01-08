#include "stdafx.h"
#include "Script_SimulationObjects.h"
#include "Script_XR_Logic.h"

namespace Cordis
{
namespace Scripts
{
Script_SimulationObjects::~Script_SimulationObjects(void) {}

float Script_SimulationObjects::evaluate_priority(CSE_ALifeDynamicObject* target, CSE_ALifeDynamicObject* squad)
{
    if (!target || !squad)
    {
        R_ASSERT2(false, "Object was null!");
        return 0.0f;
    }
    float priority = 0.0f;
    Script_SE_SimulationSquad* const p_squad_prior = squad->cast_script_se_simulationsquad();
    Script_SE_Actor* const p_actor = target->cast_script_se_actor();

    MESSAGE("calcuate priority for [%s]", target->name_replace());

    if (p_actor)
    {
        if ((p_actor->target_precondition(p_squad_prior) == false) || (Globals::is_on_the_same_level(target, squad) == false))
        {
            return priority;
        }
        else
        {
            priority = 3.0f;
        }
    }
    else
    {
        Script_SE_SmartTerrain* const p_smart = target->cast_script_se_smartterrain();
        if (p_smart)
        {
            if ((p_smart->target_precondition(squad, false) == false) || (Globals::is_on_the_same_level(target, squad) == false))
            {
                return priority;
            }
            else
            {
                priority = 3.0f;
            }
        }
        else
        {
            Script_SE_SimulationSquad* const p_squad = target->cast_script_se_simulationsquad();
            if (p_squad)
            {
                if ((p_squad->target_precondition(squad) == false) || (Globals::is_on_the_same_level(target, squad) == false))
                {
                    return priority;
                }
                else
                {
                    priority = 3.0f;
                }
            }
            else
            {
                R_ASSERT2(false, "can't be!");
            }
        }
    }

    for (const std::pair<xr_string, xr_string>& it : p_squad_prior->getBehavior())
    {
        float squad_koeff = atof(it.first.c_str());
        float target_koeff = 0.0f;
        if (target->cast_script_se_smartterrain())
        {
            target_koeff = atof(target->cast_script_se_smartterrain()->getProperties().at(it.first).c_str());
        }

        priority += squad_koeff * target_koeff;
    }

    return priority*this->evaluate_priority_by_distance(target, squad);
}

void Script_SimulationObjects::registrate(CSE_ALifeDynamicObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    this->get_properties(object);
    this->update_avaliability(object);
}

void Script_SimulationObjects::unregistrate(CSE_ALifeDynamicObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    // Lord: подумать нужно ли реальное удаление объекта (delete xr_delete), скорее всего нет
    this->m_objects[object->ID] = nullptr;
}

void Script_SimulationObjects::update_avaliability(CSE_ALifeDynamicObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    // Lord: ПЫС также туда могут передавать ai().alife().graph().actor();!!!!!
    // Lord: пока что временное решение но нужно подумать реально ли использовать DataBase или здесь всё таки только серверное используется (первый аргумент)?
    if ((XR_LOGIC::pick_section_from_condlist(ai().alife().graph().actor(), object, object->getSimulationAvail()) == "true") &&
        object->IsSimulationAvailable())
    {
        this->m_objects[object->ID] = object;
    }
    else
    {
        this->m_objects[object->ID] = nullptr;
    }
}

void Script_SimulationObjects::get_properties(CSE_ALifeDynamicObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    xr_string properties_section = object->name();

    if (object->script_clsid() == Globals::get_script_clsid(CLSID_SE_ONLINE_OFFLINE_GROUP))
        properties_section = object->name_replace();

    if (!this->m_props_ini.section_exist(properties_section.c_str()))
    {
        MESSAGEW("object [%s] has no simulation properties section!", object->name());
        properties_section = "default";

        if (object->script_clsid() == Globals::get_script_clsid(CLSID_SE_ONLINE_OFFLINE_GROUP))
            properties_section = "default_squad";

        if (object->script_clsid() == Globals::get_script_clsid(CLSID_SE_ACTOR))
            properties_section = "actor";
    }

    std::uint32_t count_lines = this->m_props_ini.line_count(properties_section.c_str());
    for (int i = 0; i < count_lines; ++i)
    {
        xr_string section, value;
        pcstr _s, _v;
        bool result = this->m_props_ini.r_line(properties_section.c_str(), i, &_s, &_v);
        section = _s;
        value = _v;
 
        if (section == "sim_avail")
            object->setSimulationAvail(XR_LOGIC::parse_condlist_by_server_object("simulation_object", "sim_avail", value));
        else
            object->getProperties()[section] = value;
    }

	if (object->getSimulationAvail().empty())
		object->setSimulationAvail(XR_LOGIC::parse_condlist_by_server_object("simulation_object", "sim_avail", "true"));
}

} // namespace Scripts
} // namespace Cordis
