#pragma once
#include "Script_SurgeManager.h"
#include "Script_XR_Sound.h"
#include "Script_XR_Gulag.h"
#include "Script_XR_Wounded.h"

namespace Cordis
{
namespace Scripts
{
namespace XR_CONDITION
{
inline bool is_surge_started(void) { return Script_SurgeManager::getInstance().IsStarted(); }

inline bool is_enemy_actor(CScriptGameObject* enemy, CScriptGameObject* object)
{
    if (!enemy)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return (enemy->ID() == object->ID());
}

inline bool is_fighting_dist_ge(CScriptGameObject* enemy, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!enemy)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    float distance = atof(buffer[0].c_str());

    return (enemy->Position().distance_to_sqr(npc->Position()) >= (distance * distance));
}

inline bool is_fighting_dist_ge(
    CSE_ALifeDynamicObject* enemy, CSE_ALifeDynamicObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!enemy)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    float distance = atof(buffer[0].c_str());

    return (enemy->Position().distance_to_sqr(npc->Position()) >= (distance * distance));
}

inline bool is_fighting_dist_le(CScriptGameObject* enemy, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!enemy)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    float distance = atof(buffer[0].c_str());

    return (enemy->Position().distance_to_sqr(npc->Position()) <= distance * distance);
}

inline bool is_fighting_dist_le(
    CSE_ALifeDynamicObject* enemy, CSE_ALifeDynamicObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!enemy)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    float distance = atof(buffer[0].c_str());

    return (enemy->Position().distance_to_sqr(npc->Position()) <= distance * distance);
}

inline bool is_enemy_in_zone(CScriptGameObject* enemy, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!enemy)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& zone_name = buffer[0];

    CScriptGameObject* zone = DataBase::Storage::getInstance().getZoneByName()[zone_name];

    if (!zone)
    {
        R_ASSERT2(false, "Can't find a script object's zone!");
        return false;
    }

    return Globals::Utils::is_npc_in_zone(enemy, zone);
}

inline bool is_enemy_in_zone(
    CSE_ALifeDynamicObject* server_enemy, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!server_enemy)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!server_npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& zone_name = buffer[0];

    CScriptGameObject* zone = DataBase::Storage::getInstance().getZoneByName()[zone_name];

    if (!zone)
    {
        R_ASSERT2(false, "Can't find a script object's zone");
        return false;
    }

    return Globals::Utils::is_npc_in_zone(server_enemy, zone);
}

inline bool is_black_screen(CScriptGameObject* actor, CScriptGameObject* npc) { return Device.dwPrecacheFrame > 1; }

inline bool is_black_screen(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc)
{
    return Device.dwPrecacheFrame > 1;
}

inline bool check_npc_name(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    if (!actor)
    {
        R_ASSERT2(false, "actor is null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "npc is null!");
        return false;
    }

    if (!npc->Name())
    {
        R_ASSERT2(false, "String can't be nullptr! Something goes wrong!");
        return false;
    }

    const xr_string& npc_name = buffer[0];

    if (!npc_name.size())
    {
        R_ASSERT2(false, "String can't be empty!");
        return false;
    }

    return (npc->Name() == npc_name);
}

inline bool check_npc_name(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!server_npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!server_npc->name_replace())
    {
        R_ASSERT2(false, "string can't be null! Something goes wrong!");
        return false;
    }

    const xr_string& npc_name = buffer[0];

    if (!npc_name.size())
    {
        R_ASSERT2(false, "Name can't be empty!");
        return false;
    }

    return (server_npc->name_replace() == npc_name);
}

inline bool check_enemy_name(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    const xr_string& npc_name = buffer[0];

    if (!npc_name.size())
    {
        R_ASSERT2(false, "Name can't be empty!");
        return false;
    }

    const std::uint16_t& enemy_id = DataBase::Storage::getInstance().getStorage()[npc->ID()].m_enemy_id;
    CScriptGameObject* enemy = DataBase::Storage::getInstance().getStorage()[enemy_id].m_object;

    if (!enemy)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (enemy->Alive())
    {
        xr_string name = enemy->Name();
        return (name == npc_name);
    }

    return false;
}

inline bool is_playing_sound(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return XR_SOUND::getSoundDatabase()[npc->ID()];
}

