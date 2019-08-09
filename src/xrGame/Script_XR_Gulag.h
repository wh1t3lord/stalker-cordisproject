#pragma once

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

    // Lord: как избежать каста!!!!!
    CSE_ALifeMonsterAbstract* server_object = (CSE_ALifeMonsterAbstract*)ai().alife().objects().object(object->ID());

    if (!server_object)
    {
        Msg("[Scripts/XR_GULAG/get_npc_smart(object)] server_object is null!");
        R_ASSERT(false); // @ Lord: нужен ли здесь assertion?
        return nullptr;
    }

    if (server_object->m_smart_terrain_id != 65535)
    {
        return ai().alife().objects().object(server_object->m_smart_terrain_id);
    }

    return nullptr;
}
// Lord: тогда прочекать за удаление то есть здесь уже Singlton 
inline static xr_map<xr_string, CInifile*>& getDynamicLtx(void) noexcept
{
    static xr_map<xr_string, CInifile*> instance;
    return instance;
}

// @ "*smart_name*type_gulag"
inline CInifile loadLtx(const xr_string& name)
{
    // Lord: доделать!
    xr_string header = "*";
    header += name;
    CInifile* file = getDynamicLtx()[header];

    if (file->fname())
        return *file;
    else
    {
        xr_string _n;
    }

    // написать по нормальному!
    return CInifile("system.ltx");
}

} // namespace XR_GULAG
} // namespace Script
} // namespace Cordis
