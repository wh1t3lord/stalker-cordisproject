#include "stdafx.h"
#include "Script_SchemeMobHome.h"

const std::uint32_t kDefaultMinRadius = 10;
const std::uint32_t kDefaultMidRadius = 20;
const std::uint32_t kDefaultMaxRadius = 70;

namespace Cordis
{
namespace Scripts
{
Script_SchemeMobHome::Script_SchemeMobHome(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
}

Script_SchemeMobHome::~Script_SchemeMobHome(void) {}

void Script_SchemeMobHome::reset_scheme(const bool, CScriptGameObject* const p_client_object)
{
    Msg("[Scripts/Script_SchemeMobHome/reset_scheme()] %s ", this->m_npc->Name());

    Script_MobStateManager::getInstance().set_state(this->m_npc, this->m_p_storage->getStateName());

    std::uint32_t min_radius = kDefaultMinRadius;
    std::uint32_t mid_radius = kDefaultMidRadius;
    std::uint32_t max_radius = kDefaultMaxRadius;

    CPatrolPathParams* patrol = nullptr;
    CondlistWaypoints::CondlistWayPointsData path_info;
    if (!this->m_p_storage->getHomeName().empty())
    {
        patrol = new CPatrolPathParams(this->m_p_storage->getHomeName().c_str());

        path_info =
            Globals::Utils::parse_waypoint_data(this->m_p_storage->getHomeName(), patrol->flags(0), patrol->name(0));
        xr_delete(patrol);
    }

    if (this->m_p_storage->getHomeMinRadius())
    {
        min_radius = this->m_p_storage->getHomeMinRadius();
    }
    else
    {
        xr_string min_radius_name = path_info.getValue("minr");
        if (!min_radius_name.empty())
        {
            if (atoi(min_radius_name.c_str()))
            {
                min_radius = atoi(min_radius_name.c_str());
            }
        }
    }

    if (this->m_p_storage->getHomeMaxRadius())
    {
        max_radius = this->m_p_storage->getHomeMaxRadius();
    }
    else
    {
        xr_string max_radius_name = path_info.getValue("maxr");
        if (!max_radius_name.empty())
        {
            if (atoi(max_radius_name.c_str()))
            {
                max_radius = atoi(max_radius_name.c_str());
            }
        }
    }

    if (min_radius > max_radius)
    {
        R_ASSERT2(false, "can't be!");
    }

    if (this->m_p_storage->getHomeMidRadius())
    {
        mid_radius = this->m_p_storage->getHomeMidRadius();

        if ((mid_radius <= min_radius) || (mid_radius >= max_radius))
            mid_radius = min_radius + ((max_radius - min_radius) / 2);
    }
    else
    {
        mid_radius = min_radius + ((max_radius - min_radius) / 2);
    }

    if (this->m_p_storage->IsGulagPoint())
    {
        Script_SE_SmartTerrain* p_smart_terrain =
            ai().alife()
                .objects()
                .object(ai().alife().objects().object(this->m_npc->ID())->cast_monster_abstract()->m_smart_terrain_id)
                ->cast_script_se_smartterrain();

        if (!p_smart_terrain)
        {
            Msg("[scripts/Script_SchemeMobHome/reset_scheme()] WARNING: p_smart_terrain == nullptr!");
        }

        std::uint32_t level_vertex_id = (p_smart_terrain) ? p_smart_terrain->m_tNodeID : 0;

        this->m_npc->set_home(level_vertex_id, min_radius, max_radius, this->m_p_storage->IsAggresive(), mid_radius);
    }
    else
    {
        this->m_npc->set_home(this->m_p_storage->getHomeName().c_str(), min_radius, max_radius,
            this->m_p_storage->IsAggresive(), mid_radius);
    }
}

void Script_SchemeMobHome::deactivate(CScriptGameObject* const p_client_object) { this->m_npc->remove_home(); }

void Script_SchemeMobHome::set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
    const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name)
{
    if (!p_client_object)
    {
        R_ASSERT2(false, "it can't be!");
        return;
    }

    DataBase::Storage_Scheme* p_storage =
        XR_LOGIC::assign_storage_and_bind(p_client_object, p_ini, scheme_name, section_name, gulag_name);

    if (!p_storage)
    {
        R_ASSERT2(false, "it can;t be!");
        return;
    }

    p_storage->setLogic(XR_LOGIC::cfg_get_switch_conditions(p_ini, section_name, p_client_object));
    p_storage->setStateName(Script_MobStateManager::getInstance().get_state(p_ini, section_name));
    p_storage->setHomeName(Globals::Utils::cfg_get_string(p_ini, section_name, "path_home"));
    p_storage->setGulagPoint(Globals::Utils::cfg_get_bool(p_ini, section_name, "gulag_point"));
    p_storage->setHomeMinRadius(static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "home_min_radius")));
    p_storage->setHomeMidRadius(static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "home_mid_radius")));
    p_storage->setHomeMaxRadius(static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(p_ini, section_name, "home_max_radius")));
    p_storage->setAggresive(Globals::Utils::cfg_get_bool(p_ini, section_name, "aggressive"));
}

} // namespace Scripts
} // namespace Cordis