inline bool is_see_npc(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    if (!npc)
    {
        Msg("[Scripts/XR_CONDITION/is_see_npc(actor, npc, npc_name)] WARNING: npc = nullptr! Returned value = false.");
        return false;
    }

    const xr_string& npc_name = buffer[0];

    CScriptGameObject* npc1 = Globals::get_story_object(npc_name);

    if (!npc1)
    {
        Msg("[Scripts/XR_CONDITION/is_see_npc(actor, npc, npc_name)] WARNING: npc1 = nullptr! Returned value = false.");
        return false;
    }

    return npc->CheckObjectVisibility(npc1);
}

inline bool is_actor_see_npc(CScriptGameObject* actor, CScriptGameObject* npc)
{
    return DataBase::Storage::getInstance().getActor()->CheckObjectVisibility(npc);
}

inline bool is_npc_in_actor_frustrum(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return Globals::is_npc_in_actor_frustrum(npc);
}

inline bool is_wounded(CScriptGameObject* actor, CScriptGameObject* npc) { return XR_WOUNDED::is_wounded(npc); }

inline bool is_dist_to_actor_ge(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    float distance = atof(buffer[0].c_str());

    if (fis_zero(distance))
    {
        Msg("[Scripts/XR_CONDITION/is_dist_to_actor_ge(actor, npc, distance)] WARNING: distance is ZERO -> %f",
            distance);
        return false;
    }

    if (distance < 0.0f)
    {
        Msg("[Scripts/XR_CONDITION/is_dist_to_actor_ge(actor, npc, distance)] WARNING: distance is a negative value -> "
            "%f",
            distance);
        return false;
    }

    return (npc->Position().distance_to_sqr(actor->Position()) >= (distance * distance));
}

inline bool is_dist_to_actor_ge(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!server_npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    float distance = atof(buffer[0].c_str());

    if (fis_zero(distance))
    {
        Msg("[Scripts/XR_CONDITION/is_dist_to_actor_ge(server_actor, server_npc, distance)] WARNING: distnace is ZERO "
            "-> %f",
            distance);
        return false;
    }

    if (distance < 0.0f)
    {
        Msg("[Scripts/XR_CONDITION/is_dist_to_actor_ge(server_actor, server_npc, distance)] WARNING: distance is a "
            "negative value -> %f",
            distance);
        return false;
    }

    return (server_npc->Position().distance_to_sqr(actor->Position()) >= (distance * distance));
}

