#pragma once

#include "Script_Database.h"
#include "Script_GlobalHelper.h"

namespace Cordis
{
namespace Scripts
{
namespace XR_GULAG
{
inline CSE_ALifeDynamicObject* get_npc_smart(CScriptGameObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object was null!");
        return nullptr;
    }

    CSE_ALifeMonsterAbstract* server_object = ai().alife().objects().object(object->ID())->cast_monster_abstract();

    if (!server_object)
    {
        Msg("[Scripts/XR_GULAG/get_npc_smart(object)] server_object is null!");
        return nullptr;
    }

    if (server_object->m_smart_terrain_id != Globals::kUnsignedInt16Undefined)
    {
        return ai().alife().objects().object(server_object->m_smart_terrain_id);
    }

    return nullptr;
}
// @ Удаляется в Script_GlobalHelper!
/*
inline static xr_map<xr_string, std::unique_ptr<CScriptIniFile>>& getDynamicLtx(void)
{
    static xr_map<xr_string, std::unique_ptr<CScriptIniFile>> instance;
    return instance;
}*/

// @ "*smart_name*type_gulag"
inline CScriptIniFile* loadLtx(const xr_string& name /*, xr_string& result*/)
{
    // Lord: доделать!
    xr_string header = "*";
    header += name;
    CScriptIniFile* file = nullptr;
    if (Script_GlobalHelper::getInstance().getDynamicLtx().find(header) !=
        Script_GlobalHelper::getInstance().getDynamicLtx().end())
        file = Script_GlobalHelper::getInstance().getDynamicLtx().at(header);

    // result = header; Lord: если что аргумент result="" сделать default
    if (file)
        return file;
    else
    {
        xr_string& data = GulagGenerator::getLtx();
        if (!data.empty())
        {
            CScriptIniFile* result = Globals::create_ini_file(data.c_str());
            Script_GlobalHelper::getInstance().setDynamicLtx(header, result);
            return result;
        }
    }

    // написать по нормальному!
    return nullptr;
}

inline bool is_job_in_restrictor(
    Script_SE_SmartTerrain* smart, const xr_string& restrictor_name, const xr_string& waypoint_name)
{
    if (!smart)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    if (!restrictor_name.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return false;
    }

    if (!waypoint_name.size())
    {
        R_ASSERT2(false, "string can't be empty!");
        return false;
    }

    CScriptGameObject* restrictor_object = DataBase::Storage::getInstance().getZoneByName().at(restrictor_name);

    if (!restrictor_object)
    {
        // @ Lord: имеет ли смысл ставить сюда ассерт?
        Msg("[Scripts/XR_GULAG/is_job_in_restrictor(smart, restrictor_name, waypoint_name)] WARNING: object doesn't "
            "spawn yet! You're trying to access it!!!");
        return false;
    }

    CPatrolPathParams patrol = CPatrolPathParams(waypoint_name.c_str());
    const std::uint32_t& total_count = patrol.count();

    for (std::uint32_t i = 0; i < total_count; ++i)
    {
        if (!restrictor_object->inside(patrol.point(i)))
            return false;
    }

    return true;
}

} // namespace XR_GULAG
} // namespace Scripts
} // namespace Cordis
