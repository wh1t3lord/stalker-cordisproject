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

    Script_MobStateManager::getInstance().set_state(this->m_npc, this->m_storage->getStateName());

    std::uint32_t min_radius = kDefaultMinRadius;
    std::uint32_t mid_radius = kDefaultMidRadius;
    std::uint32_t max_radius = kDefaultMaxRadius;

    CPatrolPathParams* patrol = nullptr;
    CondlistWaypoints::CondlistWayPointsData path_info;
    if (!this->m_storage->getHomeName().empty())
    {
        patrol = new CPatrolPathParams(this->m_storage->getHomeName().c_str());

        path_info =
            Globals::Utils::parse_waypoint_data(this->m_storage->getHomeName(), patrol->flags(0), patrol->name(0));
        xr_delete(patrol);
    }

    if (this->m_storage->getHomeMinRadius())
    {
        min_radius = this->m_storage->getHomeMinRadius();
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

    if (this->m_storage->getHomeMaxRadius())
    {
        max_radius = this->m_storage->getHomeMaxRadius();
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

    if (this->m_storage->getHomeMidRadius())
    {
        mid_radius = this->m_storage->getHomeMidRadius();

        if ((mid_radius <= min_radius) || (mid_radius >= max_radius))
            mid_radius = min_radius + ((max_radius - min_radius) / 2);
    }
    else
    {
        mid_radius = min_radius + ((max_radius - min_radius) / 2);
    }

    if (this->m_storage->IsGulagPoint())
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

        this->m_npc->set_home(level_vertex_id, min_radius, max_radius, this->m_storage->IsAggresive(), mid_radius);
    }
    else
    {
        this->m_npc->set_home(
            this->m_storage->getHomeName().c_str(), min_radius, max_radius, this->m_storage->IsAggresive(), mid_radius);
    }
}

void Script_SchemeMobHome::deactivate(CScriptGameObject* const p_client_object) 
{
    this->m_npc->remove_home(); 
}

} // namespace Scripts
} // namespace Cordis