inline bool is_dist_to_actor_ge(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    if (!server_actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!server_npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    float distance = atof(buffer[0].c_str());

    if (fis_zero(distance))
    {
        Msg("[Scripts/XR_CONDITION/is_dist_to_actor_ge(server_actor, server_npc, distance)] WARNING: distnace is ZERO "
            "-> %f",
            distance);
        return false;
    }

    if (distance < 0.0f)
    {
        Msg("[Scripts/XR_CONDITION/is_dist_to_actor_ge(server_actor, server_npc, distance)] WARNING: distance is a "
            "negative value -> %f",
            distance);
        return false;
    }

    return (server_npc->Position().distance_to_sqr(server_actor->Position()) >= (distance * distance));
}

inline bool is_obj_on_job(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    const xr_string& logic_name = buffer[0];

    if (!logic_name.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return false;
    }

    Script_SE_SmartTerrain* server_smart = nullptr;

    const xr_string& smart_terrain_name = buffer[1];

    if (!smart_terrain_name.size())
    {
        Msg("[Scripts/XR_CONDITION/is_obj_on_job(actor, npc, logic_name, smart_terrain_name)] WARNING: "
            "smart_terrain_name is empty string!");
        server_smart = XR_GULAG::get_npc_smart(npc)->cast_script_se_smartterrain();

        if (!server_smart)
        {
            Msg("[Scripts/XR_CONDITION/is_obj_on_job(actor, npc, logic_name, smart_terrain_name)] WARNING: "
                "server_smart_terrain was nullptr! Return false.");
            return false;
        }
    }
    else
    {
        Msg("[Scripts/XR_CONDITION/is_obj_on_job(actor, npc, logic_name, smart_terrain_name)] getting smart terrain by "
            "argument -> smart_terrain_name = %s",
            smart_terrain_name.c_str());
        server_smart = Script_SimulationBoard::getInstance().getSmartTerrainsByName()[smart_terrain_name];

        if (!server_smart)
        {
            Msg("[Scripts/XR_CONDITION/is_obj_on_job(actor, npc, logic_name, smart_terrain_name)] WARNING: "
                "server_smart_terrain was nullptr! Return false.");
            return false;
        }
    }

    for (std::pair<const std::uint32_t, NpcInfo>& it : server_smart->getNpcInfo())
    {
        JobDataSmartTerrain& job_data = server_smart->getJobData()[it.second.m_job_link.m_job_index];

        Msg("[Scripts/XR_CONDITION/is_obj_on_job(actor, npc, logic_name, smart_terrain_name)] section %s",
            job_data.m_job_id.first.c_str());

        if (job_data.m_job_id.first == logic_name)
            return true;
    }

    return false;
}

inline bool is_distance_to_obj_on_job_le(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& logic_name = buffer[0];
    float distance = atof(buffer[1].c_str());

    if (!logic_name.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    Script_SE_SmartTerrain* server_smart = XR_GULAG::get_npc_smart(npc)->cast_script_se_smartterrain();

    for (std::pair<const std::uint32_t, NpcInfo>& it : server_smart->getNpcInfo())
    {
        JobDataSmartTerrain& job_data = server_smart->getJobData()[it.second.m_job_link.m_job_index];

        if (job_data.m_job_id.first == logic_name)
        {
            return (npc->Position().distance_to_sqr(it.second.m_server_object->Position()) <= (distance * distance));
        }
    }

    return false;
}

inline bool is_story_obj_in_zone_by_name(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& object_id_name = buffer[0];
    const xr_string& zone_name = buffer[1];

    if (!object_id_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    if (!zone_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    std::uint16_t object_id = Globals::get_story_object_id(object_id_name);
    CScriptGameObject* zone = DataBase::Storage::getInstance().getZoneByName()[zone_name];

    if (object_id && zone)
    {
        CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(object_id);

        if (!server_object)
        {
            R_ASSERT2(false, "object was null!");
            return false;
        }

        return zone->inside(server_object->Position());
    }

    return false;
}

inline bool is_story_obj_in_zone_by_name(
    CSE_ALifeDynamicObject* actor, CSE_ALifeDynamicObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& object_id_name = buffer[0];
    const xr_string& zone_name = buffer[1];

    if (!object_id_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    if (!zone_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    std::uint16_t object_id = Globals::get_story_object_id(object_id_name);
    CScriptGameObject* zone = DataBase::Storage::getInstance().getZoneByName()[zone_name];

    if (object_id && zone)
    {
        CSE_ALifeDynamicObject* server_object = ai().alife().objects().object(object_id);

        if (!server_object)
        {
            R_ASSERT2(false, "object was null!");
            return false;
        }

        return zone->inside(server_object->Position());
    }

    return false;
}

inline bool is_actor_in_zone(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& zone_name = buffer[0];

    if (!zone_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    CScriptGameObject* zone = DataBase::Storage::getInstance().getZoneByName()[zone_name];

    if (!zone)
    {
        R_ASSERT2(false, "something goes wrong!");
        return false;
    }

    return Globals::Utils::is_npc_in_zone(DataBase::Storage::getInstance().getActor(), zone);
}

inline bool is_actor_in_zone(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& zone_name = buffer[0];

    if (!zone_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    if (!DataBase::Storage::getInstance().getActor())
    {
        Msg("[Scripts/XR_CONDITION/is_actor_in_zone(server_actor, server_npc, zone_name)] WARNING: Client still not "
            "initialize! Early calling!!! DataBase::Storage::Actor = nullptr! Return false.");
        return false;
    }

    CScriptGameObject* zone = DataBase::Storage::getInstance().getZoneByName()[zone_name];

    if (!zone)
    {
        R_ASSERT2(false, "something goes wrong!");
        return false;
    }

    return Globals::Utils::is_npc_in_zone(DataBase::Storage::getInstance().getActor(), zone);
}

inline bool is_actor_in_zone(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& zone_name = buffer[0];

    if (!zone_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    if (!DataBase::Storage::getInstance().getActor())
    {
        Msg("[Scripts/XR_CONDITION/is_actor_in_zone(server_actor, server_npc, zone_name)] WARNING: Client still not "
            "initialize! Early calling!!! DataBase::Storage::Actor = nullptr! Return false.");
        return false;
    }

    CScriptGameObject* zone = DataBase::Storage::getInstance().getZoneByName()[zone_name];

    if (!zone)
    {
        R_ASSERT2(false, "something goes wrong!");
        return false;
    }

    return Globals::Utils::is_npc_in_zone(DataBase::Storage::getInstance().getActor(), zone);
}

inline bool is_npc_in_zone(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& zone_name = buffer[0];

    if (!zone_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    CScriptGameObject* zone = DataBase::Storage::getInstance().getZoneByName()[zone_name];

    if (!zone)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return Globals::Utils::is_npc_in_zone(npc, zone);
}

inline bool is_npc_in_zone(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& zone_name = buffer[0];

    if (!zone_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    if (!server_npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    CScriptGameObject* zone = DataBase::Storage::getInstance().getZoneByName()[zone_name];

    if (!zone)
    {
        Msg("[Scripts/XR_CONDITION/is_npc_in_zone(server_actor, server_npc, zone_name)] NOTIFY: zone = nullptr! "
            "Returns true.");
        return true;
    }

    CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage()[server_npc->ID].m_object;

    if (!npc)
    {
        Msg("[Scripts/XR_CONDITION/is_npc_in_zone(server_actor, server_npc, zone_name)] NOTIFY: npc = nullptr! Returns "
            "zone->inside(server_npc->Position())");
        return zone->inside(server_npc->Position());
    }

    return Globals::Utils::is_npc_in_zone(npc, zone);
}

inline bool is_npc_in_zone(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& zone_name = buffer[0];

    if (!zone_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    if (!server_npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    CScriptGameObject* zone = DataBase::Storage::getInstance().getZoneByName()[zone_name];

    if (!zone)
    {
        Msg("[Scripts/XR_CONDITION/is_npc_in_zone(server_actor, server_npc, zone_name)] NOTIFY: zone = nullptr! "
            "Returns true.");
        return true;
    }

    CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage()[server_npc->ID].m_object;

    if (!npc)
    {
        Msg("[Scripts/XR_CONDITION/is_npc_in_zone(server_actor, server_npc, zone_name)] NOTIFY: npc = nullptr! Returns "
            "zone->inside(server_npc->Position())");
        return zone->inside(server_npc->Position());
    }

    return Globals::Utils::is_npc_in_zone(npc, zone);
}

inline bool is_health_le(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    float value = atof(buffer[0].c_str());

    if (value)
        return (npc->GetHealth() < value);

    Msg("[Scripts/XR_CONDITION/is_health_le(actor, npc, value)] WARNING: value is less than 0 or equals 0! Return "
        "false.");

    return false;
}

inline bool is_actor_health_le(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    float value = atof(buffer[0].c_str());

    if (value)
        return (actor->GetHealth() < value);

    Msg("[Scripts/XR_CONDITION/is_actor_health_le(actor, npc, value)] WARNING: value is less than 0 or equals 0! "
        "Return "
        "false.");

    return false;
}

inline bool is_npc_community(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& community_name = buffer[0];

    if (!community_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (Globals::character_community(npc) == community_name)
        return true;

    return false;
}

inline bool is_npc_community(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& community_name = buffer[0];

    if (!community_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    if (!server_npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage()[server_npc->ID].m_object;

    if (!npc)
    {
        CSE_ALifeTraderAbstract* server_trader = server_npc->cast_trader_abstract();
        if (!server_trader)
        {
            R_ASSERT2(false, "can't be check class!!!");
            return false;
        }

        return (server_trader->CommunityName() == community_name);
    }

    if (Globals::character_community(npc) == community_name)
        return true;

    return false;
}

inline bool is_npc_community(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    const xr_string& community_name = buffer[0];

    if (!community_name.size())
    {
        R_ASSERT2(false, "can't be empty!");
        return false;
    }

    if (!server_npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    CScriptGameObject* npc = DataBase::Storage::getInstance().getStorage()[server_npc->ID].m_object;

    if (!npc)
    {
        CSE_ALifeTraderAbstract* server_trader = server_npc->cast_trader_abstract();
        if (!server_trader)
        {
            R_ASSERT2(false, "can't be check class!!!");
            return false;
        }

        return (server_trader->CommunityName() == community_name);
    }

    if (Globals::character_community(npc) == community_name)
        return true;

    return false;
}

// Lord: доделать когда будет сделано xr_hit
inline bool is_hitted_by(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    return false;
}

// Lord: доделать когда будет сделано xr_hit
inline bool is_hitted_on_bone(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    return false;
}

inline bool is_best_pistol(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    CScriptGameObject* pistol = npc->item_in_slot(1);

    if (!pistol)
    {
        Msg("[Scripts/XR_CONDITION/is_best_pistol(actor, npc)] Actor doesn't have a pistol in slot!");
        return false;
    }

    return true;
}

// Lord: доделать когда будет сделано xr_hit
inline bool is_deadly_hit(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return false;
}

// Lord: доделать когда будет сделано xr_death
inline bool is_killed_by(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return false;
}

inline bool is_alive_one(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    CSE_ALifeCreatureAbstract* server_object = nullptr;
    std::uint16_t server_npc_id = 0;
    for (const xr_string& it : buffer)
    {
        server_npc_id = Globals::get_story_object_id(it);

        if (!server_npc_id)
        {
            Msg("[Scripts/XR_CONDITION/is_alive_one(actor, npc, buffer)] WARNING: server_npc_id < 0. Can't find a "
                "valid id!");
            return false;
        }

        server_object = ai().alife().objects().object(server_npc_id)->cast_creature_abstract();

        if (!server_object)
        {
            Msg("[Scripts/XR_CONDITION/is_alive_one(actor, npc, buffer)] WARNING: server_object = nullptr. Maybe a "
                "stalker was deleted! OR Maybe BAD CAST!!!!");
            return false;
        }

        if (server_object->g_Alive() && Globals::IsStalker(server_object, 0))
            return true;
    }

    return false;
}

inline bool is_alive_one(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    CSE_ALifeCreatureAbstract* server_object = nullptr;
    std::uint16_t server_npc_id = 0;
    for (const xr_string& it : buffer)
    {
        server_npc_id = Globals::get_story_object_id(it);

        if (!server_npc_id)
        {
            Msg("[Scripts/XR_CONDITION/is_alive_one(actor, npc, buffer)] WARNING: server_npc_id < 0. Can't find a "
                "valid id!");
            return false;
        }

        server_object = ai().alife().objects().object(server_npc_id)->cast_creature_abstract();

        if (!server_object)
        {
            Msg("[Scripts/XR_CONDITION/is_alive_one(actor, npc, buffer)] WARNING: server_object = nullptr. Maybe a "
                "stalker was deleted! OR Maybe BAD CAST!!!!");
            return false;
        }

        if (server_object->g_Alive() && Globals::IsStalker(server_object, 0))
            return true;
    }

    return false;
}

inline bool is_alive_one(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    CSE_ALifeCreatureAbstract* server_object = nullptr;
    std::uint16_t server_npc_id = 0;
    for (const xr_string& it : buffer)
    {
        server_npc_id = Globals::get_story_object_id(it);

        if (!server_npc_id)
        {
            Msg("[Scripts/XR_CONDITION/is_alive_one(actor, npc, buffer)] WARNING: server_npc_id < 0. Can't find a "
                "valid id!");
            return false;
        }

        server_object = ai().alife().objects().object(server_npc_id)->cast_creature_abstract();

        if (!server_object)
        {
            Msg("[Scripts/XR_CONDITION/is_alive_one(actor, npc, buffer)] WARNING: server_object = nullptr. Maybe a "
                "stalker was deleted! OR Maybe BAD CAST!!!!");
            return false;
        }

        if (server_object->g_Alive() && Globals::IsStalker(server_object, 0))
            return true;
    }

    return false;
}

inline bool is_alive(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        Msg("[Scripts/XR_CONDITION/is_alive(actor, npc, buffer)] buffer.size() = 0!");
        return false;
    }

    CSE_ALifeCreatureAbstract* server_object = nullptr;
    if (!npc)
    {
        server_object =
            ai().alife().objects().object(Globals::get_story_object_id(buffer[0]))->cast_creature_abstract();

        if (!server_object)
        {
            Msg("[Scripts/XR_CONDITION/is_alive(actor, npc, buffer)] WARNING: server_object = nullptr. Maybe bad cast "
                "or object not found!");
            return false;
        }
    }
    else
    {
        server_object = ai().alife().objects().object(npc->ID())->cast_creature_abstract();

        if (!server_object)
        {
            Msg("[Scripts/XR_CONDITION/is_alive(actor, npc, buffer)] WARNING: server_object = nullptr. Maybe bad cast "
                "or object not found!");
            return false;
        }
    }

    if (server_object->g_Alive() && Globals::IsStalker(server_object, 0))
        return true;

    return false;
}

inline bool is_alive(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        Msg("[Scripts/XR_CONDITION/is_alive(actor, server_npc, buffer)] buffer.size() = 0!");
        return false;
    }

    CSE_ALifeCreatureAbstract* server_object = nullptr;
    if (!server_npc)
    {
        server_object =
            ai().alife().objects().object(Globals::get_story_object_id(buffer[0]))->cast_creature_abstract();

        if (!server_object)
        {
            Msg("[Scripts/XR_CONDITION/is_alive(actor, server_npc, buffer)] WARNING: server_object = nullptr. Maybe "
                "bad cast "
                "or object not found!");
            return false;
        }
    }
    else
    {
        server_object = ai().alife().objects().object(server_npc->ID)->cast_creature_abstract();

        if (!server_object)
        {
            Msg("[Scripts/XR_CONDITION/is_alive(actor, server_npc, buffer)] WARNING: server_object = nullptr. Maybe "
                "bad cast "
                "or object not found!");
            return false;
        }
    }

    if (server_object->g_Alive() && Globals::IsStalker(server_object, 0))
        return true;

    return false;
}

inline bool is_alive(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        Msg("[Scripts/XR_CONDITION/is_alive(server_actor, server_npc, buffer)] buffer.size() = 0!");
        return false;
    }

    CSE_ALifeCreatureAbstract* server_object = nullptr;
    if (!server_npc)
    {
        server_object =
            ai().alife().objects().object(Globals::get_story_object_id(buffer[0]))->cast_creature_abstract();

        if (!server_object)
        {
            Msg("[Scripts/XR_CONDITION/is_alive(server_actor, server_npc, buffer)] WARNING: server_object = nullptr. "
                "Maybe bad cast "
                "or object not found!");
            return false;
        }
    }
    else
    {
        server_object = ai().alife().objects().object(server_npc->ID)->cast_creature_abstract();

        if (!server_object)
        {
            Msg("[Scripts/XR_CONDITION/is_alive(server_actor, server_npc, buffer)] WARNING: server_object = nullptr. "
                "Maybe bad cast "
                "or object not found!");
            return false;
        }
    }

    if (server_object->g_Alive() && Globals::IsStalker(server_object, 0))
        return true;

    return false;
}

inline bool is_dead(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    CScriptGameObject* npc1 = Globals::get_story_object(buffer[0]);

    if (npc1)
    {
        if (!npc1->Alive())
            return true;
        else
            return false;
    }

    return false;
}

inline bool is_dead(CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    CScriptGameObject* npc1 = Globals::get_story_object(buffer[0]);

    if (npc1)
    {
        if (!npc1->Alive())
            return true;
        else
            return false;
    }

    return false;
}

inline bool is_dead(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    CScriptGameObject* npc1 = Globals::get_story_object(buffer[0]);

    if (npc1)
    {
        if (!npc1->Alive())
            return true;
        else
            return false;
    }

    return false;
}

inline bool is_story_object_exist(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    CScriptGameObject* npc1 = Globals::get_story_object(buffer[0]);

    return (!!npc1);
}

inline bool is_story_object_exist(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    CScriptGameObject* npc1 = Globals::get_story_object(buffer[0]);

    return (!!npc1);
}

inline bool is_story_object_exist(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "Argument list can't be empty!");
        return false;
    }

    CScriptGameObject* npc1 = Globals::get_story_object(buffer[0]);

    return (!!npc1);
}

inline bool is_actor_has_item(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        Msg("[Scripts/XR_CONDITION/is_actor_has_item(actor, npc, buffer)] WARNING: buffer.size() = 0! Returning "
            "false.");
        return false;
    }

    CScriptGameObject* story_actor = Globals::get_story_object("actor");

    if (story_actor)
        if (story_actor->GetObjectByName(buffer[0].c_str()))
            return true;

    return false;
}

inline bool is_actor_has_item(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        Msg("[Scripts/XR_CONDITION/is_actor_has_item(actor, server_npc, buffer)] WARNING: buffer.size() = 0! Returning "
            "false.");
        return false;
    }

    CScriptGameObject* story_actor = Globals::get_story_object("actor");

    if (story_actor)
        if (story_actor->GetObjectByName(buffer[0].c_str()))
            return true;

    return false;
}

inline bool is_actor_has_item(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        Msg("[Scripts/XR_CONDITION/is_actor_has_item(server_actor, server_npc, buffer)] WARNING: buffer.size() = 0! "
            "Returning "
            "false.");
        return false;
    }

    CScriptGameObject* story_actor = Globals::get_story_object("actor");

    if (story_actor)
        if (story_actor->GetObjectByName(buffer[0].c_str()))
            return true;

    return false;
}

inline bool is_npc_has_item(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        Msg("[Scripts/XR_CONDITION/is_actor_has_item(actor, npc, buffer)] buffer.size() = 0! Returning false.");
        return false;
    }

    if (npc)
        if (npc->GetObjectByName(buffer[0].c_str()))
            return true;

    return false;
}


inline bool is_signal(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        Msg("[Scripts/XR_CONDITION/is_signal(actor, npc, buffer)] WARNING: buffer.size() = 0! Returning false.");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage()[npc->ID()];

    return storage_data[storage_data.m_active_scheme].m_signals[buffer[0]];
}

/* Lord наверное не используется ибо такой перегрузки вообще нет
inline bool is_signal(CSE_ALifeDynamicObject* server_actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        Msg("[Scripts/XR_CONDITION/is_signal(server_actor, npc, buffer)] WARNING: buffer.size() = 0! Returning false.");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    DataBase::Storage_Data& storage_data = DataBase::Storage::getInstance().getStorage()[npc->ID()];

    return storage_data[storage_data.m_active_scheme].m_signals[buffer[0]];
}*/

inline bool is_counter_greater(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    std::uint8_t argument_number = atoi(buffer[1].c_str());
    std::uint8_t pstor_number = XR_LOGIC::pstor_retrieve_number(actor, buffer[0]);

    return (pstor_number > argument_number);
}

inline bool is_counter_greater(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    std::uint8_t argument_number = atoi(buffer[1].c_str());
    std::uint8_t pstor_number = XR_LOGIC::pstor_retrieve_number(actor, buffer[0]);

    return (pstor_number > argument_number);
}

inline bool is_counter_equal(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    std::uint8_t argument_number = atoi(buffer[1].c_str());
    std::uint8_t pstor_number = XR_LOGIC::pstor_retrieve_number(actor, buffer[0]);

    return (pstor_number == argument_number);
}

inline bool is_counter_equal(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    std::uint8_t argument_number = atoi(buffer[1].c_str());
    std::uint8_t pstor_number = XR_LOGIC::pstor_retrieve_number(actor, buffer[0]);

    return (pstor_number == argument_number);
}

// Lord: реализовать _kamp_talker(actor, npc) когда будет xr_kamp
inline bool is_kamp_talk(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return false;
}

inline bool check_smart_alarm_status(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    const xr_string& smart_name = buffer[0];
    Script_SmartTerrainControl_States state =
        Script_GlobalHelper::getInstance().getRegisteredSmartTerrainControlScriptStates()[buffer[1]];

    Script_SE_SmartTerrain* smart = Script_SimulationBoard::getInstance().getSmartTerrainsByName()[smart_name];
    Script_SmartTerrainControl* smart_control = smart->getBaseOnActorControl();

    if (!smart_control)
    {
        R_ASSERT2(false, "Cannot calculate 'check_smart_alarm_status' for smart %s", smart_name.c_str());
        return false;
    }

    return (smart_control->getState() == state);
}

inline bool check_smart_alarm_status(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    const xr_string& smart_name = buffer[0];
    Script_SmartTerrainControl_States state =
        Script_GlobalHelper::getInstance().getRegisteredSmartTerrainControlScriptStates()[buffer[1]];

    Script_SE_SmartTerrain* smart = Script_SimulationBoard::getInstance().getSmartTerrainsByName()[smart_name];
    Script_SmartTerrainControl* smart_control = smart->getBaseOnActorControl();

    if (!smart_control)
    {
        R_ASSERT2(false, "Cannot calculate 'check_smart_alarm_status' for smart %s", smart_name.c_str());
        return false;
    }

    return (smart_control->getState() == state);
}

inline bool check_smart_alarm_status(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    const xr_string& smart_name = buffer[0];
    Script_SmartTerrainControl_States state =
        Script_GlobalHelper::getInstance().getRegisteredSmartTerrainControlScriptStates()[buffer[1]];

    Script_SE_SmartTerrain* smart = Script_SimulationBoard::getInstance().getSmartTerrainsByName()[smart_name];
    Script_SmartTerrainControl* smart_control = smart->getBaseOnActorControl();

    if (!smart_control)
    {
        R_ASSERT2(false, "Cannot calculate 'check_smart_alarm_status' for smart %s", smart_name.c_str());
        return false;
    }

    return (smart_control->getState() == state);
}

inline bool is_has_enemy(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return (!!npc->GetBestEnemy());
}

inline bool is_has_actor_enemy(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }
    CScriptGameObject* best_enemy = npc->GetBestEnemy();

    if (best_enemy)
    {
        return (best_enemy->ID() == DataBase::Storage::getInstance().getActor()->ID());
    }

    return false;
}

inline bool is_see_enemy(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    CScriptGameObject* best_enemy = npc->GetBestEnemy();

    if (best_enemy)
    {
        return npc->CheckObjectVisibility(best_enemy);
    }

    return false;
}

inline bool is_has_enemy_in_current_loopholes_fov(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (npc->GetBestEnemy() && npc->in_smart_cover())
    {
        if (npc->in_current_loophole_fov(npc->GetBestEnemy()->Position()))
            return true;
    }

    return false;
}

inline bool is_talking(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
    }

    return actor->IsTalking();
}

inline bool is_npc_talking(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return npc->IsTalking();
}

inline bool is_see_actor(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return (npc->Alive() && npc->CheckObjectVisibility(actor));
}

// Lord: реализовать когда будет xr_death или что-то
inline bool is_actor_enemy(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return false;
}

inline bool is_actor_friend(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return npc->GetRelationType(actor) == ALife::ERelationType::eRelationTypeFriend;
}

inline bool is_actor_neutral(CScriptGameObject* actor, CScriptGameObject* npc)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!npc)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    return npc->GetRelationType(actor) == ALife::ERelationType::eRelationTypeNeutral;
}

inline bool is_factions_enemies(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    return Globals::GameRelations::is_factions_enemies(Globals::character_community(actor), buffer[0]);
}

inline bool is_factions_friends(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    return Globals::GameRelations::is_factions_friends(Globals::character_community(actor), buffer[0]);
}

inline bool is_factions_neutrals(CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!actor)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    return !(is_factions_enemies(actor, npc, buffer) || is_factions_friends(actor, npc, buffer));
}

inline bool is_faction_enemy_to_actor(
    CScriptGameObject* actor, CScriptGameObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    return Globals::GameRelations::community_goodwill(
               buffer[0].c_str(), DataBase::Storage::getInstance().getActor()->ID()) <= Globals::kRelationKoeffEnemy;
}

inline bool is_faction_enemy_to_actor(
    CSE_ALifeDynamicObject* server_actor, CSE_ALifeDynamicObject* server_npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    return Globals::GameRelations::community_goodwill(
               buffer[0].c_str(), DataBase::Storage::getInstance().getActor()->ID()) <= Globals::kRelationKoeffEnemy;
}

inline bool is_faction_enemy_to_actor(
    CScriptGameObject* actor, CSE_ALifeDynamicObject* npc, const xr_vector<xr_string>& buffer)
{
    if (!buffer.size())
    {
        R_ASSERT2(false, "argument list can't be empty!");
        return false;
    }

    return Globals::GameRelations::community_goodwill(
               buffer[0].c_str(), DataBase::Storage::getInstance().getActor()->ID()) <= Globals::kRelationKoeffEnemy;
}


} // namespace XR_CONDITION
} // namespace Scripts
} // namespace Cordis
