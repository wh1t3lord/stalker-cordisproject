#include "stdafx.h"
#include "Script_SimulationObjects.h"

namespace Cordis
{
namespace Scripts
{
Script_SimulationObjects::~Script_SimulationObjects(void) {}

float Script_SimulationObjects::evaluate_priority(CSE_ALifeObject* target, CSE_ALifeObject* squad)
{
    if (!target || !squad)
    {
        R_ASSERT2(false, "Object was null!");
        return 0.0f;
    }

    float priority = 0.0f;

    // Lord: доделать!
}

void Script_SimulationObjects::registrate(CSE_ALifeObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }

    this->get_properties(object);
    this->update_avaliability(object);
}

void Script_SimulationObjects::unregistrate(CSE_ALifeObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    // Lord: подумать нужно ли реальное удаление объекта (delete xr_delete), скорее всего нет
    this->m_objects[object->ID] = nullptr;
}

void Script_SimulationObjects::update_avaliability(CSE_ALifeObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return;
    }
    // Lord: ПЫС также туда могут передавать ai().alife().graph().actor();!!!!!
    if ((XR_LOGIC::pick_section_from_condlist(
             DataBase::Storage::getInstance().getActor(), object, object->getSimulationAvail()) == "true") &&
        object->IsSimulationAvailable())
    {
        this->m_objects[object->ID] = object;
    }
    else
    {
        this->m_objects[object->ID] = nullptr;
    }
}

void Script_SimulationObjects::get_properties(CSE_ALifeObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return;
    }

    xr_string properties_section = object->name();

    if (object->script_clsid() == CLSID_SE_ONLINE_OFFLINE_GROUP)
        properties_section = object->name_replace();

    if (!this->m_props_ini.section_exist(properties_section.c_str()))
    {
        Msg("[Script_SimulationObjects] -> object [%s] has no simulation properties section!", object->name());
        properties_section = "default";

        if (object->script_clsid() == CLSID_SE_ONLINE_OFFLINE_GROUP)
            properties_section = "default_squad";

        if (object->script_clsid() == CLSID_SE_ACTOR)
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
        // Lord: доделать
        if (section == "sim_avail")
            object->getSimulationAvail() = XR_LOGIC::parse_condlist_by_server_object(
                nullptr, xr_string("simulation_object"), xr_string("sim_avail"), value);
        else
            object->getProperties()[section] = value;

        if (!object->getSimulationAvail().size())
            object->getSimulationAvail() =
                XR_LOGIC::parse_condlist_by_server_object(nullptr, "simulation_object", "sim_avail", "true");
    }
}

} // namespace Scripts
} // namespace Cordis
